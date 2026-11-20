#include "Dificultad.h"
#include "Config.h"

void Dificultad::reiniciar() {
  nivel_ = 1;
  acumulado_ms_ = 0;
}

void Dificultad::avanzar_reloj(uint32_t ahora_ms, bool jugando) {
  uint32_t dt = ahora_ms - ultimo_ms_;
  ultimo_ms_ = ahora_ms;
  if (jugando) acumulado_ms_ += dt;   // en pausa no cuenta
}

bool Dificultad::actualizar_nivel() {
  int nuevo = 1 + (int)(acumulado_ms_ / NIVEL_DURACION_MS);
  if (nuevo > nivel_) {
    nivel_ = nuevo;
    return true;
  }
  return false;
}

static uint32_t bajar(uint32_t inicial, uint32_t minimo, uint32_t baja, int nivel) {
  uint32_t resta = baja * (uint32_t)(nivel - 1);
  if (resta >= inicial - minimo) return minimo;
  return inicial - resta;
}

uint32_t Dificultad::intervalo_aparicion_ms() const {
  return bajar(APARICION_INICIAL_MS, APARICION_MIN_MS, APARICION_BAJA_MS, nivel_);
}

uint32_t Dificultad::tiempo_acercamiento_ms() const {
  return bajar(ACERCA_INICIAL_MS, ACERCA_MIN_MS, ACERCA_BAJA_MS, nivel_);
}