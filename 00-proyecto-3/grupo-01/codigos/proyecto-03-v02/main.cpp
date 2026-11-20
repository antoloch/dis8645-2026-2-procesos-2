// proyecto-03-v02 - grupo-01
// prototipo para wokwi con raspberry pi pico sdk
//
// la grabacion (INMP441) y la reproduccion de audio (PCM5102A + PAM8302)
// se reemplazan por mensajes en el serial monitor.
// la tarjeta SD no se usa: el conjuntoDeAudios vive en la RAM.
//
// -- decisiones tomadas con el grupo --
// 1. grabacion de 30 s como maximo y 0,5 s como minimo
// 2. un solo boton de reproduccion: reproducir desde 0 en bucle / detener
// 3. segmento al azar entre los 60, con superposicion, limite de 200 audios
// 4. efectos aplicados quedan fijos, el que se edita parpadea y se pueden reeditar
// 5. al reeditar se usa el valor actual del slider
// 6. al cambiar de efecto o guardar, se guarda el valor del efecto en edicion
// 7. al entrar a edicion los 9 leds se encienden fijo un momento y se apagan
// 8. al apagar con el switch la maquina queda en suspension y se descarta
//    lo que no se haya guardado
// 9. si un audio se pasa del final del ciclo de 10 min, lo que sobra suena
//    al inicio de la siguiente vuelta
// 10. pantalla: "grupo-01" por 2 s al encender, contador de audios y % libre,
//     grilla de 60 segmentos y marca del segmento que suena
// 11. el volumen especifico se mueve libremente en edicion y se guarda con el audio
// 12. velocidad: de 45 a 55 es normal (1x), menos es mas lento (hasta 0,5x)
//     y mas es mas rapido (hasta 2x)
// 13. mientras se reproduce la mezcla se puede presionar grabar: la mezcla
//     se detiene (vuelve al inicio) y empieza la grabacion

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

// =====================================================================
// conexiones (segun la tabla de conexiones del README)
// =====================================================================

// multiplexor CD74HC4067 de botones
const uint PIN_BOTONES_S0 = 14;
const uint PIN_BOTONES_S1 = 15;
const uint PIN_BOTONES_S2 = 10;
const uint PIN_BOTONES_S3 = 17;
const uint PIN_BOTONES_SIGNAL = 18;

// canales del CD74HC4067
const uint8_t CANAL_FX_PRIMERO = 0; // C0 a C8 son los 9 efectos
const uint8_t CANAL_GUARDAR_EDICION = 9;
const uint8_t CANAL_GRABAR = 10;
const uint8_t CANAL_REPRODUCIR = 11;
const uint8_t CANAL_SWITCH_ENCENDIDO = 12;

// 74HC595 encadenados de las lucecitas
const uint PIN_LUCECITAS_DS = 6;
const uint PIN_LUCECITAS_STCP = 7;
const uint PIN_LUCECITAS_SHCP = 8;

// multiplexor 74HC4051 de perillas
const uint PIN_PERILLA_SIGNAL = 26; // ADC0
const uint PIN_PERILLA_S0 = 13;
const uint PIN_PERILLA_S1 = 12;
const uint PIN_PERILLA_S2 = 11;

// canales del 74HC4051
const uint8_t CANAL_VOLUMEN_GENERAL = 0;
const uint8_t CANAL_VOLUMEN_ESPECIFICO = 1;
const uint8_t CANAL_FX6AL8 = 2;
const uint8_t CANAL_FX3AL5 = 3;
const uint8_t CANAL_FX0AL2 = 4;

// pantalla SSD1306
const uint PIN_PANTALLA_SDA = 4;
const uint PIN_PANTALLA_SCL = 5;
const uint8_t PANTALLA_DIRECCION = 0x3C;
#define PANTALLA_I2C i2c0

// =====================================================================
// constantes del funcionamiento
// =====================================================================

const uint32_t GRABACION_MAXIMA_MS = 30000;
const uint32_t GRABACION_MINIMA_MS = 500;
const int LIMITE_AUDIOS = 200;
const int CANTIDAD_SEGMENTOS = 60;
const uint32_t DURACION_SEGMENTO_MS = 10000;
const uint32_t DURACION_CICLO_MS = CANTIDAD_SEGMENTOS * DURACION_SEGMENTO_MS; // 10 min
const uint32_t PRESENTACION_MS = 2000;
const uint32_t ANIMACION_EDICION_MS = 1500; // "prenden por un corto periodo"
const uint32_t PARPADEO_MS = 250;
const uint32_t DEBOUNCE_MS = 30;
const int CANTIDAD_EFECTOS = 9;

const char *NOMBRE_EFECTO[CANTIDAD_EFECTOS] = {
    "Attack", "Release", "HighPassF",
    "Pitching", "Reverb", "Tremolo",
    "BitCrushing", "RingModulator", "Velocidad"};

// cada seccion de 3 efectos tiene su slider
const uint8_t CANAL_SLIDER_SECCION[3] = {CANAL_FX0AL2, CANAL_FX3AL5, CANAL_FX6AL8};
const char *NOMBRE_SLIDER_SECCION[3] = {"fx0al2", "fx3al5", "fx6al8"};

// =====================================================================
// aleatoriedad
// =====================================================================

