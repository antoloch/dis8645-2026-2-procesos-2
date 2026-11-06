#include "Pantalla.h"

// Si la imagen sale al revés (de cabeza), cambia 0xA1 por 0xA0 y 0xC8 por 0xC0.
static const uint8_t SEG_REMAP = 0xA1;
static const uint8_t COM_SCAN = 0xC8;

Pantalla::Pantalla(i2c_inst_t* bus, uint pinSDA, uint pinSCL, uint8_t direccion)
    : bus(bus), pinSDA(pinSDA), pinSCL(pinSCL), direccion(direccion)
{
}

void Pantalla::comando(uint8_t c)
{
    uint8_t paquete[2] = {0x00, c};
    i2c_write_blocking(bus, direccion, paquete, 2, false);
}

void Pantalla::iniciar()
{
    i2c_init(bus, 400 * 1000); // 400 kHz
    gpio_set_function(pinSDA, GPIO_FUNC_I2C);
    gpio_set_function(pinSCL, GPIO_FUNC_I2C);
    gpio_pull_up(pinSDA);
    gpio_pull_up(pinSCL);

    // secuencia de arranque estándar del SSD1306 128x64
    static const uint8_t arranque[] = {
        0xAE,        // pantalla apagada mientras se configura
        0x20, 0x00,  // modo de direccionamiento horizontal
        0x40,        // línea de inicio = 0
        SEG_REMAP,   // espejo horizontal
        0xA8, 0x3F,  // 64 filas
        COM_SCAN,    // espejo vertical
        0xD3, 0x00,  // sin desplazamiento vertical
        0xD5, 0x80,  // frecuencia del reloj
        0xD9, 0xF1,  // precarga
        0xDA, 0x12,  // configuración de pines COM
        0xDB, 0x30,  // nivel VCOMH
        0x81, 0xFF,  // contraste máximo
        0xA4,        // mostrar lo que hay en memoria
        0xA6,        // normal (1 = pixel encendido)
        0x8D, 0x14,  // bomba de carga interna
        0xAF         // pantalla encendida
    };
    for (unsigned i = 0; i < sizeof(arranque); i++)
    {
        comando(arranque[i]);
    }
}

void Pantalla::enviar(const uint8_t* buffer)
{
    // columnas 0 a 127 y páginas 0 a 7: el dibujo completo
    comando(0x21); comando(0); comando(ANCHO - 1);
    comando(0x22); comando(0); comando(ALTO / 8 - 1);

    // un solo paquete: byte de control 0x40 + los 1024 bytes del dibujo
    static uint8_t paquete[BYTES_BUFFER + 1];
    paquete[0] = 0x40;
    for (int i = 0; i < BYTES_BUFFER; i++)
    {
        paquete[i + 1] = buffer[i];
    }
    i2c_write_blocking(bus, direccion, paquete, BYTES_BUFFER + 1, false);
}

void Pantalla::apagar()
{
    comando(0xAE);
}

void Pantalla::encender()
{
    comando(0xAF);
}