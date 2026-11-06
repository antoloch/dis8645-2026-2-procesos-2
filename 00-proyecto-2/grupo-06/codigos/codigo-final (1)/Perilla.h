#ifndef PERILLA_H
#define PERILLA_H

// esto lo agregamos para GPIO
// general purpose input output
#include "hardware/gpio.h"
// la perilla es mas sofisticada
// necesita ADC
// analog to digital conversion
#include "hardware/adc.h"


// Perilla.h
// declaraciones de la clase Perilla

// definir clase Perilla
class Perilla {

  // todo publico
  // nada de andar privatizando
  public:

  // atributos
  int posicion = 0;       // 0 a 4095
  int patita;
  int ultimoPaso = -1;    // para recordar el paso anterior (histeresis)

  // constructor
  Perilla(int nuevaPatita);

  // metodos
  void leer();
  float porcentaje();
  int paso(int cantidadPasos);

};

#endif