// xorshift simple, la semilla se mezcla con el momento en que se presionan
// los botones, que depende de la persona
static uint32_t semillaAleatoria = 0x1234ABCD;

void mezclarSemilla(uint32_t valor) {
  semillaAleatoria ^= valor + 0x9E3779B9 + (semillaAleatoria << 6) + (semillaAleatoria >> 2);
  if (semillaAleatoria == 0) semillaAleatoria = 0xA5A5A5A5;
}

uint32_t aleatorio() {
  semillaAleatoria ^= semillaAleatoria << 13;
  semillaAleatoria ^= semillaAleatoria >> 17;
  semillaAleatoria ^= semillaAleatoria << 5;
  return semillaAleatoria;
}

// =====================================================================
// botones por el CD74HC4067
// =====================================================================

// los botones tienen resistencia pull-down externa: presionado = HIGH
class BotonMux {
public:
  uint8_t canal;
  bool estado = false;         // estado estable (sin rebote)
  bool presionado = false;     // true solo en la vuelta en que se presiona
  bool lecturaAnterior = false;
  uint32_t ultimoCambioMs = 0;

  BotonMux(uint8_t c) : canal(c) {}

  static void iniciarMultiplexor() {
    const uint pines[4] = {PIN_BOTONES_S0, PIN_BOTONES_S1, PIN_BOTONES_S2, PIN_BOTONES_S3};
    for (uint p : pines) {
      gpio_init(p);
      gpio_set_dir(p, GPIO_OUT);
      gpio_put(p, 0);
    }
    gpio_init(PIN_BOTONES_SIGNAL);
    gpio_set_dir(PIN_BOTONES_SIGNAL, GPIO_IN);
    gpio_pull_down(PIN_BOTONES_SIGNAL);
  }

  static bool leerCanal(uint8_t c) {
    gpio_put(PIN_BOTONES_S0, c & 1);
    gpio_put(PIN_BOTONES_S1, (c >> 1) & 1);
    gpio_put(PIN_BOTONES_S2, (c >> 2) & 1);
    gpio_put(PIN_BOTONES_S3, (c >> 3) & 1);
    // el chip del CD74HC4067 en wokwi revisa S0-S3 con un temporizador cada 100 us,
    // hay que esperar mas que eso para que COM muestre el canal nuevo
    sleep_us(250);
    return gpio_get(PIN_BOTONES_SIGNAL);
  }

  void leer() {
    presionado = false;
    uint32_t ahora = to_ms_since_boot(get_absolute_time());
    bool lectura = leerCanal(canal);
    if (lectura != lecturaAnterior) {
      lecturaAnterior = lectura;
      ultimoCambioMs = ahora;
    }
    if ((ahora - ultimoCambioMs) >= DEBOUNCE_MS && lectura != estado) {
      estado = lectura;
      if (estado) {
        presionado = true;
        mezclarSemilla(time_us_32());
      }
    }
  }
};

// =====================================================================
// perillas por el 74HC4051
// =====================================================================

class Perilla {
public:
  uint8_t canal;
  int posicion = -1;   // 0 a 100
  bool cambio = false;

  Perilla(uint8_t c) : canal(c) {}

  static void iniciarMultiplexor() {
    const uint pines[3] = {PIN_PERILLA_S0, PIN_PERILLA_S1, PIN_PERILLA_S2};
    for (uint p : pines) {
      gpio_init(p);
      gpio_set_dir(p, GPIO_OUT);
      gpio_put(p, 0);
    }
    adc_init();
    adc_gpio_init(PIN_PERILLA_SIGNAL);
    adc_select_input(0);
  }

  void leer() {
    gpio_put(PIN_PERILLA_S0, canal & 1);
    gpio_put(PIN_PERILLA_S1, (canal >> 1) & 1);
    gpio_put(PIN_PERILLA_S2, (canal >> 2) & 1);
    sleep_us(50);
    uint32_t suma = 0;
    for (int i = 0; i < 4; i++) suma += adc_read();
    int nueva = (int)((suma / 4) * 100 / 4095);
    cambio = false;
    // solo cuenta como cambio si se movio al menos 2 puntos o llego a un extremo
    if (posicion < 0 || abs(nueva - posicion) >= 2 ||
        (nueva != posicion && (nueva == 0 || nueva == 100))) {
      cambio = (posicion >= 0);
      posicion = nueva;
    }
  }
};

// =====================================================================
// lucecitas por los 74HC595 encadenados
// =====================================================================

// bit 0 a 7 = Q0 a Q7 del primer 74HC595, bit 8 = Q0 del segundo
class Lucecitas {
public:
  uint16_t estadoActual = 0xFFFF; // fuerza la primera escritura

  void iniciar() {
    const uint pines[3] = {PIN_LUCECITAS_DS, PIN_LUCECITAS_STCP, PIN_LUCECITAS_SHCP};
    for (uint p : pines) {
      gpio_init(p);
      gpio_set_dir(p, GPIO_OUT);
      gpio_put(p, 0);
    }
    escribir(0);
  }

  void escribir(uint16_t bits) {
    if (bits == estadoActual) return;
    estadoActual = bits;
    gpio_put(PIN_LUCECITAS_STCP, 0);
    for (int i = 15; i >= 0; i--) {
      gpio_put(PIN_LUCECITAS_DS, (bits >> i) & 1);
      sleep_us(1);
      gpio_put(PIN_LUCECITAS_SHCP, 1);
      sleep_us(1);
      gpio_put(PIN_LUCECITAS_SHCP, 0);
    }
    gpio_put(PIN_LUCECITAS_STCP, 1);
    sleep_us(1);
    gpio_put(PIN_LUCECITAS_STCP, 0);
  }
};

