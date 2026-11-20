#include "Pantalla.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdio>
#include <cstring>

namespace {
constexpr uint8_t BIT_RS = 0x01, BIT_EN = 0x04, BIT_LUZ = 0x08;
constexpr uint8_t DIR_FILA[4] = {0x00, 0x40, 0x14, 0x54};
constexpr uint32_t DUR_INICIO_MS = 2000, DUR_LEVEL_UP_MS = 1800;
}

// ---------- bajo nivel ----------
void Pantalla::escribir_nibble(uint8_t nibble) {
  uint8_t base = (uint8_t)((nibble << 4) | (luz_ ? BIT_LUZ : 0));
  uint8_t b[2] = {(uint8_t)(base | BIT_EN), base};
  i2c_write_timeout_us(I2C_PUERTO, direccion_, b, 2, false, 3000);
}

void Pantalla::enviar(uint8_t valor, bool es_dato) {
  uint8_t ctrl = (uint8_t)((luz_ ? BIT_LUZ : 0) | (es_dato ? BIT_RS : 0));
  uint8_t hi = valor & 0xF0, lo = (uint8_t)((valor << 4) & 0xF0);
  uint8_t b[4] = {(uint8_t)(hi | ctrl | BIT_EN), (uint8_t)(hi | ctrl),
                  (uint8_t)(lo | ctrl | BIT_EN), (uint8_t)(lo | ctrl)};
  i2c_write_timeout_us(I2C_PUERTO, direccion_, b, 4, false, 3000);
}

void Pantalla::aplicar_luz(bool encendida) {
  luz_ = encendida;
  if (!direccion_) return;
  uint8_t b = luz_ ? BIT_LUZ : 0;
  i2c_write_timeout_us(I2C_PUERTO, direccion_, &b, 1, false, 3000);
}

bool Pantalla::iniciar() {
  direccion_ = 0;
  for (uint8_t d : {LCD_DIRECCION_1, LCD_DIRECCION_2}) {
    uint8_t basura;
    if (i2c_read_timeout_us(I2C_PUERTO, d, &basura, 1, false, 5000) >= 0) { direccion_ = d; break; }
  }
  if (!direccion_) {
    printf("[LCD] no responde en 0x%02X ni 0x%02X\n", LCD_DIRECCION_1, LCD_DIRECCION_2);
    return false;
  }
  printf("[LCD] encontrada en 0x%02X\n", direccion_);

  // Secuencia de inicio (bloquea unos ms, SOLO al arrancar)
  sleep_ms(50);
  escribir_nibble(0x3); sleep_ms(5);
  escribir_nibble(0x3); sleep_ms(1);
  escribir_nibble(0x3); sleep_ms(1);
  escribir_nibble(0x2); sleep_ms(1);   // modo 4 bits
  comando(0x28);                       // 2 lineas logicas, 5x8
  comando(0x08);                       // display off
  comando(0x01); sleep_ms(3);          // limpiar
  comando(0x06);                       // entrada incremental
  // caracter personalizado 1 = corazon
  const uint8_t corazon[8] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};
  comando(0x40 | (LCD_CHAR_CORAZON << 3));
  for (uint8_t fila : corazon) dato(fila);
  comando(0x80);
  comando(0x0C);                       // display on, sin cursor

  memset(deseado_, ' ', sizeof(deseado_));
  memset(mostrado_, ' ', sizeof(mostrado_));
  cursor_ = -1;
  return true;
}

void Pantalla::volcar() {
  int enviados = 0;
  for (int f = 0; f < LCD_FILAS; f++) {
    for (int c = 0; c < LCD_COLUMNAS; c++) {
      if (deseado_[f][c] == mostrado_[f][c]) continue;
      if (enviados >= LCD_CARACTERES_POR_CICLO) return;
      int idx = f * LCD_COLUMNAS + c;
      if (cursor_ != idx) comando((uint8_t)(0x80 | (DIR_FILA[f] + c)));
      dato((uint8_t)deseado_[f][c]);
      mostrado_[f][c] = deseado_[f][c];
      cursor_ = (c == LCD_COLUMNAS - 1) ? -1 : idx + 1;
      enviados++;
    }
  }
}

// ---------- dibujo en buffer ----------
void Pantalla::limpiar_buffer() { memset(deseado_, ' ', sizeof(deseado_)); }

