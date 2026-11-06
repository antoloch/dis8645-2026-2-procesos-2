#include "PantallaUI.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

// DORE 8 - Dibuja los elementos visuales y comprueba paredes del laberinto.
/**
 * PantallaUI.cpp
 * ---------------------------------------------------------------------------
 * Implementación de la clase PantallaUI (gráficos 100% vectoriales).
 *
 * NOTA SOBRE ACENTOS:
 *   La fuente por defecto de Adafruit_GFX usa la tabla CP437. Por eso los
 *   caracteres especiales se escriben con códigos CP437 (macros de abajo) y
 *   se activa tft.setCP437(true) en begin().
 *   Cada macro va en su propio literal ("..." CP_X "...") para evitar que
 *   un escape hexadecimal "\xA2" absorba letras siguientes (a-f).
 * ---------------------------------------------------------------------------
 */
#define CP_A_AGUDA "\xA0"   // á
#define CP_E_AGUDA "\x82"   // é
#define CP_I_AGUDA "\xA1"   // í
#define CP_O_AGUDA "\xA2"   // ó
#define CP_U_AGUDA "\xA3"   // ú
#define CP_N_TILDE "\xA4"   // ñ
#define CP_EXCL_INV "\xAD"  // ¡
#define CP_INTER_INV "\xA8" // ¿

using namespace Colores;

// ============================================================================
//  DATOS DEL LABERINTO (Slide 3)
//  Segmentos de pared (solo horizontales/verticales) transcritos del boceto.
//  Coordenadas en el sistema del boceto; se escalan a la pantalla en runtime.
//  Origen del boceto: (249,1277)  |  tamaño: 456 x 283
// ============================================================================
namespace {

struct Muro {
    int16_t x1, y1, x2, y2;
};

constexpr int SRC_X0 = 249;
constexpr int SRC_Y0 = 1277;
constexpr int SRC_W  = 456;
constexpr int SRC_H  = 283;

// Área del laberinto en pantalla
constexpr int MAZE_X = 10;
constexpr int MAZE_Y = 20;
constexpr int MAZE_W = 300;
constexpr int MAZE_H = 212;
constexpr int GROSOR = 2;

const Muro MUROS[] = {
    // --- zona superior ---
    {303, 1277, 303, 1335},
    {303, 1335, 433, 1335},
    {433, 1310, 433, 1335},
    {433, 1310, 530, 1310},
    {530, 1310, 530, 1370},
    {530, 1370, 573, 1370},
    {573, 1277, 573, 1370},
    {615, 1277, 615, 1360},
    {660, 1330, 700, 1330},
    {660, 1365, 660, 1435},
    {660, 1435, 700, 1435},
    // --- zona izquierda ---
    {303, 1335, 303, 1457},
    {283, 1457, 303, 1457},
    // --- zona central ---
    {483, 1345, 483, 1372},
    {343, 1372, 483, 1372},
    {343, 1372, 343, 1448},
    {343, 1448, 368, 1448},
    {368, 1407, 417, 1407},
    {417, 1407, 417, 1497},
    {343, 1497, 417, 1497},
    {337, 1497, 337, 1528},
    {249, 1528, 337, 1528},
    // --- zona inferior ---
    {449, 1407, 573, 1407},
    {449, 1407, 449, 1528},
    {483, 1448, 573, 1448},
    {483, 1448, 483, 1528},
    {530, 1448, 530, 1528},
    {483, 1528, 573, 1528},
    {573, 1407, 573, 1558},
};

inline int escX(int x) { return MAZE_X + ((x - SRC_X0) * MAZE_W) / SRC_W; }
inline int escY(int y) { return MAZE_Y + ((y - SRC_Y0) * MAZE_H) / SRC_H; }

}  // namespace

// ============================================================================
//  CONSTRUCTOR / INICIALIZACIÓN
// ============================================================================
PantallaUI::PantallaUI(TFTDriver &tft) : _tft(tft) {}

// 08 · IMPLEMENTACIÓN DE PantallaUI
// Prepara horizontalidad, codificación de caracteres y fondo oscuro.
void PantallaUI::begin() {
    _tft.begin();  // ST7789V físico o ILI9341 en Wokwi según configuración de compilación
    _tft.setRotation(1);                                     // landscape 320x240
    _tft.setCP437(true);                                     // acentos / ¡ ¿
    _tft.setTextWrap(false);
    _tft.fillScreen(Colores::FONDO);
}

// Borra la escena antes de cambiar de pantalla.
void PantallaUI::limpiar(uint16_t color) {
    _tft.fillScreen(color);
}

