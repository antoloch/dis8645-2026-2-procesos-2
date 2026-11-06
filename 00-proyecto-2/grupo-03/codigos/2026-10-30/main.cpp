//// MAQUETA CODIGO ////
#include <stdio.h>               // permite mostrar texto por el monitor serial
#include <stdint.h>              // necesario para tipos de datos enteros de tamaño fijo
#include "pico/stdlib.h"         // funciones básicas de Raspberry Pi Pico
#include "hardware/adc.h"        // permite leer componentes analógicos
 
// componentes //

// #include "Perilla.h"          // clase para controlar los potenciómetros
// #include "Boton.h"            // clase para controlar los botones
 
// modos de pantalla //

#include "Mensajes.h"            // modo 0: mostrar mensajes motivacionales
// #include "Rostros.h"          // imagenes de expresiones faciales (bitmaps)
#include "Emociones.h"           // modo 1: elegir tu expresion facial
// #include "Juego.h"            // modo 2: jugar a saltar obstaculos


 
// clase Perilla tiene
// Perilla 0 (elige la emoción), Perilla 1 (elige el modo)
 
// clase Boton tiene
// Boton 0 (enciende la máquina), Boton 1 (salta en el juego)
 
// variables globales (la "libreta" de la máquina)
bool maquinaEncendida = false; // si se presiona el boton 0, maquinaEncendida = true
                               
int modoActual = 0;            // 0 = mensajes, 1 = emociones, 2 = juego
int emocionGuardada = 0;       // 0 = felicidad, 1 = tristeza, 2 = enojo, 3 = calma

// objetos de los modos: se crean a partir de las clases
Mensajes mensajes;             // objeto del modo 0: guarda el mensaje que se está mostrando
Emociones emociones;           // objeto del modo 1: guarda la emoción que se está viendo
 
 
int main() {
    stdio_init_all();
 
    // cuando existan las clases, iniciarlas aquí
    // adc_init();
    // perilla0.iniciar();  perilla1.iniciar();
    // boton0.iniciar();    boton1.iniciar();
 
    while (true) {
 
        // 1. si la máquina está apagada, esperar a que se presione el boton 0
        if (!maquinaEncendida) {
            // TODO: if (boton0.fuePresionado()) { maquinaEncendida = true; }
            sleep_ms(10);
            continue;          // vuelve al inicio del ciclo
        }
 
        // 2. leer la perilla 1 para saber en qué modo debería estar la máquina (0, 1 o 2)
        // TODO: int modo = perilla1.opcion(3);
        int modo = modoActual; // provisional, hasta tener la clase Perilla
 
        // 3. ¿cambió el modo? esta parte corre solo una vez, en el momento del cambio
        if (modo != modoActual) {
            modoActual = modo;
            // objeto.metodo(): el punto significa "al objeto, pídele que ejecute este método"
            if (modoActual == 0) mensajes.entrarMensajes();
            if (modoActual == 1) emociones.entrarEmociones();
            // TODO: if (modoActual == 2) entrarJuego(emociones.obtenerDificultad(emocionGuardada));
        }
 
        // 4. hacer funcionar solo el modo que está activo (corre en cada vuelta)
        switch (modoActual) {
 
            case 0:  // modo 0: mensajes
                // TODO: mensajes.siguienteMensaje() con el control que decida el grupo
                mensajes.dibujarMensaje();
                break;
 
            case 1:  // modo 1: emociones
                // TODO: emociones.actualizarEmocion(perilla0.opcion(4));
                emocionGuardada = emociones.emocionSeleccionada();   // anotamos la emoción elegida
                emociones.dibujarEmocion();
                break;
 
            case 2:  // modo 2: juego
                // TODO: actualizar y dibujar el juego (con boton1 para saltar)
                break;
        }
 
        sleep_ms(10);          // pequeña pausa para no saturar el procesador
    }
 
    return 0;
}