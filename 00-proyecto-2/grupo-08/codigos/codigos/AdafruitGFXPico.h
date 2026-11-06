#pragma once
#include <cstddef>
#include <cstdint>

// DORE 8 - API de dibujo geométrico adaptada a Pico SDK.
// No enlaza las bibliotecas Arduino/Adafruit originales.
// ========================================================================
// Controlador de pantalla: declaracion
// ========================================================================

// utilizado por DORE 8. NO es la biblioteca Adafruit original sin modificar.
// Backend: ST7789V fisico (por defecto) o ILI9341 en Wokwi.
// SPI0: SCK GP18, MOSI GP19, CS GP17, DC GP16; RST GP20 solo fisico.
// Para compilar modo Wokwi definir DORE8_PANTALLA_ILI9341.
// Sin Arduino, sin framebuffer, sin bitmaps de escenas.
// 03 · CLASE GRÁFICA: DECLARACIÓN
// Esta interfaz reúne las operaciones que necesitan los escenarios (texto, líneas y figuras).
// Su API recuerda a Adafruit GFX, pero la implementación es propia para Pico SDK.
class AdafruitGFX_Pico {
public:
    // El constructor recibe los GPIO del selector CS, datos/comandos DC y reinicio RST.
    AdafruitGFX_Pico(uint8_t cs = 17, uint8_t dc = 16, int8_t rst = 20);
    // Inicializa SPI, los pines y el controlador de pantalla.
    void begin();
    // Modifica el giro de la imagen para jugar con la pantalla en horizontal.
    void setRotation(uint8_t r);
    void setCP437(bool activo) { cp437_ = activo; }
    void setTextWrap(bool activo) { wrap_ = activo; }
    void setTextSize(uint8_t tam) { tam_ = tam ? tam : 1; }
    void setTextColor(uint16_t color) { textoColor_ = color; }
    void setCursor(int16_t x, int16_t y) { cursorX_ = x; cursorY_ = y; }
    std::size_t print(const char *txt);
    std::size_t print(int valor);
    void getTextBounds(const char *txt, int16_t x, int16_t y,
                       int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) const;
    // Primitivas de dibujo que forman paredes, vehículos, personajes y marcos.
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t ancho, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillScreen(uint16_t color);
    void drawCircle(int16_t cx, int16_t cy, int16_t radio, uint16_t color);
    void fillCircle(int16_t cx, int16_t cy, int16_t radio, uint16_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radio, uint16_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radio, uint16_t color);
    // Triángulos utilizados para pequeños detalles e indicadores de la interfaz.
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color);

private:
    uint8_t cs_, dc_;
    int8_t rst_;  // -1 desactiva el reinicio fisico.
    uint8_t tam_ = 1;
    uint8_t rotacion_ = 1;
    bool cp437_ = false;
    bool wrap_ = false;
    int16_t cursorX_ = 0, cursorY_ = 0;
    uint16_t textoColor_ = 0xFFFF;
    int16_t ancho_ = 320, alto_ = 240;
    void comando(uint8_t valor);
    void datos(const uint8_t *valores, std::size_t n);
    void ventana(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void circuloPuntos(int16_t cx, int16_t cy, int16_t x, int16_t y, uint16_t color);
    void dibujarCaracter(uint8_t caracter, int16_t x, int16_t y);
};

// Colores RGB565 compatibles con constantes usadas por Adafruit ST77xx.
// 04 · COLORES
// Se identifican con valores RGB565 de 16 bits, que entiende el controlador TFT.
constexpr uint16_t ST77XX_BLACK   = 0x0000;
constexpr uint16_t ST77XX_WHITE   = 0xFFFF;
constexpr uint16_t ST77XX_RED     = 0xF800;
constexpr uint16_t ST77XX_GREEN   = 0x07E0;
constexpr uint16_t ST77XX_BLUE    = 0x001F;
constexpr uint16_t ST77XX_CYAN    = 0x07FF;
constexpr uint16_t ST77XX_MAGENTA = 0xF81F;
constexpr uint16_t ST77XX_YELLOW  = 0xFFE0;
constexpr uint16_t ST77XX_ORANGE  = 0xFD20;