// =====================================================================
// pantalla SSD1306 128x64 (driver minimo)
// =====================================================================

// fuente 5x7, caracteres ascii del 32 al 126
static const uint8_t FUENTE[95][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x5F, 0x00, 0x00}, {0x00, 0x07, 0x00, 0x07, 0x00},
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, {0x24, 0x2A, 0x7F, 0x2A, 0x12}, {0x23, 0x13, 0x08, 0x64, 0x62},
    {0x36, 0x49, 0x56, 0x20, 0x50}, {0x00, 0x08, 0x07, 0x03, 0x00}, {0x00, 0x1C, 0x22, 0x41, 0x00},
    {0x00, 0x41, 0x22, 0x1C, 0x00}, {0x2A, 0x1C, 0x7F, 0x1C, 0x2A}, {0x08, 0x08, 0x3E, 0x08, 0x08},
    {0x00, 0x80, 0x70, 0x30, 0x00}, {0x08, 0x08, 0x08, 0x08, 0x08}, {0x00, 0x00, 0x60, 0x60, 0x00},
    {0x20, 0x10, 0x08, 0x04, 0x02}, {0x3E, 0x51, 0x49, 0x45, 0x3E}, {0x00, 0x42, 0x7F, 0x40, 0x00},
    {0x72, 0x49, 0x49, 0x49, 0x46}, {0x21, 0x41, 0x49, 0x4D, 0x33}, {0x18, 0x14, 0x12, 0x7F, 0x10},
    {0x27, 0x45, 0x45, 0x45, 0x39}, {0x3C, 0x4A, 0x49, 0x49, 0x31}, {0x41, 0x21, 0x11, 0x09, 0x07},
    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x46, 0x49, 0x49, 0x29, 0x1E}, {0x00, 0x00, 0x14, 0x00, 0x00},
    {0x00, 0x40, 0x34, 0x00, 0x00}, {0x00, 0x08, 0x14, 0x22, 0x41}, {0x14, 0x14, 0x14, 0x14, 0x14},
    {0x00, 0x41, 0x22, 0x14, 0x08}, {0x02, 0x01, 0x59, 0x09, 0x06}, {0x3E, 0x41, 0x5D, 0x59, 0x4E},
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, {0x7F, 0x49, 0x49, 0x49, 0x36}, {0x3E, 0x41, 0x41, 0x41, 0x22},
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, {0x7F, 0x49, 0x49, 0x49, 0x41}, {0x7F, 0x09, 0x09, 0x09, 0x01},
    {0x3E, 0x41, 0x41, 0x51, 0x73}, {0x7F, 0x08, 0x08, 0x08, 0x7F}, {0x00, 0x41, 0x7F, 0x41, 0x00},
    {0x20, 0x40, 0x41, 0x3F, 0x01}, {0x7F, 0x08, 0x14, 0x22, 0x41}, {0x7F, 0x40, 0x40, 0x40, 0x40},
    {0x7F, 0x02, 0x1C, 0x02, 0x7F}, {0x7F, 0x04, 0x08, 0x10, 0x7F}, {0x3E, 0x41, 0x41, 0x41, 0x3E},
    {0x7F, 0x09, 0x09, 0x09, 0x06}, {0x3E, 0x41, 0x51, 0x21, 0x5E}, {0x7F, 0x09, 0x19, 0x29, 0x46},
    {0x26, 0x49, 0x49, 0x49, 0x32}, {0x03, 0x01, 0x7F, 0x01, 0x03}, {0x3F, 0x40, 0x40, 0x40, 0x3F},
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, {0x3F, 0x40, 0x38, 0x40, 0x3F}, {0x63, 0x14, 0x08, 0x14, 0x63},
    {0x03, 0x04, 0x78, 0x04, 0x03}, {0x61, 0x59, 0x49, 0x4D, 0x43}, {0x00, 0x7F, 0x41, 0x41, 0x41},
    {0x02, 0x04, 0x08, 0x10, 0x20}, {0x00, 0x41, 0x41, 0x41, 0x7F}, {0x04, 0x02, 0x01, 0x02, 0x04},
    {0x40, 0x40, 0x40, 0x40, 0x40}, {0x00, 0x03, 0x07, 0x08, 0x00}, {0x20, 0x54, 0x54, 0x78, 0x40},
    {0x7F, 0x28, 0x44, 0x44, 0x38}, {0x38, 0x44, 0x44, 0x44, 0x28}, {0x38, 0x44, 0x44, 0x28, 0x7F},
    {0x38, 0x54, 0x54, 0x54, 0x18}, {0x00, 0x08, 0x7E, 0x09, 0x02}, {0x18, 0xA4, 0xA4, 0x9C, 0x78},
    {0x7F, 0x08, 0x04, 0x04, 0x78}, {0x00, 0x44, 0x7D, 0x40, 0x00}, {0x20, 0x40, 0x40, 0x3D, 0x00},
    {0x7F, 0x10, 0x28, 0x44, 0x00}, {0x00, 0x41, 0x7F, 0x40, 0x00}, {0x7C, 0x04, 0x78, 0x04, 0x78},
    {0x7C, 0x08, 0x04, 0x04, 0x78}, {0x38, 0x44, 0x44, 0x44, 0x38}, {0xFC, 0x18, 0x24, 0x24, 0x18},
    {0x18, 0x24, 0x24, 0x18, 0xFC}, {0x7C, 0x08, 0x04, 0x04, 0x08}, {0x48, 0x54, 0x54, 0x54, 0x24},
    {0x04, 0x04, 0x3F, 0x44, 0x24}, {0x3C, 0x40, 0x40, 0x20, 0x7C}, {0x1C, 0x20, 0x40, 0x20, 0x1C},
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, {0x44, 0x28, 0x10, 0x28, 0x44}, {0x4C, 0x90, 0x90, 0x90, 0x7C},
    {0x44, 0x64, 0x54, 0x4C, 0x44}, {0x00, 0x08, 0x36, 0x41, 0x00}, {0x00, 0x00, 0x77, 0x00, 0x00},
    {0x00, 0x41, 0x36, 0x08, 0x00}, {0x02, 0x01, 0x02, 0x04, 0x02}};

