#include "Perilla.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

// Acá preparamos el potenciómetro y el rango que vamos a usar.
Perilla::Perilla(unsigned int pin, int limite)
    : tipo_(Tipo::Potenciometro), pin_(pin), maximo_(limite) {
    valida = (pin >= 26 && pin <= 28 && limite >= 1 && limite <= 65535);
    if (!valida) return;

    // Encendemos el lector analógico una sola vez para todas las perillas.
    static bool adc_iniciado = false;
    if (!adc_iniciado) {
        adc_init();
        adc_iniciado = true;
    }
    adc_gpio_init(pin_);
}

// Acá preparamos las dos patitas del encoder para recorrer las opciones.
Perilla::Perilla(unsigned int clk, unsigned int dt, int cantidad, bool invertir)
    : tipo_(Tipo::Encoder), pin_(clk), dt_(dt),
      maximo_(cantidad), invertir_(invertir) {
    valida = (clk <= 28 && dt <= 28 && clk != dt && cantidad > 0);
    if (!valida) return;

    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);
    gpio_init(dt_);
    gpio_set_dir(dt_, GPIO_IN);
    gpio_pull_up(dt_);
    anterior_ = (int(gpio_get(pin_)) << 1) | int(gpio_get(dt_));
}

int Perilla::convertir(uint16_t valor, int limite) {
    if (limite < 1 || limite > 65535) return 0;
    if (valor > 4095) valor = 4095;
    // Multiplicamos primero para conservar los valores intermedios.
    // Sumamos 2047 para redondear al entero más cercano.
    return (static_cast<uint32_t>(valor) * limite + 2047) / 4095;
}

void Perilla::leer() {
    direccion = 0;
    cambio = false;
    if (!valida) return;

    if (tipo_ == Tipo::Encoder) {
        // Lo leemos seguido para no saltarnos partes del giro.
        procesar(gpio_get(pin_), gpio_get(dt_));
        return;
    }

    // Elegimos el canal de esta perilla y promediamos cuatro lecturas.
    adc_select_input(pin_ - 26);
    uint32_t suma = 0;
    for (int i = 0; i < 4; ++i) suma += adc_read();
    lectura = suma / 4;

    const int nueva = convertir(lectura, maximo_);
    cambio = (nueva != posicion);
    posicion = nueva;
}

void Perilla::procesar(bool clk, bool dt) {
    direccion = 0;
    cambio = false;
    if (!valida || tipo_ != Tipo::Encoder) return;

    const int actual = (int(clk) << 1) | int(dt);
    if ((actual ^ anterior_) == 3) {
        // Si las dos señales saltan juntas, esperamos un giro completo.
        acumulado_ = 0;
        anterior_ = actual;
        return;
    }

    static const int paso[16] = {
        0, -1, 1, 0,
        1, 0, 0, -1,
        -1, 0, 0, 1,
        0, 1, -1, 0
    };
    acumulado_ += paso[(anterior_ << 2) | actual];
    anterior_ = actual;

    // Contamos la opción cuando las dos señales vuelven a 1.
    // Esto depende del encoder que conectemos; hay que revisar sus clics.
    if (actual == 3) {
        if (acumulado_ == 4) direccion = 1;
        else if (acumulado_ == -4) direccion = -1;
        acumulado_ = 0;
        if (invertir_) direccion = -direccion;

        if (direccion != 0) {
            // Al pasar el final volvemos al inicio, y al retroceder hacemos lo mismo.
            posicion = (static_cast<int64_t>(posicion) + direccion + maximo_) % maximo_;
            cambio = true;
        }
    }
}
