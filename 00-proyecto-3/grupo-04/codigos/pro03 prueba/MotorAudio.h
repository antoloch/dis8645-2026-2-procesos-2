#pragma once
#include <cstdint>
#include <deque>
#include "Config.h"

// Reproduce listas de notas por canal SIN BLOQUEO (cola avanzada por tiempo)
class MotorAudio {
 public:
  enum class Modo { ENCOLAR, REEMPLAZAR };

  MotorAudio();
  void iniciar();
  void reproducir(Canal canal, const ListaNotas& notas, Modo modo = Modo::ENCOLAR);
  void actualizar(uint32_t ahora_ms);   // llamar en cada vuelta
  void silenciar();
  void establecer_volumen(uint8_t porcentaje);

  // ---- Los sonidos (listas de notas) ----
  static ListaNotas sonido_inicio();            // 1
  static ListaNotas sonido_enemigo_presente();  // 2
  static ListaNotas sonido_enemigo_se_acerca(); // 3
  static ListaNotas sonido_ataque();            // 4
  static ListaNotas sonido_obstaculo();         // 5
  static ListaNotas sonido_disparo();           // 6
  static ListaNotas sonido_explosion();         // 7
  static ListaNotas sonido_impacto();           // 8
  static ListaNotas sonido_ultima_vida();       // 9
  static ListaNotas sonido_recompensa();        // 10
  static ListaNotas sonido_derrota();           // 11 (burlesco/triste)
  static ListaNotas sonido_puntaje_final(int puntos); // 12
  static ListaNotas sonido_pausa();             // 13
  static ListaNotas sonido_level_up();          // 14 (alegre)

 private:
  struct Voz {
    std::deque<Nota> cola;
    bool activa = false;
    uint32_t fin_ms = 0;
    uint16_t hz = 0;
  };
  Voz voces_[2];
  uint8_t volumen_;
  void encolar(int voz, const ListaNotas& notas, Modo modo);
  // Salida de hardware (PWM en Wokwi / I2S en el fisico, segun AUDIO_MODO)
  void salida_iniciar();
  void salida_tono(Canal canal, uint16_t hz, uint8_t volumen_pct);  // hz 0 = silencio
  void salida_actualizar();
};