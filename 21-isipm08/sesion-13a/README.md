# sesion-13a

## apuntes sesión

muchos problemas en el metro sos

estamos revisando sobre la calendarización

entrega 20/11 proyecto 03

hoy veremos a cerca de lo visual, que queremos realizar y nuestra descripción del proyecto

### brainstorming

sí
1. chistoso
2. sentimental
3. mucha luz led
4. sensor
5. sonido, sonora (música, voz)
6. colorido, raro
7. pantalla
8. interactivo táctil
9. muchos botones
10. 3d, acrílico
11. mecanismo

no
1. no serio
2. no convencional
3. que no hable solo
4. perilla
5. no cartón
6. no grosero
7. no pequeño
8. no transparente
9. que no se mueva solo del su lugar
10. no abierto
11. no varias personas a la vez
12. no más $50.000 entre todos

### calendarización
13a: tabla materiales, texto descripción proyectos, diagrama flujo (más 2 propuestas)

13b: descripción total proyecto, prototipos, pseudo, wokwi

14a: 

14b: entrega proyecto 03

### descripción proyecto
ideas 
- cajita con regalo, adivina la canción 

El proyecto consiste en una rocola interactiva de trivia musical 100% controlada por voz, desarrollada como un sistema embebido utilizando una Raspberry Pi Pico 2 W y programada en C++. Físicamente, el dispositivo mantiene la estética de una caja retro que permanece en estado de reposo hasta que un sensor ultrasónico detecta la aproximación de un usuario a menos de 50 cm. Al activarse, el sistema prescinde de pantallas o botones físicos, operando enteramente mediante una Interfaz de Usuario por Voz. Un módulo de hardware DFPlayer Mini asume el rol de locutor reproduciendo instrucciones pregrabadas desde una tarjeta MicroSD y emitiendo los fragmentos de las canciones correspondientes a cada uno de los tres niveles del reto. El usuario interactúa respondiendo en voz alta, momento en el cual un módulo de reconocimiento de voz capta la respuesta para que el microcontrolador la valide. Dependiendo del resultado, el sistema emite una retroalimentación auditiva indicando si acertó o falló, y si el jugador logra superar los tres niveles consecutivos utilizando únicamente comandos vocales, la máquina de estados finitos que gestiona el código envía una señal PWM a un servomotor oculto que libera una compuerta mecánica para entregar un premio físico.
  
### BOM
|Componente|Cantidad|Precio|Link|
|---|---|---|---|
|Raspberry Pi Pico|1|$14.990|<https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers>|
|Reproductor de Audio (DFPlayer Mini)| 1 |$2.990|<https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0>|
|Salida de Audio (Altavoz 3W, 8 Ohm)| 1 |$3.000|<https://afel.cl/products/mini-parlante-altavoz-de-3w>|
|Sensor de Proximidad (HC-SR04)| 1 | $1.500 |<https://afel.cl/products/sensor-de-ultrasonico-hc-sr04>|
|Mecanismo de Premio (Micro Servo SG90)| 1 | $1.990 |<https://afel.cl/products/micro-servomotor-sg90>|
|Entrada de Voz (Módulo Offline o Micrófono I2S)| 1 | $3.500 - $25.000 | *Buscar en MercadoLibre o tiendas especializadas (MCI Electronics/Robótica).* |
|Protoboard|1|$1.500|<https://afel.cl/products/mini-protoboard-400-puntos>|
|Cables dupont pack 20|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-macho>|
|Kit 10 Cables Caiman a Dupont Hembra|x|$2.500|<https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra?_pos=2&_sid=aec538813&_ss=r>|
|Pack 20 Cables de Conexión Macho Hembra|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra?_pos=1&_sid=bff72fae6&_ss=r>
|Potenciómetro B100k|1|$500|<https://afel.cl/products/potenciometro-10k-ohm>|
|Cable |1| $x|<>|
|Almacenamiento (Tarjeta MicroSD) | 1 | $3.500 - $5.000 | *Cualquier tienda de electrónica o retail.* |
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
  
## lectura
### Mindstorms: Children, Computers and Powerful Ideas - Seymour Papert
- me encuentro aún en el capítulo 7 llamado "Logo´s Roots: Piaget and AI", leí 6 planas debido a que me quedan pocas páginas, para dividirlas en las lecturas siguientes

*apuntes lectura*
- estudiar estructura conocimiento es entender a la persona
- el conocimiento y la persona son dependientes entre sí
- esta relación ha ido avanzando según el estudio de las máquinas y conocimiento que estas pueden encarnar
- IA -- utilizar modelos computacionales para obtener información sobre la psicología humana, y utilizar esta como fuente de ideas sobre cómo hacer que los mecanismos emulen inteligencia humana
- se habla de los tipos de razonamiento, lo cual se llega a la teoría de que las personas son capaces de pensar solo porque pueden recurrir a grupos más grandes de conocimientos
- reglas de percepción
- nueva entidad que surge y nace desde los sistemas de programación
- capacidad de los niños para reflexionar sobre su propio pensamiento
