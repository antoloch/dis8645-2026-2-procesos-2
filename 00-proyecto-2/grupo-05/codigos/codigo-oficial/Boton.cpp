#include "Boton.h" // importar el archivo header

// ============================================================

// constructor
Boton::Boton(int nuevaPatita) {

  Boton::patita = nuevaPatita;  // guardar el valor

  gpio_init(Boton::patita);   // inicializar patita

  gpio_set_dir(Boton::patita, GPIO_IN);   // la patita es entrada: la raspberry lee, no escribe

  // No activamos pull-up ni pull-down internos porque el circuito
  // ya trae una resistencia de 10k a GND (pull-down externa).
  // Por eso el pin lee LOW en reposo y HIGH cuando se presiona.
}

// ============================================================

// metodos
void Boton::leer() {

  // Si el botón estaba en "soltado" en la vuelta anterior, ya cumplió
  // su momento y pasa a "noPresionado". Así "soltado" dura un solo
  // ciclo, como un aviso rápido.
  if (Boton::estado == soltado) {
    Boton::estado = noPresionado;
  }

  // leer boton (HIGH = true = presionado)
  bool lecturaAhora = gpio_get(Boton::patita);

  // preguntarle al reloj de la máquina qué hora es, en milisegundos
  uint32_t ahora = to_ms_since_boot(get_absolute_time());

  // Solo seguimos si la lectura es distinta a lo que ya sabíamos Y ya
  // pasó el tiempo de antirrebote. Si cambió muy rápido, se ignora:
  // es el "temblor" del botón físico.
  if (lecturaAhora != Boton::presionado &&
      (ahora - Boton::horaUltimoCambio) >= Boton::tiempoAntirrebote) {

    // anotar la hora de este cambio
    Boton::horaUltimoCambio = ahora;

    if (lecturaAhora) {
      // la persona acaba de presionar
      Boton::horaInicioPresion = ahora;
      Boton::presionar();
    } else {
      // la persona acaba de soltar
      Boton::soltar();
    }
  }

  // Si el botón sigue presionado, calculamos cuánto rato lleva así.
  if (Boton::presionado) {
    Boton::duracionPresionado = ahora - Boton::horaInicioPresion;
  }
}

void Boton::presionar() {
  Boton::presionado = true;
  Boton::estado = estaSiendoPresionado;

  // Levanta la banderita para avisar de inmediato. Sin esperas y sin
  // límite de pulsaciones por segundo.
  Boton::pulsacionPendiente = true;
}

void Boton::soltar() {
  Boton::presionado = false;
  Boton::estado = soltado;
  Boton::duracionPresionado = 0;
}

bool Boton::huboPulsacion() {

  // guardar la respuesta antes de borrarla
  bool respuesta = Boton::pulsacionPendiente;

  // bajar la banderita: el aviso ya se entregó y no se repite
  Boton::pulsacionPendiente = false;

  // entregar la respuesta
  return respuesta;
}