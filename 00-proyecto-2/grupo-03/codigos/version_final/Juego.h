#ifndef JUEGO_H
#define JUEGO_H

// Juego.h
// modo 03 de pantalla (modoActual = 2)


// COMO FUNCIONA (resumen):
//  un personaje (cuadrado) esta fijo a la izquierda de la pantalla, sobre el suelo.
//  los obstaculos (barras) aparecen por la derecha y se mueven hacia la izquierda.
//  visto desde afuera parece que el personaje "avanza solo".
//  el boton 02 hace saltar al personaje. si choca con un obstaculo, termina la partida.
//  con la partida terminada, el mismo boton 02 empieza una nueva.
//  (para volver a navegar se gira la perilla de navegacion)
//  la emocion elegida define la dificultad: velocidad y frecuencia de los obstaculos.


// esta clase NO toca pines: el boton y el LED los maneja el main.
// el juego solo recibe "el boton se presiono" y dibuja en un buffer de memoria.

#include <stdint.h>
#include "pico/stdlib.h"

// pantalla: OLED SH1106 de 128 x 64 pixeles, 1 bit por pixel
#define PANTALLA_ANCHO    128
#define PANTALLA_ALTO     64
#define MAX_OBSTACULOS    3     // cuantos obstaculos pueden existir a la vez

class Juego {
  public:

  /// pantalla ///

  // "dibujo" en memoria: 1024 bytes = 128 columnas x 8 paginas de 8 pixeles.
  // el pixel (x, y) es el bit (y % 8) del byte [x + (y / 8) * 128].
  // ese es el mismo orden que usan las pantallas SSD1306 / SH1106 (por paginas).
  uint8_t buffer[PANTALLA_ANCHO * PANTALLA_ALTO / 3];

  // funcion que manda el buffer a la pantalla real (la escribe quien haga el driver I2C).
  // mientras sea nullptr el juego corre igual, pero no se ve nada.
  void (*enviarPantalla)(const uint8_t* datos) = nullptr;

  /// atributos del juego ///

  float personajeY = 0;               // altura sobre el suelo en pixeles (0 = en el suelo)
  float velocidadY = 0;               // velocidad vertical del personaje (px/s, positiva = sube)
  float obstaculoX[MAX_OBSTACULOS];   // posicion X de cada obstaculo (borde izquierdo)
  int obstaculoAlto[MAX_OBSTACULOS];  // altura de cada obstaculo en pixeles
  bool obstaculoVivo[MAX_OBSTACULOS]; // true = el obstaculo esta en pantalla
  int puntuacion = 0;                 // obstaculos superados
  bool activo = false;                // true = partida en curso, false = partida terminada
  int dificultad = 2;                 // 0 calma, 1 tristeza, 2 felicidad, 3 enojo
  int velocidadObstaculos = 80;       // px/s a los que avanzan los obstaculos
  int frecuenciaObstaculos = 1600;    // ms (aprox.) entre un obstaculo y el siguiente

  /// internos ///
  uint32_t ultimoTiempo = 0;          // hora de la ultima actualizacion (ms)
  int msParaNuevoObstaculo = 0;       // cuenta regresiva para crear el proximo obstaculo

  /// constructor ///
  Juego();

  /// metodos ///
  void iniciar();                             // prepara y empieza una partida
  void reiniciar();                           // vuelve todo a cero y empieza de nuevo
  void configurarDificultad(int nuevaDificultad); // 0 a 3: fija velocidad y frecuencia
  void saltar();                              // hace saltar al personaje (solo si esta en el suelo)
  void actualizar();                          // avanza el juego segun el tiempo transcurrido
  bool detectarColision();                    // true si el personaje toca un obstaculo

  /// dibujo ///
  void dibujar();                             // arma el cuadro completo y lo envia a la pantalla
  void limpiar();                             // apaga todos los pixeles del buffer
  void pixel(int x, int y, bool encendido = true);                  // enciende o apaga un pixel
  void rect(int x, int y, int w, int h, bool relleno = true);       // rectangulo (relleno o solo borde)
  void numero(int x, int y, int n, int escala);                     // escribe un numero con digitos 3x5
};

extern Juego juego;   // la unica partida; se crea en Juego.cpp

/// funciones para el main ///
void entrarJuego(int dificultad);               // al entrar al modo 03: dificultad 0 a 3
void actualizarJuego(bool botonSaltoPresionado);// en cada vuelta: true si el boton 02 se acaba de presionar
void dibujarJuego();                            // en cada vuelta: dibuja el cuadro
void salirJuego();                              // al salir del modo 03 (o apagar la maquina)

#endif