class Pantalla {
public:
  uint8_t buffer[128 * 8];

  void comando(uint8_t c) {
    uint8_t datos[2] = {0x00, c};
    i2c_write_blocking(PANTALLA_I2C, PANTALLA_DIRECCION, datos, 2, false);
  }

  void iniciar() {
    i2c_init(PANTALLA_I2C, 400 * 1000);
    gpio_set_function(PIN_PANTALLA_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_PANTALLA_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_PANTALLA_SDA);
    gpio_pull_up(PIN_PANTALLA_SCL);
    const uint8_t secuencia[] = {
        0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14,
        0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1,
        0xDB, 0x40, 0xA4, 0xA6};
    for (uint8_t c : secuencia) comando(c);
    limpiar();
    mostrar();
  }

  void encender() { comando(0xAF); }
  void apagar() { comando(0xAE); }

  void limpiar() { memset(buffer, 0, sizeof(buffer)); }

  void pixel(int x, int y, bool encendido = true) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    if (encendido) buffer[x + (y / 8) * 128] |= (1 << (y % 8));
    else buffer[x + (y / 8) * 128] &= ~(1 << (y % 8));
  }

  void rectanguloLleno(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++)
      for (int j = y; j < y + h; j++) pixel(i, j);
  }

  void rectangulo(int x, int y, int w, int h) {
    for (int i = x; i < x + w; i++) { pixel(i, y); pixel(i, y + h - 1); }
    for (int j = y; j < y + h; j++) { pixel(x, j); pixel(x + w - 1, j); }
  }

  void caracter(int x, int y, char c, int escala = 1) {
    if (c < 32 || c > 126) c = '?';
    for (int col = 0; col < 5; col++) {
      uint8_t linea = FUENTE[c - 32][col];
      for (int fila = 0; fila < 8; fila++) {
        if (linea & (1 << fila)) rectanguloLleno(x + col * escala, y + fila * escala, escala, escala);
      }
    }
  }

  void texto(int x, int y, const char *t, int escala = 1) {
    while (*t) {
      caracter(x, y, *t++, escala);
      x += 6 * escala;
    }
  }

  int anchoTexto(const char *t, int escala = 1) { return (int)strlen(t) * 6 * escala - escala; }

  void mostrar() {
    comando(0x21); comando(0); comando(127);
    comando(0x22); comando(0); comando(7);
    uint8_t bloque[17];
    bloque[0] = 0x40;
    for (int i = 0; i < (int)sizeof(buffer); i += 16) {
      memcpy(&bloque[1], &buffer[i], 16);
      i2c_write_blocking(PANTALLA_I2C, PANTALLA_DIRECCION, bloque, 17, false);
    }
  }
};

// =====================================================================
// conjuntoDeAudios
// =====================================================================

struct Audio {
  int id;
  uint32_t duracionMs;
  int segmento;                  // 0 a 59
  int volumenEspecifico;         // 0 a 100
  bool fxAplicado[CANTIDAD_EFECTOS];
  int fxValor[CANTIDAD_EFECTOS]; // 0 a 100
};

static Audio conjuntoDeAudios[LIMITE_AUDIOS];
static int cantidadAudios = 0;
static int audiosPorSegmento[CANTIDAD_SEGMENTOS];

// sonido en edicion (todavia no guardado)
static Audio sonidoEnEdicion;

// =====================================================================
// estado de la maquina
// =====================================================================

enum Modo {
  SUSPENSION,
  PRESENTACION,
  GENERAL,
  REPRODUCIENDO,
  GRABANDO,
  ANIMACION_EDICION,
  EDITANDO
};

static Modo modo = SUSPENSION;
static uint32_t inicioModoMs = 0;

// grabacion
static int ultimoSegundoInformado = -1;

// edicion
static int efectoEnEdicion = -1; // -1 = ninguno
static uint32_t inicioBucleEdicionMs = 0;

// reproduccion
static int segmentoActual = -1;
static uint32_t vueltaActual = 0;

// piezas
static BotonMux botonFx[CANTIDAD_EFECTOS] = {
    BotonMux(0), BotonMux(1), BotonMux(2), BotonMux(3), BotonMux(4),
    BotonMux(5), BotonMux(6), BotonMux(7), BotonMux(8)};
