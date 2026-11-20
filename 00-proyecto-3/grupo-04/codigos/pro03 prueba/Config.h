#pragma once
// config.h - Constantes, pines, enums y structs del PROYECTO 03 (sin clase)
#include <cstdint>
#include <vector>
#include "hardware/i2c.h"

// ===== Modo de audio: PWM (Wokwi, 2 buzzers) o I2S (fisico, Pico Audio Pack) =====
#define AUDIO_MODO_PWM 0
#define AUDIO_MODO_I2S 1
#define AUDIO_MODO AUDIO_MODO_PWM   // <-- cambiar a AUDIO_MODO_I2S en el fisico

// ===== Pines =====
constexpr unsigned PIN_BOTON_DISPARO = 2;
constexpr unsigned PIN_BOTON_INICIO  = 3;   // inicio / pausa / reanudar
constexpr unsigned PIN_INTERRUPTOR   = 4;   // encendido / apagado
// Botones izquierda/derecha: SOLO para probar en Wokwi (en el fisico se usa el MPU6050)
#define CONTROL_BOTONES_MANUALES 1   // 1 = Wokwi (con botones), 0 = fisico (solo sensor)
constexpr unsigned PIN_BOTON_IZQUIERDA = 12;
constexpr unsigned PIN_BOTON_DERECHA   = 13;
constexpr unsigned PIN_PERILLA       = 26;  // ADC0
constexpr unsigned PIN_I2C_SDA       = 6;   // I2C1: LCD + MPU6050 (GP0/GP1 quedan libres para el monitor serie)
constexpr unsigned PIN_I2C_SCL       = 7;
constexpr unsigned PIN_AUDIO_IZQ     = 14;  // PWM (solo modo Wokwi)
constexpr unsigned PIN_AUDIO_DER     = 16;  // PWM (solo modo Wokwi)
// Pico Audio Pack (I2S) - POR VERIFICAR en la ficha del fabricante
constexpr unsigned PIN_I2S_DATA = 9;
constexpr unsigned PIN_I2S_BCK  = 10;
constexpr unsigned PIN_I2S_LRCK = 11;
constexpr unsigned PIN_I2S_MUTE = 22;

// ===== I2C =====
#define I2C_PUERTO i2c1
constexpr uint32_t I2C_FRECUENCIA_HZ = 400000;
constexpr uint8_t  MPU_DIRECCION   = 0x68;
constexpr uint8_t  LCD_DIRECCION_1 = 0x27;
constexpr uint8_t  LCD_DIRECCION_2 = 0x3F;
constexpr int LCD_COLUMNAS = 20;
constexpr int LCD_FILAS    = 4;
constexpr int LCD_CARACTERES_POR_CICLO = 8;  // limita el trabajo por vuelta (sin bloquear)
constexpr char LCD_CHAR_CORAZON = 1;

// ===== Inclinacion (MPU6050) =====
constexpr int   INCLINACION_EJE = 0;        // 0 = eje X, 1 = eje Y
constexpr bool  INCLINACION_INVERTIR = false;
constexpr float INCLINACION_UMBRAL_G = 0.35f; // histeresis: zona muerta entre -umbral y +umbral
constexpr uint32_t INCLINACION_PERIODO_MS = 20;

// ===== Reglas de juego =====
constexpr int VIDAS_INICIALES = 3;
constexpr int VIDAS_MAXIMAS = 3;
constexpr int PUNTOS_POR_ENEMIGO = 10;
constexpr int PUNTOS_VIDA_EXTRA = 100;
constexpr int PORC_OBSTACULOS = 25;         // % de apariciones que son obstaculos
constexpr int PORC_FASE_APARECE = 40;       // % del tiempo en estado APARECE
constexpr uint32_t NIVEL_DURACION_MS = 15000;
constexpr uint32_t APARICION_INICIAL_MS = 3000, APARICION_MIN_MS = 800, APARICION_BAJA_MS = 300;
constexpr uint32_t ACERCA_INICIAL_MS = 2000,    ACERCA_MIN_MS = 600,    ACERCA_BAJA_MS = 150;
constexpr uint32_t ANTIRREBOTE_MS = 30;
constexpr uint32_t ALERTA_ULTIMA_VIDA_MS = 3000;
constexpr int PUNTAJE_PITIDOS_MAX = 30;

// ===== Audio =====
constexpr int PWM_DUTY_MAX_PCT = 50;        // duty con perilla al 100%
constexpr int VOLUMEN_INICIAL_PCT = 30;
constexpr int MAX_NOTAS_EN_COLA = 48;

// ===== Enums =====
enum Sector : uint8_t { IZQUIERDA = 0, DERECHA = 1 };
enum Canal : uint8_t { CANAL_IZQUIERDO = 0, CANAL_DERECHO = 1, CANAL_AMBOS = 2 };
enum EstadoJuego : uint8_t { ESTADO_APAGADO, ESTADO_ESPERA, ESTADO_JUGANDO, ESTADO_PAUSA, ESTADO_GAME_OVER };
enum EstadoAmenaza : uint8_t { AM_APARECE, AM_SE_ACERCA, AM_LLEGA, AM_ELIMINADA, AM_FINALIZADA };
enum TipoEvento : uint8_t {
  EV_NINGUNO,
  EV_ENEMIGO_APARECE, EV_ENEMIGO_SE_ACERCA, EV_ENEMIGO_LLEGA, EV_ENEMIGO_ELIMINADO,
  EV_OBSTACULO_APARECE, EV_OBSTACULO_LLEGA
};

// ===== Structs simples =====
struct Nota   { uint16_t frecuencia_hz; uint16_t duracion_ms; };  // frecuencia 0 = silencio
struct Evento { TipoEvento tipo; Sector sector; };
using ListaNotas = std::vector<Nota>;

inline Canal canal_de(Sector s) { return s == IZQUIERDA ? CANAL_IZQUIERDO : CANAL_DERECHO; }
enum TipoAmenaza : uint8_t { TIPO_ENEMIGO, TIPO_OBSTACULO };