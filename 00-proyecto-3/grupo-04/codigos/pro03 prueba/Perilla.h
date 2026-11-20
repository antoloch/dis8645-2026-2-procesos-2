#pragma once
#include <cstdint>

// Perilla de volumen (potenciometro en ADC)
class Perilla {
 public:
  void iniciar();
  void actualizar(uint32_t ahora_ms);
  uint8_t porcentaje() const;   // 0..100
 private:
  uint32_t ultimo_ms_ = 0;
  uint8_t porcentaje_ = 30;
};