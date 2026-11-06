#include "Perilla.h"
#include "hardware/adc.h"

// Inicializa y configura el pin para comenzar a utilizar la perilla.

Perilla::Perilla(unsigned int pin) : pin_(pin) {}
void Perilla::iniciar() const { adc_gpio_init(pin_); }

// Lee el valor analógico de la perilla.
// Retorna un valor entre 0 y 4095, según la posición del potenciómetro.

// Determina la dirección del giro de la perilla respecto a su posición central.
// Retorna:
// -1 = giro hacia un lado.
//  0 = posición central.
// +1 = giro hacia el otro lado.

uint16_t Perilla::leer() const {
    adc_select_input(pin_ - 26); // GPIO26 => ADC0; GPIO27 => ADC1.
    return adc_read();
}
int Perilla::direccion() const {
    const uint16_t valor = leer();
    if(valor < 1660) return -1;
    if(valor > 2450) return 1;
    return 0;
}
