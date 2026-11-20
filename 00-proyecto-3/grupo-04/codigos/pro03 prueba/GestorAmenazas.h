#pragma once
#include <cstdint>
#include <vector>
#include "Config.h"
#include "Amenaza.h"
#include "Dificultad.h"

class GestorAmenazas {
 public:
  void reiniciar(uint32_t t_juego_ms);
  void limpiar();
  // Avanza las amenazas, crea nuevas y agrega los eventos ocurridos a 'eventos'
  void actualizar(uint32_t t_juego_ms, const Dificultad& dificultad, std::vector<Evento>& eventos);
  // Elimina el enemigo disparable mas antiguo del sector. EV_ENEMIGO_ELIMINADO o EV_NINGUNO
  Evento disparar_a(Sector sector);
 private:
  std::vector<Amenaza> amenazas_;
  uint32_t proxima_aparicion_ms_ = 0;
};