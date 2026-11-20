#pragma once
#include <cstdint>
#include "Config.h"

enum class ModoPantalla { APAGADA, ESPERA, JUEGO, PAUSA, GAME_OVER };
enum class Animacion { NINGUNA, INICIO, LEVEL_UP };

struct DatosHud {
  int vidas = 0;
  int nivel = 1;
  int puntos = 0;
  Sector sector = IZQUIERDA;
};

// LCD 2004 (20x4) por I2C con PCF8574. Sin bloqueo: dibuja en un buffer y
// envia pocos caracteres por vuelta.
class Pantalla {
 public:
  bool iniciar();
  void establecer_modo(ModoPantalla modo, uint32_t ahora_ms);
  void lanzar_animacion(Animacion a, uint32_t ahora_ms);
  void actualizar_hud(const DatosHud& hud);
  void actualizar(uint32_t ahora_ms);   // llamar en cada vuelta
  bool disponible() const { return direccion_ != 0; }

 private:
  uint8_t direccion_ = 0;
  bool luz_ = true;
  int cursor_ = -1;
  char deseado_[LCD_FILAS][LCD_COLUMNAS];
  char mostrado_[LCD_FILAS][LCD_COLUMNAS];
  ModoPantalla modo_ = ModoPantalla::APAGADA;
  Animacion anim_ = Animacion::NINGUNA;
  uint32_t t_modo_ = 0, t_anim_ = 0;
  DatosHud hud_;

  // bajo nivel
  void escribir_nibble(uint8_t nibble);
  void enviar(uint8_t valor, bool dato);
  void comando(uint8_t c) { enviar(c, false); }
  void dato(uint8_t d) { enviar(d, true); }
  void aplicar_luz(bool encendida);
  void volcar();
  // dibujo
  void limpiar_buffer();
  void poner(int fila, int col, const char* texto);
  void centrar(int fila, const char* texto);
  void poner_vidas(int fila);
  void dibujar(uint32_t ahora_ms);
  void dibujar_hud();
};