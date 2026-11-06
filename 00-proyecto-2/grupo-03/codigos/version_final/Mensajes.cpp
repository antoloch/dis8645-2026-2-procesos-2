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

// Elige el mensaje con el valor que viene de la perilla (siempre dentro del rango valido).
void Mensajes::seleccionarMensaje(int indice)
{
    if (indice < 0) indice = 0;
    if (indice >= cantidadMensajes) indice = cantidadMensajes - 1;
    mensajeActual = indice;
}

void Mensajes::siguienteMensaje() // Avanza al próximo mensaje y vuelve al primero cuando llega al final.
{
    mensajeActual++;
    if (mensajeActual >= cantidadMensajes) {
        mensajeActual = 0;
    }
}

void Mensajes::dibujarMensaje(Pantalla &pantalla) // Dibuja el mensaje actual, solo si cambió.
{
    if (mensajeActual == mensajeDibujado) return;
    mensajeDibujado = mensajeActual;

    // por consola (sirve para probar sin pantalla)
    printf("%s\n", mensajes[mensajeActual]);

    // en la pantalla:
    //   el mensaje en varias lineas (21 letras por linea)
    //   abajo a la derecha: numero de mensaje (por ejemplo 2/5)
    pantalla.limpiar();
    pantalla.textoEnvuelto(0, 12, mensajes[mensajeActual], 21);

    char contador[16];
    snprintf(contador, sizeof(contador), "%d/%d", mensajeActual + 1, cantidadMensajes);
    pantalla.texto(PANTALLA_ANCHO - pantalla.anchoTexto(contador), 56, contador);

    pantalla.mostrar();
}