static BotonMux guardarEdicion(CANAL_GUARDAR_EDICION);
static BotonMux botonGrabar(CANAL_GRABAR);
static BotonMux botonReproducir(CANAL_REPRODUCIR);
static BotonMux switchEncendido(CANAL_SWITCH_ENCENDIDO);

static Perilla volumenGeneral(CANAL_VOLUMEN_GENERAL);
static Perilla volumenEspecifico(CANAL_VOLUMEN_ESPECIFICO);
static Perilla sliderSeccion[3] = {Perilla(CANAL_FX0AL2), Perilla(CANAL_FX3AL5), Perilla(CANAL_FX6AL8)};

static Lucecitas lucecitas;
static Pantalla pantalla;

uint32_t ahoraMs() { return to_ms_since_boot(get_absolute_time()); }

void cambiarModo(Modo nuevo) {
  modo = nuevo;
  inicioModoMs = ahoraMs();
}

void imprimirTiempo(uint32_t ms) {
  printf("%02lu:%02lu", (unsigned long)(ms / 60000), (unsigned long)((ms / 1000) % 60));
}

// velocidad: 45 a 55 = normal (1x), menos = mas lento (hasta 0,5x en 0),
// mas = mas rapido (hasta 2x en 100)
const int EFECTO_VELOCIDAD = 8;
const int VELOCIDAD_NORMAL_MIN = 45;
const int VELOCIDAD_NORMAL_MAX = 55;

// factor de velocidad multiplicado por 100 (50 = 0,5x, 100 = 1x, 200 = 2x)
int factorVelocidadX100(int valor) {
  if (valor < VELOCIDAD_NORMAL_MIN) return 50 + valor * 50 / VELOCIDAD_NORMAL_MIN;
  if (valor > VELOCIDAD_NORMAL_MAX)
    return 100 + (valor - VELOCIDAD_NORMAL_MAX) * 100 / (100 - VELOCIDAD_NORMAL_MAX);
  return 100;
}

// imprime el valor de un efecto; la velocidad agrega su texto y su factor
void imprimirValorEfecto(int efecto, int valor) {
  printf("%d", valor);
  if (efecto == EFECTO_VELOCIDAD) {
    int f = factorVelocidadX100(valor);
    const char *texto = f == 100 ? "normal" : (f > 100 ? "mas rapido" : "mas lento");
    printf(" (%s, %d.%02dx)", texto, f / 100, f % 100);
  }
}

void imprimirEfectos(const Audio &a) {
  bool alguno = false;
  for (int i = 0; i < CANTIDAD_EFECTOS; i++) {
    if (a.fxAplicado[i]) {
      printf("%s%s=", alguno ? ", " : "", NOMBRE_EFECTO[i]);
      imprimirValorEfecto(i, a.fxValor[i]);
      alguno = true;
    }
  }
  if (!alguno) printf("sin efectos");
}

// =====================================================================
// dibujos de la pantalla
// =====================================================================

// vista general: contador arriba y grilla de 60 segmentos (12 x 5)
void dibujarVistaGeneral(int segmentoMarcado) {
  pantalla.limpiar();

  char linea[24];
  snprintf(linea, sizeof(linea), "%d audios", cantidadAudios);
  pantalla.texto(0, 4, linea);
  int libre = (LIMITE_AUDIOS - cantidadAudios) * 100 / LIMITE_AUDIOS;
  snprintf(linea, sizeof(linea), "%d%% libre", libre);
  pantalla.texto(128 - pantalla.anchoTexto(linea), 4, linea);

  for (int s = 0; s < CANTIDAD_SEGMENTOS; s++) {
    int x = 4 + (s % 12) * 10;
    int y = 17 + (s / 12) * 9;
    int cantidad = audiosPorSegmento[s];
    if (cantidad == 0) {
      // segmento en silencio: un punto
      pantalla.pixel(x + 4, y + 4);
    } else {
      // cada audio suma una agrupacion de 2x2 pixeles (maximo 12 por segmento)
      int grupos = cantidad > 12 ? 12 : cantidad;
      for (int g = 0; g < grupos; g++) {
        pantalla.rectanguloLleno(x + 1 + (g % 4) * 2, y + 1 + (g / 4) * 2, 2, 2);
      }
    }
    if (s == segmentoMarcado) pantalla.rectangulo(x - 1, y - 1, 11, 9);
  }
  pantalla.mostrar();
}

// grabacion: barra arriba que disminuye y cuenta regresiva de 30 a 0
void dibujarGrabacion(uint32_t transcurridoMs) {
  pantalla.limpiar();
  uint32_t restanteMs = transcurridoMs >= GRABACION_MAXIMA_MS ? 0 : GRABACION_MAXIMA_MS - transcurridoMs;
  int anchoBarra = (int)(restanteMs * 128 / GRABACION_MAXIMA_MS);
  pantalla.rectanguloLleno(0, 2, anchoBarra, 10);

  char numero[4];
  snprintf(numero, sizeof(numero), "%lu", (unsigned long)((restanteMs + 999) / 1000));
  pantalla.texto((128 - pantalla.anchoTexto(numero, 4)) / 2, 20, numero, 4);
  pantalla.mostrar();
}

