#include <cstdio>
#include "pico/stdlib.h"
#include "Juego.h"

int main() {
  stdio_init_all();   // printf por USB (monitor serial)
  Juego juego;
  juego.iniciar();
  while (true) {
    juego.actualizar();
  }
  return 0;
}