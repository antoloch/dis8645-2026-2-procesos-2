// Perilla.cpp
// implementaciones de la clase

// importar el archivo header
#include "Perilla.h"

// constructor
Perilla::Perilla(int nuevaPatita) {

  // guardar el valor
  Perilla::patita = nuevaPatita;

  // Enciende el ADC de la raspberry. Es como prender el traductor de
  // voltaje a número. Llamarlo dos veces (una por perilla) no hace daño.
  adc_init();

  // Le avisa a la raspberry que este pin va a leer voltaje analógico
  // y no un simple "sí o no" como el botón.
  adc_gpio_init(Perilla::patita);
}

// metodos

void Perilla::leer() {

  // El ADC tiene varias entradas llamadas canales y no se identifican
  // con el nombre del pin: GP26 es el canal 0, GP27 el 1 y GP28 el 2.
  // Por eso restamos 26 para saber qué canal le pedimos.
  adc_select_input(Perilla::patita - 26);

  // Lee el voltaje de la perilla ya convertido a número (0 a 4095)
  // y lo guarda en posicion.
  Perilla::posicion = adc_read();
}

float Perilla::porcentaje() {

  // Regla de tres: si 4095 es el 100%, entonces posicion es el
  // resultado de multiplicar por 100 y dividir entre 4095.
  // Usamos 100.0f (con decimales) para que la división no se redondee.
  return Perilla::posicion * 100.0f / 4095.0f;
}

int Perilla::paso(int cantidadPasos) {

  // Calcula el ancho de cada parte. Ejemplo: 4096 / 4 = 1024, o sea
  // que cada camino ocupa 1024 números del giro.
  float ancho = 4096.0f / cantidadPasos;

  // Calcula en qué paso cae la lectura actual. Ejemplo: 1500 / 1024
  // da 1, y le sumamos 1 para que los pasos empiecen en 1 y no en 0.
  int candidato = (int)(Perilla::posicion / ancho) + 1;

  // Si la perilla está girada al máximo, el cálculo podría dar un paso
  // de más. Con esto nos aseguramos de no pasarnos del último.
  if (candidato > cantidadPasos) {
    candidato = cantidadPasos;
  }

  // Si es la primera vez que se pregunta, todavía no hay paso anterior,
  // así que aceptamos el que acabamos de calcular.
  if (Perilla::ultimoPaso == -1) {
    Perilla::ultimoPaso = candidato;
  }

  // Si el paso calculado es distinto al anterior, revisamos si la
  // perilla realmente cruzó el borde o si solo está temblando.
  else if (candidato != Perilla::ultimoPaso) {

    // Margen de seguridad: el 10% del ancho de un paso. Para cambiar
    // de camino, la perilla debe pasarse del borde por este margen.
    float margen = ancho * 0.10f;

    // Borde de abajo y borde de arriba del paso en el que estábamos.
    float inicio = (Perilla::ultimoPaso - 1) * ancho;
    float fin = Perilla::ultimoPaso * ancho;

    // Solo cambiamos de paso si la perilla se salió del paso anterior
    // por más del margen, hacia abajo o hacia arriba. Si no, nos
    // quedamos donde estábamos y evitamos el parpadeo.
    if (Perilla::posicion < inicio - margen ||
        Perilla::posicion >= fin + margen) {
      Perilla::ultimoPaso = candidato;
    }
  }

  // Entrega el paso en el que está la perilla.
  return Perilla::ultimoPaso;
}