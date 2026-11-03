# sesion-11a

## apuntes sesión

avance en clase 

1.  Diagramas de flujo sobre: (Cami)
•  Uso usuario general
•  Diseño general 
2.  Carcasa (Bianka)
3.  Pseudo Código general + escenarios que no queremos (Daya + Cami)
4.  Esquematico (Daya + Cami)
5.  Describir lo que vamos a presentar (Daya)
6.  Nombre proyecto
7.  class Led (Bianka)
8.  **Pulir concepto README (Bianka)**

## encargos

## primer diagrama de flujo 

falta ver detalles (proceso) 

![Diagrama Botón](./imagenes/diagrama-flujo1.png)


## concepto 

Se plantea que el sonido de un computador está condicionado por la tecnología de su época, desde los chips de síntesis que generaban melodías y efectos electrónicos hasta la incorporación de audio digital y música grabada.

A partir de esta exploración, surge la pregunta: ¿cómo ha evolucionado el sonido de los videojuegos a medida que cambian las capacidades tecnológicas de los computadores? El proyecto busca reconocer estas transformaciones y comprender cómo las características sonoras permiten identificar distintas etapas en la historia de estas máquinas.

La evolución del sonido y la música en los videojuegos japoneses ha transitado desde los simples tonos monofónicos de los arcades y consolas de 8 bits hasta complejas bandas sonoras dinámicas e integradas con tecnología binaural y orquestal.

## ¿por qué 5 sonidos? 

- Un tono simple.
- Una secuencia rítmica.
- Una melodía sintetizada.
- Un sonido percusivo.
- Una muestra de audio digital.

La elección de cinco sonidos responde a la intención de representar distintas transformaciones tecnológicas en la historia del audio de los videojuegos. Cada sonido corresponde a una etapa que permite reconocer cómo evolucionaron la generación, síntesis y reproducción del sonido, desde los primeros tonos electrónicos hasta el audio digital. 

## ¿Cómo funciona la Música de Videojuegos Retro?

https://www.youtube.com/watch?v=v8Z2ft8XTHk

**Música de 1 bit  Describe cómo las primeras computadoras como el Apple II o el IBM PC generaban sonido mediante un beeper o buzzer que solo cambiaba entre dos estados (encendido/apagado). La clave del sonido musical radicaba en la frecuencia de vibración y el control del ciclo de trabajo.**

- El hardware: El sonido era generado directamente por el procesador de computadoras como el Apple II, el IBM PC o el ZX Spectrum 48k. El componente encargado de emitir el sonido era un simple bóxer o beeper (una membrana que solo podía tener dos estados: encendido o apagado, representados por un 1 y un 0).

- Funcionamiento: Al enviar cambios rápidos entre estos dos estados (1 y 0), se generaba una vibración. Si esta vibración ocurría a una frecuencia audible (entre 20 Hz y 20,000 Hz), el oído humano podía percibirla como un tono musical.

- El papel de la programación: Como el hardware era tan limitado, el compositor dependía enteramente del código (en ensamblador o BASIC) para controlar la frecuencia de esos cambios de estado en tiempo real. Esto significaba que la música no ocupaba espacio de almacenamiento, pero requería una gran habilidad técnica para crear melodías complejas.

- Limitaciones y trucos: Los programadores utilizaban técnicas como cambiar el ciclo de trabajo (cuánto tiempo está encendido vs. apagado el beeper) para modificar el timbre del sonido. Algunos genios, como Tim Follin, lograron incluso simular efectos de percusión y melodías polifónicas básicas simplemente alternando estos sonidos de manera extremadamente rápida.

**Era de los 8 bits: Explica la evolución con consolas como la NES, Sega Master System, Game Boy y la Commodore 64. Se introdujeron chips de sonido dedicados (como el Ricoh 2A03 o el SID 6581) que permitían gestionar múltiples canales de sonido simultáneos (ondas cuadradas, triangulares, de ruido y PCM) para crear melodías más complejas.**

