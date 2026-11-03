// Juego.cpp
// implementacion del minijuego (modo 03)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Juego.h"

// constantes de juego

static const int   SUELO_Y         = 56;      // fila (y) donde esta la linea del suelo
static const int   PERSONAJE_X     = 12;      // columna del borde izquierdo del personaje (no se mueve)
static const int   PERSONAJE_TAM   = 8;       // el personaje es un cuadrado de 8 x 8 px
static const int   OBSTACULO_ANCHO = 6;       // ancho de cada obstaculo

// altura de los obstaculos: entre 8 y 11 px (8 + 0..3)
static const int   OBSTACULO_ALTO_MIN = 8;
static const int   OBSTACULO_ALTO_VAR = 4;

// fisica del salto: impulso 170 px/s y gravedad 700 px/s^2
// -> sube ~20 px como maximo y todo el salto dura ~0.5 s.

// IMPORTANTE (no bajar el impulso ni subir la gravedad sin revisar esto):
// para pasar un obstaculo hay que estar arriba de (alto - 1) px durante todo el rato que
// el obstaculo cruza al personaje. Ese rato dura (12 px) / velocidad: a 50 px/s son 0.24 s.
// Con el salto anterior (impulso 140, gravedad 900) el personaje solo podia estar arriba
// ~0.16 s, asi que en las dificultades lentas (calma y tristeza) era IMPOSIBLE pasar.
// Con estos valores sobran >= 0.1 s en todas las velocidades y alturas.

static const float GRAVEDAD        = 700.0f;  // px/s^2 (hacia abajo)
static const float IMPULSO         = 170.0f;  // px/s hacia arriba al saltar

// una posicion por dificultad: 0 calma, 1 tristeza, 2 felicidad, 3 enojo
static const int VELOCIDADES[4]  = { 50, 65, 85, 120 };       // px/s (mas alto = mas rapido)
static const int FRECUENCIAS[4]  = { 2400, 1900, 1500, 1000 };// ms entre obstaculos (mas bajo = mas obstaculos)

// numeros del 0 al 9 en una grilla de 3 columnas x 5 filas.
// cada fila es un numero de 3 bits: 7 = 111 (tres pixeles), 5 = 101, 2 = 010, etc.
static const uint8_t DIGITOS[10][5] = {
  {7,5,5,5,7}, {2,6,2,2,7}, {7,1,7,4,7}, {7,1,7,1,7}, {5,5,7,1,1},
  {7,4,7,1,7}, {7,4,7,5,7}, {7,1,1,1,1}, {7,5,7,5,7}, {7,5,7,1,7}
};

// hora actual en milisegundos desde que se encendio la Pico
static uint32_t ahora() {
  return to_ms_since_boot(get_absolute_time());
}


///////////////
// CLASE JUEGO
//////////////

Juego::Juego() {
  // todos los obstaculos empiezan "muertos" (fuera de pantalla)
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    Juego::obstaculoX[i] = 0;
    Juego::obstaculoAlto[i] = 0;
    Juego::obstaculoVivo[i] = false;
  }
  Juego::limpiar();   // buffer de pantalla en blanco
}

// prepara y empieza una partida nueva
void Juego::iniciar() {
  Juego::reiniciar();
}

// deja todo en cero y empieza a jugar
void Juego::reiniciar() {
  Juego::personajeY = 0;
  Juego::velocidadY = 0;
  Juego::puntuacion = 0;
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    Juego::obstaculoVivo[i] = false;
  }
  Juego::msParaNuevoObstaculo = 1000;   // el primer obstaculo sale al segundo
  Juego::ultimoTiempo = ahora();        // desde aqui se cuenta el tiempo
  Juego::activo = true;
}

// fija la velocidad y la frecuencia segun la dificultad (0 a 3)
void Juego::configurarDificultad(int nuevaDificultad) {
  if (nuevaDificultad < 0) nuevaDificultad = 0;
  if (nuevaDificultad > 3) nuevaDificultad = 3;
  Juego::dificultad = nuevaDificultad;
  Juego::velocidadObstaculos = VELOCIDADES[nuevaDificultad];
  Juego::frecuenciaObstaculos = FRECUENCIAS[nuevaDificultad];
}

void Juego::saltar() {
  // solo se puede saltar si la partida esta en curso y el personaje esta tocando el suelo
  // (asi no hay saltos dobles en el aire)
  if (Juego::activo && Juego::personajeY <= 0.01f) {
    Juego::velocidadY = IMPULSO;
    Juego::personajeY = 0.1f;   // lo despegamos un poquito del suelo para que parta la fisica
  }
}

