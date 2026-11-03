// Matriz.cpp
// implementaciones de la clase

// importar el archivo header
#include "Matriz.h"

// Esto lo agregamos para leer la velocidad del reloj de la raspberry.
// Hace falta para calcular a qué ritmo manda datos el PIO.
#include "hardware/clocks.h"
// Esto lo agregamos para medir cuántas letras tiene un texto (strlen).
#include <string.h>

// ============================================================

// PROGRAMA DEL PIO
// Estas 4 instrucciones son el programa que le enseña al PIO cómo
// mandar la señal a los LEDs WS2812 con los tiempos exactos que
// ellos piden. Normalmente viven en un archivo .pio, pero como
// Wokwi no lo acepta, las escribimos ya traducidas a números.
// No se modifican.
static const uint16_t ws2812_instrucciones[] = {
  0x6221,
  0x1123,
  0x1400,
  0xa442
};

// Esta ficha le cuenta al PIO dónde está el programa y cuántas
// instrucciones tiene (4). El -1 significa "ponlo donde haya espacio".
static const struct pio_program ws2812_programa = {
  ws2812_instrucciones,
  4,
  -1
};

// ============================================================

// LAS LETRAS
// Cada letra se dibuja con 7 filas de 5 columnas. Aquí solo están
// las letras de "ALTIRO HERO" y "FELICITACIONES". Si más adelante
// quieren otro texto con letras nuevas, hay que agregarlas aquí.
struct Letra {
  char simbolo;
  uint8_t filas[7];
};

static const Letra LETRAS[] = {
  {'A', {0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}},
  {'C', {0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110}},
  {'E', {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b11111}},
  {'F', {0b11111, 0b10000, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000}},
  {'H', {0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}},
  {'I', {0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}},
  {'L', {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111}},
  {'N', {0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b10001}},
  {'O', {0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110}},
  {'R', {0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001}},
  {'S', {0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110}},
  {'T', {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100}}
};

// Cuántas letras hay en la lista de arriba.
static const int CANTIDAD_LETRAS = sizeof(LETRAS) / sizeof(LETRAS[0]);

// Busca una letra en la lista y devuelve sus 7 filas. Si no la
// encuentra (por ejemplo, el espacio), devuelve nada y no se dibuja.
static const uint8_t* buscarLetra(char simbolo) {
  for (int i = 0; i < CANTIDAD_LETRAS; i++) {
    if (LETRAS[i].simbolo == simbolo) {
      return LETRAS[i].filas;
    }
  }
  return nullptr;
}

// LOS NÚMEROS 1, 2 Y 3
// Miden 4 columnas por 6 filas, y no 5 por 7 como las letras, porque
// 4 y 6 caben justo en el centro de una matriz de 8x8 (quedan 2
// columnas y 1 fila libres a cada lado).
static const uint8_t NUMEROS[3][6] = {
  {0b0110, 0b0010, 0b0010, 0b0010, 0b0010, 0b0111},   // 1
  {0b0110, 0b1001, 0b0001, 0b0010, 0b0100, 0b1111},   // 2
  {0b1110, 0b0001, 0b0110, 0b0001, 0b0001, 0b1110}    // 3
};

// ============================================================

// ============================================================

