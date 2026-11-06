# generarSonidoSistema.py
# crea el sonido de sistema, un arpegio corto estilo 8 bits (onda cuadrada)
# se usa para sonidoInicio, sonidoLoop y sonidoApagado de la clase Audio
#
# genera dos archivos:
# - un .wav para escucharlo en el computador
# - SonidoSistema.h con el arreglo uint16_t sonidoSistema, igual que convertir.py
#
# uso:
#   python generarSonidoSistema.py --wav <archivo .wav> -o <carpeta del .h>
# ejemplo:
#   python generarSonidoSistema.py --wav ../audio/sonido-sistema.wav -o ../codigos/v03

import argparse
import os
import struct
import wave

# misma frecuencia que Ampli::frecuencia
FRECUENCIA = 11025
# volumen maximo del sonido, de 0 a 1
# la onda cuadrada suena fuerte, por eso se deja bajo la mitad
AMPLITUD = 0.5
# cuanto dura la subida y bajada de cada nota, evita el "tic" entre notas
SUAVIZADO = 0.004

# notas del arpegio: (frecuencia en Hz, duracion en segundos)
# la, do sostenido, mi y la de la octava siguiente (acorde de la mayor)
NOTAS = [
    (880.00, 0.06),
    (1108.73, 0.06),
    (1318.51, 0.06),
    (1760.00, 0.16),
]


# crea las muestras de una nota con onda cuadrada de 25% (sonido tipo NES)
def crearNota(frecuencia, duracion, desvanecer):
    total = int(duracion * FRECUENCIA)
    rampa = int(SUAVIZADO * FRECUENCIA)
    muestras = []
    for i in range(total):
        fase = (i * frecuencia / FRECUENCIA) % 1.0
        # 0.75 y -0.25 dejan la onda centrada en 0, asi el parlante no da golpes
        valor = 0.75 if fase < 0.25 else -0.25
        # sube y baja suave al inicio y al final de la nota
        envolvente = min(1.0, i / rampa, (total - 1 - i) / rampa)
        # la ultima nota se apaga de a poco
        if desvanecer:
            envolvente *= 1.0 - i / total
        muestras.append(int(valor * envolvente * AMPLITUD * 32767))
    return muestras


def main():
    parser = argparse.ArgumentParser(description="genera el sonido de sistema estilo 8 bits")
    parser.add_argument("--wav", default="sonido-sistema.wav", help="archivo .wav de salida")
    parser.add_argument("-o", "--salida", default=".", help="carpeta donde se guarda SonidoSistema.h")
    args = parser.parse_args()

    muestras = []
    for numero, (frecuencia, duracion) in enumerate(NOTAS):
        muestras += crearNota(frecuencia, duracion, numero == len(NOTAS) - 1)
    # termina en silencio para que el parlante quede en reposo
    muestras += [0] * int(0.02 * FRECUENCIA)

    # guarda el wav: mono, 16 bits, 11025 Hz
    with wave.open(args.wav, "wb") as archivo:
        archivo.setnchannels(1)
        archivo.setsampwidth(2)
        archivo.setframerate(FRECUENCIA)
        archivo.writeframes(struct.pack(f"<{len(muestras)}h", *muestras))

    # guarda el .h con los bits del pcm con signo tal cual, igual que convertir.py
    sinSigno = [m & 0xffff for m in muestras]
    lineas = []
    for i in range(0, len(sinSigno), 12):
        lineas.append("  " + ", ".join(f"0x{m:04x}" for m in sinSigno[i:i + 12]))
    salida = (
        f"// SonidoSistema.h\n"
        f"// {len(muestras)} muestras de 16 bits, mono, {FRECUENCIA} Hz\n"
        f"// se guardan los bits del pcm con signo tal cual, leer con (int16_t)\n"
        f"// generado con script/generarSonidoSistema.py\n\n"
        f"#ifndef SONIDO_SISTEMA_H\n#define SONIDO_SISTEMA_H\n\n#include <stdint.h>\n\n"
        f"static const uint16_t sonidoSistema[] = {{\n" + ",\n".join(lineas) + "\n};\n\n#endif\n"
    )
    destino = os.path.join(args.salida, "SonidoSistema.h")
    with open(destino, "w", encoding="utf-8", newline="\n") as archivo:
        archivo.write(salida)
    print(f"{args.wav} y {destino}: {len(muestras)} muestras, {FRECUENCIA} Hz")


if __name__ == "__main__":
    main()
