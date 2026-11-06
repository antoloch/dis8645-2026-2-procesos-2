// main.cpp (BOTONES + PERILLAS + INTERRUPTOR + MATRIZ + ANIMACIONES)
// Junta todo en los momentos del programa:
// apagado, bienvenida, menú, jugando, aviso de nivel, derrota y victoria.

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "Boton.h"
#include "Perilla.h"
#include "Matriz.h"

// Creamos los botones, las perillas, el interruptor y la matriz
// diciendo en qué pin está cada uno.
Boton boton1(7);        // navegar entre niveles
Boton boton2(8);        // seleccionar nivel / atrapar la luz
Boton interruptor(9);   // encendido (presionado = true es encendida)
Perilla perilla1(26);   // velocidad de las luces
Perilla perilla2(27);   // los 4 caminos
Matriz matriz(10);      // la matriz LED (pin DIN en GP10)

// Los momentos del programa. El programa siempre está en uno solo.
//  - apagado: la máquina está apagada.
//  - bienvenida: pasa el texto de bienvenida.
//  - menu: se muestra el número del nivel.
//  - jugando: las luces bajan y la jugadora las atrapa.
//  - avisoNivel: zoom out y el número del nivel nuevo parpadea.
//  - derrota: la pantalla se pone roja.
//  - victoria: pantalla de colores y texto de felicitaciones.
enum Momento { apagado, bienvenida, menu, jugando, avisoNivel, derrota, victoria };

// ============================================================

// CONFIGURACIÓN DEL JUEGO
// Son valores fáciles de cambiar para ajustar cómo se siente.

// Duración de cada nivel en milisegundos: 2 minutos = 120000 ms.
// Para probar más rápido en Wokwi se puede bajar a 10000 (10 s).
const uint32_t DURACION_NIVEL = 10000;

// Desde esta fila hacia abajo una luz se puede atrapar.
const int ZONA_CAPTURA = 6;

// Si es true, presionar el botón 2 sin tener una luz en tu camino
// dentro de la zona hace perder. Si es false, no pasa nada.
const bool CASTIGAR_FALLOS = true;

// Los textos que pasan por la pantalla.
const char TEXTO_BIENVENIDA[] = "ALTIRO HERO";
const char TEXTO_VICTORIA[] = "FELICITACIONES";

// Cada cuántos milisegundos el texto avanza una columna.
// Más bajo = más rápido.
const uint32_t TICK_TEXTO = 40;

// Cada cuántos milisegundos cambia un paso del aviso de nivel
// (3 pasos de zoom, 1 vacío, y 6 de parpadeo = 10 pasos en total).
const uint32_t PASO_AVISO = 100;

// Cuánto dura la pantalla roja de derrota, en milisegundos.
const uint32_t DURACION_DERROTA = 1000;

// Colores de las luces de cada camino, sacados de la paleta.
const uint32_t coloresCaminos[4] = { AZUL_PETROLEO, TURQUESA, MORADO, MENTA };

// Colores del número de cada nivel (1, 2 y 3). Evitamos el azul
// petróleo porque se ve muy tenue en los LEDs.
const uint32_t coloresNiveles[3] = { TURQUESA, MORADO, MENTA };

// ============================================================

// Una Luz es una luz que baja por la matriz. Guarda si está
// activa, en qué fila va (0 arriba, 7 abajo) y por cuál camino baja.
struct Luz {
  bool activa = false;
  int fila = 0;
  int camino = 1;
};

// La lista de luces que pueden estar bajando al mismo tiempo.
const int MAX_LUCES = 8;
Luz luces[MAX_LUCES];

// ============================================================

// Quita todas las luces de la lista (para empezar limpio).
void quitarLuces() {
  for (int i = 0; i < MAX_LUCES; i++) {
    luces[i].activa = false;
  }
}

// Dibuja el menú: el número del nivel, centrado.
void dibujarMenu(int nivel) {
  matriz.limpiar();
  matriz.dibujarNumero(nivel, coloresNiveles[nivel - 1]);
  matriz.mostrar();
}

// Dibuja el juego: las luces que bajan y el LED de la jugadora.
void dibujarJuego(int caminoJugadora) {
  matriz.limpiar();

  // Pinta cada luz activa con el color de su camino.
  for (int i = 0; i < MAX_LUCES; i++) {
    if (luces[i].activa) {
      matriz.pintarCamino(luces[i].camino, luces[i].fila,
                          coloresCaminos[luces[i].camino - 1]);
    }
  }

  // Pinta el LED de la jugadora en la fila de abajo. Va al final
  // para que quede encima si una luz llega al mismo lugar.
  matriz.pintarCamino(caminoJugadora, 7, BLANCO_HUMO);

  matriz.mostrar();
}

