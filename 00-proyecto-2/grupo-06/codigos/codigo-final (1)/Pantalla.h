#ifndef PANTALLA_H
#define PANTALLA_H

#include <cstdint>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Driver mínimo para la OLED SSD1306 de 128x64 por I2C.
// Wokwi no trae librería de pantalla para el Pico SDK, así que la hablamos
// directo: mandamos comandos de configuración y luego los 1024 bytes del dibujo.
//
// Conexión (igual que tu diagram.json): SDA = GP4, SCL = GP5, dirección 0x3C.
class Pantalla
{
  public:
    static const int ANCHO = 128;
    static const int ALTO = 64;
    static const int BYTES_BUFFER = ANCHO * ALTO / 8; // 1024

    Pantalla(i2c_inst_t* bus = i2c0, uint pinSDA = 4, uint pinSCL = 5, uint8_t direccion = 0x3C);

    // prepara el bus I2C y configura la pantalla; se llama UNA vez al inicio
    void iniciar();

    // manda un framebuffer de 1024 bytes (formato por páginas) a la pantalla
    void enviar(const uint8_t* buffer);

    // apaga / enciende el panel (la memoria se conserva)
    void apagar();
    void encender();

    i2c_inst_t* bus;
    uint pinSDA;
    uint pinSCL;
    uint8_t direccion;

    // control 0x00 = lo que sigue es un comando; 0x40 = lo que sigue son datos
    void comando(uint8_t c);
};

#endif