void Pantalla::poner(int fila, int col, const char* texto) {
  if (fila < 0 || fila >= LCD_FILAS) return;
  for (int i = 0; texto[i] && col + i < LCD_COLUMNAS; i++)
    if (col + i >= 0) deseado_[fila][col + i] = texto[i];
}

void Pantalla::centrar(int fila, const char* texto) {
  int len = (int)strlen(texto);
  poner(fila, (LCD_COLUMNAS - len) / 2, texto);
}

void Pantalla::poner_vidas(int fila) {
  poner(fila, 0, "VIDAS:");
  for (int i = 0; i < hud_.vidas && 7 + i * 2 < LCD_COLUMNAS; i++)
    deseado_[fila][7 + i * 2] = LCD_CHAR_CORAZON;
}

void Pantalla::dibujar_hud() {
  char t[24];
  poner_vidas(0);
  snprintf(t, sizeof(t), "NIVEL: %d", hud_.nivel);   poner(1, 0, t);
  snprintf(t, sizeof(t), "PUNTOS: %d", hud_.puntos); poner(2, 0, t);
  poner(3, 0, hud_.sector == IZQUIERDA ? "NAVE: << IZQUIERDA" : "NAVE: DERECHA >>");
}

void Pantalla::dibujar(uint32_t ahora) {
  limpiar_buffer();
  char t[24];
  uint32_t t_modo = ahora - t_modo_;
  uint32_t t_anim = ahora - t_anim_;

  switch (modo_) {
    case ModoPantalla::APAGADA:
      break;
    case ModoPantalla::ESPERA:
      centrar(0, "NAVE DE SONIDO");
      centrar(2, "Pulsa INICIO");
      centrar(3, "Usa audifonos");
      break;
    case ModoPantalla::JUEGO:
      if (anim_ == Animacion::INICIO && t_anim < DUR_INICIO_MS) {
        centrar(0, "A JUGAR!");
        centrar(1, "NAVE LISTA");
        int n = (int)(t_anim * 18 / DUR_INICIO_MS);
        poner(2, 0, "[");
        for (int i = 0; i < 18; i++) deseado_[2][1 + i] = (i < n) ? '=' : ' ';
        poner(2, 19, "]");
        poner_vidas(3);
      } else if (anim_ == Animacion::LEVEL_UP && t_anim < DUR_LEVEL_UP_MS) {
        bool alt = ((t_anim / 200) % 2) == 0;
        centrar(0, alt ? "*** LEVEL UP! ***" : ">>> LEVEL UP! <<<");
        snprintf(t, sizeof(t), "NIVEL %d", hud_.nivel);
        centrar(1, t);
        centrar(2, "Sigue asi!");
        poner_vidas(3);
      } else {
        dibujar_hud();
      }
      break;
    case ModoPantalla::PAUSA:
      centrar(0, "== PAUSA ==");
      snprintf(t, sizeof(t), "NIVEL: %d", hud_.nivel);   poner(1, 0, t);
      snprintf(t, sizeof(t), "PUNTOS: %d", hud_.puntos); poner(2, 0, t);
      centrar(3, "INICIO = seguir");
      break;
    case ModoPantalla::GAME_OVER: {
      bool visible = (t_modo >= 3000) || ((t_modo / 300) % 2 == 0);  // parpadea 3 s
      if (visible) centrar(0, "*** GAME OVER ***");
      snprintf(t, sizeof(t), "Puntos: %d", hud_.puntos); poner(1, 0, t);
      snprintf(t, sizeof(t), "Nivel: %d", hud_.nivel);   poner(2, 0, t);
      poner(3, 0, "INICIO = reintentar");
      break;
    }
  }
}

// ---------- API ----------
void Pantalla::establecer_modo(ModoPantalla modo, uint32_t ahora_ms) {
  if (modo == modo_) return;
  modo_ = modo;
  t_modo_ = ahora_ms;
  if (modo != ModoPantalla::JUEGO) anim_ = Animacion::NINGUNA;
  if (direccion_) aplicar_luz(modo != ModoPantalla::APAGADA);
}

void Pantalla::lanzar_animacion(Animacion a, uint32_t ahora_ms) {
  anim_ = a;
  t_anim_ = ahora_ms;
}

void Pantalla::actualizar_hud(const DatosHud& hud) { hud_ = hud; }

void Pantalla::actualizar(uint32_t ahora_ms) {
  if (!direccion_) return;
  dibujar(ahora_ms);
  volcar();
}