// constructor
Matriz::Matriz(int nuevaPatita) {

  // guardar el valor
  Matriz::patita = nuevaPatita;

  // Escogemos el PIO 0 y su máquina número 0.
  Matriz::pio = pio0;
  Matriz::maquina = 0;

  // Cargamos el programa en el PIO y anotamos dónde quedó guardado.
  int offset = pio_add_program(Matriz::pio, &ws2812_programa);

  // Le decimos al PIO que este pin va a ser controlado por él.
  pio_gpio_init(Matriz::pio, Matriz::patita);

  // Dice que el pin es de SALIDA: la raspberry escribe, no lee.
  pio_sm_set_consecutive_pindirs(Matriz::pio, Matriz::maquina,
                                 Matriz::patita, 1, true);

  // Creamos la configuración de la máquina del PIO.
  pio_sm_config configuracion = pio_get_default_sm_config();

  // Le decimos en qué instrucciones empieza y termina el programa
  // para que lo repita en ciclo (de la 0 a la 3).
  sm_config_set_wrap(&configuracion, offset + 0, offset + 3);

  // El programa usa 1 bit extra para subir y bajar el pin.
  sm_config_set_sideset(&configuracion, 1, false, false);
  sm_config_set_sideset_pins(&configuracion, Matriz::patita);

  // Cada LED recibe 24 bits (8 de verde, 8 de rojo y 8 de azul).
  sm_config_set_out_shift(&configuracion, false, true, 24);

  // Juntamos las dos filas de espera del PIO para mandar más datos
  // seguidos sin que se atasque.
  sm_config_set_fifo_join(&configuracion, PIO_FIFO_JOIN_TX);

  // Los WS2812B piden 800000 bits por segundo, y cada bit gasta
  // 10 ciclos del programa. Con eso calculamos qué tan lento debe
  // ir el PIO respecto al reloj de la raspberry.
  float divisor = clock_get_hz(clk_sys) / (800000.0f * 10.0f);
  sm_config_set_clkdiv(&configuracion, divisor);

  // Dejamos todo cargado y encendemos la máquina del PIO.
  pio_sm_init(Matriz::pio, Matriz::maquina, offset, &configuracion);
  pio_sm_set_enabled(Matriz::pio, Matriz::maquina, true);

  // partir apagada
  Matriz::limpiar();
  Matriz::mostrar();
}

// metodos

void Matriz::limpiar() {

  // Recorremos los 64 LEDs del dibujo y los dejamos en negro (apagados).
  for (int i = 0; i < 64; i++) {
    Matriz::pixeles[i] = NEGRO;
  }
}

void Matriz::pintarPixel(int fila, int columna, uint32_t color) {

  // Si la fila o la columna se salen de la matriz (menores a 0 o
  // mayores a 7), no hacemos nada, para no escribir fuera del dibujo.
  if (fila < 0 || fila > 7 || columna < 0 || columna > 7) {
    return;
  }

  // Guardamos el color en la posición de esa fila y columna.
  Matriz::pixeles[fila * 8 + columna] = color;
}

void Matriz::pintarCamino(int camino, int fila, uint32_t color) {

  // Si el camino no está entre 1 y 4, no hacemos nada.
  if (camino < 1 || camino > 4) {
    return;
  }

  // Cada camino ocupa 2 columnas. El camino 1 empieza en la columna 0,
  // el 2 en la 2, el 3 en la 4 y el 4 en la 6.
  int primeraColumna = (camino - 1) * 2;

  // Pintamos las dos columnas del camino en esa fila.
  Matriz::pintarPixel(fila, primeraColumna, color);
  Matriz::pintarPixel(fila, primeraColumna + 1, color);
}

void Matriz::mostrar() {

  // Recorremos los LEDs en el orden en que están cableados en la
  // tira: el 0 es el primero que recibe la señal, y así hasta el 63.
  for (int i = 0; i < 64; i++) {

    // Calculamos en qué fila y columna del dibujo está este LED.
    int fila = i / 8;
    int columna = i % 8;

    // Si la matriz es serpentina, en las filas impares el orden va
    // al revés, y entonces damos vuelta la columna.
    if (Matriz::serpentina && (fila % 2 == 1)) {
      columna = 7 - columna;
    }

    // Tomamos el color de ese LED desde el dibujo.
    uint32_t color = Matriz::pixeles[fila * 8 + columna];

    // Separamos el color en sus tres partes: rojo, verde y azul.
    uint32_t rojo  = (color >> 16) & 0xFF;
    uint32_t verde = (color >> 8) & 0xFF;
    uint32_t azul  = color & 0xFF;

    // Aplicamos el brillo: con brillo 20, cada color queda al 20%.
    rojo  = rojo  * Matriz::brillo / 100;
    verde = verde * Matriz::brillo / 100;
    azul  = azul  * Matriz::brillo / 100;

    // Los WS2812 piden los colores en orden VERDE, ROJO, AZUL.
    // El corrimiento final de 8 es porque el PIO lee desde la izquierda.
    uint32_t palabra = (verde << 24) | (rojo << 16) | (azul << 8);

    // Enviamos el color al PIO.
    pio_sm_put_blocking(Matriz::pio, Matriz::maquina, palabra);
  }
}

void Matriz::rellenar(uint32_t color) {

  // Recorremos los 64 LEDs del dibujo y los dejamos todos del mismo color.
  for (int i = 0; i < 64; i++) {
    Matriz::pixeles[i] = color;
  }
}

