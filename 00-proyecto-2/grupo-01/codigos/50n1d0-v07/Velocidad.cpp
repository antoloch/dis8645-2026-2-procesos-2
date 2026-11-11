// Velocidad.cpp
// implementacion de la clase Velocidad

#include "Velocidad.h"

// constructor
Velocidad::Velocidad() {
  // parte en la mitad del rango, que corresponde a velocidad normal
  valor = 50;
  // calcula el factor inicial para dejarlo listo
  calcularFactor();
}

// asigna un nuevo valor y recalcula el factor de inmediato
void Velocidad::cambiarValor(int nuevoValor) {
  // guarda el valor pedido
  valor = nuevoValor;
  // limita el valor y calcula el nuevo factor
  calcularFactor();
}

// define como se calcula la velocidad
// se trabaja con valores de 0 a 100
void Velocidad::calcularFactor() {
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
    // lado lento: de 0 a margenInferior el factor va de factorMinimo a 1.0
    // con los valores por defecto: 0 -> 0.5x y 44 -> casi 1x
    // recorrido va de 0.0 (valor 0) a 1.0 (valor margenInferior)
    float recorrido = (float)valor / (float)margenInferior;
    // escala exponencial, se siente pareja al oido
    factor = powf(factorMinimo, 1.0f - recorrido);
  } else if (valor > margenSuperior) {
    // lado rapido: de margenSuperior a 100 el factor va de 1.0 a factorMaximo
    // con los valores por defecto: 56 -> poco mas de 1x y 100 -> 2x
    // recorrido va de 0.0 (valor margenSuperior) a 1.0 (valor 100)
    float recorrido = (float)(valor - margenSuperior) / (float)(100 - margenSuperior);
    // escala exponencial, se siente pareja al oido
    factor = powf(factorMaximo, recorrido);
  } else {
    // dentro del margen la velocidad queda fija en normal
    factor = 1.0f;
  }
}
