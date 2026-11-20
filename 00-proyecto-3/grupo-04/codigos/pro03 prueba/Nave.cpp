#include "Nave.h"

void Nave::reiniciar(Sector inicial) {
  sector_ = inicial;
  vidas_ = VIDAS_INICIALES;
  puntos_ = 0;
}

bool Nave::mover_a(Sector s) {
  if (s == sector_) return false;
  sector_ = s;
  return true;
}

bool Nave::recibir_dano() {
  if (vidas_ > 0) vidas_--;
  return vidas_ > 0;
}

int Nave::sumar_puntos(int puntos) {
  int antes = puntos_ / PUNTOS_VIDA_EXTRA;
  puntos_ += puntos;
  int premios = puntos_ / PUNTOS_VIDA_EXTRA - antes;
  for (int i = 0; i < premios; i++)
    if (vidas_ < VIDAS_MAXIMAS) vidas_++;   // la melodia suena igual aunque ya tenga el maximo
  return premios;
}