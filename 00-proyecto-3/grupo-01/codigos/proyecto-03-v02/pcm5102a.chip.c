#include "wokwi-api.h"
#include <stdlib.h>

void chip_init(void) {
  pin_init("SCK", INPUT);
  pin_init("BCK", INPUT);
  pin_init("DIN", INPUT);
  pin_init("LCK", INPUT);
  pin_init("LOUT", ANALOG);
  pin_init("ROUT", ANALOG);
}