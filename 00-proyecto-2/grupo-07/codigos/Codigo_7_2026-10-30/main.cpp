#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "Perilla.h"
#include "BotonGrupo1.h"
#include "Pantallita.h"
// Dos estados: MOVER (el punto se mueve sin dejar marca) y DIBUJAR (el punto deja una línea).
// Botón de trazo (GP14): cada pulsación alterna entre MOVER y DIBUJAR (la variable "dibujando" vive aquí en main).
// Botón de borrar (GP15): limpia el dibujo y vuelve a MOVER. Si se aprietan los dos a la vez, gana BORRAR.
// Código 7: todo lo de la OLED (I2C, SSD1306, buffers) vive en la clase Pantallita; aquí solo la usamos.
int main() {
    // 1. PREPARAR ENTRADAS Y RANGOS
    stdio_init_all(); adc_init();                      // Prendemos el monitor serial y el lector analógico.
    Perilla horizontal(26, 127), vertical(27, 55);     // X va de 0 a 127; Y recorre 0 a 55, dejando 8 píxeles para el estado.
    BotonGrupo1 trazo(14), borrar(15);                 // Dos botones de la clase del grupo, en GP14 y GP15.
    // 2. PREPARAR PANTALLA Y TIEMPOS
    Pantallita pantalla;                               // La OLED completa: ahora el main no ve buffers ni I2C.
    if (!pantalla.iniciar()) {                         // Revisamos que se haya podido reservar la memoria.
        printf("No se pudo reservar la pantalla.\n");
        while (true) tight_loop_contents();
    }
    uint64_t lecturaAnterior = 0, informeAnterior = 0; // Para medir cuánto tiempo pasó entre lecturas.
    bool primera = true;                               // La primera vez dibujamos sí o sí, apenas leamos las perillas.
    bool redibujar = false;                            // Cuando vale true, armamos y mandamos una imagen nueva.
    bool dibujando = false;                            // false = MOVER, true = DIBUJAR.
    bool dibujandoAntes = false;                       // Cómo estaba "dibujando" en la imagen anterior (para saber cuándo empieza un trazo).
    int xAnterior = 0, yAnterior = 8;                  // Dónde estaba el punto en la imagen anterior.
    // 3. LEER, ACTUALIZAR Y MOSTRAR
    while (true) {
        const uint64_t ahora = time_us_64();           // Tiempo desde que se prendió la placa, en microsegundos.
        // 3a. BOTONES: los revisamos en CADA vuelta, sin esperar, para no perder ninguna pulsación.
        trazo.leer();                                  // Lee el botón de trazo (con filtro de rebote de 50 ms).
        borrar.leer();                                 // Lee el botón de borrar.
        if (trazo.recienPresionado || borrar.recienPresionado) redibujar = true; // Un botón cambió algo: hay que actualizar la imagen.
        if (trazo.recienPresionado) dibujando = !dibujando; // Cada pulsación de trazo alterna MOVER <-> DIBUJAR.
        if (borrar.recienPresionado) {                 // Si se apretó BORRAR...
            pantalla.limpiar();                        // ...vaciamos el dibujo guardado...
            dibujando = false;                         // ...y volvemos a MOVER, aunque trazo también se haya apretado ahora.
        }
        // 3b. PERILLAS: las leemos cada 20 milisegundos.
        if (ahora - lecturaAnterior >= 20000) {
            lecturaAnterior = ahora;
            horizontal.leer(); vertical.leer();        // Cada perilla elige su canal antes de leer, para que X e Y sean independientes.
            if (primera || horizontal.cambio || vertical.cambio) { // Si es la primera vez o alguna perilla cambió...
                redibujar = true;                      // ...hay que actualizar la imagen.
                primera = false;
            }
        }
        // 3c. DIBUJAR Y MOSTRAR: solo cuando algo cambió.
        if (redibujar) {
            redibujar = false;
            const int x = horizontal.posicion;         // Columna del punto, de 0 a 127.
            const int y = 8 + vertical.posicion;           // Fila del punto, de 8 a 63.
            if (dibujando && !dibujandoAntes) {        // Si el trazo recién empieza...
                xAnterior = x; yAnterior = y;          // ...parte desde aquí; no se conecta con el trazo anterior.
            }
            if (dibujando) pantalla.dibujarLinea(xAnterior, yAnterior, x, y); // En DIBUJAR, unimos el punto anterior con el actual.
            dibujandoAntes = dibujando;                // Recordamos el estado para la próxima imagen.
            xAnterior = x; yAnterior = y;              // Recordamos dónde quedó el punto.
            pantalla.mostrar(x, y, dibujando);                    // Dibujo guardado + cursor (el cursor no deja rastro).
        }
        // 4. REVISAR RESULTADOS
        // Mostramos las coordenadas, el estado y las lecturas para revisar cada parte.
        if (ahora - informeAnterior >= 200000) {
            informeAnterior = ahora;
            printf("X=%d Y=%d estado=%s ADC_X=%u ADC_Y=%u\n", horizontal.posicion, 8 + vertical.posicion,
                   dibujando ? "DIBUJAR" : "MOVER", unsigned(horizontal.lectura), unsigned(vertical.lectura));
        }
        tight_loop_contents();
    }
}
