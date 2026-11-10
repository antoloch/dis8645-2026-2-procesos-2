// BotonGrupo1.h
// Este archivo define la clase Boton
// desarrollado por:
//
// licencia

#ifndef BOTONGRUPO1_H
#define BOTONGRUPO1_H

// agregamos el GPIO
// lo que hace es agregar la biblioteca del GPIO
// que desglosado es General Purpose Input/Output
#include "hardware/gpio.h"
#include "pico/stdlib.h"

// a continuación definimos la clase Boton
class BotonGrupo1 {

  // la hacemos public
  public:
  // definimos los atributos de Boton

  // IMPORTANTE
  // esta clase funciona solo con resistencia externa,
  // es decir que revisar su circuito antes de usar
  // no nos responsabilizamos por daños en el hardware si no se usa correctamente
  // la clase espera una conexión "pull-down":
  // - una pata del botón va a 3.3V
  // - la otra pata va al pin y, a través de una resistencia, a GND
  // así, al soltar el botón el pin lee 0 (false)
  // y al presionarlo el pin recibe voltaje y lee 1 (true)

  // todos los atributos son public, pero los separamos en dos grupos
  // para saber cuáles conviene usar desde main.cpp y cuáles no

  // --- atributos para leer desde main.cpp ---

  // lugar que lee el Boton
  int pin;
  // detecta si Boton está siendo presionado
  bool presionado = false;
  // es true solo en la vuelta del loop en que el Boton
  // pasa de estar suelto a estar presionado
  // (a esto se le llama "flanco de subida")
  // lo calcula leer() y lo usan doblePresionado() y presionarSwitch()
  bool recienPresionado = false;
  // valor asociado al boton funcionando como switch
  bool estadoPresionarSwitch = false;

  // para detectar tiempo se recomienda usar uint32_t
  // optimiza el uso de memoria
  // además, to_ms_since_boot() entrega el tiempo en uint32_t
  // al ser un número sin signo (nunca negativo), la resta
  // tiempoActual - ultimaLectura sigue dando el resultado correcto
  // incluso cuando el contador de tiempo se reinicia (cerca de los 49 días)

  // por cuanto tiempo se mantiene presionado
  uint32_t duracionPresionado = 0;
  // por cuanto tiempo se mantiene suelto
  uint32_t duracionSuelto = 0;
  // ventana de tiempo para detectar doble click
  uint32_t tiempoDoblePresionado = 300;

  // --- atributos internos (no modificar desde main.cpp) ---
  // los usan los métodos de la clase para hacer sus cálculos
  // si se cambian desde afuera, el Boton puede funcionar mal

  // observa el estado anterior del Boton
  bool estadoAnterior = false;
  // detecta si se presionó el Boton dos veces
  bool estadoDoblePresionado = false;
  // variable que indica si se está esperando un doble click
  bool esperandoDoblePresionado = false;
  // última vez que se leyó el estado del botón
  uint32_t ultimaLectura = 0;
  // lecturas para los diferentes modos de presionado
  uint32_t ultimaLecturaPresionado = 0;
  uint32_t ultimaLecturaDoblePresionado = 0;


  // definimos constructor
  // a qué numero de pin está conectado
  BotonGrupo1(int nuevoPin);
  // definimos métodos
  // lee que BotonGrupo1 está conectado en el gpio
  void leer();
  // detecta un doble click
  // devuelve true si se presionó dos veces
  // IMPORTANTE: llamar a leer() antes de este método en cada vuelta del loop
  bool doblePresionado();
  // convierte el boton pulsador en switch
  // IMPORTANTE: llamar a leer() antes de este método en cada vuelta del loop
  void presionarSwitch();

  // metodos especificos

  // apaga el modo switch
  // estadoPresionarSwitch vuelve a false, aunque estuviera encendido
  // se usa junto a leer() y presionarSwitch()
  // por ejemplo, para "borrar" o reiniciar lo que se había activado
  void modoBorrar();
};

#endif
