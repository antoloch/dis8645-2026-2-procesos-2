# script

## convertir.py

Convierte sonidos `.wav` en archivos `sonidoN.h` que el código de la Raspberry Pi Pico puede incluir directamente. Cada archivo trae un arreglo `uint16_t` llamado `sonidoN` con las muestras de audio, listo para las clases `Audio` y `Ampli` de [codigos/v02](../codigos/v02).

### Qué hace

1. Lee cada archivo `.wav`. También acepta los `.h` antiguos que tenían el wav volcado byte por byte.
2. Recorre los bloques del wav y se queda solo con el audio (bloque `data`). Descarta la cabecera (`RIFF`, `fmt`) y los metadatos (`LIST`), que si no se reproducirían como ruido.
3. Revisa que el sonido sea PCM, mono y de 16 bits. Si no lo es, se detiene con un mensaje de error.
4. Une los bytes de dos en dos (little-endian) para formar cada muestra de 16 bits. Los bits se guardan tal cual, sin cambiarlos. Por ejemplo, la muestra −2 queda como `0xfffe`.
5. Escribe `sonidoN.h` con:
   - un comentario con la cantidad de muestras y la frecuencia;
   - la protección `#ifndef SONIDON_H`;
   - el arreglo `static const uint16_t sonidoN[]`, con 12 valores por línea.
6. Avisa si la frecuencia no es 11025 Hz, que es la que usa `Ampli`.

El número `N` se saca del último número que aparece en el nombre del archivo: `Sonido_3.wav` genera `sonido3.h`.

### Cómo se usa

Se necesita Python 3 y nada más (no usa librerías externas). Desde esta carpeta:

```sh
python convertir.py ../audio/audios-test/*.wav -o ../codigos/v02
```

- Los primeros argumentos son los archivos a convertir.
- `-o` es la carpeta donde se guardan los `sonidoN.h`. Si se omite, se usa la carpeta actual.

Si se reemplaza el archivo de un sonido, basta con volver a correr el script: el `sonidoN.h` se sobrescribe.

### Requisitos de los sonidos

- Formato WAV PCM, **mono**, **16 bits**.
- **11025 Hz**, la misma frecuencia que `Ampli::frecuencia`. Si no, el sonido se reproduce más rápido o más lento.
- El nombre debe llevar el número del sonido, por ejemplo `Sonido_0.wav`.

### Cómo se lee en el código

Las muestras originales tienen signo (de −32768 a 32767), pero se guardan en `uint16_t`. Por eso `Ampli::actualizar()` las lee con `(int16_t)`:

```cpp
int16_t muestraOriginal = (int16_t)datosAudio[posicionAudio];
```

En `Audio.cpp`, el tamaño de cada sonido se calcula en cantidad de muestras, no en bytes:

```cpp
catalogo[0].tamano = sizeof(sonido0) / sizeof(sonido0[0]);
```

### Qué se pidió

El script nació al revisar los archivos `Sonido_N.h` de `codigos/v02`. Se pidió lo siguiente:

- **Corregir el formato de los archivos de sonido.** Los arreglos decían `uint16_t`, pero cada elemento era un solo byte del wav. Además incluían la cabecera y los metadatos.
- **Cambiar el nombre a `sonidoN.h`**, donde N es el número del sonido.
- **Numerar los sonidos de 0 a 4** (`sonido0` … `sonido4`), igual que el índice del catálogo de `Audio`.
- **Usar arreglos `uint16_t`**, guardando los bits del wav tal cual en vez de sumarles 32768.
- **Ajustar el código que usa los sonidos**:
  - `Audio.h`, `Audio.cpp`, `Ampli.h` y `Ampli.cpp` pasan de `uint8_t` a `uint16_t`;
  - se corrigen los `#include` y los nombres de los arreglos;
  - se quita la conversión de 8 bits (`- 128` y `* 256`).
- **Dejar el script en el repositorio** con este README, para poder convertir sonidos nuevos.

## generarSonidoSistema.py

Crea el sonido de sistema que usa la clase `Audio` de [codigos/v03](../codigos/v03) para `sonidoInicio`, `sonidoLoop` y `sonidoApagado` (por ahora los tres usan el mismo sonido).

Es un arpegio corto (la, do sostenido, mi, la) con onda cuadrada de 25 %, el sonido típico de los chips de 8 bits. Dura unos 0,36 s, empieza y termina en silencio, y la onda está centrada en 0 para que el parlante no dé "tics".

Genera dos archivos:

- un `.wav` (mono, 16 bits, 11025 Hz) para escucharlo en el computador;
- `SonidoSistema.h`, con el arreglo `static const uint16_t sonidoSistema[]`, en el mismo formato que `convertir.py`.

```sh
python generarSonidoSistema.py --wav ../audio/sonido-sistema.wav -o ../codigos/v03
```

Para cambiar el sonido se editan las listas `NOTAS` (frecuencia y duración de cada nota) y `AMPLITUD` al inicio del script, y se vuelve a correr.
