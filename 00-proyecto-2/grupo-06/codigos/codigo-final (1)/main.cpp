// Juego tipo Flappy Bird para Raspberry Pi Pico (Pico SDK, Wokwi)
// Estados: pantalla apagada -> selección de personaje -> jugando.
// Si el jugador choca o cae, vuelve a la selección de personajes.

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "Boton.h"
#include "Perilla.h"
#include "Mapa.h"
#include "Juego.h"
#include "Pantalla.h"

// Conexiones (iguales a las de diagram.json)
//   OLED SSD1306:      SDA = GP4, SCL = GP5
//   Botón seleccionar: GP14   (toque largo = encender/apagar, toque corto = elegir)
//   Botón volar:       GP15   (mantener = sube, soltar = cae)
//   Perilla velocidad: GP26   (ADC0, izquierda)
//   Perilla personaje: GP27   (ADC1, derecha)
static const uint PIN_BOTON_SELECCION = 14;
static const uint PIN_BOTON_VOLAR = 15;
static const int PIN_PERILLA_VELOCIDAD = 26;
static const int PIN_PERILLA_PERSONAJE = 27;

// Lo grande (framebuffer, mapa) va FUERA del main, como variable global:
// la pila de la Pico es chica y no cabe ahí.
// Estos objetos no tocan el hardware al crearse, así que pueden ser globales.
static Pantalla pantalla;
static Boton botonSeleccion(PIN_BOTON_SELECCION);
static Boton botonVolar(PIN_BOTON_VOLAR);
static uint8_t framebuffer[Pantalla::BYTES_BUFFER];

int main()
{
  

    // Las perillas SÍ configuran el ADC cuando se crean, por eso se crean aquí
    // dentro, cuando la Pico ya terminó de arrancar.
    Perilla perillaVelocidad(PIN_PERILLA_VELOCIDAD);
    Perilla perillaPersonaje(PIN_PERILLA_PERSONAJE);

    // "static" dentro del main = vive en memoria global (no en la pila),
    // pero se crea recién aquí, después de las perillas que necesita.
    static Juego juego(botonSeleccion, botonVolar, perillaVelocidad, perillaPersonaje);

    pantalla.iniciar();
    juego.iniciar(); // también inicia los dos botones

    memset(framebuffer, 0, sizeof(framebuffer));
    pantalla.enviar(framebuffer); // arranca en negro
    pantalla.apagar();            // el juego parte apagado
    bool panelEncendido = false;

    while (true)
    {
        juego.actualizar();

        if (juego.pantallaEncendida())
        {
            if (!panelEncendido)
            {
                pantalla.encender();
                panelEncendido = true;
            }
            juego.dibujar(framebuffer);
            pantalla.enviar(framebuffer);
        }
        else if (panelEncendido)
        {
            pantalla.apagar();
            panelEncendido = false;
        }
    }
}