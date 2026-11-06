// Perilla.cpp

// importar el archivo header
#include "Perilla.h"

// este prefijo
// Perilla::
// significa dentro de la clase Perilla

// constructor
Perilla::Perilla(int nuevaPatita) {

  // guardar el valor
  Perilla::patita = nuevaPatita;

  // preparar la entrada analoga
  adc_init();

  // parte la patita
  adc_gpio_init(Perilla::patita);

}

// metodos
void Perilla::leer() {

  // seleccionar canal del ADC
  // eso se hace con la patita
  // ese - 26 es porque
  // patita 26 va a ADC0
  // patita 27 va a ADC1
  adc_select_input(Perilla::patita - 26);

  // leer con ADC (0 a 4095)
  // y guardar en posicion
  Perilla::posicion = adc_read();

}

float Perilla::porcentaje() {

  // convertir posicion a un valor entre 0.0 y 1.0
  // 0.0 perilla a la izquierda
  // 1.0 perilla a la derecha
  return Perilla::posicion / 4095.0f;

}

int Perilla::paso(int cantidadPasos) {

  // dividir el giro de la perilla en pasos
  // por ejemplo con 4 pasos devuelve 0, 1, 2 o 3
  if (cantidadPasos < 1) return 0;

  // ancho de cada zona del giro
  int anchoZona = 4096 / cantidadPasos;

  // margen de histeresis
  // evita que salte de un paso a otro
  // cuando la perilla queda justo en el borde
  int margen = 80;

  // en que paso estaria la perilla ahora
  int objetivo = Perilla::posicion / anchoZona;
  if (objetivo > cantidadPasos - 1) objetivo = cantidadPasos - 1;

  if (Perilla::ultimoPaso < 0 || Perilla::ultimoPaso >= cantidadPasos) {
    // primera vez, quedarse con el paso actual
    Perilla::ultimoPaso = objetivo;
  }
  else if (objetivo != Perilla::ultimoPaso) {
    // solo cambiar de paso si la perilla
    // se salio de la zona mas el margen
    int limiteBajo = Perilla::ultimoPaso * anchoZona;
    int limiteAlto = (Perilla::ultimoPaso + 1) * anchoZona;
    if (Perilla::posicion < limiteBajo - margen || Perilla::posicion >= limiteAlto + margen) {
      Perilla::ultimoPaso = objetivo;
    }
  }

  return Perilla::ultimoPaso;

}
