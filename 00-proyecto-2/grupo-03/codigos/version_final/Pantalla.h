#ifndef PANTALLA_H
#define PANTALLA_H

// Pantalla.h
// pantalla OLED monocromatica de 128 x 64 pixeles por I2C.
// Funciona con la SH1106 (la que van a usar) y tambien con la SSD1306
// (la unica que simula Wokwi). Ver PANTALLA_SH1106 mas abajo.
//
// COMO FUNCIONA:
//  - la clase tiene un "dibujo" en memoria (buffer). Todo lo que se dibuja
//    (pixeles, texto, rostros) se pinta primero ahi.
//  - mostrar() manda el buffer completo a la pantalla por I2C.
//  - el buffer esta organizado en 8 "paginas" de 8 filas: cada byte son 8 pixeles
//    en vertical (el bit 0 es el de arriba). El pixel (x, y) es el bit (y % 8)
//    del byte [x + (y / 8) * 128].

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

// ---- configuracion -------------------------------------------------------

// 1 = pantalla SH1106 real
// 0 = pantalla SSD1306 (para probar en Wokwi, que no tiene la SH1106)
#define PANTALLA_SH1106        1

// conexion I2C: bus i2c0 en GP4 (SDA) y GP5 (SCL)
// (no chocan con los pines de botones, perillas y LED)
#define PANTALLA_I2C           i2c0
#define PANTALLA_PIN_SDA       4
#define PANTALLA_PIN_SCL       5
#define PANTALLA_DIRECCION     0x3C   // direccion I2C habitual (a veces 0x3D)

// ---- tamano ---------------------------------------------------------------

#ifndef PANTALLA_ANCHO
#define PANTALLA_ANCHO         128
#endif
#ifndef PANTALLA_ALTO
#define PANTALLA_ALTO          64
#endif

class Pantalla {
  public:

  // dibujo en memoria: 128 columnas x 8 paginas = 1024 bytes
  uint8_t buffer[PANTALLA_ANCHO * PANTALLA_ALTO / 8];

  /// constructor: solo deja el buffer en blanco. No toca el hardware. ///
  Pantalla();

  /// hardware ///
  bool iniciar();                              // configura I2C y la pantalla. false si no responde
  void encender();                             // prende la pantalla (no borra lo que tenia)
  void apagar();                               // apaga la pantalla (ahorra energia, no pierde el dibujo)
  void enviarBuffer(const uint8_t* datos);     // manda un buffer cualquiera (por ejemplo el del juego)
  void mostrar();                              // manda el buffer propio a la pantalla

  /// dibujo ///
  void limpiar();                              // todo el buffer en negro
  void pixel(int x, int y, bool encendido = true);
  void rect(int x, int y, int w, int h, bool relleno = true);

  // dibuja una imagen de 1 bit por pixel. Formato: filas de izquierda a derecha,
  // 8 pixeles por byte con el bit 7 como el mas a la izquierda (el formato de Rostros.h).
  void bitmap(const uint8_t* datos, int ancho, int alto, int x = 0, int y = 0);

  /// texto (letras de 5 x 8 pixeles; cada letra ocupa 6 columnas con su espacio) ///
  void caracter(int x, int y, char c, int escala = 1);
  void texto(int x, int y, const char* s, int escala = 1);
  void textoCentrado(int y, const char* s, int escala = 1);   // centrado en horizontal
  int  anchoTexto(const char* s, int escala = 1);             // cuantos pixeles de ancho ocupa
  // escribe el texto en varias lineas sin cortar palabras. Devuelve cuantas lineas uso.
  int  textoEnvuelto(int x, int y, const char* s, int columnasMax, int escala = 1);
};

#endif