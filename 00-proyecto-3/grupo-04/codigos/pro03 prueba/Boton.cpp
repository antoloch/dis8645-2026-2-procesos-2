#include "Boton.h"
#include "Config.h"
#include "pico/stdlib.h"

Boton::Boton(unsigned pin) : pin_(pin) {}

void Boton::iniciar() {
  gpio_init(pin_);
  gpio_set_dir(pin_, GPIO_IN);
  gpio_pull_up(pin_);
}

void Boton::actualizar(uint32_t ahora_ms) {
  bool lectura = !gpio_get(pin_);  // pulsado = nivel bajo
  if (lectura != ultima_lectura_) {
    ultima_lectura_ = lectura;
    t_cambio_ = ahora_ms;
  }
  if ((ahora_ms - t_cambio_) >= ANTIRREBOTE_MS && lectura != estable_) {
    estable_ = lectura;
    if (estable_) flanco_ = true;
  }
}

bool Boton::recien_presionado() {
  bool r = flanco_;
  flanco_ = false;
  return r;
}

bool Boton::presionado() const { return estable_; }