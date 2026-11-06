// DORE 8 - Programa principal, versión física SIN AUDIO.
// Raspberry Pi Pico 2 W / RP2350, ST7789V, C++17 y Pico SDK (sin Arduino).
// Se han extraído las clases a .h/.cpp sin modificar las reglas del juego.

#include "AdafruitGFXPico.h"
#include "PantallaUI.h"
#include "Perilla.h"
#include "Boton2.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>


// ========================================================================
// Programa principal - DORE 8 sin audio
// ========================================================================

// DORE 8 - prototipo integrado, Raspberry Pi Pico 2 W + Pico SDK C++.
// SIN ARDUINO. Mantiene el diagram.json que ya funciona en Wokwi.
// Textos, personajes, laberinto y escenarios: PantallaUI original adaptada al montaje físico.

// 13 · CONFIGURACIÓN GENERAL DEL JUEGO
// Las constantes indican GPIO, no posición física de la placa.
// No se asigna salida PWM a GP13 porque esta versión no incluye audio.
namespace {
constexpr unsigned int PIN_PERILLA_IZQUIERDA = 26; // Segun textos: VERTICAL.
constexpr unsigned int PIN_PERILLA_DERECHA   = 27; // Segun textos: HORIZONTAL.
constexpr unsigned int PIN_BOTON_MUSICA = 10;
constexpr unsigned int PIN_BOTON_AVANZAR = 11;
// GP13 reservado: sin parlante ni buzzer en esta version.
constexpr unsigned int PIN_LED = 7;

// Laberinto: 20 s. Carretera: 15 s para esta versión de prueba.
// Temporizadores de cada desafío: 20 segundos para el laberinto y 15 para la micro.
constexpr int TIEMPO_LABERINTO = 20;
constexpr int TIEMPO_CARRETERA = 15;
// Intervalos mínimos entre actualizaciones para evitar redibujar más veces de lo necesario.
constexpr uint32_t INTERVALO_DIBUJO_MS = 85;
constexpr uint32_t INTERVALO_CARRETERA_MS = 65; // Carretera mas fluida.
// La micro avanza con pasos mayores para dar una sensación de desplazamiento más rápida.
constexpr int PASO_MICRO = 9;  // Velocidad especial del autobus.
constexpr int PASO_MICRO_VERTICAL = 9;

// Cada nivel puede estar en curso, superado o fallido; solo Superado habilita el avance.
enum class Estado { EnCurso, Superado, Fallido };
// Cada vehículo guarda posición, velocidad horizontal y color de la carrocería.
struct Vehiculo {
    int x, y, paso;
    uint16_t color;
};

// Se crea el controlador TFT utilizando CS=GP17 y DC=GP16 (RST por defecto GP20).
AdafruitGFX_Pico tft(17,16); // Mismos pines de la pantalla actual (GP17/GP16).
PantallaUI pantalla(tft);
// Se instancian las perillas y los botones con los GPIO del montaje físico.
Perilla perillaVertical(PIN_PERILLA_IZQUIERDA);
Perilla perillaHorizontal(PIN_PERILLA_DERECHA);
Boton2 botonMusica(PIN_BOTON_MUSICA);
Boton2 botonAvanzar(PIN_BOTON_AVANZAR);
// Audio retirado temporalmente por seguridad del parlante 8 ohm / 1W.

int slideActual = PantallaUI::SLIDE_INICIO;
Estado estado = Estado::EnCurso;
uint32_t inicioNivelMs = 0;
uint32_t ultimoDibujoMs = 0;
int segundosPrevios = -1;
int jugadorX = 18;
int jugadorY = 32;
// Posiciones iniciales y velocidades de los tres automóviles enemigos.
Vehiculo autos[] = {
    {238, PantallaUI::yCarril(0), 3, Colores::ROJO},
    {305, PantallaUI::yCarril(1), 4, Colores::CIAN},
    {192, PantallaUI::yCarril(2), 2, Colores::NARANJA}
};

// 14 · FUNCIONES DE APOYO DEL JUEGO
// Diferencia pantallas informativas de las tres etapas jugables.
bool esNivel(int slide) {
    return slide == PantallaUI::SLIDE_LABERINTO ||
           slide == PantallaUI::SLIDE_CARRETERA ||
           slide == PantallaUI::SLIDE_SALA;
}
// Devuelve el límite de tiempo del nivel actual; la sala no tiene temporizador.
int limiteActual() {
    return slideActual == PantallaUI::SLIDE_LABERINTO ? TIEMPO_LABERINTO :
           slideActual == PantallaUI::SLIDE_CARRETERA ? TIEMPO_CARRETERA : 0;
}
// Obtiene milisegundos desde el arranque para mover objetos y contar segundos.
uint32_t relojMs() {
    return to_ms_since_boot(get_absolute_time());
}
// Detecta superposición de rectángulos (colisión AABB) entre personajes, autos y metas.
bool interseccion(int x0,int y0,int w0,int h0,int x1,int y1,int w1,int h1) {
    return x0 < x1+w1 && x0+w0 > x1 && y0 < y1+h1 && y0+h0 > y1;
}

// El mensaje aparece cuando la etapa se gana o se pierde. El avance SOLO
// ocurre si el estado es Superado; si pierde, el mismo boton reinicia nivel.
// 15 · RESULTADOS DE CADA ETAPA
// Si se ganó, avisa que se puede continuar; si se perdió, avisa que se puede reintentar.
void cambiarEstado(Estado nuevo) {
    if (estado != Estado::EnCurso) return;
    estado = nuevo;
    if (nuevo == Estado::Superado) {
        pantalla.mensajeEstado("LOGRADO! BOTON 2: CONTINUAR", Colores::VERDE);
        // Efecto de victoria desactivado (sin audio).
    } else if(nuevo == Estado::Fallido) {
        pantalla.mensajeEstado("INTENTA OTRA VEZ - BOTON 2", Colores::ROJO);
        // Efecto de error desactivado (sin audio).
    }
}

// La meta está al extremo DERECHO de la carretera. La franja amarilla
// se repinta porque los vehículos que pasan pueden taparla parcialmente.
// 16 · META DE LA CARRETERA
// Dibuja una franja amarilla con marcas negras al extremo derecho del camino.
void dibujarMetaCarretera(bool incluirTexto) {
    tft.fillRect(303, 26, 6, 204, Colores::AMARILLO);
    // Cuadrados negros para que la línea de llegada sea reconocible.
    for (int y = 26; y < 226; y += 32)
        tft.fillRect(303, y, 6, 12, Colores::NEGRO);
    if (incluirTexto) {
        tft.setTextSize(1);
        tft.setTextColor(Colores::AMARILLO);
        tft.setCursor(279, 3);
        tft.print("META");
    }
}

// 17 · OBJETOS MÓVILES
// Dibuja estudiante, micro y automóviles según el desafío activo.
void dibujarMoviles() {
    switch(slideActual) {
    case PantallaUI::SLIDE_LABERINTO:
    case PantallaUI::SLIDE_SALA:
        pantalla.dibujarEstudiante(jugadorX,jugadorY);
        break;
    case PantallaUI::SLIDE_CARRETERA:
        pantalla.dibujarAutobus(jugadorX,jugadorY);
        for (const auto &autoMovil: autos)
            pantalla.dibujarAuto(autoMovil.x,autoMovil.y,autoMovil.color);
        break;
    default: break;
    }
}
// 18 · INICIALIZACIÓN DE CADA PANTALLA
// Restablece estado, cronómetro y posiciones, y dibuja el escenario correspondiente.
void iniciarSlide(int slide) {
    slideActual=slide;
    estado=Estado::EnCurso;
    inicioNivelMs=relojMs();
    ultimoDibujoMs=inicioNivelMs;
    segundosPrevios=-1;

    if (slide == PantallaUI::SLIDE_LABERINTO) {jugadorX=18; jugadorY=32;}
    if (slide == PantallaUI::SLIDE_CARRETERA) {
        jugadorX=18; jugadorY=PantallaUI::yCarril(1)-1;
        autos[0].x=238; autos[1].x=305; autos[2].x=192;
    }
    if (slide == PantallaUI::SLIDE_SALA) {jugadorX=16; jugadorY=34;}

    pantalla.dibujarSlide(slideActual);
    if (slideActual == PantallaUI::SLIDE_CARRETERA) dibujarMetaCarretera(true);
    dibujarMoviles();
    if (limiteActual() > 0) pantalla.dibujarTiempo(limiteActual());
    printf("DORE 8: pantalla %d\n", slideActual);
}

// 19 · REGLAS DEL BOTÓN DE CONTINUAR
// En pantallas informativas avanza; en niveles exige victoria o permite reintentar una derrota.
void controlarBotonContinuar() {
    if (!esNivel(slideActual)) {
        iniciarSlide((slideActual+1) % PantallaUI::TOTAL_SLIDES);
        return;
    }
    if (estado == Estado::Superado) {
        iniciarSlide((slideActual+1) % PantallaUI::TOTAL_SLIDES);
    } else if (estado == Estado::Fallido) {
        iniciarSlide(slideActual);
    } else {
        // En curso: no se permite saltar el desafio.
        printf("Desafio pendiente: completa el nivel para continuar.\n");
    }
}

// 20 · ACTUALIZACIÓN DE ANIMACIÓN
// Borra el personaje anterior, restaura elementos del fondo y lo dibuja en la nueva posición.
void dibujarMovimiento(int antesX,int antesY) {
    // Restaurar solo el rectangulo anterior: no se borran escenas completas.
    if(slideActual == PantallaUI::SLIDE_CARRETERA) {
        tft.fillRect(antesX,antesY,Sprite::AUTOBUS_W,Sprite::AUTOBUS_H,Colores::NEGRO);
        for(const auto &v:autos)
            tft.fillRect(v.x+v.paso,v.y,Sprite::AUTO_W,Sprite::AUTO_H,Colores::NEGRO);
        pantalla.redibujarLineasCarretera();
        dibujarMetaCarretera(false);
    } else {
        tft.fillRect(antesX,antesY,Sprite::ESTUDIANTE_W,Sprite::ESTUDIANTE_H,Colores::NEGRO);
    }
    dibujarMoviles();
}

// 21 · LÓGICA PRINCIPAL DE LOS DESAFÍOS
// Comprueba tiempo, lee dirección de perillas y resuelve movimiento, colisiones y metas.
void actualizarNivel(uint32_t ahoraMs) {
    if (!esNivel(slideActual) || estado != Estado::EnCurso) return;
    const int limite = limiteActual();
    // Los niveles cronometrados muestran el tiempo restante; la sala se juega sin límite.
    if (limite > 0) {
        const int transcurrido = static_cast<int>((ahoraMs-inicioNivelMs)/1000);
        const int restante = std::max(0,limite-transcurrido);
        if(restante != segundosPrevios) {
            segundosPrevios=restante;
            pantalla.dibujarTiempo(restante);
        }
        // El LED de GP7 parpadea en los últimos cinco segundos.
        if(restante <= 5) {
            gpio_put(PIN_LED, (ahoraMs / 250)%2 == 0);
        } else gpio_put(PIN_LED,0);
        if(restante == 0){cambiarEstado(Estado::Fallido);return;}
    } else gpio_put(PIN_LED,0);

    // La carretera utiliza una actualización más frecuente que las demás etapas.
    const uint32_t intervalo = (slideActual == PantallaUI::SLIDE_CARRETERA)
                               ? INTERVALO_CARRETERA_MS : INTERVALO_DIBUJO_MS;
    if(ahoraMs-ultimoDibujoMs < intervalo)return;
    ultimoDibujoMs=ahoraMs;

    const int antesX=jugadorX,antesY=jugadorY;
    // Se convierte cada posición de la perilla en desplazamientos horizontal y vertical.
    const int dx=perillaHorizontal.direccion() *
                 (slideActual == PantallaUI::SLIDE_CARRETERA ? PASO_MICRO : 6);
    const int dy=perillaVertical.direccion() *
                 (slideActual == PantallaUI::SLIDE_CARRETERA ? PASO_MICRO_VERTICAL : 6);

    // DESAFÍO 1 · LABERINTO
    // Mueve al estudiante sin atravesar paredes; llegar a la micro marca victoria.
    if(slideActual == PantallaUI::SLIDE_LABERINTO) {
        // Movimiento pixel a pixel: evita atravesar muros delgados (2 px).
        for(int paso=0;paso<std::abs(dx);++paso) {
            int proximoX=jugadorX+(dx>0 ? 1 : -1);
            if(pantalla.colisionLaberinto(proximoX,jugadorY))break;
            jugadorX=proximoX;
        }
        for(int paso=0;paso<std::abs(dy);++paso) {
            int proximoY=jugadorY+(dy>0 ? 1 : -1);
            if(pantalla.colisionLaberinto(jugadorX,proximoY))break;
            jugadorY=proximoY;
        }
        if(interseccion(jugadorX,jugadorY,Sprite::ESTUDIANTE_W,Sprite::ESTUDIANTE_H,
                        266,204,Sprite::AUTOBUS_W,Sprite::AUTOBUS_H)){
            dibujarMovimiento(antesX,antesY);
            cambiarEstado(Estado::Superado);return;
        }
    // DESAFÍO 2 · CARRETERA
    // Mueve la micro, desplaza autos en sentido contrario y comprueba choques o meta.
    } else if(slideActual == PantallaUI::SLIDE_CARRETERA) {
        jugadorX=std::clamp(jugadorX+dx,10,268);
        jugadorY=std::clamp(jugadorY+dy,24,211);
        for(auto &v:autos){
            v.x-=v.paso;
            if(v.x < -Sprite::AUTO_W) v.x = 326 + (v.paso*10);
            if(interseccion(jugadorX,jugadorY,Sprite::AUTOBUS_W,Sprite::AUTOBUS_H,
                            v.x,v.y,Sprite::AUTO_W,Sprite::AUTO_H)){
                // Redibujar despues de colision antes del mensaje.
                pantalla.dibujarSlide(slideActual);
                dibujarMetaCarretera(true);
                dibujarMoviles();
                cambiarEstado(Estado::Fallido);
                return;
            }
        }
        // La micro completa la etapa cuando su borde derecho toca la META.
        if(jugadorX >= 268){
            pantalla.dibujarSlide(slideActual);
            dibujarMetaCarretera(true);
            dibujarMoviles();
            cambiarEstado(Estado::Superado);return;
        }
    // DESAFÍO 3 · SALA DE CLASES
    // Mueve al estudiante hasta la puerta 304, sin cronómetro.
    } else if(slideActual == PantallaUI::SLIDE_SALA) {
        jugadorX=std::clamp(jugadorX+dx,9,291);
        jugadorY=std::clamp(jugadorY+dy,24,209);
        if(interseccion(jugadorX,jugadorY,Sprite::ESTUDIANTE_W,Sprite::ESTUDIANTE_H,
                        266,174,40,52)){
            dibujarMovimiento(antesX,antesY);
            cambiarEstado(Estado::Superado);return;
        }
    }

    if(antesX!=jugadorX || antesY!=jugadorY || slideActual==PantallaUI::SLIDE_CARRETERA)
        dibujarMovimiento(antesX,antesY);
}
} // namespace

