// -- Cambios respecto a v00 --
// 1. la validacion del indice usa CANTIDAD_SONIDOS
// 2. enviarSonido() ahora tambien guarda sonidoActual en el ampli
// 3. nuevo metodo reproducirSonido()
// 4. usa los arreglos sonido0..sonido4 de 16 bits y tamano cuenta muestras

// -- Cambios respecto a v02 --
// 1. el constructor tambien acomoda sonidoInicio, sonidoLoop y sonidoApagado
// 2. enviarSonido() deja unaVez en false, para que el loop funcione
// 3. reproducirSonido() respeta margenSonido y devuelve si reprodujo
// 4. nuevos metodos de sistema y sistemaSonando()

#include "Audio.h"

// incluye los archivos externos que contienen los arreglos de 16 bits
#include "Sonido0.h"
#include "Sonido1.h"
#include "Sonido2.h"
#include "Sonido3.h"
#include "Sonido4.h"
// incluye el sonido de sistema, hecho con script/generarSonidoSistema.py
#include "SonidoSistema.h"

// funcion inicial que se ejecuta al crear la clase
Audio::Audio() {
  // guarda el puntero del primer sonido en el espacio cero
  catalogo[0].muestras = sonido0;
  // calcula y guarda el tamano exacto del primer sonido automaticamente
  catalogo[0].tamano = sizeof(sonido0) / sizeof(sonido0[0]);

  // guarda el puntero del segundo sonido en el espacio uno
  catalogo[1].muestras = sonido1;
  // calcula y guarda el tamano del segundo sonido automaticamente
  catalogo[1].tamano = sizeof(sonido1) / sizeof(sonido1[0]);

  // guarda el puntero del tercer sonido en el espacio dos
  catalogo[2].muestras = sonido2;
  // calcula y guarda el tamano del tercer sonido automaticamente
  catalogo[2].tamano = sizeof(sonido2) / sizeof(sonido2[0]);

  // guarda el puntero del cuarto sonido en el espacio tres
  catalogo[3].muestras = sonido3;
  // calcula y guarda el tamano del cuarto sonido automaticamente
  catalogo[3].tamano = sizeof(sonido3) / sizeof(sonido3[0]);

  // guarda el puntero del quinto sonido en el espacio cuatro
  catalogo[4].muestras = sonido4;
  // calcula y guarda el tamano del quinto sonido automaticamente
  catalogo[4].tamano = sizeof(sonido4) / sizeof(sonido4[0]);

  // los tres sonidos de sistema usan el mismo arreglo
  // para cambiar uno basta con apuntarlo a otro arreglo
  sonidoInicio.muestras = sonidoSistema;
  sonidoInicio.tamano = sizeof(sonidoSistema) / sizeof(sonidoSistema[0]);
  sonidoLoop = sonidoInicio;
  sonidoApagado = sonidoInicio;
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
  // los sonidos del catalogo si pueden quedar en loop
  amplificador.unaVez = false;
}

// se usa al presionar el encoder
// el orden importa: detener, cargar y luego reproducir
bool Audio::reproducirSonido(Ampli &amplificador, int indiceEncoder) {
  // revisa si el numero del encoder esta fuera de los limites
  if (indiceEncoder < 0 || indiceEncoder >= CANTIDAD_SONIDOS) {
    // si el numero es incorrecto sale sin cortar lo que esta sonando
    return false;
  }

  // leemos el tiempo transcurrido desde que se encendio el microcontrolador
  uint32_t tiempoActual = to_ms_since_boot(get_absolute_time());

  // si algo esta sonando y no ha pasado margenSonido, no se reinicia
  // asi presionar muchas veces seguidas no corta el sonido una y otra vez
  if (amplificador.sonando && (tiempoActual - ultimoSonido < margenSonido)) {
    return false;
  }

  // calla el sonido anterior sin cambiar el modo loop
  amplificador.detener();
  // carga el sonido elegido
  enviarSonido(amplificador, indiceEncoder);
  // reproduce el sonido nuevo
  // como enLoop no se toco, si el loop estaba activo sigue activo
  amplificador.reproducir();
  // guarda el momento para medir el margen la proxima vez
  ultimoSonido = tiempoActual;
  return true;
}

// suena al encender la raspi
void Audio::reproducirInicio(Ampli &amplificador) {
  reproducirSistema(amplificador, sonidoInicio);
}

// suena al activar el modo loop
void Audio::reproducirLoop(Ampli &amplificador) {
  reproducirSistema(amplificador, sonidoLoop);
}

// queda listo por si se agrega una forma de apagar la maquina
void Audio::reproducirApagado(Ampli &amplificador) {
  reproducirSistema(amplificador, sonidoApagado);
}

// devuelve true mientras suena un sonido de sistema
// unaVez solo es true cuando el sonido cargado es de sistema
bool Audio::sistemaSonando(Ampli &amplificador) {
  return amplificador.sonando && amplificador.unaVez;
}

// detiene lo que suena, carga el sonido de sistema y lo reproduce una sola vez
void Audio::reproducirSistema(Ampli &amplificador, DatosSonido &sonido) {
  // calla el sonido anterior, nunca suenan dos a la vez
  amplificador.detener();
  // carga el sonido de sistema
  amplificador.cargarAudio(sonido.muestras, sonido.tamano);
  // -1 indica que el sonido cargado no es del catalogo
  amplificador.sonidoActual = -1;
  // aunque el loop este activo, este sonido no se repite
  amplificador.unaVez = true;
  // reproduce el sonido de sistema
  amplificador.reproducir();
}
