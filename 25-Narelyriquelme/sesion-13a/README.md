# sesion-13a

## apuntes sesión

### calendarización
13a: tabla materiales, texto descripción proyectos, diagrama flujo (más 2 propuestas)

13b: descripción total proyecto, prototipos, pseudo, wokwi

14a: 

14b: entrega proyecto 03

### descripción proyecto
ideas 
- cajita con regalo, adivina la canción 

El proyecto consiste en una rocola interactiva, la cual consta con música controlada por voz, desarrollado mediante un sistema utilizando una Raspberry Pi Pico 2 W y programada en C++. De forma física, se mantendrá la estética de una caja retro-rockola que se mantiene en un estado de reposo hasta que un sensor ultrasónico detecta la aproximación de un usuario a menos de 50 cm. Al activarse, este sistema opera enteramente mediante una interfaz de usuario por voz. Un módulo de hardware DFPlayer Mini asume el rol de locutor reproduciendo instrucciones pregrabadas desde una tarjeta MicroSD y emitiendo los fragmentos de las canciones correspondientes a cada uno de los tres niveles del reto. El usuario interactúa respondiendo en voz alta, momento en el cual un módulo de reconocimiento de voz capta la respuesta para que el microcontrolador la valide. Dependiendo del resultado, el sistema emite una retroalimentación auditiva indicando si acertó o falló, y si el jugador logra superar los tres niveles consecutivos utilizando únicamente comandos vocales, la máquina de estados finitos que gestiona el código envía una señal PWM a un servomotor oculto que libera una compuerta mecánica para entregar un premio de forma física.

### BOM
|Componente|Cantidad|Precio|Link|
|---|---|---|---|
|Raspberry Pi Pico|1|$14.990|<https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers>|
|Reproductor de Audio (DFPlayer Mini)| 1 |$2.990|<https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0>|
|Salida de Audio (Altavoz 3W, 8 Ohm)| 1 |$3.000|<https://afel.cl/products/mini-parlante-altavoz-de-3w>|
|Sensor de Proximidad (HC-SR04)| 1 | $1.500 |<https://afel.cl/products/sensor-de-ultrasonico-hc-sr04>|
|Micro Servo SG90| 1 | $1.990 |<https://afel.cl/products/micro-servomotor-sg90>|
|Entrada de Voz (Módulo Offline o Micrófono I2S)| 1 | $x | x |
|Protoboard|1|$1.500|<https://afel.cl/products/mini-protoboard-400-puntos>|
|Cables dupont pack 20|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-macho>|
|Kit 10 Cables Caiman a Dupont Hembra|x|$2.500|<https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra?_pos=2&_sid=aec538813&_ss=r>|
|Pack 20 Cables de Conexión Macho Hembra|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra?_pos=1&_sid=bff72fae6&_ss=r>
|Potenciómetro B100k|1|$500|<https://afel.cl/products/potenciometro-10k-ohm>|
|Cable |1| $x|<https://www.masterg.cl/mgcamic/p?idsku=6>|
|Almacenamiento (Tarjeta MicroSD) | 1 | $x | x |
|Led| x | $x | x |


### diagrama flujo
- representación gráfica de pasos en forma secuencial
- presentación información y apoyo al razonamiento
- visualizar procesos complejos
- todos puedan leer el trabajo de todos

círculo/ovalos: start/stop del sistema

cuadrado: operación en particular

rombo: decisiones o bifurcaación, puntas indican diferentes subprocesos

<https://www.visual-paradigm.com/tutorials/flowchart-tutorial/>

3 referentes de estudio para viernes:
- david byrne, playing the building <https://en.wikipedia.org/wiki/Playing_the_Building>
- kelly dobsonn, scream body <https://www.youtube.com/watch?v=Ta7rN5TeKzw>
- dato musical instruments <https://dato.mu/>
- brian dewan
- <https://bossfightbooks.com/>

## encargos

## lectura
