// Boton.cpp
// implementaciones de la clase

// importar el archivo header
#include "Boton.h"

// funcion chiquitita para saber la hora en milisegundos
static uint32_t ahora() {
  return to_ms_since_boot(get_absolute_time());
}

// constructor
Boton::Boton(int nuevaPatita, bool nuevoPermitirDoble) {

  // guardar los valores
  Boton::patita = nuevaPatita;
  Boton::permitirDoble = nuevoPermitirDoble;

  // inicializar patita
  gpio_init(Boton::patita);
  // la patita es entrada
  gpio_set_dir(Boton::patita, GPIO_IN);
  // el boton va a GND, asi que usamos pull-up interno
  // (sin presionar lee 1, presionado lee 0)
  gpio_pull_up(Boton::patita);
}

void Boton::leer() {

  // borrar los eventos de la vuelta anterior
  Boton::seAcabaDePresionar = false;
  Boton::seAcabaDeSoltar = false;
  Boton::clicSimple = false;
  Boton::dobleClic = false;
  Boton::pulsacionLarga = false;

  uint32_t t = ahora();

  // leer boton (invertido por el pull-up)
  bool lectura = !gpio_get(Boton::patita);

  // antirrebote: solo creemos el cambio si se mantiene estable
  if (lectura != Boton::ultimaLectura) {
    Boton::ultimaLectura = lectura;
    Boton::ultimoCambio = t;
  }

  if ((t - Boton::ultimoCambio) >= (uint32_t)Boton::tiempoAntirrebote
      && lectura != Boton::presionado) {
    if (lectura) {
      Boton::presionar();
    }
    else {
      Boton::soltar();
    }
  }

  if (Boton::presionado) {
    // calcular cuanto rato lleva presionado
    Boton::duracionPresionado = t - Boton::inicioPresion;

    // pulsacion larga (solo se dispara una vez por pulsacion)
    if (!Boton::largoDisparado
        && Boton::duracionPresionado >= Boton::tiempoLargo) {
      Boton::largoDisparado = true;
      Boton::pulsacionLarga = true;
      Boton::clics = 0;
    }
  }
  else if (Boton::clics > 0
           && (t - Boton::ultimoSoltado) >= (uint32_t)Boton::ventanaDoble) {
    // paso la ventana y nadie mas apreto: era un clic simple
    Boton::clicSimple = true;
    Boton::clics = 0;
  }
}

// metodos
void Boton::presionar() {
  Boton::presionado = true;
  Boton::seAcabaDePresionar = true;
  Boton::inicioPresion = ahora();
  Boton::largoDisparado = false;
}

void Boton::soltar() {
  Boton::presionado = false;
  Boton::duracionPresionado = 0;
  Boton::seAcabaDeSoltar = true;

  // si fue pulsacion larga no cuenta como clic
  if (Boton::largoDisparado) {
    return;
  }

  // sin doble clic: el clic simple sale al tiro
  if (!Boton::permitirDoble) {
    Boton::clicSimple = true;
    return;
  }

  Boton::clics++;
  Boton::ultimoSoltado = ahora();

  // segundo clic dentro de la ventana
  if (Boton::clics >= 2) {
    Boton::dobleClic = true;
    Boton::clics = 0;
  }
}