
#ifndef LUCECITA_H
#define LUCECITA_H

#include "hardware/gpio.h"
// para medir el tiempo con to_ms_since_boot() sin usar sleep_ms
#include "pico/stdlib.h"

// CLASE Lucecita
//   Guardar el número del pin GPIO.
//   Guardar si la luz está encendida o apagada.
//   Permitir encender y apagar la luz.

class Lucecita {
  public:

    // Atributos
    int patita;
    bool prendida = false;

    // para medir tiempo se usa uint32_t, igual que en BotonGrupo1
    // cuantos milisegundos dura cada estado del parpadeo
    // 250 = 250 ms prendida y 250 ms apagada
    uint32_t tiempoParpadeo = 250;
    // ultima vez que la luz cambio de estado en el parpadeo
    uint32_t ultimaLecturaParpadeo = 0;

    // Constructor:
    // Recibe el pin donde está conectada la luz.
    Lucecita(int nuevaPatita);

    // Métodos para controlar la luz.
    void prender();
    void apagar();
    void parpadeo();
};

#endif