// main.cpp — Proyecto 3
// Botones de cuentos, música, mensaje de la persona cuidadora y grabar,
// con LED de grabación, pantalla y mensajes por el monitor serial.
//
// créditos:
// - BotonGrupo1: grupo 1 del proyecto 2 (Camila Ramírez, Bianka Vilchez y Dayana Pañitrur)
// - Lucecita: material docente de la sesión 08a
// - Pantallita: usa la biblioteca ILI934X de Darren Horrocks (licencia BSD de 3 cláusulas,
//   ver LICENSE-ILI934X.txt) y sus fuentes FreeMono
//
// main solo coordina las categorías y los estados;
// la lectura de botones la hace BotonGrupo1, el LED lo maneja Lucecita
// y lo que se ve en la pantalla lo dibuja Pantallita.

#include <stdio.h>
#include "pico/stdlib.h"
#include "BotonGrupo1.h"
#include "Lucecita.h"
#include "Pantallita.h"

// ------------------------------------------------------------
// DECLARACIONES QUE TODAVÍA NO IMPLEMENTAMOS (solo comentarios)
// ------------------------------------------------------------
// pantalla: ya muestra inicio, categoría (símbolo provisorio), mensaje y grabación
//   (ver Pantallita.h). Lo que falta, en main:
//   título del audio en pantalla
//   void mostrarTituloAudio(/* categoría, audio */);
// tonos: avisos sonoros cortos
//   void reproducirTono(/* frecuencia, duración */);
// audio real: reproducir desde la SD hacia el amplificador y el parlante
//   void reproducirAudio(/* archivo */);
//   void detenerAudio();
// fin del audio: detectar cuándo el audio termina de verdad
//   bool audioTermino();
//   (cuando exista, el mensaje de la persona cuidadora deja de estar
//    activo al terminar y se puede volver a escuchar)
// micrófono: capturar la voz con el MAX4466
//   void capturarMicrofono();
// tarjeta SD: revisar, leer y guardar archivos
//   bool sdDisponible();
//   bool mensajeGuardadoEnSD();

// ------------------------------------------------------------
// PINES Y CONSTANTES (según el wokwi)
// ------------------------------------------------------------
const int PIN_CUENTOS = 2;   // botón cuentos (GP2)
const int PIN_MUSICA  = 3;   // botón música (GP3)
const int PIN_MENSAJE = 4;   // botón mensaje de la persona cuidadora (GP4)
const int PIN_GRABAR  = 5;   // botón grabar (GP5)
const int PIN_LED     = 6;   // led de grabación (GP6)
// GP0 y GP1 los usa el monitor serial (UART0)

const uint32_t ESPERA_CATEGORIAS_MS = 3000;   // espera entre pulsaciones aceptadas
const uint32_t ESPERA_GRABAR_MS     = 0;      // grabar solo usa el filtro de rebote
const uint32_t MAX_GRABACION_MS     = 60000;  // máximo de un minuto
const uint32_t TIEMPO_REPOSO_MS     = 180000; // 3 minutos sin uso: reposo

const int TOTAL_CUENTOS = 3;
const int TOTAL_MUSICA  = 4;

// ------------------------------------------------------------
// ESTADO DEL PROGRAMA
// ------------------------------------------------------------
// solo hay una categoría activa a la vez
enum class Categoria { NINGUNA, CUENTOS, MUSICA, MENSAJE };

Categoria categoria = Categoria::NINGUNA;

// cada categoría recuerda su posición (0 = primero)
int posCuento = 0;
int posMusica = 0;

// grabación simulada
bool grabando = false;
uint32_t inicioGrabacion = 0;

// solo está en memoria RAM: se pierde al reiniciar la placa
// (la versión real lo guardaría en la SD)
bool mensajeDisponibleSimulado = false;

// reposo por software (la pantalla queda apagada y cualquier botón despierta)
// OJO: es un reposo por software, no corta la alimentación:
// si se cortara, los botones no podrían encender el dispositivo
bool dormido = false;
// última vez que hubo uso (pulsación aceptada, grabación o audio activo)
uint32_t ultimoUso = 0;

