# convertir.py
# convierte sonidos wav en archivos sonidoN.h con arreglos uint16_t
# para usarlos con las clases Audio y Ampli del proyecto
#
# uso:
#   python convertir.py <archivos de entrada> -o <carpeta de salida>
# ejemplo:
#   python convertir.py ../audio/audios-test/*.wav -o ../codigos/v02

import argparse
import os
import re
import struct


# lee los bytes del wav, ya sea un .wav o un .h con el wav volcado byte por byte
def leerBytes(ruta):
    if ruta.lower().endswith(".wav"):
        with open(ruta, "rb") as archivo:
            return archivo.read()
    texto = open(ruta, encoding="utf-8").read()
    # toma solo lo que esta entre las llaves del arreglo
    cuerpo = texto[texto.index("{") + 1:texto.rindex("}")]
    return bytes(int(valor, 16) for valor in re.findall(r"0x[0-9a-fA-F]+", cuerpo))


# recorre los bloques del wav y devuelve el formato y las muestras de audio
# descarta la cabecera y los metadatos (bloque LIST)
def extraerPcm(datos, ruta):
    if datos[:4] != b"RIFF" or datos[8:12] != b"WAVE":
        raise ValueError(f"{ruta}: no es un archivo wav")
    posicion, formato, pcm = 12, None, None
    while posicion + 8 <= len(datos):
        nombre = datos[posicion:posicion + 4]
        tamano = struct.unpack_from("<I", datos, posicion + 4)[0]
        if nombre == b"fmt ":
            formato = struct.unpack_from("<HHIIHH", datos, posicion + 8)
        if nombre == b"data":
            pcm = datos[posicion + 8:posicion + 8 + tamano]
        # los bloques de tamano impar llevan un byte de relleno
        posicion += 8 + tamano + (tamano & 1)
    if formato is None or pcm is None:
        raise ValueError(f"{ruta}: le falta el bloque fmt o data")
    tipo, canales, frecuencia, _, _, bits = formato
    if (tipo, canales, bits) != (1, 1, 16):
        raise ValueError(f"{ruta}: debe ser pcm, mono y de 16 bits "
                         f"(tiene tipo {tipo}, {canales} canales, {bits} bits)")
    return frecuencia, pcm


# escribe el archivo sonidoN.h con 12 muestras por linea
def escribirHeader(numero, frecuencia, pcm, carpeta, origen):
    pcm = pcm[:len(pcm) // 2 * 2]
    # une los bytes de a dos (little-endian) sin cambiar los bits
    muestras = struct.unpack(f"<{len(pcm) // 2}H", pcm)
    lineas = []
    for i in range(0, len(muestras), 12):
        lineas.append("  " + ", ".join(f"0x{m:04x}" for m in muestras[i:i + 12]))
    salida = (
        f"// sonido{numero}.h\n"
        f"// {len(muestras)} muestras de 16 bits, mono, {frecuencia} Hz\n"
        f"// se guardan los bits del pcm con signo tal cual, leer con (int16_t)\n\n"
        f"#ifndef SONIDO{numero}_H\n#define SONIDO{numero}_H\n\n#include <stdint.h>\n\n"
        f"static const uint16_t sonido{numero}[] = {{\n" + ",\n".join(lineas) + "\n};\n\n#endif\n"
    )
    destino = os.path.join(carpeta, f"sonido{numero}.h")
    with open(destino, "w", encoding="utf-8", newline="\n") as archivo:
        archivo.write(salida)
    print(f"{os.path.basename(origen)} -> sonido{numero}.h: {len(muestras)} muestras, {frecuencia} Hz")
    if frecuencia != 11025:
        print(f"  ojo: Ampli usa 11025 Hz y este sonido esta a {frecuencia} Hz")


def main():
    parser = argparse.ArgumentParser(description="convierte wav en sonidoN.h con arreglos uint16_t")
    parser.add_argument("entradas", nargs="+", help="archivos .wav (o .h con el wav volcado en bytes)")
    parser.add_argument("-o", "--salida", default=".", help="carpeta donde se guardan los sonidoN.h")
    args = parser.parse_args()

    for ruta in args.entradas:
        # el numero N se saca del ultimo numero que aparece en el nombre del archivo
        numeros = re.findall(r"\d+", os.path.splitext(os.path.basename(ruta))[0])
        if not numeros:
            raise SystemExit(f"{ruta}: el nombre debe tener el numero del sonido, por ejemplo Sonido_0.wav")
        frecuencia, pcm = extraerPcm(leerBytes(ruta), ruta)
        escribirHeader(int(numeros[-1]), frecuencia, pcm, args.salida, ruta)


if __name__ == "__main__":
    main()