// Dibuja un paso del aviso de nivel superado:
//  - pasos 0, 1 y 2: el último cuadro del juego, cada vez más chico.
//  - paso 3: pantalla vacía.
//  - pasos 4 al 9: el número del nivel nuevo, prendido en los pasos
//    pares y apagado en los impares (parpadea 3 veces).
void dibujarAviso(int paso, int nivelNuevo) {
  matriz.limpiar();

  if (paso <= 2) {
    matriz.dibujarZoom(paso + 1);
  } else if (paso >= 4 && paso % 2 == 0) {
    matriz.dibujarNumero(nivelNuevo, coloresNiveles[nivelNuevo - 1]);
  }

  matriz.mostrar();
}

// Dibuja el texto de bienvenida en la posición de scroll dada.
void dibujarBienvenida(int desplazamiento) {
  matriz.limpiar();
  matriz.dibujarTexto(TEXTO_BIENVENIDA, desplazamiento, TURQUESA);
  matriz.mostrar();
}

// Dibuja la victoria: fondo de colores y el texto de felicitaciones
// en negro (LEDs apagados) para que se lea bien sobre los colores.
void dibujarVictoria(int desplazamiento, int desfase) {
  matriz.fondoColores(desfase);
  matriz.dibujarTexto(TEXTO_VICTORIA, desplazamiento, NEGRO);
  matriz.mostrar();
}

// ============================================================

