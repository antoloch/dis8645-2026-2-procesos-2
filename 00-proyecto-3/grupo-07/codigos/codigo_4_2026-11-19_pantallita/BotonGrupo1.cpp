//BotonGrupo1.cpp
// desarrollado por: grupo 1 del proyecto 2
// Camila Ramírez, Bianka Vilchez y Dayana Pañitrur
#include "BotonGrupo1.h"

//constructor
BotonGrupo1::BotonGrupo1(int nuevoPin, uint32_t nuevaEspera) {
  pin = nuevoPin;
  // guardamos la espera entre pulsaciones de esta instancia
  esperaEntrePulsaciones = nuevaEspera;

  // inicializamos el pin como entrada
  gpio_init(pin);
  // configuramos el pin como entrada
  gpio_set_dir(pin, GPIO_IN);
  // el botón va a tierra, así que activamos la resistencia pull-up
  // suelto = 1, presionado = 0
  gpio_pull_up(pin);
}


// definimos el método leer
// definimos que la gran mayoria de lecturas ocurran en este metodo
// de esta manera escondemos la complejidad de la clase Boton
void BotonGrupo1::leer(){
  // leemos el tiempo transcurrido desde que se encendió el microcontrolador
  // tiempoActual almacena el tiempo en milisegundos
  uint32_t tiempoActual = to_ms_since_boot(get_absolute_time());

  // al comenzar cada lectura suponemos que el Boton no se acaba de presionar
  // más abajo, si detectamos que recién se presionó, lo cambiamos a true
  recienPresionado = false;
  // lo mismo para la pulsación aceptada: solo dura una vuelta
  pulsacionAceptada = false;

  // se crea una variable momentánea que lee el estado del pin
  // con pull-up, el pin lee 0 cuando el botón está presionado
  // por eso invertimos la lectura con !
  // si el pin lee 0, nuevoPresionado = true
  bool nuevoPresionado = !gpio_get(pin);

  // si el estado anterior es diferente del actual
  // != significa diferente
  if (nuevoPresionado != estadoAnterior) {
    // si lo anterior es verdad
    // actualizamos la última lectura
    // ahora ultimaLectura es igual al tiempo desde que se encendió el microcontrolador

    ultimaLectura = tiempoActual;
  }

  // ahora vamos a filtrar el ruido
  // si el tiempo transcurrido desde la última lectura es mayor a 50 milisegundos
  if ((tiempoActual - ultimaLectura) > 50) {
    // si la lectura actual es diferente de presionado
    // esto nos protege de tener miles de lecturas al mantener presionado el boton
    if (nuevoPresionado != presionado) {
      // presionado toma el valor de nuevoPresionado
      // actualizamos el valor oficial basicamente
      presionado = nuevoPresionado;

      //vamos a guardar el tiempo que transcurrió para cada evento importante
      ultimaLecturaPresionado = tiempoActual;

      // si el valor oficial cambió y ahora está presionado
      // significa que el Boton recién se presionó
      // si ahora está suelto, recienPresionado se queda en false
      recienPresionado = presionado;

      // --- espera entre pulsaciones aceptadas ---
      // solo evaluamos cuando hay un flanco de subida (soltar -> presionar)
      // así mantener apretado nunca cuenta más de una vez
      // y si empiezan a apretar durante la espera y lo dejan así,
      // ese toque se ignora y no se activa solo cuando termina la espera
      if (recienPresionado && aceptarPulsaciones) {
        // se acepta si es la primera desde que se encendió
        // o si ya pasó el tiempo de espera desde la última aceptada
        // >= significa mayor o igual
        if (!hayPulsacionAceptada ||
            (tiempoActual - ultimaPulsacionAceptada) >= esperaEntrePulsaciones) {
          pulsacionAceptada = true;
          hayPulsacionAceptada = true;
          // la espera empieza de nuevo solo con las pulsaciones aceptadas
          // los toques ignorados no la reinician ni quedan pendientes
          ultimaPulsacionAceptada = tiempoActual;
        }
      }
    }
  }

  // ahora vamos a calcular la duración que se mantuvo presionado o suelto
  if (presionado) {
    // calculamos la duración que se mantuvo presionado
    duracionPresionado = tiempoActual - ultimaLecturaPresionado;
    // reiniciamos la duración de suelto
    duracionSuelto = 0;
    // si presionado es falso
  } else {
    // calculamos la duración que se mantuvo suelto
    duracionSuelto = tiempoActual - ultimaLecturaPresionado;
    // reiniciamos la duración de presionado
    duracionPresionado = 0;
  }

  // actualizamos el estado anterior
  estadoAnterior = nuevoPresionado;
}


