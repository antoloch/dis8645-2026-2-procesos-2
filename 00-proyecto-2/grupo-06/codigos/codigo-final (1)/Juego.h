#ifndef JUEGO_H
#define JUEGO_H

#include <cstdint>
#include "pico/stdlib.h"
#include "Boton.h"
#include "Perilla.h"
#include "Mapa.h"

// Coordina todo el juego: botones, perillas, mapa y personaje.
// Dinámica:
// Hay 4 personajes (se eligen con la perilla derecha) y uno de ellos funciona
// como nave que debe ir pasando los obstáculos a lo largo del mapa.
// Se pierde si choca con un planeta o cae al suelo.
// Si pierde, vuelve al menú de selección de personajes.
// Si no pierde, el mapa avanza infinitamente.
class Juego
{
  public:
    enum class Estado
    {
        APAGADO,    // pantalla apagada
        SELECCION,  // menú de personajes
        JUGANDO     // partida en curso
    };
//dimensiones de la pantalla
    static const int ANCHO = 128;
    static const int ALTO = 64;
//cantidad de personajes para escoger
    static const int TOTAL_PERSONAJES = 4;

// Recibe los objetos ya creados (Boton.h y Perilla.h):
// seleccionador: toque largo (>= 2 s) enciende/apaga, toque corto selecciona
// volador: mientras se mantiene presionado, el personaje sube
// perillaVelocidad: izquierda, controla qué tan rápido avanza el mapa
// perillaPersonaje: derecha, recorre los 4 personajes
    Juego(Boton& seleccionador, Boton& volador, Perilla& perillaVelocidad, Perilla& perillaPersonaje);

// inicia los botones y calcula los datos de los personajes; se llama UNA vez
    void iniciar();

// lee botones y perillas, mueve personaje y mapa, revisa choques;
// se llama en CADA vuelta del loop
    void actualizar();

    // dibuja lo que corresponda al estado actual en el framebuffer (1024 bytes)
    void dibujar(uint8_t* buffer) const;

    // false = hay que apagar el panel
    bool pantallaEncendida() const;

    Estado estadoActual() const;

  // (todo público, como en Perilla.h: aquí abajo van los atributos y ayudas internas)
  public:
    Boton& botonSeleccionador;
    Boton& botonVolador;
    Perilla& perillaVelocidad;
    Perilla& perillaPersonaje;
    Mapa mapa;

    Estado estado;
    int opcionMenu;          // posición marcada en el menú, 0 a 3 (de izquierda a derecha)
    int personaje;           // personaje en juego (índice dentro de PERSONAJES)
    float posY;              // altura del borde superior del personaje
    uint32_t ultimoTickMs;
    uint32_t inicioPartidaMs;
    bool preparado;

    // para cada personaje: su imagen completa y la caja que contiene el dibujo
    struct Ficha
    {
        const uint8_t* datos;
        int x0, y0, x1, y1;
        bool tintaEsUno; // true si el dibujo son los bits en 1; false si son los bits en 0
    };
    Ficha fichas[TOTAL_PERSONAJES];
    int orden[TOTAL_PERSONAJES]; // posición del menú -> índice de ficha (izquierda a derecha)

    // ---- ajustes que puedes tocar ----
    static const int POS_X_PERSONAJE = 12;            // dónde se para el personaje en pantalla
    static constexpr float VELOCIDAD_SUBIDA = 50.0f;  // pixeles por segundo
    static constexpr float VELOCIDAD_CAIDA = 50.0f;
    static const uint32_t GRACIA_MS = 1000;           // al partir, los planetas no cuentan por 1 s
    static const int TAM_HITBOX = 8;                  // zona de choque: cuadrado de 8x8 al centro del personaje
    static const uint32_t INTERVALO_LENTO_MS = 60;    // perilla a la izquierda: 1 px cada 60 ms
    static const uint32_t INTERVALO_RAPIDO_MS = 10;   // perilla a la derecha: 1 px cada 10 ms

    void iniciarPartida();
    void actualizarPartida();
    void perder();
    bool hayColision(uint32_t ahora) const;
    bool tinta(const Ficha& f, int x, int y) const; // ¿es dibujo el pixel (x,y) de la imagen?
};

#endif