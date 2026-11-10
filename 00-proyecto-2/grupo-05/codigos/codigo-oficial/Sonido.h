// Sonido.h
// Declaraciones del sonido. El juego pide notas y sigue funcionando.
#ifndef SONIDO_H
#define SONIDO_H

#include <stdint.h>

// ============================================================
// AMPLITUD DEL SONIDO
// 2000 es el valor probado. Para reducir: 1000, 500, 200 o 100.
// 0 envia silencio digital. Guardar y volver a compilar para aplicar.
constexpr int AUDIO_AMPLITUDE{2000};
static_assert(AUDIO_AMPLITUDE >= 0 && AUDIO_AMPLITUDE <= 2000,
              "Usar un entero entre 0 y 2000");

// Los momentos del sonido siguen los momentos del juego.
enum MomentoSonido {
  silencioSonido, bienvenidaSonido, menuSonido, juegoSonido,
  cambioNivelSonido, derrotaSonido, victoriaSonido
};

enum EfectoSonido {
  navegarSonido, caminoSonido, capturaSonido,
  falloSonido, perdidaSonido, nuevaLuzSonido
};

// ============================================================
// INICIALIZACION Y NOTAS
bool iniciarSonido();
void sonido(uint16_t frecuencia, uint32_t duracion);
void silencio();

// ============================================================
// MOMENTOS, EFECTOS Y AVANCE
void cambiarSonido(MomentoSonido nuevo, int nivel = 1);
void efectoSonido(EfectoSonido efecto, int valor = 1);
void actualizarSonido();
void pasoSonido(uint32_t intervalo);
void textoSonido(int desplazamiento, uint32_t intervalo);
void avisoSonido(int paso, uint32_t intervalo);

#endif
