/*
 * PROYECTO 03 - Juego de nave controlado por sonido
 
 * Hardware : Raspberry Pi Pico + 4 botones + audio estéreo
 *            GP14 = canal izquierdo, GP16 = canal derecho
 *            Físico : PAM8403 + 2 parlantes de 8 ohm
 *            Wokwi  : 2 buzzers en GP14 y GP16
 
 * Reglas del juego:
 *   - 3 sectores: IZQUIERDA, CENTRO, DERECHA. La nave parte en CENTRO.
 *   - Enemigos: se pueden disparar. Atacan al final de su acercamiento.
 *   - Obstáculos (asteroides): NO se disparan, solo se esquivan.
 *   - 3 vidas iniciales (máximo 3). Vida extra cada 100 puntos.
 *   - La dificultad sube con el tiempo de juego (sin contar la pausa).
 */

#ifndef PROYECTO03_H
#define PROYECTO03_H


// ============================================================
// 1. CONSTANTES Y CONFIGURACIÓN  (config.py)
// ============================================================

// --- Pines ---
const PIN_BTN_IZQ     = 2;
const PIN_BTN_DER     = 3;
const PIN_BTN_DISPARO = 4;
const PIN_BTN_START   = 5;
const PIN_AUDIO_L     = 14;   // PWM (slice distinto de R, tonos independientes)
const PIN_AUDIO_R     = 16;   // PWM

// --- Enumeraciones ---
enum Sector       { IZQUIERDA = 0, CENTRO = 1, DERECHA = 2 };
enum Estado       { ESPERA, JUGANDO, PAUSA, GAME_OVER };
enum TipoAmenaza  { ENEMIGO, OBSTACULO };
enum EtapaAmenaza { APARECE, SE_ACERCA, LLEGA, ELIMINADA, FINALIZADA };
enum TipoEvento   { ANUNCIO_ENEMIGO, ACERCAMIENTO_ENEMIGO, ATAQUE_ENEMIGO,
                    ANUNCIO_OBSTACULO, LLEGADA_OBSTACULO };

// --- Reglas de juego ---
const VIDAS_INICIALES            = 3;
const VIDAS_MAXIMAS              = 3;
const PUNTOS_POR_ENEMIGO         = 10;
const PUNTOS_PARA_VIDA_EXTRA     = 100;
const PROBABILIDAD_OBSTACULO     = 25;     // % de que un spawn sea obstáculo
const DEBOUNCE_MS                = 30;

// --- Dificultad ---
const SPAWN_INTERVALO_INICIAL_MS     = 3000;
const SPAWN_INTERVALO_MIN_MS         = 800;
const ACERCAMIENTO_INICIAL_MS        = 2000;  // tiempo desde que aparece hasta que llega
const ACERCAMIENTO_MIN_MS            = 600;
const SUBIR_NIVEL_CADA_MS            = 15000;
const REDUCCION_SPAWN_POR_NIVEL_MS   = 300;
const REDUCCION_ACERCAMIENTO_POR_NIVEL_MS = 150;

// --- Audio ---
const VOLUMEN_DUTY         = 20000;   // de 65535, moderado (parlantes de 0,5 W)
const ALERTA_ULTIMA_VIDA_CADA_MS = 3000;
const PUNTOS_POR_PITIDO_FINAL    = 10;    // 1 pitido de puntaje cada 10 puntos
const MAX_PITIDOS_FINAL          = 30;    // tope para que no dure eternamente

// --- Frecuencias de la tabla de sonidos (Hz) ---
const HZ_ENEMIGO       = 880;
const HZ_ACERCAMIENTO  = 1200;
const HZ_ATAQUE        = 220;
const HZ_OBSTACULO     = 300;
const HZ_DISPARO       = 1500;
const HZ_PUNTAJE       = 1000;
const HZ_ALERTA_VIDA   = 700;


// ============================================================
// 2. RELOJ DE JUEGO  (reloj.py)
//    Reloj que SOLO avanza mientras se está jugando.
//    Así la pausa no cuenta para la dificultad ni para los
//    tiempos de los enemigos (no hay que "compensar" nada).
// ============================================================
class RelojJuego {
public:
    RelojJuego();

