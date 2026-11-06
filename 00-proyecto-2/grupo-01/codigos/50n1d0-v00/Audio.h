#ifndef AUDIO_H
#define AUDIO_H

// incluye la clase del amplificador para poder enviarle los datos
#include "Ampli.h"
// incluye los tipos de datos de tamano exacto
#include <stdint.h>

// agrupa el arreglo de audio y su tamano en un solo paquete
struct DatosSonido {
  // puntero al arreglo de bytes del sonido
  const uint8_t* muestras;
  // tamano total del arreglo de audio
  uint32_t tamano;
};

class Audio {
  public:
  
  // arreglo de cinco espacios que funciona como estante de sonidos
  DatosSonido catalogo[5];

  // funcion inicial que acomoda los arreglos en el estante
  Audio();

  // toma el amplificador y le inyecta el sonido elegido por el encoder
  void enviarSonido(Ampli &amplificador, int indiceEncoder);
};

#endif