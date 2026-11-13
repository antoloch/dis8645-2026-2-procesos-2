# sesion-12b

## apuntes sesión

## retroalimentación Proyecto 02

Durante la clase revisamos los proyectos 02 y recibimos comentarios para mejorar las próximas entregas.

Misa habló sobre la importancia de comenzar a citar correctamente desde ahora, porque más adelante, en Seminario SOS, puede ser mucho más complicado ordenar todas las referencias si no tenemos ese hábito.

Se mencionó la importancia de:
- Utilizar un marco teórico y referencias para respaldar nuestras ideas.
- Dar crédito y citar correctamente los materiales utilizados.
- Argumentar y explicar las decisiones del proyecto.
- Comenzar a utilizar **Zotero** para organizar las referencias.

## inicio Proyecto 03

También conversamos sobre las dudas, lo que nos ha gustado y lo que no, además de revisar los requisitos y las posibilidades del nuevo proyecto.

- Los grupos serán de 4 personas.
- Cada clase tendrá mini entregas que serán evaluadas.
- En vez de tener más clases teóricas, se harán pequeñas cápsulas.
- El proyecto debe tener un parlante.
- La pantalla es opcional.
- Se pueden crear máquinas que produzcan sonidos.
- Se puede incorporar un micrófono para grabar sonidos y reproducirlos.
- Si se utiliza un parlante, no es necesario agregar un buzzer.

## encargos

### Tratado de los objetos musicales de Pierre Schaeffer

Muchas veces escuchamos los sonidos sin prestarles demasiada atención, porque estamos acostumbrados a ellos. Schaeffer propone escuchar de otra manera, fijarse en cómo suenan las cosas y no solamente en reconocer qué objeto está produciendo el sonido. Un mismo sonido puede tener distintas características que normalmente no notamos porque no nos detenemos a escucharlo.

### Nunca vi un sonido de R. Murray Schafer

En esta lectura me llamó la atención la idea de que los sonidos también forman parte de los lugares y de nuestras experiencias. Hay sonidos que escuchamos todos los días y que ya son parte de nuestra rutina, como los autos, las conversaciones o la lluvia. También creo que los sonidos pueden hacernos recordar momentos o personas, e incluso hacernos sentir distintas emociones dependiendo de la situación.

### ¿Cómo suena la tristeza?

Creo que la tristeza podría sonar como la lluvia o el viento golpeando una ventana, una canción que se escucha a lo lejos o incluso el silencio de una pieza. Son sonidos suaves, lentos y un poco apagados. Aparecen principalmente en momentos en que uno está solo o pensando en cosas que han pasado. ¿Cómo un sonido tan cotidiano puede transmitir una emoción? Es algo que me parece muy raro, pero a la vez interesante. Culturalmente, la lluvia y las canciones lentas suelen relacionarse con la tristeza, la nostalgia y los recuerdos. También pienso que un mismo sonido puede sentirse distinto dependiendo de cómo estemos emocionalmente en ese momento.


### Tristan Perich – Microtonal Wall (2011)

Está formada por muchos parlantes pequeños que producen sonidos al mismo tiempo. Al escucharla, se siente como un zumbido que va cambiando dependiendo de dónde uno esté. Me parece curioso que algo tan simple pueda crear una experiencia sonora diferente. También me llamó la atención que el sonido no sea una melodía como la que estamos acostumbrados a escuchar, sino que sean muchos sonidos mezclados.

### Christine Sun Kim – Face Opera II (2013)

Esta obra me pareció interesante porque habla sobre la comunicación y las distintas formas que tenemos de expresarnos. Utiliza textos y lenguaje de señas para mostrar que no todos nos comunicamos de la misma manera. Muchas veces relacionamos el sonido solamente con escuchar o hablar, pero también existen otras formas de comunicarse. Creo que es interesante cómo la artista logra mostrar algo que normalmente no nos detenemos a pensar.

# Revisión: amplificador y parlante (grupo-01, proyecto 2)

 Amplificador

Usan un **MAX98357A**, un amplificador clase D con DAC I2S integrado. El audio sale digital desde la Pico, sin DAC externo ni filtros analógicos.

# Revisión: amplificador y parlante (grupo-01, proyecto 2)

## Amplificador

Usan un **MAX98357A**, un amplificador clase D con DAC I2S integrado. El audio sale digital desde la Pico, sin DAC externo ni filtros analógicos.

## Parlante

- `LS1` es un parlante **pasivo** (lo indican con una nota).
- Va directo a los terminales de salida del amplificador, con `+` y `-` marcados, sin componentes intermedios.

## Control de volumen

- No se hace en el amplificador sino **por software**.
- Un potenciómetro lineal (`RV1`) entra por GPIO26 (ADC0) y la Pico escala las muestras de audio.
- Un capacitor de 100 nF (`C1`) estabiliza la lectura del ADC.

