#pragma once
#include "Config.h"

class Nave {
 public:
  void reiniciar(Sector inicial);
  bool mover_a(Sector s);          // true si cambio de sector
  bool recibir_dano();             // true si sigue viva
  int sumar_puntos(int puntos);    // devuelve cuantas vidas extra se ganaron (100 pts c/u)
  Sector sector() const { return sector_; }
  int vidas() const { return vidas_; }
  int puntos() const { return puntos_; }
  bool viva() const { return vidas_ > 0; }
 private:
  Sector sector_ = IZQUIERDA;
  int vidas_ = VIDAS_INICIALES;
  int puntos_ = 0;
};