- Nintendo Entertainment System (NES) - Ricoh 2A03 (7:03): Utilizaba este chip APU (Audio Processing Unit) que generaba sonido a través de cinco canales: dos de onda cuadrada (para melodías), uno triangular (para bajos), uno de ruido (para percusión) y un canal DPCM (para grabaciones de muy baja calidad).

- Sega Master System - Texas Instruments SN76496 (16:25): Contaba con un chip que manejaba cuatro canales: tres dedicados a pulsos cuadrados y uno para generar ruido. Era más limitado que el de la NES, pero permitía crear melodías muy interesantes.

- Nintendo Game Boy - Sharp LR35902 (16:47): Su chip de audio funcionaba con cuatro canales: dos de onda cuadrada, uno para ruido y un canal personalizable donde el programador podía diseñar la forma de onda bit a bit, lo que le daba gran versatilidad.

- Commodore 64 - SID 6581 (17:18): Este es uno de los chips más famosos y potentes de la época. A diferencia de otros, generaba ondas de diente de sierra, además de las cuadradas, triangulares y de ruido. Aunque solo tenía tres canales, su arquitectura permitía técnicas avanzadas donde los canales podían alternar entre tipos de onda tan rápido que lograban emular una polifonía mucho mayor.

- Síntesis FM (Sega Genesis): Sega utilizó chips basados en síntesis de frecuencia modulada (FM), tecnología famosa por instrumentos como el teclado Yamaha DX7. Este sistema permitía combinar múltiples ondas (como una cuadrada con una senoidal) para crear timbres mucho más ricos y únicos (19:16-19:57).

- Muestreo de audio o ADPCM (Super Nintendo): Nintendo optó por un enfoque distinto donde los canales funcionaban principalmente como reproductores de muestras (samples) de baja resolución. Esto permitió reproducir instrumentos reales grabados, lo que dio como resultado un sonido mucho más orquestal y realista comparado con los tonos electrónicos de la generación anterior (20:06-20:50).

**Era de los 16 bits: Se presenta como un prólogo técnico sobre el avance hacia la Sega Genesis (con su síntesis FM famosa en teclados Yamaha DX7) y la Super Nintendo (basada en el muestreo de audio o ADPCM), lo que permitió una transición de sonidos electrónicos a composiciones más orquestales.**

### Resumir la información más relevante e identificar qué computadores japoneses coinciden con las características tecnológicas estudiadas, especialmente en sus chips de sonido y componentes de audio

## Fases de la evolución del sonido en Japón

• Era de 8 bits y 16 bits (Años 80 - Principios de los 90): Limitada por chips de sonido de canales reducidos (como el chip PSG o el chip de FM de Yamaha en sistemas como la NES, MSX o Mega Drive/Genesis). Compositores como Koji Kondo (en Super Mario o The Legend of Zelda) y Nobuo Uematsu (en Final Fantasy) crearon melodías altamente pegajosas que definieron la identidad del chip chiptune debido a las restricciones técnicas de la memoria.

• Era del CD-ROM y 32 bits (Mediados de los 90): Con la llegada de soportes como la PlayStation o la Sega Saturn, se dio el salto al audio grabado (pistas en formato Red Book CD o MIDI avanzado), permitiendo incorporar instrumentos acústicos reales, coros y arreglos orquestales más emotivos.

Era moderna y el sonido interactivo (Siglo XXI - Presente): La música y los efectos dejaron de ser simples archivos lineales para volverse adaptativos y dinámicos, reaccionando en tiempo real a las acciones del jugador, la exploración o el combate. Además, géneros como el jazz, la música electrónica y fusiones culturales pasaron a ser sellos distintivos en sagas como Persona o Sonic the Hedgehog.

https://digibuo.uniovi.es/dspace/bitstream/handle/10651/73779/reflexion.pdf?sequence=3&isAllowed=y

## Computadores japoneses para el caso de estudio

