#include "Inclinacion.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdio>

bool Inclinacion::iniciar() {
  const uint8_t despertar[2] = {0x6B, 0x00};  // PWR_MGMT_1 = 0 (sale de reposo)
  ok_ = i2c_write_timeout_us(I2C_PUERTO, MPU_DIRECCION, despertar, 2, false, 5000) == 2;
  if (!ok_) printf("[MPU6050] no responde en 0x%02X\n", MPU_DIRECCION);
  return ok_;
}

void Inclinacion::actualizar(uint32_t ahora_ms) {
  if (!ok_ || (ahora_ms - ultimo_ms_) < INCLINACION_PERIODO_MS) return;
  ultimo_ms_ = ahora_ms;

  const uint8_t reg = 0x3B;  // ACCEL_XOUT_H
  uint8_t b[6];
  if (i2c_write_timeout_us(I2C_PUERTO, MPU_DIRECCION, &reg, 1, true, 3000) != 1) return;
  if (i2c_read_timeout_us(I2C_PUERTO, MPU_DIRECCION, b, 6, false, 3000) != 6) return;

  int16_t ax = (int16_t)((b[0] << 8) | b[1]);
  int16_t ay = (int16_t)((b[2] << 8) | b[3]);
  float g = (INCLINACION_EJE == 0 ? ax : ay) / 16384.0f;  // rango +-2 g
  if (INCLINACION_INVERTIR) g = -g;

  if (g > INCLINACION_UMBRAL_G) sector_ = DERECHA;
  else if (g < -INCLINACION_UMBRAL_G) sector_ = IZQUIERDA;
  // entre ambos umbrales: se mantiene el sector (histeresis)
}

Sector Inclinacion::sector() const { return sector_; }
bool Inclinacion::disponible() const { return ok_; }