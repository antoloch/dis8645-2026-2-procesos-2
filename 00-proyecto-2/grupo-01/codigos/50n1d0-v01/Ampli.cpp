// -- Cambios respecto a v00 --
// 1. nuevo metodo detener()
// 2. reproducirLoop() se reemplaza por alternarLoop()
// 3. reproducir() ya no cambia enLoop y siempre parte desde el inicio
// 4. nuevo metodo cambiarVolumen()
// 5. el constructor usa cantidadBuffers en lugar del 3 fijo

#include "Ampli.h"

//constructor
Ampli::Ampli(int nuevoPinReloj, int nuevoPinDatos) {

  // guarda el pin asignado para el reloj de bits
  pinReloj = nuevoPinReloj;
  // guarda el pin asignado para la linea de datos digitales
  pinDatos = nuevoPinDatos;
  // calcula de forma automatica el pin del reloj de canal sumando uno
  pinLRC = pinReloj + 1;

   // inicia el ampli MAX98357A pero sin reproducir ningún sonido

  // define la velocidad de reproduccion en 11025 muestras por segundo
  formatoAudio.sample_freq = frecuencia;
  // define el formato de salida a 16 bits con signo que requiere el chip
  formatoAudio.format = AUDIO_BUFFER_FORMAT_PCM_S16;
  // define dos canales de salida para enviar la misma senal a izquierda y derecha
  formatoAudio.channel_count = 2;

  // enlaza el formato base de audio con el productor
  formatoProductor.format = &formatoAudio;
  // indica que cada muestra combinada ocupa cuatro bytes en memoria
  formatoProductor.sample_stride = 4;

  // asigna el pin de datos din para la maquina pio
  configuracionI2S.data_pin = pinDatos;
  // asigna el pin base del reloj bclk para la maquina pio
  configuracionI2S.clock_pin_base = pinReloj;
  // selecciona el canal cero de transferencia directa dma
  // ojo: ninguna otra parte del programa debe usar este canal
  configuracionI2S.dma_channel = 0;
  // selecciona la maquina de estados cero del pio
  // ojo: ninguna otra parte del programa debe usar esta maquina
  configuracionI2S.pio_sm = 0;

  // crea la reserva de cajitas de memoria con capacidad de 256 muestras cada una
  colaBuffers = audio_new_producer_pool(&formatoProductor, cantidadBuffers, 256);

  // configura el hardware del microcontrolador con los parametros indicados
  audio_i2s_setup(&formatoAudio, &configuracionI2S);
  // enlaza la reserva de memoria con la salida de sonido por pio
  audio_i2s_connect(colaBuffers);
  // enciende el flujo de senal digital hacia el amplificador
  audio_i2s_set_enabled(true);

  // calcula el nivel de volumen inicial para dejarlo listo
  controlVolumen();
}

// reproduce el sonido cargado desde el inicio
// no cambia enLoop, asi que respeta el modo del boton Loop
void Ampli::reproducir(){
  // verifica que existan datos de audio validos para reproducir
  if (datosAudio == nullptr || totalMuestras == 0) {
    // sale de la funcion si no hay sonido cargado
    return;
  }
  // reinicia el puntero de lectura al inicio del sonido
  // si ya estaba sonando, vuelve a empezar
  posicionAudio = 0;
  // marca el estado como reproduciendo activamente
  sonando = true;
}

// calla el sonido y deja el puntero al inicio
// no cambia enLoop
void Ampli::detener(){
  // marca que ya no se esta reproduciendo
  // actualizar() enviara silencio desde ahora
  sonando = false;
  // regresa el puntero al inicio para la proxima vez
  posicionAudio = 0;
}

// cambia el modo loop, primera pulsacion activa y segunda desactiva
void Ampli::alternarLoop(){
  // invierte el modo loop
  enLoop = !enLoop;

  // si se acaba de activar y no hay nada sonando parte el sonido cargado
  if (enLoop && !sonando) {
    // reproducir() valida que exista un sonido cargado
    reproducir();
  }
  // si se desactiva no se hace nada mas
  // actualizar() deja terminar la vuelta actual y luego se calla
}

