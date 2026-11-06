#pragma once
#include <cstdint>
#include "AdafruitGFXPico.h"

// DORE 8 - Interfaz de usuario: ocho pantallas, sprites, textos y laberinto.
/**
 * PantallaUI.h
 * ---------------------------------------------------------------------------
 * Clase 4: PantallaUI - Capa de interfaz gráfica del juego arcade.
 * Hardware : Raspberry Pi Pico + TFT LCD 2.0" ST7789V (240x320)
 * Gráficos: interfaz tipo Adafruit GFX adaptada a Pico SDK (sin Arduino)
 *
 * Todos los gráficos son VECTORIALES (líneas, rectángulos, círculos):
 * no se usan bitmaps ni framebuffer, por lo que el consumo de RAM es mínimo.
 *
 * Orientación: landscape (setRotation(1)) -> lienzo útil 320 x 240 px.
 * ---------------------------------------------------------------------------
 */
// No requiere Arduino. Para Wokwi usamos el controlador ILI9341.
// 06 · INTERFAZ DEL JUEGO: DIMENSIONES Y OBJETOS
// PantallaUI usa el controlador adaptado para construir ocho escenas.
using TFTDriver = AdafruitGFX_Pico;

// ============================================================================
//  DIMENSIONES DEL LIENZO (modo landscape)
// ============================================================================
// Define el lienzo horizontal de 320x240 y el espacio reservado al juego bajo el contador.
namespace Pantalla {
constexpr int ANCHO = 320;
constexpr int ALTO  = 240;

// Resolución física nativa del panel (para tft.init)
constexpr uint16_t PANEL_ANCHO = 240;
constexpr uint16_t PANEL_ALTO  = 320;

// Márgenes y marcos
constexpr int MARGEN = 6;

// Zona de juego (debajo del HUD de tiempo)
constexpr int JUEGO_X = 6;
constexpr int JUEGO_Y = 22;
constexpr int JUEGO_W = ANCHO - 2 * 6;      // 308
constexpr int JUEGO_H = ALTO - 22 - 6;      // 212
}  // namespace Pantalla

// ============================================================================
//  TAMAÑO DE LOS SPRITES VECTORIALES
// ============================================================================
// Define las dimensiones que se usarán tanto al dibujar como al detectar colisiones.
namespace Sprite {
constexpr int ESTUDIANTE_W = 20;
constexpr int ESTUDIANTE_H = 20;
constexpr int AUTOBUS_W    = 36;
constexpr int AUTOBUS_H    = 20;
constexpr int AUTO_W       = 28;
constexpr int AUTO_H       = 18;
}  // namespace Sprite

// ============================================================================
//  CARRETERA (Slide 4): 3 carriles horizontales
// ============================================================================
// La carretera se distribuye en tres carriles; cada auto tendrá un carril.
namespace Carretera {
constexpr int NUM_CARRILES = 3;
constexpr int CARRIL_ALTO  = Pantalla::JUEGO_H / NUM_CARRILES;  // 70 px
constexpr int CARRIL_Y0    = Pantalla::JUEGO_Y;                 // 22
}  // namespace Carretera

// ============================================================================
//  PALETA DE COLORES (RGB565, alto contraste sobre fondo negro)
// ============================================================================
// Paleta RGB565 compartida por textos, personajes y escenarios.
namespace Colores {
constexpr uint16_t NEGRO       = ST77XX_BLACK;
constexpr uint16_t BLANCO      = ST77XX_WHITE;
constexpr uint16_t AMARILLO    = ST77XX_YELLOW;
constexpr uint16_t AZUL        = ST77XX_BLUE;
constexpr uint16_t ROJO        = ST77XX_RED;
constexpr uint16_t VERDE       = ST77XX_GREEN;
constexpr uint16_t CIAN        = ST77XX_CYAN;
constexpr uint16_t MAGENTA     = ST77XX_MAGENTA;
constexpr uint16_t NARANJA     = ST77XX_ORANGE;
constexpr uint16_t GRIS        = 0x7BEF;
constexpr uint16_t GRIS_OSCURO = 0x4208;
constexpr uint16_t MARRON      = 0x8A22;  // RGB(139,69,19)
constexpr uint16_t AZUL_CLARO  = 0x5D1F;

// Colores por defecto de la interfaz
constexpr uint16_t FONDO       = NEGRO;
constexpr uint16_t TEXTO       = BLANCO;
constexpr uint16_t MARCO       = BLANCO;
}  // namespace Colores

// ============================================================================
//  CLASE PantallaUI
// ============================================================================
// 07 · CLASE PantallaUI
// Agrupa el dibujo de las escenas y las utilidades de texto, sprites y colisiones.
class PantallaUI {
public:
    // El juego se organiza en ocho pantallas, entre introducción, niveles y cierre.
    static constexpr int TOTAL_SLIDES = 8;

    // Slides (estados de pantalla)
    // Cada estado recibe un nombre para evitar confundir números de pantalla.
    enum Slide : int {
        SLIDE_INICIO        = 0,
        SLIDE_INSTRUCCIONES = 1,
        SLIDE_DESAFIO       = 2,
        SLIDE_LABERINTO     = 3,
        SLIDE_CARRETERA     = 4,
        SLIDE_SALA          = 5,
        SLIDE_VICTORIA      = 6,
        SLIDE_REFLEXION     = 7
    };

    /** Inyección de dependencia: recibe la referencia al controlador TFT. */
    // La interfaz recibe una referencia a la pantalla: no crea una pantalla nueva.
    explicit PantallaUI(TFTDriver &tft);

    /** Inicializa el panel, orientación horizontal y fondo negro. */
    // Inicializa SPI, los pines y el controlador de pantalla.
    void begin();

    /** Limpia toda la pantalla con el color de fondo. */
    void limpiar(uint16_t color = Colores::FONDO);

    // ---------------- Sprites vectoriales (x,y = esquina superior izquierda)
    void dibujarEstudiante(int x, int y);
    void dibujarAutobus(int x, int y);
    void dibujarAuto(int x, int y, uint16_t color);

    // ---------------- HUD
    void dibujarTiempo(int segundos);
    void mensajeEstado(const char *texto, uint16_t color = Colores::AMARILLO);
    void redibujarLineasCarretera();
    bool colisionLaberinto(int x, int y, int ancho = Sprite::ESTUDIANTE_W,
                          int alto = Sprite::ESTUDIANTE_H) const;

    // ---------------- Pantallas completas (0..7)
    void dibujarSlide(int numeroSlide);

    // ---------------- Utilidades para la lógica del juego
    /** Coordenada Y (esquina sup.) para centrar un auto en el carril 0..2. */
    static int yCarril(int carril);

private:
    TFTDriver &_tft;

    // Helpers de dibujo
    void textoCentrado(const char *txt, int y, uint8_t tam, uint16_t color);
    void marcoDoble(int x, int y, int w, int h, uint16_t color, int radio = 0);
    void lineaGruesa(int x0, int y0, int x1, int y1, uint16_t color);

    // Slides individuales
    void slideInicio();
    void slideInstrucciones();
    void slideDesafio();
    void slideLaberinto();
    void slideCarretera();
    void slideSala();
    void slideVictoria();
    void slideReflexion();
};
