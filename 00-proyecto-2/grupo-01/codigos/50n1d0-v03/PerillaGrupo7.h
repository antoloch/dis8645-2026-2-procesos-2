// -- Cambios respecto a clases/PerillaGrupo7 --
// 1. el guard PERILLA_H pasa a PERILLA_GRUPO7_H
//    para no chocar con las perillas de otros grupos
// 2. la clase Perilla pasa a llamarse PerillaGrupo7, igual que el archivo

#ifndef PERILLA_GRUPO7_H
#define PERILLA_GRUPO7_H

#include <cstdint>

// Acá usamos la misma clase para las dos perillas:
// - Potenciometro: lectura analogica que se convierte a un rango (ej. 0 a 100).
// - Encoder: direccion del giro y seleccion circular
// Referencia para la parte analogica: https://github.com/piruetasxyz/Perilla
// La usamos con Raspberry Pi Pico y Pico SDK.
class PerillaGrupo7 {
public:
    int posicion = 0;       // Potenciometro: valor convertido. Encoder: opcion de 0 a cantidad-1.
    int direccion = 0;      // Encoder: -1, 0 o +1 en la ultima lectura.
    uint16_t lectura = 0;  // Potenciometro: lectura ADC promediada (0 a 4095).
    bool cambio = false;   // Verdadero si cambio la posicion en la ultima lectura.
    bool valida = false;   // Verdadero si la configuracion tiene sentido.

    // Potenciometro: GPIO analogico (26, 27 o 28) y limite del rango de salida.
    PerillaGrupo7(unsigned int pin, int limite);

    // Encoder: dos GPIO digitales, numero de opciones y sentido invertido opcional.
    // Ejemplo: PerillaGrupo7 sonidos(14, 15, 5);
    PerillaGrupo7(unsigned int clk, unsigned int dt, int cantidad, bool invertir = false);

    void leer();
    void procesar(bool clk, bool dt);  // Revisamos un giro con las dos señales.
    static int convertir(uint16_t valor, int limite);

private:
    enum class Tipo { Potenciometro, Encoder };
    Tipo tipo_;
    unsigned int pin_;
    unsigned int dt_ = 0;
    int maximo_;
    int anterior_ = 3;
    int acumulado_ = 0;
    bool invertir_ = false;
};

#endif