// se llama en cada vuelta del main: mueve todo segun el tiempo que paso
void Juego::actualizar() {
  uint32_t t = ahora();
  uint32_t dtMs = t - Juego::ultimoTiempo;   // ms desde la ultima actualizacion

  if (dtMs == 0) return; // si no paso ni 1 ms no hay nada que calcular

  Juego::ultimoTiempo = t;
  if (dtMs > 50) dtMs = 50;   // tope: si hubo un atraso largo, el juego no "pega un salto" de golpe

  if (!Juego::activo) {
    return;   // partida terminada: no se mueve nada
  }

  float dt = dtMs / 1000.0f;   // lo mismo, pero en segundos

  // fisica del salto: la gravedad frena la subida y despues lo hace caer
  if (Juego::personajeY > 0 || Juego::velocidadY > 0) {
    Juego::velocidadY -= GRAVEDAD * dt;
    Juego::personajeY += Juego::velocidadY * dt;
    if (Juego::personajeY <= 0) {   // volvio al suelo
      Juego::personajeY = 0;
      Juego::velocidadY = 0;
    }
  }

  // mover los obstaculos hacia la izquierda
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (!Juego::obstaculoVivo[i]) continue;
    Juego::obstaculoX[i] -= Juego::velocidadObstaculos * dt;
    // si ya paso por completo al personaje: desaparece y suma un punto
    if (Juego::obstaculoX[i] + OBSTACULO_ANCHO < PERSONAJE_X) {
      Juego::obstaculoVivo[i] = false;
      Juego::puntuacion++;
    }
  }

  // crear obstaculos nuevos cuando termina la cuenta regresiva
  Juego::msParaNuevoObstaculo -= (int)dtMs;
  if (Juego::msParaNuevoObstaculo <= 0) {
    // buscar un espacio libre en el arreglo y usarlo
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
      if (!Juego::obstaculoVivo[i]) {
        Juego::obstaculoVivo[i] = true;
        Juego::obstaculoX[i] = PANTALLA_ANCHO;   // entra por el borde derecho
        Juego::obstaculoAlto[i] = OBSTACULO_ALTO_MIN + rand() % OBSTACULO_ALTO_VAR;
        break;
      }
    }
    // proxima espera: la frecuencia base + un poco al azar para que no sea tan predecible
    Juego::msParaNuevoObstaculo =
        Juego::frecuenciaObstaculos + rand() % (Juego::frecuenciaObstaculos / 2 + 1);
  }

  // revisar si choco
  if (Juego::detectarColision()) {
    Juego::activo = false;
    printf("FIN DE LA PARTIDA - puntos: %d\n", Juego::puntuacion);
  }
}

// true si el personaje esta tocando algun obstaculo
bool Juego::detectarColision() {
  // la "caja" del personaje es un poco mas angosta que el dibujo (1 px por lado),
  // para que los roces de pixel no cuenten como choque
  int px1 = PERSONAJE_X + 1;
  int px2 = PERSONAJE_X + PERSONAJE_TAM - 1;

  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (!Juego::obstaculoVivo[i]) continue;
    int ox1 = (int)Juego::obstaculoX[i];   // borde izquierdo del obstaculo
    int ox2 = ox1 + OBSTACULO_ANCHO;       // borde derecho del obstaculo

    // se cruzan en horizontal cuando las dos "cajas" se solapan en X
    bool seCruzanEnX = (px1 < ox2) && (px2 > ox1);
    // los pies del personaje estan mas abajo que el tope del obstaculo
    bool pieAbajoDelTope = Juego::personajeY < (Juego::obstaculoAlto[i] - 1);

    if (seCruzanEnX && pieAbajoDelTope) {
      return true;
    }
  }
  return false;
}

// DIBUJO

// apaga todos los pixeles del buffer
void Juego::limpiar() {
  memset(Juego::buffer, 0, sizeof(Juego::buffer));
}

// enciende (o apaga) un pixel. Lo que quede fuera de la pantalla se ignora.
void Juego::pixel(int x, int y, bool encendido) {
  if (x < 0 || x >= PANTALLA_ANCHO || y < 0 || y >= PANTALLA_ALTO) return;
  // la pantalla se organiza en paginas de 8 filas: cada byte guarda 8 pixeles en vertical
  int indice = x + (y / 8) * PANTALLA_ANCHO;
  if (encendido) {
    Juego::buffer[indice] |= (1 << (y % 8));
  }
  else {
    Juego::buffer[indice] &= ~(1 << (y % 8));
  }
}

