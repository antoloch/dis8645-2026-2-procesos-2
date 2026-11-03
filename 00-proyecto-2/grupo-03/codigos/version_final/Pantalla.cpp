// Pantalla.cpp
// implementacion del driver de la pantalla OLED I2C (SH1106 / SSD1306)

#include <stdio.h>
#include <string.h>
#include "Pantalla.h"

// ---- FUENTE DE LETRAS ----------------------------------------------------------
// tipografia de 5 x 7 pixeles (8 filas con la cola de las letras g, j, p, q, y) para los ASCII 32 (espacio) a 126 (~).
// Cada letra son 5 bytes = 5 columnas; en cada byte el bit 0 es el pixel de arriba.
// (mismo orden que usa la pantalla: columnas de 8 pixeles verticales)
// Fuente "classic" de Adafruit GFX (licencia BSD, Copyright (c) 2012 Adafruit Industries).
// Solo tiene caracteres ASCII: no hay tildes ni "n" con tilde, por eso los mensajes no las usan.

#define FUENTE_PRIMER_CARACTER 32
#define FUENTE_ULTIMO_CARACTER 126
#define FUENTE_ANCHO 5
#define FUENTE_ALTO 8   // 7 filas de letra + 1 para la cola de g, j, p, q, y

static const uint8_t FUENTE_5X7[95][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00},  // espacio
  {0x00, 0x00, 0x5F, 0x00, 0x00},  // !
  {0x00, 0x07, 0x00, 0x07, 0x00},  // "
  {0x14, 0x7F, 0x14, 0x7F, 0x14},  // #
  {0x24, 0x2A, 0x7F, 0x2A, 0x12},  // $
  {0x23, 0x13, 0x08, 0x64, 0x62},  // %
  {0x36, 0x49, 0x56, 0x20, 0x50},  // &
  {0x00, 0x08, 0x07, 0x03, 0x00},  // '
  {0x00, 0x1C, 0x22, 0x41, 0x00},  // (
  {0x00, 0x41, 0x22, 0x1C, 0x00},  // )
  {0x2A, 0x1C, 0x7F, 0x1C, 0x2A},  // *
  {0x08, 0x08, 0x3E, 0x08, 0x08},  // +
  {0x00, 0x80, 0x70, 0x30, 0x00},  // ,
  {0x08, 0x08, 0x08, 0x08, 0x08},  // -
  {0x00, 0x00, 0x60, 0x60, 0x00},  // .
  {0x20, 0x10, 0x08, 0x04, 0x02},  // /
  {0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0
  {0x00, 0x42, 0x7F, 0x40, 0x00},  // 1
  {0x72, 0x49, 0x49, 0x49, 0x46},  // 2
  {0x21, 0x41, 0x49, 0x4D, 0x33},  // 3
  {0x18, 0x14, 0x12, 0x7F, 0x10},  // 4
  {0x27, 0x45, 0x45, 0x45, 0x39},  // 5
  {0x3C, 0x4A, 0x49, 0x49, 0x31},  // 6
  {0x41, 0x21, 0x11, 0x09, 0x07},  // 7
  {0x36, 0x49, 0x49, 0x49, 0x36},  // 8
  {0x46, 0x49, 0x49, 0x29, 0x1E},  // 9
  {0x00, 0x00, 0x14, 0x00, 0x00},  // :
  {0x00, 0x40, 0x34, 0x00, 0x00},  // ;
  {0x00, 0x08, 0x14, 0x22, 0x41},  // <
  {0x14, 0x14, 0x14, 0x14, 0x14},  // =
  {0x00, 0x41, 0x22, 0x14, 0x08},  // >
  {0x02, 0x01, 0x59, 0x09, 0x06},  // ?
  {0x3E, 0x41, 0x5D, 0x59, 0x4E},  // @
  {0x7C, 0x12, 0x11, 0x12, 0x7C},  // A
  {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
  {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
  {0x7F, 0x41, 0x41, 0x41, 0x3E},  // D
  {0x7F, 0x49, 0x49, 0x49, 0x41},  // E
  {0x7F, 0x09, 0x09, 0x09, 0x01},  // F
  {0x3E, 0x41, 0x41, 0x51, 0x73},  // G
  {0x7F, 0x08, 0x08, 0x08, 0x7F},  // H
  {0x00, 0x41, 0x7F, 0x41, 0x00},  // I
  {0x20, 0x40, 0x41, 0x3F, 0x01},  // J
  {0x7F, 0x08, 0x14, 0x22, 0x41},  // K
  {0x7F, 0x40, 0x40, 0x40, 0x40},  // L
  {0x7F, 0x02, 0x1C, 0x02, 0x7F},  // M
  {0x7F, 0x04, 0x08, 0x10, 0x7F},  // N
  {0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
  {0x7F, 0x09, 0x09, 0x09, 0x06},  // P
  {0x3E, 0x41, 0x51, 0x21, 0x5E},  // Q
  {0x7F, 0x09, 0x19, 0x29, 0x46},  // R
  {0x26, 0x49, 0x49, 0x49, 0x32},  // S
  {0x03, 0x01, 0x7F, 0x01, 0x03},  // T
  {0x3F, 0x40, 0x40, 0x40, 0x3F},  // U
  {0x1F, 0x20, 0x40, 0x20, 0x1F},  // V
  {0x3F, 0x40, 0x38, 0x40, 0x3F},  // W
  {0x63, 0x14, 0x08, 0x14, 0x63},  // X
  {0x03, 0x04, 0x78, 0x04, 0x03},  // Y
  {0x61, 0x59, 0x49, 0x4D, 0x43},  // Z
  {0x00, 0x7F, 0x41, 0x41, 0x41},  // [
  {0x02, 0x04, 0x08, 0x10, 0x20},  // barra invertida
  {0x00, 0x41, 0x41, 0x41, 0x7F},  // ]
  {0x04, 0x02, 0x01, 0x02, 0x04},  // ^
  {0x40, 0x40, 0x40, 0x40, 0x40},  // _
  {0x00, 0x03, 0x07, 0x08, 0x00},  // `
  {0x20, 0x54, 0x54, 0x78, 0x40},  // a
  {0x7F, 0x28, 0x44, 0x44, 0x38},  // b
  {0x38, 0x44, 0x44, 0x44, 0x28},  // c
  {0x38, 0x44, 0x44, 0x28, 0x7F},  // d
  {0x38, 0x54, 0x54, 0x54, 0x18},  // e
  {0x00, 0x08, 0x7E, 0x09, 0x02},  // f
  {0x18, 0xA4, 0xA4, 0x9C, 0x78},  // g
  {0x7F, 0x08, 0x04, 0x04, 0x78},  // h
  {0x00, 0x44, 0x7D, 0x40, 0x00},  // i
  {0x20, 0x40, 0x40, 0x3D, 0x00},  // j
  {0x7F, 0x10, 0x28, 0x44, 0x00},  // k
  {0x00, 0x41, 0x7F, 0x40, 0x00},  // l
  {0x7C, 0x04, 0x78, 0x04, 0x78},  // m
  {0x7C, 0x08, 0x04, 0x04, 0x78},  // n
  {0x38, 0x44, 0x44, 0x44, 0x38},  // o
  {0xFC, 0x18, 0x24, 0x24, 0x18},  // p
  {0x18, 0x24, 0x24, 0x18, 0xFC},  // q
  {0x7C, 0x08, 0x04, 0x04, 0x08},  // r
  {0x48, 0x54, 0x54, 0x54, 0x24},  // s
  {0x04, 0x04, 0x3F, 0x44, 0x24},  // t
  {0x3C, 0x40, 0x40, 0x20, 0x7C},  // u
  {0x1C, 0x20, 0x40, 0x20, 0x1C},  // v
  {0x3C, 0x40, 0x30, 0x40, 0x3C},  // w
  {0x44, 0x28, 0x10, 0x28, 0x44},  // x
  {0x4C, 0x90, 0x90, 0x90, 0x7C},  // y
  {0x44, 0x64, 0x54, 0x4C, 0x44},  // z
  {0x00, 0x08, 0x36, 0x41, 0x00},  // {
  {0x00, 0x00, 0x77, 0x00, 0x00},  // |
  {0x00, 0x41, 0x36, 0x08, 0x00},  // }
  {0x02, 0x01, 0x02, 0x04, 0x02},  // ~
};

// la SH1106 tiene 132 columnas de memoria y muestra de la 2 a la 129,
// por eso hay que empezar a escribir en la columna 2.
// la SSD1306 tiene 128 y empieza en la 0.
#if PANTALLA_SH1106
#define COLUMNA_INICIAL 2
#else
#define COLUMNA_INICIAL 0
#endif

// ---- funciones internas para hablar por I2C -----------------------------------
// en I2C cada mensaje parte con un byte de control:
//   0x00 = lo que viene son comandos      0x40 = lo que viene son datos de pixeles

static bool enviarComandos(const uint8_t* comandos, int cantidad) {
  uint8_t mensaje[8];
  mensaje[0] = 0x00;
  memcpy(mensaje + 1, comandos, cantidad);
  // devuelve un numero negativo si la pantalla no contesta
  return i2c_write_blocking(PANTALLA_I2C, PANTALLA_DIRECCION, mensaje, cantidad + 1, false) >= 0;
}

static bool comando1(uint8_t c) {
  return enviarComandos(&c, 1);
}

static bool comando2(uint8_t c, uint8_t argumento) {
  uint8_t dos[2] = { c, argumento };
  return enviarComandos(dos, 2);
}


Pantalla::Pantalla() {
  memset(Pantalla::buffer, 0, sizeof(Pantalla::buffer));
}

// configura el bus I2C y deja la pantalla lista. Devuelve false si la pantalla no responde.
bool Pantalla::iniciar() {
  i2c_init(PANTALLA_I2C, 400 * 1000);                  // 400 kHz
  gpio_set_function(PANTALLA_PIN_SDA, GPIO_FUNC_I2C);
  gpio_set_function(PANTALLA_PIN_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(PANTALLA_PIN_SDA);                      // I2C necesita las lineas con pull-up
  gpio_pull_up(PANTALLA_PIN_SCL);

  sleep_ms(100);   // la pantalla necesita un momento para despertar tras recibir energia

  // si el primer comando no obtiene respuesta, no hay pantalla conectada
  bool responde = comando1(0xAE);              // apagar mientras se configura

  comando2(0xD5, 0x80);                        // frecuencia del reloj interno
  comando2(0xA8, 0x3F);                        // 64 filas (multiplex 1/64)
  comando2(0xD3, 0x00);                        // sin desplazamiento vertical
  comando1(0x40);                              // la primera fila de memoria es la de arriba
#if PANTALLA_SH1106
  comando2(0xAD, 0x8B);                        // SH1106: convertidor de voltaje interno encendido
#else
  comando2(0x8D, 0x14);                        // SSD1306: bomba de carga encendida
  comando2(0x20, 0x02);                        // SSD1306: modo de direccionamiento por paginas
#endif
  comando1(0xA1);                              // espejo horizontal (si la imagen sale al reves, usar 0xA0)
  comando1(0xC8);                              // espejo vertical   (si la imagen sale al reves, usar 0xC0)
  comando2(0xDA, 0x12);                        // distribucion de pines de las filas
  comando2(0x81, 0xCF);                        // contraste (brillo)
  comando2(0xD9, 0xF1);                        // tiempo de precarga
  comando2(0xDB, 0x40);                        // nivel de voltaje de apagado
  comando1(0xA4);                              // mostrar el contenido de la memoria
  comando1(0xA6);                              // pixel encendido = blanco (no invertido)
  comando1(0xAF);                              // encender pantalla

  return responde;
}

void Pantalla::encender() {
  comando1(0xAF);
}

void Pantalla::apagar() {
  comando1(0xAE);
}

// manda un buffer de 1024 bytes a la pantalla, una pagina (8 filas) a la vez
void Pantalla::enviarBuffer(const uint8_t* datos) {
  uint8_t mensaje[PANTALLA_ANCHO + 1];
  mensaje[0] = 0x40;   // byte de control: lo que sigue son pixeles

  for (int pagina = 0; pagina < PANTALLA_ALTO / 8; pagina++) {
    // elegir pagina y columna donde se empieza a escribir
    uint8_t posicion[3] = {
      (uint8_t)(0xB0 | pagina),                       // numero de pagina
      (uint8_t)(COLUMNA_INICIAL & 0x0F),              // columna: 4 bits bajos
      (uint8_t)(0x10 | (COLUMNA_INICIAL >> 4))        // columna: 4 bits altos
    };
    enviarComandos(posicion, 3);

    // los 128 bytes de la pagina
    memcpy(mensaje + 1, datos + pagina * PANTALLA_ANCHO, PANTALLA_ANCHO);
    i2c_write_blocking(PANTALLA_I2C, PANTALLA_DIRECCION, mensaje, PANTALLA_ANCHO + 1, false);
  }
}

void Pantalla::mostrar() {
  Pantalla::enviarBuffer(Pantalla::buffer);
}


// ---- DIBUJO -----------------------------------------------------------------------

void Pantalla::limpiar() {
  memset(Pantalla::buffer, 0, sizeof(Pantalla::buffer));
}

// enciende (o apaga) un pixel. Lo que quede fuera de la pantalla se ignora.
void Pantalla::pixel(int x, int y, bool encendido) {
  if (x < 0 || x >= PANTALLA_ANCHO || y < 0 || y >= PANTALLA_ALTO) return;
  int indice = x + (y / 8) * PANTALLA_ANCHO;
  if (encendido) {
    Pantalla::buffer[indice] |= (1 << (y % 8));
  }
  else {
    Pantalla::buffer[indice] &= ~(1 << (y % 8));
  }
}

void Pantalla::rect(int x, int y, int w, int h, bool relleno) {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      bool borde = (i == 0 || j == 0 || i == w - 1 || j == h - 1);
      if (relleno || borde) {
        Pantalla::pixel(x + i, y + j);
      }
    }
  }
}

// dibuja una imagen de 1 bit por pixel (formato de Rostros.h):
// cada fila ocupa ancho/8 bytes y el bit 7 de cada byte es el pixel de la izquierda
void Pantalla::bitmap(const uint8_t* datos, int ancho, int alto, int x0, int y0) {
  int bytesPorFila = (ancho + 7) / 8;
  for (int y = 0; y < alto; y++) {
    for (int x = 0; x < ancho; x++) {
      uint8_t byte = datos[y * bytesPorFila + x / 8];
      if (byte & (0x80 >> (x % 8))) {
        Pantalla::pixel(x0 + x, y0 + y);
      }
    }
  }
}


// ---- TEXTO -------------------------------------------------------------------------

// dibuja una letra con su esquina superior izquierda en (x, y). escala 2 = el doble de grande.
void Pantalla::caracter(int x, int y, char c, int escala) {
  // lo que la fuente no tiene (tildes, n con tilde, etc.) se muestra como "?"
  if (c < FUENTE_PRIMER_CARACTER || c > FUENTE_ULTIMO_CARACTER) c = '?';
  const uint8_t* letra = FUENTE_5X7[c - FUENTE_PRIMER_CARACTER];

  for (int columna = 0; columna < FUENTE_ANCHO; columna++) {
    for (int fila = 0; fila < FUENTE_ALTO; fila++) {
      if (letra[columna] & (1 << fila)) {
        Pantalla::rect(x + columna * escala, y + fila * escala, escala, escala);
      }
    }
  }
}

void Pantalla::texto(int x, int y, const char* s, int escala) {
  for (int i = 0; s[i] != 0; i++) {
    Pantalla::caracter(x + i * (FUENTE_ANCHO + 1) * escala, y, s[i], escala);
  }
}

// ancho en pixeles: 6 columnas por letra, menos el espacio final
int Pantalla::anchoTexto(const char* s, int escala) {
  int largo = (int)strlen(s);
  if (largo == 0) return 0;
  return largo * (FUENTE_ANCHO + 1) * escala - escala;
}

void Pantalla::textoCentrado(int y, const char* s, int escala) {
  Pantalla::texto((PANTALLA_ANCHO - Pantalla::anchoTexto(s, escala)) / 2, y, s, escala);
}

// escribe el texto en varias lineas de maximo "columnasMax" letras, sin cortar palabras.
// entre linea y linea hay 1 pixel de espacio. Devuelve cuantas lineas escribio.
int Pantalla::textoEnvuelto(int x, int y, const char* s, int columnasMax, int escala) {
  char linea[32];
  int largo = 0;
  int lineas = 0;
  int altoLinea = (FUENTE_ALTO + 1) * escala;
  if (columnasMax > 31) columnasMax = 31;

  const char* p = s;
  while (*p != 0) {
    while (*p == ' ') p++;            // saltar espacios
    if (*p == 0) break;

    // medir la palabra que viene
    int largoPalabra = 0;
    while (p[largoPalabra] != 0 && p[largoPalabra] != ' ') largoPalabra++;

    // si no cabe en la linea actual, escribir la linea y partir una nueva
    if (largo > 0 && largo + 1 + largoPalabra > columnasMax) {
      linea[largo] = 0;
      Pantalla::texto(x, y + lineas * altoLinea, linea, escala);
      lineas++;
      largo = 0;
    }

    // agregar la palabra (si una palabra sola es mas larga que la linea, se corta)
    if (largo > 0) linea[largo++] = ' ';
    for (int i = 0; i < largoPalabra && largo < columnasMax; i++) {
      linea[largo++] = p[i];
    }
    p += largoPalabra;
  }

  // la ultima linea
  if (largo > 0) {
    linea[largo] = 0;
    Pantalla::texto(x, y + lineas * altoLinea, linea, escala);
    lineas++;
  }
  return lineas;
}