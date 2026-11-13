# sesion-12b

## apuntes sesión
- se realizó retroalimentación de nuestros proyectos 02

Misa habla sobre citar mejor y realizarlo desde ya, ya que si no sufriremos en seminario sos
- marco teórico/referencia - acreditarlo - citar
- argumentar y por qué
- software zotero

```
@book{nombre libro,
variables
}
```
Se habla sobre dudas, que nos ha gustado y que no, además de hablar sobre requisitos y opciones proyecto-03
- entregables cada clase evaluados (mini entregables)
- mantener grupos de a 4
- no hay más materia, se realizarán mini cápsulas
- si o si parlante
- pantalla opcional
- máquinas que van a sonar (pueden tener micrófono, grabar algo y reproducirlo)
- no buzzer si parlante
- no usar chip 386
  
### links varios
<https://www.brandnewnoise.com/>

<https://bleeplabs.com/the-bleep-drum/>

<https://hubot.cl/producto/modulo-conversor-adc-audio-estereo-pcm1808-24-bits-96-khz-i2s/>

<https://www.korg.com/cl/products/dj/volca_sample_okgo_edition/index.php>

<https://www.youtube.com/watch?v=UQ7BZlV_0zQ&t=108s>

<https://www.elf-audio.com/koala/how-it-was-made.php>

audio recorder module

isd 1820 -- voltaje -- no programable código 


### comentarios proyecto 02 
- nos dieron comentarios de nuestro proyecto, para ir mejorando para nuestra siguiente entrega
- mejorar descripción componentes
- interfásica adversarial -- mejorar
- documentación más extensa
- esquemático
- ortografía
- diagramas flujo

## encargos
- tratado de los objetos musicales Pierre Schafer <https://monoskop.org/images/1/1e/Schaeffer_Pierre_Tratado_de_los_objetos_musicales.pdf>
- leer el texto de schafer y escribir un párrafo en la bitácora donde describan un sonido que les llame su atención. Nombrar el sonido, y elaborar como aparece, intentar describir como suena, y su relación cultural <https://estefaniasoundscape.wordpress.com/wp-content/uploads/2017/10/nunca-vi-un-sonido_r-murray-schafer.pdf>

```nunca vi un sonido```
- creación ciego - creación sonora
- toda creación es original, todo sonido es nuevo
- sonoro es activo y generativo
- sonido no es comparable -- sensaciones, intuiciones, misterios
- creación por el sonido y analizado por la vista
- condenados a oír
- escuchar lo que percibimos, un conjunto de todos los sentidos, podemos saber la temperatura dependiendo de como se escuchan los pasos
- repetición: medio de la memoria para el sonido, retención, insiste
- intuición y sensación
- cuerpo convertido en oído
- dos objetos, un sonido

¿cómo suena el amor?
- ¿cómo aparece? -- aparece en el momento que sientes alguna conexión con una persona o quizá alguna persona realiza algún gesto hacia tu persona, ya sea un tecito, abrazo, avisarte de algo. Este tipo de conexión puede aparecer en el tiempo, generar un lazo con gente que compartes constantemente o quizá un clic instantáneo con alguien que ves por primera vez 
- ¿cómo suena? -- según mi percepción, este "sonido" se expresa a través de la sensación, la calidez y protección que te genera esa persona, en la cual puedes confiar y sentirte acogido, para mi se puede expresar en un sonido dulce, viento cálido con brisa, sonido de pájaros en la mañana
- relación cultural -- la relación principal es sentirlo mediante las personas, el entorno acogedor que te rodea
  
```ver grupo 1 tema parlantes```
- salida audio I2S
- raspi y amplificador <https://es.aliexpress.com/item/1005006823938000.html> MAX98357A
- lectura analógica ADC (pote)
- lectura digital (encoder KY-040 + pulsadores)

```protocolo I2S```
- intercambio de datos audio digital entre dispositivos de procesamiento de sonido
- señal reloj compartida, sincronizar transmisión de datos entre dispositivos y así se transmite el audio en serie
- transmisión audio alta calidad
- arquitectura controlador/dispositivo
- enviar MSB primero siempre (bit más significativo)
- tres líneas: reloj de bits, selección palabra (qué canal estéreo se utilizará izq o der) y datos

<https://www.digikey.com/en/maker/tutorials/2023/what-is-the-i2s-communication-protocol>

<https://www.keysight.com/blogs/en/tech/bench/2022/04/29/the-i2s-protocol-and-why-digital-audio-is-everywhere>

<https://es.wikipedia.org/wiki/I%C2%B2S>

```Christine Sun Kim```
- artista sonora intérprete
- dibujo, performance, video
- obras multidisciplinarias
  
<https://ghebaly.com/artists/christine-sun-kim/>

- creo que este video refleja muy bien la artista que es, me gusta el hecho de que implementa y toma el concepto de performance para sus obras, siento que al momento de realizar estas obras, es en un sentido intuitiva y sabe su objetivo. Además siendo sus obras un tipo de alerta para tomar consciencia, ya sea sobre el sobre empoderamiento del mundo de los oyentes y cómo esto es capaz de afectarle a ella y su comunidad, también aborda otros temas como el cambio climático y agotamiento político.
- escuchar desde la mirada
  
<https://www.youtube.com/watch?v=mqJA0SZm9zI>
