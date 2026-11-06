#include "Juego.h"
#include "Personajes.h"
#include <cstring>

// ---- ayudas para dibujar (solo se usan en este archivo) ----

static void ponerPixel(uint8_t* buffer, int x, int y)
{
    if (x < 0 || x >= Juego::ANCHO || y < 0 || y >= Juego::ALTO)
    {
        return;
    }
    buffer[(y / 8) * Juego::ANCHO + x] |= (1 << (y % 8));
}

static void quitarPixel(uint8_t* buffer, int x, int y)
{
    if (x < 0 || x >= Juego::ANCHO || y < 0 || y >= Juego::ALTO)
    {
        return;
    }
    buffer[(y / 8) * Juego::ANCHO + x] &= ~(1 << (y % 8));
}

static void dibujarMarco(uint8_t* buffer, int x, int y, int ancho, int alto)
{
    for (int i = 0; i < ancho; i++)
    {
        ponerPixel(buffer, x + i, y);
        ponerPixel(buffer, x + i, y + alto - 1);
    }
    for (int j = 0; j < alto; j++)
    {
        ponerPixel(buffer, x, y + j);
        ponerPixel(buffer, x + ancho - 1, y + j);
    }
}

// Clase Juego

Juego::Juego(Boton& seleccionador, Boton& volador, Perilla& perillaVelocidad, Perilla& perillaPersonaje)
    : botonSeleccionador(seleccionador),
      botonVolador(volador),
      perillaVelocidad(perillaVelocidad),
      perillaPersonaje(perillaPersonaje),
      mapa(),
      estado(Estado::APAGADO),
      opcionMenu(0),
      personaje(0),
      posY(0.0f),
      ultimoTickMs(0),
      inicioPartidaMs(0),
      preparado(false)
{
}

// ¿Es dibujo el pixel (x, y) de la imagen del personaje?
// Según cómo se exporte la imagen, el dibujo puede venir como bits en 1 o en 0.
// En iniciar() se detecta solo: el dibujo es el color que tiene MENOS pixeles.
bool Juego::tinta(const Ficha& f, int x, int y) const
{
    bool bit = f.datos[y * 16 + x / 8] & (0x80 >> (x % 8));
    return f.tintaEsUno ? bit : !bit;
}

void Juego::iniciar()
{
    botonSeleccionador.iniciar();
    botonVolador.iniciar();

    // las 4 imágenes, en el orden en que están en Personajes.h
    const uint8_t* imagenes[TOTAL_PERSONAJES] = {
        personaje_gato, personaje_calabaza, personaje_vaca, personaje_chango
    };

    // para cada una, buscamos la caja más chica que contiene el dibujo
    for (int i = 0; i < TOTAL_PERSONAJES; i++)
    {
        Ficha& f = fichas[i];
        f.datos = imagenes[i];

        // contamos los bits en 1: si son la minoría, el dibujo es lo que está en 1
        int unos = 0;
        for (int b = 0; b < 1024; b++)
        {
            for (int k = 0; k < 8; k++)
            {
                if (f.datos[b] & (1 << k)) unos++;
            }
        }
        f.tintaEsUno = (unos < 4096); // 4096 = la mitad de los 8192 bits de la imagen

        f.x0 = ANCHO; f.y0 = ALTO; f.x1 = -1; f.y1 = -1;
        for (int y = 0; y < ALTO; y++)
        {
            for (int x = 0; x < ANCHO; x++)
            {
                if (tinta(f, x, y))
                {
                    if (x < f.x0) f.x0 = x;
                    if (x > f.x1) f.x1 = x;
                    if (y < f.y0) f.y0 = y;
                    if (y > f.y1) f.y1 = y;
                }
            }
        }
        orden[i] = i;
    }

    // el menú los ordena de izquierda a derecha según donde quedó cada dibujo
    for (int i = 1; i < TOTAL_PERSONAJES; i++)
    {
        int k = orden[i];
        int j = i - 1;
        while (j >= 0 && fichas[orden[j]].x0 > fichas[k].x0)
        {
            orden[j + 1] = orden[j];
            j--;
        }
        orden[j + 1] = k;
    }

    preparado = true;
    estado = Estado::APAGADO;
}

bool Juego::pantallaEncendida() const
{
    return estado != Estado::APAGADO;
}

Juego::Estado Juego::estadoActual() const
{
    return estado;
}

void Juego::actualizar()
{
    if (!preparado)
    {
        return;
    }

    botonSeleccionador.actualizar();
    botonVolador.actualizar();
    perillaVelocidad.leer();
    perillaPersonaje.leer();

    // La velocidad del mapa sigue a la perilla izquierda en todo momento:
    // 0.0 (izquierda) = lento, 1.0 (derecha) = rápido
    float v = perillaVelocidad.porcentaje();
    mapa.setIntervaloMs((uint32_t)(INTERVALO_LENTO_MS - (INTERVALO_LENTO_MS - INTERVALO_RAPIDO_MS) * v));

    // Se leen SIEMPRE los dos eventos, aunque el estado no los use,
    // para que no queden pendientes y se disparen más tarde por error.
    bool toqueLargo = botonSeleccionador.huboMantenido();
    bool toqueCorto = botonSeleccionador.huboToque();

    // toque largo: enciende o apaga, sin importar el estado
    if (toqueLargo)
    {
        if (estado == Estado::APAGADO)
        {
            mapa.reiniciar();
            estado = Estado::SELECCION;
        }
        else
        {
            estado = Estado::APAGADO;
        }
        return;
    }

    switch (estado)
    {
        case Estado::APAGADO:
            break;

        case Estado::SELECCION:
            // la perilla derecha recorre las 4 opciones (con histéresis incluida)
            opcionMenu = perillaPersonaje.paso(TOTAL_PERSONAJES);

            // toque corto: confirma el personaje marcado
            if (toqueCorto)
            {
                personaje = orden[opcionMenu];
                iniciarPartida();
            }
            break;

        case Estado::JUGANDO:
            actualizarPartida();
            break;
    }
}

