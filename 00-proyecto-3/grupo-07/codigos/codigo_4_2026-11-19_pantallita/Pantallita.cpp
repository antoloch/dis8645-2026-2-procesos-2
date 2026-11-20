// Pantallita.cpp
// Dibuja en la pantalla lo que main.cpp le pide.
//
// créditos:
// - biblioteca ILI934X de Darren Horrocks (BSD 3-Clause, ver LICENSE-ILI934X.txt)
// - fuentes FreeMono que vienen con esa biblioteca

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Pantallita.h"

// las fuentes se incluyen solo aquí (cada una define sus datos en el .h)
#include "FreeMono12pt7b.h"
#include "FreeMono18pt7b.h"
#include "FreeMono24pt7b.h"

// la biblioteca pide GFXfont* sin const, pero no modifica las fuentes
static GFXfont* fuentePequena() { return (GFXfont*)&FreeMono12pt7b; }
static GFXfont* fuenteMediana() { return (GFXfont*)&FreeMono18pt7b; }
static GFXfont* fuenteGrande()  { return (GFXfont*)&FreeMono24pt7b; }

// velocidad del SPI para wokwi
// (para la pantalla física hay que revisar cuánto aguanta, ver Pantallita.h)
static const uint32_t VELOCIDAD_SPI = 40 * 1000 * 1000;

// margen a cada lado para los textos
static const int MARGEN = 8;

// OJO: en la biblioteca fillRect y drawRect reciben (x, y, ALTO, ANCHO, color)
// es decir, el alto va ANTES del ancho

// constructor
Pantallita::Pantallita()
  : lcd(spi0, PIN_CS, PIN_DC, PIN_RST, 240, 320, R90DEG) {
  // la ILI9341 mide 240x320; con R90DEG queda en horizontal (320x240)
  colorFondo    = lcd.colour565(24, 32, 72);
  colorTexto    = COLOUR_WHITE;
  colorCirculo  = lcd.colour565(80, 220, 230);
  colorCuadrado = lcd.colour565(130, 220, 120);
  colorCara     = lcd.colour565(255, 210, 60);
  colorCorazon  = lcd.colour565(235, 50, 80);
  colorAviso    = lcd.colour565(255, 210, 60);
}

void Pantallita::iniciar() {
  // SPI y pines según el diagrama de wokwi
  spi_init(spi0, VELOCIDAD_SPI);
  gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

  // CS, DC y RST los maneja la biblioteca con gpio_put,
  // pero hay que dejarlos como salida antes
  gpio_init(PIN_CS);
  gpio_set_dir(PIN_CS, GPIO_OUT);
  gpio_put(PIN_CS, 1);
  gpio_init(PIN_DC);
  gpio_set_dir(PIN_DC, GPIO_OUT);
  gpio_put(PIN_DC, 0);
  gpio_init(PIN_RST);
  gpio_set_dir(PIN_RST, GPIO_OUT);
  gpio_put(PIN_RST, 1);

  lcd.init();
  // pausa corta solo al encender, para que la pantalla termine de arrancar
  sleep_ms(120);

  vista = VistaPantalla::APAGADA;
  mostrarInicio();
}

// ------------------------------------------------------------
// VISTAS
// ------------------------------------------------------------
void Pantallita::limpiarFondo() {
  lcd.clear(colorFondo);
}

void Pantallita::mostrarInicio() {
  if (vista == VistaPantalla::INICIO) return;
  vista = VistaPantalla::INICIO;
  limpiarFondo();
  // reemplazo temporal de una ilustración amigable para la infancia
  dibujarTextoCentrado("ilustración", ALTO / 2 + 10, colorTexto, fuenteGrande());
}

void Pantallita::mostrarCuentos() {
  if (vista == VistaPantalla::CUENTOS) return;
  vista = VistaPantalla::CUENTOS;
  limpiarFondo();
  dibujarCirculo(ANCHO / 2, ALTO / 2, 70);
}

void Pantallita::mostrarMusica() {
  if (vista == VistaPantalla::MUSICA) return;
  vista = VistaPantalla::MUSICA;
  limpiarFondo();
  dibujarCuadrado(ANCHO / 2, ALTO / 2, 130);
}

