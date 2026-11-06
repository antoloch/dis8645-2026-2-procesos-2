#ifndef TRAZO_PERILLA_H
#define TRAZO_PERILLA_H
#include <cstdint>
// Nos basamos en la clase Perilla que vimos con los profes en la sesión 08a.
// La adaptamos para que cada perilla tenga su rango. El ADC lo iniciamos una vez en main.
class Perilla {
public:
    const unsigned int patita; // Aquí guardamos el GPIO: GP26, GP27 o GP28 (ojo, no el número físico de la patita).
    const int maximo;          // Este es el último valor que queremos recibir; para 128 píxeles usamos 127.
    uint16_t lectura = 0;      // Guardamos el promedio de cuatro lecturas, entre 0 y 4095.
    int posicion = 0;          // Aquí queda el valor listo para usar, desde 0 hasta maximo.
    bool cambio = false;      // Nos avisa si la posición cambió desde la lectura anterior.
    bool valida = false;      // Primero revisamos que el pin y el rango nos sirvan.
    Perilla(unsigned int pin, int limite);
    void leer();
    static int convertir(uint16_t valor, int limite); // Hace la conversión sin leer hardware, así la podemos probar por separado.
};
#endif
