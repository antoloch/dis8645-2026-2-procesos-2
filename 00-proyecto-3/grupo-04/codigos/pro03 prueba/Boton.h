#pragma once
#include <cstdint>

// Boton con antirrebote y deteccion de flanco (activo en bajo, pull-up interno)
class Boton {
 public:
  explicit Boton(unsigned pin);
  void iniciar();
  void actualizar(uint32_t ahora_ms);
  bool recien_presionado();       // true UNA sola vez por pulsacion
  bool presionado() const;        // estado estable actual
 private:
  unsigned pin_;
  bool ultima_lectura_ = false;
  bool estable_ = false;
  bool flanco_ = false;
  uint32_t t_cambio_ = 0;
};