// dibuja un rectangulo con la esquina superior izquierda en (x, y)
// relleno = true: lleno; relleno = false: solo el contorno
void Juego::rect(int x, int y, int w, int h, bool relleno) {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      bool borde = (i == 0 || j == 0 || i == w - 1 || j == h - 1);
      if (relleno || borde) {
        Juego::pixel(x + i, y + j);
      }
    }
  }
}

// escribe un numero con digitos de 3x5 pixeles; "escala" los agranda (1 = tamano original)
void Juego::numero(int x, int y, int n, int escala) {
  char texto[12];
  int largo = 0;
  if (n < 0) n = 0;
  // separar el numero en cifras (quedan al reves: de la ultima a la primera)
  do {
    texto[largo++] = '0' + (n % 10);
    n /= 10;
  } while (n > 0 && largo < 11);

  for (int k = 0; k < largo; k++) {
    int d = texto[largo - 1 - k] - '0';   // cifra k, ya en el orden correcto
    int baseX = x + k * 4 * escala;       // cada cifra ocupa 3 columnas + 1 de espacio
    for (int fila = 0; fila < 5; fila++) {
      for (int col = 0; col < 3; col++) {
        // el bit (2 - col) de la fila dice si ese pixel va encendido
        if (DIGITOS[d][fila] & (1 << (2 - col))) {
          Juego::rect(baseX + col * escala, y + fila * escala, escala, escala);
        }
      }
    }
  }
}

// arma el cuadro completo y lo manda a la pantalla
void Juego::dibujar() {
  Juego::limpiar();

  // suelo
  Juego::rect(0, SUELO_Y, PANTALLA_ANCHO, 1);

  // personaje: un cuadrado que sube segun personajeY
  float py = Juego::personajeY;
  if (py < 0) py = 0;
  int topeY = SUELO_Y - (int)py - PERSONAJE_TAM;   // el eje Y de la pantalla crece hacia abajo
  Juego::rect(PERSONAJE_X, topeY, PERSONAJE_TAM, PERSONAJE_TAM);
  Juego::pixel(PERSONAJE_X + 5, topeY + 2, false);   // un "ojo" apagado

  // obstaculos
  for (int i = 0; i < MAX_OBSTACULOS; i++) {
    if (Juego::obstaculoVivo[i]) {
      int h = Juego::obstaculoAlto[i];
      Juego::rect((int)Juego::obstaculoX[i], SUELO_Y - h, OBSTACULO_ANCHO, h);
    }
  }

  if (Juego::activo) {
    // jugando: puntaje chiquito arriba a la derecha
    Juego::numero(PANTALLA_ANCHO - 20, 2, Juego::puntuacion, 1);
  }
  else {
    // fin de la partida: marco en toda la pantalla y puntaje grande al centro
    Juego::rect(0, 0, PANTALLA_ANCHO, PANTALLA_ALTO, false);
    int cifras = 1;
    for (int n = Juego::puntuacion; n >= 10; n /= 10) cifras++;
    int ancho = cifras * 4 * 3 - 3;   // ancho total del numero con escala 3
    Juego::numero((PANTALLA_ANCHO - ancho) / 2, 8, Juego::puntuacion, 3);
  }

  // mandar el dibujo a la pantalla (si ya hay un driver conectado)
  if (Juego::enviarPantalla != nullptr) {
    Juego::enviarPantalla(Juego::buffer);
  }
}


// =====================================================
// FUNCIONES PARA EL MAIN
// =====================================================

Juego juego;   // la unica partida del programa (declarada como extern en Juego.h)

// al entrar al modo 03: dificultad de 0 (calma) a 3 (enojo). Empieza la partida.
void entrarJuego(int dificultad) {
  srand(to_ms_since_boot(get_absolute_time()));   // semilla distinta cada vez
  juego.configurarDificultad(dificultad);
  juego.iniciar();
}

// en cada vuelta del main. botonSaltoPresionado = true solo en la vuelta en que se presiono el boton 02.
void actualizarJuego(bool botonSaltoPresionado) {
  if (botonSaltoPresionado) {
    if (juego.activo) {
      juego.saltar();       // partida en curso: salta
    }
    else {
      juego.reiniciar();    // partida terminada: el mismo boton empieza otra
    }
  }
  juego.actualizar();
}

// en cada vuelta del main: dibuja el cuadro actual
void dibujarJuego() {
  juego.dibujar();
}

// al salir del modo 03 o apagar la maquina
void salirJuego() {
  juego.activo = false;
}