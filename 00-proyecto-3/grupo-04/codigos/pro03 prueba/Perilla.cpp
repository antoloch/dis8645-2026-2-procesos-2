#include "Perilla.h"
#include "Config.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

void Perilla::iniciar() {
  adc_init();
  adc_gpio_init(PIN_PERILLA);
}

void Perilla::actualizar(uint32_t ahora_ms) {
  if ((ahora_ms - ultimo_ms_) < 30) return;
  ultimo_ms_ = ahora_ms;
  adc_select_input(PIN_PERILLA - 26);
  uint32_t suma = 0;
  for (int i = 0; i < 4; i++) suma += adc_read();
  int pct = (int)((suma / 4) * 100 / 4095);
  int dif = pct - (int)porcentaje_;
  if (dif < 0) dif = -dif;
  if (dif >= 2 || pct == 0 || pct == 100) porcentaje_ = (uint8_t)pct;  // evita temblor
}

uint8_t Perilla::porcentaje() const { return porcentaje_; }