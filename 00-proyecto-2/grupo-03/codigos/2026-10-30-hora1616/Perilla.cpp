// Perilla.cpp
// GRUPO 03 Y 04: implementaciones de la clase Perilla


// GRUPO 03 Y 04: importar el archivo header
#include "Perilla.h"
#include "pico/stdlib.h"


//////////////////////////////////////////////////////
// CONSTRUCTOR - GRUPO 03 Y 04
//////////////////////////////////////////////////////

Perilla::Perilla(int nuevoPin, int nuevoTipo,
                 int nuevaCantidadOpciones,
                 bool nuevoInvertir, int nuevoUmbral) {

   // GRUPO 03 Y 04: guardar el numero del GPIO
   patita = nuevoPin;
   pin = nuevoPin;

   // GRUPO 04: calcular el canal ADC
   // GP26 = canal 0, GP27 = canal 1, GP28 = canal 2
   canal = pin - 26;

   // GRUPO 03 Y 04: configurar el pin analogico
   adc_gpio_init(pin);

   // GRUPO 03: guardar el tipo de perilla
   // 0 = emociones, 1 = navegacion
   tipo = nuevoTipo;

   // GRUPO 03: guardar cantidad de opciones
   cantidadOpciones = nuevaCantidadOpciones;

   // GRUPO 03: iniciar la opcion seleccionada
   opcionSeleccionada = 0;

   // GRUPO 04: configurar el umbral
   umbral = nuevoUmbral;

   // GRUPO 04: configurar sentido de giro
   invertir = nuevoInvertir;

   // GRUPO 04: iniciar sin movimiento
   movio = false;

   // GRUPO 03 Y 04: obtener primera lectura
   valor = muestrear();

   // GRUPO 03: calcular primera opcion
   calcularOpcion();

}


//////////////////////////////////////////////////////
// MUESTREAR - GRUPO 04
//////////////////////////////////////////////////////

int Perilla::muestrear() {

   // GRUPO 04: seleccionar el canal de esta perilla
   adc_select_input(canal);

   // GRUPO 04: variable para sumar las lecturas
   int suma = 0;

   // GRUPO 04: realizar 10 lecturas
   // esto ayuda a reducir el ruido del potenciometro
   for (int i = 0; i < 10; i++) {

      // GRUPO 04: sumar cada lectura analogica
      suma = suma + adc_read();

   }

   // GRUPO 04: calcular el promedio
   int promedio = suma / 10;

   // GRUPO 04: devolver el promedio
   return promedio;

}


//////////////////////////////////////////////////////
// LEER VALOR - GRUPO 03
//////////////////////////////////////////////////////

int Perilla::leerValor() {

   // GRUPO 03: obtener una nueva lectura
   valor = muestrear();

   // GRUPO 03: devolver el valor analogico
   return valor;

}


//////////////////////////////////////////////////////
// DEFINIR OPCIONES - GRUPO 03
//////////////////////////////////////////////////////

void Perilla::definirOpciones(int numeroOpciones) {

   // GRUPO 03: guardar la cantidad de opciones
   cantidadOpciones = numeroOpciones;

   // GRUPO 03: actualizar la opcion seleccionada
   calcularOpcion();

}


//////////////////////////////////////////////////////
// CALCULAR OPCION - GRUPO 03
//////////////////////////////////////////////////////

void Perilla::calcularOpcion() {

   // GRUPO 03: comprobar que existan opciones
   if (cantidadOpciones <= 0) {

      opcionSeleccionada = 0;
      return;

   }

   // GRUPO 03: calcular la opcion segun el rango
   // divide los valores de 0 a 4095 en partes iguales
   opcionSeleccionada = posicion(cantidadOpciones);

}


//////////////////////////////////////////////////////
// LEER - GRUPO 04
//////////////////////////////////////////////////////

void Perilla::leer() {

   // GRUPO 04: reiniciar movimiento
   movio = false;

   // GRUPO 04: obtener una nueva lectura
   int nuevoValor = muestrear();

   // GRUPO 04: calcular la diferencia
   int diferencia = nuevoValor - valor;

   // GRUPO 04: convertir diferencia a positiva
   if (diferencia < 0) {

      diferencia = -diferencia;

   }

   // GRUPO 04: comprobar si supera el umbral
   if (diferencia > umbral) {

      // GRUPO 04: actualizar valor
      valor = nuevoValor;

      // GRUPO 04: indicar movimiento
      movio = true;

   }

}


//////////////////////////////////////////////////////
// POSICION - GRUPO 04
//////////////////////////////////////////////////////

int Perilla::posicion(int cantidad) {

   // GRUPO 04: comprobar que existan opciones
   if (cantidad <= 0) {

      return 0;

   }

   // GRUPO 04: guardar la lectura actual
   int lectura = valor;

   // GRUPO 04: comprobar si se invierte el sentido
   if (invertir == true) {

      lectura = 4095 - lectura;

   }

   // GRUPO 04: dividir el recorrido en partes iguales
   int opcion = (lectura * cantidad) / 4096;

   // GRUPO 04: evitar valores menores a cero
   if (opcion < 0) {

      opcion = 0;

   }

   // GRUPO 04: evitar superar el limite
   if (opcion >= cantidad) {

      opcion = cantidad - 1;

   }

   // GRUPO 04: devolver la posicion seleccionada
   return opcion;

}