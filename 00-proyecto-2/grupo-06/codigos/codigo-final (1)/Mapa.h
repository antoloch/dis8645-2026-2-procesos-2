#ifndef MAPA_H
#define MAPA_H

#include <cstdint>
#include "pico/stdlib.h"

// Muestra el mapa (una imagen de 600x64 px) avanzando de corrido hacia la
// izquierda; al llegar al final vuelve al principio sin corte.
// La pantalla es una "ventana" de 128 px que se desliza por la imagen larga.
//
// Además sabe qué partes del mapa son OBSTÁCULOS (planetas, cometa, portal...)
// y cuáles son solo decoración (las estrellitas), para detectar choques.
class Mapa
{
  public:
    static const int ANCHO = 128;                       // ancho de la pantalla
    static const int ALTO = 64;
    static const int ANCHO_TOTAL = 600;                 // ancho de la imagen completa
    static const int BYTES_POR_FILA = ANCHO_TOTAL / 8;  // 75
    static const int BYTES_BUFFER = ANCHO * (ALTO / 8); // 1024 (formato SSD1306)

    Mapa();

    // deja el mapa en el inicio; se llama al empezar la partida
    void iniciar();

    // avanza según el tiempo transcurrido; se llama en CADA vuelta del loop.
    // devuelve true si el mapa se movió
    bool actualizar();

    // milisegundos que tarda el mapa en avanzar 1 px (lo decide la perilla)
    void setIntervaloMs(uint32_t ms);

    // vuelve al inicio del mapa
    void reiniciar();

    // dibuja la ventana actual en el framebuffer de la OLED (1024 bytes)
    void dibujar(uint8_t* buffer) const;

    int desplazamiento() const;

    // true si en ese pixel de la PANTALLA (x 0..127, y 0..63) hay un obstáculo
    bool hayObstaculo(int xPantalla, int y) const;

    int desplazamientoX;
    uint32_t intervaloMs;
    uint32_t ultimoPasoMs;

    // máscara de obstáculos: mismo formato que la imagen (75 bytes x 64 filas)
    uint8_t obstaculos[BYTES_POR_FILA * ALTO];

    // un grupo de pixeles encendidos con al menos este tamaño cuenta como objeto;
    // las estrellitas (10 a 14 px) quedan como decoración
    static const int TAM_MIN_OBJETO = 100;
    static const int RADIO_AGRUPACION = 3; // pixeles a menos de 3 px se consideran del mismo objeto
    static const uint32_t INTERVALO_INICIAL_MS = 30;

    void prepararObstaculos();
};

#endif