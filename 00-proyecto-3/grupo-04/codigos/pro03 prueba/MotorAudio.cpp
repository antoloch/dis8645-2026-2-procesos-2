#include "MotorAudio.h"
#include <cstdio>

// ================= SALIDA DE HARDWARE =================
#if AUDIO_MODO == AUDIO_MODO_PWM
// ---- Wokwi: un canal PWM por pin (GP14 izquierdo, GP16 derecho) ----
#include "pico/stdlib.h"
#include "hardware/pwm.h"

static unsigned pin_de(Canal c) { return c == CANAL_IZQUIERDO ? PIN_AUDIO_IZQ : PIN_AUDIO_DER; }

void MotorAudio::salida_iniciar() {
  for (unsigned pin : {PIN_AUDIO_IZQ, PIN_AUDIO_DER}) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    unsigned slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, 125.0f);        // 125 MHz / 125 = 1 MHz
    pwm_set_wrap(slice, 999);
    pwm_set_gpio_level(pin, 0);
    pwm_set_enabled(slice, true);
  }
}

void MotorAudio::salida_tono(Canal canal, uint16_t hz, uint8_t volumen_pct) {
  unsigned pin = pin_de(canal);
  if (hz == 0 || volumen_pct == 0) {
    pwm_set_gpio_level(pin, 0);
    return;
  }
  unsigned slice = pwm_gpio_to_slice_num(pin);
  uint32_t wrap = 1000000u / hz;          // ticks por periodo
  if (wrap < 2) wrap = 2;
  if (wrap > 65535) wrap = 65535;
  uint32_t duty_pct = (uint32_t)PWM_DUTY_MAX_PCT * volumen_pct / 100;
  pwm_set_wrap(slice, wrap - 1);
  pwm_set_gpio_level(pin, wrap * duty_pct / 100);
}

void MotorAudio::salida_actualizar() {}

#else
// ---- Fisico: I2S hacia el Pico Audio Pack. ESQUELETO: aun no genera sonido ----
static uint16_t i2s_hz[2] = {0, 0};
static uint8_t i2s_vol[2] = {0, 0};

void MotorAudio::salida_iniciar() {
  printf("[AUDIO] I2S: esqueleto, aun no genera sonido (pendiente etapa fisica)\n");
}

void MotorAudio::salida_tono(Canal canal, uint16_t hz, uint8_t volumen_pct) {
  i2s_hz[canal] = hz;
  i2s_vol[canal] = volumen_pct;
}

void MotorAudio::salida_actualizar() {
  // TODO fisico: rellenar el buffer I2S con onda cuadrada de i2s_hz[0] (izq) y i2s_hz[1] (der),
  // amplitud proporcional a i2s_vol[canal].
}
#endif

// ================= MOTOR =================
MotorAudio::MotorAudio() : volumen_(VOLUMEN_INICIAL_PCT) {}

void MotorAudio::iniciar() { salida_iniciar(); }

void MotorAudio::encolar(int v, const ListaNotas& notas, Modo modo) {
  Voz& voz = voces_[v];
  if (modo == Modo::REEMPLAZAR) {
    voz.cola.clear();
    voz.activa = false;
    voz.hz = 0;
    salida_tono((Canal)v, 0, volumen_);
  }
  for (const Nota& n : notas) {
    if ((int)voz.cola.size() >= MAX_NOTAS_EN_COLA) break;
    voz.cola.push_back(n);
  }
}

void MotorAudio::reproducir(Canal canal, const ListaNotas& notas, Modo modo) {
  if (canal == CANAL_AMBOS) {
    encolar(0, notas, modo);
    encolar(1, notas, modo);
  } else {
    encolar((int)canal, notas, modo);
  }
}

void MotorAudio::actualizar(uint32_t ahora_ms) {
  for (int i = 0; i < 2; i++) {
    Voz& v = voces_[i];
    if (v.activa && (int32_t)(ahora_ms - v.fin_ms) < 0) continue;  // sigue la nota actual
    if (v.cola.empty()) {
      if (v.activa) {
        v.activa = false;
        v.hz = 0;
        salida_tono((Canal)i, 0, volumen_);
      }
      continue;
    }
    Nota n = v.cola.front();
    v.cola.pop_front();
    uint32_t base = v.activa ? v.fin_ms : ahora_ms;  // evita acumular retraso
    v.activa = true;
    v.fin_ms = base + n.duracion_ms;
    v.hz = n.frecuencia_hz;
    salida_tono((Canal)i, v.hz, volumen_);
  }
  salida_actualizar();
}

void MotorAudio::silenciar() {
  for (int i = 0; i < 2; i++) {
    voces_[i].cola.clear();
    voces_[i].activa = false;
    voces_[i].hz = 0;
    salida_tono((Canal)i, 0, volumen_);
  }
}

void MotorAudio::establecer_volumen(uint8_t porcentaje) {
  if (porcentaje > 100) porcentaje = 100;
  if (porcentaje == volumen_) return;
  volumen_ = porcentaje;
  for (int i = 0; i < 2; i++)
    if (voces_[i].activa) salida_tono((Canal)i, voces_[i].hz, volumen_);
}

// ================= SONIDOS =================
static ListaNotas pitidos(uint16_t hz, int n, uint16_t on, uint16_t off) {
  ListaNotas l;
  for (int i = 0; i < n; i++) {
    l.push_back({hz, on});
    if (i < n - 1 && off > 0) l.push_back({0, off});
  }
  return l;
}

ListaNotas MotorAudio::sonido_inicio() { return {{440, 80}, {660, 80}, {880, 80}}; }
ListaNotas MotorAudio::sonido_enemigo_presente() { return {{880, 120}}; }
ListaNotas MotorAudio::sonido_enemigo_se_acerca() { return {{1200, 50}, {0, 50}, {1200, 50}}; }
ListaNotas MotorAudio::sonido_ataque() { return {{220, 400}}; }
ListaNotas MotorAudio::sonido_obstaculo() { return pitidos(300, 4, 60, 60); }
ListaNotas MotorAudio::sonido_disparo() { return {{1500, 30}}; }
ListaNotas MotorAudio::sonido_explosion() { return {{600, 40}, {500, 40}, {400, 40}, {300, 40}, {200, 40}}; }
ListaNotas MotorAudio::sonido_impacto() { return {{300, 30}, {200, 30}, {300, 30}, {200, 30}}; }
ListaNotas MotorAudio::sonido_ultima_vida() { return pitidos(700, 3, 100, 100); }
ListaNotas MotorAudio::sonido_recompensa() { return {{523, 90}, {659, 90}, {784, 90}, {1047, 90}}; }
ListaNotas MotorAudio::sonido_pausa() { return {{600, 100}, {400, 100}}; }
ListaNotas MotorAudio::sonido_level_up() {
  return {{523, 70}, {659, 70}, {784, 70}, {1047, 70}, {1319, 140}};
}

ListaNotas MotorAudio::sonido_derrota() {
  // "trombon triste": 3 notas descendentes y una final larga con vibrato
  ListaNotas l = {{392, 300}, {370, 300}, {349, 300}};
  for (int i = 0; i < 8; i++) {
    l.push_back({330, 60});
    l.push_back({310, 60});
  }
  return l;
}

ListaNotas MotorAudio::sonido_puntaje_final(int puntos) {
  int n = puntos / 10;
  if (n > PUNTAJE_PITIDOS_MAX) n = PUNTAJE_PITIDOS_MAX;
  return pitidos(1000, n, 80, 80);
}