void dibujarPresentacion() {
  pantalla.limpiar();
  const char *nombre = "grupo-01";
  pantalla.texto((128 - pantalla.anchoTexto(nombre, 2)) / 2, 24, nombre, 2);
  pantalla.mostrar();
}

// =====================================================================
// lucecitas segun el estado de la edicion
// =====================================================================

void actualizarLucecitas() {
  uint16_t bits = 0;
  if (modo == ANIMACION_EDICION) {
    bits = 0x01FF; // las 9 encendidas fijo
  } else if (modo == EDITANDO) {
    for (int i = 0; i < CANTIDAD_EFECTOS; i++) {
      if (i == efectoEnEdicion) {
        if (((ahoraMs() - inicioModoMs) / PARPADEO_MS) % 2 == 0) bits |= (1 << i);
      } else if (sonidoEnEdicion.fxAplicado[i]) {
        bits |= (1 << i);
      }
    }
  }
  lucecitas.escribir(bits);
}

// =====================================================================
// acciones
// =====================================================================

void entrarSuspension() {
  if (modo == GRABANDO) printf("[grabar] grabacion interrumpida, el sonido se descarta\n");
  if (modo == ANIMACION_EDICION || modo == EDITANDO)
    printf("[editar] edicion interrumpida, el sonido sin guardar se descarta\n");
  if (modo == REPRODUCIENDO) printf("[mezcla] reproduccion detenida\n");
  cambiarModo(SUSPENSION);
  efectoEnEdicion = -1;
  lucecitas.escribir(0);
  pantalla.apagar();
  printf("[sistema] interruptorGeneral apagado: maquina en suspension, funciones bloqueadas\n");
  printf("[sistema] el conjuntoDeAudios se conserva (%d audios)\n", cantidadAudios);
}

void encender() {
  printf("\n[sistema] interruptorGeneral encendido\n");
  pantalla.encender();
  dibujarPresentacion();
  cambiarModo(PRESENTACION);
}

void irAGeneral() {
  cambiarModo(GENERAL);
  dibujarVistaGeneral(-1);
  printf("[sistema] %d audios guardados (%d%% libre)\n", cantidadAudios,
         (LIMITE_AUDIOS - cantidadAudios) * 100 / LIMITE_AUDIOS);
  if (cantidadAudios == 0) {
    printf("[sistema] no hay conjuntoDeAudios: solo se puede grabar\n");
  } else if (cantidadAudios >= LIMITE_AUDIOS) {
    printf("[sistema] limite de %d audios alcanzado: solo se puede reproducir\n", LIMITE_AUDIOS);
  } else {
    printf("[sistema] modos disponibles: reproduccion y grabacion\n");
  }
}

void iniciarGrabacion() {
  cambiarModo(GRABANDO);
  ultimoSegundoInformado = -1;
  printf("[grabar] microfono INMP441 activado, grabando (maximo %lu s)\n",
         (unsigned long)(GRABACION_MAXIMA_MS / 1000));
  dibujarGrabacion(0);
}

void terminarGrabacion(uint32_t duracionMs) {
  memset(&sonidoEnEdicion, 0, sizeof(sonidoEnEdicion));
  sonidoEnEdicion.duracionMs = duracionMs;
  sonidoEnEdicion.volumenEspecifico = volumenEspecifico.posicion;
  printf("[grabar] grabacion terminada: sonido de %lu.%lu s guardado temporalmente\n",
         (unsigned long)(duracionMs / 1000), (unsigned long)((duracionMs % 1000) / 100));

  efectoEnEdicion = -1;
  cambiarModo(ANIMACION_EDICION);
  dibujarVistaGeneral(-1);
  printf("[editar] zona de edicion habilitada\n");
}

void iniciarBucleEdicion() {
  cambiarModo(EDITANDO);
  inicioBucleEdicionMs = ahoraMs();
  printf("[editar] reproduciendo el sonido en bucle (%lu.%lu s, volumen especifico %d)\n",
         (unsigned long)(sonidoEnEdicion.duracionMs / 1000),
         (unsigned long)((sonidoEnEdicion.duracionMs % 1000) / 100),
         sonidoEnEdicion.volumenEspecifico);
}

// guarda el valor del slider en el efecto que se esta editando
void aplicarEfectoEnEdicion() {
  if (efectoEnEdicion < 0) return;
  int seccion = efectoEnEdicion / 3;
  sonidoEnEdicion.fxValor[efectoEnEdicion] = sliderSeccion[seccion].posicion;
  sonidoEnEdicion.fxAplicado[efectoEnEdicion] = true;
  printf("[editar] %s aplicado con valor ", NOMBRE_EFECTO[efectoEnEdicion]);
  imprimirValorEfecto(efectoEnEdicion, sonidoEnEdicion.fxValor[efectoEnEdicion]);
  printf("\n");
  efectoEnEdicion = -1;
}

void seleccionarEfecto(int i) {
  if (efectoEnEdicion == i) {
    // presionar el mismo efecto lo confirma
    aplicarEfectoEnEdicion();
    return;
  }
  // cambiar de efecto guarda el que se estaba editando
  aplicarEfectoEnEdicion();
  efectoEnEdicion = i;
  inicioModoMs = ahoraMs(); // reinicia el parpadeo
  int seccion = i / 3;
  printf("[editar] editando %s%s con el slider %s (valor actual ", NOMBRE_EFECTO[i],
         sonidoEnEdicion.fxAplicado[i] ? " (reedicion)" : "", NOMBRE_SLIDER_SECCION[seccion]);
  imprimirValorEfecto(i, sliderSeccion[seccion].posicion);
  printf(")\n");
}

