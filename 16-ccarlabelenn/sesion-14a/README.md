# sesion-14a

## apuntes sesión

+ **definición de la idea:** evaluamos y descartamos varias opciones (desde las más simples hasta las más ambiciosas) porque nada nos terminaba de convencer, hasta definir la propuesta final.
+ **mecánica del sistema:** elegimos hacer **dos máquinas tipo stream deck de 4 botones cada una** (una con 4 tonos agudos y otra con 4 tonos graves). la idea es que dos personas las toquen de manera simultánea para enviar las frecuencias a un parlante y generar las **figuras de Lissajous** con la vibración.
+ **concepto:** buscamos que sea una interacción efímera donde dos personas creen juntas sin dejar rastro.
+ **pruebas en Wokwi:** estuvimos haciendo varias simulaciones en **Wokwi** probando distintos códigos generados con ayuda de gemini para validar el funcionamiento de los botones, tonos y lo que se vería en la pantalla.

### códigos de prueba 
**uno de los primeros codigos de prueba con palabras, movimiento y con potenciometros** 
```
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/uart.h"

// UART Monitor Serial
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Pines SPI para TFT ILI9341 (240x320)
#define SPI_PORT spi0
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_CS   17
#define PIN_RST  20
#define PIN_DC   21

// Colores RGB565
#define COLOR_BLACK   0x0000
#define COLOR_CYAN    0x07FF // Palabra Persona 1
#define COLOR_MAGENTA 0xF81F // Palabra Persona 2
#define COLOR_WHITE   0xFFFF // Interseccion / Fusion
#define COLOR_GRAY    0x4208 // Guias

// Botones de velocidad
#define BTN_SPEED_1 14
#define BTN_SPEED_2 15

// Configuracion de palabras punteadas
#define MAX_WORD_LEN 10
#define MAX_DOTS 350 // Maximo de puntos por palabra (10 letras * 35 puntos max)

char word1[MAX_WORD_LEN + 1] = "HOLA";
char word2[MAX_WORD_LEN + 1] = "PICO";

// Arreglos para guardar coordenadas actuales y anteriores (evita parpadeo)
int16_t curr_x1[MAX_DOTS], curr_y1[MAX_DOTS];
int16_t curr_x2[MAX_DOTS], curr_y2[MAX_DOTS];
int16_t prev_x1[MAX_DOTS], prev_y1[MAX_DOTS];
int16_t prev_x2[MAX_DOTS], prev_y2[MAX_DOTS];
int count_dots_1 = 0, prev_count_1 = 0;
int count_dots_2 = 0, prev_count_2 = 0;

const float speeds[3] = {0.5f, 1.0f, 2.0f};
const char* speed_names[3] = {"LENTO (0.5x)", "NORMAL (1.0x)", "RAPIDO (2.5x)"};
int speed_idx_1 = 1;
int speed_idx_2 = 1;

// --- Fuente Punteada 5x7 (A-Z, 0-9, espacio) ---
const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // ' ' (0)
    {0x7E,0x11,0x11,0x11,0x7E}, // A (1)
    {0x7F,0x49,0x49,0x49,0x36}, // B (2)
    {0x3E,0x41,0x41,0x41,0x22}, // C (3)
    {0x7F,0x41,0x41,0x22,0x1C}, // D (4)
    {0x7F,0x49,0x49,0x49,0x41}, // E (5)
    {0x7F,0x09,0x09,0x09,0x01}, // F (6)
    {0x3E,0x41,0x49,0x49,0x7A}, // G (7)
    {0x7F,0x08,0x08,0x08,0x7F}, // H (8)
    {0x00,0x41,0x7F,0x41,0x00}, // I (9)
    {0x20,0x40,0x41,0x3F,0x01}, // J (10)
    {0x7F,0x08,0x14,0x22,0x41}, // K (11)
    {0x7F,0x40,0x40,0x40,0x40}, // L (12)
    {0x7F,0x02,0x0C,0x02,0x7F}, // M (13)
    {0x7F,0x04,0x08,0x10,0x7F}, // N (14)
    {0x3E,0x41,0x41,0x41,0x3E}, // O (15)
    {0x7F,0x09,0x09,0x09,0x06}, // P (16)
    {0x3E,0x41,0x51,0x21,0x5E}, // Q (17)
    {0x7F,0x09,0x19,0x29,0x46}, // R (18)
    {0x46,0x49,0x49,0x49,0x31}, // S (19)
    {0x01,0x01,0x7F,0x01,0x01}, // T (20)
    {0x3F,0x40,0x40,0x40,0x3F}, // U (21)
    {0x1F,0x20,0x40,0x20,0x1F}, // V (22)
    {0x3F,0x40,0x38,0x40,0x3F}, // W (23)
    {0x63,0x14,0x08,0x14,0x63}, // X (24)
    {0x07,0x08,0x70,0x08,0x07}, // Y (25)
    {0x61,0x51,0x49,0x45,0x43}, // Z (26)
    {0x3E,0x51,0x49,0x45,0x3E}, // 0 (27)
    {0x00,0x42,0x7F,0x40,0x00}, // 1 (28)
    {0x42,0x61,0x51,0x49,0x46}, // 2 (29)
    {0x21,0x41,0x45,0x4B,0x31}, // 3 (30)
    {0x18,0x14,0x12,0x7F,0x10}, // 4 (31)
    {0x27,0x45,0x45,0x45,0x39}, // 5 (32)
    {0x3C,0x4A,0x49,0x49,0x30}, // 6 (33)
    {0x01,0x71,0x09,0x05,0x03}, // 7 (34)
    {0x36,0x49,0x49,0x49,0x36}, // 8 (35)
    {0x06,0x49,0x49,0x29,0x1E}  // 9 (36)
};

int get_glyph_index(char c) {
    c = toupper((unsigned char)c);
    if (c >= 'A' && c <= 'Z') return (c - 'A') + 1;
    if (c >= '0' && c <= '9') return (c - '0') + 27;
    return 0; // Espacio o caracter no soportado
}

// --- Driver rapido ILI9341 por SPI nativo ---
static inline void tft_cmd(uint8_t cmd) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);
}

static inline void tft_data(uint8_t data) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(PIN_CS, 1);
}

void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    tft_cmd(0x2A);
    tft_data(x0 >> 8); tft_data(x0 & 0xFF);
    tft_data(x1 >> 8); tft_data(x1 & 0xFF);
    tft_cmd(0x2B);
    tft_data(y0 >> 8); tft_data(y0 & 0xFF);
    tft_data(y1 >> 8); tft_data(y1 & 0xFF);
    tft_cmd(0x2C);
}

void tft_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    tft_set_window(x, y, x + w - 1, y + h - 1);
    uint8_t hi = color >> 8, lo = color & 0xFF;
    uint8_t line[480];
    for (int i = 0; i < w; i++) {
        line[i * 2] = hi;
        line[i * 2 + 1] = lo;
    }
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int row = 0; row < h; row++) {
        spi_write_blocking(SPI_PORT, line, w * 2);
    }
    gpio_put(PIN_CS, 1);
}

static inline void tft_draw_pixel(int x, int y, uint16_t color) {
    if (x < 0 || x >= 240 || y < 0 || y >= 320) return;
    tft_set_window(x, y, x, y);
    uint8_t buf[2] = {color >> 8, color & 0xFF};
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

// Dibuja un punto grueso (2x2) para el efecto punteado
static inline void tft_draw_dot(int x, int y, uint16_t color) {
    tft_draw_pixel(x, y, color);
    tft_draw_pixel(x + 1, y, color);
    tft_draw_pixel(x, y + 1, color);
    tft_draw_pixel(x + 1, y + 1, color);
}

void tft_init(void) {
    spi_init(SPI_PORT, 40 * 1000 * 1000); // 40 MHz SPI
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);  gpio_set_dir(PIN_CS, GPIO_OUT);  gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC);  gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_put(PIN_RST, 0); sleep_ms(50);
    gpio_put(PIN_RST, 1); sleep_ms(120);

    tft_cmd(0x01); sleep_ms(120); // Software reset
    tft_cmd(0x3A); tft_data(0x55); // Formato 16-bit RGB565
    tft_cmd(0x36); tft_data(0x48); // Orientacion vertical
    tft_cmd(0x11); sleep_ms(120); // Salir de sleep
    tft_cmd(0x29);                 // Encender display

    tft_fill_rect(0, 0, 240, 320, COLOR_BLACK);
}

// Lee comandos por UART sin bloquear el bucle de animacion
void check_serial_input(void) {
    static char rx_buf[32];
    static int rx_idx = 0;

    while (uart_is_readable(UART_ID)) {
        char ch = uart_getc(UART_ID);
        if (ch == '\r' || ch == '\n') {
            if (rx_idx > 0) {
                rx_buf[rx_idx] = '\0';
                // Formato esperado: "1:PALABRA" o "2:PALABRA"
                if (rx_idx > 2 && rx_buf[1] == ':') {
                    if (rx_buf[0] == '1') {
                        strncpy(word1, &rx_buf[2], MAX_WORD_LEN);
                        word1[MAX_WORD_LEN] = '\0';
                        uart_puts(UART_ID, "-> Palabra 1 (Cian) actualizada!\r\n");
                    } else if (rx_buf[0] == '2') {
                        strncpy(word2, &rx_buf[2], MAX_WORD_LEN);
                        word2[MAX_WORD_LEN] = '\0';
                        uart_puts(UART_ID, "-> Palabra 2 (Magenta) actualizada!\r\n");
                    }
                } else {
                    // Si escribe directo sin prefijo, actualiza la Palabra 1
                    strncpy(word1, rx_buf, MAX_WORD_LEN);
                    word1[MAX_WORD_LEN] = '\0';
                    uart_puts(UART_ID, "-> Palabra 1 actualizada (usa 2:TEXTO para la segunda)\r\n");
                }
                rx_idx = 0;
            }
        } else if (rx_idx < 30) {
            rx_buf[rx_idx++] = ch;
        }
    }
}

// Calcula las posiciones (X, Y) de todos los puntos de una palabra con efecto onda y salto
int compute_word_dots(const char* word, int16_t* out_x, int16_t* out_y,
                      float amp_wave, float freq_wave, float phase,
                      float orbit_radius, int base_y, bool invert_dir) {
    int len = strlen(word);
    if (len == 0) return 0;

    // Ajuste dinamico de escala segun el largo de la palabra para que quepa en 240px
    int dot_step = (len <= 5) ? 5 : (len <= 7 ? 4 : 3);
    int char_width = 5 * dot_step;
    int char_spacing = dot_step * 2;
    int total_w = len * char_width + (len - 1) * char_spacing;
    int start_x = (240 - total_w) / 2;

    int dot_count = 0;
    float dir = invert_dir ? -1.0f : 1.0f;

    for (int c = 0; c < len; c++) {
        int glyph = get_glyph_index(word[c]);
        int char_x0 = start_x + c * (char_width + char_spacing);

        // Efecto de salto individual por letra
        float letter_jump = sinf(phase * 1.4f + c * 0.9f * dir) * (amp_wave * 0.35f);

        for (int col = 0; col < 5; col++) {
            uint8_t col_bits = font5x7[glyph][col];
            for (int row = 0; row < 7; row++) {
                if (col_bits & (1 << row)) {
                    if (dot_count >= MAX_DOTS) return dot_count;

                    int bx = char_x0 + col * dot_step;
                    int by = base_y + (row - 3) * dot_step;

                    // Onda viajera sobre cada punto + orbita de entrelazado
                    float wave_arg = (bx * 0.035f * freq_wave) + (phase * dir);
                    int dx = (int)(cosf(wave_arg) * orbit_radius);
                    int dy = (int)(sinf(wave_arg) * amp_wave + letter_jump);

                    out_x[dot_count] = bx + dx;
                    out_y[dot_count] = by + dy;
                    dot_count++;
                }
            }
        }
    }
    return dot_count;
}

int main(void) {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    tft_init();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    gpio_init(BTN_SPEED_1); gpio_set_dir(BTN_SPEED_1, GPIO_IN); gpio_pull_up(BTN_SPEED_1);
    gpio_init(BTN_SPEED_2); gpio_set_dir(BTN_SPEED_2, GPIO_IN); gpio_pull_up(BTN_SPEED_2);

    float phase1 = 0.0f;
    float phase2 = 0.0f;

    uint64_t last_press_1 = 0;
    uint64_t last_press_2 = 0;
    bool prev_btn_1 = true;
    bool prev_btn_2 = true;

    // Guias sutiles superior e inferior
    tft_fill_rect(0, 20, 240, 1, COLOR_GRAY);
    tft_fill_rect(0, 300, 240, 1, COLOR_GRAY);

    uart_puts(UART_ID, "=== PALABRAS PUNTEADAS ENTRELAZADAS ===\r\n");
    uart_puts(UART_ID, "Escribe '1:TEXTO' para Palabra Cian o '2:TEXTO' para Palabra Magenta\r\n");
    uart_puts(UART_ID, "Pot 1: Salto/Amplitud | Pot 2: Onda/Entrelazado\r\n");

    while (true) {
        uint64_t now = time_us_64();

        // 0. Revisar si el usuario escribio nuevas palabras por Serial
        check_serial_input();

        // 1. Lectura ADC en tiempo real
        adc_select_input(0);
        uint16_t raw_x = adc_read();
        adc_select_input(1);
        uint16_t raw_y = adc_read();

        // Pot 1 controla cuanto saltan/ondulan las palabras (5px a 65px)
        float amp_wave = 5.0f + (raw_x / 4095.0f) * 60.0f;
        // Pot 2 controla la frecuencia de la onda y el entrelazado
        float freq_wave = 0.4f + (raw_y / 4095.0f) * 2.2f;
        float orbit_rad = (raw_y / 4095.0f) * 14.0f;

        // 2. Boton 1 (Velocidad Palabra 1)
        bool curr_btn_1 = gpio_get(BTN_SPEED_1);
        if (!curr_btn_1 && prev_btn_1 && (now - last_press_1 > 200000)) {
            speed_idx_1 = (speed_idx_1 + 1) % 3;
            last_press_1 = now;
        }
        prev_btn_1 = curr_btn_1;

        // 3. Boton 2 (Velocidad Palabra 2)
        bool curr_btn_2 = gpio_get(BTN_SPEED_2);
        if (!curr_btn_2 && prev_btn_2 && (now - last_press_2 > 200000)) {
            speed_idx_2 = (speed_idx_2 + 1) % 3;
            last_press_2 = now;
        }
        prev_btn_2 = curr_btn_2;

        // 4. Calcular posiciones de ambas palabras punteadas
        // Ambas orbitan alrededor del centro (Y=145 y Y=175) para entrelazarse al subir amp_wave
        count_dots_1 = compute_word_dots(word1, curr_x1, curr_y1, amp_wave, freq_wave, phase1, orbit_rad, 145, false);
        count_dots_2 = compute_word_dots(word2, curr_x2, curr_y2, amp_wave, freq_wave, phase2, orbit_rad, 175, true);

        // 5. Borrar los puntos del frame anterior
        for (int i = 0; i < prev_count_1; i++) {
            tft_draw_dot(prev_x1[i], prev_y1[i], COLOR_BLACK);
        }
        for (int i = 0; i < prev_count_2; i++) {
            tft_draw_dot(prev_x2[i], prev_y2[i], COLOR_BLACK);
        }

        // 6. Dibujar Palabra 1 (Cian) y Palabra 2 (Magenta), iluminando en Blanco si se cruzan
        for (int i = 0; i < count_dots_1; i++) {
            tft_draw_dot(curr_x1[i], curr_y1[i], COLOR_CYAN);
        }

        for (int j = 0; j < count_dots_2; j++) {
            bool cruce = false;
            // Verificar si este punto se entrelaza/cruza con algun punto de la Palabra 1
            for (int i = 0; i < count_dots_1; i += 2) {
                if (abs(curr_x2[j] - curr_x1[i]) < 4 && abs(curr_y2[j] - curr_y1[i]) < 4) {
                    cruce = true;
                    break;
                }
            }
            tft_draw_dot(curr_x2[j], curr_y2[j], cruce ? COLOR_WHITE : COLOR_MAGENTA);
        }

        // 7. Guardar frame actual para borrarlo en la siguiente iteracion
        memcpy(prev_x1, curr_x1, count_dots_1 * sizeof(int16_t));
        memcpy(prev_y1, curr_y1, count_dots_1 * sizeof(int16_t));
        prev_count_1 = count_dots_1;

        memcpy(prev_x2, curr_x2, count_dots_2 * sizeof(int16_t));
        memcpy(prev_y2, curr_y2, count_dots_2 * sizeof(int16_t));
        prev_count_2 = count_dots_2;

        // Avanzar fase segun velocidad elegida
        phase1 += 0.08f * speeds[speed_idx_1];
        phase2 += 0.08f * speeds[speed_idx_2];

        sleep_ms(15);
    }

    return 0;
}
```

