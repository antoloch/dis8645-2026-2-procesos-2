# sesion-14a

### apuntes sesión

durante esta sesión continuamos trabajando en el proyecto 03 y avanzamos en la definición de su funcionamiento. se mantuvo la idea de crear un muestrario de sonidos, pero se definió que las grabaciones serán realizadas por el propio equipo, permitiendo seleccionar sonidos específicos del entorno universitario y trabajar con ellos de manera más detallada.

también se estableció la organización de los nueve sonidos en tres categorías, cada una controlada por un botón y con una secuencia de reproducción independiente. se mantuvieron las dos perillas para controlar el volumen y modificar el tono de las grabaciones, además de la función adicional de una décima pista sorpresa que se activa al presionar los tres botones juntos.

por último, se continuó definiendo la implementación del dispositivo mediante la raspberry pi pico 2 w y el módulo dfplayer mini, considerando los componentes necesarios para su construcción y la simulación de su funcionamiento en wokwi.

### descripción del proyecto

el proyecto consiste en desarrollar un muestrario interactivo de sonidos cotidianos, compuesto por nueve grabaciones realizadas por el propio equipo. la idea es crear una consola que permita explorar y reconocer distintos sonidos del entorno a través de una interacción sencilla.

el dispositivo contará con tres botones físicos, cada uno asociado a una categoría de sonidos. las categorías serán definidas por el grupo y podrán incluir elementos, objetos, acciones o ambientes específicos de la universidad y otros espacios cotidianos.

cada categoría tendrá asignadas tres grabaciones diferentes. al presionar un botón, se reproducirá el primer sonido correspondiente y, con cada nueva pulsación, se avanzará al siguiente, siguiendo un orden de tres pistas que volverá a comenzar desde la primera. cada botón mantendrá su propia secuencia de reproducción.

la consola contará con un parlante y dos perillas. una permitirá regular el volumen, desde el silencio hasta el máximo configurado, mientras que la otra modificará el tono de los sonidos, permitiendo pasar de una frecuencia más grave a una más aguda, con el tono original en la posición central.

el sistema permanecerá en silencio hasta que se presione alguno de los botones. si se activa otro botón durante una reproducción, o se vuelve a presionar el mismo, el sonido actual se interrumpirá y la nueva pista comenzará desde el principio después de una pausa breve.

cuando termine una reproducción, el dispositivo volverá automáticamente al estado de espera silenciosa.

como función adicional, se incorporará una décima grabación sorpresa, creada por el grupo, que se activará al presionar los tres botones simultáneamente. esta función permitirá reproducir un sonido distinto sin alterar las secuencias de las categorías principales.

para su construcción se utilizará una raspberry pi pico 2 w, un módulo reproductor de audio dfplayer mini, un parlante, tres botones físicos, dos potenciómetros y una tarjeta microsd para almacenar las grabaciones. los componentes estarán integrados en una carcasa con etiquetas o íconos que permitan identificar las categorías y facilitar su uso.

### pseudocódigo 
```
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
### bom
|Componente|Cantidad|Precio|Link|
|---|---|---|---|
|Raspberry Pi Pico|1|$14.990|<https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers>|
|Reproductor de Audio (DFPlayer Mini)| 1 |$2.990|<https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0>|
|Salida de Audio (Altavoz 3W, 8 Ohm)| 1 |$3.000|<https://afel.cl/products/mini-parlante-altavoz-de-3w>|
|Protoboard|1|$1.500|<https://afel.cl/products/mini-protoboard-400-puntos>|
|Cables dupont pack 20|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-macho>|
|Kit 10 Cables Caiman a Dupont Hembra|x|$2.500|<https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra?_pos=2&_sid=aec538813&_ss=r>|
|Pack 20 Cables de Conexión Macho Hembra|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra?_pos=1&_sid=bff72fae6&_ss=r>
|Potenciómetro B100k|2|$500|<https://afel.cl/products/potenciometro-10k-ohm>|
|Cable micro usb |1| $x|<https://www.masterg.cl/mgcamic/p?idsku=6>|
|Almacenamiento (Tarjeta MicroSD) | 1 | $x | x |
|Botones| 3 | $300 | <https://afel.cl/products/boton-tactil-tapa-12x12x7-3-interruptor?_pos=1&_sid=2ce280b32&_ss=r> |

## lectura



