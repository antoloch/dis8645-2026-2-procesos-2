// ============================================================
// Matriz.h
// declaraciones de la clase Matriz, que maneja la matriz LED
// 8x8 con WS2812B donde bajan las luces del juego.
// ============================================================

// Esqueleto base: evita que este archivo se cargue dos veces.
#ifndef MATRIZ_H
#define MATRIZ_H

// esto lo agregamos para usar funciones generales de la Pico
#include "pico/stdlib.h"

// Esto lo agregamos para usar el PIO, el procesador pequeño de la
// raspberry que manda la señal a los LEDs con tiempos muy precisos.
#include "hardware/pio.h"

// ============================================================

// PALETA DE COLORES DEL PROYECTO
// Cada color se escribe en formato 0xRRGGBB (rojo, verde, azul),
// igual que los códigos de color que usamos en los diagramas.
const uint32_t NEGRO          = 0x000000; // LED apagado
const uint32_t AZUL_PETROLEO  = 0x005968;
const uint32_t TURQUESA       = 0x4BC7CF;
const uint32_t MENTA          = 0xBCEDE5;
const uint32_t BLANCO_HUMO    = 0xF5F5F5;
const uint32_t MORADO         = 0xA247D3;
// El rojo no está en la paleta del proyecto, pero lo necesitamos para
// la pantalla de derrota.
const uint32_t ROJO = 0xFF0000;

// ============================================================

// definir clase Matriz
class Matriz {

  // todo publico
  // nada de andar privatizando
  public:

  // atributos

  // Guarda el pin (la patita) de la raspberry por donde sale la
  // señal hacia la matriz (el pin DIN de la matriz).
  int patita;

  // Guarda el color de cada uno de los 64 LEDs, antes de mostrarlos.
  // Se ordena por fila y columna: el LED de la fila 2, columna 3
  // queda en la posición 2 * 8 + 3. Así se puede dibujar todo y
  // enviarlo de una sola vez.
  uint32_t pixeles[64];
    // Guarda una foto del dibujo actual. Sirve para el zoom out: dibujamos
  // la foto cada vez más pequeña, hasta que desaparece.
  uint32_t copia[64];

  // Guarda qué tan fuerte brillan los LEDs, de 0 a 100. Empieza en 20
  // porque los LEDs WS2812 al máximo encandilan y gastan mucha corriente.
  int brillo = 20;

  // Guarda si la matriz está cableada en zigzag (serpentina): la fila 0
  // va de izquierda a derecha, la fila 1 de derecha a izquierda, y así.
  // Algunas matrices vienen así y otras no. Si el dibujo sale al revés
  // en las filas impares, se cambia a true.
  bool serpentina = false;

  // Guarda cuál PIO y cuál de sus máquinas internas estamos usando.
  PIO pio;
  int maquina;


  // constructor
  // Recibe el pin donde está conectada la matriz.
  Matriz(int nuevaPatita);

  // metodos

  // Apaga todos los LEDs en el dibujo (todavía no se ven los cambios
  // hasta que se llame a mostrar()).
  void limpiar();

  // Pinta un solo LED, dada su fila (0 a 7) y su columna (0 a 7).
  void pintarPixel(int fila, int columna, uint32_t color);

  // Pinta el LED de un camino (1 a 4) en una fila dada. Como cada
  // camino mide 2 columnas, pinta las dos al mismo tiempo.
  void pintarCamino(int camino, int fila, uint32_t color);

  // Manda el dibujo a la matriz para que los LEDs realmente cambien.
  // Se llama solo cuando algo cambió, no en cada vuelta.
  void mostrar();
    // Pinta toda la pantalla de un solo color.
  void rellenar(uint32_t color);

  // Pinta un texto que se desplaza de derecha a izquierda.
  // "desplazamiento" es cuántas columnas ha avanzado: con 0 el texto
  // todavía no entra, y a medida que sube, va apareciendo por la derecha.
  void dibujarTexto(const char* texto, int desplazamiento, uint32_t color);

  // Devuelve cuántas columnas mide el texto completo. Sirve para saber
  // cuándo terminó de pasar (cuando desplazamiento > ancho + 8).
  int anchoTexto(const char* texto);

  // Pinta un número del 1 al 3, centrado en la matriz.
  void dibujarNumero(int numero, uint32_t color);

  // Guarda una foto del dibujo actual.
  void guardarCopia();

  // Limpia la pantalla y dibuja la foto guardada, achicada hacia el
  // centro. paso 1 = 6x6, paso 2 = 4x4, paso 3 = 2x2.
  void dibujarZoom(int paso);

  // Llena la pantalla con franjas diagonales de los colores de la
  // paleta. "desfase" las va corriendo para que parezcan moverse.
  void fondoColores(int desfase);

};

#endif