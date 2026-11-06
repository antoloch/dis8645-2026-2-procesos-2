#include "Mapa.h"
#include "Mapaimagen.h"
#include <cstring>

// pixel de la imagen completa (sin vuelta: x de 0 a 599)
static inline bool pixelImagen(int x, int y)
{
    return mapa_completo[y * Mapa::BYTES_POR_FILA + x / 8] & (0x80 >> (x % 8));
}

Mapa::Mapa()
    : desplazamientoX(0),
      intervaloMs(INTERVALO_INICIAL_MS),
      ultimoPasoMs(0)
{
    prepararObstaculos();
}

// Recorre la imagen, agrupa los pixeles encendidos que están cerca unos de otros
// y marca como obstáculo los grupos grandes. Además rellena el interior de cada
// objeto fila por fila, porque los planetas están dibujados solo con contorno y
// si no el personaje podría "atravesarlos" por dentro.
void Mapa::prepararObstaculos()
{
    // son estáticos (no van en la pila, que en la Pico es chica)
    static uint8_t visitado[BYTES_POR_FILA * ALTO];
    static uint16_t pila[4096];

    memset(obstaculos, 0, sizeof(obstaculos));
    memset(visitado, 0, sizeof(visitado));

    for (int y0 = 0; y0 < ALTO; y0++)
    {
        for (int x0 = 0; x0 < ANCHO_TOTAL; x0++)
        {
            if (!pixelImagen(x0, y0) || (visitado[y0 * BYTES_POR_FILA + x0 / 8] & (0x80 >> (x0 % 8))))
            {
                continue;
            }

            // nuevo grupo: lo exploramos y anotamos, por fila, su x mínima y máxima
            int minX[ALTO], maxX[ALTO];
            for (int i = 0; i < ALTO; i++) { minX[i] = ANCHO_TOTAL; maxX[i] = -1; }
            int cantidad = 0;
            int sp = 0;

            visitado[y0 * BYTES_POR_FILA + x0 / 8] |= (0x80 >> (x0 % 8));
            pila[sp++] = (uint16_t)(y0 * ANCHO_TOTAL + x0);

            while (sp > 0)
            {
                int v = pila[--sp];
                int x = v % ANCHO_TOTAL;
                int y = v / ANCHO_TOTAL;
                cantidad++;
                if (x < minX[y]) minX[y] = x;
                if (x > maxX[y]) maxX[y] = x;

                for (int dy = -RADIO_AGRUPACION; dy <= RADIO_AGRUPACION; dy++)
                {
                    for (int dx = -RADIO_AGRUPACION; dx <= RADIO_AGRUPACION; dx++)
                    {
                        int nx = x + dx, ny = y + dy;
                        if (nx < 0 || nx >= ANCHO_TOTAL || ny < 0 || ny >= ALTO) continue;
                        if (!pixelImagen(nx, ny)) continue;
                        uint8_t& byte = visitado[ny * BYTES_POR_FILA + nx / 8];
                        uint8_t bit = 0x80 >> (nx % 8);
                        if ((byte & bit) || sp >= 4096) continue;
                        byte |= bit;
                        pila[sp++] = (uint16_t)(ny * ANCHO_TOTAL + nx);
                    }
                }
            }

            if (cantidad < TAM_MIN_OBJETO)
            {
                continue; // estrellita: decoración
            }

            // objeto grande: marcar de la x mínima a la x máxima en cada fila
            for (int y = 0; y < ALTO; y++)
            {
                for (int x = minX[y]; x <= maxX[y]; x++)
                {
                    obstaculos[y * BYTES_POR_FILA + x / 8] |= (0x80 >> (x % 8));
                }
            }
        }
    }
}

void Mapa::iniciar()
{
    desplazamientoX = 0;
    ultimoPasoMs = to_ms_since_boot(get_absolute_time());
}

void Mapa::reiniciar()
{
    iniciar();
}

void Mapa::setIntervaloMs(uint32_t ms)
{
    if (ms == 0)
    {
        ms = 1; // evita dividir por cero
    }
    intervaloMs = ms;
}

bool Mapa::actualizar()
{
    uint32_t ahora = to_ms_since_boot(get_absolute_time());

    // cuántos pixeles tocaba avanzar desde la última vez
    uint32_t pasos = (ahora - ultimoPasoMs) / intervaloMs;
    if (pasos == 0)
    {
        return false;
    }

    // el % hace que al pasar del pixel 599 vuelva al 0 (bucle infinito)
    desplazamientoX = (desplazamientoX + (int)pasos) % ANCHO_TOTAL;

    // avanzamos la referencia solo lo "gastado", para no perder tiempo sobrante
    ultimoPasoMs += pasos * intervaloMs;
    return true;
}

int Mapa::desplazamiento() const
{
    return desplazamientoX;
}

bool Mapa::hayObstaculo(int xPantalla, int y) const
{
    if (xPantalla < 0 || xPantalla >= ANCHO || y < 0 || y >= ALTO)
    {
        return false;
    }
    int col = (desplazamientoX + xPantalla) % ANCHO_TOTAL;
    return obstaculos[y * BYTES_POR_FILA + col / 8] & (0x80 >> (col % 8));
}

void Mapa::dibujar(uint8_t* buffer) const
{
    memset(buffer, 0, BYTES_BUFFER);

    for (int sx = 0; sx < ANCHO; sx++)
    {
        int col = (desplazamientoX + sx) % ANCHO_TOTAL;
        int byteColumna = col / 8;
        uint8_t mascara = 0x80 >> (col % 8);

        for (int y = 0; y < ALTO; y++)
        {
            if (mapa_completo[y * BYTES_POR_FILA + byteColumna] & mascara)
            {
                // la imagen viene por filas; la OLED pide páginas de 8 px verticales
                buffer[(y / 8) * ANCHO + sx] |= (1 << (y % 8));
            }
        }
    }
}