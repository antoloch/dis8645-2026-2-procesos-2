#include "pico/stdlib.h" // funciones básicas de Raspberry Pi Pico
#include <stdio.h>   // necesario para printf
#include <stdint.h> // necesario para tipos de datos enteros de tamaño fijo
#include "Mensajes.h" // para dibujar mensajes motivacionales

// METODOS
void Mensajes::entrarMensajes() // Se llama al entrar al modo: vuelve al primer mensaje y fuerza a dibujarlo.
{
    mensajeActual = 0;
    mensajeDibujado = -1;
}

void Mensajes::siguienteMensaje() // Avanza al próximo mensaje y vuelve al primero cuando llega al final.
{
    mensajeActual++;
    if (mensajeActual >= cantidadMensajes) {
        mensajeActual = 0;
    }
}

void Mensajes::dibujarMensaje() // Dibuja el mensaje actual, solo si cambió.
{
    if (mensajeActual == mensajeDibujado) return;
    mensajeDibujado = mensajeActual;

    // Aquí iría el código para dibujar el texto en la pantalla.
    // Por ahora, solo imprimimos el texto en la consola.
    printf("%s\n", mensajes[mensajeActual]);
}
