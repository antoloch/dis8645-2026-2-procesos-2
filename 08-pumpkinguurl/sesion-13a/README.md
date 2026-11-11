# sesion-13a

## apuntes sesión

entrega proyecto 3 14b (previa)

13a y 13b trabajo conceptual

Vamos a hacer maquinas sonoras 

BRAINSTORMING

- Planta sensorial sonora.
- Monos que bailan y dan la hora con musiquita.
- Una pecera llena de agua con sensores que detectan cuando caen las gotas y generan un sonido.

QUE SI

1. Interactivo, tocar cosas, botones, sensores
2. Es tactil 
3. Es sutil
4. Buscamos revolver las emociones
5. Un usuario para la interacción  
6. Apoyado en una mesa 
7. Carcasa( en duda) 
8. Historias/relatos
9. Aumentar y disminuir volumen del parlante
10. Barato

 QUE NO

1. No es de madera
2. No es ensordecedor
3. No es estático¿¿¿
4. Audífonos
5. Sonido constante, solamente que se active por alguna acción 
6. Juego
7. Colgado en la pared
8. Sonido artificial
9. Costoso
10. Luces

otras ideas

- Máquina que te permite grabar un mensaje para la siguiente persona que la utilice, al terminar la interacción y volver a grabar un mensaje para la siguiente persona, se borra el mensaje anterior.

consejos, palabras bonitas, chistes, confesionario, etc.

- sonidos de acuerdo al clima del día

¿Cómo suena el día?. Una máquina que genere sonidos de acuerdo a la temperatura del día en cierto lugar geográfico. El usuario selecciona el día y un lugar del mundo, la máquina tendrá botones que almacenarán sonidos de las variaciones de temperatura los cuales se definirán por un promedio de aumento general entre temperaturas de acuerdo a la cantidad de botones que coloquemos y amplitudes de onda según número, por ejemplo; hoy la temperatura máxima será de 27°C, la máquina tendrá cuatro botones, desde que comienza el día hasta que se genera la temperatura máxima se dividirá en 4 rangos, entre esos rangos se genera un promedio entre las temperaturas que existieron y se define un sonido utilizando ese numero como amplitud de onda(?????????). El usuario podrá controlar el volumen también a través de una perilla

Se utiliza el rango semanal entregado por algún medio meteorológico y se carga en la raspi

Consulté con la IA la viabilidad del proyecto, es posible, pero confundí conceptos, de un principio pensaba en sonidos graves y agudos, pero eso tiene que ver con la frecuencia según el Claudio, la amplitud tenía que ver con sonar mas fuerte o mas despacio.

Creo que es la idea más completa que tenemos así que probablemente nos quedemos con ese, quien sabe. 

> Sí, es totalmente posible, y además cumple lo del parlante obligatorio. Hay un detalle conceptual que conviene arreglar antes de empezar (lo de los "?????????" tenía razón de existir).

## El problema: la temperatura no debería ser la amplitud

La **amplitud** de una onda es básicamente el **volumen**, y ustedes ya tienen una perilla para el volumen. Si la temperatura define la amplitud, un día de 27°C sonaría "más fuerte" que uno de 12°C, pero la perilla lo pisaría todo y no se notaría ninguna diferencia.

Lo que les conviene es que la temperatura defina la **frecuencia**, o sea qué tan **agudo o grave** suena:

- más frío → más grave
- más calor → más agudo

Así cada botón suena a una nota distinta y el día "se escucha" subiendo de grave a agudo a medida que calienta. Es mucho más claro de oír, que es el punto del proyecto.

## Cómo funcionaría (ejemplo)

Supongamos temperaturas por hora desde las 00:00 hasta la máxima (27°C a las 15:00), con 4 botones:

| Botón | Horas | Temps | Promedio | Frecuencia |
|---|---|---|---|---|
| 1 | 00–03 | 12, 11, 11, 10 | 11°C | ~253 Hz |
| 2 | 04–07 | 10, 10, 11, 13 | 11°C | ~253 Hz |
| 3 | 08–11 | 15, 18, 20, 22 | 18,8°C | ~509 Hz |
| 4 | 12–15 | 24, 25, 26, 27 | 25,5°C | ~731 Hz |

