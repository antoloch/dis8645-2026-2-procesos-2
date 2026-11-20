#include "wokwi-api.h"
#include <stdlib.h>

void chip_init(void) {
  pin_init("SD", INPUT_PULLUP);
  pin_init("A+", ANALOG);
  pin_init("A-", ANALOG);
  pin_init("SPK+", ANALOG);
  pin_init("SPK-", ANALOG);
}