// ============================================================================
//  HELPERS
// ============================================================================
// Mide un texto, calcula su posición horizontal y lo dibuja centrado.
void PantallaUI::textoCentrado(const char *txt, int y, uint8_t tam, uint16_t color) {
    int16_t x1, y1;
    uint16_t w, h;
    _tft.setTextSize(tam);
    _tft.setTextColor(color);
    _tft.getTextBounds(txt, 0, 0, &x1, &y1, &w, &h);
    _tft.setCursor((Pantalla::ANCHO - (int)w) / 2 - x1, y);
    _tft.print(txt);
}

// Dibuja dos bordes paralelos para dar profundidad a los cuadros de diálogo.
void PantallaUI::marcoDoble(int x, int y, int w, int h, uint16_t color, int radio) {
    if (radio > 0) {
        _tft.drawRoundRect(x, y, w, h, radio, color);
        _tft.drawRoundRect(x + 1, y + 1, w - 2, h - 2, radio, color);
    } else {
        _tft.drawRect(x, y, w, h, color);
        _tft.drawRect(x + 1, y + 1, w - 2, h - 2, color);
    }
}

// Superpone dos líneas casi iguales para dar mayor grosor al personaje.
void PantallaUI::lineaGruesa(int x0, int y0, int x1, int y1, uint16_t color) {
    _tft.drawLine(x0, y0, x1, y1, color);
    _tft.drawLine(x0 + 1, y0, x1 + 1, y1, color);
}

// Calcula la coordenada vertical de un vehículo y evita índices fuera de los carriles.
int PantallaUI::yCarril(int carril) {
    if (carril < 0) carril = 0;
    if (carril >= Carretera::NUM_CARRILES) carril = Carretera::NUM_CARRILES - 1;
    return Carretera::CARRIL_Y0 + carril * Carretera::CARRIL_ALTO +
           (Carretera::CARRIL_ALTO - Sprite::AUTO_H) / 2;
}

// ============================================================================
//  SPRITES VECTORIALES
// ============================================================================

/** Monigote corriendo (20x20 px): cabeza circular + tronco + extremidades. */
// 09 · PERSONAJES Y OBJETOS
// El estudiante es una figura hecha con cabeza circular, tronco, brazos y piernas.
void PantallaUI::dibujarEstudiante(int x, int y) {
    const uint16_t c = BLANCO;

    _tft.drawCircle(x + 10, y + 4, 4, c);              // cabeza
    lineaGruesa(x + 10, y + 8, x + 10, y + 14, c);     // tronco

    lineaGruesa(x + 10, y + 10, x + 3, y + 8, c);      // brazo atrás
    lineaGruesa(x + 10, y + 10, x + 17, y + 13, c);    // brazo adelante

    lineaGruesa(x + 10, y + 14, x + 4, y + 19, c);     // pierna atrás
    lineaGruesa(x + 10, y + 14, x + 15, y + 16, c);    // pierna adelante (muslo)
    lineaGruesa(x + 15, y + 16, x + 14, y + 19, c);    // pierna adelante (pantorrilla)
}

/** Autobús compacto (36x20 px): carrocería, ventanas, franja y ruedas. */
// La micro se construye con carrocería, ventanas, franja y dos ruedas.
void PantallaUI::dibujarAutobus(int x, int y) {
    _tft.fillRoundRect(x, y, Sprite::AUTOBUS_W, 15, 2, AMARILLO);   // carrocería
    _tft.drawRoundRect(x, y, Sprite::AUTOBUS_W, 15, 2, NEGRO);      // contorno

    for (int i = 0; i < 4; i++) {                                    // ventanas
        _tft.fillRect(x + 3 + i * 8, y + 3, 6, 5, CIAN);
    }
    _tft.drawFastHLine(x + 1, y + 10, Sprite::AUTOBUS_W - 2, NEGRO); // franja

    // Ruedas
    _tft.fillCircle(x + 8, y + 16, 3, NEGRO);
    _tft.drawCircle(x + 8, y + 16, 3, BLANCO);
    _tft.fillCircle(x + Sprite::AUTOBUS_W - 9, y + 16, 3, NEGRO);
    _tft.drawCircle(x + Sprite::AUTOBUS_W - 9, y + 16, 3, BLANCO);
}

