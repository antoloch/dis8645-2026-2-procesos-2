// ============================================================
// Perilla.h
// Vamos a definir la clase Perilla, que sirve para las dos perillas
// del proyecto:
//  - Perilla 1: velocidad de bajada de las luces.
//  - Perilla 2: moverse entre los 4 caminos del juego.
// ============================================================

// Esqueleto base: evita que este archivo se cargue dos veces.
#ifndef PERILLA_H
#define PERILLA_H

// Biblioteca que nos deja interactuar con los pines físicos de la raspberry.
#include "hardware/gpio.h"

// La perilla es más sofisticada que un botón: necesita ADC
// (analog to digital conversion), que traduce el voltaje de la
// perilla a un número que el programa puede entender.
#include "hardware/adc.h"

// ============================================================

// Aquí empieza la clase Perilla
class Perilla {

  public: // nada privado

// ============================================================

    // ATRIBUTOS
    // Los atributos son lo que cada perilla necesita saber.

    // Guarda el número que entrega el ADC: 0 si la perilla está
    // girada al mínimo y 4095 si está girada al máximo.
    int posicion = 0;

    // Guarda el pin (la patita) de la raspberry donde está conectada
    // la perilla. Debe ser GP26, GP27 o GP28, que son los que tienen ADC.
    int patita;

    // Recuerda el paso (el camino) en el que estaba la perilla la
    // última vez. Sirve para que no "parpadee" entre dos caminos
    // cuando la perilla está justo en el borde (histéresis).
    // Empieza en -1, que significa "todavía no hay paso anterior".
    int ultimoPaso = -1;

// ============================================================

    // CONSTRUCTOR
    // Recibe el pin donde está conectada la perilla y deja el ADC listo.
    Perilla(int nuevaPatita);

// ============================================================

    // MÉTODOS
    // Acciones que sabe hacer la perilla.

    // Lee la perilla en este momento y guarda el número (0 a 4095)
    // en posicion. Se llama muchísimas veces por segundo desde el
    // ciclo principal.
    void leer();

    // Devuelve qué tan girada está la perilla como porcentaje,
    // de 0 (al mínimo) a 100 (al máximo). Es la que usa la perilla 1,
    // porque la velocidad cambia de forma suave.
    float porcentaje();

    // Divide el giro en partes iguales y devuelve en cuál está,
    // de 1 a cantidadPasos. paso(4) da el camino 1, 2, 3 o 4. Es la
    // que usa la perilla 2. Usa ultimoPaso para no parpadear.
    int paso(int cantidadPasos);

};

#endif