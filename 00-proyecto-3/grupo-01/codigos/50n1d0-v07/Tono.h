// Tono.h
// Este archivo define la clase Tono
// convierte un valor de 0 a 100 en un filtro que cambia el timbre del sonido
// no cambia la nota ni la duracion, solo el "color" del sonido:
// 0 = muy apagado / grave, 45 a 55 = sonido original, 100 = muy brillante / agudo
// entre 45 y 55 hay un margen sin filtro, asi es facil dejar el sonido original
// aunque el potenciometro oscile un poco
// solo afecta a los sonidos del catalogo, los sonidos de sistema suenan siempre igual
//
// como funciona:
// - lado grave (0 a 44): filtro pasa bajos, deja pasar los graves y quita los agudos
//   mientras mas cerca de 0, mas agudos se quitan
// - lado brillante (56 a 100): filtro pasa altos, deja pasar los agudos y quita los graves
//   mientras mas cerca de 100, mas graves se quitan
// los dos son filtros simples de un polo, livianos para la raspi

#ifndef TONO_H
#define TONO_H

// incluye funciones matematicas para el calculo exponencial
#include <math.h>

// tipos de filtro que puede entregar la clase
// los usa Ampli::cambiarTono() para saber que filtro aplicar
const int FILTRO_GRAVE = -1;     // pasa bajos
const int FILTRO_NINGUNO = 0;    // sonido original
const int FILTRO_BRILLANTE = 1;  // pasa altos

class Tono {

  public:

  // --- atributos para leer desde main.cpp ---

  // valor pedido de 0 a 100
  // para cambiarlo usar cambiarValor() y no asignarlo directo
  int valor = 50;
  // que filtro se aplica: FILTRO_GRAVE, FILTRO_NINGUNO o FILTRO_BRILLANTE
  int tipoFiltro = FILTRO_NINGUNO;
  // frecuencia de corte del filtro en Hz
  // vale 0 cuando tipoFiltro es FILTRO_NINGUNO
  float frecuenciaCorte = 0.0f;
  // coeficiente del filtro, de 0.0 a 1.0
  // es lo que se le entrega al ampli con cambiarTono()
  float coeficiente = 1.0f;

  // --- margen sin filtro ---
  // todo valor entre margenInferior y margenSuperior (incluidos) deja el sonido original
  int margenInferior = 45;
  int margenSuperior = 55;

  // --- limites del lado grave (pasa bajos) ---
  // frecuencia de corte cuando valor es 0, el sonido queda muy apagado
  float corteGraveMinimo = 200.0f;
  // frecuencia de corte justo antes del margen, casi no se nota el filtro
  float corteGraveMaximo = 5000.0f;

  // --- limites del lado brillante (pasa altos) ---
  // frecuencia de corte justo despues del margen, casi no se nota el filtro
  float corteBrillanteMinimo = 20.0f;
  // frecuencia de corte cuando valor es 100, el sonido queda muy delgado y brillante
  float corteBrillanteMaximo = 2000.0f;

  // muestras por segundo de los sonidos
  // debe ser igual a la frecuencia del ampli (Ampli::frecuencia)
  float frecuenciaMuestreo = 11025.0f;

  // -- constructor --
  // deja el tono en 50, es decir sonido original
  Tono();

  // --metodos--
  // asigna un nuevo valor de 0 a 100 y recalcula el filtro
  void cambiarValor(int nuevoValor);

  private:

  // limita valor entre 0 y 100 y calcula tipoFiltro, frecuenciaCorte y coeficiente
  void calcularFiltro();
};

#endif
