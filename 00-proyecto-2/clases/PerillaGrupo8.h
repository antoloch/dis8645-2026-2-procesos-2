#pragma once
#include <cstdint>

// Clase reutilizable para controles analogicos del RP2040 (Pico SDK).
class Perilla {
public:
// Determina la dirección del giro de la perilla respecto a su posición central.
// Retorna:
// -1 = giro hacia un lado.
//  0 = posición central.
// +1 = giro hacia el otro lado.
    explicit Perilla(unsigned int pin);
    void iniciar() const;
    uint16_t leer() const; // ADC 0 a 4095.
    int direccion() const; // -1, 0, +1: giro relativo a la zona central.
    
// Variable privada que almacena el número del pin donde está conectada la perilla.
private:
    unsigned int pin_;
};

};

#endif