// ------------------------------------------------------------
// FRASES PARA LOS MENSAJES
// ------------------------------------------------------------
// modo 0 = origen ("del cuento 2", "de música 1")
// modo 1 = destino ("al cuento 2", "a música 1")
// modo 2 = nombre  ("el cuento 2", "la música 1")
void frase(char* destino, size_t n, Categoria c, int modo) {
  switch (c) {
    case Categoria::CUENTOS:
      snprintf(destino, n, "%s cuento %d",
               modo == 0 ? "del" : (modo == 1 ? "al" : "el"), posCuento + 1);
      break;
    case Categoria::MUSICA:
      if (modo == 2) snprintf(destino, n, "la música %d", posMusica + 1);
      else snprintf(destino, n, "%s música %d", modo == 0 ? "de" : "a", posMusica + 1);
      break;
    case Categoria::MENSAJE:
      snprintf(destino, n, "%s mensaje de la persona cuidadora",
               modo == 0 ? "del" : (modo == 1 ? "al" : "el"));
      break;
    default:
      snprintf(destino, n, "nada");
      break;
  }
}

// ------------------------------------------------------------
// CATEGORÍAS: cuentos y música
// ------------------------------------------------------------
// se llama cuando una pulsación aceptada pide cuentos o música
void cambiarA(Categoria nueva, Pantallita& pantalla) {
  char de[64];
  char a[64];

  if (categoria == Categoria::NINGUNA) {
    // no había nada activo: empieza desde la posición recordada
    categoria = nueva;
    if (nueva == Categoria::CUENTOS) {
      printf("aquí empieza el audio del cuento %d\n", posCuento + 1);
    } else {
      printf("aquí empieza el audio de música %d\n", posMusica + 1);
    }
  } else if (categoria == nueva) {
    // misma categoría: avanzamos al siguiente y volvemos al primero después del último
    frase(de, sizeof(de), categoria, 0);
    if (nueva == Categoria::CUENTOS) posCuento = (posCuento + 1) % TOTAL_CUENTOS;
    else posMusica = (posMusica + 1) % TOTAL_MUSICA;
    frase(a, sizeof(a), categoria, 1);
    printf("se cambia %s %s\n", de, a);
  } else {
    // otra categoría: se muestra de dónde sale y a dónde cambia
    // (la posición de la categoría nueva es la que recordaba)
    frase(de, sizeof(de), categoria, 0);
    categoria = nueva;   // si era el mensaje, deja de estar activo
    frase(a, sizeof(a), categoria, 1);
    printf("se cambia %s %s\n", de, a);
  }

  // la pantalla muestra el símbolo provisorio de la categoría
  // (si estaba el corazón del mensaje, se reemplaza)
  if (nueva == Categoria::CUENTOS) pantalla.mostrarCuentos();
  else pantalla.mostrarMusica();
}

// ------------------------------------------------------------
// MENSAJE DE LA PERSONA CUIDADORA
// ------------------------------------------------------------
void pulsarMensaje(Pantallita& pantalla, uint32_t ahora) {
  if (!mensajeDisponibleSimulado) {
    // no cambia la categoría activa
    printf("todavía no hay un mensaje de la persona cuidadora\n");
    // solo la carita triste, sin texto; la categoría activa sigue como estaba
    pantalla.mostrarSinMensaje(ahora);
    return;
  }

  char de[64];
  if (categoria != Categoria::NINGUNA) {
    frase(de, sizeof(de), categoria, 0);
    printf("se cambia %s al mensaje de la persona cuidadora\n", de);
  }
  categoria = Categoria::MENSAJE;
  printf("ahora empieza el mensaje de la persona cuidadora\n");
  // solo el corazón, sin texto, mientras el mensaje está activo
  pantalla.mostrarMensajeActivo();

  // OJO: el mensaje queda marcado como activo mientras no se cambie de categoría.
  // Falta implementar la detección del final real del audio
  // (ver audioTermino() arriba) para poder escucharlo de nuevo después de que termine.
  // El corazón se queda en pantalla hasta que se elija cuentos o música:
  // no hay un tiempo inventado para quitarlo. Cuando exista audioTermino(),
  // el corazón se quita cuando el audio termine de verdad.
}

// ------------------------------------------------------------
// GRABACIÓN (SIMULADA)
// ------------------------------------------------------------
void iniciarGrabacion(Lucecita& led, Pantallita& pantalla) {
  char nombre[64];

  // si había una categoría activa, se detiene para grabar
  if (categoria != Categoria::NINGUNA) {
    frase(nombre, sizeof(nombre), categoria, 2);
    printf("se detiene %s para grabar\n", nombre);
    categoria = Categoria::NINGUNA;
  }

  grabando = true;
  inicioGrabacion = to_ms_since_boot(get_absolute_time());
  led.prender();
  printf("aquí empieza la grabación del mensaje de la persona cuidadora — simulación\n");
  // "grabando...", el conteo en 0 y las instrucciones
  pantalla.mostrarGrabando();
}

