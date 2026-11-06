
#include "Lucecita.h"

// Recibir el número del pin.
// Guardar el número.
// Inicializar el GPIO.
// Configurar el pin como salida.
// Dejar la luz apagada inicialmente.

Lucecita::Lucecita(int nuevaPatita) {
    patita = nuevaPatita;

    gpio_init(patita);
    gpio_set_dir(patita, GPIO_OUT);

    apagar();
}

// MÉTODO prender():
//   Cambiar el estado a encendido.
//   Activar el pin GPIO.

void Lucecita::prender() {
    prendida = true;
    gpio_put(patita, true);
}

// MÉTODO apagar():
//   Cambiar el estado a apagado.
//   Desactivar el pin GPIO.

void Lucecita::apagar() {
    prendida = false;
    gpio_put(patita, false);
}
