#pragma once
#include <cstdint>
#include "Config.h"

// Enemigo u obstaculo. Ciclo: APARECE -> SE_ACERCA -> LLEGA -> FINALIZADA (o ELIMINADA si la dispararon)
class Amenaza {
 public:
  Amenaza(TipoAmenaza tipo, Sector sector, uint32_t t_inicio_ms, uint32_t t_total_ms);

  Evento actualizar(uint32_t t_ms);       // devuelve el evento de la transicion (o EV_NINGUNO)
  TipoEvento evento_de(EstadoAmenaza nuevo) const;
  void eliminar();                        // solo tiene efecto si es disparable
  bool disparable() const;                // enemigo en APARECE o SE_ACERCA
  bool terminada() const;                 // ELIMINADA o FINALIZADA
  bool es_enemigo() const { return tipo_ == TIPO_ENEMIGO; }
  Sector sector() const { return sector_; }
  EstadoAmenaza estado() const { return estado_; }

 private:
  TipoAmenaza tipo_;
  Sector sector_;
  EstadoAmenaza estado_;
  uint32_t t_inicio_ms_;
  uint32_t t_aparece_ms_;
  uint32_t t_total_ms_;
};