// 22 · PUNTO DE ENTRADA Y BUCLE CONTINUO
// Inicia la Pico 2 W, ADC, pantalla, controles y LED; no inicializa audio.
int main() {
    // Habilita la comunicación estándar (USB si está activada en CMake).
    stdio_init_all();
    // Inicializa el conversor analógico-digital para ambas perillas.
    adc_init();
    perillaVertical.iniciar();
    perillaHorizontal.iniciar();
    // Boton2 configura cada GPIO en el constructor.
    // Prepara el LED como salida y lo mantiene apagado al arrancar.
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_put(PIN_LED,0);
    // Sin inicializar PWM ni el pin GP13 (parlante desconectado).
    // Inicia el controlador ST7789V y dibuja la pantalla de bienvenida.
    pantalla.begin();
    iniciarSlide(PantallaUI::SLIDE_INICIO);
    printf("DORE 8 sin audio | GP10 reservado | GP11 continuar | GP26 vertical | GP27 horizontal\n");

    // BUCLE PRINCIPAL
    // Cada iteración lee botones, comprueba gestos, actualiza el nivel y espera 5 ms.
    while(true) {
        const uint32_t ahora=relojMs();
        // Sin audio.
        // Se lee GP10 pero sus gestos se descartan porque el audio quedó desactivado.
        botonMusica.leer();
        botonAvanzar.leer();
        // Para DORE 8, solo un clic simple realiza acciones.
        // Los gestos doble, triple y largo están disponibles para usos futuros.
        // Botón GP10 reservado hasta instalar un amplificador de audio.
        (void)botonMusica.leerGesto();
        // Solo el clic sencillo en GP11 puede comenzar, avanzar o reiniciar una etapa.
        if (botonAvanzar.leerGesto() == Gesto::PULSADO1VEZ)
            controlarBotonContinuar();
        // Actualiza cronómetro, posiciones, colisiones, LED y dibujo de la etapa.
        actualizarNivel(ahora);
        // Una espera breve limita el sondeo sin detener de forma visible el juego.
        sleep_ms(5);
    }
}