void Juego::iniciarPartida()
{
    const Ficha& f = fichas[personaje];
    estado = Estado::JUGANDO;
    posY = (ALTO - (f.y1 - f.y0 + 1)) / 2.0f; // parte a media altura
    mapa.iniciar();
    ultimoTickMs = to_ms_since_boot(get_absolute_time());
    inicioPartidaMs = ultimoTickMs;
}

void Juego::actualizarPartida()
{
    uint32_t ahora = to_ms_since_boot(get_absolute_time());
    float dt = (ahora - ultimoTickMs) / 1000.0f; // segundos desde la última vuelta
    ultimoTickMs = ahora;
    if (dt > 0.1f)
    {
        dt = 0.1f; // evita saltos si una vuelta se demoró mucho
    }

    // botón volador: presionado sube, soltado cae
    if (botonVolador.estaPresionado())
    {
        posY -= VELOCIDAD_SUBIDA * dt;
    }
    else
    {
        posY += VELOCIDAD_CAIDA * dt;
    }

    // el techo no mata: el personaje solo se queda arriba
    if (posY < 0.0f)
    {
        posY = 0.0f;
    }

    mapa.actualizar();

    if (hayColision(ahora))
    {
        perder();
    }
}

bool Juego::hayColision(uint32_t ahora) const
{
    const Ficha& f = fichas[personaje];
    int alto = f.y1 - f.y0 + 1;
    int ancho = f.x1 - f.x0 + 1;
    int y = (int)posY;

    // caer al suelo (borde de abajo de la pantalla)
    if (y + alto >= ALTO)
    {
        return true;
    }

    // al partir hay un segundo de gracia con los planetas
    if (ahora - inicioPartidaMs < GRACIA_MS)
    {
        return false;
    }

    // chocar con un planeta: solo cuenta un cuadradito en el centro del personaje.
    // Los dibujos miden unos 25 px de alto y los planetas son enormes, así que con
    // el cuerpo completo algunos tramos del mapa serían imposibles de pasar.
    int hx = POS_X_PERSONAJE + (ancho - TAM_HITBOX) / 2;
    int hy = y + (alto - TAM_HITBOX) / 2;
    for (int sy = 0; sy < TAM_HITBOX; sy++)
    {
        for (int sx = 0; sx < TAM_HITBOX; sx++)
        {
            if (mapa.hayObstaculo(hx + sx, hy + sy))
            {
                return true;
            }
        }
    }
    return false;
}

void Juego::perder()
{
    // vuelve al menú de selección de personajes
    estado = Estado::SELECCION;
    mapa.reiniciar();
}

void Juego::dibujar(uint8_t* buffer) const
{
    memset(buffer, 0, Mapa::BYTES_BUFFER);

    switch (estado)
    {
        case Estado::APAGADO:
            break;

        case Estado::SELECCION:
        {
            // Los 4 personajes se reparten solos a lo ancho de la pantalla:
            // cada uno queda centrado en su cuarto (32 px) y a media altura,
            // sin importar su tamaño ni dónde esté dibujado dentro de su imagen.
            const int anchoCuarto = ANCHO / TOTAL_PERSONAJES;
            for (int p = 0; p < TOTAL_PERSONAJES; p++)
            {
                const Ficha& f = fichas[orden[p]];
                int w = f.x1 - f.x0 + 1;
                int h = f.y1 - f.y0 + 1;
                int px = p * anchoCuarto + (anchoCuarto - w) / 2; // esquina superior izquierda
                int py = (ALTO - h) / 2;

                for (int sy = 0; sy < h; sy++)
                {
                    for (int sx = 0; sx < w; sx++)
                    {
                        if (tinta(f, f.x0 + sx, f.y0 + sy))
                        {
                            ponerPixel(buffer, px + sx, py + sy);
                        }
                    }
                }

                // marco alrededor del personaje marcado con la perilla
                if (p == opcionMenu)
                {
                    dibujarMarco(buffer, px - 3, py - 3, w + 6, h + 6);
                }
            }
            break;
        }

        case Estado::JUGANDO:
        {
            mapa.dibujar(buffer);

            // primero limpiamos la caja del personaje y luego dibujamos su contorno,
            // así no se confunde con las estrellas de atrás
            const Ficha& f = fichas[personaje];
            int y = (int)posY;
            for (int sy = 0; sy <= f.y1 - f.y0; sy++)
            {
                for (int sx = 0; sx <= f.x1 - f.x0; sx++)
                {
                    quitarPixel(buffer, POS_X_PERSONAJE + sx, y + sy);
                    if (tinta(f, f.x0 + sx, f.y0 + sy))
                    {
                        ponerPixel(buffer, POS_X_PERSONAJE + sx, y + sy);
                    }
                }
            }
            break;
        }
    }
}