/** Auto compacto (28x18 px) del color indicado. */
// Cada automóvil usa la misma forma; el color permite diferenciarlos.
void PantallaUI::dibujarAuto(int x, int y, uint16_t color) {
    _tft.fillRoundRect(x, y + 6, Sprite::AUTO_W, 8, 2, color);       // carrocería
    _tft.fillRoundRect(x + 6, y, 16, 8, 3, color);                   // cabina
    _tft.fillRect(x + 8, y + 2, 5, 4, CIAN);                         // ventana 1
    _tft.fillRect(x + 15, y + 2, 5, 4, CIAN);                        // ventana 2

    // Ruedas
    _tft.fillCircle(x + 7, y + 14, 3, NEGRO);
    _tft.drawCircle(x + 7, y + 14, 3, BLANCO);
    _tft.fillCircle(x + 21, y + 14, 3, NEGRO);
    _tft.drawCircle(x + 21, y + 14, 3, BLANCO);
}

// ============================================================================
//  HUD
// ============================================================================
// 10 · INDICADORES Y ESCENAS
// Actualiza únicamente el área del contador para no redibujar toda la pantalla.
void PantallaUI::dibujarTiempo(int segundos) {
    char buf[20];
    snprintf(buf, sizeof(buf), "Tiempo: %ds", segundos);

    _tft.fillRect(0, 0, 90, 13, FONDO);   // borra el valor anterior
    _tft.setTextSize(1);
    _tft.setTextColor(TEXTO);
    _tft.setCursor(3, 3);
    _tft.print(buf);
}

// ============================================================================
//  SLIDES
// ============================================================================
// Despacha el dibujo al método correspondiente según la pantalla actual.
void PantallaUI::dibujarSlide(int numeroSlide) {
    limpiar();
    switch (numeroSlide) {
        case SLIDE_INICIO:        slideInicio();        break;
        case SLIDE_INSTRUCCIONES: slideInstrucciones(); break;
        case SLIDE_DESAFIO:       slideDesafio();       break;
        case SLIDE_LABERINTO:     slideLaberinto();     break;
        case SLIDE_CARRETERA:     slideCarretera();     break;
        case SLIDE_SALA:          slideSala();          break;
        case SLIDE_VICTORIA:      slideVictoria();      break;
        case SLIDE_REFLEXION:     slideReflexion();     break;
        default: break;
    }
}

// ---- Slide 0: Inicio -------------------------------------------------------
// Pantalla 0: saludo inicial y aviso para presionar el botón.
void PantallaUI::slideInicio() {
    marcoDoble(4, 4, Pantalla::ANCHO - 8, Pantalla::ALTO - 8, MARCO, 6);

    textoCentrado("Hola!", 50, 4, AMARILLO);
    textoCentrado("Comencemos...", 100, 2, BLANCO);

    // Botón "Presiona el botón" (recuadro inferior)
    const int bx = 84, by = 160, bw = 220, bh = 36;
    _tft.drawRect(bx, by, bw, bh, BLANCO);
    _tft.drawRect(bx + 1, by + 1, bw - 2, bh - 2, BLANCO);
    _tft.setTextSize(2);
    _tft.setTextColor(VERDE);
    _tft.setCursor(bx + 8, by + 11);
    _tft.print("Presiona el bot" CP_O_AGUDA "n");

    // Flecha apuntando al botón
    const int ay = by + bh / 2;
    lineaGruesa(26, ay, 74, ay, BLANCO);
    _tft.fillTriangle(12, ay, 28, ay - 8, 28, ay + 8, BLANCO);
}

// ---- Slide 1: Instrucciones ------------------------------------------------
// Pantalla 1: muestra los controles del prototipo y su función prevista.
void PantallaUI::slideInstrucciones() {
    marcoDoble(4, 4, Pantalla::ANCHO - 8, Pantalla::ALTO - 8, MARCO, 6);
    textoCentrado("Instrucciones", 14, 2, CIAN);
    _tft.drawFastHLine(20, 36, Pantalla::ANCHO - 40, GRIS);

    struct Item {
        const char *etiqueta;
        const char *detalle;
        uint16_t color;
        bool relleno;
    };
    const Item items[] = {
        {"Bot" CP_O_AGUDA "n amarillo:", "continuar",                   AMARILLO, true},
        {"Bot" CP_O_AGUDA "n azul:",     "cambiar canci" CP_O_AGUDA "n", AZUL_CLARO, true},
        {"Perilla izquierda:",           "vertical",                    BLANCO,   false},
        {"Perilla derecha:",             "horizontal",                  BLANCO,   false},
    };

    int y = 46;
    for (const Item &it : items) {
        // Icono del control
        if (it.relleno) {
            _tft.fillCircle(24, y + 14, 8, it.color);
            _tft.drawCircle(24, y + 14, 8, BLANCO);
        } else {
            _tft.drawCircle(24, y + 14, 8, it.color);
            _tft.fillCircle(24, y + 14, 3, it.color);
        }
        _tft.setTextSize(2);
        _tft.setTextColor(it.color);
        _tft.setCursor(42, y);
        _tft.print(it.etiqueta);
        _tft.setTextColor(BLANCO);
        _tft.setCursor(54, y + 18);
        _tft.print(it.detalle);
        y += 46;
    }
}

