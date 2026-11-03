
#ifndef LUCECITA_H
#define LUCECITA_H

#include "hardware/gpio.h"

// CLASE Lucecita
//   Guardar el número del pin GPIO.
//   Guardar si la luz está encendida o apagada.
//   Permitir encender y apagar la luz.

class Lucecita {
  public:

    // Atributos
    int patita;
    bool prendida = false;

    // Constructor:
    // Recibe el pin donde está conectada la luz.
    Lucecita(int nuevaPatita);

    // Métodos para controlar la luz.
    void prender();
    void apagar();
};

#endif

