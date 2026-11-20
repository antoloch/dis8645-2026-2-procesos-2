#include "Juego.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

Juego::Juego() : boton_disparo_(PIN_BOTON_DISPARO), boton_inicio_(PIN_BOTON_INICIO),
                 boton_izq_(PIN_BOTON_IZQUIERDA), boton_der_(PIN_BOTON_DERECHA),
                 interruptor_(PIN_INTERRUPTOR) {}

void Juego::iniciar_i2c() {
  i2c_init(I2C_PUERTO, I2C_FRECUENCIA_HZ);
  gpio_set_function(PIN_I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(PIN_I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(PIN_I2C_SDA);
  gpio_pull_up(PIN_I2C_SCL);
}

void Juego::iniciar() {
  iniciar_i2c();
  boton_disparo_.iniciar();
  boton_inicio_.iniciar();
#if CONTROL_BOTONES_MANUALES
  boton_izq_.iniciar();
  boton_der_.iniciar();
#endif
  interruptor_.iniciar();
  perilla_.iniciar();
  inclinacion_.iniciar();
  pantalla_.iniciar();
  motor_.iniciar();
  estado_ = ESTADO_APAGADO;
}

void Juego::actualizar() {
  uint32_t ahora = to_ms_since_boot(get_absolute_time());

  boton_disparo_.actualizar(ahora);
  boton_inicio_.actualizar(ahora);
#if CONTROL_BOTONES_MANUALES
  boton_izq_.actualizar(ahora);
  boton_der_.actualizar(ahora);
  pulso_izq_ = boton_izq_.recien_presionado();   // se leen siempre para descartar pulsaciones fuera de JUGANDO
  pulso_der_ = boton_der_.recien_presionado();
#endif
  interruptor_.actualizar(ahora);
  perilla_.actualizar(ahora);
  inclinacion_.actualizar(ahora);
  dificultad_.avanzar_reloj(ahora, estado_ == ESTADO_JUGANDO);
  motor_.establecer_volumen(perilla_.porcentaje());

  // El sensor manda cuando cambia de lado; los botones mandan cuando se pulsan
  if (inclinacion_.sector() != ultimo_sensor_) {
    ultimo_sensor_ = inclinacion_.sector();
    objetivo_ = ultimo_sensor_;
  }

  // Se leen SIEMPRE para descartar pulsaciones fuera de JUGANDO
  bool disparo = boton_disparo_.recien_presionado();
  bool inicio = boton_inicio_.recien_presionado();

  if (!interruptor_.presionado()) {
    if (estado_ != ESTADO_APAGADO) apagar(ahora);
  } else if (estado_ == ESTADO_APAGADO) {
    encender(ahora);
  } else {
    switch (estado_) {
      case ESTADO_ESPERA:
      case ESTADO_GAME_OVER:
        if (inicio) iniciar_partida(ahora);
        break;
      case ESTADO_PAUSA:
        if (inicio) reanudar(ahora);
        break;
      case ESTADO_JUGANDO:
        actualizar_jugando(ahora, disparo, inicio);
        break;
      default:
        break;
    }
  }

  pantalla_.actualizar(ahora);
  motor_.actualizar(ahora);
}

// ---------- transiciones ----------
void Juego::encender(uint32_t ahora) {
  estado_ = ESTADO_ESPERA;
  pantalla_.establecer_modo(ModoPantalla::ESPERA, ahora);
}

void Juego::apagar(uint32_t ahora) {
  gestor_.limpiar();
  motor_.silenciar();
  estado_ = ESTADO_APAGADO;
  pantalla_.establecer_modo(ModoPantalla::APAGADA, ahora);
}

void Juego::iniciar_partida(uint32_t ahora) {
  nave_.reiniciar(objetivo_);   // parte del ultimo lado elegido
  dificultad_.reiniciar();
  gestor_.reiniciar(0);
  proxima_alerta_ms_ = ALERTA_ULTIMA_VIDA_MS;
  estado_ = ESTADO_JUGANDO;
  refrescar_hud();
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_inicio(), MotorAudio::Modo::REEMPLAZAR);
  pantalla_.establecer_modo(ModoPantalla::JUEGO, ahora);
  pantalla_.lanzar_animacion(Animacion::INICIO, ahora);
}

void Juego::pausar(uint32_t ahora) {
  estado_ = ESTADO_PAUSA;
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_pausa(), MotorAudio::Modo::REEMPLAZAR);
  pantalla_.establecer_modo(ModoPantalla::PAUSA, ahora);
}

