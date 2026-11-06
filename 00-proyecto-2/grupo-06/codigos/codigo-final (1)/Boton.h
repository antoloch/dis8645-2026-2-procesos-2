#ifndef BOTON_H
#define BOTON_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Clase genérica para un botón físico en la Pico.
// Sirve para los dos botones del juego: ella solo "informa" qué pasó
// (toque, mantenido, presionado); el juego decide qué hacer con eso.
//
// Conexión: un lado del botón al GPIO y el otro a GND (usa pull-up interno).
class Boton
{
  public:
    // constructor: recibe el número de GPIO (la "patita") y,
    // opcionalmente, cuánto hay que mantener para que cuente como "largo"
    Boton(uint patita, uint32_t umbralLargoMs = 2000);

    // configura el pin; se llama UNA vez en el setup
    void iniciar();

    // lee el botón y actualiza todo; se llama en CADA vuelta del loop
    void actualizar();

    // ---- para el botón 2 (mantener = personaje vuela) ----
    // true mientras el botón esté apretado
    bool estaPresionado() const;

    // ---- para el botón 1 (encender/apagar y seleccionar) ----
    // true UNA sola vez cuando se suelta tras un toque corto (< umbral)
    bool huboToque();
    // true UNA sola vez cuando se completa el tiempo de mantenido (>= umbral)
    bool huboMantenido();

    // cuánto lleva apretado (ms); 0 si no está apretado
    uint32_t tiempoPresionado() const;

    uint patita;
    uint32_t umbralLargoMs;

    bool presionado;          // estado estable (ya filtrado)
    bool ultimaLectura;       // última lectura cruda del pin
    uint32_t ultimoCambioMs;  // cuándo cambió la lectura cruda
    uint32_t inicioPresionMs; // cuándo empezó la presión estable

    bool toqueListo;          // evento pendiente: toque corto
    bool mantenidoListo;      // evento pendiente: mantenido largo
    bool mantenidoEmitido;    // para no repetir el mantenido mientras sigue apretado

    static const uint32_t REBOTE_MS = 20; // filtro anti-rebote
};

#endif