void guardarAudio() {
  aplicarEfectoEnEdicion();
  sonidoEnEdicion.id = cantidadAudios + 1;
  mezclarSemilla(time_us_32());
  sonidoEnEdicion.segmento = (int)(aleatorio() % CANTIDAD_SEGMENTOS);
  conjuntoDeAudios[cantidadAudios] = sonidoEnEdicion;
  cantidadAudios++;
  audiosPorSegmento[sonidoEnEdicion.segmento]++;

  printf("[editar] se detiene el bucle del sonido\n");
  printf("[guardar] audio #%d guardado en el segmento %d (", sonidoEnEdicion.id,
         sonidoEnEdicion.segmento);
  imprimirTiempo(sonidoEnEdicion.segmento * DURACION_SEGMENTO_MS);
  printf("), duracion %lu.%lu s, volumen especifico %d\n",
         (unsigned long)(sonidoEnEdicion.duracionMs / 1000),
         (unsigned long)((sonidoEnEdicion.duracionMs % 1000) / 100),
         sonidoEnEdicion.volumenEspecifico);
  printf("[guardar] efectos: ");
  imprimirEfectos(sonidoEnEdicion);
  printf("\n");
  if (audiosPorSegmento[sonidoEnEdicion.segmento] > 1) {
    printf("[guardar] se superpone con %d audio(s) del mismo segmento\n",
           audiosPorSegmento[sonidoEnEdicion.segmento] - 1);
  }

  efectoEnEdicion = -1;
  lucecitas.escribir(0);
  irAGeneral();
}

void iniciarReproduccion() {
  cambiarModo(REPRODUCIENDO);
  segmentoActual = -1;
  vueltaActual = 0;
  printf("[mezcla] reproduciendo el conjuntoDeAudios desde 00:00 en bucle (volumen general %d)\n",
         volumenGeneral.posicion);
}

void detenerReproduccion() {
  printf("[mezcla] reproduccion detenida, vuelve al inicio\n");
  irAGeneral();
}

// informa que audios empiezan y cuales siguen sonando al inicio de un segmento
void informarSegmento(int segmento, uint32_t vuelta) {
  uint32_t inicioSegmentoMs = segmento * DURACION_SEGMENTO_MS;
  printf("[mezcla] segmento %02d (", segmento);
  imprimirTiempo(inicioSegmentoMs);
  printf(")");

  bool suena = false;
  for (int i = 0; i < cantidadAudios; i++) {
    const Audio &a = conjuntoDeAudios[i];
    if (a.segmento == segmento) {
      printf("\n          empieza audio #%d (%lu.%lu s, vol %d, ", a.id,
             (unsigned long)(a.duracionMs / 1000), (unsigned long)((a.duracionMs % 1000) / 100),
             a.volumenEspecifico);
      imprimirEfectos(a);
      printf(")");
      suena = true;
    }
  }
  for (int i = 0; i < cantidadAudios; i++) {
    const Audio &a = conjuntoDeAudios[i];
    if (a.segmento == segmento) continue;
    uint32_t inicioAudioMs = a.segmento * DURACION_SEGMENTO_MS;
    uint32_t desfase;
    if (inicioAudioMs < inicioSegmentoMs) {
      desfase = inicioSegmentoMs - inicioAudioMs;
    } else {
      // audio de la vuelta anterior que se paso del final del ciclo
      if (vuelta == 0) continue;
      desfase = DURACION_CICLO_MS - inicioAudioMs + inicioSegmentoMs;
    }
    if (desfase < a.duracionMs) {
      printf("\n          sigue sonando audio #%d (le quedan %lu.%lu s)", a.id,
             (unsigned long)((a.duracionMs - desfase) / 1000),
             (unsigned long)(((a.duracionMs - desfase) % 1000) / 100));
      suena = true;
    }
  }
  if (!suena) printf(" silencio");
  printf("\n");
}

// =====================================================================
// programa principal
// =====================================================================

