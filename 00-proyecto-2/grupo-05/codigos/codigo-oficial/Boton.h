//  - Botón 1: navegar entre los niveles (en el menú).
//  - Botón 2: seleccionar el nivel (en el menú) y atrapar
//    la luz (durante el juego).
// ============================================================

// Esqueleto base.
#ifndef BOTON_H
#define BOTON_H

// esto lo agregamos para GPIO general purpose input output
#include "hardware/gpio.h"

// Esto lo agregamos para usar el reloj de la máquina. Nos sirve
// para medir cuánto tiempo pasa sin usar pausas (delays), porque
// el juego nunca se debe quedar congelado esperando un botón.
#include "pico/stdlib.h"

// ============================================================

// Los tres estados que puede tener un botón. El botón siempre
// está en uno solo de ellos:
//  - noPresionado: nadie lo está tocando.
//  - estaSiendoPresionado: la persona lo mantiene apretado.
//  - soltado: la persona acaba de soltarlo (dura un solo ciclo).
enum EstadoBoton { noPresionado, estaSiendoPresionado, soltado };

// ============================================================

// definir clase Boton
class Boton {

  // nada de andar privatizando
  public:

// ============================================================

  // atributos
  bool presionado = false;  // Guarda si el botón está presionado en este momento (true o false).
  int duracionPresionado = 0; // Guarda cuántos milisegundos lleva presionado el botón.
                              // Cuando se suelta vuelve a 0.
  int patita;   // Guarda el pin (la patita) de la raspberry donde está conectado el botón.

  // Guarda el estado actual del botón (uno de los tres de arriba).
  EstadoBoton estado = noPresionado;

  // Es una banderita que se levanta cuando la persona presiona el
  // botón y se queda levantada hasta que el programa principal
  // pregunte. Así no se pierde ninguna pulsación aunque el programa
  // esté ocupado en otra cosa.
  bool pulsacionPendiente = false;

  // Guarda la hora (en milisegundos desde que se encendió la máquina)
  // en que la persona empezó a presionar. 
  // Sirve para calcular duracionPresionado.
  uint32_t horaInicioPresion = 0;

  // Guarda la hora del último cambio aceptado. Sirve para el antirrebote.
  uint32_t horaUltimoCambio = 0;

  // Tiempo en milisegundos que se ignoran los cambios después de
  // detectar uno, porque el botón físico "tiembla" al apretarse.
  // 20 ms es tan corto que la persona no lo nota, ni siquiera si
  // presiona muy rápido. Es fácil de cambiar si hace falta.
  uint32_t tiempoAntirrebote = 20;

// ============================================================

  // constructor
  // Recibe el pin donde está conectado el botón.
  Boton(int nuevaPatita);

// ============================================================

  // metodos

  void leer();   // Lee el pin y decide si la persona presionó o soltó el botón.
  void presionar();   // Cambia el botón a "estaSiendoPresionado".
  void soltar();   // Cambia el botón a "soltado" y reinicia la duración.
  bool huboPulsacion();  // Responde "¿hubo una pulsación nueva?". Devuelve true una sola vez
                         // por pulsación, para que no se cuente dos veces.
 
};

#endif