#include "Pantallita.h"
#include <cstdlib>   // aquí viene abs() (valor sin signo) y malloc() (reservar memoria)
#include <cstring>   // aquí viene memcpy(), que copia un bloque de memoria a otro
#include "hardware/i2c.h"

bool Pantallita::iniciar() {
    i2c_init(i2c0, 400000);                            // Prendemos el bus I2C a 400.000 bits por segundo.
    gpio_set_function(4, GPIO_FUNC_I2C); gpio_set_function(5, GPIO_FUNC_I2C); // GP4 es SDA y GP5 es SCL.
    gpio_pull_up(4); gpio_pull_up(5);                  // I2C necesita las resistencias pull-up.
    pantalla.external_vcc = false;
    // Solo la pantalla se inicia como hardware. Ojo: esto solo confirma que se reservó la memoria,
    // no que la pantalla física haya respondido por I2C.
    if (!ssd1306_init(&pantalla, 128, 64, 0x3c, i2c0)) return false;
    // El lienzo no es hardware: solo necesita un buffer. Copiamos los datos de la pantalla
    // (ancho, alto, tamaño del buffer...) y le damos su propio buffer, sin llamar otra vez a ssd1306_init().
    lienzo = pantalla;
    lienzo.buffer = static_cast<uint8_t*>(malloc(pantalla.bufsize));
    if (lienzo.buffer == nullptr) return false;
    ssd1306_clear(&lienzo);                            // El dibujo parte vacío.
    return true;
}

void Pantallita::limpiar() {
    ssd1306_clear(&lienzo);
}

void Pantallita::dibujarLinea(int x0, int y0, int x1, int y1) {
    const int dx = x1 - x0;                            // Cuánto nos movimos hacia los lados.
    const int dy = y1 - y0;                            // Cuánto nos movimos hacia arriba o abajo.
    int pasos = abs(dx);                               // Usamos el movimiento más largo como cantidad de pasos...
    if (abs(dy) > pasos) pasos = abs(dy);              // ...para no dejar huecos en las líneas inclinadas.
    for (int i = 0; i <= pasos; i++) {                 // Recorremos la línea paso a paso, desde el punto anterior hasta el actual.
        int px = x0, py = y0;                          // Si pasos vale 0, el punto no se movió y solo pintamos ese píxel.
        if (pasos > 0) {
            px = x0 + dx * i / pasos;                  // Columna de este paso.
            py = y0 + dy * i / pasos;                  // Fila de este paso.
        }
        ssd1306_draw_pixel(&lienzo, px, py);           // Pintamos el píxel en el lienzo, donde queda guardado.
    }
}

void Pantallita::mostrar(int x, int y, bool dibujando) {
    memcpy(pantalla.buffer, lienzo.buffer, pantalla.bufsize); // Copiamos el dibujo guardado a la pantalla, así no se pierde.
    ssd1306_clear_square(&pantalla, 0, 0, 128, 8); // Franja superior para mostrar el modo.
    ssd1306_draw_string(&pantalla, 0, 0, 1, dibujando ? "DIBUJAR" : "MOVER");
    ssd1306_draw_pixel(&pantalla, x, y);                      // El cursor es una cruz chica; solo se pinta en "pantalla", así no deja rastro.
    if (x > 0)                  ssd1306_draw_pixel(&pantalla, x - 1, y); // Brazo izquierdo de la cruz (si no estamos en el borde).
    if (x < pantalla.width - 1) ssd1306_draw_pixel(&pantalla, x + 1, y); // Brazo derecho.
    if (y > 8)                  ssd1306_draw_pixel(&pantalla, x, y - 1); // Brazo de arriba (si no estamos en el borde).
    if (y < pantalla.height - 1) ssd1306_draw_pixel(&pantalla, x, y + 1); // Brazo de abajo.
    ssd1306_show(&pantalla);                           // Mandar la imagen por I2C toma tiempo, aunque no agreguemos sleep.
}