### 1. NEC PC-8801mkII SR (1985)

Chip: Yamaha YM2203 (OPN).

Tecnología: 3 canales FM y 3 canales SSG.

Videojuegos:
- Thexder
- Ys I: Ancient Ys Vanished
- Hydlide


### 2. Sharp X68000 (1987)

Chips:
- Yamaha YM2151 (OPM).
- OKI MSM6258.
Tecnología: 8 canales de síntesis FM y un canal de audio ADPCM.

Videojuegos:
- Gradius II
- Akumajō Dracula
- Final Fight
- 
### 3. Fujitsu FM TOWNS (1989)

Chip / tarjeta: Yamaha YM2608 (OPNA), mediante tarjeta de sonido PC-9801-86.

Tecnología: 6 canales FM, 3 SSG, canales de ritmo y ADPCM.

Videojuegos:
- Touhou Reiiden: The Highly Responsive to Prayers
- Touhou Fūmajinroku: The Story of Eastern Wonderland
- Rusty

### 4. NEC PC-9801 (1980s–1990s)

Hardware: unidad CD-ROM integrada y capacidades multimedia.

Tecnología: reproducción de audio digital, muestras y contenido multimedia.

Videojuegos:
- Prince of Persia
- Wing Commander
- Might and Magic III

### 5. NEC PC-9821 Ra43 (2000)

Computador: NEC PC-9821 Ra43.

Año: 2000.

Procesador: Intel Celeron de 443 MHz.

Audio: la configuración de audio depende del modelo y sus componentes. La serie PC-9821 tuvo modelos con reproducción PCM y tarjetas de sonido adicionales.

### 6. Toshiba DynaBook DB70P/5MC (2000)

Computador: Toshiba Qosmio V65/87M.

Año: 2010.

Audio:
- Intel High Definition Audio.
- Altavoces estéreo harman/kardon.
- Dolby Advanced Audio.
- MaxxAudio.
- Reproducción multimedia mediante Blu-ray.
  
## repo para estudiar 

https://github.com/yuuqilin/RetroJapSound?utm

## lectura

Caos y fractales (Apéndice A, págs 205-215)


Hoy leí el Apéndice A y por fin entendí por qué todo el libro usa fórmulas a saltos $x_{n+1} = f(x_n)$.

1.  **Hay cosas que son discretas.** Como el banco que te paga interés una vez al año, o las plantas que ponen una hoja a la vez.
2.  **Las plantas son inteligentes.** Si simulas que pone cada hoja donde hay menos veneno, al final siempre queda un ángulo de 137,5° entre hojas. Es el ángulo áureo. Así ninguna tapa a la otra. Por eso las piñas tienen 5, 8, 13 espirales (Fibonacci).
3.  **Lo continuo también se puede medir a saltos.** Como contar salmones una vez al año, o medir un latido del corazón. Al graficar $x_{n+1}$ vs $x_n$ te sale una parábola.
4.  **El truco del corte de Poincaré.** Le pones un vidrio a una trayectoria 3D (como la de Lorenz del clima) y anotas donde cruza. De 3D pasas a 1D. Con eso el Bruselador y la reacción química de Belousov-Zhabotinsky también se vuelven una parábola.
5.  **Hasta se puede llegar al mapa de Hénon.** Si tienes una rueda con roce y le das patadas periódicas, integrando a mano te sale el mapa de Hénon, que es un fractal famoso.

**Conclusión:** Todo se puede escribir como $x_{n+1} = f(x_n)$. Por eso el libro lo usa.

## Citas textuales

> "Lo sorprendente es que el atractor 'reconstruido' de esta manera (con valores apropiados de $\\tau$ y $j$ suficientemente grandes) tiene las mismas propiedades globales que un atractor proveniente de coordenadas experimentales independientes."

> "Existen sistemas para los que las ecuaciones diferenciales se pueden integrar analíticamente, de modo que se pueden obtener ecuaciones iterativas."
