# sesion-13b

## apuntes sesión

Esta clase los profes revisaron los proyectos que habíamos subido y las ideas. Ángel y yo hablamos con Misa sobre nuestra idea y nos dieron un reajuste. Originalmente queríamos hacer algo con canciones, pero decidimos cambiar la idea para que tuviera un sentido más propio, algo que nos represente como estudiantes y no solo canciones al azar.

Flujo de la clase: revisión de la idea → correcciones → idea nueva → prototipo.

---

### La idea original: la rocola

Era un juego interactivo físico inspirado en una rocola clásica, para probar cuánto saben de música los jugadores.

1. **Escucha**: la máquina reproduce al azar un pedazo de una canción.
2. **Elige**: muestra tres opciones (A, B o C).
3. **Presiona**: el jugador responde con uno de tres botones físicos.

- **Niveles**: 3 seguidos. Para ganar había que adivinar las tres canciones.
- **Premio**: un mecanismo interno liberaba un dulce a una bandeja exterior.

---

### Correcciones del proyecto

- **Simplificar**: evitar funciones demasiado complejas, como reconocer palabras específicas con la voz.
- **Alternativas**: medir la frecuencia (Hz) y la intensidad de la voz puede ser más simple.
- **Referencias**: el filtro de TikTok y la pelota fueron ejemplos para explicar posibilidades, por ejemplo hacer que pase un objeto específico por un lugar según la frecuencia medida.
- **Describir bien el juego**: de qué trata, su objetivo, cómo se juega, cuántos jugadores participan, cuántos niveles tiene y cuáles son sus reglas.
- **El qué antes que el cómo**: primero la experiencia del jugador, y los detalles de Raspberry Pi, C++ y componentes al final.
- **Redacción**: unos tres párrafos bien desarrollados y separados, sin resumir demasiado.

---

### Nueva dirección: muestrario de sonidos

Nos sugirieron que, en vez de ser un challenge, sea más un **muestrario de sonidos cotidianos**, por ejemplo pájaros, taladros, sonidos de la feria.

- Que se maneje con **botones**.
- Que nos indique qué hacer con **sonidos o con una pantalla**, según lo que elijamos.
- O dejar escrito en la **carcasa** cómo se usa, de forma simple y con íconos, y dejar solo los sonidos en el reproductor.

#### Idea que armamos

Un parlante con **3 botones**, cada uno con un título arriba que indica su tema. Al presionar un botón suena uno de los 3 sonidos de ese tema, uno por cada pulsación.

- 3 botones y **9 sonidos** en total, 3 por tema.
- Si hace falta una perilla, puede servir para variar el tono de los sonidos.

---

### Prototipo: consola de sonidos retro

Es un muestrario interactivo que permite reconocer acciones de videojuegos a través del sonido, sin mirar una pantalla.

- **3 botones**: Mario, Sonic y Pac-Man.
- Cada "juego" tiene **3 sonidos** que representan lo icónico de cada uno.
- **Perilla 1**: tono más grave o más agudo.
- **Perilla 2**: cuatro posiciones, AUTO, 1, 2 y 3.
  - En **AUTO**, cada botón reproduce sus pistas en orden 1 → 2 → 3 → 1, con un contador independiente.
  - En **1, 2 o 3** se elige directamente la pista del personaje que se pulse.
- Las perillas se pueden ajustar de antemano y sus valores se guardan al pulsar. El audio que ya está sonando conserva sus ajustes.
- La consola está **en silencio** hasta que se pulsa un botón.
- Cada pulsación nueva puede interrumpir la pista anterior, incluso si es el mismo botón.
- Los efectos cortos se repiten un número limitado de veces y después la consola vuelve a la espera.
- **Pista sorpresa**: si se pulsan los tres botones juntos suena algo como el "¡D'oh!" de Homero Simpson.

| Posición | Acción | Mario | Sonic | Pac-Man |
| --- | --- | --- | --- | --- |
| 1 | Movimiento | Salto | Spin Dash | "Waka-waka" |
| 2 | Recompensa | Moneda | Anillo | Fruta o bonificación |
| 3 | Pérdida | Pérdida de vida | Pérdida de anillos | Pérdida de vida |

Este es el prototipo preliminar. En el proyecto definitivo vamos a elegir un tema específico con sonidos de ese ámbito, y todo sigue sujeto a cambios.

---

### Diagrama y pseudocódigo

- [Diagrama de flujo](https://github.com/Narelyriquelme/dis8645-2026-2-procesos-2/blob/main/27-angel-udp/sesion-13b/imagenes/diagrama-proyecto03.png)
- [Prototipo en Wokwi](https://wokwi.com/projects/480948910221458433)

```text
INICIO
  Iniciar en silencio y poner los tres contadores en 1

  REPETIR
    Leer perillas y detectar nuevas pulsaciones con antirrebote
    Guardar la posición del selector y el tono de cada pulsación

    SI se pulsan los tres botones dentro de la ventana del combo
      Seleccionar la pista sorpresa sin avanzar los contadores
      Rearmar el combo al soltar los tres botones
    SI NO, al cerrar la ventana
      Procesar las pulsaciones en orden
      En AUTO, seleccionar la pista del contador y avanzar 1 → 2 → 3 → 1
      En 1, 2 o 3, seleccionar esa pista sin avanzar el contador
      Si hay varias solicitudes, lanzar la última

    AL LANZAR UNA PISTA
      Cortar el audio anterior y reemplazar cualquier inicio pendiente
      Dejar 15 ms de silencio mientras se siguen leyendo los botones
      Reproducir desde el principio con el tono guardado

    AL TERMINAR UNA VUELTA
      Repetir si quedan vueltas programadas
      Si era la última, volver a la espera silenciosa
  FIN REPETIR
FIN
```

---

## encargos

## lectura
