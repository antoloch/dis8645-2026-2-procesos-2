# sesion-13b

## apuntes sesión
correcciones proyecto

libro: arte sonora - santiago auserón

agregar información de cada especie de alguna manera

recursos para sonidos:
- https://audiomapa.org
- https://museobioacustico.org/coleccion

### descripción total del proyecto
Este proyecto busca educar sobre la biodiversidad de las especies amenazadas o en peligro de extinción que habitan nuestro territorio, a través de una máquina sonora que da visibilidad a sus voces, a su existencia y, como consecuencia, apela a su cuidado, ya que no se puede cuidar lo que no se conoce. La máquina se organiza en cuatro categorías: mamíferos, aves, anfibios e insectos. Cada categoría contiene tres especies, una por cada zona del país (norte, centro y sur). 

La máquina permite escuchar, silenciar y superponer los sonidos de los distintos animales mediante perillas, generando composiciones sonoras que surgen de la combinación de sus voces. Así, el instrumento funciona tanto como un pequeño archivo de biodiversidad sonora como una herramienta de exploración y composición de un paisaje sonoro.

### pseudocódigo

```cpp
// ============ DATOS ============

categorias = [mamiferos, aves, anfibios, insectos]
especiesPorCategoria = 3

// Un audio por especie (todos preparados para repetirse en loop)
audios[categoria][especie] = "mamiferos/huemul", "aves/...", etc.
sonidoEncendido = "sistema/encendido"
sonidoApagado = "sistema/apagado"

// Estado general
maquinaEncendida = falso

// Estado de cada categoría
para cada categoria:
    especieActual[categoria] = ninguna
    volumenActual[categoria] = 0


// ============ PROGRAMA PRINCIPAL ============

inicio:
    prepararMaquina()
    quedarEnReposo()

    repetir siempre:
        si botonEncendidoPresionado():
            si maquinaEncendida:
                apagarMaquina()
            sino:
                encenderMaquina()

        si maquinaEncendida:
            para cada categoria en categorias:
                revisarEspecie(categoria)
                revisarVolumen(categoria)


// ============ ENCENDIDO Y APAGADO ============

funcion encenderMaquina():
    maquinaEncendida = verdadero
    reproducir(sonidoEncendido)
    esperarFinDelSonido()

    para cada categoria en categorias:
        especieActual[categoria] = ninguna
        volumenActual[categoria] = ninguno
        revisarEspecie(categoria)          // carga la especie que marca la perilla
        revisarVolumen(categoria)          // aplica el volumen que marca la perilla

funcion apagarMaquina():
    reproducir(sonidoApagado)
    esperarFinDelSonido()
    maquinaEncendida = falso
    quedarEnReposo()

funcion quedarEnReposo():
    para cada categoria en categorias:
        detenerSonido(categoria)
        fijarVolumen(categoria, 0)


// ============ CONTROLES ============

funcion revisarEspecie(categoria):
    posicion = leerPerillaEspecie(categoria)
    nuevaEspecie = convertirAEspecie(posicion, especiesPorCategoria)

    si nuevaEspecie es distinta de especieActual[categoria]:
        cambiarEspecie(categoria, nuevaEspecie)

funcion revisarVolumen(categoria):
    posicion = leerPerillaVolumen(categoria)
    nuevoVolumen = convertirAVolumen(posicion)       // de 0 a 100

    si nuevoVolumen es distinto de volumenActual[categoria]:
        volumenActual[categoria] = nuevoVolumen
        fijarVolumen(categoria, nuevoVolumen)


// ============ CAMBIO DE ESPECIE ============

funcion cambiarEspecie(categoria, nuevaEspecie):
    especieActual[categoria] = nuevaEspecie
    detenerSonido(categoria)
    cargarAudio(categoria, audios[categoria][nuevaEspecie])
    reproducirEnLoop(categoria)                      // siempre suena, el volumen decide si se oye
    fijarVolumen(categoria, volumenActual[categoria])
```

### esqueleto en wokwi
El simulador se trabajó con IA para hacer un prototipo de las conexiones que necesitábamos.

Prompt:
> "Para este examen vamos a trabajar con C++. Necesito desarrollar un código de prueba para controlar 8 potenciómetros y un botón de encendido/apagado. Los primeros 4 potenciómetros serán para seleccionar especies. Cada uno tendrá 3 rangos de valores, correspondientes a Especie 01, Especie 02 y Especie 03, dependiendo de cuánto se gire. Los otros 4 potenciómetros controlarán el volumen de cada uno de los selectores, de forma independiente. Además, necesitamos un botón que permita encender y apagar todo el sistema, deteniendo la reproducción de los sonidos cuando esté apagado. La idea es probar primero esta lógica de funcionamiento en C++, utilizando sonidos de prueba y dejando el código preparado para integrar posteriormente los audios definitivos."

https://wokwi.com/projects/480943431055838209
