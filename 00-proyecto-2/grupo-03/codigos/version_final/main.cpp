//// MAQUINA DE EMOCIONES - MAIN ////
//
// pantallas de la maquina (valor de perillaNavegacion):
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
#include "Pantalla.h"             // pantalla OLED por I2C

// modos de pantalla //
#include "Mensajes.h"            // modo 01: mensajes motivacionales
#include "Emociones.h"           // modo 02: elegir expresion facial
#include "Rostros.h"             // grafico de cada emocion (bitmap)
#include "Juego.h"               // modo 03: jugar a saltar obstaculos

// pines (cambiar aqui si el circuito de Wokwi usa otros)
const int PIN_BOTON_ENCENDIDO     = 14;  // boton 01: enciende y apaga
const int PIN_BOTON_SALTO         = 15;  // boton 02: salta
const int PIN_PERILLA_NAVEGACION  = 26;  // ADC0: elige la pantalla
const int PIN_PERILLA_EMOCIONES   = 27;  // ADC1: elige emocion o mensaje
const int PIN_LED_SALTO           = 13;  // LED del boton de salto
// la pantalla usa GP4 (SDA) y GP5 (SCL); se configuran en Pantalla.h

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
Pantalla pantalla;             // la pantalla OLED
// el juego es el objeto "juego" declarado en Juego.h


// el juego dibuja en su propio buffer; esta funcion lo manda a la pantalla
void enviarJuegoAPantalla(const uint8_t* datos) {
    pantalla.enviarBuffer(datos);
}


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

    // boton 01: enciende y apaga la maquina
    Boton botonEncendido(PIN_BOTON_ENCENDIDO, false);

    // boton 02: salto, solo funciona en el modo 03
    Boton botonSalto(PIN_BOTON_SALTO, false);

    // perilla de navegacion: elige entre las 3 pantallas
    Perilla perillaNavegacion(PIN_PERILLA_NAVEGACION, 1);
    perillaNavegacion.definirOpciones(3);

    // perilla 01: elige mensaje (modo 01) o emocion (modo 02)
    Perilla perillaEmociones(PIN_PERILLA_EMOCIONES, 0);
    perillaEmociones.definirOpciones(4);

    // LED: se enciende en el modo 03 (boton de salto disponible)
    Led ledSalto(PIN_LED_SALTO);

    // pausa para que alcance a conectarse el monitor serial
    sleep_ms(2000);

    printf("================================\n");
    printf("MAQUINA DE EMOCIONES\n");
    printf("================================\n");
    // iniciar la pantalla. Si no responde, la maquina sigue funcionando por consola.
    if (pantalla.iniciar()) {
        printf("PANTALLA OK\n");
    }
    else {
        printf("PANTALLA NO RESPONDE (revisar cables SDA/SCL y la direccion I2C)\n");
    }
    juego.enviarPantalla = enviarJuegoAPantalla;   // el juego usa esta funcion para dibujar

    // la maquina parte apagada: pantalla en negro y apagada
    pantalla.limpiar();
    pantalla.mostrar();
    pantalla.apagar();

    printf("MAQUINA APAGADA\n");

    while (true) {

        // 1. leer los botones en cada vuelta
        botonEncendido.leer();
        botonSalto.leer();

        // 2. boton 01: un clic enciende o apaga la maquina
        if (botonEncendido.clicSimple) {

            maquinaEncendida = !maquinaEncendida;

            // si estaba jugando, cerrar la partida antes de cambiar de estado
            if (modoActual == MODO_JUEGO) salirJuego();

            // -1 obliga a detectar de nuevo la pantalla y a redibujarla
            modoActual = -1;

            if (maquinaEncendida) {
                printf("MAQUINA ENCENDIDA\n");
                pantalla.encender();
                // las perillas pudieron moverse mientras estaba apagada
                refrescarPerilla(perillaNavegacion);
                refrescarPerilla(perillaEmociones);
            }
            else {
                printf("MAQUINA APAGADA\n");
                ledSalto.apagar();
                pantalla.limpiar();
                pantalla.mostrar();
                pantalla.apagar();
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

            // si salimos del juego, cerrarlo
            if (modoActual == MODO_JUEGO) salirJuego();

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
                // Emociones da la dificultad de 1 a 4 y el juego la pide de 0 a 3
                int dificultad = emociones.obtenerDificultad(emocionGuardada) - 1;
                printf("MODO 03 - JUEGO (emocion %s, dificultad %d)\n",
                       emociones.obtenerNombre(emocionGuardada), dificultad);
                entrarJuego(dificultad);
            }
        }

        // 7. hacer funcionar solo la pantalla activa (corre en cada vuelta)
        switch (modoActual) {

            case MODO_MENSAJES:
                // la perilla 01 recorre los mensajes (usa el valor con histeresis)
                mensajes.seleccionarMensaje(perillaEmociones.posicion(mensajes.cantidadMensajes));
                mensajes.dibujarMensaje(pantalla);
                break;

            case MODO_EMOCIONES:
                // la perilla 01 (4 opciones) elige el rostro
                emociones.actualizarEmocion(perillaEmociones.opcionSeleccionada);
                emocionGuardada = emociones.emocionSeleccionada();   // anotamos la emocion elegida
                emociones.dibujarEmocion(pantalla);
                break;

            case MODO_JUEGO:
                // el juego solo recibe "se presiono el boton 02": salta, o reinicia si ya termino
                actualizarJuego(botonSalto.seAcabaDePresionar);
                dibujarJuego();
                break;
        }

        // 8. el LED se enciende solo en el modo 03
        ledSalto.actualizar(modoActual);
        static bool avisoLed = false;

            if (modoActual == 2 && !avisoLed) {
                printf("LED GP13 ACTIVADO\n");
                avisoLed = true;
            }
            if (modoActual != 2) {
                avisoLed = false;
            }

                    sleep_ms(10);          // pequena pausa para no saturar el procesador
                }

    return 0;
}