    void reiniciar();            // tiempo_juego = 0

    void actualizar(corriendo);  // llamar en cada vuelta del bucle principal
                                 //   ahora = ticks_ms()
                                 //   si corriendo (estado JUGANDO):
                                 //        tiempo_juego += ticks_diff(ahora, ultima_lectura)
                                 //   ultima_lectura = ahora

    int ahoraMs();               // tiempo_juego actual

private:
    tiempo_juego;
    ultima_lectura;
};


// ============================================================
// 3. BOTÓN  (boton.py)
//    Pulsador con antirrebote que detecta "recién presionado".
// ============================================================
class Boton {
public:
    Boton(pin);                  // Pin entrada con PULL_UP interno
                                 // (botón entre GPIO y GND; presionado = 0)

    void actualizar();           // en cada vuelta del bucle:
                                 //   lee el pin
                                 //   si cambió y pasaron DEBOUNCE_MS -> actualiza estado
                                 //   si pasó de suelto a presionado -> bandera = true

    bool fuePresionado();        // true UNA sola vez por pulsación; limpia la bandera

private:
    pin;
    estado_actual;
    estado_anterior;
    ultimo_cambio_ms;
    bandera_presionado;
};


// ============================================================
// 4. MOTOR DE AUDIO  (audio.py)
//    Reproduce tonos SIN bloquear el juego (nada de sleep).
//    Cada canal (L, R) tiene su propia cola de notas.
//    Usa el tiempo real (ticks_ms), NO el reloj de juego.
// ============================================================
struct Nota {
    frecuencia_hz;               // 0 = silencio (pausa entre notas)
    duracion_ms;
};

class MotorAudio {
public:
    MotorAudio(pin_l, pin_r);    // crea un PWM por canal con duty = 0

    void reproducir(sector, lista_de_notas);
                                 // IZQUIERDA -> cola del canal L
                                 // DERECHA   -> cola del canal R
                                 // CENTRO    -> misma secuencia en L y R
                                 // Reemplaza lo que estuviera sonando en ese canal.

    void actualizar();           // en cada vuelta del bucle, por cada canal:
                                 //   si no hay nota o la actual ya terminó
                                 //      (ticks_diff(ahora, inicio) >= duracion):
                                 //         tomar la siguiente nota de la cola
                                 //         si la cola quedó vacía -> duty = 0
                                 //         si frecuencia == 0     -> duty = 0 (silencio)
                                 //         si no -> freq(f), duty = VOLUMEN_DUTY
                                 //         inicio = ticks_ms()

    void silenciar();            // vacía colas y apaga ambos canales
    bool estaSonando();          // true si algún canal tiene notas pendientes

    // ---------------------------------------------------------
    // BIBLIOTECA DE SONIDOS (misma numeración que la tabla del README)
    // Cada función devuelve una lista de Notas. [f, ms] = una nota.
    // ---------------------------------------------------------

    static sonidoInicio();           // 1  : [440,80] [660,80] [880,80]
    static sonidoEnemigo();          // 2  : [880,120]
    static sonidoAcercamiento();     // 3  : [1200,50] [0,50] [1200,50]
    static sonidoAtaque();           // 4  : [220,400]
    static sonidoObstaculo();        // 5  : 4 pulsos: [300,60] [0,60] x4
    static sonidoDisparo();          // 6  : [1500,30]
    static sonidoExplosion();        // 7  : [600,40] [500,40] [400,40] [300,40] [200,40]
    static sonidoImpacto();          // 8  : [300,30] [200,30] [300,30] [200,30]
    static sonidoUltimaVida();       // 9  : [700,100] [0,100] x3
    static sonidoRecompensa();       // 10 : [523,90] [659,90] [784,90] [1047,90]
    static sonidoDerrota();          // 11 : [400,250] [350,250] [300,250] [200,250]
    static sonidoPuntaje(puntos);    // 12 : 1 pitido [1000,100] + [0,100]
                                     //      por cada 10 puntos, tope MAX_PITIDOS_FINAL.
                                     //      Cada 5 pitidos agregar un silencio extra
                                     //      [0,300] para poder contarlos.
    static sonidoPausa();            // 13 : [600,100] [400,100]

private:
    pwm_l;
    pwm_r;
    cola_l;
    cola_r;
    nota_actual_l;
    nota_actual_r;
    inicio_nota_l_ms;
    inicio_nota_r_ms;
};


