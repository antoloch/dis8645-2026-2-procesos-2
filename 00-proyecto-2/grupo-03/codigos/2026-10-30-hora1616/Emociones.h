//// MAQUETA CODIGO ////

// modo 02 de pantalla (modoActual = 1)
// muestra ilustraciones de expresiones faciales
// nota: por ahora este archivo se incluye solo desde main.cpp.
// Al convertirlo en clase se evita que los nombres choquen con los de otros archivos.
 
#ifndef EMOCIONES_H
#define EMOCIONES_H

#pragma once
#include "pico/stdlib.h" // funciones básicas de Raspberry Pi Pico
#include <stdio.h>   // necesario para printf
#include <stdint.h> // necesario para tipos de datos enteros de tamaño fijo
#include "Rostros.h"   // bitmaps de cada emocion

class Emociones {

    /// ATRIBUTOS ///

public: 
 
// 0 = Felicidad, 1 = Tristeza, 2 = Enojo, 3 = Calma
const char* emociones[4] = {
    "Felicidad",
    "Tristeza",
    "Enojo",
    "Calma"
};

// nivel de dificultad del juego
int dificultades[4] = {
    3, // Felicidad: velocidad media, obstaculos moderados
    2, // Tristeza: velocidad lenta, pocos obstaculos
    4, // Enojo: velocidad muy rapida, obstaculos muy frecuentes
    1  // Calma: velocidad muy lenta, muy pocos obstaculos
};
 
// ilustracion de cada emocion, en el mismo orden que emociones[]
const uint8_t* rostros[4] = {
    rostroFelicidad,
    rostroTristeza,
    rostroEnojo,
    rostroCalma
};
 
int indiceActual = 0;      // la emoción que se está viendo en pantalla.
int ultimoDibujado = -1;   // la última emoción dibujada (-1 = ninguna). evita redibujar sin necesidad.



    /// METODOS ///

void entrarEmociones();
void actualizarEmocion(int opcion);
int emocionSeleccionada();
const char* obtenerNombre(int emocion);
int obtenerDificultad(int emocion);
void dibujarEmocion();
};
 
#endif // EMOCIONES_H