**más codigos de prueba de movimiento**
```
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/spi.h"
#include "hardware/uart.h"

// UART Monitor Serial
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Pines SPI para TFT ILI9341 (240x320)
#define SPI_PORT spi0
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_CS   17
#define PIN_RST  20
#define PIN_DC   21

// Colores RGB565
#define COLOR_BLACK   0x0000
#define COLOR_CYAN    0x07FF // Onda Potenciometro 1
#define COLOR_MAGENTA 0xF81F // Onda Potenciometro 2
#define COLOR_WHITE   0xFFFF // Punto donde ambas ondas se cruzan/fusionan

// Botones de velocidad
#define BTN_SPEED_1 14
#define BTN_SPEED_2 15

// Buffer circular efimero en RAM
#define BUFFER_SIZE 256
#define NUM_POINTS  140

uint16_t buffer_p1[BUFFER_SIZE];
uint16_t buffer_p2[BUFFER_SIZE];

const float speeds[3] = {0.5f, 1.0f, 2.0f};
const char* speed_names[3] = {"LENTO (0.5x)", "NORMAL (1.0x)", "RAPIDO (2.0x)"};
int speed_idx_1 = 1;
int speed_idx_2 = 1;

// --- Driver rapido ILI9341 por SPI nativo ---
static inline void tft_cmd(uint8_t cmd) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);
}

static inline void tft_data(uint8_t data) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(PIN_CS, 1);
}

void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    tft_cmd(0x2A);
    tft_data(x0 >> 8); tft_data(x0 & 0xFF);
    tft_data(x1 >> 8); tft_data(x1 & 0xFF);
    tft_cmd(0x2B);
    tft_data(y0 >> 8); tft_data(y0 & 0xFF);
    tft_data(y1 >> 8); tft_data(y1 & 0xFF);
    tft_cmd(0x2C);
}

void tft_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    tft_set_window(x, y, x + w - 1, y + h - 1);
    uint8_t hi = color >> 8, lo = color & 0xFF;
    uint8_t line[480];
    for (int i = 0; i < w; i++) {
        line[i * 2] = hi;
        line[i * 2 + 1] = lo;
    }
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int row = 0; row < h; row++) {
        spi_write_blocking(SPI_PORT, line, w * 2);
    }
    gpio_put(PIN_CS, 1);
}

static inline void tft_draw_pixel(int x, int y, uint16_t color) {
    if (x < 0 || x >= 240 || y < 0 || y >= 320) return;
    tft_set_window(x, y, x, y);
    uint8_t buf[2] = {color >> 8, color & 0xFF};
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

static inline void tft_draw_dot(int x, int y, uint16_t color) {
    tft_draw_pixel(x, y, color);
    tft_draw_pixel(x + 1, y, color);
    tft_draw_pixel(x, y + 1, color);
    tft_draw_pixel(x + 1, y + 1, color);
}

void tft_init(void) {
    spi_init(SPI_PORT, 40 * 1000 * 1000);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);  gpio_set_dir(PIN_CS, GPIO_OUT);  gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC);  gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT);

    gpio_put(PIN_RST, 0); sleep_ms(50);
    gpio_put(PIN_RST, 1); sleep_ms(120);

    tft_cmd(0x01); sleep_ms(120);
    tft_cmd(0x3A); tft_data(0x55);
    tft_cmd(0x36); tft_data(0x48);
    tft_cmd(0x11); sleep_ms(120);
    tft_cmd(0x29);

    tft_fill_rect(0, 0, 240, 320, COLOR_BLACK);
}

void log_estado_serial(const char* evento, float f1, float f2) {
    char msg[128];
    snprintf(msg, sizeof(msg),
             "[%s] Onda Cian (P1): %s [F:%.2f] <---> Onda Magenta (P2): %s [F:%.2f]\r\n",
             evento, speed_names[speed_idx_1], f1, speed_names[speed_idx_2], f2);
    uart_puts(UART_ID, msg);
}

int main(void) {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    tft_init();

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    gpio_init(BTN_SPEED_1); gpio_set_dir(BTN_SPEED_1, GPIO_IN); gpio_pull_up(BTN_SPEED_1);
    gpio_init(BTN_SPEED_2); gpio_set_dir(BTN_SPEED_2, GPIO_IN); gpio_pull_up(BTN_SPEED_2);

    uint32_t write_idx = 0;
    float read_idx_1 = 0.0f;
    float read_idx_2 = 0.0f;
    float phase = 0.0f;

    uint64_t last_press_1 = 0;
    uint64_t last_press_2 = 0;
    bool prev_btn_1 = true;
    bool prev_btn_2 = true;

    // Guardar posiciones previas de ambas ondas entrelazadas
    int prev_x1[NUM_POINTS], prev_y1[NUM_POINTS];
    int prev_x2[NUM_POINTS], prev_y2[NUM_POINTS];
    for (int i = 0; i < NUM_POINTS; i++) {
        prev_x1[i] = 120; prev_y1[i] = 160;
        prev_x2[i] = 120; prev_y2[i] = 160;
    }

    uart_puts(UART_ID, "=== ONDAS ENTRELAZADAS LISSAJOUS ===\r\n");
    uart_puts(UART_ID, "Cian = Potenciometro 1 | Magenta = Potenciometro 2 | Blanco = Interseccion\r\n");

    while (true) {
        uint64_t now = time_us_64();

        // 1. Lectura ADC en tiempo real
        adc_select_input(0);
        uint16_t raw_x = adc_read();
        adc_select_input(1);
        uint16_t raw_y = adc_read();

        buffer_p1[write_idx] = raw_x;
        buffer_p2[write_idx] = raw_y;
        write_idx = (write_idx + 1) % BUFFER_SIZE;

        // Rango de frecuencia mas expresivo (de 1.0 a 5.0 multiplicado por velocidad)
        float base_f1 = 1.0f + (raw_x / 4095.0f) * 4.0f;
        float base_f2 = 1.0f + (raw_y / 4095.0f) * 4.0f;
        float eff_f1 = base_f1 * speeds[speed_idx_1];
        float eff_f2 = base_f2 * speeds[speed_idx_2];

        // 2. Boton 1 (Debounce exacto 200 ms)
        bool curr_btn_1 = gpio_get(BTN_SPEED_1);
        if (!curr_btn_1 && prev_btn_1 && (now - last_press_1 > 200000)) {
            speed_idx_1 = (speed_idx_1 + 1) % 3;
            last_press_1 = now;
            eff_f1 = base_f1 * speeds[speed_idx_1];
            log_estado_serial("BOTON 1", eff_f1, eff_f2);
        }
        prev_btn_1 = curr_btn_1;

        // 3. Boton 2 (Debounce exacto 200 ms)
        bool curr_btn_2 = gpio_get(BTN_SPEED_2);
        if (!curr_btn_2 && prev_btn_2 && (now - last_press_2 > 200000)) {
            speed_idx_2 = (speed_idx_2 + 1) % 3;
            last_press_2 = now;
            eff_f2 = base_f2 * speeds[speed_idx_2];
            log_estado_serial("BOTON 2", eff_f1, eff_f2);
        }
        prev_btn_2 = curr_btn_2;

        // 4. Dibujar ambas ondas entrelazadas en el mismo espacio central
        for (int i = 0; i < NUM_POINTS; i++) {
            int idx1 = ((int)(read_idx_1 + i * speeds[speed_idx_1])) % BUFFER_SIZE;
            int idx2 = ((int)(read_idx_2 + i * speeds[speed_idx_2])) % BUFFER_SIZE;

            float f1 = (1.0f + (buffer_p1[idx1] / 4095.0f) * 4.0f) * speeds[speed_idx_1];
            float f2 = (1.0f + (buffer_p2[idx2] / 4095.0f) * 4.0f) * speeds[speed_idx_2];

            float t = i * 0.065f;

            // Borrar trazos del fotograma anterior
            tft_draw_dot(prev_x1[i], prev_y1[i], COLOR_BLACK);
            tft_draw_dot(prev_x2[i], prev_y2[i], COLOR_BLACK);

            // Onda CIAN (Persona 1): Eje X gobernado por P1, modulado en Y por P2
            int x1 = 120 + (int)(100.0f * sinf(f1 * t + phase));
            int y1 = 160 + (int)(135.0f * sinf(f2 * t));

            // Onda MAGENTA (Persona 2): Complementaria desfasada y cruzada con P1
            int x2 = 120 + (int)(100.0f * cosf(f2 * t - phase));
            int y2 = 160 + (int)(135.0f * sinf(f1 * t + 1.57f));

            // Si ambas ondas se cruzan o estan muy cerca, brillan en BLANCO (fusion)
            if (abs(x1 - x2) < 4 && abs(y1 - y2) < 4) {
                tft_draw_dot(x1, y1, COLOR_WHITE);
                tft_draw_dot(x2, y2, COLOR_WHITE);
            } else {
                tft_draw_dot(x1, y1, COLOR_CYAN);
                tft_draw_dot(x2, y2, COLOR_MAGENTA);
            }

            prev_x1[i] = x1; prev_y1[i] = y1;
            prev_x2[i] = x2; prev_y2[i] = y2;
        }

        read_idx_1 += speeds[speed_idx_1];
        if (read_idx_1 >= BUFFER_SIZE) read_idx_1 -= BUFFER_SIZE;

        read_idx_2 += speeds[speed_idx_2];
        if (read_idx_2 >= BUFFER_SIZE) read_idx_2 -= BUFFER_SIZE;

        phase += 0.05f;
        sleep_ms(15);
    }

    return 0;
}
```
**codigo inicial del proyecto final, que probamos en clase y mejoramos durante el día miércoles**
```
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#define I2C_PORT i2c0
#define OLED_ADDR 0x3C

// Pines de los dos teclados (4 botones por persona)
const uint botonesA[4] = {2, 3, 4, 5};
const uint botonesB[4] = {6, 7, 8, 9};

// Escala pentatónica mayor en dos octavas (siempre suena armónica)
const float notasA[4] = {261.63f, 293.66f, 329.63f, 392.00f}; // Do4, Re4, Mi4, Sol4
const float notasB[4] = {523.25f, 587.33f, 659.25f, 783.99f}; // Do5, Re5, Mi5, Sol5

#define BUZZER_A 14
#define BUZZER_B 15

// Buffer gráfico para pantalla OLED 128x64 (1024 bytes)
uint8_t buffer[128 * 64 / 8];

void oled_cmd(uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd};
    i2c_write_blocking(I2C_PORT, OLED_ADDR, data, 2, false);
}

void oled_init(void) {
    uint8_t cmds[] = {
        0xAE, 0x20, 0x00, 0xB0, 0xC8, 0x00, 0x10, 0x40,
        0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F, 0xA4, 0xD3,
        0x00, 0xD5, 0xF0, 0xD9, 0x22, 0xDA, 0x12, 0xDB,
        0x20, 0x8D, 0x14, 0xAF
    };
    for (uint i = 0; i < sizeof(cmds); i++) {
        oled_cmd(cmds[i]);
    }
}

void oled_clear(void) {
    for (int i = 0; i < 1024; i++) {
        buffer[i] = 0x00;
    }
}

void oled_pixel(int x, int y) {
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;
    buffer[x + (y / 8) * 128] |= (1 << (y % 8));
}

void oled_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        oled_pixel(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void oled_update(void) {
    oled_cmd(0x21); oled_cmd(0); oled_cmd(127);
    oled_cmd(0x22); oled_cmd(0); oled_cmd(7);
    uint8_t out[1025];
    out[0] = 0x40;
    for (int i = 0; i < 1024; i++) {
        out[i + 1] = buffer[i];
    }
    i2c_write_blocking(I2C_PORT, OLED_ADDR, out, 1025, false);
}

void set_tone(uint gpio, float freq) {
    uint slice = pwm_gpio_to_slice_num(gpio);
    if (freq <= 0.0f) {
        pwm_set_enabled(slice, false);
        return;
    }
    float divider = 64.0f;
    uint32_t wrap = (uint32_t)(1953125.0f / freq);
    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), wrap / 2);
    pwm_set_enabled(slice, true);
}

// Convierte la máscara de botones presionados (1..15) en un armónico entero (1..5)
// como en la tabla de referencia (1:1, 2:1, 3:2, 4:3, 5:4, etc.)
int obtener_armonico(int mask) {
    switch (mask) {
        case 1:  return 1; // Botón 1 solo -> 1
        case 2:  return 2; // Botón 2 solo -> 2
        case 4:  return 3; // Botón 3 solo -> 3
        case 8:  return 4; // Botón 4 solo -> 4
        default: return (mask % 5) + 1; // Combinaciones múltiples -> entre 1 y 5
    }
}

int main(void) {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    oled_init();

    for (int i = 0; i < 4; i++) {
        gpio_init(botonesA[i]);
        gpio_set_dir(botonesA[i], GPIO_IN);
        gpio_pull_up(botonesA[i]);

        gpio_init(botonesB[i]);
        gpio_set_dir(botonesB[i], GPIO_IN);
        gpio_pull_up(botonesB[i]);
    }

    gpio_set_function(BUZZER_A, GPIO_FUNC_PWM);
    gpio_set_function(BUZZER_B, GPIO_FUNC_PWM);

    float delta = 0.0f; // Representa el parámetro k (desfase) de tu referente
    uint32_t tick = 0;

    while (true) {
        float activasA[4], activasB[4];
        int actA = 0, actB = 0;
        int maskA = 0, maskB = 0;

        for (int i = 0; i < 4; i++) {
            if (!gpio_get(botonesA[i])) {
                activasA[actA++] = notasA[i];
                maskA |= (1 << i);
            }
            if (!gpio_get(botonesB[i])) {
                activasB[actB++] = notasB[i];
                maskB |= (1 << i);
            }
        }

        // --- SÍNTESIS SONORA EXPRESIVA ---
        float freqSalidaA = 0.0f;
        float freqSalidaB = 0.0f;

        if (actA > 0) {
            int divA = (actB > 0) ? (5 + (maskB % 4)) : 8;
            int idxA = (tick / divA) % actA;
            freqSalidaA = activasA[idxA];
            if (actB > 0) freqSalidaA += 6.0f * sinf(delta * (float)actB);
        }

        if (actB > 0) {
            int divB = (actA > 0) ? (4 + (maskA % 4)) : 7;
            int idxB = (tick / divB) % actB;
            freqSalidaB = activasB[idxB];
            if (actA > 0) freqSalidaB += 8.0f * cosf(delta * (float)actA);
        }

        set_tone(BUZZER_A, freqSalidaA);
        set_tone(BUZZER_B, freqSalidaB);

        // --- VISUALIZACIÓN: CURVAS DE LISSAJOUS PURAS ---
        oled_clear();

        if (actA > 0 && actB > 0) {
            // Relación nx : ny exactamente como en el referente
            int nx = obtener_armonico(maskA);
            int ny = obtener_armonico(maskB);

            // Amplitud centrada en la pantalla 128x64 (margen de 4px arriba/abajo)
            float ampX = 46.0f;
            float ampY = 27.0f;

            int prevX = -1, prevY = -1;
            int pasos = 160; // Alta definición para que las curvas cerradas sean suaves

            for (int i = 0; i <= pasos; i++) {
                float t = (6.283185f * i) / pasos;
                int x = 64 + (int)(ampX * sinf(nx * t + delta));
                int y = 32 + (int)(ampY * sinf(ny * t));

                if (prevX != -1) {
                    oled_line(prevX, prevY, x, y);
                }
                prevX = x;
                prevY = y;
            }

            // Avanza el desfase k suavemente para mostrar toda la rotación de la curva
            delta += 0.045f;
            if (delta > 6.283185f) delta -= 6.283185f;
        } 
        else if (actA > 0) {
            // Solo Persona A: Oscilación horizontal pura (eje X sin eje Y)
            int nx = obtener_armonico(maskA);
            int x = 64 + (int)(46.0f * sinf(nx * delta));
            oled_line(18, 32, 110, 32); // Eje base tenue
            oled_line(x - 2, 30, x + 2, 34);
            oled_line(x - 2, 34, x + 2, 30);
            delta += 0.08f;
        } 
        else if (actB > 0) {
            // Solo Persona B: Oscilación vertical pura (eje Y sin eje X)
            int ny = obtener_armonico(maskB);
            int y = 32 + (int)(27.0f * sinf(ny * delta));
            oled_line(64, 5, 64, 59); // Eje base tenue
            oled_line(62, y - 2, 66, y + 2);
            oled_line(62, y + 2, 66, y - 2);
            delta += 0.08f;
        }

        oled_update();
        tick++;
        sleep_ms(16);
    }

    return 0;
}
```
> seguiremos probando códigos para perfeccionar la muestra. 
## encargos

# el tiempo para mi 
<https://docs.google.com/document/d/1WA7PEXJ8ot-VD-x5DBP0bUAyzAGR-Rw1q2d97i85CNc/edit?usp=sharing>
## lectura
