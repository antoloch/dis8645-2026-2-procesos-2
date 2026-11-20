#include "wokwi-api.h"
#include <stdio.h>

static pin_t pin_in;
static pin_t pin_out;

static void entrada_cambio(void *user_data, pin_t pin, uint32_t value) {
  pin_write(pin_out, value);
}

void chip_init(void) {
  pin_in = pin_init("IN", INPUT);
  pin_out = pin_init("OUT", OUTPUT);

  pin_watch_config_t config = {
    .edge = BOTH,
    .pin_change = entrada_cambio,
    .user_data = NULL,
  };
  pin_watch(pin_in, &config);

  printf("PAM8403 listo\n");
}