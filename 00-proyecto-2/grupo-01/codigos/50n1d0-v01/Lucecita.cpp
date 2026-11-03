
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

// metodo prender():
//   Cambiar el estado a encendido.
//   Activar el pin GPIO.

void Lucecita::prender() {
    prendida = true;
    gpio_put(patita, true);
}

// metodo apagar():
//   Cambiar el estado a apagado.
//   Desactivar el pin GPIO.

void Lucecita::apagar() {
    prendida = false;
    gpio_put(patita, false);
}

// Metodo parpadeo (): 
// se prende luz pasa tiempo
// de determinado tiempo se apaga 
// se repite la accion
// no ocupar el  sleep_ms
//   Se llama en cada vuelta del while del main.
//   Revisa cuanto tiempo paso desde el ultimo cambio.
//   Si ya paso tiempoParpadeo, invierte la luz y guarda el momento.
//   Si no ha pasado, no hace nada y deja seguir al resto del programa.

void Lucecita::parpadeo() {
    // leemos el tiempo transcurrido desde que se encendio el microcontrolador
    // tiempoActual almacena el tiempo en milisegundos
    uint32_t tiempoActual = to_ms_since_boot(get_absolute_time());

    // si todavia no pasa tiempoParpadeo sale sin hacer nada
    if (tiempoActual - ultimaLecturaParpadeo < tiempoParpadeo) {
        return;
    }

    // invierte el estado de la luz
    if (prendida) {
        apagar();
    } else {
        prender();
    }

    // guarda el momento de este cambio para medir el siguiente
    ultimaLecturaParpadeo = tiempoActual;
}


