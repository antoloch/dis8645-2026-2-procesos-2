# sesion-14a

## apuntes sesión

Hoy seguimos avanzando en el **proyecto 03** (grupo 8), y ya tenemos la idea definida. La clase pasada habíamos corregido varias veces la idea en la carpeta de la sesión-13b, y en esta revisión la ajustamos por última vez: en vez de usar sonidos de videojuegos, vamos a hacer un **repertorio de sonidos propios**. Así no tenemos que lidiar con temas de derechos y podemos darle la profundidad y especificidad que queramos, eligiendo sonidos específicos de la universidad, como en un soundboard.

Flujo de la clase: idea ajustada → descripción definitiva → controles → pseudocódigo → lista de componentes (BOM).

---

### Descripción del proyecto: Sound Board (con sonidos propios)

Es un **muestrario interactivo con nueve sonidos grabados por nosotros**, para escuchar y reconocer sonidos cotidianos de forma sencilla.

- **3 categorías**, cada una asociada a un botón y con **3 grabaciones** distintas. Las categorías y los objetos, acciones o ambientes a registrar los define el grupo (por ejemplo, naturaleza, herramientas o sonidos de la feria).
- **Ficha por pista**: cada grabación tiene una ficha breve con el nombre del sonido, qué lo produjo y quién lo grabó.
- **Consola**: 3 botones, 2 perillas y un parlante, dentro de una carcasa con etiquetas o íconos para que sea fácil de usar.
- **Secuencia**: al presionar un botón suena el primer sonido de su categoría. Las siguientes pulsaciones avanzan al segundo, al tercero y vuelven al primero (1 → 2 → 3 → 1). Cada botón conserva su propia posición.
- **Perilla de volumen**: va desde el silencio hasta el máximo configurado.
- **Perilla de tono**: modifica la altura del sonido, de más grave a más agudo, con el tono original al centro.
- **Standby**: la consola está en silencio hasta que se pulsa un botón.
- **Interrupción**: si se pulsa otro botón, o el mismo mientras suena una pista, la reproducción se corta y la siguiente empieza desde el principio después de una pausa breve. Al terminar, la consola vuelve al silencio.
- **Grabación sorpresa**: una décima grabación, hecha por el grupo, que se activa al presionar los tres botones juntos.

#### Controles

| Control | Función definida |
| --- | --- |
| Botón 1 | Reproduce las tres grabaciones de la categoría 1 en orden: 1, 2, 3 y 1. |
| Botón 2 | Reproduce las tres grabaciones de la categoría 2 con su propia secuencia. |
| Botón 3 | Reproduce las tres grabaciones de la categoría 3 con su propia secuencia. |
| Perilla de volumen | A la izquierda, silencio. Al girar a la derecha, el volumen sube hasta el máximo configurado. El cambio se escucha mientras suena la pista. |
| Perilla de tono | A la izquierda, más grave. Al centro, tono original. A la derecha, más agudo. Se puede ajustar antes de pulsar cualquier botón y se aplica a la pista que empieza. |
| Tres botones juntos | Reproducen la grabación sorpresa una vez, sin avanzar las secuencias. Para activarla de nuevo hay que soltar los tres botones. |

---

### Pseudocódigo

