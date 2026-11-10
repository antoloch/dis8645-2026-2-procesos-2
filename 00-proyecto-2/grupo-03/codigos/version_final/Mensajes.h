// modo 01 de pantalla (modoActual = 0)
// muestra mensajes motivacionales o afirmaciones.
// la perilla 01 permite recorrerlos. No dependen de la emocion.
 
#ifndef MENSAJES_H
#define MENSAJES_H
 
#include "pico/stdlib.h" // funciones básicas de Raspberry Pi Pico
#include <stdio.h>   // necesario para printf
#include <stdint.h> // necesario para tipos de datos enteros de tamaño fijo
#include "Pantalla.h"   // para dibujar en la pantalla
 
class Mensajes {
 
    /// ATRIBUTOS ///
 
public:
 
// sin tildes ni "ñ": la fuente de la pantalla solo tiene letras ASCII.
// cada linea de la pantalla tiene espacio para 21 letras (se parten solas en varias lineas).

const char* mensajes[5] = {
    "Eres capaz de lograr grandes cosas.",
    "Cada dia es una nueva oportunidad.",
    "Confia en ti mismo y en tus habilidades.",
    "El exito es la suma de pequenos esfuerzos.",
    "Nunca es tarde para perseguir tus suenos."
};
// mensajes de ejemplos hasta el momento
// hay que mejorarlos
 
int cantidadMensajes = sizeof(mensajes) / sizeof(mensajes[0]); // se calcula sola: cantidad de mensajes motivacionales
int mensajeActual = 0;    // mensaje que se esta mostrando actualmente
int mensajeDibujado = -1; // ultimo mensaje dibujado (-1 = ninguno). Evita redibujar sin necesidad
 
    /// METODOS ///
 
void entrarMensajes(); // Se llama al entrar al modo: vuelve al primer mensaje y fuerza a dibujarlo.
void seleccionarMensaje(int indice); // Elige el mensaje con el valor de la perilla (se limita al rango valido).
void siguienteMensaje(); // Avanza al próximo mensaje y vuelve al primero cuando llega al final.
void dibujarMensaje(Pantalla &pantalla); // Dibuja el mensaje actual, solo si cambió.
 
};
 
#endif // MENSAJES_H
 