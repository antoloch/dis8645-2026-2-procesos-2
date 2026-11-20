#pragma once
#include <cstdint>

// Niveles de dificultad + reloj de juego (el tiempo solo avanza mientras se juega)
class Dificultad {
  public:
    void reiniciar();                                      // nivel 1 y tiempo 0
    void avanzar_reloj(uint32_t ahora_ms, bool jugando);   // llamar en cada vuelta
    bool actualizar_nivel();                               // true si acaba de subir de nivel
    uint32_t tiempo_ms() const {
      return acumulado_ms_;
    }
    int nivel() const {
      return nivel_;
    }
    uint32_t intervalo_aparicion_ms() const;
    uint32_t tiempo_acercamiento_ms() const;
  private:
    int nivel_ = 1;
    uint32_t ultimo_ms_ = 0;
    uint32_t acumulado_ms_ = 0;
};