int main() {
  stdio_init_all();
  sleep_ms(200);
  printf("\n=== grupo-01 | proyecto-03-v02 ===\n");
  printf("audio reemplazado por mensajes en el serial monitor\n\n");

  BotonMux::iniciarMultiplexor();
  Perilla::iniciarMultiplexor();
  lucecitas.iniciar();
  pantalla.iniciar();
  memset(audiosPorSegmento, 0, sizeof(audiosPorSegmento));

  // lectura inicial de las perillas
  volumenGeneral.leer();
  volumenEspecifico.leer();
  for (Perilla &s : sliderSeccion) s.leer();

  // estado inicial del switch (se lee varias veces para pasar el debounce)
  for (int i = 0; i < 10; i++) {
    switchEncendido.leer();
    sleep_ms(5);
  }
  if (switchEncendido.estado) {
    encender();
  } else {
    pantalla.apagar();
    printf("[sistema] maquina en suspension, mueve el switch para encender\n");
  }

  uint32_t ultimoDibujoMs = 0;

  while (true) {
    uint32_t ahora = ahoraMs();

    // -- lecturas --
    switchEncendido.leer();
    for (BotonMux &b : botonFx) b.leer();
    guardarEdicion.leer();
    botonGrabar.leer();
    botonReproducir.leer();
    volumenGeneral.leer();
    volumenEspecifico.leer();
    for (Perilla &s : sliderSeccion) s.leer();

    // -- interruptor general --
    if (switchEncendido.estado && modo == SUSPENSION) {
      encender();
    } else if (!switchEncendido.estado && modo != SUSPENSION) {
      entrarSuspension();
    }

    // -- volumen general (en cualquier modo mientras este encendida) --
    if (modo != SUSPENSION && volumenGeneral.cambio) {
      printf("[mezcla] volumen general: %d\n", volumenGeneral.posicion);
    }

    switch (modo) {

    case SUSPENSION:
      break;

    case PRESENTACION:
      if (ahora - inicioModoMs >= PRESENTACION_MS) irAGeneral();
      break;

    case GENERAL:
      if (botonGrabar.presionado) {
        if (cantidadAudios >= LIMITE_AUDIOS) {
          printf("[grabar] bloqueado: se alcanzo el limite de %d audios\n", LIMITE_AUDIOS);
        } else {
          iniciarGrabacion();
        }
      } else if (botonReproducir.presionado) {
        if (cantidadAudios == 0) {
          printf("[mezcla] bloqueado: no hay audios guardados\n");
        } else {
          iniciarReproduccion();
        }
      }
      break;

    case REPRODUCIENDO: {
      // el boton de reproduccion detiene la mezcla
      if (botonReproducir.presionado) {
        detenerReproduccion();
        break;
      }
      // grabar detiene la mezcla (vuelve al inicio) e inicia la grabacion
      if (botonGrabar.presionado) {
        if (cantidadAudios >= LIMITE_AUDIOS) {
          printf("[grabar] bloqueado: se alcanzo el limite de %d audios\n", LIMITE_AUDIOS);
        } else {
          printf("[mezcla] reproduccion detenida para grabar, vuelve al inicio\n");
          iniciarGrabacion();
          break;
        }
      }
      uint32_t transcurrido = ahora - inicioModoMs;
      uint32_t vuelta = transcurrido / DURACION_CICLO_MS;
      uint32_t posicion = transcurrido % DURACION_CICLO_MS;
      int segmento = (int)(posicion / DURACION_SEGMENTO_MS);
      if (vuelta != vueltaActual) {
        vueltaActual = vuelta;
        printf("[mezcla] fin del ciclo de 10 min, vuelve a empezar (vuelta %lu)\n",
               (unsigned long)(vuelta + 1));
      }
      if (segmento != segmentoActual) {
        segmentoActual = segmento;
        informarSegmento(segmento, vuelta);
        dibujarVistaGeneral(segmento);
      }
      break;
    }

    case GRABANDO: {
      uint32_t transcurrido = ahora - inicioModoMs;
      // la segunda pulsacion corta la grabacion, solo despues de 0,5 s
      if (botonGrabar.presionado && transcurrido >= GRABACION_MINIMA_MS) {
        printf("[grabar] grabacion detenida con el boton\n");
        terminarGrabacion(transcurrido);
        break;
      }
      if (transcurrido >= GRABACION_MAXIMA_MS) {
        dibujarGrabacion(GRABACION_MAXIMA_MS);
        printf("[grabar] se alcanzo el maximo de %lu s\n", (unsigned long)(GRABACION_MAXIMA_MS / 1000));
        terminarGrabacion(GRABACION_MAXIMA_MS);
        break;
      }
      int segundo = (int)(transcurrido / 1000);
      if (segundo != ultimoSegundoInformado) {
        ultimoSegundoInformado = segundo;
        if (segundo > 0) printf("[grabar] grabando... %d s\n", segundo);
      }
      if (ahora - ultimoDibujoMs >= 100) {
        ultimoDibujoMs = ahora;
        dibujarGrabacion(transcurrido);
      }
      break;
    }

    case ANIMACION_EDICION:
      if (ahora - inicioModoMs >= ANIMACION_EDICION_MS) iniciarBucleEdicion();
      break;

    case EDITANDO: {
      // bucle del sonido
      if (ahora - inicioBucleEdicionMs >= sonidoEnEdicion.duracionMs) {
        inicioBucleEdicionMs = ahora;
        printf("[editar] bucle: el sonido vuelve a empezar\n");
      }

      // botones de efecto
      for (int i = 0; i < CANTIDAD_EFECTOS; i++) {
        if (botonFx[i].presionado) seleccionarEfecto(i);
      }

      // slider de la seccion del efecto en edicion
      if (efectoEnEdicion >= 0) {
        Perilla &slider = sliderSeccion[efectoEnEdicion / 3];
        if (slider.cambio) {
          printf("[editar] %s: ", NOMBRE_EFECTO[efectoEnEdicion]);
          imprimirValorEfecto(efectoEnEdicion, slider.posicion);
          printf("\n");
        }
      }

      // volumen especifico
      if (volumenEspecifico.cambio) {
        sonidoEnEdicion.volumenEspecifico = volumenEspecifico.posicion;
        printf("[editar] volumen especifico: %d\n", volumenEspecifico.posicion);
      }

      if (guardarEdicion.presionado) guardarAudio();
      break;
    }
    }

    actualizarLucecitas();
    sleep_ms(2);
  }
  return 0;
}
