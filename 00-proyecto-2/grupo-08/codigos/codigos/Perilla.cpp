#include "Perilla.h"
#include "hardware/adc.h"

// DORE 8 - Lectura analógica y dirección del movimiento.
// Al crear cada objeto Perilla se guarda su GPIO de lectura.
Perilla::Perilla(unsigned int pin) : pin_(pin) {}
// Habilita el pin para que el conversor ADC pueda leer la tensión.
void Perilla::iniciar() const { adc_gpio_init(pin_); }
// Selecciona ADC0 para GP26 o ADC1 para GP27 y devuelve el valor de 12 bits (0..4095).
uint16_t Perilla::leer() const {
    adc_select_input(pin_ - 26); // GPIO26 => ADC0; GPIO27 => ADC1.
    return adc_read();
}
// Usa una zona neutra central: devuelve -1, 0 o 1 según la posición del potenciómetro.
int Perilla::direccion() const {
    const uint16_t valor = leer();
    if(valor < 1660) return -1;
    if(valor > 2450) return 1;
    return 0;
}