Con una fórmula simple tipo `f = 220 + (T − 10) × 33`, que lleva 10°C a 220 Hz (un La grave) y 30°C a 880 Hz. Si quieren que suene menos a alarma y más a música, pueden "redondear" cada frecuencia a la nota más cercana de una escala (pentatónica, por ejemplo), y así cualquier combinación de botones suena bonita.

## Cosas a decidir como grupo

1. **¿Qué pasa después de la máxima?** Su idea corta el día en la hora de la máxima, y esa hora cambia según el día, así que cada botón cubre una cantidad de horas distinta. Es más simple (y quizás más interesante) dividir el día completo de 00 a 23 en N partes. Así también se escucha cómo enfría en la noche.
2. **Elegir día y lugar.** Con la Pico no van a tener un mapa del mundo, así que lo realista es precargar unas 3 a 5 ciudades × 7 días y elegirlas con una perilla mostrando el nombre en la pantalla OLED (que ya saben usar del proyecto anterior 😉). Hagan contraste: Santiago, Oslo, Dubái, Sídney…
3. **Los datos.** [Open-Meteo](https://open-meteo.com) entrega temperatura por hora para cualquier lugar del mundo, gratis y sin registrarse. Descargan la semana, la pasan a un arreglo en C++ (`float temps[ciudad][dia][24]`) y queda guardada en la Pico. Si su placa es una **Pico W** podrían bajarla por WiFi en vivo, pero precargada es mucho más simple y no les falla en la presentación.

## Hardware

- **Parlante:** la Pico genera el sonido con **PWM** (onda cuadrada) con una sola función, `tone(pin, frecuencia)`. Es la forma más fácil de hacer sonido si nunca lo han programado.
- **Volumen:** lo más limpio es un módulo amplificador chico (PAM8403 o similar). Muchos traen su propia perilla de volumen, así que se resuelve sin código. Si la perilla tiene que leerse desde la Pico, se puede bajar el volumen variando el *duty cycle* del PWM, pero funciona más o menos y cambia un poco el timbre.
- **Botones:** N botones (4 está perfecto) + 1 perilla para volumen + 1 perilla para elegir ciudad/día + la pantalla.

**Resumen:** el proyecto es viable y no es tan difícil técnicamente. Lo único que tienen que cambiar es que la temperatura defina la frecuencia (tono) en vez de la amplitud.

Si quieren, les armo la estructura de clases en C++ (`Sonido.h/.cpp`, `Datos.h`, etc.) con la conversión temperatura → frecuencia ya lista, para que prueben en Wokwi.

## encargos

**Sonido que llama mi atención**

Hay dos sonidos que llaman particularmente mi atención, las llaves de mi mamá, y el sonido que hacen sus zapatos cuando camina, el tac tac de sus tacos.

Llaman mi atención porque al oírlos se que viene llegando. Suenan a mamá, a la mona. El sonido es inconfundible, si lo escucho y miro por la ventana, siempre es ella.

“ Uso los sonidos como indicios para identificar dichas cosas. Si están ocultas, los sonidos las revelarán” (Nunca vi un sonido, R. Murray Schafer)

Las llaves hacen clinclinclinclin, parece como el sonido de la magia en las películas. Sus zapatos suenan tac toc tac toc, en el libro dice que un mismo sonido no es igual cada vez que suena, siempre es diferente, pero si suena similar, pero aunque el modelo de zapato sea el mismo, y la persona que los usa es la misma, un paso suele sonar mas grave que el otro.

Hace poco leí un libro que se llama Dorayaki, en este le dan mucha importancia a escuchar, escuchar lo que pasa al rededor, escuchar como se cocinan los porotos rojos para hacer la pasta de an para que esta quede con buen sabor. Al final la persona que recalca la importancia de escuchar decía que realmente los porotos rojos no le decían nada pero “que si una cree que pueden ser escuchadas, quizás un día finalmente las oiga”. Los sonidos dan significado. Los sonidos demuestran vida. Estar vivo.

## lectura
