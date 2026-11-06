// BotonGrupo1.h
// Este archivo define la clase Boton
// desarrollado por: 
// 
// Licencia 

#ifndef BOTONGRUPO1_H
#define BOTONGRUPO1_H

// agregamos el GPIO
// lo que hace es agregar la biblioteca del GPIO
// que deglosado es General Purpose Input/Output
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
 
 // lugar que lee el Boton
 int pin;
 // detecta si Boton está siendo presionado
 bool presionado = false;
 // observa el estado anterior del Boton
 bool estadoAnterior = false;
 // valor asociado al boton funcionando como switch
 bool estadoMantenerPresionado = false;
 // detecta si se presionó el Boton dos veces
 bool estadoDoblePresionado = false;
 // variable que indica si se está esperando un doble click
 bool esperandoDoblePresionado = false;
 // variable que ayuda a procesar el estado del botón
 bool presionadoProcesado = false;



 // variable que ayuda a utilizar el modo borrar
 bool borrado = true;

 // para detectar tiempo se recomienda usar uint32_t
 // optimiza el uso de memoria

 // por cuanto tiempo se mantiene presionado
 uint32_t duracionPresionado = 0;
 // por cuanto tiempo se mantiene suelto
 uint32_t duracionSuelto = 0;
 // última vez que se leyó el estado del botón
 uint32_t ultimaLectura = 0;
 // ventana de tiempo para detectar doble click
 uint32_t tiempoDoblePresionado = 300; // tiempo en milisegundos para detectar doble click


 // definimos constructor
 // a qué numero de pin está conectado
 BotonGrupo1(int nuevoPin);
 // definimos métodos
 // lee que Boton está conectado en el gpio
 void leer();
 // detecta un doble click
 // devuelve true si se presionó dos veces
 bool doblePresionado();
 // convierte el boton pulsador en switch
 void mantenerPresionado();
 
 // metodos especificos

 // realiza una lectura y cambia de modo
 // leer() + modoSwitch() = modoBorrar()
 void modoBorrar();
}; 

#endif