int Matriz::anchoTexto(const char* texto) {

  // Cada letra ocupa 5 columnas más 1 de espacio entre letras: 6.
  return strlen(texto) * 6;
}

void Matriz::dibujarTexto(const char* texto, int desplazamiento, uint32_t color) {

  // Cuántas letras tiene el texto.
  int cantidad = strlen(texto);

  // Recorremos las 8 columnas de la matriz, de izquierda a derecha.
  for (int columna = 0; columna < 8; columna++) {

    // Calculamos qué columna del texto cae en esta columna de la
    // matriz. Con desplazamiento 0 el texto está justo afuera, a la
    // derecha; cada vez que sube, el texto se corre una columna a la
    // izquierda. El -8 hace que arranque desde afuera.
    int columnaTexto = columna + desplazamiento - 8;

    // Si todavía no hay texto en esta columna (o ya pasó), la saltamos.
    if (columnaTexto < 0 || columnaTexto >= cantidad * 6) {
      continue;
    }

    // Cada letra mide 6 columnas: dividimos para saber de cuál letra
    // se trata, y el resto nos dice qué columna de esa letra es.
    int indiceLetra = columnaTexto / 6;
    int columnaLetra = columnaTexto % 6;

    // La columna 5 de cada letra es el espacio entre letras: no se pinta.
    if (columnaLetra == 5) {
      continue;
    }

    // Buscamos el dibujo de esta letra. Si no existe (ej. el espacio),
    // no se pinta nada.
    const uint8_t* filas = buscarLetra(texto[indiceLetra]);
    if (filas == nullptr) {
      continue;
    }

    // Revisamos las 7 filas de la letra. Si el bit de esta columna
    // está en 1, prendemos el LED.
    for (int fila = 0; fila < 7; fila++) {
      if (filas[fila] & (1 << (4 - columnaLetra))) {
        Matriz::pintarPixel(fila, columna, color);
      }
    }
  }
}

void Matriz::dibujarNumero(int numero, uint32_t color) {

  // Solo sabemos dibujar el 1, el 2 y el 3.
  if (numero < 1 || numero > 3) {
    return;
  }

  // Recorremos las 6 filas y las 4 columnas del número.
  for (int fila = 0; fila < 6; fila++) {
    for (int columna = 0; columna < 4; columna++) {

      // Si el bit está en 1, prendemos el LED. Sumamos 1 a la fila y
      // 2 a la columna para dejarlo centrado en la matriz.
      if (NUMEROS[numero - 1][fila] & (1 << (3 - columna))) {
        Matriz::pintarPixel(fila + 1, columna + 2, color);
      }
    }
  }
}

void Matriz::guardarCopia() {

  // Copiamos los 64 LEDs del dibujo actual a la foto.
  for (int i = 0; i < 64; i++) {
    Matriz::copia[i] = Matriz::pixeles[i];
  }
}

void Matriz::dibujarZoom(int paso) {

  // Empezamos con la pantalla limpia.
  Matriz::limpiar();

  // El lado del cuadrado achicado: paso 1 = 6, paso 2 = 4, paso 3 = 2.
  int lado = 8 - 2 * paso;
  if (lado <= 0) {
    return;
  }

  // Dónde empieza el cuadrado para que quede centrado.
  int inicio = (8 - lado) / 2;

  // Para cada LED del cuadrado chico, buscamos qué LED de la foto
  // original le corresponde, escalando por la proporción 8 / lado.
  for (int fila = 0; fila < lado; fila++) {
    for (int columna = 0; columna < lado; columna++) {
      int filaOriginal = fila * 8 / lado;
      int columnaOriginal = columna * 8 / lado;
      Matriz::pixeles[(inicio + fila) * 8 + inicio + columna] =
          Matriz::copia[filaOriginal * 8 + columnaOriginal];
    }
  }
}

void Matriz::fondoColores(int desfase) {

  // La paleta del proyecto, en el orden en que se repiten las franjas.
  const uint32_t paleta[5] = { AZUL_PETROLEO, TURQUESA, MENTA, BLANCO_HUMO, MORADO };

  // Cada LED toma el color según su fila + columna (eso hace las
  // franjas diagonales) más el desfase (eso las hace moverse).
  for (int fila = 0; fila < 8; fila++) {
    for (int columna = 0; columna < 8; columna++) {
      Matriz::pixeles[fila * 8 + columna] = paleta[(fila + columna + desfase) % 5];
    }
  }
}