// ============================================================
// 5. NAVE  (nave.py)
//    Posición, vidas y puntaje del jugador.
// ============================================================
class Nave {
public:
    Nave();                      // posición = CENTRO, vidas = VIDAS_INICIALES, puntos = 0

    void moverIzquierda();       // DERECHA -> CENTRO -> IZQUIERDA (se detiene en el borde)
    void moverDerecha();         // IZQUIERDA -> CENTRO -> DERECHA
    Sector posicion();

    void recibirDano();          // vidas -= 1
    bool estaEnUltimaVida();     // vidas == 1
    bool estaDestruida();        // vidas == 0
    int  vidas();

    bool sumarPuntos(cantidad);  // puntos += cantidad
                                 // si se cruzó un múltiplo de PUNTOS_PARA_VIDA_EXTRA:
                                 //     si vidas < VIDAS_MAXIMAS -> vidas += 1
                                 //     return true  (hubo premio -> sonar recompensa)
                                 // si no: return false

    int puntaje();
    void reiniciar();            // posición, vidas, puntos y siguiente_premio a valores iniciales

private:
    sector_actual;
    vidas_restantes;
    puntos;
    siguiente_premio_en;         // empieza en PUNTOS_PARA_VIDA_EXTRA, sube de 100 en 100
};


// ============================================================
// 6. AMENAZA, ENEMIGO Y OBSTÁCULO  (amenaza.py)
//    Clase base + 2 hijas. Ciclo de vida:
//      APARECE -> SE_ACERCA -> LLEGA -> FINALIZADA
//                       \-> ELIMINADA (solo enemigos, si los disparan antes de llegar)
// ============================================================
class Amenaza {
public:
    Amenaza(sector, tiempo_acercamiento_ms, ahora_ms);

    EtapaAmenaza actualizar(ahora_ms);
                                 // t = ahora_ms - tiempo_creacion
                                 //   t < 40% del acercamiento -> APARECE
                                 //   t < 100%                 -> SE_ACERCA
                                 //   t >= 100%                -> LLEGA (una sola vez)
                                 //                               y luego FINALIZADA

    virtual bool esDisparable() = 0;
    virtual TipoAmenaza tipo()  = 0;

    void eliminar();             // etapa = ELIMINADA
    Sector sector();
    EtapaAmenaza etapa();
    bool anuncioEmitido();       // para no repetir el sonido de aparición
    bool avisoEmitido();         // para no repetir el aviso de acercamiento
    void marcarAnuncio();
    void marcarAviso();

protected:
    sector_asignado;
    tiempo_creacion_ms;
    duracion_acercamiento_ms;
    etapa_actual;
    anuncio_hecho;
    aviso_hecho;
};

class Enemigo : public Amenaza {
public:
    bool esDisparable() { return true; }
    TipoAmenaza tipo()  { return ENEMIGO; }
    // Eventos que genera:
    //   APARECE   -> ANUNCIO_ENEMIGO        (sonido 2)
    //   SE_ACERCA -> ACERCAMIENTO_ENEMIGO   (sonido 3)
    //   LLEGA     -> ATAQUE_ENEMIGO         (sonido 4)
};

class Obstaculo : public Amenaza {
public:
    bool esDisparable() { return false; }
    TipoAmenaza tipo()  { return OBSTACULO; }
    // Eventos que genera:
    //   APARECE   -> ANUNCIO_OBSTACULO      (sonido 5)
    //   SE_ACERCA -> (sin aviso adicional)
    //   LLEGA     -> LLEGADA_OBSTACULO      (daño si la nave está en ese sector)
};


// ============================================================
// 7. DIFICULTAD  (dificultad.py)
//    Controla qué tan rápido aparecen y llegan las amenazas.
// ============================================================
class Dificultad {
public:
    Dificultad();

