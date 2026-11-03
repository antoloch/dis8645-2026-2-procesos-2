// -- Cambios respecto a v00 --
// 1. nuevo metodo detener() para callar el sonido sin tocar el modo loop
// 2. enLoop ahora es el modo del boton Loop y solo lo cambia alternarLoop()
// 3. reproducirLoop() se reemplaza por alternarLoop()
// 4. reproducir() ya no cambia enLoop y siempre parte desde el inicio
// 5. nuevo metodo cambiarVolumen() que asigna el volumen y lo recalcula
// 6. sonidoActual ahora lo actualiza la clase Audio
// 7. nuevo atributo cantidadBuffers en lugar del 3 fijo del constructor

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
  // solo lo cambia alternarLoop(), reproducir() y detener() no lo tocan
  bool enLoop = false;
  // almacena el puntero al arreglo de muestras de audio de 8 bits
  const uint8_t *datosAudio = nullptr;
  // guarda la cantidad total de muestras que tiene el sonido cargado
  uint32_t totalMuestras = 0;
  // lleva la cuenta de que muestra se esta enviando actualmente
  uint32_t posicionAudio = 0;
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
 // al activar, si no hay nada sonando, parte el sonido cargado
 // al desactivar, el sonido termina su vuelta y se calla solo
 void alternarLoop();

 // asigna un nuevo volumen de 0 a 100 y recalcula la ganancia
 void cambiarVolumen(int nuevoVolumen);

 // estipula como se regula el volumen
 // utiliza valores de 0 a 100
 void controlVolumen();

 // entrega a la clase el arreglo de audio y su cantidad de muestras
 void cargarAudio(const uint8_t *arregloMuestras, uint32_t cantidadMuestras);

 // alimenta el chip con muestras de sonido sin detener el resto del programa
 void actualizar();
};

#endif
