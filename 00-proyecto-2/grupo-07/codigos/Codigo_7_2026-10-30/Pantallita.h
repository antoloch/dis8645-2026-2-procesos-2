#ifndef TRAZO_PANTALLITA_H
#define TRAZO_PANTALLITA_H
#include "pico/stdlib.h"
extern "C" { // La biblioteca de pantalla está en C; así la podemos llamar desde nuestro C++.
#include "Ssd1306.h"
}
// Pantallita se encarga de TODO lo que tiene que ver con la OLED: el I2C, la SSD1306 y los buffers.
// El main solo le dice qué hacer: iniciar, limpiar, dibujar una línea y mostrar.
class Pantallita {
public:
    bool iniciar();                                       // Prende el I2C y la SSD1306. Devuelve false si no pudo reservar memoria.
    void limpiar();                                       // Vacía el dibujo guardado.
    void dibujarLinea(int x0, int y0, int x1, int y1);    // Pinta en el dibujo guardado una línea desde (x0,y0) hasta (x1,y1).
    void mostrar(int x, int y, bool dibujando);                           // Muestra el dibujo, el estado MOVER/DIBUJAR y el cursor sin guardarlo.
private:
    ssd1306_t pantalla{}; // Lo que se ve: dibujo + estado + cursor.
    ssd1306_t lienzo{};   // Hoja en memoria donde guardamos SOLO el dibujo. Nunca la mostramos.
};
#endif