int main() {

  // Activa la comunicación por USB para poder imprimir mensajes.
  stdio_init_all();

  // Pausa SOLO en esta prueba, para darle tiempo al monitor serial.
  sleep_ms(2000);

  // Cambia la semilla de los números al azar, usando la hora, para
  // que las luces no salgan siempre en el mismo orden.
  srand(to_ms_since_boot(get_absolute_time()));

  // Guarda en qué momento está el programa. Empieza apagado.
  Momento momento = apagado;

  // Guarda el nivel que se está mostrando o jugando (1, 2 o 3).
  int nivel = 1;

  // Guarda la hora en que empezó el nivel que se está jugando.
  uint32_t horaInicioNivel = 0;

  // Guarda la hora del último paso de las luces (cuando bajaron una fila).
  uint32_t horaUltimoPaso = 0;

  // Cuenta cuántos pasos han pasado desde que salió la última luz.
  int pasosDesdeUltimaLuz = 0;

  // Guarda el camino donde estaba la jugadora la última vez, para
  // saber si hay que redibujar cuando mueve la perilla 2.
  int ultimoCaminoJugadora = -1;

  // Guarda la hora del último avance de una animación (texto,
  // aviso de nivel o derrota). Se reutiliza en todas.
  uint32_t horaTick = 0;

  // Cuántas columnas ha avanzado el texto en scroll, y cuánto se han
  // corrido las franjas de colores de la victoria.
  int desplazamiento = 0;
  int desfase = 0;

  // En qué paso va el aviso de nivel superado (0 al 9).
  int pasoAviso = 0;

  printf("Maquina apagada\n");

  // Ciclo principal: se repite para siempre, sin pausas.
  while (true) {

    // Leemos TODO en cada vuelta, sin importar el momento.
    interruptor.leer();
    boton1.leer();
    boton2.leer();
    perilla1.leer();
    perilla2.leer();

    // Preguntamos las pulsaciones UNA sola vez por vuelta, porque
    // huboPulsacion() borra el aviso después de entregarlo.
    bool pulsoBoton1 = boton1.huboPulsacion();
    bool pulsoBoton2 = boton2.huboPulsacion();

    // Preguntamos la hora una sola vez por vuelta.
    uint32_t ahora = to_ms_since_boot(get_absolute_time());

    // ========================================================
    // REVISAR EL INTERRUPTOR (en cualquier momento)
    // ========================================================

    // Interruptor en apagado pero el programa seguía funcionando:
    // apagamos todo, incluida la matriz, sin importar la animación.
    if (!interruptor.presionado && momento != apagado) {
      momento = apagado;
      nivel = 1;
      quitarLuces();
      matriz.limpiar();
      matriz.mostrar();
      printf("Maquina apagada\n");
    }

    // Interruptor en encendido pero el programa estaba apagado:
    // empieza el texto de bienvenida desde el principio.
    else if (interruptor.presionado && momento == apagado) {
      momento = bienvenida;
      nivel = 1;
      desplazamiento = 0;
      horaTick = ahora;
      dibujarBienvenida(desplazamiento);
      printf("Maquina encendida. Bienvenida\n");
    }

    // ========================================================
    // MOMENTO: APAGADO
    // ========================================================
    // No hace nada. Los botones y las perillas se ignoran.

    // ========================================================
    // MOMENTO: BIENVENIDA
    // ========================================================
    if (momento == bienvenida) {

      // Cada TICK_TEXTO el texto avanza una columna hacia la izquierda.
      if (ahora - horaTick >= TICK_TEXTO) {
        horaTick = ahora;
        desplazamiento = desplazamiento + 1;

        // Si el texto ya salió por completo, pasamos al menú.
        if (desplazamiento > matriz.anchoTexto(TEXTO_BIENVENIDA) + 8) {
          momento = menu;
          nivel = 1;
          dibujarMenu(nivel);
          printf("Menu listo. Nivel actual: %d\n", nivel);
        } else {
          dibujarBienvenida(desplazamiento);
        }
      }
    }

    // ========================================================
    // MOMENTO: MENÚ
    // ========================================================
    else if (momento == menu) {

      // Botón 1: pasa al siguiente nivel (1 -> 2 -> 3 -> 1).
      if (pulsoBoton1) {
        nivel = nivel + 1;
        if (nivel > 3) {
          nivel = 1;
        }
        dibujarMenu(nivel);
        printf("Navegando. Nivel actual: %d\n", nivel);
      }

      // Botón 2: selecciona el nivel y empieza el juego.
      if (pulsoBoton2) {
        printf("Nivel %d SELECCIONADO. Empieza el juego\n", nivel);

        momento = jugando;
        horaInicioNivel = ahora;
        horaUltimoPaso = ahora;
        pasosDesdeUltimaLuz = 0;
        ultimoCaminoJugadora = -1;
        quitarLuces();
      }
    }

    // ========================================================
    // MOMENTO: JUGANDO
    // ========================================================
    else if (momento == jugando) {

      // Esta banderita dice si hay algo nuevo que dibujar.
      bool hayCambio = false;

      // Esta banderita dice si la jugadora perdió en esta vuelta.
      bool perdio = false;

      // Preguntamos el camino donde está la jugadora.
      int caminoJugadora = perilla2.paso(4);

      // Si la jugadora cambió de camino, hay que redibujar.
      if (caminoJugadora != ultimoCaminoJugadora) {
        ultimoCaminoJugadora = caminoJugadora;
        hayCambio = true;
      }

      // ------------------------------------------------------
      // BOTÓN 2: la jugadora intenta atrapar la luz.
      // El botón solo avisa. Aquí revisamos dos cosas: si hay una
      // luz en la zona de captura, y si está en el mismo camino
      // donde está el LED de la jugadora.
      // ------------------------------------------------------
      if (pulsoBoton2) {
        bool atrapo = false;

        for (int i = 0; i < MAX_LUCES; i++) {
          if (luces[i].activa &&
              luces[i].camino == caminoJugadora &&
              luces[i].fila >= ZONA_CAPTURA) {
            // Había una luz en la zona y en su camino: la atrapa.
            luces[i].activa = false;
            atrapo = true;
            hayCambio = true;
            printf("Luz atrapada en el camino %d\n", caminoJugadora);
            break;
          }
        }

        // Si no atrapó nada y los fallos se castigan, pierde.
        if (!atrapo && CASTIGAR_FALLOS) {
          printf("Presiono sin luz en su camino\n");
          perdio = true;
        }
      }

      // ------------------------------------------------------
      // PERILLA 1: velocidad de bajada.
      // Convertimos el porcentaje en un tiempo entre pasos:
      // al 0% espera 500 ms (lento) y al 100% espera 100 ms (rápido).
      // ------------------------------------------------------
      uint32_t intervalo = 500 - (uint32_t)(perilla1.porcentaje() * 4.0f);

      // Si ya pasó el tiempo, todas las luces bajan una fila.
      if (ahora - horaUltimoPaso >= intervalo) {
        horaUltimoPaso = ahora;
        hayCambio = true;
        pasosDesdeUltimaLuz = pasosDesdeUltimaLuz + 1;

        for (int i = 0; i < MAX_LUCES; i++) {
          if (luces[i].activa) {
            luces[i].fila = luces[i].fila + 1;

            // Si la luz se pasó de la fila 7 sin ser atrapada, pierde.
            if (luces[i].fila > 7) {
              luces[i].activa = false;
              printf("Se le escapo una luz en el camino %d\n", luces[i].camino);
              perdio = true;
            }
          }
        }

        // Cada cierta cantidad de pasos sale una luz nueva. Mientras
        // más alto el nivel, más seguido salen (nivel 1 = 5 pasos,
        // nivel 2 = 4 pasos, nivel 3 = 3 pasos).
        int separacion = 6 - nivel;
        if (pasosDesdeUltimaLuz >= separacion) {
          pasosDesdeUltimaLuz = 0;

          // Buscamos un espacio libre en la lista y ponemos la luz
          // nueva arriba (fila 0), en un camino al azar de 1 a 4.
          for (int i = 0; i < MAX_LUCES; i++) {
            if (!luces[i].activa) {
              luces[i].activa = true;
              luces[i].fila = 0;
              luces[i].camino = 1 + (rand() % 4);
              break;
            }
          }
        }
      }

      // ------------------------------------------------------
      // SI PERDIÓ: la pantalla completa se pone roja.
      // ------------------------------------------------------
      if (perdio) {
        momento = derrota;
        nivel = 1;
        quitarLuces();
        matriz.rellenar(ROJO);
        matriz.mostrar();
        horaTick = ahora;
        printf("Perdio\n");
      }

      // ------------------------------------------------------
      // SI NO PERDIÓ: revisamos el tiempo del nivel y dibujamos.
      // ------------------------------------------------------
      else {

        // Revisamos si ya pasaron los 2:00 minutos del nivel.
        if (ahora - horaInicioNivel >= DURACION_NIVEL) {

          if (nivel < 3) {
            // Todavía quedan niveles. Dibujamos el último cuadro del
            // juego, le sacamos una foto para el zoom out y empezamos
            // el aviso con el número del nivel nuevo.
            dibujarJuego(caminoJugadora);
            matriz.guardarCopia();

            nivel = nivel + 1;
            quitarLuces();

            momento = avisoNivel;
            pasoAviso = 0;
            horaTick = ahora;
            dibujarAviso(pasoAviso, nivel);
            printf("Nivel superado. Empieza el nivel %d\n", nivel);
          } else {
            // Terminó el nivel 3: empieza la pantalla de victoria.
            quitarLuces();
            momento = victoria;
            desplazamiento = 0;
            desfase = 0;
            horaTick = ahora;
            dibujarVictoria(desplazamiento, desfase);
            printf("Gano el juego\n");
          }
        }

        // Si no cambió de momento y algo cambió, dibujamos el juego.
        else if (hayCambio) {
          dibujarJuego(caminoJugadora);
        }
      }
    }

    // ========================================================
    // MOMENTO: AVISO DE NIVEL SUPERADO
    // ========================================================
    // Se ignoran los botones. Cada PASO_AVISO avanza un paso:
    // zoom out, pantalla vacía, y el número parpadeando.
    else if (momento == avisoNivel) {

      if (ahora - horaTick >= PASO_AVISO) {
        horaTick = ahora;
        pasoAviso = pasoAviso + 1;

        // Al terminar los 10 pasos, empieza el nivel nuevo.
        if (pasoAviso >= 10) {
          momento = jugando;
          horaInicioNivel = ahora;
          horaUltimoPaso = ahora;
          pasosDesdeUltimaLuz = 0;
          ultimoCaminoJugadora = -1;
          printf("Empieza el nivel %d\n", nivel);
        } else {
          dibujarAviso(pasoAviso, nivel);
        }
      }
    }

    // ========================================================
    // MOMENTO: DERROTA
    // ========================================================
    // La pantalla ya está roja. Esperamos y volvemos al menú.
    else if (momento == derrota) {

      if (ahora - horaTick >= DURACION_DERROTA) {
        momento = menu;
        nivel = 1;
        dibujarMenu(nivel);
        printf("Menu listo. Nivel actual: %d\n", nivel);
      }
    }

    // ========================================================
    // MOMENTO: VICTORIA
    // ========================================================
    // Pantalla de colores con "FELICITACIONES" en scroll.
    else if (momento == victoria) {

      if (ahora - horaTick >= TICK_TEXTO) {
        horaTick = ahora;
        desplazamiento = desplazamiento + 1;
        desfase = desfase + 1;

        // Si el texto ya salió por completo, volvemos al menú.
        if (desplazamiento > matriz.anchoTexto(TEXTO_VICTORIA) + 8) {
          momento = menu;
          nivel = 1;
          dibujarMenu(nivel);
          printf("Menu listo. Nivel actual: %d\n", nivel);
        } else {
          dibujarVictoria(desplazamiento, desfase);
        }
      }
    }
  }
}