// -- Cambios respecto a v00 --
// 1. nueva constante CANTIDAD_SONIDOS en lugar del 5 y el 4 fijos
// 2. enviarSonido() ahora tambien guarda sonidoActual en el ampli
// 3. nuevo metodo reproducirSonido() para usar al presionar el encoder

#ifndef AUDIO_H
#define AUDIO_H

// incluye la clase del amplificador para poder enviarle los datos
#include "Ampli.h"
// incluye los tipos de datos de tamano exacto
#include <stdint.h>

// cantidad de sonidos del catalogo
// si se agrega un sonido solo hay que cambiar este numero y el constructor
const int CANTIDAD_SONIDOS = 5;

// agrupa el arreglo de audio y su tamano en un solo paquete
struct DatosSonido {
  // puntero al arreglo de bytes del sonido
  const uint8_t* muestras;
  // tamano total del arreglo de audio
  uint32_t tamano;
};

class Audio {
  public:

  // arreglo que funciona como estante de sonidos
  DatosSonido catalogo[CANTIDAD_SONIDOS];

  // funcion inicial que acomoda los arreglos en el estante
  Audio();

  // carga en el ampli el sonido elegido por el encoder, sin reproducirlo
  void enviarSonido(Ampli &amplificador, int indiceEncoder);

  // se usa al presionar el encoder
  // detiene lo que suena, carga el sonido elegido y lo reproduce
  // si el modo loop esta activo el nuevo sonido tambien queda en loop
  void reproducirSonido(Ampli &amplificador, int indiceEncoder);
};

#endif
