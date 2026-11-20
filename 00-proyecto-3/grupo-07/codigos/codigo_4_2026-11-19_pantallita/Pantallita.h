// Pantallita.h
// Clase que se encarga de MOSTRAR lo que pasa en el proyecto 3.
// No lee botones ni decide estados: main.cpp le avisa qué mostrar.
//
// créditos:
// - biblioteca de pantalla ILI934X de Darren Horrocks
//   (https://github.com/bizzehdee/pico-libs), licencia BSD de 3 cláusulas
//   (ver LICENSE-ILI934X.txt y los encabezados de ili934x.h / ili934x.cpp)
// - fuentes FreeMono (FreeMono12pt7b.h, FreeMono18pt7b.h y FreeMono24pt7b.h)
//   que vienen con esa biblioteca, sin modificar
//
// IMPORTANTE
// - En wokwi la pantalla es una ILI9341 (240x320, la usamos en horizontal: 320x240).
// - La pantalla física propuesta es una ILI9488 de 480x320. La biblioteca adjunta
//   corresponde a la ILI9341: NO asumimos que sirve para la ILI9488.
//   Su integración queda declarada más abajo.

#ifndef PANTALLITA_H
#define PANTALLITA_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "Ili934x.h"

// qué hay dibujado en la pantalla en este momento
enum class VistaPantalla {
  APAGADA,            // reposo: pantalla en negro
  INICIO,             // "ilustración" (reemplazo temporal)
  CUENTOS,            // círculo (símbolo provisorio)
  MUSICA,             // cuadrado (símbolo provisorio)
  MENSAJE_ACTIVO,     // corazón
  SIN_MENSAJE,        // carita triste (pasajera)
  GRABANDO,           // "grabando..." con el conteo
  GRABACION_TERMINADA,// termina la simulación
  LIMITE_ALCANZADO    // se llegó a 60 segundos: hay que soltar
};

class Pantallita {
  public:

  // --- configuración (wokwi: spi0, ver diagram.json) ---
  static constexpr uint8_t PIN_MISO = 16;  // GP16 (spi0 RX, la pantalla no se lee)
  static constexpr uint8_t PIN_CS   = 17;  // GP17
  static constexpr uint8_t PIN_SCK  = 18;  // GP18 (spi0 SCK)
  static constexpr uint8_t PIN_MOSI = 19;  // GP19 (spi0 TX)
  static constexpr uint8_t PIN_DC   = 20;  // GP20
  static constexpr uint8_t PIN_RST  = 21;  // GP21

  // tamaño en horizontal (la ILI9341 es de 240x320 y se gira 90 grados)
  static constexpr uint16_t ANCHO = 320;
  static constexpr uint16_t ALTO  = 240;

  // cuánto se ve la carita triste antes de volver a lo que había
  // (no detiene el programa: se revisa en actualizar())
  static constexpr uint32_t DURACION_CARITA_MS = 3000;

  // --- lo que main puede leer ---
  VistaPantalla vista = VistaPantalla::APAGADA;

  // --- atributos internos (no modificar desde main.cpp) ---
  // la biblioteca de la pantalla (el constructor no toca el hardware)
  ILI934X lcd;
  // vista que había antes de la carita triste, para volver a ella
  VistaPantalla vistaBase = VistaPantalla::INICIO;
  uint32_t inicioCarita = 0;
  // último número del conteo que se dibujó (-1 = ninguno)
  int segundoMostrado = -1;

  // constructor: solo prepara el objeto; el hardware se inicia en iniciar()
  Pantallita();

  // inicia el SPI y la pantalla, y muestra la pantalla de inicio
  void iniciar();

  // --- lo que se muestra ---
  void mostrarInicio();               // al encender y al despertar
  void mostrarCuentos();              // círculo
  void mostrarMusica();               // cuadrado
  void mostrarMensajeActivo();        // corazón, sin texto
  void mostrarSinMensaje(uint32_t ahora); // carita triste, sin texto
  void mostrarGrabando();             // "grabando..." + conteo + instrucciones
  // dibuja solo el número si cambió el segundo (0 a 60)
  void actualizarConteo(uint32_t segundos);
  void mostrarGrabacionTerminada();   // termina la simulación (sin confirmar archivo)
  void mostrarLimiteAlcanzado();      // se llegó a 60: soltar para grabar otra vez
  void apagar();                      // reposo: pantalla en negro

  // llamar en cada vuelta del loop (no bloquea)
  // revisa si ya pasó el tiempo de la carita triste
  void actualizar(uint32_t ahora);

  private:
  // colores
  uint16_t colorFondo, colorTexto, colorCirculo, colorCuadrado;
  uint16_t colorCara, colorCorazon, colorAviso;

  void redibujar(VistaPantalla v);
  void limpiarFondo();

  // figuras (solo con la biblioteca: sin emojis ni imágenes)
  void circuloRelleno(int cx, int cy, int r, uint16_t color);
  void dibujarCirculo(int cx, int cy, int r);
  void dibujarCuadrado(int cx, int cy, int lado);
  void dibujarCorazon(int cx, int cy, int tam);
  void dibujarCaritaTriste(int cx, int cy, int r);

  // texto: las fuentes solo traen ASCII, así que á, é, ó y ú
  // se dibujan como la letra base más una tilde hecha con líneas
  int anchoLetra(GFXfont* fuente);
  int contarLetras(const char* texto);
  int dibujarLetra(int x, int yBase, const char* utf8, int* usados, uint16_t color, GFXfont* fuente);
  void dibujarTextoCentrado(const char* texto, int yBase, uint16_t color, GFXfont* fuente);
  // parte el texto en líneas que caben en la pantalla; devuelve cuántas dibujó
  int dibujarParrafo(const char* texto, int yBase, int interlineado, uint16_t color, GFXfont* fuente);
};

// ------------------------------------------------------------
// DECLARADO PARA DESPUÉS (no implementado)
// ------------------------------------------------------------
// - ilustración definitiva: hoy "ilustración" es solo un texto de reemplazo
//   para una ilustración amigable para la infancia que todavía no diseñamos
// - personajes o iconos definitivos de cuentos y música: hoy son un círculo
//   y un cuadrado provisorios
// - pantalla física ILI9488 de 480x320 por SPI: la biblioteca adjunta es de la
//   ILI9341 y no asumimos que sirva para ambas; hay que revisar el controlador,
//   el formato de color (la ILI9488 por SPI usa 18 bits por pixel), los
//   tiempos de arranque y recalcular las posiciones (ANCHO y ALTO)
// - reposo con la pantalla realmente apagada: en wokwi la luz de fondo de la
//   pantalla está conectada directo a VCC, así que "apagada" es negra.
//   En la versión física hay que poder cortar la luz de fondo (un pin o
//   transistor) y enviar a la pantalla el comando de apagado
//   void apagarLuzDeFondo();
//   void encenderLuzDeFondo();
// - sin touch: no se usa

#endif
