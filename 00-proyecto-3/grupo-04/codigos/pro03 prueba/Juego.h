#pragma once
#include <cstdint>
#include <vector>
#include "Config.h"
#include "Boton.h"
#include "Perilla.h"
#include "Inclinacion.h"
#include "Pantalla.h"
#include "MotorAudio.h"
#include "Nave.h"
#include "Dificultad.h"
#include "GestorAmenazas.h"

// Maquina de estados y coordinador: APAGADO -> ESPERA -> JUGANDO <-> PAUSA -> GAME_OVER
class Juego {
 public:
  Juego();
  void iniciar();
  void actualizar();   // una vuelta del bucle principal (sin bloqueos)

 private:
  Boton boton_disparo_, boton_inicio_;
  Boton boton_izq_, boton_der_;   // solo si CONTROL_BOTONES_MANUALES = 1
  Boton interruptor_;          // interruptor de encendido: contacto a GND = encendido
  Perilla perilla_;
  Inclinacion inclinacion_;
  Pantalla pantalla_;
  MotorAudio motor_;
  Nave nave_;
  Dificultad dificultad_;
  GestorAmenazas gestor_;
  Sector objetivo_ = IZQUIERDA;        // sector al que quiere ir la nave (sensor o botones)
  Sector ultimo_sensor_ = IZQUIERDA;
  bool pulso_izq_ = false, pulso_der_ = false;
  EstadoJuego estado_ = ESTADO_APAGADO;
  uint32_t proxima_alerta_ms_ = 0;
  std::vector<Evento> eventos_;

  void iniciar_i2c();
  void encender(uint32_t ahora);
  void apagar(uint32_t ahora);
  void iniciar_partida(uint32_t ahora);
  void pausar(uint32_t ahora);
  void reanudar(uint32_t ahora);
  void terminar_partida(uint32_t ahora);
  void actualizar_jugando(uint32_t ahora, bool disparo, bool inicio);
  void procesar_eventos(uint32_t t_juego, uint32_t ahora);
  void recibir_dano(uint32_t t_juego, uint32_t ahora);
  void refrescar_hud();
};