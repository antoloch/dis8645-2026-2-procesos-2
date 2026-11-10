#include "pico/stdlib.h" // funciones básicas de Raspberry Pi Pico
#include <stdio.h>   // necesario para printf
#include <stdint.h> // necesario para tipos de datos enteros de tamaño fijo
#include "Emociones.h"
#include "Rostros.h"   // bitmaps de cada emocion
#include "Pantalla.h"

// METODOS

void Emociones::entrarEmociones() // se llama al entrar al modo: fuerza a dibujar de nuevo el rostro.
{
    ultimoDibujado = -1;
}

void Emociones::actualizarEmocion(int opcion) // guarda la opción que viene de la perilla como emoción actual.
{
    if (opcion < 0) opcion = 0;
    if (opcion > 3) opcion = 3;
    indiceActual = opcion;
}

int Emociones::emocionSeleccionada() // devuelve el índice de la emoción elegida.
{
    return indiceActual;
}

const char* Emociones::obtenerNombre(int emocion) // devuelve el nombre de esa emoción ("Desconocida" si el indice no existe).
{
    if (emocion < 0 || emocion > 3) {
        return "Desconocida";
    }
    return emociones[emocion];
}

int Emociones::obtenerDificultad(int emocion) // devuelve el nivel de dificultad (1 a 4) de esa emoción (1 si el indice no existe).
{
    if (emocion < 0 || emocion > 3) {
        return 1;   // dificultad mas baja por seguridad
    }
    return dificultades[emocion];
}

void Emociones::dibujarEmocion(Pantalla &pantalla) // dibuja el rostro de la emoción actual, solo si cambió.
{
    if (indiceActual == ultimoDibujado) return;
    ultimoDibujado = indiceActual;

    // por consola (sirve para probar sin pantalla)
    printf("Emocion: %s\n", emociones[indiceActual]);

    // en la pantalla: solo el rostro (128 x 64)
    pantalla.limpiar();
    pantalla.bitmap(rostros[indiceActual], ANCHO_ROSTRO, ALTO_ROSTRO);
    pantalla.mostrar();
}