// BotonGrupo1.h
// Este archivo define la clase Boton
// desarrollado por: grupo 1 del proyecto 2
// Camila Ramírez, Bianka Vilchez y Dayana Pañitrur
// (adaptado para el proyecto 3: pull-up y tiempo de espera entre pulsaciones)
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
  // en esta versión la clase espera una conexión "pull-up":
  // - una pata del botón va a GND (tierra)
  // - la otra pata va directo al pin
  // - la resistencia pull-up la activa la propia clase por software
  // así, al soltar el botón el pin lee 1 y al presionarlo lee 0
  // la clase invierte esa lectura: presionado = true cuando el pin lee 0
  // no nos responsabilizamos por daños en el hardware si no se usa correctamente

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
  // OJO: esto sigue ocurriendo aunque la pulsación se ignore por la espera
  bool recienPresionado = false;
  // es true solo en la vuelta del loop en que una pulsación fue ACEPTADA
  // (pasó el filtro de rebote y además ya pasó la espera entre pulsaciones)
  // esta es la que usa main.cpp para contar toques
  bool pulsacionAceptada = false;
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

  // tiempo mínimo (en ms) entre una pulsación aceptada y la siguiente
  // se configura por instancia: 3000 para categorías, 0 para grabar
  // con 0 solo actúa el filtro de rebote
  uint32_t esperaEntrePulsaciones = 0;
  // si main.cpp lo pone en false, las pulsaciones no se aceptan
  // (no se guardan para después y no reinician la espera)
  // leer() sigue actualizando presionado y recienPresionado igual
  bool aceptarPulsaciones = true;

  // --- atributos internos (no modificar desde main.cpp) ---
  // los usan los métodos de la clase para hacer sus cálculos
  // si se cambian desde afuera, el Boton puede funcionar mal

  // observa el estado anterior del Boton
  bool estadoAnterior = false;
  // detecta si se presionó el Boton dos veces
  bool estadoDoblePresionado = false;
  // variable que indica si se está esperando un doble click
  bool esperandoDoblePresionado = false;
  // true cuando ya se aceptó al menos una pulsación desde que se encendió
  // (así la primera pulsación se puede contar altiro)
  bool hayPulsacionAceptada = false;
  // última vez que se leyó el estado del botón
  uint32_t ultimaLectura = 0;
  // lecturas para los diferentes modos de presionado
  uint32_t ultimaLecturaPresionado = 0;
  uint32_t ultimaLecturaDoblePresionado = 0;
  // momento en que se aceptó la última pulsación
  uint32_t ultimaPulsacionAceptada = 0;


  // definimos constructor
  // a qué numero de pin está conectado
  // y cuánta espera extra (ms) se pide entre pulsaciones aceptadas
  BotonGrupo1(int nuevoPin, uint32_t nuevaEspera = 0);
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
};

#endif
