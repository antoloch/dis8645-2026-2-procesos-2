#ifndef BOTON_H
#define BOTON_H

// esto lo agregamos para GPIO y para el tiempo
// general purpose input output
#include "pico/stdlib.h"
#include "hardware/gpio.h"


// Boton.h
// declaraciones de la clase Boton


// definir clase Boton
class Boton {

  // todo publico
  // nada de andar privatizando
  public:

  // atributos
  bool presionado = false;
  int duracionPresionado = 0;   
  // milisegundos que lleva presionado
  int patita;

  // eventos
  // son true SOLO en la vuelta del loop donde ocurren
  bool seAcabaDePresionar = false;
  bool seAcabaDeSoltar = false;
  bool clicSimple = false;
  bool dobleClic = false;
  bool pulsacionLarga = false;

  // configuracion (en milisegundos)
  bool permitirDoble;           
  // si es false, el clic simple sale al tiro
  int tiempoAntirrebote = 20;
  int tiempoLargo = 1000;       
  // cuanto rato para ser pulsacion larga
  int ventanaDoble = 300;       
  // tiempo maximo entre 2 clics

  // atributos internos
  bool ultimaLectura = false;
  bool largoDisparado = false;
  int clics = 0;
  uint32_t ultimoCambio = 0;
  uint32_t inicioPresion = 0;
  uint32_t ultimoSoltado = 0;


  // constructor
  Boton(int nuevaPatita, bool nuevoPermitirDoble = true);

  // metodos
  void leer();
  void presionar();
  void soltar();

};

#endif