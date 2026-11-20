
# Proyecto-03

**grupo-08**

*Integrantes*

☆ Vanessa García / [*vanessagarciaM*](https://github.com/vanessagarciaM)

☆ Isidora Pérez / [*isipm08*](https://github.com/isipm08)

☆ Narely Riquelme / [*Narelyriquelme*](https://github.com/Narelyriquelme)

☆ Angel Sabogal / [*angel-udp*](https://github.com/angel-udp)

# *9dB*

## sobre el proyecto

```9dB``` es una cápsula sonora interactiva diseñado para poner en valor nuestro paisaje auditivo cotidiano de la vida universitario como estudiantes de diseño, mediante una interfaz física con 3 botones, los cuales son disparadores de 3 sonidos de cada temática y 2 potenciómetros, los cuales regulan volumen y tono, traduciendo y transformando los sonidos y ruidos cotidianos que solemos normalizar y escuchar diariamente en un archivo vivo del proceso creativo y en una experiencia táctil, sonora y nostálgica de la memoria universitaria. Nos basamos en 3 instancias claves dentro de la universidad [pañol - break - casino]  

El nombre ```9dB``` se debe a 3 sonidos y 3 botones [9 registros auditivos], los cuales representan la matriz sonora de nuestro proyecto, dB [decibeles] alude a la medición física del sonido y la ergonomía auditiva. En nuestro entorno que nos basamos, los decibeles simbolizan la frontera entre ruido ambiental que solemos ignorar y el sonido intencional que rescatamos al presionar un botón.

La consola estará en una espera silenciosa hasta que se pulse un botón. Si se presiona otro botón, o se vuelve a presionar el mismo mientras suena una pista, la reproducción se corta y la siguiente comienza desde el principio después de una pausa breve de aproximadamente medio milisegundo. Al terminar, la consola vuelve a quedar en silencio.

También se contempla una décima grabación sorpresa, integrada por el grupo, y se activa al presionar los tres botones juntos.

## funcionamiento

| Control | Función definida |
| --------- | ------------------ |
| **Botón 1** | Reproduce las tres grabaciones de la categoría 1 en orden: 1, 2, 3 y 1. |
| **Botón 2** | Reproduce las tres grabaciones de la categoría 2 con su propia secuencia. |
| **Botón 3** | Reproduce las tres grabaciones de la categoría 3 con su propia secuencia. |
| **Perilla de volumen** | A la izquierda, silencio. Al girar hacia la derecha, el volumen aumenta progresivamente hasta el máximo configurado. El cambio se escucha mientras suena la pista. |
| **Perilla de tono** | A la izquierda, más grave. Al centro, tono original. A la derecha, más agudo. Se puede ajustar antes de pulsar cualquier botón y el ajuste se aplica a la pista que empieza. |
| **Tres botones juntos** | Reproducen la grabación sorpresa una vez, sin avanzar las secuencias. Para volver a activarla, se deben soltar los tres botones. |

## ejemplo de sonidos a utilizar

- [lijadora](imagenes/lijadora.mp3)
- [cierra-huincha](imagenes/cierra-huincha.mp3)

## diagrama

![imagen](imagenes/diagramaa.jpeg)

## esquematico

![imagen](imagenes/esquematico-proyecto03.png)

## pseudo - código

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

## wokwi

<https://wokwi.com/projects/480948910221458433>

## bom

|Componente|Cantidad|Precio|Link|
|---|---|---|---|
|Raspberry Pi Pico|1|$14.990|<https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers>|
|Reproductor de Audio (DFPlayer Mini)|1|$2.990|<https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0>|
|Salida de Audio (Altavoz 3W, 8 Ohm)|1|$3.000|<https://afel.cl/products/mini-parlante-altavoz-de-3w>|
|Protoboard|1|$1.500|<https://afel.cl/products/mini-protoboard-400-puntos>|
|Cables dupont pack 20|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-macho>|
|Kit 10 Cables Caiman a Dupont Hembra|x|$2.500|<https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra?_pos=2&_sid=aec538813&_ss=r>|
|Pack 20 Cables de Conexión Macho Hembra|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra?_pos=1&_sid=bff72fae6&_ss=r>
|Potenciómetro B100k|2|$500|<https://afel.cl/products/potenciometro-10k-ohm>|
|Cable micro usb|1|$x|<https://www.masterg.cl/mgcamic/p?idsku=6>|
|Almacenamiento (Tarjeta MicroSD)|1|$x|[pendiente]|
|Botones|3|$300|<https://afel.cl/products/boton-tactil-tapa-12x12x7-3-interruptor?_pos=1&_sid=2ce280b32&_ss=r>|

## referentes

### Sound Diaries

Sound Diaries es un proyecto que se dedica a explorar y registrar los sonidos de la vida cotidiana, como objetos, acciones y espacios que muchas veces pasan desapercibidos. Busca reconocer el valor cultural y significativo de estos sonidos que pueden parecer insignificantes. Este referente nos inspira a explorar y grabar sonidos cotidianos para utilizarlos como material en la creación de nuestra propia colección sonora.

Sound Diares. (s.f). About Sound Diares. <https://www.sound-diaries.co.uk/>

### Freesound

Freesound es una plataforma colaborativa que funciona como una biblioteca de sonidos, donde las personas pueden compartir, buscar y descargar grabaciones de distintas fuentes. Los audios se organizan mediante etiquetas y descripciones que facilitan su búsqueda. Este referente nos entrega una idea de cómo organizar nuestros sonidos por categorías y construir una biblioteca sonora interactiva que permita descubrir e identificar cada grabación.

Freesound. (s.f). Frequently asked questions. <https://freesound.org/help/faq/#what-is-this-site-anyway>

### World Listening Project

World Listening Project es una iniciativa que promueve la escucha activa y la valoración del entorno sonoro. Busca generar conciencia sobre los sonidos que nos rodean y cómo estos forman parte de nuestra relación con los espacios y la vida cotidiana. Este referente nos inspira a invitar a las personas a descubrir sonidos ambientales y cotidianos, prestando atención a sus diferencias, variaciones y características a través de la escucha.

World Listening Project. (s.f). About us. <https://www.worldlisteningproject.org/>

### Adafruit Audio FX Sound Board

Adafruit Audio FX Sound Board es un dispositivo que permite reproducir sonidos mediante botones físicos. Incluye una función para cambiar al siguiente sonido con cada pulsación y volver al primero cuando termina la secuencia.

Lady Ada. (2014, 23 de septiembre). Adafruit Audio FX Sound Board. Adafruit Learning System. <https://learn.adafruit.com/adafruit-audio-fx-sound-board>

### Mixkit

Es una biblioteca de sonidos gratuitos, como pasos, puertas, campanas, ambientes escolares y lugares públicos. No requiere registro ni atribución para los efectos cubiertos por su licencia gratuita

Mixkit. (s. f.). Free sound effects. <https://mixkit.co/free-sound-effects/>

### Teenage Engineering PO-33 K.O!

El PO-33 K.O! es un micro sampler de bolsillo que asigna audios a botones individuales para dispararlos al instante e incorpora perillas para alterar su tono y volumen. Este referente nos sirve a nivel funcional como modelo de interacción directa, inspirándonos en la dinámica de presionar un botón para reproducir un sonido guardado y modificar su tono o volumen de forma inmediata.

Teenage Engineering. (s.f.). PO-33 K.O! guide. <https://teenage.engineering/guides/po-33/en>

### Roland AIRA Compact P-6

El Roland AIRA Compact P-6 es un sampler compacto diseñado para organizar muestras precargadas en pads táctiles y manipular su reproducción mediante controles giratorios. Este referente aporta a la lógica funcional de nuestro proyecto, mostrándonos cómo estructurar un sistema donde los sonidos almacenados se activan por botones y se transforman sonoramente al instante con perillas.

Roland. (s.f.). P-6 Creative Sampler. <https://www.roland.com/es-es/products/p-6/>

### Korg Volca Sample

El Korg Volca Sample es un reproductor de muestras compacto con memoria interna de sonidos precargados, botones para activar cada pista y perillas para controlar su tono y volumen. Este referente es clave porque pudimos tocarlo e interactuar con él en clases, comprobando en la práctica la respuesta táctil de un sistema que reproduce audios ya guardados sin necesidad de grabar en el momento.

Korg. (s.f.). volca sample - Digital Sample Sequencer. <https://www.korg.com/cl/products/dj/volca_sample/index.php>

### Adafruit Audio FX Sound Board

Adafruit Audio FX Sound Board es un dispositivo que permite reproducir sonidos mediante botones físicos. Incluye una función para cambiar al siguiente sonido con cada pulsación y volver al primero cuando termina la secuencia.

Lady Ada. (2014, 23 de septiembre). Adafruit Audio FX Sound Board. Adafruit Learning System. <https://learn.adafruit.com/adafruit-audio-fx-sound-board>

### Rock-ola
Uno de nuestros referentes en el ámbito de carcasa serían las rockcolas, inspirarnos en su evolución en el tiempo y cómo estas se han mantenido a lo largo de los años, realizar una carcasa representativa para estas, colores, iluminación y botones. Donde sea intuitivo el toque de botones y perillas.

Wikipedia contributors. (2026, 9 junio). Rock-Ola. Wikipedia. <https://en.wikipedia.org/wiki/Rock-Ola>

### Máquina de arcade
Nuestro segundo referente en el ámbito de carcasa serían las máquinas de arcade, las cuales visualmente generan interactividad, esa sensación de presionar botones y descubrir lo que sucede, contar con perillas y botones reguladores dentro de nuestra interacción.

colaboradores de Wikipedia. (2026b, septiembre 25). Máquina de arcade. Wikipedia, la Enciclopedia Libre. <https://es.wikipedia.org/wiki/M%C3%A1quina_de_arcade>