void Juego::reanudar(uint32_t ahora) {
  estado_ = ESTADO_JUGANDO;
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_inicio(), MotorAudio::Modo::REEMPLAZAR);
  pantalla_.establecer_modo(ModoPantalla::JUEGO, ahora);
}

void Juego::terminar_partida(uint32_t ahora) {
  estado_ = ESTADO_GAME_OVER;
  gestor_.limpiar();
  refrescar_hud();
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_derrota(), MotorAudio::Modo::REEMPLAZAR);
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_puntaje_final(nave_.puntos()));
  pantalla_.establecer_modo(ModoPantalla::GAME_OVER, ahora);
}

// ---------- jugando ----------
void Juego::actualizar_jugando(uint32_t ahora, bool disparo, bool inicio) {
  if (inicio) { pausar(ahora); return; }

  if (pulso_izq_) objetivo_ = IZQUIERDA;
  if (pulso_der_) objetivo_ = DERECHA;
  nave_.mover_a(objetivo_);

  if (disparo) {
    motor_.reproducir(canal_de(nave_.sector()), MotorAudio::sonido_disparo(), MotorAudio::Modo::REEMPLAZAR);
    Evento e = gestor_.disparar_a(nave_.sector());
    if (e.tipo == EV_ENEMIGO_ELIMINADO) {
      motor_.reproducir(canal_de(e.sector), MotorAudio::sonido_explosion());
      if (nave_.sumar_puntos(PUNTOS_POR_ENEMIGO) > 0)
        motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_recompensa());
    }
  }

  uint32_t t = dificultad_.tiempo_ms();

  if (dificultad_.actualizar_nivel()) {
    motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_level_up());
    pantalla_.lanzar_animacion(Animacion::LEVEL_UP, ahora);
  }

  eventos_.clear();
  gestor_.actualizar(t, dificultad_, eventos_);
  procesar_eventos(t, ahora);

  // Alerta de ultima vida (se reinicia al recibir dano para no pisar el impacto)
  if (estado_ == ESTADO_JUGANDO && nave_.vidas() == 1 && (int32_t)(t - proxima_alerta_ms_) >= 0) {
    motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_ultima_vida());
    proxima_alerta_ms_ = t + ALERTA_ULTIMA_VIDA_MS;
  }

  refrescar_hud();
}

void Juego::procesar_eventos(uint32_t t, uint32_t ahora) {
  for (const Evento& e : eventos_) {
    Canal c = canal_de(e.sector);
    switch (e.tipo) {
      case EV_ENEMIGO_APARECE:   motor_.reproducir(c, MotorAudio::sonido_enemigo_presente()); break;
      case EV_ENEMIGO_SE_ACERCA: motor_.reproducir(c, MotorAudio::sonido_enemigo_se_acerca()); break;
      case EV_ENEMIGO_LLEGA:
        motor_.reproducir(c, MotorAudio::sonido_ataque());
        if (nave_.sector() == e.sector) recibir_dano(t, ahora);
        break;
      case EV_OBSTACULO_APARECE: motor_.reproducir(c, MotorAudio::sonido_obstaculo()); break;
      case EV_OBSTACULO_LLEGA:
        if (nave_.sector() == e.sector) recibir_dano(t, ahora);
        break;
      default: break;
    }
    if (estado_ != ESTADO_JUGANDO) break;
  }
}

void Juego::recibir_dano(uint32_t t, uint32_t ahora) {
  bool viva = nave_.recibir_dano();
  motor_.reproducir(CANAL_AMBOS, MotorAudio::sonido_impacto(), MotorAudio::Modo::REEMPLAZAR);
  proxima_alerta_ms_ = t + ALERTA_ULTIMA_VIDA_MS;   // la alerta no pisa el impacto
  if (!viva) terminar_partida(ahora);
}

void Juego::refrescar_hud() {
  DatosHud h;
  h.vidas = nave_.vidas();
  h.nivel = dificultad_.nivel();
  h.puntos = nave_.puntos();
  h.sector = nave_.sector();
  pantalla_.actualizar_hud(h);
}