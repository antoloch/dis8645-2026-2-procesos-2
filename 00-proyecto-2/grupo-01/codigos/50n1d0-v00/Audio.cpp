#include "Audio.h"

// incluye los archivos externos que contienen los arreglos de 8 bits
// debes ajustar estos nombres para que coincidan con tus archivos reales
#include "sonido_1.h"
#include "sonido_2.h"
#include "sonido_3.h"
#include "sonido_4.h"
#include "sonido_5.h"

// funcion inicial que se ejecuta al crear la clase
Audio::Audio() {
  // guarda el puntero del primer sonido en el espacio cero
  catalogo[0].muestras = arreglo_sonido_1;
  // calcula y guarda el tamano exacto del primer sonido automaticamente
  catalogo[0].tamano = sizeof(arreglo_sonido_1);
  
  // guarda el puntero del segundo sonido en el espacio uno
  catalogo[1].muestras = arreglo_sonido_2;
  // calcula y guarda el tamano del segundo sonido automaticamente
  catalogo[1].tamano = sizeof(arreglo_sonido_2);
  
  // guarda el puntero del tercer sonido en el espacio dos
  catalogo[2].muestras = arreglo_sonido_3;
  // calcula y guarda el tamano del tercer sonido automaticamente
  catalogo[2].tamano = sizeof(arreglo_sonido_3);
  
  // guarda el puntero del cuarto sonido en el espacio tres
  catalogo[3].muestras = arreglo_sonido_4;
  // calcula y guarda el tamano del cuarto sonido automaticamente
  catalogo[3].tamano = sizeof(arreglo_sonido_4);
  
  // guarda el puntero del quinto sonido en el espacio cuatro
  catalogo[4].muestras = arreglo_sonido_5;
  // calcula y guarda el tamano del quinto sonido automaticamente
  catalogo[4].tamano = sizeof(arreglo_sonido_5);
}

// toma el amplificador y le inyecta el sonido elegido por el encoder
void Audio::enviarSonido(Ampli &amplificador, int indiceEncoder) {
  // revisa si el numero del encoder esta fuera de los limites
  // como son cinco sonidos los numeros validos son de cero a cuatro
  if (indiceEncoder < 0 || indiceEncoder > 4) {
    // si el numero es incorrecto sale sin hacer nada para evitar errores
    return;
  }

  // usa el metodo del amplificador para cargar el sonido desde el estante
  amplificador.cargarAudio(catalogo[indiceEncoder].muestras, catalogo[indiceEncoder].tamano);
}