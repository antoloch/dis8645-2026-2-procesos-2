// Sonido.cpp
// Implementaciones del sonido con Pico SDK, PIO y DMA.
#include "Sonido.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/critical_section.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/pio.h"

#if !PICO_RP2350 || !defined(__arm__)
#error "Compilar para Raspberry Pi Pico 2 W / ARM"
#endif

// ============================================================
// PROGRAMA DEL PIO
/*
Programa PIO adaptado de Raspberry Pi pico-extras:
https://github.com/raspberrypi/pico-extras/blob/master/src/rp2_common/pico_audio_i2s/audio_i2s.pio
Copyright (c) 2020 Raspberry Pi (Trading) Ltd.

Copyright 2020 (c) 2020 Raspberry Pi (Trading) Ltd.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
   disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// Igual que en Matriz.cpp, guardamos aqui las instrucciones traducidas.
// Transmite 16 bits por canal, con el retardo I2S de un bit.
static const uint16_t i2s_instrucciones[] = {
  0x6001, // out pins, 1       lado izquierdo, BCLK bajo
  0x0840, // jmp x--, 0        BCLK alto
  0x7001, // out pins, 1       cambiar LRC en el ultimo bit
  0xf82e, // set x, 14         preparar el lado derecho
  0x7001, // out pins, 1       lado derecho, BCLK bajo
  0x1844, // jmp x--, 4        BCLK alto
  0x6001, // out pins, 1       cambiar LRC en el ultimo bit
  0xe82e  // set x, 14         preparar el lado izquierdo
};

static const struct pio_program i2s_programa = {
  i2s_instrucciones,
  8,
  -1
};
const int ENTRADA_I2S = 7;

// ============================================================
// CONFIGURACION DEL AUDIO
const uint32_t FRECUENCIA_AUDIO = 16000;
const int PIN_BCLK = 6;
const int PIN_LRC = 7;
const int PIN_DIN = 8;
const int MUESTRAS_BLOQUE = 64;

// La matriz original usa PIO0. El sonido tiene su propio PIO.
PIO pioSonido = pio1;
int maquinaSonido = -1;
int canalesSonido[2] = {-1, -1};
bool sonidoListo = false;
alignas(4) uint32_t bloquesSonido[2][MUESTRAS_BLOQUE] = {};
// Pila independiente de 8 KiB para el segundo nucleo.
alignas(8) uint32_t pilaSonido[2048];

// El candado protege solamente los datos de la nota entre nucleos.
critical_section_t candadoSonido;
uint16_t frecuenciaSonido = 0;
uint32_t finSonido = 0;
uint32_t versionSonido = 0;

// ============================================================
// PEDIR UNA NOTA DESDE EL JUEGO
void sonido(uint16_t frecuencia, uint32_t duracion) {
  if (!sonidoListo) {
    return;
  }
  if (frecuencia >= FRECUENCIA_AUDIO / 2 || duracion > 0x7fffffff) {
    frecuencia = 0;
    duracion = 0;
  }

  critical_section_enter_blocking(&candadoSonido);
  frecuenciaSonido = frecuencia;
  finSonido = to_ms_since_boot(get_absolute_time()) + duracion;
  versionSonido = versionSonido + 1;
  critical_section_exit(&candadoSonido);
}

void silencio() {
  sonido(0, 0);
}

// ============================================================
// SEGUNDO NUCLEO: GENERAR AUDIO CONTINUAMENTE
void ejecutarSonido() {
  uint32_t fase = 0;
  uint32_t ultimaVersion = 0;
  int libre = 0;

  // Los dos bloques empiezan con ceros.
  dma_start_channel_mask(1u << canalesSonido[0]);
  pio_sm_set_enabled(pioSonido, maquinaSonido, true);

  while (true) {
    // Espera solamente el nucleo de audio, nunca el juego.
    while (dma_channel_is_busy(canalesSonido[libre])) {
      tight_loop_contents();
    }

    critical_section_enter_blocking(&candadoSonido);
    uint16_t frecuencia = frecuenciaSonido;
    uint32_t fin = finSonido;
    uint32_t version = versionSonido;
    critical_section_exit(&candadoSonido);

    if (version != ultimaVersion) {
      ultimaVersion = version;
      fase = 0;
    }

    for (int i = 0; i < MUESTRAS_BLOQUE; i++) {
      int16_t muestra = 0;
      uint32_t ahora = to_ms_since_boot(get_absolute_time());
      if (frecuencia > 0 && (int32_t)(fin - ahora) > 0) {
        fase = fase + frecuencia;
        if (fase >= FRECUENCIA_AUDIO) {
          fase = fase - FRECUENCIA_AUDIO;
        }
        if (fase < FRECUENCIA_AUDIO / 2) {
          muestra = AUDIO_AMPLITUDE;
        } else {
          muestra = -AUDIO_AMPLITUDE;
        }
      }

      // Mismo int16_t en ambos canales. Sin nota, la palabra es cero.
      uint32_t palabra = (uint16_t)muestra;
      bloquesSonido[libre][i] = (palabra << 16) | palabra;
    }

    __dmb();
    dma_channel_set_read_addr(canalesSonido[libre], bloquesSonido[libre], false);
    dma_channel_set_trans_count(canalesSonido[libre], MUESTRAS_BLOQUE, false);
    // Recuperar la transmision si una pausa excepcional agoto ambos bloques.
    if (!dma_channel_is_busy(canalesSonido[1 - libre]) &&
        !dma_channel_is_busy(canalesSonido[libre])) {
      dma_start_channel_mask(1u << canalesSonido[libre]);
    }
    libre = 1 - libre;
  }
}

// ============================================================
// DEJAR LISTO EL PIO Y LANZAR EL SEGUNDO NUCLEO
bool iniciarSonido() {
  if (sonidoListo) {
    return true;
  }
  maquinaSonido = pio_claim_unused_sm(pioSonido, false);
  if (maquinaSonido < 0) {
    return false;
  }
  if (!pio_can_add_program(pioSonido, &i2s_programa)) {
    pio_sm_unclaim(pioSonido, maquinaSonido);
    return false;
  }

  canalesSonido[0] = dma_claim_unused_channel(false);
  canalesSonido[1] = dma_claim_unused_channel(false);
  if (canalesSonido[0] < 0 || canalesSonido[1] < 0) {
    for (int i = 0; i < 2; i++) {
      if (canalesSonido[i] >= 0) {
        dma_channel_unclaim(canalesSonido[i]);
      }
    }
    pio_sm_unclaim(pioSonido, maquinaSonido);
    return false;
  }

  critical_section_init(&candadoSonido);
  uint offset = pio_add_program(pioSonido, &i2s_programa);
  pio_gpio_init(pioSonido, PIN_DIN);
  pio_gpio_init(pioSonido, PIN_BCLK);
  pio_gpio_init(pioSonido, PIN_LRC);
  pio_sm_set_consecutive_pindirs(pioSonido, maquinaSonido, PIN_DIN, 1, true);
  pio_sm_set_consecutive_pindirs(pioSonido, maquinaSonido, PIN_BCLK, 2, true);

  pio_sm_config configuracion = pio_get_default_sm_config();
  sm_config_set_wrap(&configuracion, offset, offset + 7);
  sm_config_set_sideset(&configuracion, 2, false, false);
  sm_config_set_out_pins(&configuracion, PIN_DIN, 1);
  sm_config_set_sideset_pins(&configuracion, PIN_BCLK);
  sm_config_set_out_shift(&configuracion, false, true, 32);
  sm_config_set_fifo_join(&configuracion, PIO_FIFO_JOIN_TX);
  // 16 bits x 2 canales x 2 ciclos por bit = 64 ciclos por muestra.
  float divisor = clock_get_hz(clk_sys) / (FRECUENCIA_AUDIO * 64.0f);
  sm_config_set_clkdiv(&configuracion, divisor);
  pio_sm_init(pioSonido, maquinaSonido, offset, &configuracion);
  pio_sm_set_pins(pioSonido, maquinaSonido, 0);
  pio_sm_exec(pioSonido, maquinaSonido,
              pio_encode_jmp(offset + ENTRADA_I2S));

  for (int i = 0; i < 2; i++) {
    dma_channel_config configuracionDMA = dma_channel_get_default_config(canalesSonido[i]);
    channel_config_set_transfer_data_size(&configuracionDMA, DMA_SIZE_32);
    channel_config_set_read_increment(&configuracionDMA, true);
    channel_config_set_write_increment(&configuracionDMA, false);
    channel_config_set_dreq(&configuracionDMA, pio_get_dreq(pioSonido, maquinaSonido, true));
    channel_config_set_chain_to(&configuracionDMA, canalesSonido[1 - i]);
    dma_channel_configure(canalesSonido[i], &configuracionDMA,
                          &pioSonido->txf[maquinaSonido], bloquesSonido[i],
                          MUESTRAS_BLOQUE, false);
  }

  sonidoListo = true;
  multicore_launch_core1_with_stack(ejecutarSonido, pilaSonido, sizeof(pilaSonido));
  return true;
}

// ============================================================
// MELODIAS Y EFECTOS
struct Nota { uint16_t frecuencia; uint16_t duracion; };
const Nota entradaMenu[] = {{523, 70}, {659, 90}};
const Nota seleccionInicio[] = {{523, 70}, {659, 70}, {784, 120}};
const Nota entradaVictoria[] = {{523, 90}, {659, 90}, {784, 90}, {1047, 260}};
Nota salidaDerrota[] = {{180, 140}, {0, 25}, {147, 140}, {110, 400}};
Nota efectoBreve[1];
const uint16_t melodias[3][16] = {
  {262,330,392,523,392,330,294,392,330,262,330,440,392,330,294,262},
  {330,440,523,659,523,440,392,523,587,659,523,440,392,440,523,330},
  {523,659,784,988,880,784,659,784,988,1047,880,784,659,784,988,1047}
};
const uint16_t bienvenida[] = {523,659,784,659,587,698,880,784};
const uint16_t felicitaciones[] = {1047,784,880,988,1047,1319,1175,1047};
MomentoSonido actual = silencioSonido;
int nivelActual = 1;
uint32_t indiceMelodia = 0;
uint16_t ultimoFallo = 180;
const Nota* secuencia = nullptr;
uint8_t cantidad = 0, indice = 0;
uint32_t finNota = 0;
uint16_t fondoHz = 0;
uint32_t fondoHasta = 0;

// Al terminar un efecto, retoma la nota del paso que todavia esta vigente.
void reanudarFondo() {
  uint32_t ahora = to_ms_since_boot(get_absolute_time());
  if (fondoHz > 0 && (int32_t)(fondoHasta - ahora) > 0) {
    sonido(fondoHz, fondoHasta - ahora);
  } else {
    silencio();
  }
}

void iniciarSecuencia(const Nota* notas, uint8_t total) {
  secuencia = notas;
  cantidad = total;
  indice = 0;
  finNota = to_ms_since_boot(get_absolute_time()) + notas[0].duracion;
  sonido(notas[0].frecuencia, notas[0].duracion);
}

void breve(uint16_t frecuencia, uint16_t duracion) {
  efectoBreve[0] = {frecuencia, duracion};
  iniciarSecuencia(efectoBreve, 1);
}

void fondo(uint16_t frecuencia, uint32_t duracion) {
  fondoHz = frecuencia;
  fondoHasta = to_ms_since_boot(get_absolute_time()) + duracion;
  if (secuencia == nullptr) {
    sonido(frecuencia, duracion);
  }
}

// ============================================================
// AVANZAR LAS NOTAS SIN ESPERAS

void actualizarSonido() {
  uint32_t ahora = to_ms_since_boot(get_absolute_time());
  if (secuencia == nullptr || (int32_t)(ahora - finNota) < 0) {
    return;
  }

  indice = indice + 1;
  if (indice >= cantidad) {
    secuencia = nullptr;
    reanudarFondo();
    return;
  }

  finNota = ahora + secuencia[indice].duracion;
  sonido(secuencia[indice].frecuencia, secuencia[indice].duracion);
}

// ============================================================
// CAMBIAR DE MOMENTO, IGUAL QUE EN EL JUEGO

void cambiarSonido(MomentoSonido nuevo, int nivel) {
  actual = nuevo;
  nivelActual = nivel;
  if (nivelActual < 1) {
    nivelActual = 1;
  } else if (nivelActual > 3) {
    nivelActual = 3;
  }

  indiceMelodia = 0;
  secuencia = nullptr;
  fondoHz = 0;
  silencio();

  if (nuevo == menuSonido) {
    iniciarSecuencia(entradaMenu, 2);
  } else if (nuevo == juegoSonido) {
    iniciarSecuencia(seleccionInicio, 3);
  } else if (nuevo == derrotaSonido) {
    salidaDerrota[0].frecuencia = ultimoFallo;
    iniciarSecuencia(salidaDerrota, 4);
    ultimoFallo = 180;
  } else if (nuevo == victoriaSonido) {
    iniciarSecuencia(entradaVictoria, 4);
  }
  // Bienvenida y aviso suenan cuando la animacion avanza.
}

// ============================================================
// EFECTOS DE LOS CONTROLES Y LAS LUCES

void efectoSonido(EfectoSonido tipo, int valor) {
  if (actual == silencioSonido) {
    return;
  }

  int camino = valor;
  if (camino < 1) {
    camino = 1;
  } else if (camino > 4) {
    camino = 4;
  }

  if (tipo == navegarSonido) {
    breve(350 + camino * 120, 110);
  } else if (tipo == capturaSonido) {
    breve(880 + (camino - 1) * 110, 100);
  } else if (tipo == falloSonido) {
    ultimoFallo = 180;
    breve(180, 130);
  } else if (tipo == perdidaSonido) {
    ultimoFallo = 130;
    breve(130, 150);
  } else if (tipo == caminoSonido && secuencia == nullptr) {
    breve(440 + camino * 110, 35);
  } else if (tipo == nuevaLuzSonido && secuencia == nullptr) {
    breve(1568, 22);
  }
}

// ============================================================
// SEGUIR EL AVANCE REAL DEL JUEGO Y LAS ANIMACIONES

void pasoSonido(uint32_t intervalo) {
  if (actual != juegoSonido) {
    return;
  }

  // Una nota por bajada de fila. La perilla cambia el ritmo del sonido.
  uint16_t frecuencia = melodias[nivelActual - 1][indiceMelodia % 16];
  indiceMelodia = indiceMelodia + 1;
  fondo(frecuencia, intervalo * 3 / 4);
}

void textoSonido(int desplazamiento, uint32_t intervalo) {
  if (actual != bienvenidaSonido && actual != victoriaSonido) {
    return;
  }
  if (desplazamiento % 4 != 0) {
    return;
  }

  uint16_t frecuencia;
  if (actual == bienvenidaSonido) {
    frecuencia = bienvenida[indiceMelodia % 8];
  } else {
    frecuencia = felicitaciones[indiceMelodia % 8];
  }
  indiceMelodia = indiceMelodia + 1;
  fondo(frecuencia, intervalo * 3);
}

void avisoSonido(int paso, uint32_t intervalo) {
  if (actual != cambioNivelSonido) {
    return;
  }

  const uint16_t zoom[3] = {659, 784, 988};
  if (paso >= 0 && paso < 3) {
    sonido(zoom[paso], intervalo / 2);
  } else if (paso >= 4 && paso % 2 == 0) {
    sonido(700 + nivelActual * 120, intervalo / 2);
  } else {
    silencio();
  }
}