// asigna un nuevo volumen y recalcula la ganancia de inmediato
void Ampli::cambiarVolumen(int nuevoVolumen){
  // guarda el volumen pedido
  volumen = nuevoVolumen;
  // limita el valor y calcula la nueva ganancia
  controlVolumen();
}

// Define como se controla el volumen
// se trabaja con valores de 0 a 100
void Ampli::controlVolumen(){
  // asegura que el volumen no sea menor que cero
  if (volumen < 0) {
    // fija el limite inferior en cero
    volumen = 0;
  }
  // asegura que el volumen no supere el valor de cien
  if (volumen > 100) {
    // fija el limite superior en cien
    volumen = 100;
  }

  // si el volumen es cero silencia por completo la salida
  if (volumen == 0) {
    // asigna cero a la ganancia para no emitir ruido
    gananciaCalculada = 0.0f;
  } else {
    // transforma el rango lineal de 0 a 100 a una escala logaritmica natural al oido
    gananciaCalculada = (powf(10.0f, (float)volumen / 100.0f) - 1.0f) / 9.0f;
  }
}

// entrega a la clase el arreglo de audio y su cantidad de muestras
// si se llama mientras suena, el nuevo sonido parte de inmediato
// por eso Audio::reproducirSonido() llama antes a detener()
void Ampli::cargarAudio(const uint8_t *arregloMuestras, uint32_t cantidadMuestras) {
  // guarda la direccion en memoria del arreglo que contiene las muestras
  datosAudio = arregloMuestras;
  // guarda la cantidad de muestras que componen el sonido
  totalMuestras = cantidadMuestras;
  // reinicia el indice de lectura a la primera muestra
  posicionAudio = 0;
}

// alimenta el chip con muestras de sonido sin detener el resto del programa
// debe llamarse en cada vuelta del while del main
void Ampli::actualizar() {
  // intenta obtener una cajita de memoria libre sin detener el programa si esta ocupada
  struct audio_buffer *buffer = take_audio_buffer(colaBuffers, false);

  // si no hay cajitas de memoria listas sale de inmediato para no congelar el bucle
  if (buffer == nullptr) {
    // regresa al programa principal
    return;
  }

  // apunta al espacio de memoria donde se guardaran las muestras calculadas
  int16_t *muestrasSalida = (int16_t *)buffer->buffer->bytes;
  // determina la cantidad de muestras que caben en esta cajita
  uint16_t espacioDisponible = buffer->max_sample_count;

  // recorre cada espacio disponible dentro de la cajita de memoria
  for (uint16_t i = 0; i < espacioDisponible; i++) {
    // variable temporal para guardar el valor de la muestra
    int16_t muestraProcesada = 0;

    // revisa si el sonido esta activo y hay datos por leer
    if (sonando && datosAudio != nullptr) {
      // toma la muestra de 8 bits sin signo de 0 a 255
      uint8_t muestraOriginal = datosAudio[posicionAudio];
      // centra el audio restando 128 para convertirlo a rango con signo de menos 128 a 127
      int16_t muestraCentrada = (int16_t)muestraOriginal - 128;
      // amplifica la senal a 16 bits multiplicando y aplicando la atenuacion logaritmica
      muestraProcesada = (int16_t)((float)muestraCentrada * 256.0f * gananciaCalculada);

      // avanza a la siguiente muestra del arreglo
      posicionAudio++;

      // revisa si se llego al final de todas las muestras del sonido
      if (posicionAudio >= totalMuestras) {
        // si el modo loop esta activo vuelve al comienzo del sonido
        if (enLoop) {
          // regresa el puntero al inicio del audio
          posicionAudio = 0;
        } else {
          // detiene la reproduccion al finalizar
          detener();
        }
      }
    } else {
      // si no esta sonando envia silencio absoluto
      muestraProcesada = 0;
    }

    // envia la muestra calculada al canal de sonido izquierdo
    muestrasSalida[i * 2] = muestraProcesada;
    // envia la misma muestra al canal derecho para tener sonido en ambos lados
    muestrasSalida[i * 2 + 1] = muestraProcesada;
  }

  // indica cuantas muestras fueron escritas en la cajita de memoria
  buffer->sample_count = espacioDisponible;
  // entrega la cajita llena al sistema dma para que la envie al chip de sonido
  give_audio_buffer(colaBuffers, buffer);
}
