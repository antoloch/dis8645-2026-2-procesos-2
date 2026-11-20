// main.cpp — Proyecto 3
// Botones de cuentos, música, mensaje de la persona cuidadora y grabar,
// con LED de grabación y mensajes por el monitor serial.
//
// créditos:
// - BotonGrupo1: grupo 1 del proyecto 2 (Camila Ramírez, Bianka Vilchez y Dayana Pañitrur)
// - Lucecita: material docente de la sesión 08a
//
// main solo coordina las categorías y los estados;
// la lectura de botones la hace BotonGrupo1 y el LED lo maneja Lucecita.

#include <stdio.h>
#include "pico/stdlib.h"
#include "BotonGrupo1.h"
#include "Lucecita.h"

// ------------------------------------------------------------
// DECLARACIONES QUE TODAVÍA NO IMPLEMENTAMOS (solo comentarios)
// ------------------------------------------------------------
// pantalla: mostrar categoría, título del audio y estado
//   void mostrarEnPantalla(/* categoría, audio, estado */);
// tonos: avisos sonoros cortos
//   void reproducirTono(/* frecuencia, duración */);
// audio real: reproducir desde la SD hacia el amplificador y el parlante
//   void reproducirAudio(/* archivo */);
//   void detenerAudio();
// fin del audio: detectar cuándo el audio termina de verdad
//   bool audioTermino();
//   (cuando exista, el mensaje de la persona cuidadora deja de estar
//    activo al terminar y se puede volver a escuchar)
// micrófono: capturar la voz con el MAX4466
//   void capturarMicrofono();
// tarjeta SD: revisar, leer y guardar archivos
//   bool sdDisponible();
//   bool mensajeGuardadoEnSD();

// ------------------------------------------------------------
// PINES Y CONSTANTES (según el wokwi)
// ------------------------------------------------------------
const int PIN_CUENTOS = 2;   // botón cuentos (GP2)
const int PIN_MUSICA  = 3;   // botón música (GP3)
const int PIN_MENSAJE = 4;   // botón mensaje de la persona cuidadora (GP4)
const int PIN_GRABAR  = 5;   // botón grabar (GP5)
const int PIN_LED     = 6;   // led de grabación (GP6)
// GP0 y GP1 los usa el monitor serial (UART0)

const uint32_t ESPERA_CATEGORIAS_MS = 3000;   // espera entre pulsaciones aceptadas
const uint32_t ESPERA_GRABAR_MS     = 0;      // grabar solo usa el filtro de rebote
const uint32_t MAX_GRABACION_MS     = 60000;  // máximo de un minuto

const int TOTAL_CUENTOS = 3;
const int TOTAL_MUSICA  = 4;

// ------------------------------------------------------------
// ESTADO DEL PROGRAMA
// ------------------------------------------------------------
// solo hay una categoría activa a la vez
enum class Categoria { NINGUNA, CUENTOS, MUSICA, MENSAJE };

Categoria categoria = Categoria::NINGUNA;

// cada categoría recuerda su posición (0 = primero)
int posCuento = 0;
int posMusica = 0;

// grabación simulada
bool grabando = false;
uint32_t inicioGrabacion = 0;

// solo está en memoria RAM: se pierde al reiniciar la placa
// (la versión real lo guardaría en la SD)
bool mensajeDisponibleSimulado = false;

// ------------------------------------------------------------
// FRASES PARA LOS MENSAJES
// ------------------------------------------------------------
// modo 0 = origen ("del cuento 2", "de música 1")
// modo 1 = destino ("al cuento 2", "a música 1")
// modo 2 = nombre  ("el cuento 2", "la música 1")
void frase(char* destino, size_t n, Categoria c, int modo) {
  switch (c) {
    case Categoria::CUENTOS:
      snprintf(destino, n, "%s cuento %d",
               modo == 0 ? "del" : (modo == 1 ? "al" : "el"), posCuento + 1);
      break;
    case Categoria::MUSICA:
      if (modo == 2) snprintf(destino, n, "la música %d", posMusica + 1);
      else snprintf(destino, n, "%s música %d", modo == 0 ? "de" : "a", posMusica + 1);
      break;
    case Categoria::MENSAJE:
      snprintf(destino, n, "%s mensaje de la persona cuidadora",
               modo == 0 ? "del" : (modo == 1 ? "al" : "el"));
      break;
    default:
      snprintf(destino, n, "nada");
      break;
  }
}

// ------------------------------------------------------------
// CATEGORÍAS: cuentos y música
// ------------------------------------------------------------
// se llama cuando una pulsación aceptada pide cuentos o música
void cambiarA(Categoria nueva) {
  char de[64];
  char a[64];

  if (categoria == Categoria::NINGUNA) {
    // no había nada activo: empieza desde la posición recordada
    categoria = nueva;
    if (nueva == Categoria::CUENTOS) {
      printf("aquí empieza el audio del cuento %d\n", posCuento + 1);
    } else {
      printf("aquí empieza el audio de música %d\n", posMusica + 1);
    }
  } else if (categoria == nueva) {
    // misma categoría: avanzamos al siguiente y volvemos al primero después del último
    frase(de, sizeof(de), categoria, 0);
    if (nueva == Categoria::CUENTOS) posCuento = (posCuento + 1) % TOTAL_CUENTOS;
    else posMusica = (posMusica + 1) % TOTAL_MUSICA;
    frase(a, sizeof(a), categoria, 1);
    printf("se cambia %s %s\n", de, a);
  } else {
    // otra categoría: se muestra de dónde sale y a dónde cambia
    // (la posición de la categoría nueva es la que recordaba)
    frase(de, sizeof(de), categoria, 0);
    categoria = nueva;   // si era el mensaje, deja de estar activo
    frase(a, sizeof(a), categoria, 1);
    printf("se cambia %s %s\n", de, a);
  }
}

