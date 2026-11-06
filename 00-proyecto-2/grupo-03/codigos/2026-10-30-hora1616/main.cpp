//// MAQUINA DE EMOCIONES - MAIN UNIFICADO ////
// une main.cpp (estructura por clases) y prueba.cpp (hardware ya probado)
//
//   pantallas de la maquina (valor de perillaNavegacion):
//   modo 01 (0) = mensajes: perilla 01 recorre los mensajes
//   modo 02 (1) = expresiones: perilla 01 elige la emocion (rostro)
//   modo 03 (2) = minijuego: boton 02 salta, el LED avisa que esta disponible

#include <stdio.h>               // printf por el monitor serial
#include <stdint.h>              // tipos de datos enteros de tamano fijo
#include "pico/stdlib.h"         // funciones basicas de Raspberry Pi Pico
#include "hardware/adc.h"        // lectura analogica (potenciometros)
#include "hardware/gpio.h"       // pines digitales

// componentes //
#include "Boton.h"               // botones
#include "Perilla.h"             // potenciometros
#include "Led.h"                 // LED del boton de salto

// modos de pantalla //
#include "Mensajes.h"            // modo 01: mensajes motivacionales
#include "Emociones.h"           // modo 02: elegir expresion facial
#include "Rostros.h"            // bitmaps de cada emocion
// #include "Juego.h"            // modo 03: jugar a saltar obstaculos (TODO)

// numeros de modo, para no andar escribiendo 0, 1 y 2 a ciegas
const int MODO_MENSAJES  = 0;
const int MODO_EMOCIONES = 1;
const int MODO_JUEGO     = 2;

// variables globales (la "libreta" de la maquina)
bool maquinaEncendida = false; // false = apagada, true = encendida
int modoActual = -1;           // -1 = aun no se detecta el modo
int emocionGuardada = 0;       // 0 = felicidad, 1 = tristeza, 2 = enojo, 3 = calma

// objetos de los modos: se crean a partir de las clases
Mensajes mensajes;             // guarda el mensaje que se esta mostrando
Emociones emociones;           // guarda la emocion que se esta viendo


// HISTERESIS: perilla.leer() solo actualiza "valor" cuando la lectura cambia mas que el
// umbral (30). Asi el ruido del potenciometro no hace saltar la opcion en los limites.
// La opcion solo se recalcula cuando la perilla realmente se movio.
void actualizarPerilla(Perilla &perilla) {
    perilla.leer();
    if (perilla.movio) {
        perilla.calcularOpcion();
    }
}

// lectura fresca sin histeresis: se usa al encender, para partir desde la posicion real
void refrescarPerilla(Perilla &perilla) {
    perilla.leerValor();
    perilla.calcularOpcion();
}


int main() {

    stdio_init_all();

    // el ADC se inicia ANTES de crear las perillas,
    // porque el constructor de Perilla ya lee el potenciometro
    adc_init();

    //////////////////////////////////////////////////////
    // CREAR COMPONENTES (dentro de main, despues de adc_init)
    //////////////////////////////////////////////////////

    // boton 01 en GP14: enciende y apaga la maquina
    Boton botonEncendido(14, false);

    // boton 02 en GP15: salto, solo funciona en el modo 03
    Boton botonSalto(15, false);

    // perilla de navegacion en GP26: elige entre las 3 pantallas
    Perilla perillaNavegacion(26, 1);
    perillaNavegacion.definirOpciones(3);

    // perilla 01 en GP27: elige mensaje (modo 01) o emocion (modo 02)
    Perilla perillaEmociones(27, 0);
    perillaEmociones.definirOpciones(4);

    // LED en GP13: se enciende en el modo 03 (boton de salto disponible)
    Led ledSalto(13);

    // pausa para que alcance a conectarse el monitor serial
    sleep_ms(2000);

    printf("================================\n");
    printf("MAQUINA DE EMOCIONES\n");
    printf("================================\n");
    printf("MAQUINA APAGADA\n");

    while (true) {

        // 1. leer los botones en cada vuelta
        botonEncendido.leer();
        botonSalto.leer();

        // 2. boton 01: un clic enciende o apaga la maquina
        if (botonEncendido.clicSimple) {

            maquinaEncendida = !maquinaEncendida;

            // -1 obliga a detectar de nuevo la pantalla y a redibujarla
            modoActual = -1;

            if (maquinaEncendida) {
                printf("MAQUINA ENCENDIDA\n");
                // las perillas pudieron moverse mientras estaba apagada
                refrescarPerilla(perillaNavegacion);
                refrescarPerilla(perillaEmociones);
            }
            else {
                printf("MAQUINA APAGADA\n");
                ledSalto.apagar();
            }
        }

        // 3. si la maquina esta apagada no se hace nada mas
        if (!maquinaEncendida) {
            sleep_ms(10);
            continue;          // vuelve al inicio del ciclo
        }

        // 4. leer ambas perillas con histeresis (siempre, para que su valor este al dia)
        actualizarPerilla(perillaNavegacion);
        actualizarPerilla(perillaEmociones);

        // 5. perilla de navegacion: en que pantalla deberia estar la maquina (0, 1 o 2)
        int nuevoModo = perillaNavegacion.opcionSeleccionada;

        // 6. cambio de pantalla: esta parte corre solo una vez, en el momento del cambio
        if (nuevoModo != modoActual) {
            modoActual = nuevoModo;

            if (modoActual == MODO_MENSAJES) {
                printf("MODO 01 - MENSAJES\n");
                mensajes.entrarMensajes();
            }
            else if (modoActual == MODO_EMOCIONES) {
                printf("MODO 02 - EXPRESIONES\n");
                emociones.entrarEmociones();
            }
            else if (modoActual == MODO_JUEGO) {
                printf("MODO 03 - JUEGO (dificultad %d, emocion %s)\n",
                       emociones.obtenerDificultad(emocionGuardada),
                       emociones.obtenerNombre(emocionGuardada));
                // TODO: entrarJuego(emociones.obtenerDificultad(emocionGuardada));
            }
        }

        // 7. hacer funcionar solo la pantalla activa (corre en cada vuelta)
        switch (modoActual) {

            case MODO_MENSAJES:
                // la perilla 01 recorre los mensajes (usa el valor con histeresis)
                mensajes.mensajeActual = perillaEmociones.posicion(mensajes.cantidadMensajes);
                mensajes.dibujarMensaje();
                break;

            case MODO_EMOCIONES:
                // la perilla 01 (4 opciones) elige el rostro
                emociones.actualizarEmocion(perillaEmociones.opcionSeleccionada);
                emocionGuardada = emociones.emocionSeleccionada();   // anotamos la emocion elegida
                emociones.dibujarEmocion();
                break;

            case MODO_JUEGO:
                // TODO: actualizar y dibujar el juego (obstaculos segun la dificultad)
                if (botonSalto.seAcabaDePresionar) {
                    printf("SALTO\n");
                    // TODO: juego.saltar();
                }
                break;
        }

        // 8. el LED se enciende solo en el modo 03
        ledSalto.actualizar(modoActual);

        sleep_ms(10);          // pequena pausa para no saturar el procesador
    }

    return 0;
}