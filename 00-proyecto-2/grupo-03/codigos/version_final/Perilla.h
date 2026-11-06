#ifndef PERILLA_H
#define PERILLA_H

// Perilla.h
// declaraciones de la clase Perilla
// GRUPO 03 Y 04: permite utilizar potenciometros
// conectados a las entradas analogicas de la Raspberry Pi Pico

#include "hardware/adc.h"


// GRUPO 03 Y 04: definir clase Perilla
class Perilla {

  // GRUPO 03 Y 04: todo publico
  // nada de andar privatizando
  public:


  //////////////////////////////////////////////////////
  // ATRIBUTOS
  //////////////////////////////////////////////////////

  // GRUPO 03 Y 04: identifica el pin donde esta conectada
  int patita;

  // GRUPO 03: identifica la entrada analoga
  // representa el mismo GPIO que patita
  int pin;

  // GRUPO 04: identifica el canal ADC pines físicos
  // que corresponde a la patita
  int canal;

  // GRUPO 03 Y 04: guarda el ultimo valor analogico
  // de la perilla entre 0 y 4095
  int valor = 0;

  // GRUPO 03: define cuantas opciones
  // puede seleccionar la perilla
  int cantidadOpciones = 0;

  // GRUPO 03: guarda la opcion seleccionada
  // segun el rango del potenciometro
  int opcionSeleccionada = 0;

  // GRUPO 03: identifica el tipo de perilla
  // 0 = emociones
  // 1 = navegacion entre modos de pantalla
  int tipo = 0;

  // GRUPO 04: indica si la perilla se movio
  // vuelve a false en cada nueva lectura
  bool movio = false;

  // GRUPO 04: define el cambio minimo
  // para considerar que la perilla se movio
  int umbral = 30;

  // GRUPO 04: permite invertir
  // el sentido de la perilla
  bool invertir = false;


  //////////////////////////////////////////////////////
  // CONSTRUCTOR
  //////////////////////////////////////////////////////

  // GRUPO 03 Y 04: recibe el pin de la perilla
  // GRUPO 03: recibe el tipo y cantidad de opciones
  // GRUPO 04: permite configurar inversion y umbral
  Perilla(int nuevoPin, int nuevoTipo = 0,
          int nuevaCantidadOpciones = 0,
          bool nuevoInvertir = false,
          int nuevoUmbral = 30);


  //////////////////////////////////////////////////////
  // METODOS COMPARTIDOS
  //////////////////////////////////////////////////////

  // GRUPO 04: realiza varias lecturas
  // del potenciometro y devuelve el promedio
  int muestrear();

  // GRUPO 03: obtiene y guarda
  // el valor analogico del potenciometro
  int leerValor();

  // GRUPO 04: convierte el valor analogico
  // en una posicion de 0 a cantidad - 1
  int posicion(int cantidad);


  //////////////////////////////////////////////////////
  // METODOS GRUPO 03
  //////////////////////////////////////////////////////

  // GRUPO 03: establece cuantas opciones
  // puede seleccionar la perilla
  void definirOpciones(int numeroOpciones);

  // GRUPO 03: convierte el valor analogico
  // en una opcion segun los rangos definidos
  // guarda el resultado en opcionSeleccionada
  void calcularOpcion();

  //////////////////////////////////////////////////////
  // METODOS GRUPO 04
  //////////////////////////////////////////////////////

  // GRUPO 04: revisa si la perilla se movio
  // compara la nueva lectura con el valor anterior
  // actualiza movio cuando supera el umbral
  void leer();

};

#endif