// -- Cambios respecto a v00 --
// 1. nuevo metodo detener() para callar el sonido sin tocar el modo loop
// 2. enLoop ahora es el modo del boton Loop y solo lo cambia alternarLoop()
// 3. reproducirLoop() se reemplaza por alternarLoop()
// 4. reproducir() ya no cambia enLoop y siempre parte desde el inicio
// 5. nuevo metodo cambiarVolumen() que asigna el volumen y lo recalcula
// 6. sonidoActual ahora lo actualiza la clase Audio
// 7. nuevo atributo cantidadBuffers en lugar del 3 fijo del constructor
// 8. datosAudio y cargarAudio() usan muestras de 16 bits (uint16_t)

// -- Cambios respecto a v02 --
// 1. alternarLoop() se reemplaza por cambiarLoop(), que recibe el estado del boton Loop
//    y ya no reproduce el sonido cargado al activarse
// 2. nuevo atributo unaVez para que los sonidos de sistema no queden en loop

// -- Cambios respecto a v06 --
// 1. nuevos atributos factorVelocidad y fraccionAudio
// 2. nuevo metodo cambiarVelocidad() que recibe el factor de la clase Velocidad
// 3. actualizar() avanza posicionAudio segun factorVelocidad
//    los sonidos de sistema (unaVez) siempre avanzan a 1x
// 4. nuevos atributos tipoFiltro, coeficienteFiltro y estadoFiltro
// 5. nuevo metodo cambiarTono() que recibe el filtro de la clase Tono
// 6. nuevo atributo gananciaSonido: volumen base propio de cada sonido, lo asigna Audio
// 7. actualizar() aplica gananciaSonido y el filtro, y limita la muestra
//    para que no se desborde de 16 bits

#ifndef AMPLI_H
#define AMPLI_H

// incluye las herramientas basicas del sdk de raspberry pi
#include "pico/stdlib.h"
// incluye la libreria oficial de audio i2s por pio
#include "pico/audio_i2s.h"
// incluye tipos de datos enteros de tamano fijo como uint8_t y uint32_t
#include <stdint.h>
// incluye funciones matematicas para el calculo del volumen logaritmico
#include <math.h>

class Ampli {

  public:

  // --atributos--
  // a que pin se conecta el BCLK
  // Bit CLocK
  int pinReloj;

  // a que pin se conecta LRC
  // debe ir consecutivo a reloj
  int pinLRC;

  // a que pin se conecta el DIN
  // Serial Audio Data
  int pinDatos;

  // indica si en este momento se esta enviando un sonido al ampli
  bool sonando = false;

  // volumen deseado de 0 a 100
  // para cambiarlo usar cambiarVolumen() y no asignarlo directo
  int volumen = 50;
  // muestras por segundo, solo se usa en el constructor
  // los sonidos deben estar exportados a esta misma frecuencia
  int frecuencia = 11025;

  // indice del sonido cargado en el catalogo de Audio
  // cuenta desde 0, vale -1 si aun no se carga ninguno
  // lo actualiza Audio::enviarSonido()
  int sonidoActual = -1;

  // modo loop controlado por el boton Loop
  // solo lo cambia cambiarLoop(), reproducir() y detener() no lo tocan
  bool enLoop = false;
  // si es true el sonido cargado suena una sola vez aunque enLoop este activo
  // lo usa Audio para los sonidos de sistema (inicio, loop y apagado)
  bool unaVez = false;
  // almacena el puntero al arreglo de muestras de audio de 16 bits
  const uint16_t *datosAudio = nullptr;
  // guarda la cantidad total de muestras que tiene el sonido cargado
  uint32_t totalMuestras = 0;
  // lleva la cuenta de que muestra se esta enviando actualmente
  uint32_t posicionAudio = 0;
  // parte decimal de la posicion, de 0.0 a 1.0
  // se usa cuando la velocidad no es 1x y se avanza "entre" muestras
  float fraccionAudio = 0.0f;
  // factor de velocidad de reproduccion, lo entrega la clase Velocidad
  // 1.0 es normal, 0.5 la mitad y 2.0 el doble
  // para cambiarlo usar cambiarVelocidad() y no asignarlo directo
  float factorVelocidad = 1.0f;

  // filtro de tono, lo entrega la clase Tono
  // -1 pasa bajos (grave), 0 sin filtro, 1 pasa altos (brillante)
  // para cambiarlo usar cambiarTono() y no asignarlo directo
  int tipoFiltro = 0;
  // coeficiente del filtro, de 0.0 a 1.0
  float coeficienteFiltro = 1.0f;
  // memoria del filtro: guarda la ultima muestra filtrada
  // se reinicia en 0 al empezar un sonido
  float estadoFiltro = 0.0f;

  // volumen base propio del sonido cargado
  // lo asigna Audio al cargar un sonido, 1.0 deja el sonido como viene
  // se multiplica junto al volumen de la perilla
  float gananciaSonido = 1.0f;
  // factor matematico de ganancia calculado de forma logaritmica
  // se recalcula en el constructor con controlVolumen()
  float gananciaCalculada = 0.5f;

  // cantidad de cajitas de memoria para el audio
  // cada una dura unos 23 ms, con 3 hay unos 70 ms de margen
  // actualizar() debe llamarse en cada vuelta del while y sin sleep_ms
  // si igual hay cortes se puede subir a 4 o 5
  int cantidadBuffers = 3;

  // estructura que define las caracteristicas del formato de audio
  audio_format_t formatoAudio;
  // estructura que define como se empaqueta el buffer para el sistema dma
  audio_buffer_format_t formatoProductor;
  // configuracion de los pines fisicos y la maquina de estados pio
  struct audio_i2s_config configuracionI2S;
  // puntero a la cola de buffers de audio que maneja el chip
  struct audio_buffer_pool *colaBuffers = nullptr;

 // -- constructor --
 Ampli(int nuevoPinReloj, int nuevoPinDatos);

 // --metodos--
 // reproduce el sonido cargado desde el inicio
 // respeta el modo loop que este activo
 void reproducir();

 // calla el sonido y vuelve al inicio
 // no cambia el modo loop
 void detener();

 // activa o desactiva el modo loop, se usa con el boton Loop
 // recibe estadoPresionarSwitch del boton: true activa y false desactiva
 // al activar no reproduce nada, el sonido que este sonando se repite al terminar
 // al desactivar, el sonido termina su vuelta y se calla solo
 void cambiarLoop(bool nuevoLoop);

 // asigna un nuevo volumen de 0 a 100 y recalcula la ganancia
 void cambiarVolumen(int nuevoVolumen);

 // estipula como se regula el volumen
 // utiliza valores de 0 a 100
 void controlVolumen();

 // asigna el factor de velocidad calculado por la clase Velocidad
 // solo afecta a los sonidos del catalogo, los de sistema suenan a 1x
 void cambiarVelocidad(float nuevoFactor);

 // asigna el filtro calculado por la clase Tono
 // recibe tipoFiltro (-1, 0 o 1) y el coeficiente de 0.0 a 1.0
 // solo afecta a los sonidos del catalogo, los de sistema suenan sin filtro
 void cambiarTono(int nuevoTipo, float nuevoCoeficiente);

 // entrega a la clase el arreglo de audio y su cantidad de muestras
 void cargarAudio(const uint16_t *arregloMuestras, uint32_t cantidadMuestras);

 // alimenta el chip con muestras de sonido sin detener el resto del programa
 void actualizar();
};

#endif
