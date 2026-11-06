#include <stdio.h>
#include "pico/stdlib.h"
#include "BotonGrupo1.h"
#include "Led.h"

int main() {
  stdio_init_all();

  BotonGrupo1 interruptor(15);
  Led lucesota(14);

  while (true) {
    // Lee el estado físico actual del pin
    interruptor.leer();
    
    // Procesa el estado para alternar la variable estadoMantenerPresionado
    interruptor.mantenerPresionado();

    // Evalúa el estado alterno (modo switch)
    if (interruptor.estadoMantenerPresionado) {
      lucesota.encenderLed();
    } else {
      lucesota.apagarLed();
    }
  }
}