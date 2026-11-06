#include <cstdio>
#include <cstdlib>   // aquí viene abs(), que nos da el valor sin signo de un número
#include <cstring>   // aquí viene memcpy(), que copia un bloque de memoria a otro
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "Perilla.h"
#include "BotonGrupo1.h"
extern "C" { // La biblioteca de pantalla está en C; así la podemos llamar desde nuestro C++.
#include "Ssd1306.h"
}
// Código 5: perillas + botones + dibujo.
// Dos estados: MOVER (el punto se mueve sin dejar marca) y DIBUJAR (el punto deja una línea).
// Botón de trazo (GP14): cada pulsación alterna entre MOVER y DIBUJAR.
// Botón de borrar (GP15): limpia el dibujo y vuelve a MOVER. Si se aprietan los dos a la vez, gana BORRAR.
int main() {
    // 1. PREPARAR ENTRADAS Y RANGOS
    stdio_init_all(); adc_init();                      // Prendemos el monitor serial y el lector analógico.
    Perilla horizontal(26, 127), vertical(27, 55);     // X va de 0 a 127; Y va de 0 a 55 (56 valores, porque arriba hay 8 píxeles de franja).
    BotonGrupo1 trazo(14), borrar(15);                 // Dos botones de la clase del grupo, en GP14 y GP15.
    // 2. PREPARAR PANTALLA Y TIEMPOS
    i2c_init(i2c0, 400000);                            // Prendemos el bus I2C a 400.000 bits por segundo.
    gpio_set_function(4, GPIO_FUNC_I2C); gpio_set_function(5, GPIO_FUNC_I2C); // GP4 es SDA y GP5 es SCL.
    gpio_pull_up(4); gpio_pull_up(5);                  // I2C necesita las resistencias pull-up.
    ssd1306_t pantalla{}; pantalla.external_vcc = false; // "pantalla" es lo que se ve: dibujo + franja + cursor.
    ssd1306_t lienzo{};   lienzo.external_vcc = false;   // "lienzo" es una hoja en memoria donde guardamos SOLO el dibujo. Nunca la mostramos.
    // Aquí revisamos que se haya podido reservar la memoria de las dos hojas.
    // Ojo: esto solo no confirma que la pantalla física haya respondido por I2C.
    if (!ssd1306_init(&pantalla, 128, 64, 0x3c, i2c0) || !ssd1306_init(&lienzo, 128, 64, 0x3c, i2c0)) {
        printf("No se pudo reservar la pantalla.\n");
        while (true) tight_loop_contents();
    }
    ssd1306_clear(&lienzo);                            // El dibujo parte vacío.
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
        trazo.presionarSwitch();                       // Si recién se apretó, cambia su "interruptor" (MOVER <-> DIBUJAR).
        borrar.leer();                                 // Lee el botón de borrar.
        if (trazo.recienPresionado || borrar.recienPresionado) redibujar = true; // Un botón cambió algo: hay que actualizar la imagen.
        if (borrar.recienPresionado) {                 // Si se apretó BORRAR...
            ssd1306_clear(&lienzo);                    // ...vaciamos el dibujo guardado...
            trazo.modoBorrar();                        // ...y apagamos el interruptor del trazo: vuelve a MOVER, aunque trazo también se haya apretado ahora.
        }
        dibujando = trazo.estadoPresionarSwitch;       // El estado oficial sale del interruptor del botón de trazo.
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
            const int y = 8 + vertical.posicion;       // Fila del punto, de 8 a 63 (sumamos 8 para no entrar a la franja de estado).
            if (dibujando && !dibujandoAntes) {        // Si el trazo recién empieza...
                xAnterior = x; yAnterior = y;          // ...parte desde aquí; no se conecta con el trazo anterior.
            }
            if (dibujando) {                           // Si estamos en DIBUJAR, unimos el punto anterior con el actual.
                const int dx = x - xAnterior;          // Cuánto nos movimos hacia los lados.
                const int dy = y - yAnterior;          // Cuánto nos movimos hacia arriba o abajo.
                int pasos = abs(dx);                   // Usamos el movimiento más largo como cantidad de pasos...
                if (abs(dy) > pasos) pasos = abs(dy);  // ...para no dejar huecos en las líneas inclinadas.
                for (int i = 0; i <= pasos; i++) {     // Recorremos la línea paso a paso, desde el punto anterior hasta el actual.
                    int px = xAnterior, py = yAnterior; // Si pasos vale 0, el punto no se movió y solo pintamos ese píxel.
                    if (pasos > 0) {
                        px = xAnterior + dx * i / pasos; // Columna de este paso.
                        py = yAnterior + dy * i / pasos; // Fila de este paso.
                    }
                    ssd1306_draw_pixel(&lienzo, px, py); // Pintamos el píxel en el lienzo, donde queda guardado.
                }
            }
            dibujandoAntes = dibujando;                // Recordamos el estado para la próxima imagen.
            xAnterior = x; yAnterior = y;              // Recordamos dónde quedó el punto.
            memcpy(pantalla.buffer, lienzo.buffer, pantalla.bufsize); // Copiamos el dibujo guardado a la pantalla, así no se pierde.
            ssd1306_clear_square(&pantalla, 0, 0, 128, 8);            // Dejamos limpia la franja de estado (los trazos nunca entran ahí).
            ssd1306_draw_string(&pantalla, 0, 0, 1, dibujando ? "DIBUJAR" : "MOVER"); // Escribimos el estado en la franja.
            ssd1306_draw_pixel(&pantalla, x, y);                      // El cursor es una cruz chica; solo se pinta en "pantalla", así no deja rastro.
            if (x > 0)   ssd1306_draw_pixel(&pantalla, x - 1, y);     // Brazo izquierdo de la cruz (si no estamos en el borde).
            if (x < 127) ssd1306_draw_pixel(&pantalla, x + 1, y);     // Brazo derecho.
            if (y > 8)   ssd1306_draw_pixel(&pantalla, x, y - 1);     // Brazo de arriba (sin entrar a la franja de estado).
            if (y < 63)  ssd1306_draw_pixel(&pantalla, x, y + 1);     // Brazo de abajo.
            ssd1306_show(&pantalla);                   // Mandar la imagen por I2C toma tiempo, aunque no agreguemos sleep.
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