// ---- Slide 2: Intro desafío ------------------------------------------------
// Pantalla 2: presenta el reto de llegar a la universidad.
void PantallaUI::slideDesafio() {
    marcoDoble(4, 4, Pantalla::ANCHO - 8, Pantalla::ALTO - 8, MARCO, 6);
    textoCentrado("Desaf" CP_I_AGUDA "o", 60, 3, AMARILLO);
    textoCentrado("Llega a la universidad", 125, 2, BLANCO);

    // Decoración: estudiante hacia el autobús
    dibujarEstudiante(110, 180);
    _tft.drawFastHLine(136, 190, 24, GRIS);
    dibujarAutobus(168, 180);
}

// ---- Slide 3: Juego - Laberinto -------------------------------------------
// Pantalla 3: dibuja los límites y paredes; la micro es la meta.
void PantallaUI::slideLaberinto() {
    // Marco exterior (grosor 2)
    _tft.drawRect(MAZE_X, MAZE_Y, MAZE_W, MAZE_H, BLANCO);
    _tft.drawRect(MAZE_X + 1, MAZE_Y + 1, MAZE_W - 2, MAZE_H - 2, BLANCO);

    // Paredes interiores
    const size_t n = sizeof(MUROS) / sizeof(MUROS[0]);
    for (size_t i = 0; i < n; i++) {
        const int x1 = escX(MUROS[i].x1), y1 = escY(MUROS[i].y1);
        const int x2 = escX(MUROS[i].x2), y2 = escY(MUROS[i].y2);
        if (y1 == y2) {                       // horizontal
            _tft.fillRect(std::min(x1, x2), y1, abs(x2 - x1) + GROSOR, GROSOR, BLANCO);
        } else {                              // vertical
            _tft.fillRect(x1, std::min(y1, y2), GROSOR, abs(y2 - y1) + GROSOR, BLANCO);
        }
    }

    // Punto de partida (arriba-izquierda) y meta (abajo-derecha)
    // El estudiante movil se dibuja desde main.cpp segun las perillas.
    dibujarAutobus(MAZE_X + MAZE_W - Sprite::AUTOBUS_W - 8,
                   MAZE_Y + MAZE_H - Sprite::AUTOBUS_H - 8);
}

// ---- Slide 4: Juego - Carretera -------------------------------------------
// Pantalla 4: prepara tres carriles y sus líneas divisorias.
void PantallaUI::slideCarretera() {
    using namespace Pantalla;
    // Borde de la calle
    _tft.drawRect(JUEGO_X, JUEGO_Y, JUEGO_W, JUEGO_H, BLANCO);

    // Líneas divisorias de carriles (discontinuas)
    for (int c = 1; c < Carretera::NUM_CARRILES; c++) {
        const int y = Carretera::CARRIL_Y0 + c * Carretera::CARRIL_ALTO;
        for (int x = JUEGO_X + 4; x < JUEGO_X + JUEGO_W - 16; x += 22) {
            _tft.drawFastHLine(x, y, 14, BLANCO);
            _tft.drawFastHLine(x, y + 1, 14, BLANCO);
        }
    }
}

// ---- Slide 5: Juego - Pasillo / Sala --------------------------------------
// Pantalla 5: muestra el espacio de llegada y la puerta de la sala 304.
void PantallaUI::slideSala() {
    using namespace Pantalla;
    // Marco de la sala
    _tft.drawRect(JUEGO_X, JUEGO_Y, JUEGO_W, JUEGO_H, BLANCO);
    _tft.drawRect(JUEGO_X + 1, JUEGO_Y + 1, JUEGO_W - 2, JUEGO_H - 2, BLANCO);

    // Puerta en la esquina inferior derecha
    const int pw = 40, ph = 52;
    const int px = JUEGO_X + JUEGO_W - pw - 2;
    const int py = JUEGO_Y + JUEGO_H - ph - 2;

    _tft.fillRect(px, py, pw, ph, MARRON);
    _tft.drawRect(px, py, pw, ph, BLANCO);
    _tft.drawRect(px + 1, py + 1, pw - 2, ph - 2, BLANCO);

    // Cartel "SALA" sobre la puerta y "304" en la hoja
    _tft.setTextSize(1);
    _tft.setTextColor(AMARILLO);
    _tft.setCursor(px + (pw - 24) / 2, py - 12);
    _tft.print("SALA");

    _tft.setTextSize(2);
    _tft.setTextColor(BLANCO);
    _tft.setCursor(px + (pw - 36) / 2, py + 8);
    _tft.print("304");

    // Perilla
    _tft.fillCircle(px + pw - 9, py + 36, 2, AMARILLO);
}

