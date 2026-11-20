#include "Amenaza.h"

Amenaza::Amenaza(TipoAmenaza tipo, Sector sector, uint32_t t_inicio_ms, uint32_t t_total_ms)
    : tipo_(tipo), sector_(sector), estado_(AM_APARECE), t_inicio_ms_(t_inicio_ms),
      t_aparece_ms_(t_total_ms * PORC_FASE_APARECE / 100), t_total_ms_(t_total_ms) {}

TipoEvento Amenaza::evento_de(EstadoAmenaza nuevo) const {
  if (tipo_ == TIPO_ENEMIGO) {
    switch (nuevo) {
      case AM_APARECE:   return EV_ENEMIGO_APARECE;
      case AM_SE_ACERCA: return EV_ENEMIGO_SE_ACERCA;
      case AM_LLEGA:     return EV_ENEMIGO_LLEGA;
      default:           return EV_NINGUNO;
    }
  }
  switch (nuevo) {                       // el obstaculo no suena al acercarse
    case AM_APARECE: return EV_OBSTACULO_APARECE;
    case AM_LLEGA:   return EV_OBSTACULO_LLEGA;
    default:         return EV_NINGUNO;
  }
}

Evento Amenaza::actualizar(uint32_t t_ms) {
  Evento ninguno{EV_NINGUNO, sector_};
  if (estado_ == AM_ELIMINADA || estado_ == AM_FINALIZADA) return ninguno;
  if (estado_ == AM_LLEGA) { estado_ = AM_FINALIZADA; return ninguno; }

  uint32_t transcurrido = t_ms - t_inicio_ms_;
  if (estado_ == AM_APARECE && transcurrido >= t_aparece_ms_) {
    estado_ = AM_SE_ACERCA;
    return {evento_de(AM_SE_ACERCA), sector_};
  }
  if (estado_ == AM_SE_ACERCA && transcurrido >= t_total_ms_) {
    estado_ = AM_LLEGA;
    return {evento_de(AM_LLEGA), sector_};
  }
  return ninguno;
}

bool Amenaza::disparable() const {
  return es_enemigo() && (estado_ == AM_APARECE || estado_ == AM_SE_ACERCA);
}

void Amenaza::eliminar() {
  if (disparable()) estado_ = AM_ELIMINADA;
}

bool Amenaza::terminada() const { return estado_ == AM_ELIMINADA || estado_ == AM_FINALIZADA; }