#include "wokwi-api.h"
#include <stdlib.h>

void chip_init(void) {
  pin_init("SCK", INPUT);
  pin_init("WS", INPUT);
  pin_init("L/R", INPUT_PULLDOWN);
  pin_init("SD", OUTPUT);
}