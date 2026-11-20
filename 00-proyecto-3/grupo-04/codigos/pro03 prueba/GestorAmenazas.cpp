#include "GestorAmenazas.h"
#include "pico/rand.h"

void GestorAmenazas::limpiar() { amenazas_.clear(); }

void GestorAmenazas::reiniciar(uint32_t t_juego_ms) {
  limpiar();
  proxima_aparicion_ms_ = t_juego_ms + APARICION_INICIAL_MS;
}

void GestorAmenazas::actualizar(uint32_t t, const Dificultad& dif, std::vector<Evento>& eventos) {
  // 1) quitar las terminadas
  for (size_t i = 0; i < amenazas_.size();) {
    if (amenazas_[i].terminada()) amenazas_.erase(amenazas_.begin() + i);
    else i++;
  }
  // 2) nueva aparicion
  if ((int32_t)(t - proxima_aparicion_ms_) >= 0) {
    Sector s = (get_rand_32() % 2 == 0) ? IZQUIERDA : DERECHA;
    bool obstaculo = (get_rand_32() % 100) < (uint32_t)PORC_OBSTACULOS;
    Amenaza a(obstaculo ? TIPO_OBSTACULO : TIPO_ENEMIGO, s, t, dif.tiempo_acercamiento_ms());
    eventos.push_back({a.evento_de(AM_APARECE), s});
    amenazas_.push_back(a);
    proxima_aparicion_ms_ = t + dif.intervalo_aparicion_ms();
  }
  // 3) avanzar
  for (Amenaza& a : amenazas_) {
    Evento e = a.actualizar(t);
    if (e.tipo != EV_NINGUNO) eventos.push_back(e);
  }
}

Evento GestorAmenazas::disparar_a(Sector sector) {
  for (Amenaza& a : amenazas_) {
    if (a.sector() == sector && a.disparable()) {
      a.eliminar();
      return {EV_ENEMIGO_ELIMINADO, sector};
    }
  }
  return {EV_NINGUNO, sector};
}