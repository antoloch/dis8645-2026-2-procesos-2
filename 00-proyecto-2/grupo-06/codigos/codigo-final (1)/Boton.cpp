#include "Boton.h"

Boton::Boton(uint patita, uint32_t umbralLargoMs)
    : patita(patita),
      umbralLargoMs(umbralLargoMs),
      presionado(false),
      ultimaLectura(false),
      ultimoCambioMs(0),
      inicioPresionMs(0),
      toqueListo(false),
      mantenidoListo(false),
      mantenidoEmitido(false)
{
}

void Boton::iniciar()
{
    gpio_init(patita);
    gpio_set_dir(patita, GPIO_IN);
    gpio_pull_up(patita); // sin apretar = 1, apretado = 0
}

void Boton::actualizar()
{
    uint32_t ahora = to_ms_since_boot(get_absolute_time());

    // como es pull-up, apretado significa que el pin lee 0
    bool lectura = !gpio_get(patita);

    // si la lectura cambió, reiniciamos el cronómetro del rebote
    if (lectura != ultimaLectura)
    {
        ultimaLectura = lectura;
        ultimoCambioMs = ahora;
    }

    // solo aceptamos el cambio si se mantuvo estable REBOTE_MS
    if ((ahora - ultimoCambioMs) >= REBOTE_MS && lectura != presionado)
    {
        presionado = lectura;

        if (presionado)
        {
            // recién se apretó
            inicioPresionMs = ahora;
            mantenidoEmitido = false;
        }
        else
        {
            // recién se soltó: si nunca llegó a "largo", fue un toque
            if (!mantenidoEmitido)
            {
                toqueListo = true;
            }
        }
    }

    // ¿se cumplió el tiempo de mantenido mientras sigue apretado?
    if (presionado && !mantenidoEmitido &&
        (ahora - inicioPresionMs) >= umbralLargoMs)
    {
        mantenidoListo = true;
        mantenidoEmitido = true;
    }
}

bool Boton::estaPresionado() const
{
    return presionado;
}

bool Boton::huboToque()
{
    bool resultado = toqueListo;
    toqueListo = false; // se "consume" el evento
    return resultado;
}

bool Boton::huboMantenido()
{
    bool resultado = mantenidoListo;
    mantenidoListo = false; // se "consume" el evento
    return resultado;
}

uint32_t Boton::tiempoPresionado() const
{
    if (!presionado)
    {
        return 0;
    }
    return to_ms_since_boot(get_absolute_time()) - inicioPresionMs;
}