```text
INICIO

  El soundboard reproduce nueve grabaciones propias.

  Hay tres botones y cada botón corresponde a una categoría con tres grabaciones.

  Cada grabación tiene tres versiones: grave, original y aguda.
  Una perilla regula el volumen y la otra elige la versión del sonido.

  Guardar el número de grabación que esté y avanzar al siguiente en la categoría.
  Al encender, las tres categorías empiezan en la grabación 1.
  Mantener el parlante en silencio hasta recibir una pulsación.

  REPETIR MIENTRAS LA CONSOLA ESTÁ ENCENDIDA

    Leer la perilla de volumen y ajustar la salida.
    El mínimo deja el parlante en silencio, incluso durante una pista.

    Detectar nuevas pulsaciones de los botones.

    SI se presionan los tres botones juntos
      Interrumpir la pista actual, si hay una en reproducción.
      Reproducir la grabación sorpresa.
      Conservar el número de la próxima grabación de cada categoría.
      Esperar a que se suelten los tres botones para aceptar otra pulsación.

    SI se registra una pulsación de un solo botón
      Identificar la categoría de ese botón.
      Consultar qué grabación sigue en esa categoría.
      Leer la perilla de tono y elegir grave, original o aguda.
      Interrumpir la pista actual, si hay una en reproducción.
      Dejar una pausa breve e iniciar la grabación elegida desde el comienzo.
      Avanzar la secuencia de esa categoría: 1, 2, 3 y nuevamente 1.

    SI termina la reproducción
      Volver a la espera silenciosa.

  FIN REPETIR

FIN
```

---

### BOM (lista de componentes)

| Componente | Cantidad | Precio | Link |
| --- | --- | --- | --- |
| Raspberry Pi Pico 2 W (con headers) | 1 | $14.990 | https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers |
| Reproductor de audio (DFPlayer Mini) | 1 | $2.990 | https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0 |
| Salida de audio (altavoz 3W, 8 Ohm) | 1 | $3.000 | https://afel.cl/products/mini-parlante-altavoz-de-3w |
| Protoboard | 1 | $1.500 | https://afel.cl/products/mini-protoboard-400-puntos |
| Cables dupont, pack 20 (macho-macho) | por definir | $1.000 | https://afel.cl/products/pack-20-cables-de-conexion-macho-macho |
| Kit 10 cables caimán a dupont hembra | por definir | $2.500 | https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra |
| Pack 20 cables de conexión macho-hembra | por definir | $1.000 | https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra |
| Potenciómetro B100k | 2 | $500 | https://afel.cl/products/potenciometro-10k-ohm |
| Cable micro USB | 1 | por definir | https://www.masterg.cl/mgcamic/p?idsku=6 |
| Almacenamiento (tarjeta microSD) | 1 | por definir | por definir |
| Botones | 3 | $300 | https://afel.cl/products/boton-tactil-tapa-12x12x7-3-interruptor |

---

## encargos

## lectura

- **Qué entendí:** En estas últimas hojas del libro se ve cómo John Cage va cerrando su planteamiento sobre la liberación, la organización colectiva y la ruptura de las estructuras rígidas o autoritarias. En los capitulos 19 y 20 se retoman ideas sobre la acción consciente, el vínculo con la naturaleza, la libertad individual y la idea de construir una sociedad donde las cosas no se impongan desde arriba ni se reduzcan a meras posesiones o control. El texto sigue jugando con esa disposición dispersa y fragmentada que mezcla palabras y mayúsculas irregulares, hasta llegar a la página final con los datos de catalogación del libro de John Cage (*Anarchy*, 1st Wesleyan ed., 2001).

- **Cita interesante:** *"laws of our own individual nature"* (p. 20)[cite: 8].

- **Reflexión personal:** Elegí esta cita porque me llamó la atención cómo cierra el recorrido pensando en vivir o guiarnos desde nuestra propia naturaleza y no desde imposiciones externas que nos aprietan o nos limitan. Para ser sincera, el libro en general tampoco me encantó del todo porque su formato fragmentado y su forma de plantear las ideas a veces se sienten súper abstractas y cuesta enganchar de una, pero sí me pareció muy interesante esta manera de mirar lo que es un sistema complejo y de buscar soluciones bien radicales frente a estructuras que ya no dan más. Destacar esta frase también sirve para conectar con lo que me mueve desde mis ideas de izquierda y mi interés por lo colectivo: pensar que los cambios profundos pasan por cuestionar el orden impuesto, mirar los sistemas desde la raíz y imaginar formas de organizarnos y existir que sean mucho más libres y reales.
