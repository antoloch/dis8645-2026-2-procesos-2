#pragma once
#include <cstdint>

// DORE 8 - Clase Perilla reutilizada y adaptada para GP26/GP27.
// 11 · CLASE Perilla (reutilizada del proyecto anterior)
// Convierte la señal analógica del potenciómetro en movimientos discretos.
class Perilla {
public:
    explicit Perilla(unsigned int pin);
    void iniciar() const;
    uint16_t leer() const; // ADC 0 a 4095.
    int direccion() const; // -1, 0, +1: giro relativo a la zona central.
private:
    unsigned int pin_;
};