void Pantallita::mostrarMensajeActivo() {
  if (vista == VistaPantalla::MENSAJE_ACTIVO) return;
  vista = VistaPantalla::MENSAJE_ACTIVO;
  limpiarFondo();
  dibujarCorazon(ANCHO / 2, ALTO / 2 + 5, 52);
}

void Pantallita::mostrarSinMensaje(uint32_t ahora) {
  // si ya estaba la carita, solo se reinicia su tiempo (sin redibujar)
  if (vista == VistaPantalla::SIN_MENSAJE) {
    inicioCarita = ahora;
    return;
  }
  // se recuerda qué había para volver ahí cuando pase el tiempo
  // la categoría activa NO se detiene ni cambia: eso lo decide main
  vistaBase = vista;
  vista = VistaPantalla::SIN_MENSAJE;
  inicioCarita = ahora;
  limpiarFondo();
  dibujarCaritaTriste(ANCHO / 2, ALTO / 2, 80);
}

void Pantallita::mostrarGrabando() {
  vista = VistaPantalla::GRABANDO;
  limpiarFondo();
  dibujarTextoCentrado("grabando...", 40, colorTexto, fuenteMediana());
  dibujarParrafo("puedes grabar máximo 60 segundos", 142, 22, colorTexto, fuentePequena());
  dibujarParrafo("suelta el botón para guardar tu mensaje", 196, 22, colorTexto, fuentePequena());
  // antes del primer segundo se muestra 0
  segundoMostrado = -1;
  actualizarConteo(0);
}

void Pantallita::actualizarConteo(uint32_t segundos) {
  if (vista != VistaPantalla::GRABANDO) return;
  if (segundos > 60) segundos = 60;
  // solo se dibuja si cambió el segundo (no se redibuja la pantalla)
  if ((int)segundos == segundoMostrado) return;
  segundoMostrado = (int)segundos;

  // se borra solo la zona del número y se vuelve a escribir
  lcd.fillRect(100, 60, 50, 120, colorFondo);
  char numero[8];
  snprintf(numero, sizeof(numero), "%u", (unsigned)segundos);
  dibujarTextoCentrado(numero, 100, colorTexto, fuenteGrande());
}

void Pantallita::mostrarGrabacionTerminada() {
  vista = VistaPantalla::GRABACION_TERMINADA;
  limpiarFondo();
  // solo dice que termina la simulación: no confirma que haya un archivo guardado
  dibujarParrafo("termina la simulación", 105, 40, colorTexto, fuenteMediana());
}

void Pantallita::mostrarLimiteAlcanzado() {
  vista = VistaPantalla::LIMITE_ALCANZADO;
  limpiarFondo();
  dibujarParrafo("llegaste a 60 segundos", 50, 40, colorAviso, fuenteMediana());
  dibujarParrafo("termina la simulación", 142, 22, colorTexto, fuentePequena());
  dibujarParrafo("suelta el botón para poder grabar otra vez", 192, 22, colorTexto, fuentePequena());
}

void Pantallita::apagar() {
  // en wokwi la luz de fondo no se puede cortar: la pantalla queda en negro
  // (la versión física debe apagar la luz de fondo, ver Pantallita.h)
  lcd.clear(COLOUR_BLACK);
  vista = VistaPantalla::APAGADA;
}

void Pantallita::actualizar(uint32_t ahora) {
  // la carita triste se quita sola sin detener el programa
  if (vista == VistaPantalla::SIN_MENSAJE &&
      (ahora - inicioCarita) >= DURACION_CARITA_MS) {
    redibujar(vistaBase);
  }
}

// vuelve a mostrar una vista que estaba antes de la carita
void Pantallita::redibujar(VistaPantalla v) {
  // se pone una vista "distinta" para que el método de cada vista dibuje
  vista = VistaPantalla::APAGADA;
  switch (v) {
    case VistaPantalla::CUENTOS:             mostrarCuentos(); break;
    case VistaPantalla::MUSICA:              mostrarMusica(); break;
    case VistaPantalla::MENSAJE_ACTIVO:      mostrarMensajeActivo(); break;
    case VistaPantalla::GRABACION_TERMINADA: mostrarGrabacionTerminada(); break;
    case VistaPantalla::LIMITE_ALCANZADO:    mostrarLimiteAlcanzado(); break;
    default:                                 mostrarInicio(); break;
  }
}

