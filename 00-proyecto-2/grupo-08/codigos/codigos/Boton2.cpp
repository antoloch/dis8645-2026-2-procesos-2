#include "Boton2.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

// DORE 8 - Reconocimiento de gestos mediante estados, sin esperas largas.
// conserva sus gestos y su API, añade antirrebote y tiempos de liberación
// independientes para identificar pulsaciones múltiples correctamente.
// Configura el pin como entrada con pull-up; un botón presionado entrega nivel bajo.
Boton2::Boton2(int pin) : pin(pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin); // Cablear entre GPIO y GND: pulsado = nivel 0.
    presionado = !gpio_get(pin);
    lecturaBrutaAnterior = presionado;
    instanteCambioBruto = to_ms_since_boot(get_absolute_time());
}

// Actualiza el estado del botón en cada vuelta del juego, sin esperas bloqueantes.
void Boton2::leer() {
    const uint32_t ahora = to_ms_since_boot(get_absolute_time());
    // Se invierte la lectura porque el pulsador conecta el GPIO a GND.
    const bool bruto = !gpio_get(pin);

    // Guarda el momento del cambio eléctrico bruto para filtrar los rebotes del contacto.
    if (bruto != lecturaBrutaAnterior) {
        lecturaBrutaAnterior = bruto;
        instanteCambioBruto = ahora;
    }

    bool apretaron = false;
    bool soltaron = false;
    // Solo se acepta el cambio si se mantuvo estable por tiempoAntirrebote.
    if (bruto != presionado &&
        static_cast<uint32_t>(ahora - instanteCambioBruto) >= tiempoAntirrebote) {
        apretaron = !presionado && bruto;
        soltaron = presionado && !bruto;
        presionado = bruto;
    }

    switch (estado) {
        // REPOSO: espera el inicio de una pulsación.
        case EstadoBoton::REPOSO:
            if (apretaron) {
                numeroPresiones = 0;
                instanteCambio = ahora;
                estado = EstadoBoton::PRESIONADO;
            }
            break;

        // PRESIONADO: mide cuánto duró y decide si fue corta, larga o muy larga.
        case EstadoBoton::PRESIONADO:
            if (soltaron) {
                const uint32_t duracion = ahora - instanteCambio;
                if (duracion >= tiempoMuyLargo) {
                    gestoPendiente = Gesto::PULSACIONMUYLARGA;
                    numeroPresiones = 0;
                    estado = EstadoBoton::REPOSO;
                } else if (duracion >= tiempoLargo) {
                    gestoPendiente = Gesto::PULSACIONLARGA;
                    numeroPresiones = 0;
                    estado = EstadoBoton::REPOSO;
                } else {
                    ++numeroPresiones;
                    instanteUltimaLiberacion = ahora;
                    estado = EstadoBoton::ESPERANDO_PRESION;
                }
            } else if (presionado &&
                       static_cast<uint32_t>(ahora - instanteCambio) >= tiempoMuyLargo) {
                gestoPendiente = Gesto::PULSACIONMUYLARGA;
                numeroPresiones = 0;
                estado = EstadoBoton::REPOSO;
            }
            break;

        // ESPERANDO_PRESION: espera un segundo/tercer clic durante la ventana configurada.
        case EstadoBoton::ESPERANDO_PRESION:
            if (apretaron) {
                instanteCambio = ahora;
                estado = EstadoBoton::PRESIONADO;
            } else if (static_cast<uint32_t>(ahora - instanteUltimaLiberacion) >=
                       tiempoEntrePresionado) {
                switch (numeroPresiones) {
                    case 1: gestoPendiente = Gesto::PULSADO1VEZ; break;
                    case 2: gestoPendiente = Gesto::PULSADO2VECES; break;
                    case 3: gestoPendiente = Gesto::PULSADO3VECES; break;
                    default: break;
                }
                numeroPresiones = 0;
                estado = EstadoBoton::REPOSO;
            }
            break;
    }
}

// Devuelve el gesto acumulado y lo borra para que no se ejecute dos veces.
Gesto Boton2::leerGesto() {
    const Gesto gesto = gestoPendiente;
    gestoPendiente = Gesto::NADA;
    return gesto;
}
