#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t pin_z;
  pin_t pin_e;
  pin_t pin_s0;
  pin_t pin_s1;
  pin_t pin_s2;
  pin_t channels[8];
} chip_state_t;

static void update_mux(void *user_data) {
  chip_state_t *chip = (chip_state_t *)user_data;

  // Si E (Inhibit) está en HIGH, el multiplexor está deshabilitado
  if (pin_read(chip->pin_e) == HIGH) {
    pin_dac_write(chip->pin_z, 0.0f);
    return;
  }

  uint8_t s0 = pin_read(chip->pin_s0);
  uint8_t s1 = pin_read(chip->pin_s1);
  uint8_t s2 = pin_read(chip->pin_s2);
  uint8_t channel = (s2 << 2) | (s1 << 1) | s0;

  float voltage = pin_adc_read(chip->channels[channel]);
  pin_dac_write(chip->pin_z, voltage);
}

static void on_pin_change(void *user_data, pin_t pin, uint32_t value) {
  update_mux(user_data);
}

void chip_init(void) {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->pin_z  = pin_init("Z", ANALOG);
  chip->pin_e  = pin_init("E", INPUT_PULLDOWN);
  chip->pin_s0 = pin_init("S0", INPUT_PULLDOWN);
  chip->pin_s1 = pin_init("S1", INPUT_PULLDOWN);
  chip->pin_s2 = pin_init("S2", INPUT_PULLDOWN);

  const char *ch_names[8] = {"Y0", "Y1", "Y2", "Y3", "Y4", "Y5", "Y6", "Y7"};
  for (int i = 0; i < 8; i++) {
    chip->channels[i] = pin_init(ch_names[i], ANALOG);
  }

  // Actualización inmediata cuando cambian los pines de selección o Enable
  const pin_watch_config_t watch_config = {
    .edge = BOTH,
    .pin_change = on_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->pin_e, &watch_config);
  pin_watch(chip->pin_s0, &watch_config);
  pin_watch(chip->pin_s1, &watch_config);
  pin_watch(chip->pin_s2, &watch_config);

  // Timer de 1 ms para reflejar cambios continuos al girar los potenciómetros
  const timer_config_t timer_config = {
    .callback = update_mux,
    .user_data = chip,
  };
  timer_t timer = timer_init(&timer_config);
  timer_start(timer, 1000, true);
}