// ------------------------------------------------------------
// FIGURAS (solo con la biblioteca)
// ------------------------------------------------------------
// círculo lleno: una línea horizontal por cada fila
void Pantallita::circuloRelleno(int cx, int cy, int r, uint16_t color) {
  for (int dy = -r; dy <= r; dy++) {
    int mitad = (int)sqrtf((float)(r * r - dy * dy));
    lcd.fillRect(cx - mitad, cy + dy, 1, 2 * mitad + 1, color);
  }
}

// símbolo provisorio de cuentos
void Pantallita::dibujarCirculo(int cx, int cy, int r) {
  circuloRelleno(cx, cy, r, colorCirculo);
}

// símbolo provisorio de música
void Pantallita::dibujarCuadrado(int cx, int cy, int lado) {
  lcd.fillRect(cx - lado / 2, cy - lado / 2, lado, lado, colorCuadrado);
}

// corazón con la curva (x² + y² - 1)³ - x²y³ <= 0
// se recorre cada fila y se rellenan los tramos que quedan dentro
void Pantallita::dibujarCorazon(int cx, int cy, int tam) {
  int xMax = (int)(1.3f * tam);
  for (int py = -(int)(1.3f * tam); py <= (int)(1.1f * tam); py++) {
    float y = -(float)py / tam;          // en la pantalla y crece hacia abajo
    int inicio = 0;
    bool dentro = false;
    for (int px = -xMax; px <= xMax + 1; px++) {
      bool adentro = false;
      if (px <= xMax) {
        float x = (float)px / tam;
        float a = x * x + y * y - 1.0f;
        adentro = (a * a * a - x * x * y * y * y) <= 0.0f;
      }
      if (adentro && !dentro) {
        inicio = px;
        dentro = true;
      } else if (!adentro && dentro) {
        lcd.fillRect(cx + inicio, cy + py, 1, px - inicio, colorCorazon);
        dentro = false;
      }
    }
  }
}

// carita triste: círculo, dos ojos y una boca hacia abajo (sin texto)
void Pantallita::dibujarCaritaTriste(int cx, int cy, int r) {
  circuloRelleno(cx, cy, r, colorCara);
  // ojos
  circuloRelleno(cx - 28, cy - 18, 9, COLOUR_BLACK);
  circuloRelleno(cx + 28, cy - 18, 9, COLOUR_BLACK);
  // boca: parte de arriba de un círculo, así las puntas quedan más abajo
  const float radio = 36.0f;
  const int centroY = cy + 64;
  for (int dx = -28; dx <= 28; dx++) {
    int y = centroY - (int)sqrtf(radio * radio - (float)(dx * dx));
    lcd.fillRect(cx + dx - 2, y - 2, 5, 5, COLOUR_BLACK);
  }
}

// ------------------------------------------------------------
// TEXTO
// ------------------------------------------------------------
// las fuentes FreeMono son monoespaciadas: todas las letras avanzan igual
int Pantallita::anchoLetra(GFXfont* fuente) {
  return fuente->glyph[0].xAdvance;
}

// cuenta letras (no bytes): una letra con tilde en UTF-8 ocupa 2 bytes
int Pantallita::contarLetras(const char* texto) {
  int n = 0;
  for (; *texto; texto++) {
    if (((unsigned char)*texto & 0xC0) != 0x80) n++;
  }
  return n;
}