void terminarGrabacion(Lucecita& led, Pantallita& pantalla, bool porMinuto) {
  grabando = false;
  led.apagar();

  if (porMinuto) {
    printf("se llegó a un minuto: hay que soltar el botón y volver a presionar para grabar otra vez\n");
  }
  printf("aquí termina la grabación y se guarda el mensaje — simulación\n");

  // la grabación es simulada: no hay micrófono ni archivo.
  // esta variable solo vive en memoria y se pierde al reiniciar la placa.
  mensajeDisponibleSimulado = true;

  // la pantalla solo dice que termina la simulación (no confirma un archivo guardado)
  // si fue por el minuto, avisa que hay que soltar para poder grabar otra vez
  if (porMinuto) pantalla.mostrarLimiteAlcanzado();
  else pantalla.mostrarGrabacionTerminada();

  // ----------------------------------------------------------
  // GRABACIÓN REAL (declarado, no implementado)
  // ----------------------------------------------------------
  // - La grabación real usa el micrófono MAX4466 y se guarda en la tarjeta SD.
  // - El mensaje anterior solo se reemplaza cuando el nuevo se guarda bien;
  //   si falla la grabación o el guardado, se conserva el anterior.
  // - El mensaje no se borra después de un día ni por tiempo.
  // - La versión física debe detectar si el módulo está unido,
  //   permitir grabar solo cuando está conectado, y cancelar la grabación
  //   si se separa mientras graba, sin reemplazar el mensaje anterior.
  //   bool moduloUnido();
  //   bool guardarGrabacionEnSD();
  //   void cancelarGrabacion();
}

