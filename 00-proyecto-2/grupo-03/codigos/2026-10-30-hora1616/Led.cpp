#include "Led.h"

// Recibir el número del pin.
// Guardar el número.
// Inicializar el GPIO.
// Configurar el pin como salida.
// Dejar el LED apagado inicialmente.

Led::Led(int nuevoPin) {
    patita = nuevoPin;

    gpio_init(patita);
    gpio_set_dir(patita, GPIO_OUT);

    apagar();
}

// Metodo prender():
// Cambiar el estado a encendido.
// Activar el pin GPIO.

void Led::prender() {
    prendido = true;
    gpio_put(patita, true);
}

// Metodo apagar():
// Cambiar el estado a apagado.
// Desactivar el pin GPIO.

void Led::apagar() {
    prendido = false;
    gpio_put(patita, false);
}

// Metodo actualizar():
// Revisar el modo actual de la máquina.
// Si está en el modo 03 (juego), encender el LED.
// Si está en otro modo, apagarlo.

void Led::actualizar(int modoActual) {

    if (modoActual == 2) {
        prender();
    } 
    else {
        apagar();
    }
}