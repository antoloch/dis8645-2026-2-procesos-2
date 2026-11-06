#pragma once
#include <cstdint>

// DORE 8 - Clase recibida de otro grupo y adaptada: clics y antirrebote.
// 12 · CLASE Boton2 (recibida de otro grupo y adaptada)
// Su máquina de estados permite reconocer pulsaciones sin bloquear el programa.
enum class EstadoBoton {
    REPOSO,
    PRESIONADO,
    ESPERANDO_PRESION
};

// Se definen clic sencillo, doble, triple y dos duraciones de pulsación larga.
enum class Gesto {
    NADA,
    PULSADO1VEZ,
    PULSADO2VECES,
    PULSADO3VECES,
    PULSACIONLARGA,
    PULSACIONMUYLARGA
};

// El constructor prepara un GPIO; leer() analiza cambios; leerGesto() entrega el evento.
class Boton2 {
public:
    // El número es GPIO, no número físico de la placa.
    explicit Boton2(int pin);
    void leer();
    Gesto leerGesto();

    // Parámetros ajustables para el proyecto que reutiliza esta clase.
    uint32_t tiempoLargo = 1000;
    uint32_t tiempoMuyLargo = 5000;
    uint32_t tiempoEntrePresionado = 400;
    uint32_t tiempoAntirrebote = 25;

private:
    int pin;
    EstadoBoton estado = EstadoBoton::REPOSO;
    Gesto gestoPendiente = Gesto::NADA;
    bool presionado = false;
    int numeroPresiones = 0;
    uint32_t instanteCambio = 0;
    uint32_t instanteUltimaLiberacion = 0;
    bool lecturaBrutaAnterior = false;
    uint32_t instanteCambioBruto = 0;
};
