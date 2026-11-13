// Tono.cpp
// implementacion de la clase Tono

#include "Tono.h"

// constructor
Tono::Tono() {
  // parte en la mitad del rango, que corresponde al sonido original
  valor = 50;
  // calcula el filtro inicial para dejarlo listo
  calcularFiltro();
}

// asigna un nuevo valor y recalcula el filtro de inmediato
void Tono::cambiarValor(int nuevoValor) {
  // guarda el valor pedido
  valor = nuevoValor;
  // limita el valor y calcula el nuevo filtro
  calcularFiltro();
}

// define como se calcula el filtro
// se trabaja con valores de 0 a 100
void Tono::calcularFiltro() {
  // asegura que el valor no sea menor que cero
  if (valor < 0) {
    // fija el limite inferior en cero
    valor = 0;
  }
  // asegura que el valor no supere cien
  if (valor > 100) {
    // fija el limite superior en cien
    valor = 100;
  }

  if (valor < margenInferior) {
    // lado grave: filtro pasa bajos
    tipoFiltro = FILTRO_GRAVE;
    // recorrido va de 0.0 (valor 0) a 1.0 (valor margenInferior)
    float recorrido = (float)valor / (float)margenInferior;
    // escala exponencial entre corteGraveMinimo y corteGraveMaximo
    // el oido percibe las frecuencias de forma exponencial, asi el giro se siente parejo
    // con los valores por defecto: 0 -> 200 Hz (muy apagado) y 44 -> casi 5000 Hz
    frecuenciaCorte = corteGraveMinimo * powf(corteGraveMaximo / corteGraveMinimo, recorrido);
  } else if (valor > margenSuperior) {
    // lado brillante: filtro pasa altos
    tipoFiltro = FILTRO_BRILLANTE;
    // recorrido va de 0.0 (valor margenSuperior) a 1.0 (valor 100)
    float recorrido = (float)(valor - margenSuperior) / (float)(100 - margenSuperior);
    // escala exponencial entre corteBrillanteMinimo y corteBrillanteMaximo
    // con los valores por defecto: 56 -> poco mas de 20 Hz y 100 -> 2000 Hz (muy delgado)
    frecuenciaCorte = corteBrillanteMinimo * powf(corteBrillanteMaximo / corteBrillanteMinimo, recorrido);
  } else {
    // dentro del margen no se filtra, queda el sonido original
    tipoFiltro = FILTRO_NINGUNO;
    frecuenciaCorte = 0.0f;
    // con coeficiente 1.0 el filtro copia la muestra tal cual
    coeficiente = 1.0f;
    return;
  }

  // convierte la frecuencia de corte en el coeficiente del filtro de un polo
  // mientras mas alta la frecuencia, mas cerca de 1.0 queda el coeficiente
  coeficiente = 1.0f - expf(-2.0f * (float)M_PI * frecuenciaCorte / frecuenciaMuestreo);
}
