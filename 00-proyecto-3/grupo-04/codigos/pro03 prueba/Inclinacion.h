#pragma once
#include <cstdint>
#include "Config.h"

// MPU6050: la inclinacion del control decide el sector (con histeresis)
class Inclinacion {
 public:
  bool iniciar();
  void actualizar(uint32_t ahora_ms);
  Sector sector() const;
  bool disponible() const;
 private:
  bool ok_ = false;
  Sector sector_ = IZQUIERDA;
  uint32_t ultimo_ms_ = 0;
};