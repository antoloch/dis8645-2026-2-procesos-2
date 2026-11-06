#include "BotonGrupo1.h"

// constructor
BotonGrupo1::BotonGrupo1(int nuevoPin) {
  pin = nuevoPin;
  gpio_init(pin);
  gpio_set_dir(pin, GPIO_IN);
}

void BotonGrupo1::leer() {
  uint32_t tiempoActual = to_ms_since_boot(get_absolute_time());
  bool nuevoPresionado = gpio_get(pin);

  // Si hay un cambio físico en el pin, reiniciamos el temporizador
  if (nuevoPresionado != estadoAnterior) {
    ultimaLectura = tiempoActual;  
  }
  
  // Si han pasado más de 50ms desde el último cambio físico (anti-rebote)
  if ((tiempoActual - ultimaLectura) > 50) {
    if (nuevoPresionado != presionado) {
      presionado = nuevoPresionado;  
    }
  }

  // Lógica de estado y doble click
  if (presionado) {
    if (esperandoDoblePresionado) {
      if ((tiempoActual - ultimaLectura) < tiempoDoblePresionado) {
        estadoDoblePresionado = true;
        esperandoDoblePresionado = false;
      }
    }
    duracionPresionado = tiempoActual - ultimaLectura;
    duracionSuelto = 0;
  } else {
    esperandoDoblePresionado = true;
    duracionSuelto = tiempoActual - ultimaLectura;
    duracionPresionado = 0; 
  }

  // Desactivar la espera de doble click si se excede el tiempo
  if (esperandoDoblePresionado && ((tiempoActual - ultimaLectura) > tiempoDoblePresionado)) {
     esperandoDoblePresionado = false;
  }

  estadoAnterior = nuevoPresionado;
}

bool BotonGrupo1::doblePresionado() {
  if (estadoDoblePresionado) {
    estadoDoblePresionado = false;
    return true;
  }
  return false;
}

void BotonGrupo1::mantenerPresionado() {
  if (presionado && !presionadoProcesado) {
    estadoMantenerPresionado = !estadoMantenerPresionado;
    presionadoProcesado = true;  
  } 
  else if (!presionado) {
    presionadoProcesado = false;
  }
}

void BotonGrupo1::modoBorrar() {
  estadoMantenerPresionado = false;
}