// lectura directa del pin, sin filtro de rebote
// con pull-up, el pin lee 0 cuando el botón está presionado
// no modifica ningún atributo, solo informa
bool BotonGrupo1::crudoPresionado() {
  return !gpio_get(pin);
}


// utilizamos bool para detectar doble click
// esto facilita su uso en el main.cpp
// solo leemos el estado de doble click y si es verdadero
bool BotonGrupo1::doblePresionado() {
  // obtenemos el tiempo actual para este metodo
  uint32_t tiempoActual = to_ms_since_boot(get_absolute_time());

  // definimos la ventana de tiempo para detectar doble click
  // si esperandoDoblePresionado es verdadero y el tiempo transcurrido
  // desde la última lectura es mayor a tiempoDoblePresionado
  // esperandoDoblePresionado se actualiza a falso
  // && es un operador lógico que significa y
  // osea que si 2 elementos son verdaderos
  // se ejecuta la instrucción
  // revisamos esto ANTES de detectar el click
  // así, si un segundo click llega tarde, no se pierde:
  // se cuenta como el primer click de un nuevo doble click
  // >= significa mayor o igual
  if (esperandoDoblePresionado && (tiempoActual - ultimaLecturaDoblePresionado >= tiempoDoblePresionado)) {
    esperandoDoblePresionado = false;
  }

  // -- deteccion de un doble click --
  // si presionado es verdadero
  // y además recién cambió de suelto a presionado (recienPresionado)
  if (recienPresionado) {
    // y si esperamos un doble click
    // y el tiempo transcurrido desde la última lectura es menor a tiempoDoblePresionado
    // entonces se detecta un doble click
    // (el tiempo ya lo revisamos arriba: si seguimos esperando,
    // es porque todavía estamos dentro de la ventana)
    if (esperandoDoblePresionado) {
      estadoDoblePresionado = true;
      esperandoDoblePresionado = false;
    } else {
      // si no esperábamos un doble click, este es el primer click
      // guardamos cuándo ocurrió y empezamos a esperar el segundo
      ultimaLecturaDoblePresionado = tiempoActual;
      esperandoDoblePresionado = true;
    }
  }

  // si estadoDoblePresionado es verdadero
  if (estadoDoblePresionado) {
    // reiniciamos estadoDoblePresionado a falso
    // para no leer de nuevo el estado de doble click
    estadoDoblePresionado = false;
    // devolvemos verdadero
    // return es una instrucción que devuelve un valor
    // y termina la accion de la funcion
    return true;
  }
  return false;
}

// metodo que convierte el boton pulsador en switch
void BotonGrupo1::presionarSwitch() {
  // si presiono el boton y antes no estaba presionado
  // se inicia el modo switch
  // que es lo mismo que decir
  // que se mantiene presionado el boton

  // si presiono el boton y antes estaba presionado
  // se desactiva el modo switch
  // que es lo mismo que decir
  // que se suelta el boton

  // recienPresionado es true solo una vez por cada vez que se presiona
  // por eso, aunque se mantenga apretado, el switch cambia una sola vez
  if (recienPresionado) {
    // cambiamos el estado de estadoPresionarSwitch
    // ! significa "lo contrario": true pasa a false y false pasa a true
    estadoPresionarSwitch = !estadoPresionarSwitch;
  }
}