// ------------------------------------------------------------
// MENSAJE DE LA PERSONA CUIDADORA
// ------------------------------------------------------------
void pulsarMensaje() {
  if (!mensajeDisponibleSimulado) {
    // no cambia la categoría activa
    printf("todavía no hay un mensaje de la persona cuidadora\n");
    return;
  }

  char de[64];
  if (categoria != Categoria::NINGUNA) {
    frase(de, sizeof(de), categoria, 0);
    printf("se cambia %s al mensaje de la persona cuidadora\n", de);
  }
  categoria = Categoria::MENSAJE;
  printf("ahora empieza el mensaje de la persona cuidadora\n");

  // OJO: el mensaje queda marcado como activo mientras no se cambie de categoría.
  // Falta implementar la detección del final real del audio
  // (ver audioTermino() arriba) para poder escucharlo de nuevo después de que termine.
}

// ------------------------------------------------------------
// GRABACIÓN (SIMULADA)
// ------------------------------------------------------------
void iniciarGrabacion(Lucecita& led) {
  char nombre[64];

  // si había una categoría activa, se detiene para grabar
  if (categoria != Categoria::NINGUNA) {
    frase(nombre, sizeof(nombre), categoria, 2);
    printf("se detiene %s para grabar\n", nombre);
    categoria = Categoria::NINGUNA;
  }

  grabando = true;
  inicioGrabacion = to_ms_since_boot(get_absolute_time());
  led.prender();
  printf("aquí empieza la grabación del mensaje de la persona cuidadora — simulación\n");
}

void terminarGrabacion(Lucecita& led, bool porMinuto) {
  grabando = false;
  led.apagar();

  if (porMinuto) {
    printf("se llegó a un minuto: hay que soltar el botón y volver a presionar para grabar otra vez\n");
  }
  printf("aquí termina la grabación y se guarda el mensaje — simulación\n");

  // la grabación es simulada: no hay micrófono ni archivo.
  // esta variable solo vive en memoria y se pierde al reiniciar la placa.
  mensajeDisponibleSimulado = true;

  // ----------------------------------------------------------
  // GRABACIÓN REAL (declarado, no implementado)
  // ----------------------------------------------------------
  // - La grabación real usa el micrófono MAX4466 y se guarda en la tarjeta SD.
  // - El mensaje anterior solo se reemplaza cuando el nuevo se guarda bien;
  //   si falla la grabación o el guardado, se conserva el anterior.
  // - El mensaje no se borra después de un día ni por tiempo.
  // - La versión física debe detectar si el módulo está unido,
  //   permitir grabar solo cuando está conectado, y cancelar la grabación
  //   si se separa mientras graba, sin reemplazar el mensaje anterior.
  //   bool moduloUnido();
  //   bool guardarGrabacionEnSD();
  //   void cancelarGrabacion();
}

// ------------------------------------------------------------
// PROGRAMA PRINCIPAL
// ------------------------------------------------------------
int main() {
  // monitor serial por UART0 (GP0/GP1)
  stdio_init_all();

  // botones: categorías con 3 segundos de espera, grabar sin espera extra
  BotonGrupo1 botonCuentos(PIN_CUENTOS, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonMusica(PIN_MUSICA, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonMensaje(PIN_MENSAJE, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonGrabar(PIN_GRABAR, ESPERA_GRABAR_MS);

  // led de grabación
  Lucecita ledGrabacion(PIN_LED);

  // se imprime una sola vez al iniciar
  printf("esperando una acción\n");

  while (true) {
    // 1) grabar se lee primero: si coincide con una solicitud de reproducción, gana grabar
    botonGrabar.leer();

    if (!grabando) {
      // solo un flanco aceptado empieza la grabación (mantener apretado no la reinicia)
      if (botonGrabar.pulsacionAceptada) {
        iniciarGrabacion(ledGrabacion);
      }
    } else {
      uint32_t ahora = to_ms_since_boot(get_absolute_time());
      if (!botonGrabar.presionado) {
        // al soltar, termina
        terminarGrabacion(ledGrabacion, false);
      } else if ((ahora - inicioGrabacion) >= MAX_GRABACION_MS) {
        // llegó al minuto: termina aunque sigan apretando
        // no empieza otra hasta soltar y volver a presionar
        terminarGrabacion(ledGrabacion, true);
      }
    }

    // 2) mientras grabamos, las categorías no aceptan pulsaciones
    // (no se guardan para después ni reinician su espera)
    // el botón de mensaje tampoco acepta mientras el mensaje está activo,
    // así no se reinicia la espera de 3 segundos
    botonCuentos.aceptarPulsaciones = !grabando;
    botonMusica.aceptarPulsaciones  = !grabando;
    botonMensaje.aceptarPulsaciones = !grabando && (categoria != Categoria::MENSAJE);

    // 3) leer siempre los tres botones para mantener su estado actualizado
    botonCuentos.leer();
    botonMusica.leer();
    botonMensaje.leer();

    // 4) atender las pulsaciones aceptadas (solo si no estamos grabando)
    if (!grabando) {
      if (botonCuentos.pulsacionAceptada) cambiarA(Categoria::CUENTOS);
      if (botonMusica.pulsacionAceptada)  cambiarA(Categoria::MUSICA);
      if (botonMensaje.pulsacionAceptada) pulsarMensaje();
    }

    // sin sleep ni esperas: el ciclo sigue leyendo todos los botones
  }

  return 0;
}
