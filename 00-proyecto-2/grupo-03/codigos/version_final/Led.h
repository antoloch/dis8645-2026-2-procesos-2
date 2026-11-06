#ifndef LED_H
#define LED_H

#include "hardware/gpio.h"
#include "pico/stdlib.h"

// CLASE Led
// Guarda el número del pin GPIO.
// Permite encender y apagar el LED según el modo de la máquina.

class Led {
  public:

    // Atributos

    int patita;                  // Pin donde está conectado el LED.
    bool prendido = false;       // Indica si el LED está encendido o apagado.

    // Constructor:
    // Recibe el pin donde está conectado el LED.
    Led(int nuevoPin);

    // Métodos para controlar el LED.
    void prender();
    void apagar();
    void actualizar(int modoActual);
};

#endif