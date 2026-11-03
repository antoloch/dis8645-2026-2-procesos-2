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
  
  bool sonando = false; 

  int volumen = 50;
  int frecuencia = 11025;

 // variable que enlaza al listado de sonidos que se carguen eventualmente
 // cuenta desde 0
  int sonidoActual = -1;

  // guarda si la reproduccion actual debe repetirse en bucle
  bool enLoop = false;
  // almacena el puntero al arreglo de muestras de audio de 8 bits
  const uint8_t *datosAudio = nullptr;
  // guarda la cantidad total de muestras que tiene el sonido cargado
  uint32_t totalMuestras = 0;
  // lleva la cuenta de que muestra se esta enviando actualmente
  uint32_t posicionAudio = 0;
  // factor matematico de ganancia calculado de forma logaritmica
  float gananciaCalculada = 0.5f;

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
 // realiza el proceso de reproducir audio
 // reproduce sonidoActual
 void reproducir();

 // el audio debe reproducirse de manera ciclica
 //reproduce sonidoActual
 void reproducirLoop();

 // estipula como se regula el volumen
 // utiliza valores de 0 a 100
 void controlVolumen(); 

 // entrega a la clase el arreglo de audio y su cantidad de muestras
 void cargarAudio(const uint8_t *arregloMuestras, uint32_t cantidadMuestras);

 // alimenta el chip con muestras de sonido sin detener el resto del programa
 void actualizar();
};

#endif