    bool actualizar(tiempo_juego_ms);
                                 // nivel = tiempo_juego_ms / SUBIR_NIVEL_CADA_MS
                                 // intervalo_spawn = max(SPAWN_INTERVALO_MIN_MS,
                                 //        SPAWN_INTERVALO_INICIAL_MS - nivel * REDUCCION_SPAWN_POR_NIVEL_MS)
                                 // acercamiento   = max(ACERCAMIENTO_MIN_MS,
                                 //        ACERCAMIENTO_INICIAL_MS - nivel * REDUCCION_ACERCAMIENTO_POR_NIVEL_MS)
                                 // return true si subió de nivel (por si se quiere avisar)

    int intervaloSpawnMs();
    int tiempoAcercamientoMs();
    int nivel();
    void reiniciar();

private:
    nivel_actual;
    intervalo_spawn_ms;
    tiempo_acercamiento_ms;
};


// ============================================================
// 8. GESTOR DE AMENAZAS  (gestor_amenazas.py)
//    Crea enemigos y obstáculos al azar y mantiene la lista activa.
// ============================================================
struct Evento {
    TipoEvento tipo;
    Sector     sector;
};

class GestorAmenazas {
public:
    GestorAmenazas();

    lista_de_Eventos actualizar(ahora_ms, dificultad);
                                 // 1. SPAWN: si pasó intervaloSpawnMs desde el último:
                                 //      sector = aleatorio entre IZQUIERDA, CENTRO, DERECHA
                                 //      si aleatorio(0..99) < PROBABILIDAD_OBSTACULO -> Obstaculo
                                 //      si no -> Enemigo
                                 //      agregar a la lista (con dificultad.tiempoAcercamientoMs())
                                 // 2. Por cada amenaza activa: etapa = amenaza.actualizar(ahora_ms)
                                 //      generar Evento según tipo y etapa (ver clases hijas)
                                 //      respetando anuncioEmitido / avisoEmitido
                                 // 3. Quitar de la lista las FINALIZADA y ELIMINADA
                                 // 4. Devolver la lista de eventos de esta vuelta

    bool dispararA(sector);      // busca la amenaza DISPARABLE más antigua en ese sector
                                 // cuya etapa sea APARECE o SE_ACERCA:
                                 //   si existe -> eliminar(), return true
                                 //   si no (vacío u obstáculo) -> return false

    void reiniciar();            // vacía la lista y el último spawn

private:
    lista_amenazas;
    ultimo_spawn_ms;
};


// ============================================================
// 9. JUEGO  (juego.py)  -- CLASE PRINCIPAL
//    Máquina de estados que coordina todas las demás clases.
//
//    ESPERA --start--> JUGANDO <--start--> PAUSA
//                         |
//                   sin vidas
//                         v
//                     GAME_OVER --start--> JUGANDO (nueva partida)
// ============================================================
class Juego {
public:
    Juego();                     // crea: 4 Botones, MotorAudio, RelojJuego, Nave,
                                 //       Dificultad, GestorAmenazas
                                 // estado = ESPERA

    void ejecutar();             // bucle principal:
                                 //   mientras true:
                                 //     actualizar los 4 botones
                                 //     reloj.actualizar(estado == JUGANDO)
                                 //     audio.actualizar()
                                 //     según estado:
                                 //        ESPERA    -> manejarEspera()
                                 //        JUGANDO   -> manejarJuego()
                                 //        PAUSA     -> manejarPausa()
                                 //        GAME_OVER -> manejarGameOver()
                                 //     sleep_ms(5)   // respiro corto, NO bloquea el audio

private:
    // ---------- Estados ----------
    void manejarEspera();
                                 // si boton_start.fuePresionado(): iniciarPartida()