// dibuja una letra que empieza en utf8 y devuelve cuánto avanzó en x
// usados = cuántos bytes del texto se gastaron
int Pantallita::dibujarLetra(int x, int yBase, const char* utf8, int* usados,
                             uint16_t color, GFXfont* fuente) {
  unsigned char c0 = (unsigned char)utf8[0];
  char base = '?';
  char tilde = 0;   // 'a' = acento agudo, 'n' = virgulilla (ñ)
  *usados = 1;

  if (c0 < 0x80) {
    base = (char)c0;
  } else if (c0 == 0xC3 && utf8[1] != 0) {
    *usados = 2;
    switch ((unsigned char)utf8[1]) {
      case 0xA1: base = 'a'; tilde = 'a'; break;   // á
      case 0xA9: base = 'e'; tilde = 'a'; break;   // é
      case 0xAD: base = 'i'; tilde = 'a'; break;   // í
      case 0xB3: base = 'o'; tilde = 'a'; break;   // ó
      case 0xBA: base = 'u'; tilde = 'a'; break;   // ú
      case 0xB1: base = 'n'; tilde = 'n'; break;   // ñ
      default: break;
    }
  } else {
    // otro símbolo que la fuente no tiene: se salta completo
    while ((((unsigned char)utf8[*usados]) & 0xC0) == 0x80) (*usados)++;
  }

  if (base < 0x20 || base > 0x7E) base = '?';
  lcd.drawChar(x, yBase, base, color, fuente);

  if (tilde != 0) {
    // la tilde se dibuja con líneas, arriba de la letra
    GFXglyph* g = &fuente->glyph[base - fuente->first];
    int arriba = yBase + g->yOffset;                 // fila más alta de la letra
    int centro = x + g->xOffset + g->width / 2;
    int k = fuente->yAdvance / 12;                   // tamaño según la fuente
    if (k < 2) k = 2;
    if (tilde == 'a') {
      // acento agudo: línea que sube hacia la derecha (dos líneas, para que se vea)
      lcd.drawLine(centro - k, arriba - 1, centro + k, arriba - 2 * k - 1, color);
      lcd.drawLine(centro - k + 1, arriba - 1, centro + k + 1, arriba - 2 * k - 1, color);
    } else {
      // virgulilla: dos tramos
      lcd.drawLine(centro - k, arriba - k, centro, arriba - 2 * k, color);
      lcd.drawLine(centro, arriba - 2 * k, centro + k, arriba - k, color);
    }
  }
  return anchoLetra(fuente);
}

void Pantallita::dibujarTextoCentrado(const char* texto, int yBase,
                                      uint16_t color, GFXfont* fuente) {
  int ancho = contarLetras(texto) * anchoLetra(fuente);
  int x = (ANCHO - ancho) / 2;
  if (x < 0) x = 0;
  const char* p = texto;
  while (*p) {
    int usados = 1;
    x += dibujarLetra(x, yBase, p, &usados, color, fuente);
    p += usados;
  }
}

// parte el texto por palabras para que cada línea quepa en la pantalla
int Pantallita::dibujarParrafo(const char* texto, int yBase, int interlineado,
                               uint16_t color, GFXfont* fuente) {
  int maxLetras = (ANCHO - 2 * MARGEN) / anchoLetra(fuente);
  char linea[96];
  int bytes = 0;
  int letras = 0;
  int lineas = 0;
  const char* p = texto;

  while (*p) {
    // siguiente palabra: hasta el próximo espacio
    const char* q = p;
    int letrasPalabra = 0;
    while (*q && *q != ' ') {
      if (((unsigned char)*q & 0xC0) != 0x80) letrasPalabra++;
      q++;
    }
    int bytesPalabra = (int)(q - p);

    // si no cabe en esta línea, se dibuja la línea y se empieza otra
    if (letras > 0 && letras + 1 + letrasPalabra > maxLetras) {
      linea[bytes] = 0;
      dibujarTextoCentrado(linea, yBase + lineas * interlineado, color, fuente);
      lineas++;
      bytes = 0;
      letras = 0;
    }
    if (bytes + bytesPalabra + 2 < (int)sizeof(linea)) {
      if (letras > 0) {
        linea[bytes++] = ' ';
        letras++;
      }
      memcpy(linea + bytes, p, bytesPalabra);
      bytes += bytesPalabra;
      letras += letrasPalabra;
    }
    p = q;
    while (*p == ' ') p++;
  }

  if (bytes > 0) {
    linea[bytes] = 0;
    dibujarTextoCentrado(linea, yBase + lineas * interlineado, color, fuente);
    lineas++;
  }
  return lineas;
}
