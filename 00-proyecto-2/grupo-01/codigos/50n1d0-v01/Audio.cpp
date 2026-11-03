// -- Cambios respecto a v00 --
// 1. la validacion del indice usa CANTIDAD_SONIDOS
// 2. enviarSonido() ahora tambien guarda sonidoActual en el ampli
// 3. nuevo metodo reproducirSonido()

#include "Audio.h"

// incluye los archivos externos que contienen los arreglos de 8 bits
// debes ajustar estos nombres para que coincidan con tus archivos reales
#include "Sonido1.h"
#include "Sonido2.h"
#include "Sonido3.h"
#include "Sonido4.h"
#include "Sonido5.h"

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

// carga en el ampli el sonido elegido por el encoder, sin reproducirlo
void Audio::enviarSonido(Ampli &amplificador, int indiceEncoder) {
  // revisa si el numero del encoder esta fuera de los limites
  // los numeros validos van de cero a CANTIDAD_SONIDOS - 1
  if (indiceEncoder < 0 || indiceEncoder >= CANTIDAD_SONIDOS) {
    // si el numero es incorrecto sale sin hacer nada para evitar errores
    return;
  }

  // usa el metodo del amplificador para cargar el sonido desde el estante
  amplificador.cargarAudio(catalogo[indiceEncoder].muestras, catalogo[indiceEncoder].tamano);
  // avisa al ampli que sonido quedo cargado
  amplificador.sonidoActual = indiceEncoder;
}

// se usa al presionar el encoder
// el orden importa: detener, cargar y luego reproducir
void Audio::reproducirSonido(Ampli &amplificador, int indiceEncoder) {
  // revisa si el numero del encoder esta fuera de los limites
  if (indiceEncoder < 0 || indiceEncoder >= CANTIDAD_SONIDOS) {
    // si el numero es incorrecto sale sin cortar lo que esta sonando
    return;
  }

  // calla el sonido anterior sin cambiar el modo loop
  amplificador.detener();
  // carga el sonido elegido
  enviarSonido(amplificador, indiceEncoder);
  // reproduce el sonido nuevo
  // como enLoop no se toco, si el loop estaba activo sigue activo
  amplificador.reproducir();
}