// ---- Slide 6: Victoria -----------------------------------------------------
// Pantalla 6: confirma la victoria y agrega elementos de celebración.
void PantallaUI::slideVictoria() {
    marcoDoble(4, 4, Pantalla::ANCHO - 8, Pantalla::ALTO - 8, VERDE, 6);

    textoCentrado(CP_EXCL_INV "Lo lograste!", 70, 3, AMARILLO);

    // Confeti vectorial
    const int cx[] = {30, 60, 270, 295, 45, 280};
    const int cy[] = {40, 90, 50, 100, 150, 160};
    const uint16_t cc[] = {ROJO, CIAN, MAGENTA, VERDE, NARANJA, AMARILLO};
    for (int i = 0; i < 6; i++) {
        _tft.fillCircle(cx[i], cy[i], 3, cc[i]);
        _tft.drawLine(cx[i] - 6, cy[i] - 6, cx[i] - 3, cy[i] - 3, cc[i]);
    }

    // Estudiante junto al autobús
    dibujarEstudiante(120, 170);
    dibujarAutobus(160, 170);
}

// ---- Slide 7: Reflexión ----------------------------------------------------
// Pantalla 7: presenta la frase de reflexión provisional al finalizar.
void PantallaUI::slideReflexion() {
    marcoDoble(4, 4, Pantalla::ANCHO - 8, Pantalla::ALTO - 8, MARCO, 6);
    textoCentrado("Reflexi" CP_O_AGUDA "n", 14, 3, CIAN);

    // Recuadro del mensaje
    const int bx = 24, by = 62, bw = Pantalla::ANCHO - 48, bh = 110;
    _tft.drawRect(bx, by, bw, bh, GRIS);
    _tft.drawRect(bx + 1, by + 1, bw - 2, bh - 2, GRIS);

    textoCentrado("Cada meta se logra", by + 18, 2, BLANCO);
    textoCentrado("paso a paso y con",   by + 44, 2, BLANCO);
    textoCentrado("mucha constancia.",   by + 70, 2, BLANCO);

    textoCentrado(CP_EXCL_INV "Gracias por jugar!", 190, 2, VERDE);
}

// El mensaje ocupa la franja inferior de la escena; solo se muestra al ganar/perder.
// Muestra una indicación al finalizar una etapa, sin borrar toda la escena.
void PantallaUI::mensajeEstado(const char *texto, uint16_t color) {
    _tft.fillRect(8, 210, 304, 22, Colores::NEGRO);
    _tft.drawRect(8, 210, 304, 22, Colores::BLANCO);
    _tft.setTextSize(1);
    _tft.setTextColor(color);
    _tft.setCursor(16, 218);
    _tft.print(texto);
}

// Collision AABB de estudiante contra muros geométricos del boceto original.
// Comprueba si la caja del estudiante cruza el borde o toca alguna pared del laberinto.
bool PantallaUI::colisionLaberinto(int x, int y, int w, int h) const {
    if (x < MAZE_X + 2 || y < MAZE_Y + 2 ||
        x + w > MAZE_X + MAZE_W - 2 ||
        y + h > MAZE_Y + MAZE_H - 2) return true;

    for (const Muro &muro : MUROS) {
        int x0 = std::min(escX(muro.x1), escX(muro.x2));
        int x1 = std::max(escX(muro.x1), escX(muro.x2)) + GROSOR;
        int y0 = std::min(escY(muro.y1), escY(muro.y2));
        int y1 = std::max(escY(muro.y1), escY(muro.y2)) + GROSOR;
        if (x < x1 && x+w > x0 && y < y1 && y+h > y0) return true;
    }
    return false;
}

// Restaura el camino cuando los vehículos en movimiento borran parcialmente las marcas.
void PantallaUI::redibujarLineasCarretera() { slideCarretera(); }
