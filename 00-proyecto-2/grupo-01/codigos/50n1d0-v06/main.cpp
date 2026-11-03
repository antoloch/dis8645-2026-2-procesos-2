// -- Cambios respecto a v02 --
// 1. al encender suena sonidoInicio
// 2. al activar el loop suena sonidoLoop, solo si no hay nada sonando
// 3. mientras suena un sonido de sistema no se pueden usar botones ni encoder
// 4. el encoder solo cambia led y sonido si no hay nada sonando
// 5. el boton Loop funciona con presionarSwitch() y estadoPresionarSwitch
// 6. si se presionan Loop y selector a la vez, primero se revisa Loop
// 7. presionar el selector mientras suena respeta audio.margenSonido
// 8. el sonido elegido se carga recien al presionar el selector

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
// incluye el catalogo de sonidos, los sonidos de sistema y CANTIDAD_SONIDOS
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
  // funciona como switch: primera pulsacion activa el loop y la segunda lo desactiva
  BotonGrupo1 botonLoop(13);

  // definimos el amplificador
  // BCLK en el 18, LRC queda solo en el 19 y DIN en el 20
  Ampli amplificador(18, 20);

  // definimos el sistema de audio
  // el constructor acomoda los 5 sonidos en el catalogo y los sonidos de sistema
  Audio audio;

  // definimos las perillas de volumen y selector de sonido
  // perillaVolumen es un potenciometro en el GPIO 26 que entrega valores de 0 a 100
  PerillaGrupo7 perillaVolumen(26, 100);
  // perillaSelector es un encoder con CLK en el 14 y DT en el 15
  // recorre las opciones de 0 a CANTIDAD_SONIDOS - 1 de forma circular
  PerillaGrupo7 perillaSelector(14, 15, CANTIDAD_SONIDOS);

  // -- estado inicial --

  // guarda que sonido esta elegido con el encoder, parte en el sonido 0
  int sonidoSeleccionado = 0;

  // se setea el inicio del sistema en led y sonido 0
  // prende el led del sonido 0
  led[sonidoSeleccionado].prender();

  // lectura inicial de volumen
  // va antes de sonidoInicio para que suene con el volumen de la perilla
  perillaVolumen.leer();
  // entrega al ampli el valor de 0 a 100 de la perilla
  amplificador.cambiarVolumen(perillaVolumen.posicion);

  // suena sonidoInicio una sola vez
  // mientras suena no se pueden usar los botones ni el encoder
  audio.reproducirInicio(amplificador);

  // el while no usa sleep_ms para que el audio y el encoder no se corten
  while (true) {

    // -- audio --

    // alimenta el ampli con nuevas muestras en cada vuelta
    // si se deja de llamar seguido el sonido se corta
    amplificador.actualizar();

    // -- lectura de botones y perillas --

    // leemos el estado de los botones y perillas
    // se leen siempre, aunque esten bloqueados, para que el filtro de rebote
    // y el encoder no se pierdan pasos
    perillaVolumen.leer();
    perillaSelector.leer();
    botonSelector.leer();
    botonLoop.leer();

    // -- volumen --

    // el volumen se puede cambiar siempre, incluso mientras suena
    // el valor es entregado por perillaVolumen.leer() y va de 0 a 100
    // solo se cambia cuando la perilla se movio, para no recalcular en cada vuelta
    if (perillaVolumen.cambio) {
      amplificador.cambiarVolumen(perillaVolumen.posicion);
    }

    // -- bloqueo por sonido de sistema --

    // mientras suena sonidoInicio, sonidoLoop o sonidoApagado no se puede hacer nada
    if (audio.sistemaSonando(amplificador)) {
      // si se giro el encoder, la perilla vuelve al sonido elegido
      // asi el giro se ignora y no queda desfasada con los leds
      perillaSelector.posicion = sonidoSeleccionado;
      // continue salta al inicio del while sin revisar lo que sigue
      continue;
    }

    // -- loop --

    // va antes del selector: si se presionan los dos a la vez, primero se revisa Loop
    if (botonLoop.recienPresionado) {
      // cambia estadoPresionarSwitch: false pasa a true y true pasa a false
      botonLoop.presionarSwitch();

      // si se acaba de activar y no hay nada sonando, suena sonidoLoop
      // si algo suena no se corta: al terminar se repetira en loop
      // si tambien se presiono el selector, se salta sonidoLoop
      // para que el sonido elegido parta de inmediato en loop
      if (botonLoop.estadoPresionarSwitch && !amplificador.sonando && !botonSelector.recienPresionado) {
        audio.reproducirLoop(amplificador);
      }

      // entrega al ampli el estado del switch
      // sonidoLoop no se repite aunque el loop quede activo, porque es de sistema
      // al desactivar, el sonido termina su vuelta y se calla solo
      amplificador.cambiarLoop(botonLoop.estadoPresionarSwitch);
    }

    // -- reproducir --

    // al presionar el encoder se reproduce el sonido elegido
    // recienPresionado es true una sola vez por pulsacion aunque se mantenga apretado
    if (botonSelector.recienPresionado) {
      // detiene lo que suena, carga el sonido elegido y lo reproduce desde el inicio
      // si el loop esta activo el nuevo sonido tambien queda en loop
      // si algo suena y no ha pasado audio.margenSonido, no hace nada
      audio.reproducirSonido(amplificador, sonidoSeleccionado);
    }

    // -- seleccion de sonido con el encoder --

    // cambio es true solo en la vuelta en que el encoder avanzo un clic
    if (perillaSelector.cambio) {
      if (amplificador.sonando) {
        // si algo esta sonando no cambia ni el led ni el sonido
        // la perilla vuelve al sonido elegido para ignorar el giro
        perillaSelector.posicion = sonidoSeleccionado;
      } else {
        // apaga el led del sonido que estaba elegido
        led[sonidoSeleccionado].apagar();
        // guarda el nuevo sonido elegido
        // la perilla ya da la vuelta sola: despues del 4 viene el 0 y antes del 0 el 4
        sonidoSeleccionado = perillaSelector.posicion;
        // prende el led del nuevo sonido
        led[sonidoSeleccionado].prender();
      }
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
