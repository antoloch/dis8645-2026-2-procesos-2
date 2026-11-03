// incluye funciones para imprimir mensajes por el puerto serial
#include <stdio.h>
// incluye las herramientas basicas del sdk de raspberry pi
#include "pico/stdlib.h"
// incluye la clase de las luces led
#include "Lucecita.h"
// incluye la clase de los botones (necesita resistencia pull-down externa)
#include "BotonGrupo1.h"
// incluye la clase del amplificador MAX98357A
#include "Ampli.h"
// incluye el catalogo de sonidos y CANTIDAD_SONIDOS
#include "Audio.h"
// incluye la clase de las perillas, sirve para el potenciometro y el encoder
#include "PerillaGrupo7.h"

int main() {
  // inicia el puerto serial para poder imprimir mensajes de prueba
  stdio_init_all();

  // -- definicion de las piezas --

  // definimos 5 leds en la clase y sus respectivos pines
  // led[0] corresponde al sonido 0, led[1] al sonido 1 y asi hasta led[4]
  Lucecita led[] = { Lucecita(2), Lucecita(3), Lucecita(4), Lucecita(5), Lucecita(6) };

  // definimos el boton selector del encoder (patita SW)
  // al presionarlo se reproduce el sonido elegido
  BotonGrupo1 botonSelector(12);
  // definimos el boton de loop
  // primera pulsacion activa el loop y la segunda lo desactiva
  BotonGrupo1 botonLoop(13);

  // definimos el amplificador
  // BCLK en el 18, LRC queda solo en el 19 y DIN en el 20
  Ampli amplificador(18, 20);

  // definimos el sistema de audio
  // el constructor acomoda los 5 sonidos en el catalogo
  Audio audio;

  // definimos las perillas de volumen y selector de sonido
  // perillaVolumen es un potenciometro en el GPIO 26 que entrega valores de 0 a 100
  // antes era perillaVolumen(26), pero el constructor del potenciometro
  // pide dos datos: el pin y el limite del rango, por eso se agrega el 100
  PerillaGrupo7 perillaVolumen(26, 100);
  // perillaSelector es un encoder con CLK en el 14 y DT en el 15
  // recorre las opciones de 0 a CANTIDAD_SONIDOS - 1 de forma circular
  // antes era perillaSelector(), pero con los parentesis vacios C++ no crea una perilla:
  // lo entiende como una funcion llamada perillaSelector, y perillaSelector.leer() da error
  // ademas la clase no tiene un constructor sin datos, el encoder necesita
  // los dos pines (CLK y DT) y la cantidad de opciones para saber cuando dar la vuelta
  // se usa CANTIDAD_SONIDOS para que el encoder tenga las mismas opciones que el catalogo
  PerillaGrupo7 perillaSelector(14, 15, CANTIDAD_SONIDOS);

  // -- estado inicial --

  // guarda que sonido esta elegido con el encoder, parte en el sonido 0
  int sonidoSeleccionado = 0;

  // se setea el inicio del sistema en led y sonido 0
  // prende el led del sonido 0
  led[sonidoSeleccionado].prender();
  // deja cargado el sonido 0 en el ampli sin reproducirlo
  // asi el boton Loop tiene un sonido para repetir desde el comienzo
  audio.enviarSonido(amplificador, sonidoSeleccionado);

  // lectura inicial de volumen
  perillaVolumen.leer();
  // entrega al ampli el valor de 0 a 100 de la perilla
  amplificador.cambiarVolumen(perillaVolumen.posicion);

  // el while no usa sleep_ms para que el audio y el encoder no se corten
  while (true) {

    // -- audio --

    // alimenta el ampli con nuevas muestras en cada vuelta
    // si se deja de llamar seguido el sonido se corta
    amplificador.actualizar();

    // -- lectura de botones y perillas --

    // leemos el estado de los botones y perillas
    perillaVolumen.leer();
    perillaSelector.leer();
    botonSelector.leer();
    botonLoop.leer();

    // -- volumen --

    // actualizamos el volumen del ampli con la perilla
    // el valor es entregado por perillaVolumen.leer() y va de 0 a 100
    // solo se cambia cuando la perilla se movio, para no recalcular en cada vuelta
    if (perillaVolumen.cambio) {
      amplificador.cambiarVolumen(perillaVolumen.posicion);
    }

    // -- seleccion de sonido con el encoder --

    // se actualiza el led y el sonido segun la perilla de selector
    // cambio es true solo en la vuelta en que el encoder avanzo un clic
    if (perillaSelector.cambio) {
      // apaga el led del sonido que estaba elegido
      led[sonidoSeleccionado].apagar();
      // guarda el nuevo sonido elegido
      // la perilla ya da la vuelta sola: despues del 4 viene el 0 y antes del 0 el 4
      sonidoSeleccionado = perillaSelector.posicion;
      // prende el led del nuevo sonido
      led[sonidoSeleccionado].prender();

      // si no hay nada sonando deja cargado el nuevo sonido
      // asi el boton Loop repite el sonido que se ve en los leds
      // si algo esta sonando no se carga, para no cortarlo a la mitad
      if (!amplificador.sonando) {
        audio.enviarSonido(amplificador, sonidoSeleccionado);
      }
    }

    // -- reproducir --

    // al presionar el encoder se reproduce el sonido elegido
    // recienPresionado es true una sola vez por pulsacion aunque se mantenga apretado
    if (botonSelector.recienPresionado) {
      // detiene lo que suena, carga el sonido elegido y lo reproduce desde el inicio
      // si el loop esta activo el nuevo sonido tambien queda en loop
      audio.reproducirSonido(amplificador, sonidoSeleccionado);
    }

    // -- loop --

    // al presionar el boton Loop se activa o desactiva la repeticion
    if (botonLoop.recienPresionado) {
      // al activar, si no hay nada sonando parte el sonido cargado
      // al desactivar, el sonido termina su vuelta y se calla solo
      amplificador.alternarLoop();
    }

    // -- led del sonido elegido --

    // con el loop activo el led del sonido elegido parpadea
    if (amplificador.enLoop) {
      // parpadeo() revisa el tiempo y cambia la luz sin detener el programa
      led[sonidoSeleccionado].parpadeo();
    } else if (!led[sonidoSeleccionado].prendida) {
      // si el loop se desactivo justo con la luz apagada, la vuelve a prender
      led[sonidoSeleccionado].prender();
    }
  }
}