// ------------------------------------------------------------
// PROGRAMA PRINCIPAL
// ------------------------------------------------------------
int main() {
  // monitor serial por UART0 (GP0/GP1)
  stdio_init_all();

  // botones: categorías con 3 segundos de espera, grabar sin espera extra
  BotonGrupo1 botonCuentos(PIN_CUENTOS, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonMusica(PIN_MUSICA, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonMensaje(PIN_MENSAJE, ESPERA_CATEGORIAS_MS);
  BotonGrupo1 botonGrabar(PIN_GRABAR, ESPERA_GRABAR_MS);

  // led de grabación
  Lucecita ledGrabacion(PIN_LED);

  // pantalla: al encender muestra "ilustración"
  Pantallita pantalla;
  pantalla.iniciar();

  // el tiempo sin uso cuenta desde que se enciende
  ultimoUso = to_ms_since_boot(get_absolute_time());

  // true cuando ya se avisó que hay varios botones de categoría apretados
  // se vuelve a false solo cuando se sueltan todos (así se avisa una sola vez)
  bool variosAvisado = false;
  // true cuando se llegó al minuto y todavía no se suelta el botón de grabar
  bool avisoSoltar = false;

  // se imprime una sola vez al iniciar
  printf("esperando una acción\n");

  while (true) {
    uint32_t ahora = to_ms_since_boot(get_absolute_time());

    // 1) grabar se lee primero: si coincide con una solicitud de reproducción, gana grabar
    // en reposo ninguna pulsación se acepta: la primera solo despierta
    botonGrabar.aceptarPulsaciones = !dormido;
    botonGrabar.leer();

    if (!grabando) {
      // solo un flanco aceptado empieza la grabación (mantener apretado no la reinicia)
      if (botonGrabar.pulsacionAceptada) {
        iniciarGrabacion(ledGrabacion, pantalla);
      }
    } else {
      // el conteo usa el mismo tiempo que controla el límite de grabación
      uint32_t transcurrido = ahora - inicioGrabacion;
      if (!botonGrabar.presionado) {
        // al soltar, termina
        terminarGrabacion(ledGrabacion, pantalla, false);
      } else if (transcurrido >= MAX_GRABACION_MS) {
        // llegó al minuto: termina aunque sigan apretando
        // no empieza otra hasta soltar y volver a presionar
        terminarGrabacion(ledGrabacion, pantalla, true);
        avisoSoltar = true;
      } else {
        // 0 antes del primer segundo, 1 al cumplirse el primero, y así hasta 60
        // la pantalla solo dibuja cuando cambia el segundo
        pantalla.actualizarConteo(transcurrido / 1000);
      }
    }

    // cuando sueltan después del minuto, el aviso de soltar ya no corresponde
    // (si ya se mostró otra cosa, por ejemplo el mensaje, no se toca)
    if (avisoSoltar && !botonGrabar.presionado) {
      avisoSoltar = false;
      if (pantalla.vista == VistaPantalla::LIMITE_ALCANZADO) {
        pantalla.mostrarGrabacionTerminada();
      }
    }

    // 2) mientras grabamos, las categorías no aceptan pulsaciones
    // (no se guardan para después ni reinician su espera)
    // el botón de mensaje tampoco acepta mientras el mensaje está activo,
    // así no se reinicia la espera de 3 segundos
    // si hay dos botones de categoría apretados a la vez, ninguno cuenta:
    // cada botón se bloquea mientras OTRO botón de categoría esté apretado
    // (se mira el pin directo, así también se detecta aunque el otro
    // todavía no pase el filtro de rebote de 50 ms)
    // el bloqueo no guarda toques ni empieza la espera de 3 segundos;
    // para que cuente hay que soltar y volver a presionar
    bool cuentosApretado = botonCuentos.crudoPresionado();
    bool musicaApretado  = botonMusica.crudoPresionado();
    bool mensajeApretado = botonMensaje.crudoPresionado();

    // aviso solo por el monitor serial (no aparece en la pantalla)
    // se imprime una vez por cada vez que se juntan 2 o más botones de categoría
    int apretados = (int)cuentosApretado + (int)musicaApretado + (int)mensajeApretado;
    if (apretados >= 2) {
      if (!variosAvisado) {
        printf("se detectaron varios botones; no se ejecuta ninguna acción\n");
        variosAvisado = true;
      }
    } else if (apretados == 0) {
      variosAvisado = false;
    }

    botonCuentos.aceptarPulsaciones = !dormido && !grabando && !musicaApretado && !mensajeApretado;
    botonMusica.aceptarPulsaciones  = !dormido && !grabando && !cuentosApretado && !mensajeApretado;
    botonMensaje.aceptarPulsaciones = !dormido && !grabando && (categoria != Categoria::MENSAJE)
                                      && !cuentosApretado && !musicaApretado;

    // 3) leer siempre los tres botones para mantener su estado actualizado
    botonCuentos.leer();
    botonMusica.leer();
    botonMensaje.leer();

    if (dormido) {
      // 4a) en reposo: cualquier botón despierta y vuelve a la pantalla de inicio
      // esa pulsación solo despierta (ninguna fue aceptada): para hacer
      // otra acción hay que soltar y volver a presionar
      if (botonCuentos.recienPresionado || botonMusica.recienPresionado ||
          botonMensaje.recienPresionado || botonGrabar.recienPresionado) {
        dormido = false;
        ultimoUso = ahora;
        pantalla.mostrarInicio();
        printf("se despierta: esa pulsación solo despierta\n");
      }
    } else {
      // 4b) atender las pulsaciones aceptadas (solo si no estamos grabando)
      if (!grabando) {
        if (botonCuentos.pulsacionAceptada) cambiarA(Categoria::CUENTOS, pantalla);
        if (botonMusica.pulsacionAceptada)  cambiarA(Categoria::MUSICA, pantalla);
        if (botonMensaje.pulsacionAceptada) pulsarMensaje(pantalla, ahora);
      }

      // solo las pulsaciones aceptadas cuentan como uso
      // (los toques ignorados no reinician el tiempo de reposo)
      if (botonCuentos.pulsacionAceptada || botonMusica.pulsacionAceptada ||
          botonMensaje.pulsacionAceptada || botonGrabar.pulsacionAceptada) {
        ultimoUso = ahora;
      }

      // la carita triste se quita sola, sin detener el programa
      pantalla.actualizar(ahora);

      // 5) reposo: no entra mientras graba ni mientras hay un audio activo
      // (el audio real todavía no existe: una categoría activa cuenta como audio activo
      // hasta que audioTermino() esté implementado)
      // el tiempo sin uso se cuenta desde que termina la grabación o el audio
      if (grabando || categoria != Categoria::NINGUNA) {
        ultimoUso = ahora;
      } else if ((ahora - ultimoUso) >= TIEMPO_REPOSO_MS) {
        dormido = true;
        pantalla.apagar();
        printf("reposo: pantalla apagada\n");
      }
    }

    // sin sleep ni esperas: el ciclo sigue leyendo todos los botones
  }

  return 0;
}
