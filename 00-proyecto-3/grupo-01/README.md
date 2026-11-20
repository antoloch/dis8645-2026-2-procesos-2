# grupo-01

## integrantes

- [19 - Dayana Pañitrur](https://www.github.com/dayanapanitrur)
- [23 - Camila Ramírez](https://www.github.com/Estrabismx)
- [29 - Bianka Vilchez](https://www.github.com/biankavilchez)

## sobre el proyecto

Nuestra máquina es un instrumento que no es musical ¿o sí? sin la intervención de quien la usa no produce nada, pero lo que resulta sea música o ruido depende de quien lo escucha. Nuestro proyecto recorre 3 etapas de interacción: captura, edición y reproducción del sonido. En la fase de captura, se incorpora un botón y un micrófono el cuál está articulado para que la persona pueda grabar su voz, disponiendo de un máximo de 30 segundos por grabación.

En la etapa de edición, cada grabación utiliza una lógica de edición irreversible. El dispositivo tiene una sección física de edición con efectos y modulaciones, controlables con deslizadores, perillas y botones. Al terminar una grabación, esta queda repitiéndose en bucle disponible para ser editada y solo a esta se le podrán aplicar los efectos. Una vez que ya se hayan hecho los cambios y almacenado a la mezcla de sonidos, ya no se podrán realizar más modificaciones.

En la etapa de reproducción las grabaciones se acumulan en una mezcla de unos diez minutos, dividida en 60 puntos de inicio, uno cada 10 segundos. Cada nueva grabación se ubica en uno de esos puntos y como puede durar hasta 30 segundos, se superpone con las que suenan cerca de ella. La lógica irreversible que se incorpora para cada audio individual, también aplica para la mezcla de sonidos, de esta manera solo se puede reproducir desde el principio o detener por completo: no se puede pausar ni avanzar a un punto específico. Tampoco se puede borrar. Una vez que los 60 puntos de inicio hayan sido totalmente ocupados, la máquina ya no permite grabar más, solo reproducir la mezcla final. El sonido sale por un altavoz con una perilla de volumen, y una pantalla muestra una abstracción gráfica de cuán saturada está la mezcla.

Dentro del funcionamiento interno, este dispositivo es controlado por una Raspberry Pi Pico 2 W, la cual se encarga de gestionar las entradas y salidas de la interfaz física correspondientes a las conexiones con perillas, sliders, botones, leds, altavoz y pantalla, y también gestiona el procesamiento del sonido y su manipulación. Para asegurar un rendimiento continuo y evitar las limitaciones de memoria de la Raspi al trabajar con audios muy largos o con muchas capas, el sistema integra un módulo de tarjeta SD, que permite almacenar y reproducir el conjunto de sonidos grabados de manera fluida.

### Interfaz

El dispositivo presenta una interfaz organizada en tres zonas principales: reproducción, grabación y edición, dispuestas de izquierda a derecha para establecer un recorrido visual y facilitar la identificación de sus funciones.

![](./imagenes/interfaz-coreografia.png)

| Zona | Elemento | Función |
|---|---|---|
| **Reproducción** | Parlante | Canal de salida por donde se reproduce el audio. |
| | Pantalla | Visualiza la información de los audios almacenados. |
| | Botón reproducción / Detener | Permite iniciar o detener la reproducción de los audios registrados. |
| | Perilla de volumen | Regula el volumen general de reproducción. |
| **Grabación** | Micrófono | Medio de entrada por donde se captura el sonido. |
| | Botón GRABAR | Activa la captura de sonido durante 10 segundos. |
| | Pantalla | Muestra la cuenta regresiva de grabación. |
| **Edición** | 9 botones de efectos | Permiten seleccionar los nueve efectos disponibles. |
| | 3 perillas | Una por sección, permiten modificar los parámetros de los efectos seleccionados. |
| | 9 luces LED | Indican el estado de cada efecto: disponible, en edición o aplicado. |
| | Control de volumen | Regula el volumen específico del audio grabado. |
| | Botón guardar | Almacena el audio modificado junto con los registros anteriores. |

#### Coreografía

##### 01. Encendido del dispositivo

- Activar el interruptor tipo *switch* para iniciar el sistema. La pantalla muestra el nombre del proyecto como presentación inicial.

##### 02. Zona de reproducción

- Al encender el dispositivo, la pantalla muestra la información de los registros sonoros almacenados, incluyendo la cantidad de audios y sus representaciones gráficas.
- Presionar el botón de reproducción para escuchar los audios acumulados.
- Presionar nuevamente el botón de reproducción para detener
- Girar la perilla de volumen para ajustar la intensidad sonora de la reproducción.

##### 03. Zona de grabación

- Presionar una vez el botón grabar para activar el micrófono e iniciar la captura de un nuevo sonido.
- La pantalla muestra una cuenta regresiva de 10 segundos, durante los cuales el micrófono captura el sonido del entorno.
- Al finalizar la cuenta regresiva, el registro queda almacenado temporalmente y disponible para su posterior edición.

##### 04. zona de edición

- Una vez finalizada la grabación, se habilitan los controles de edición para intervenir el nuevo audio.
- Seleccionar uno de los nueve efectos disponibles, distribuidos en tres secciones.
- Girar la perilla asociada a la sección correspondiente para modificar el parámetro del efecto y escuchar la transformación del sonido.
- Repetir el procedimiento con otros efectos para acumular progresivamente modificaciones sobre el audio.
- Ajustar el volumen específico del audio grabado mediante el control correspondiente.
- Una vez finalizada la edición, presionar el botón GUARDAR para almacenar el audio intervenido.
- El dispositivo incorpora el audio modificado junto con los audios anteriores, dejándolo disponible para su reproducción.

#### Pantalla 

##### **1. zona de reproducción**

La pantalla presenta una visualización de píxeles luminosos que se van iluminando progresivamente a medida que se guardan nuevos audios, representando visualmente la acumulación de registros.

En la parte superior se visualiza un contador numérico con la cantidad de audios guardados y la memoria disponible del dispositivo.

##### **2. zona de grabación**

Al iniciar la grabación, la pantalla muestra una cuenta regresiva de 10 a 0 segundos. En la parte superior, una barra de progreso disminuye progresivamente para indicar el tiempo restante de captura.

Al finalizar la grabación, la pantalla vuelve a mostrar la visualización general de los audios acumulados.

##### **3. zona de edición**

Durante la edición, la pantalla mantiene visible la visualización de los píxeles correspondientes a los audios almacenados.

Una vez finalizada la intervención y presionado el botón *guardar*, se incorpora una nueva agrupación de píxeles a la visualización general, actualizando el contador de audios acumulados.

![](./imagenes/pantalla.png)

#### Audio 

1. **Duración total**: el dispositivo cuenta con un ciclo de reproducción de 10 minutos, dividido en 60 segmentos de 10 segundos cada uno.

2. **Ingreso del audio**: cada nuevo audio grabado tiene una duración de 10 segundos.

3. **Distribución aleatoria**: al guardar un audio, el sistema le asigna aleatoriamente uno de los 60 segmentos disponibles. Esta distribución permite generar espacios vacíos entre los registros.

4. **Superposición**: si un segmento ya contiene uno o más audios, el nuevo registro puede incorporarse al mismo espacio, reproduciéndose simultáneamente con los anteriores.

5. **Espacios vacíos**: los segmentos que no contienen audios permanecen en silencio, generando pausas dentro de la composición sonora.

6. **Reproducción**: el dispositivo recorre los 60 segmentos durante los 10 minutos, reproduciendo los audios según su distribución y respetando los espacios de silencio.

7. **Loop**: al finalizar los 10 minutos, el ciclo vuelve a comenzar, conservando la distribución aleatoria de los audios almacenados.

![](./imagenes/audio.png)
   
#### Luces 

1. **Distribución:** el dispositivo cuenta con nueve luces LED, distribuidas en tres secciones de tres luces cada una. Cada luz corresponde a uno de los nueve efectos disponibles y está asociada a un botón.

2. **Estado inicial:** al encender el dispositivo, las nueve luces permanecen apagadas.

3. **Activación de la zona:** al finalizar la grabación, las nueve luces se encienden progresivamente, aumentan su brillo y luego se apagan, indicando que la zona de edición está disponible.

4. **Selección del efecto:** al presionar uno de los nueve botones, se activa la luz correspondiente y comienza a parpadear.

5. **Modificación del efecto:** mientras la luz parpadea, se modifica el sonido mediante la perilla asociada a la sección.

6. **Confirmación:** al presionar nuevamente el botón del efecto, la luz deja de parpadear y permanece encendida, indicando que la modificación fue aplicada.

7. **Selección de otro efecto:** al seleccionar otro botón, su luz comienza a parpadear, mientras las luces de los efectos anteriores permanecen encendidas de forma fija.

8. **Finalización:** al terminar la edición, las luces encendidas permiten identificar los efectos que fueron aplicados al audio.

![](./imagenes/luces.png)
   
<br>

#### Modificadores de sonido

| Categoría | Efecto | Descripción original | Descripción simplificada | Descripción coloquial |
| :--- | :--- | :--- | :--- | :--- |
| Envolventes | Attack | regula en cuánto tiempo tarda el sonido en llegar a su volumen máximo desde que empieza, le importa cuándo y cómo inicia, no importa cómo termina. | tiempo que demora el sonido en aparecer desde el silencio hasta su punto máximo. | si el sonido entra de golpe o despacio. |
| Envolventes | Release | regula en cuánto tiempo tarda el sonido en apagarse por completo desde que dejas de tocarlo, le importa cuándo y cómo termina, no cómo empezó. | tiempo que demora el sonido en desaparecer al soltarlo. | si el sonido se corta en seco o se va apagando de a poco. |
| FX / Modulación | Pitching | modifica la altura tonal del sonido para hacerlo más agudo o más grave sin cambiar necesariamente su duración, alterando directamente la afinación de la señal. | sube o baja qué tan agudo o grave suena el audio. | dejar el sonido más finito o más grueso. |
| FX / Modulación | Reverb | agrega rebotes y cola al sonido simulando las dimensiones de un espacio físico, cambiando la sensación de amplitud y distancia alrededor del audio. | simula el tamaño y el rebote del lugar donde suena el audio. | hacer que suene seco o con eco de un lugar grande. |
| FX / Modulación | Trémolo | hace que el volumen suba y baje de manera repetitiva siguiendo un ritmo constante, manteniendo el tono intacto mientras oscila solo su intensidad. | oscilación rápida y repetitiva del volumen. | hacer que el volumen suba y baje rápido todo el rato. |
| FX / Modulación | Bit crushing | reduce la resolución y la calidad digital del sonido, perdiendo detalles limpios para generar una textura áspera, granulada y sucia. | baja la calidad del audio hasta volverlo áspero y pixelado. | queda el sonido roto, sucio y con poca calidad. |
| FX / Modulación | Ring modulator | multiplica el sonido original por otra onda interna creando frecuencias nuevas y disonantes, haciendo que pierda su forma natural y tome un carácter metálico. | cruza el audio con otra señal para volverlo metálico y disonante. | vuelve el sonido metálico, robótico o raro. |
| FX / Modulación | Velocidad | controla qué tan rápido o lento se reproduce el audio en el tiempo, lo que al cambiar el ritmo también suele alterar qué tan agudo o grave se escucha. Al medio se puede escuchar a velocidad normal, luego dependiendo si el valor es mayor irá más rápido y si es menor irá más lento | acelera o frena el tiempo de reproducción del audio. | poner el audio más rápido o más lento. |
| Niveles / Ganancia | Volumen de audio | regula la intensidad de un sonido o pista individual antes de juntarse con el resto, y luego se integrará con ese nivel de volumen a la mezcla de sonidos | controla qué tan fuerte suena una sola pista. | subirle o bajarle el volumen a un solo sonido sin tocar el resto. |
| Niveles / Ganancia | Volumen general | regula la intensidad de salida de toda la mezcla, controlando el nivel final que escuchas al sumar todos los sonidos y efectos. | controla la fuerza final de todo el conjunto a la vez. | sube o baja el volumen general |

### Diagramas

![](./imagenes/dis8645-2026-2-esq.png)

![](./imagenes/dis8645-2026-2.png)

### Proceso

|   |   |   |   |
| --- | --- | --- | --- |
| <img src="./imagenes/drive/00.jpg" alt="00" width="100%"> | <img src="./imagenes/drive/01.jpg" alt="01" width="100%"> | <img src="./imagenes/drive/02.jpg" alt="02" width="100%"> | <img src="./imagenes/drive/03.jpg" alt="03" width="100%"> |
| <img src="./imagenes/drive/04.jpg" alt="04" width="100%"> | <img src="./imagenes/drive/05.jpg" alt="05" width="100%"> | <img src="./imagenes/drive/06.jpg" alt="06" width="100%"> | <img src="./imagenes/drive/07.jpg" alt="07" width="100%"> |
| <img src="./imagenes/drive/08.jpg" alt="08" width="100%"> | <img src="./imagenes/drive/09.jpg" alt="09" width="100%"> | <img src="./imagenes/drive/10.jpg" alt="10" width="100%"> | <img src="./imagenes/drive/11.jpg" alt="11" width="100%"> |
| <img src="./imagenes/drive/12.jpg" alt="12" width="100%"> | <img src="./imagenes/drive/13.jpg" alt="13" width="100%"> | <img src="./imagenes/drive/14.jpg" alt="14" width="100%"> | <img src="./imagenes/drive/15.jpg" alt="15" width="100%"> |
| <img src="./imagenes/drive/16.jpg" alt="16" width="100%"> | <img src="./imagenes/drive/17.jpg" alt="17" width="100%"> | <img src="./imagenes/drive/18.jpg" alt="18" width="100%"> | <img src="./imagenes/drive/19.jpg" alt="19" width="100%"> |
| <img src="./imagenes/drive/20.jpg" alt="20" width="100%"> | <img src="./imagenes/drive/21.jpg" alt="21" width="100%"> | <img src="./imagenes/drive/22.jpg" alt="22" width="100%"> | <img src="./imagenes/drive/23.jpg" alt="23" width="100%"> |
| <img src="./imagenes/drive/24.jpg" alt="24" width="100%"> | <img src="./imagenes/drive/25.jpg" alt="25" width="100%"> | <img src="./imagenes/drive/26.jpg" alt="26" width="100%"> | <img src="./imagenes/drive/27.jpg" alt="27" width="100%"> |
| <img src="./imagenes/drive/28.jpg" alt="28" width="100%"> | <img src="./imagenes/drive/29.jpg" alt="29" width="100%"> | <img src="./imagenes/drive/30.jpg" alt="30" width="100%"> | <img src="./imagenes/drive/31.jpg" alt="31" width="100%"> |
| <img src="./imagenes/drive/32.jpg" alt="32" width="100%"> | <img src="./imagenes/drive/33.jpg" alt="33" width="100%"> | <img src="./imagenes/drive/34.jpg" alt="34" width="100%"> | <img src="./imagenes/drive/35.jpg" alt="35" width="100%"> |
| <img src="./imagenes/drive/36.jpg" alt="36" width="100%"> | <img src="./imagenes/drive/37.jpg" alt="37" width="100%"> |   |   |


### BOM

| # | Item | Cant. | Valor | Link | Imagen |
| - | -------------------- | ----- | ------- | ---- | ------ |
| 1 | Raspberry Pi Pico 2W | 01 | $14.990 | <https://mcielectronics.cl/shop/product/raspberry-pi-pico-2-w-con-headers/> | <img src="./imagenes/pi-pico-2w.png" alt="" width="100%"> |
| 2 | Micrófono I2S INMP441 | 01 | $5.890<sup><b>1</b> | <https://www.mechatronicstore.cl/modulo-microfono-i2s-inmp441/> | <img src="./imagenes/microfono.jpg" alt="" width="100%"> |
| 3 | PCM5102A decodificador DAC | 01 | $1.122<sup><b>2</b> | <https://es.aliexpress.com/item/1005006104038963.html> | ![](./imagenes/pcm.avif) |
| 4 | Módulo PAM8302 | 01 | $720<sup><b>2</b> | https://es.aliexpress.com/item/1005007274382818.html | ![](./imagenes/amp.avif) |
| 5 | Pantalla LCD Oled Azul y Amarillo 0,96 | 01 | $4.500 | https://afel.cl/products/pantalla-lcd-oled-azul-y-amarillo-0-96 | ![](./imagenes/pantalla.jpg)
| 6 | Módulo Multiplexor CD74HC4067 | 02 | $2.000 | https://afel.cl/products/modulo-multiplexor-cd74hc4067 | ![](./imagenes/multi.webp) |
| 7 | Botones | 12 | $1.500 | https://afel.cl/products/kit-boton-tactil-6x6x5-interruptor-5-unidades | ![](./imagenes/botones.png) |
| 8 | Potenciometros | 2 | $2.330 | https://es.aliexpress.com/item/1005009244496972.html | ![](./imagenes/potenciometro.png) |
| 9 | Luces led | 9 | $1.138 |https://es.aliexpress.com/item/1005003144912658.html | ![](./imagenes/led.png) |
| 10 | Slider | 3 | $1.973 | https://es.aliexpress.com/item/1005010380785910.html | ![](./imagenes/slider.png) |
| 11 | 74HC595 | 2 | $4.000 | https://afel.cl/products/pack-5-chips-74hc595-shift-registers | ![](./imagenes/registro.png) |
| 12 | 74HC4051 | 1 | $2.300 | https://www.mechatronicstore.cl/multiplexor-analogico-cjmcu-4051-8-canales/ | ![](./imagenes/multiplexor2.png) |
| 13 | Lector SD Card | 1 | $1.000 | https://afel.cl/products/modulo-lector-tarjetas-sd | ![](./imagenes/lector.png) |
| 14 | Parlante | 1 | $3.000 | https://afel.cl/products/mini-parlante-altavoz-de-3w | ![](./imagenes/parlante.png) |
| 15 | PCB perforada | 4 | $3.153 | https://es.aliexpress.com/item/1005012773468288.html| ![](./imagenes/pcb.png) |
| 16 | Pin Header 1 x20 | 2 | $500 | https://afel.cl/products/regleta-macho-hembra-pin-header | ![](./imagenes/header.png) |

### pseudo - código

- Se establece que:
  - `sonido` es una grabación sin ninguna modificación. Es nuestro pez
  -  Al editar sus parámetros la entendemos como `audio`. Ahora nuestro pez es un pescado
  - Una vez que se guarda, se añade a los demás `audios` y forma un `conjuntoDeAudios`. Este es nuetro pescado cocinado, no se puede "_**des**cocinar_"
  -Todos los `audios` guardados se reproducen en nuestro `canvas`, es decir, nuestro lienzo que podemos reproducir en cualquier momento pero no editar. El equivalente a decir el tamño del plato

- Ahora se inicia la máquina con el `interruptorGeneral`, cuando esta apagada queda en estado suspensión y se bloquean todas las funciones

```cpp

bool interruptorGeneral; // valor asociado a la lectura del switch o botón 
bool funcionamientoMaquina; // depende del interruptorGeneral
// si es true la maquina realiza sus funciones con normalidad
// si es false, la maquina debe tener un led encendido que indica que recibe corriente

if (interruptorGeneral){
funcionamientoMaquina = 1;}

```

- Se consulta si hay `conjuntoDeAudios`

- En caso de haber se habilitan 2 modos y estos corresponden a `reproduccion` y `grabacion`

    - `reproduccion` permite escuchar el `conjuntoDeAudios`

        - Al activar esta opción, se inhabilta el modo grabar mientras dure la reproducción (los bótones dejar de ser leidos)

        - Posee 1 boton de `reproducción`, que si se presiona una vez inicia el proceso y para cancelarlo se presiona nuevamente para `detener`

        - La reproducción de `audios` es siempre en bucle
        
    - `grabacion` permite guardar sonidos de hasta un máximo de 30 segundos

        - Para grabar se presiona el boton asociado

        - para detener la grabacion antes de los 30s se vuelve a presionar el botón. Esto inicia automáticamente el modo de `edición`

        - Mientras esto ocurre se bloquea cualquier otro modo e inica la reproducción en bucle del sonido hasta que se guarde

        - Además los leds asociados a los efectos prenden por un corto periodo de tiempo, de manera que indican que esta sección se habilitó

        - Estos efectos de sonido se dividen en 3 secciones con 3 modificadores cada una

        - Cada sección posee su potenciómetro deslizable que modifica los valores asociados

        - Al seleccionar un efecto se mantiene oscilando el led

        - Para terminar con el efecto y guardar sus cambios se presiona el botón asociado o se selecciona otro efecto

        - Al terminar de editar se guarda el `audio` con su botón asignado y se deja de reproducir en bucle

        - Una vez guardado el `audio` se le asigna una posición "aleatoria" en la que va a sonar en el `conjuntoDeAudios`

- En caso de no haber `conjuntoDeAudios` se bloquean todas las funciones a exepción de `grabar`, la cual al iniciarse ocurre con normalidad tal como se describio anteriormente

- Si la máquina es apagada desde el `interruptorGeneral`, el `conjuntoDeAudios` debe quedar almancenado

#### Conexiones

[Esquematico](./esquematicos/v01/sin-nombre/sin-nombre.pdf)

| class | Lugar      | Nombre |   |
| ----- | ---------- | ------ | - |
| Boton | CD74HP4067 C0  | `fxAttack` | - |
| Boton | CD74HP4067 C1  | `fxRelease` | - |
| Boton | CD74HP4067 C2  | `fxHighPassF` | - |
| Boton | CD74HP4067 C3  | `fxPitching` | - |
| Boton | CD74HP4067 C4  | `fxReverb` | - |
| Boton | CD74HP4067 C5  | `fxTremolo` | - |
| Boton | CD74HP4067 C6  | `fxBitCrushing` | - |
| Boton | CD74HP4067 C7  | `fxRingModulator` | - |
| Boton | CD74HP4067 C8  | `fxVelocidad` | - |
| Boton | CD74HP4067 C9  | `guardarEdicion` | - |
| Boton | CD74HP4067 C10 | `botonGrabar` | - |
| Boton | CD74HP4067 C11 | `botonReproducir` | - |
| Boton | CD74HP4067 C12 | `SwitchEncendido` | - |
| MultiplexorCD74HP4067 | GPIO 14 | `botonesS0` | - |
| MultiplexorCD74HP4067 | GPIO 15 | `botonesS1` | - |
| MultiplexorCD74HP4067 | GPIO 10 | `botonesS2` | - |
| MultiplexorCD74HP4067 | GPIO 17 | `botonesS3` | - |
| MultiplexorCD74HP4067 | GPIO 18 | `botonesSignal` | - |
| MultiplexorCD74HP4067 | GND | `botonesEnable` | - |
| Lucecita | 74HC595 Q0  | `fxAttack` | - |
| Lucecita | 74HC595 Q1  | `fxRelease` | - |
| Lucecita | 74HC595 Q2  | `fxHighPassF` | - |
| Lucecita | 74HC595 Q3  | `fxPitching` | - |
| Lucecita | 74HC595 Q4  | `fxReverb` | - |
| Lucecita | 74HC595 Q5  | `fxTremolo` | - |
| Lucecita | 74HC595 Q6  | `fxBitCrushing` | - |
| Lucecita | 74HC595 Q7  | `fxRingModulator` | - |
| Lucecita | 74HC595 2do Q0  | `fxVelocidad` | - |
| Multiplexor74HC595 | GPIO 8 | `lucecitasSCHP` | - |
| Multiplexor74HC595 | GPIO 7 | `lucecitasSTCP` | - |
| Multiplexor74HC595 | GPIO 6 | `lucecitasDS` | - |
| Perilla | 74HC4051 Q0 | `volumenGeneral` | - |
| Perilla | 74HC4051 Q1 | `volumenEspecifico` | - |
| Perilla | 74HC4051 Q2  | `fx6al8` | - |
| Perilla | 74HC4051 Q3 | `fx3al5` | - |
| Perilla | 74HC4051 Q4 | `fx0al2` | - |
| Multiplexor74HC4051 | GPIO 26 | `perillaSignal` | - |
| Multiplexor74HC4051 | GPIO 13 | `perillaS0` | - |
| Multiplexor74HC4051 | GPIO 12 | `perillaS1` | - |
| Multiplexor74HC4051 | GPIO 11 | `perillaS2` | - |
| Pantalla | GPIO 4 | `pantallaSDA` | - |
| Pantalla | GPIO 5 | `pantallaSCL` | - |
| SDModulo | GPIO 9 | `sdCS` | - |
| SDModulo | GPIO 3 | `sdDI` | - |
| SDModulo | GPIO 2 | `sdCLK` | - |
| SDModulo | GPIO 16 | `sdDO` | - |
| INMP441 |  GPIO 19 | `micSD` | - |
| INMP441 | GPIO 28 | `micWS` | - |
| INMP441 | GPIO 27 | `micCLK` | - |
| INMP441 | GND | `micLR` | - |
| PCM5102A | GPIO20 | `ampliDIN` | - | 
| PCM5102A |GPIO 21 | `ampliBCK` | - |
| PCM5102A | GND | `ampliSCK` | - |

#### Modo Reproducir

Encargado de la reproducción del `conjuntoDeAudios`, utiliza 2 botones y posee una pantalla para visualizar desde la cantidad de audios que existen dentro del conjunto hasta la frecuencia con la que pueden ingresar cada audio. Los 2 botones son para la reproducción del conjunto y para detener la reproducción.

El elemento de la pantalla muestra [...]

`botonReproducir` es la instancia de la clase BotonGrupo1 y consiste de un push button, este lee y detecta cuando es presionado el botón, ignora doble click, cuanto tiempo se presiona (sin contar el _debounce_), una vez que inicia se debe utilizar `botonDetener`para detener el bucle

`botonDetener` es la instancia que detiene y cancela la reproducción

> revisar que tan necesario son 2 botones que hagan esto, ver que sea solo uno o un switch

#### Modo Grabar

Inicia presionando `botonGrabar`, el que solo detecta presionados y cambio de estados, es decir que se presiona una vez para iniciar y una segunda vez para cortar la grabación antes de 10 segundos (este corresponde al límite de duración que posee la grabación), posee un pequeño tiempo para que no se presione 2 veces y queden grabados de menos de 0.5s.

Una vez que se detiene la grabación con el segundo _click_ automáticamente se inicia la edición del sonido grabado

Se establece que el limite de audios guardados es 200

En este modo quedan bloqueados cualquier otra interacción que no sea grabar, además que si se presiona grabar durante la reproducción, se detiene está y se inicia el modo grabar. 

##### Modo Editar

Al entrar a esta sección el sonido grabado se reproduce en bucle para notar como los efectos interfieren en tiempo real, además el brillo de los leds subirá y disminuira una sola vez al inicio por 1.5s para indicar que es esta la sección en la que se debe trabajar. Para seleccionar alguno de estos efectos se seleccionará cualquier botón asociado, su led se encenderá y se bloquearan todos las opciones fuera de este modo. 

Con el efecto deseado listo a editar se cambiará el valor del potenciómetro deslizable y se oíra su intervención en el sonido de manera inmediata, al seleccionar el mismo efecto u otro el valor del pote se guardará automáticamente y como respuesta visual se tendrá su led asociado encendido de manera constante

Una vez se hayan hecho los cambios deseados, se podrá presionar el botón de `guardarEdicion` lo que hace que el sónido editado ingrese a uno de los 60 puntos posibles en los que inicia a sonar con la mezcla / conjunto

Siempre que se cambie de efecto o se salga con el botón guardar efectos se deberá guardar el valor asociado al efecto que se estaba editando

Al reeditar un efecto, este ocurre con el valor actual de la slider

<br>

#### Estructura

Se considera ordenar el código en distintas clases, a continuación se listan algunas a considerar

- Boton (lectura de botones)

- Perilla (lectura de perillas)

- Multiplexor (pensado en un multiplexor para optimizar la lecutra de perillas)

- lucecita (describen el funcionamiento de los distintos estados)

- [NombreEfecto] (se encarga de la complejidad de como debe modificar el `audio`)

- Aleatoriedad (es una estructura que simula aleatoriadad necesaria para el funcionamiento de la máquina)

- Conversor (lógica detrás de la placa ---- que convierte audio digital a análogo)

- Amplificacion (lógica detrás de la placa ---- que convierte amplifica el sónido)

- Reproduccion (define al modo `reproduccion`)

- Grabar (define al modo `grabar`)

- Microfono (establece como se comunica el micrófono con el microcontrolador)

- SDCard (se encarga de la lógica detrás de la grabación y lectura de datos)

<br>

### Wokwi

### version_01

https://wokwi.com/projects/480724591872061441

En este primer acercamiento se buscó replicar la lógica detrás del guardado de audios en la memoria, es por ello que se utlizo el `Serial Monitor` para imprimir mensajes que dan a entender la cantidad de sonidos guardados y su duración, esta se observa según la cantidad de dígitos de cada número

### version_02

https://wokwi.com/projects/479151932434500609

En esta versión nos acercamos a la relación que existen entre los diversos leds y botones al momento de trabajar en la edición de interfaz, estos fueron los primeros acercamienos a ver una interfaz y como dialogar con el usuario

### version_03

https://wokwi.com/projects/481142445555191809

Finalmente tenemos una versión donde vemos conexiones y funcionamientos, por la imposibilidad de representar la lógica pensada para el sónido se utilizará el `Serial Monitor` para representar lo que ocurre en cada sección y proceso, es importante mencionar que se tuvieron que realizar algunos IC para poder representar funcionamientos, tales como el INMP441, que corresponde a un micrófono

![](./imagenes/image.png)

<br>

## Referentes

### [Kaleidoloop](https://forum.critterandguitari.com/t/kaleidoloop-returns/10321)

El Critter & Guitari Kaleidoloop es un sampler y looper portátil diseñado para grabar sonidos, crear bucles con estos y manipularlos en tiempo real. 

Esta fue otra de las máquinas que apenas la vimos nos abrió muchas respuestas como también muchas preguntas. Partiendo desde cómo podría interactuar el usuario con la interfaz, ya sea en el gesto de acercarse a grabar su voz, o incluso abriendo la discusión sobre si el usuario tiende a grabar solo presionando una vez el botón o manteniéndolo presionado.

La disposición de los componentes también nos dio a comprender ciertas importancias, por ejemplo que el micrófono esté extendido físicamente da a entender muy de inmediato que su función principal es grabar. Sin embargo, los botones al ser todos iguales y no tener ningún símbolo que los diferencie uno de otro (viéndolo desde frente) dejan una confusión al usuario al no comprender como se utilizan y tiende a buscar la respuesta en cómo se está reproduciendo el sonido, antes de ver el manual que contiene detrás por ejemplo. 

Otra cosa que rescatamos al estar utilizando este aparato, fue que nos importa mucho que el usuario tenga ambas opciones de reproducir lo que está escuchando desde el mismo objeto o conectarlo a otro dispositivo amplificador.

Además nosotras queremos representar visualmente cómo el sonido se va sumando hasta que ya no quede espacio para más audio. El Kaleidoloop muestra qué tan llena está la memoria a través de una lógica de un rango de 1 a 4 parpadeos de un led. Nosotras seguiremos este premisa de representar la capacidad de audios que se van almacenando, pero a través de una pantalla.

<br>

<img src="./imagenes/kaleidoloop.png" alt="" width="50%">

<br>

También desarmamos este aparato para ver el funcionamiento interno y nos dimos cuenta de que utiliza este chip. Algo que nos demuestra que existen otros tipos de controladores que son capaces de realizar cosas similares a las que estamos haciendo con la Raspi.

#### [Circuito Integrado LPC2138FBD64](https://www.y-ic.com/pdf/Freescale-NXP-Semiconductors/LPC2138FBD64.html)

<img src="./imagenes/lpc2138fbd64.jpg" alt="" width="20%">

### [MOOG GRANDMOTHER](https://www.moogmusic.com/synthesizers/grandmother/)

<br>

El Grandmother de Moog, pertenece a una familia de sintetizadores analógicos semimodulares, que quiere decir que tiene 41 puntos de conexión para conectar con otros módulos de sonido, pero que también se puede usar individualmente sin conectar cables.
Este es uno de nuestros principales referentes porque desde aquí pudimos interactuar de manera tangible para comprender como se modula el sonido a través de los efectos que se aplican a las ondas propias de los osciladores del sintetizador.

Por otra parte, hay varias cosas que al momento de estar utilizando este dispositivo nos hicieron ruido respecto a la accesibilidad y organización de la interfaz, por ejemplo, el hecho de que nos costó demasiado encontrar el volumen general, el cuál estaba en un lugar con poca jerarquía visual y pasaba muy desapercibido al estar contiguo al efecto de reverb y, si bien estaba en la sección de output, habían perillas que parecían ser más importantes que esta.

<img src="./imagenes/grandmother.jpg" alt="" width="50%">

### [Mini Glitch](https://electricmojoguitars.com/products/tkog-mini-glitch)

<br>

Es un pedal que crea un glitch aleatorio en el sonido y sirve también para hacer looping experimental. Funciona capturando una muestra corta de señal y luego la repite, en otras palabras graba por sobre sí mismo y se realiza un feedback. Lo que rescatamos de este pedal es el funcionamiento que tiene: el bucle, el efecto de glitch y por sobre todo que use su propia señal para repetir es una lógica que nuestro proyecto busca al introducir audios sobre otros.

<img src="./imagenes/mini-glitch.png" alt="" width="50%">

<br>

### [NYQUIST - Bit Crush](https://notpedals.com/products/nyquist)

<br>

Este es un pedal de efectos hechos con componentes analógicos. El efecto que produce es como el de pixelar el sonido, haciendo que este pierda nitidez. Nosotras rescatamos el efecto como tal y lo integramos a nuestra sección de edición de audio.

<img src="./imagenes/bit-crusher.webp" alt="" width="50%">

<br>

## Comentarios Claude

### v01

Revisión de inconsistencias entre las secciones del README y el `diagram.json` de `codigos/proyecto-03-v02`, hecha antes de escribir el `main.cpp` para Wokwi (Raspberry Pi Pico SDK). En el código, la grabación y la reproducción de audio se reemplazan por mensajes en el Serial Monitor.

El `diagram.json` actual coincide con la tabla de **Conexiones**: el switch de encendido va al C12 del CD74HC4067 y el INMP441 está en GPIO 19, 28 y 27. Las inconsistencias que siguen están dentro del propio README.

#### Decisiones ya tomadas

| # | Tema | Inconsistencia en el README | Decisión |
| - | ---- | --------------------------- | -------- |
| 1 | Duración de la grabación | "Sobre el proyecto" dice 30 s; la interfaz, la coreografía y el pseudocódigo dicen 10 s. | Máximo de 30 s. Se puede cortar antes con una segunda pulsación, siempre que hayan pasado al menos 0,5 s. |
| 2 | Botón de reproducción | La interfaz y la coreografía dicen reproducir/pausar. "Sobre el proyecto" dice que no hay pausa. El pseudocódigo y el Modo Reproducir usan dos botones (`botonReproducir` y `botonDetener`), pero en las conexiones solo existe `botonReproducir` (C11). | Un solo botón que alterna: reproduce desde el segundo 0 en bucle y detiene volviendo al inicio. No hay pausa. |
| 3 | Llenado de la mezcla | "Sobre el proyecto" dice que al ocupar los 60 puntos ya no se puede grabar; la sección Audio permite superponer varios audios en un mismo segmento. | El segmento se asigna al azar entre los 60, con superposición. El límite son **120 audios**; al llegar a 120 se bloquea la grabación. Este límite no aparece en el README. |
| 4 | LEDs al seleccionar un efecto | La sección Luces dice que los efectos ya aplicados quedan encendidos fijos; el pseudocódigo dice que los demás se apagan. | Los aplicados quedan fijos, el seleccionado parpadea y un efecto aplicado se puede volver a seleccionar para reeditarlo. |
| 5 | Tarjeta SD | Se usa para almacenar el `conjuntoDeAudios`, pero el Pico SDK de Wokwi no incluye una librería FAT. | No se usa. El conjunto vive en RAM y el guardado se informa por serial. |
| 6 | DAC y amplificador | PCM5102A y PAM8302. | No se controlan sus pines. Todo lo que sonaría se informa por serial. |
| 7 | Pantalla | — | Se implementa lo descrito en la sección Pantalla: presentación al encender, contador de audios, grilla de 60 segmentos y cuenta regresiva con barra durante la grabación. |

#### Ambigüedades pendientes

Estas afectan directamente el código:

1. **Efectos que no calzan.** La tabla de *Modificadores de sonido* tiene 8 efectos más 2 volúmenes y no incluye High-pass. Las *Conexiones* tienen 9 botones de efecto e incluyen `fxHighPassF`, que no se describe en ningún lado.
2. **Agrupación de los efectos.** La tabla agrupa en Envolventes (2), FX/Modulación (6) y Niveles (2). Las conexiones agrupan de a 3 por slider: `fx0al2` (Attack, Release, HighPass), `fx3al5` (Pitching, Reverb, Trémolo) y `fx6al8` (BitCrushing, RingModulator, Velocidad).
3. **LEDs que no existen en el hardware.** El pseudocódigo pide un LED que indique que la máquina recibe corriente y 2 LEDs indicadores de modo (reproducción y grabación). No están en el BOM, en las conexiones ni en el diagrama.
4. **Animación al entrar a edición.** Hay tres versiones:
   - *Luces*: "se encienden progresivamente, aumentan su brillo y luego se apagan".
   - *Modo Editar*: el brillo sube y baja una sola vez durante 1,5 s.
   - *Pseudocódigo*: "empiezan a variar su brillo mediante PWM", sin decir que termine.

   Además, los LEDs van por 74HC595, que no tiene PWM, así que el brillo habría que simularlo por software.
5. **Textos de la pantalla.**
   - No se dice cuál es "el nombre del proyecto" que aparece al encender.
   - "Memoria disponible" no tiene unidad (¿audios restantes de 120? ¿porcentaje?).
   - En *Modo Reproducir*, "El elemento de la pantalla muestra [...]" quedó incompleto.
   - "La frecuencia con la que pueden ingresar cada audio" no se entiende.
   - No se dice si la pantalla muestra algo distinto mientras se reproduce la mezcla.
6. **Reproducir y grabar a la vez.** El pseudocódigo bloquea la grabación mientras se reproduce la mezcla, pero no se dice qué pasa con la mezcla durante la grabación y la edición: ¿se bloquea el botón de reproducir o se detiene la mezcla?
7. **Apagar sin guardar.** No se dice qué pasa con un sonido que se está grabando o editando sin guardar si se apaga la máquina: ¿se descarta, se guarda solo o se bloquea el apagado?
8. **Guardar con un efecto parpadeando.** No se dice si ese efecto se aplica con el valor actual del slider o se descarta.
9. **Valor inicial al reeditar.** No se dice si un efecto reeditado parte del valor que tenía guardado o de la posición actual del slider.
10. **Volumen específico.** No se dice si se puede ajustar en cualquier momento de la edición o solo de una forma particular, ni si se guarda junto al audio.

Estas no cambian el código, pero conviene corregirlas en el README:

11. **Nombres que no coinciden.**
    - `guardadEfectos` (C9) en las conexiones, `guardarEdicion` en Modo Editar y "Botón guardar" en la interfaz.
    - `interruptorGeneral`, `interruptorEncendido` y `botonEncendido`. Además está listado como clase `Boton`, pero en el diagrama es un slide switch.
    - `Lucesita` / `Lucecita`.
    - HP4067 / CD74HC4067.
12. **"74HC595 Q8" no existe.** El 74HC595 tiene Q0 a Q7. `fxVelocidad` está en el Q0 del segundo 74HC595, encadenado desde el Q7S del primero.
13. **Conexiones incompletas.** La tabla no incluye el LCK del PCM5102A, que en el diagrama va a GPIO 22. El PAM8302 no aparece en la tabla.
14. **Cantidad de multiplexores.** El BOM indica 2 CD74HC4067, pero se usa uno solo (más un 74HC4051 para las perillas).
15. **Placa distinta.** El README habla de una Raspberry Pi Pico 2 W y el diagrama de Wokwi usa `wokwi-pi-pico`, que es la Pico 1 (RP2040).
16. **Modo Borrar.** La sección *Estructura* lista una clase `Borrar`, pero "Sobre el proyecto" dice que no se puede borrar.
17. **Persistencia al apagar.** Se pide que el `conjuntoDeAudios` quede almacenado al apagar. Sin la SD, en Wokwi solo se conserva en RAM mientras dure la simulación.
18. **Doble click en reproducir.** En *Modo Reproducir*, `botonReproducir` "ignora doble click, cuanto tiempo se presiona", y no queda claro qué comportamiento se busca.
19. **Pausa en la interfaz.** La tabla de interfaz y la coreografía todavía dicen "reproducción / pausa", lo que contradice la decisión 2.

### v02

`codigos/proyecto-03-v02/main.cpp` ya está escrito y compila sin errores ni advertencias con el Pico SDK 2.3.1. No se ha probado en Wokwi. Usa `pico_stdlib`, `hardware_adc` y `hardware_i2c`.

#### Decisiones nuevas

| # | Tema | Decisión |
| - | ---- | -------- |
| 1 | Duración de la grabación | Máximo de 30 s. La cuenta regresiva de la pantalla va de 30 a 0. Un audio puede seguir sonando en los 2 segmentos siguientes. |
| 2 | Límite de audios | 200, como dice *Modo Grabar* (reemplaza el 120 de la tabla de v01). |
| 3 | Animación al entrar a edición | Los 9 LEDs se encienden fijos un momento y se apagan, sin PWM. |
| 4 | Apagar sin guardar | El sonido que se estaba grabando o editando se descarta. El `conjuntoDeAudios` se conserva en RAM. |
| 5 | Presentación | "grupo-01" durante 2 s. |
| 6 | Memoria disponible | Porcentaje libre respecto de los 200 audios. |
| 7 | Pantalla durante la reproducción | La vista general, con un marco en el segmento que está sonando. |
| 8 | Volumen específico | Se mueve libremente durante la edición y su valor se guarda junto al audio. |
| 9 | Audios que se pasan del ciclo | Lo que sobra después de 10:00 suena al inicio de la vuelta siguiente. |
| 10 | Velocidad | De 45 a 55 es velocidad normal (1x). Bajo 45 va más lento, hasta 0,5x en 0; sobre 55 va más rápido, hasta 2x en 100. El serial muestra el valor, el texto (normal, más lento o más rápido) y el factor. |
| 11 | Grabar durante la reproducción | Mientras se reproduce la mezcla se puede presionar grabar. La mezcla se detiene y vuelve al inicio, y empieza la grabación. Después de guardar, la máquina queda en el modo general sin reproducir. |

#### Valores elegidos sin especificación (revisar)

Estos valores no están definidos en el README. Son constantes al inicio de `main.cpp` y se pueden cambiar:

- **Duración de la animación de entrada a edición:** 1,5 s, tomados de *Modo Editar*.
- **Parpadeo del efecto en edición:** 250 ms encendido y 250 ms apagado.
- **Anti-rebote de los botones:** 30 ms.
- **Al apagar, la pantalla se apaga.** El README solo dice que "se bloquean todas las funciones".
- **Al volver a encender, se muestra de nuevo la presentación de 2 s.**
- **Grilla de la pantalla:** 12 × 5 segmentos. Cada audio suma un bloque de 2×2 píxeles, con un máximo visible de 12 por segmento; desde el audio 13 de un mismo segmento la celda ya no cambia. Un segmento vacío se ve como un punto.
- **Primera vuelta de la mezcla:** lo que sobra de los audios de los últimos segmentos no suena al inicio, porque antes no había sonado nada.
- **El volumen general se informa por serial cada vez que cambia**, en cualquier modo mientras la máquina está encendida.
- **El switch de encendido (C12) en HIGH significa encendido.**
- **Aleatoriedad:** un *xorshift* cuya semilla se mezcla con el momento exacto en que se presiona cada botón.

#### Inconsistencias que siguen en el README

1. La interfaz ("Botón GRABAR: durante 10 segundos"), la coreografía (cuenta regresiva de 10 segundos), *Pantalla* (cuenta regresiva de 10 a 0) y *Audio* ("cada nuevo audio grabado tiene una duración de 10 segundos") todavía dicen 10 s.
2. *Sobre el proyecto* todavía dice que al ocupar los 60 puntos ya no se puede grabar, y no menciona el límite de 200.
3. *Luces* (punto 3) y *Modo Editar* todavía describen el brillo subiendo y bajando, en vez de encenderse fijo y apagarse.
4. La decisión 3 de la tabla de v01 todavía dice 120 audios.
5. `fxHighPassF` sigue sin descripción en la tabla de *Modificadores de sonido*.
6. En la interfaz, la zona de edición dice "3 perillas", pero son sliders.
7. En la tabla de conexiones dice `CD74HP4067`; el nombre del integrado es CD74HC4067.
8. El pseudocódigo todavía dice que al reproducir "se inhabilita el modo grabar mientras dure la reproducción", lo que contradice la decisión 11.

## Bibliografía

* Analogue Media Technologies. (2025, 18 de marzo). *C-20 black audio cassettes*. https://www.duplication.com/c-20-black-audio-cassettes.html
* chrisk. (2024, 29 de febrero). *Kaleidoloop returns!* [Publicación en foro]. Critter & Guitari Forum. https://forum.critterandguitari.com/t/kaleidoloop-returns/10321
* @creative-stuff. (s.f.). *Inexpensive preamp with mic (updated)* [Video]. YouTube. https://youtube.com/shorts/euH0vpYYxks
* Hologram Electronics. (s.f.). *Microcosm: Granular looper & glitch pedal*. https://www.hologramelectronics.com/pages/microcosm
* The King of Gear. (s.f.). *TKOG Mini Glitch stutter/looper*. Electric Mojo Guitars. https://electricmojoguitars.com/products/tkog-mini-glitch
* Mad Valley. (s.f.). *Staggered light performance on the OP-1 and Hologram Microcosm* [Video]. YouTube. https://www.youtube.com/watch?v=8vhdkyEW5As
* Miyara, F. (2001, marzo-abril). El sonido, la música y el ruido. *Tecnopolitan*. https://www.fceia.unr.edu.ar/acustica/biblio/sonmurui.pdf
* Real Academia Española. (s.f.). Audio. En *Diccionario de la lengua española* (23.ª ed., versión 23.8.1 en línea). Recuperado el 20 de noviembre de 2026, de https://dle.rae.es/audio
* Schafer, R. M. (s.f.). *Nunca vi un sonido* [Documento PDF]. https://estefaniasoundscape.wordpress.com/wp-content/uploads/2017/10/nunca-vi-un-sonido_r-murray-schafer.pdf
* Shojaei, A. M. (2023, 15 de abril). *74HC595 with Arduino: How it works & how to use [Full guide]*. ElectroPeak. https://electropeak.com/learn/74hc595-with-arduino-how-it-works-how-to-use-full-guide/
* Sonido. (s.f.). En *Wikipedia*. Recuperado el 20 de noviembre de 2026, de https://es.wikipedia.org/wiki/Sonido
* Techinn. (s.f.). *Teenage Engineering sintetizador OP-1 field*. Tradeinn. https://www.tradeinn.com/techinn/es/teenage-engineering-sintetizador-op-1-field/143706579/p
* Turbulent Audio. (s.f.). *NYQUIST bitcrusher*. NotPedals. https://notpedals.com/products/nyquist