    void manejarJuego();
                                 // 1. ENTRADAS
                                 //    boton_izq     -> nave.moverIzquierda()
                                 //    boton_der     -> nave.moverDerecha()
                                 //    boton_disparo -> procesarDisparo()
                                 //    boton_start   -> estado = PAUSA; audio.reproducir(CENTRO, sonidoPausa)
                                 //                     (y salir de esta función)
                                 // 2. ACTUALIZAR MUNDO
                                 //    ahora = reloj.ahoraMs()
                                 //    dificultad.actualizar(ahora)
                                 //    eventos = gestor.actualizar(ahora, dificultad)
                                 // 3. PROCESAR EVENTOS
                                 //    ANUNCIO_ENEMIGO(s)      -> audio.reproducir(s, sonidoEnemigo)
                                 //    ACERCAMIENTO_ENEMIGO(s) -> audio.reproducir(s, sonidoAcercamiento)
                                 //    ATAQUE_ENEMIGO(s)       -> procesarLlegada(s, sonidoAtaque)
                                 //    ANUNCIO_OBSTACULO(s)    -> audio.reproducir(s, sonidoObstaculo)
                                 //    LLEGADA_OBSTACULO(s)    -> procesarLlegada(s, ninguno)
                                 // 4. ALERTA ÚLTIMA VIDA
                                 //    si nave.estaEnUltimaVida() y pasó
                                 //       ALERTA_ULTIMA_VIDA_CADA_MS desde la última alerta:
                                 //         audio.reproducir(CENTRO, sonidoUltimaVida)
                                 // 5. ¿FIN?
                                 //    si nave.estaDestruida(): terminarPartida()

    void manejarPausa();
                                 // si boton_start.fuePresionado():
                                 //     estado = JUGANDO
                                 //     audio.reproducir(CENTRO, sonidoPausa)
                                 // (el reloj de juego no avanzó durante la pausa)

    void manejarGameOver();
                                 // si boton_start.fuePresionado(): iniciarPartida()
                                 // (permite saltarse el sonido del puntaje)

    // ---------- Acciones ----------
    void iniciarPartida();
                                 // nave.reiniciar(); dificultad.reiniciar()
                                 // gestor.reiniciar(); reloj.reiniciar()
                                 // enemigos_eliminados = 0; ultima_alerta_ms = 0
                                 // audio.reproducir(CENTRO, sonidoInicio)
                                 // estado = JUGANDO

    void procesarDisparo();
                                 // sector = nave.posicion()
                                 // audio.reproducir(sector, sonidoDisparo)   // también delata
                                 //                                           // dónde está la nave
                                 // si gestor.dispararA(sector):
                                 //     audio.reproducir(sector, sonidoExplosion)
                                 //     enemigos_eliminados += 1
                                 //     si nave.sumarPuntos(PUNTOS_POR_ENEMIGO):
                                 //         audio.reproducir(CENTRO, sonidoRecompensa)
                                 //         (la recompensa pisa a la explosión: es lo importante)

    void procesarLlegada(sector, sonido_o_ninguno);
                                 // si hay sonido: audio.reproducir(sector, sonido)  // ataque
                                 // si nave.posicion() == sector:
                                 //     nave.recibirDano()
                                 //     audio.reproducir(CENTRO, sonidoImpacto)
                                 // (si la nave estaba en otro sector, esquivó)

    void terminarPartida();
                                 // lista = sonidoDerrota() + [0,500] + sonidoPuntaje(nave.puntaje())
                                 // audio.reproducir(CENTRO, lista)
                                 // estado = GAME_OVER

    // ---------- Datos ----------
    estado;
    boton_izq, boton_der, boton_disparo, boton_start;
    audio;
    reloj;
    nave;
    dificultad;
    gestor;
    ultima_alerta_ms;            // tiempo real, no de juego
    enemigos_eliminados;
};


// ============================================================
// 10. PUNTO DE ENTRADA  (main.py)
// ============================================================
//   juego = Juego()
//   juego.ejecutar()


// ============================================================
// ESTRUCTURA SUGERIDA DE ARCHIVOS (MicroPython / Wokwi)
// ============================================================
//   main.py            -> punto de entrada
//   config.py          -> constantes y enumeraciones
//   reloj.py           -> RelojJuego
//   boton.py           -> Boton
//   audio.py           -> MotorAudio + Nota + sonidos
//   nave.py            -> Nave
//   amenaza.py         -> Amenaza, Enemigo, Obstaculo
//   dificultad.py      -> Dificultad
//   gestor_amenazas.py -> GestorAmenazas
//   juego.py           -> Juego
//
// ============================================================

#endif // PROYECTO03_H
