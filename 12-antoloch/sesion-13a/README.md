# sesion-13a

## apuntes sesión
Brainstorming

# Definición del proyecto

## Lo que sí queremos

- Tinder de sonido: conectar dispositivos a través del sonido.
- Acercarse al dispositivo para poder escuchar su sonido.
- Transmisión de sonido mediante Bluetooth.
- Máquina que permita enviar sonidos a otro dispositivo.
- Interacción mediante botones (pulsar para activar).
- Mínimo 2 personas para que exista interacción.
- Si no hay alguien en el lugar B, desde el lugar A no se podrán enviar señales.
- Funcionamiento dentro de parámetros preestablecidos.
- Incorporar una cámara.
- Considerar alimentación eléctrica.
- Incorporar un parlante de tamaño mediano.
- Los sonidos de A y B se fusionan, creando una composición sonora compartida.
- Ambos dispositivos tendrán características propias y sonidos particulares.

## Lo que no queremos

- Alarma de auto.
- Alarma de incendios.
- Despertador.
- Madera.
- Memoria o almacenamiento de sonidos.
- Detector de metales.
- Parlante pequeño.
- Radio.
- Máquina de arcade.
- Máquina de pago.



podría ser
cartón
plástico

## Texto descriptivo
La propuesta consiste en dos objetos individuales, cada uno con un micrófono, que permiten la interacción entre dos personas. Cada una puede grabar o capturar un sonido propio, ya sean palabras, voces o sonidos realizados con el cuerpo. Luego, pueden experimentar modificando la velocidad de sus sonidos, reproduciéndolos de forma rápida, normal o lenta.
Una vez que ambas personas hayan experimentado con sus sonidos, estos se fusionan dentro de la máquina, generando una nueva composición sonora que se transforma visualmente mediante figuras de Lissajous. Estas formas cambian dependiendo de las frecuencias y de la relación entre ambos sonidos, creando una figura única que representa el encuentro de las dos personas.
La idea es que cada persona aporte un sonido distinto y que, al juntarse, se genere algo nuevo que ninguna podría producir por separado. Todo sucede en tiempo real, sin almacenar los sonidos, convirtiendo la interacción en una experiencia física, visual y sonora.
Bom
# BOM 

| Cantidad | Componente | Descripción |
|---:|---|---|
| 1 | Raspberry Pi Pico | Microcontrolador principal |
| 1 | Micrófono MAX4466 | Captura y amplificación de sonido. [Ver producto](https://afel.cl/products/modulo-microfono-amplificador-max4466?_pos=1&_sid=604d93d7d&_ss=r) |
| 4 | Potenciómetros | Control de parámetros |
| 2 | Botones | Activación y control de funciones |
| 1 | Láser | Emisión de luz láser |
| 1 | parlante | |

# Diagrama de flujo

Captura el sonido que la persona realiza frente al micrófono
Transforma (debemos verlo) darle características al sonido como frecuencia y amplitud
Encuentro fusión de las señales con sus características ya establecidas que ambas generan una nueva composición visual y sonora
Reproducción se transmite la señal y visualizamos como estas conversan

inicio → dos personas → definir características de frecuencia y amplitud → perturban el micrófono → visualizar las figuras creadas desde lo hablado

-------------------------------------------
## Descripción proyecto 

Una máquina que intercambie sonidos con la posibilidad de hacer visible el sonido, observar el comportamiento de lo que estamos realizando en este momento, sin memoria, situaciones genuinas del cuerpo aprendiendo, descubriendo objetos y sobre experiencias nuevas para convertirlo en una experiencia física y visual. Se plantea como una interacción entre dos personas, donde cada una pueda transmitir sonidos, observar cómo estos se transforman y experimentar una comunicación a través del sonido, el movimiento y las formas.
Tenemos 3 estados humanos que esperamos que es:
Escuchar para conseguir y reproducir sonidos.
Visualizar observar el comportamiento de nuestras palabras mediante formas.
Sentir experimentar el movimiento y ver cómo se comparte con otras personas y cómo actúa con otras personas.
TRANSMITIR EL SONIDO, NO GUARDARLO.
La propuesta consiste en dos objetos individuales, cada uno con un micrófono, que permiten la interacción entre dos personas. Cada una puede grabar o capturar un sonido propio, ya sean palabras, voces o sonidos realizados con el cuerpo. Luego, pueden experimentar modificando la velocidad de sus sonidos, reproduciéndolos de forma rápida, normal o lenta.
Una vez que ambas personas hayan experimentado con sus sonidos, estos se fusionan dentro de la máquina, generando una nueva composición sonora que se transforma visualmente mediante figuras de Lissajous. Estas formas cambian dependiendo de las frecuencias y de la relación entre ambos sonidos, creando una figura única que representa el encuentro de las dos personas.
La idea es que cada persona aporte un sonido distinto y que, al juntarse, se genere algo nuevo que ninguna podría producir por separado. Todo sucede en tiempo real, sin almacenar los sonidos, convirtiendo la interacción en una experiencia física, visual y sonora.

## encargos

## lectura
