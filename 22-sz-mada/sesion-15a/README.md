# sesion-15a

2026.11.24

## apuntes sesión

Aarón, ya chate de lidiar con la asistencia (lol) decidió simplemente dejar que todos puedan presentarse a examen, dijo que había gente que tenía un 78% de asistencia y decidió simplemente redondearlo para que tuvieran el mínimo (80%)

El examen del taller actual es el 50% de la nota semestral

(parte de?) El plan de hoy es nos van a dar acceso a un excel en el que cada grupo se anotará en los componentes que necesita para su proyecto, para que se pueda comprar todo al por mayor.

El viernes es momento de devolver el libro (Chromatic Algorithms), si bien no leí mucho lol igual me interesó lo poco que si leí, mi problema claramente yace en que una vez dejé de leerlo, no pude retomarlo, cosa que me sucede muy a menudo con todo tipo de media (otros libros, series, juegos, películas, hasta música). Por más que me gustaría quedarme con el libro porque me interesa terminarlo, encuentro injusto quitárselo a Aarón considerando que quizás mi cerebro me permita continuar leyéndolo como en 10 años más, así que me conformaré con continuarlo en formato PDF (el cual [ya encontré online](https://attachments.are.na/10870952/b915679d4c7261a4c67d4f3f6dd9625d.pdf?1614012230))

### Factibilidad (revisado por Claudito LLM)

#### 1. ¿Alcanza la memoria interna de la Pico?

**No, necesitan la microSD.** Las cuentas son estas:

- La Pico 2 W trae 4 MB de memoria flash, y el programa ya ocupa una parte.
- Su biblioteca suma 3 cuentos de 5 min, 4 músicas de 1 min y 1 mensaje de 1 min, o sea unos 20 minutos de audio.
- Un audio WAV de voz (mono, 16 kHz, 16 bits) pesa unos 32 KB por segundo. Los 20 minutos son **unos 38 MB**.
- Aunque lo bajen a calidad muy mala (8 kHz, 8 bits) o lo compriman a MP3 de 32 kbps, igual pasan los 4 MB.

Además, el mensaje del cuidador tiene que poder **reemplazarse**. Eso es muy simple en una SD y muy complicado en la flash interna, que además se desgasta. Entonces la SD se queda.

Ojo con la tarjeta: en la lista dice "por definir". Sirve cualquier microSD de 2 a 32 GB, formateada en FAT32, y 4 GB sobra. Se puede comprar en cualquier tienda de computación, no tiene que ser en Afel.

**Si dejan solo una categoría + mensaje** (lo que sugiere el profe), la SD sigue siendo necesaria, pero tienen menos audios que preparar. Eso baja bastante el trabajo.

#### 2. Cosas a corregir en los materiales

**Parlante y amplificador.** La Pico no tiene salida de audio analógica real. Con el PAM8403 tendrían que sacar el sonido por PWM, que funciona pero suena con ruido y es más difícil de ajustar. Una alternativa mejor es el módulo **MAX98357A**, que recibe el audio digital (I2S) y ya trae convertidor y amplificador en una sola placa. Es mono, de 3 W, y calza con su parlante de 4 Ω. Reemplazaría al PAM8403 y probablemente cueste parecido; hay que cotizarlo. Si prefieren no cambiar el PAM8403, es posible, pero esperen más ruido.

**Micrófono MAX4466.** Sirve para voz y la Pico lo puede leer con su entrada analógica (ADC). Dos cuidados:

- Tengan el cable al módulo de grabación lo más corto posible, porque una señal analógica larga capta ruido.
- Alimenten el micrófono desde 3,3 V.

**Pulsadores.** Los de 12x12 mm son demasiado chicos para una mano de 3 a 4 años. Dos soluciones:

- Botones tipo arcade de 24 a 30 mm.
- O imprimir en 3D una tapa grande que presione el pulsador chico que ya tienen. Esto además les permite poner el símbolo en relieve, como quieren.

**Lo que falta en la lista:**

- Fuente USB de 5 V y **al menos 1 A**. El parlante y la pantalla piden más corriente de la que da un puerto de computador.
- Resistencia para el LED (220 Ω) y un condensador grande (470 a 1000 µF) cerca del amplificador, para evitar ruido.
- Placa perforada (perfboard), estaño y soldador. Los cables Dupont se sueltan con cualquier golpe, y este objeto lo van a manipular niñes.
- Tornillos M3 para cerrar la carcasa.
- Revisar que la Pico 2 W venga con los pines ya soldados.

#### 3. Factibilidad de software

Todo lo siguiente es posible con la Pico 2 W, pero va a costar más que lo que hicieron hasta ahora, sobre todo siendo principiantes:

- **Leer la SD:** hay una biblioteca llamada FatFs para SPI que funciona bien con la Pico. Usen un bus SPI distinto al de la pantalla. Su cableado actual (SD en GP10 a GP13, pantalla en GP16 a GP21) ya los separa, así que está bien.
- **Reproducir:** usen archivos **WAV** y no MP3. El WAV se lee directo, sin decodificar. Para música, 22 kHz suena mejor que 16 kHz, y igual cabe en la SD.
- **Reproducir mientras se actualiza la pantalla:** el audio no se puede cortar. La Pico 2 tiene dos núcleos, así que pueden dejar uno para el audio y otro para la pantalla.
- **Guardar el mensaje sin perder el anterior:** guarden la grabación nueva en un archivo temporal y, solo si salió bien, reemplacen el viejo. Con FatFs se hace bien y calza con su lógica actual.
- **Probar:** Wokwi no simula el audio real ni el micrófono. Mi recomendación es comprar los componentes **pronto** y probar en un protoboard antes de diseñar la carcasa.

#### 4. Fabricación con lo que tienen (Bambu Lab A1)

La carcasa es muy factible. La A1 imprime piezas de hasta 256 mm por lado, y una carcasa para una pantalla de 3,5", un parlante y las placas cabe sin problema.

- **Material:** PETG es más resistente a golpes que PLA. Si usan PLA, hagan paredes de 2 a 3 mm.
- **Diseño en dos piezas** (frente y fondo), cerradas con tornillos M3 para que niñes no la abran. Con inserto metálico o tornillo autorroscante.
- **Bordes redondeados** y sin piezas pequeñas sueltas.
- **Rejilla del parlante:** agujeros de unos 2 mm. Si el parlante queda dentro de una cavidad cerrada atrás, suena mejor.
- **Acceso a la SD y al USB:** déjenlos accesibles para ustedes, pero cubiertos con una tapa atornillada.
- **Botones blandos:** el documento menciona botones blandos al tacto. Se pueden imprimir en TPU, pero ese material es más delicado de imprimir. Para esta entrega yo usaría tapas rígidas con relieve, y dejaría el TPU como mejora.
- **Seguridad:** limiten el volumen máximo por software, porque es un parlante de 3 W para oídos de 3 a 4 años.

#### 5. El módulo de grabación separado

Es factible. Para que el sistema sepa si está conectado, basta usar un conector de 6 pines (3,3 V, GND, micrófono, botón, LED y un cable extra de detección). El cable de detección se conecta a tierra solo cuando el módulo está enchufado, y la Pico lo lee como un botón más. Si se desconecta durante la grabación, la cancelan sin reemplazar el mensaje anterior.

Una alternativa más simple es **integrar el botón de grabar en la misma carcasa**, en un lugar que una guagua no alcance, por ejemplo atrás, hundido, y que requiera mantener apretado varios segundos. Elimina el conector, el cable largo con ruido y las piezas sueltas. Pierden la separación física como seguro, pero a cambio hay menos cosas que se puedan romper. Ustedes deciden cuál conviene para el examen.

#### Mi recomendación resumida

1. Mantengan la microSD.
2. Confirmen el controlador de la pantalla, o cambien a una ILI9341 de 2,8".
3. Cambien el PAM8403 por un MAX98357A, o acepten el audio por PWM con más ruido.
4. Cambien los pulsadores a unos grandes, o impriman tapas grandes.
5. Agreguen a la lista fuente de 1 A, perfboard y soldador.
6. Dejen solo una categoría + mensaje, como sugiere el profe.
7. Compren y prueben en protoboard antes de imprimir la carcasa.

### Actualización B.O.M

Respecto a nuestro proyecto y la lista de materiales, hicimos unos pequeños cambios en referencia a lo que salía en nuestro README del proyecto:

- cambiamos los botones pulsadores por botones tecla (idea de Carlo)
- agregamos las resistencias (220Ω x 3, 10KΩ x 5, 100KΩ x 2)
- agregamos la PCB perforada, porque usar una protoboard haría todo desorganizado con la conexión de los cables
- originalmente teníamos marcado la raspi como "0" porque técnicamente no tendríamos que comprar una ya que nos la dará la U (from what I understand), pero aún así decidí marcar que necesitamos una just in case, para evitar misunderstandings
- agregamos el filamento petg que vamos a usar para hacer la carcasa. preguntamos por el discord si algún otro grupo quería sumarse, y nos re ghostearon loco :skull:

#### Comentarios pre-purchase

- necesitan df-player mini
- no necesitan lector de micro SD
- Cómo grabar: usar el ADC con FIFO y DMA a una frecuencia fija, de 8 a 16 kHz para voz.
- No leer el ADC en un loop con pausas, porque el tiempo entre muestras queda irregular y el audio suena mal. Hay una biblioteca oficial de Arm que ya resuelve esto: microphone-library-for-pico, que trae ejemplos con micrófono analógico y con micrófonos PDM.

Ya que vamos a tener que usar DfPlayerMini, Seba Sáez nos recomendó unos repos de exámenes anteriores [2025-2 grupo-01](https://github.com/disenoUDP/dis8645-2025-2-proyectos/tree/main/00-examen/grupo-01) - [2025-2 grupo-05](https://github.com/disenoUDP/dis8645-2025-2-proyectos/tree/main/00-examen/grupo-05) - [2025-2 grupo-06](https://github.com/disenoUDP/dis8645-2025-2-proyectos/tree/main/00-examen/grupo-06)

Even more revised feedback:

- grupo-07, ocupen lector SD en vez de DfPlayerMini
- El grupo 07 no tiene DFPlayer. Tiene un lector microSD por SPI, que es justo lo que necesitan: la Pico lee y escribe la tarjeta directamente.
- Grabar: la Pico guarda el mensaje del cuidador como archivo WAV en la SD.
- Reproducir: la Pico lee de la misma SD los cuentos, la música y el mensaje.
Un solo camino de audio y un solo lugar donde están todos los archivos.

Lo que les cambiaría:

Reemplazar el PAM8403 por un MAX98357A. Sin DAC, la Pico tendría que sacar el audio por PWM, y eso suena con ruido. El MAX98357A recibe audio digital por I2S y amplifica, todo en un módulo de unos $1.100.
Poner la SD y la pantalla en buses SPI distintos (SPI0 y SPI1), como les comenté antes. Si no, cada redibujo de pantalla corta el audio.
Al grabar, usar un buffer grande (medio segundo o más) y crear el archivo con su tamaño completo antes de empezar, para evitar las pausas de escritura de la SD.

---

Seba Sáez nos hizo una quick presentation sobre su proyecto de título (i think)

Kineosóma (?) consta de un chaleco y una aplicación. El chaleco se conecta por bluetooth a la app y permite monitorear la postura del individuo.

El chaleco tiene los sensores en el interior para evitar el monitoreo sea visible.

La app tiene un main hub con la mascota, para el infante hay una parte de actividades para aprender sobre su postura, mientras el cuidador puede ver una parte más detallada del monitoreo

- Is it meant to be worn for the whole day?
  - Una decisión de diseño es que está echo en mente para que se pueda usar por la mayor parte del tiempo (Seba spoke some more about it but I missed it, sorry)

- (hicieron otra pregunta, me la perdí)
  - (también me perdí la respuesta :skull:)

- What about long-term use, how does it work as the kid grows up?
  - Se pensó una talla única, pero no se puede ya que debe ser algo close to skin tight para que la detección de la postura funcione bien

## lectura
