// Velocidad.h
// Este archivo define la clase Velocidad
// convierte un valor de 0 a 100 en un factor de velocidad de reproduccion
// 0 = 0.5x (mitad de velocidad), 45 a 55 = 1x (normal), 100 = 2x (doble de velocidad)
// entre 45 y 55 hay un margen fijo en 1x, asi es facil dejar la perilla en velocidad normal
// aunque el potenciometro oscile un poco
// fuera del margen la escala es exponencial, asi cada lado de la perilla se siente parejo al oido
// al cambiar la velocidad tambien cambia el tono, como un disco de vinilo:
// mas rapido suena mas agudo y mas lento suena mas grave
// solo afecta a los sonidos del catalogo, los sonidos de sistema suenan siempre a 1x

#ifndef VELOCIDAD_H
#define VELOCIDAD_H

// incluye funciones matematicas para el calculo exponencial
#include <math.h>

class Velocidad {

  public:

  // --- atributos para leer desde main.cpp ---

  // valor pedido de 0 a 100
  // para cambiarlo usar cambiarValor() y no asignarlo directo
  int valor = 50;
  // factor de velocidad calculado a partir de valor
  // 1.0 es velocidad normal, 0.5 la mitad y 2.0 el doble
  // es lo que se le entrega al ampli con cambiarVelocidad()
  float factor = 1.0f;

  // --- limites del rango ---

  // factor cuando valor es 0
  float factorMinimo = 0.5f;
  // factor cuando valor es 100
  float factorMaximo = 2.0f;

  // --- margen de velocidad normal ---
  // todo valor entre margenInferior y margenSuperior (incluidos) da factor 1.0
  int margenInferior = 45;
  int margenSuperior = 55;

  // -- constructor --
  // deja la velocidad en 50, es decir velocidad normal
  Velocidad();

  // --metodos--
  // asigna un nuevo valor de 0 a 100 y recalcula el factor
  void cambiarValor(int nuevoValor);

  private:

  // limita valor entre 0 y 100 y calcula factor
  void calcularFactor();
};

#endif
