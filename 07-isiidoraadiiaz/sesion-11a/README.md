# sesion-11a

## apuntes sesión

# TRAZO — FASE 02
## Mejoras del código y organización del funcionamiento

### 1. ¿En qué nos enfocamos?

Después de desarrollar la primera estructura de TRAZO, comenzamos a enfocarnos en mejorar la organización del código y entender con más profundidad cómo funcionaban las clases que estábamos utilizando.

En esta etapa ya no se trataba solamente de conseguir que los potenciómetros entregaran valores, sino de tener un código más ordenado, reutilizable y fácil de modificar.

También comenzamos a revisar cómo podíamos integrar nuestro trabajo con el de otros grupos, especialmente porque existían componentes y funcionalidades que podían compartir una misma estructura de programación.

### 2. Unificación de las clases Perilla y Encoder

Al principio habíamos desarrollado dos clases independientes: `Perilla` y `Encoder`.

La clase Perilla estaba pensada para trabajar con los potenciómetros, mientras que Encoder permitía interpretar el movimiento de un encoder rotatorio.

Al conversar con el grupo 1, que necesitaba trabajar con un encoder, nos compartieron un repositorio del profesor que utilizaba una estructura para reunir ambas funcionalidades.

A partir de ese ejemplo comenzamos a adaptar el código para que una sola clase llamada `Perilla` pudiera reconocer y trabajar con ambos tipos de componentes.

Esto fue posible utilizando algo llamado **sobrecarga de constructores** en C++.

La idea es que una misma clase pueda tener distintas formas de inicializarse dependiendo de los parámetros que recibe.

Por ejemplo:

`Perilla volumen(26, 100);`

Esta inicialización corresponde a un potenciómetro. Recibe el número del pin y el límite de valores que queremos utilizar.

En cambio:

`Perilla selector(14, 15, 5);`

Esta corresponde a un encoder. Recibe los pines CLK y DT, además de la cantidad de opciones que puede seleccionar.

La clase guarda qué tipo de componente se está utilizando y, cuando se llama a la función `leer()`, ejecuta el procedimiento correspondiente.

Algo importante es que esta identificación no ocurre automáticamente por detectar el componente conectado. Nosotros indicamos qué tipo de objeto queremos crear mediante el constructor.

En nuestro caso, TRAZO utiliza la parte correspondiente al potenciómetro. La funcionalidad del encoder se mantiene dentro de la clase compartida porque también era necesaria para el otro grupo.

### 3. ¿Cómo funciona la lectura de un potenciómetro?

Para entender mejor nuestra clase, revisamos cómo se realizaba la lectura de los valores analógicos.

En el código utilizamos funciones del ADC de la Raspberry Pi Pico.

Una parte que me llamó la atención fue:

`adc_select_input(patita - 26);`

Al principio puede parecer extraño que tengamos que restar 26 al número del pin, pero esto ocurre porque el número GPIO y el número del canal ADC son identificadores diferentes.

Por ejemplo:

- GP26 corresponde al ADC0.
- GP27 corresponde al ADC1.
- GP28 corresponde al ADC2.

Por eso, al restar 26 podemos obtener el número del canal que necesitamos seleccionar.

También revisamos por qué realizamos cuatro lecturas consecutivas y calculamos su promedio.

Esto permite suavizar pequeñas variaciones en las señales analógicas y obtener una lectura más estable.

Después de obtener el valor, lo convertimos a una coordenada de pantalla utilizando la función `convertir()` que habíamos desarrollado anteriormente.

### 4. Detectar cuándo cambia la posición

Otra parte importante que revisamos fue cómo saber si el movimiento de una perilla realmente había producido un cambio en la posición del cursor.

En el código utilizamos:

`cambio = nueva != posicion;`

El operador `!=` significa distinto de.

Esto permite comparar la posición nueva con la anterior.

Si ambas son diferentes, la variable `cambio` queda en verdadero. Si son iguales, queda en falso.

Después actualizamos la posición:

`posicion = nueva;`

El orden de estas instrucciones es importante porque primero necesitamos comparar el valor nuevo con el anterior y solamente después reemplazarlo.

Esto también nos permite evitar actualizaciones innecesarias. Por ejemplo, si la lectura analógica cambia un poco, pero sigue representando el mismo píxel, no necesitamos actualizar la pantalla.

### 5. Control del tiempo: time_us_64()

Durante la revisión del código también estudiamos una función que nos permitió entender mejor cómo controlar los tiempos dentro del programa: `time_us_64()`.

Esta función pertenece al entorno de programación de la Raspberry Pi Pico y permite consultar el tiempo transcurrido desde el inicio del sistema en microsegundos.

En nuestro código utilizamos variables de tipo `uint64_t` para guardar referencias de tiempo.

Lo interesante es que esto nos permite ejecutar determinadas acciones cada cierto intervalo sin detener completamente el programa.

Por ejemplo, en vez de utilizar una pausa con `sleep`, podemos revisar constantemente cuánto tiempo ha pasado y ejecutar una acción cuando corresponde.

Trabajamos con dos intervalos diferentes:

- **20.000 microsegundos:** para leer las perillas aproximadamente cada 20 milisegundos.
- **200.000 microsegundos:** para imprimir información aproximadamente cada 200 milisegundos.

¿Por qué utilizar dos tiempos?

Porque no todas las acciones necesitan ejecutarse con la misma frecuencia.

Necesitamos leer las perillas más seguido para que el movimiento del cursor responda adecuadamente, pero no necesitamos imprimir información constantemente porque eso podría llenar demasiado rápido la salida de datos.

También entendí que estos intervalos no significan que las acciones ocurran exactamente al mismo tiempo. El programa revisa cada condición dentro del ciclo principal y ejecuta las que ya cumplieron su intervalo.

### 6. Funcionamiento de la pantalla OLED

Otra parte que revisamos fue cómo representar el movimiento de las perillas dentro de la pantalla.

La idea inicial era mostrar un punto que cambiara de posición cuando girábamos los controles.

Para esto utilizamos tres operaciones principales de la biblioteca SSD1306:

- `ssd1306_clear()`: limpia el contenido del búfer de la pantalla.
- `ssd1306_draw_pixel()`: dibuja un píxel en las coordenadas indicadas.
- `ssd1306_show()`: envía el contenido del búfer a la pantalla OLED.

Algo que aprendí en esta parte es que limpiar la pantalla en cada actualización tiene una consecuencia importante: el punto se mueve, pero no conserva el recorrido anterior.

Es decir, podemos observar la posición actual del cursor, pero todavía no estamos generando un dibujo permanente.

Para conseguir líneas necesitamos conservar la posición anterior y conectarla con la nueva mediante una función de dibujo de líneas.

Esto también nos ayudó a distinguir dos conceptos que parecen similares, pero no son lo mismo: mover un cursor y dibujar un recorrido.

### 7. Comprensión del encoder rotatorio

Aunque TRAZO utiliza principalmente potenciómetros, también revisamos el funcionamiento del encoder porque trabajamos en una clase que permite manejar ambos componentes.

Un encoder rotatorio funciona de manera diferente a un potenciómetro.

En lugar de entregar un valor analógico continuo según su posición, utiliza dos señales digitales llamadas CLK y DT.

Estas señales cambian entre 0 y 1 mientras giramos el componente.

Para reconocer la dirección del movimiento, el código compara los estados anteriores y actuales de ambas señales.

Una parte que estudiamos fue:

`(int(clk) << 1) | int(dt)`

Aquí utilizamos operaciones binarias para combinar las dos señales y obtener un número que representa su estado.

Como cada señal puede tener dos valores, existen cuatro combinaciones posibles: 00, 01, 10 y 11.

Después utilizamos una tabla de 16 posiciones para comparar el estado anterior con el actual y determinar si el encoder avanzó, retrocedió o no registró un movimiento válido.

También revisamos el operador XOR (`^`), que permite identificar cuándo ambos bits cambiaron simultáneamente. En ese caso, el código considera que ocurrió una transición inválida y reinicia el acumulado.

Esto me permitió entender que un encoder no funciona simplemente contando cada cambio de señal, sino que necesita interpretar una secuencia de estados para reconocer correctamente el sentido del giro.

### 8. Selección circular de opciones

Dentro de la clase también revisamos cómo se puede hacer que un encoder recorra una lista de opciones sin detenerse en los extremos.

Para esto utilizamos el operador módulo `%`, que entrega el resto de una división.

Por ejemplo, si tenemos cinco opciones, las posiciones disponibles son:

0, 1, 2, 3 y 4.

Cuando estamos en la posición 4 y avanzamos una vez más, queremos volver a la posición 0.

La operación:

`(posicion + direccion + maximo_) % maximo_`

permite realizar este comportamiento circular.

También funciona en sentido contrario: si estamos en la posición 0 y retrocedemos, volvemos a la última opción.

Aunque esta funcionalidad corresponde al encoder, entenderla nos ayudó a comprender cómo se pueden construir controles reutilizables dentro de una misma clase.

### 9. ¿Qué mejoramos respecto a la primera fase?

Al comparar ambas etapas, siento que el principal cambio fue pasar de preocuparnos por hacer funcionar cada componente a preocuparnos también por cómo estaba construido el programa.

En la primera fase nos enfocamos en:

- Entender las entradas analógicas.
- Leer los potenciómetros.
- Convertir valores en coordenadas.
- Corregir errores matemáticos.
- Mostrar un punto en la pantalla.

En la segunda fase comenzamos a profundizar en:

- La organización de las clases.
- La reutilización del código.
- La detección de cambios en las posiciones.
- El control de tiempos sin bloquear el programa.
- La lectura y el procesamiento de señales digitales.
- La lógica de selección circular de un encoder.

## encargos

## lectura

En esta parte me llamó mucho la atención cómo las matemáticas empiezan a mezclarse cada vez más con el arte y la arquitectura. Aparece el ejemplo de Max Ernst, que mezcla la Venus de Milo con una superficie matemática, creando algo que está entre una escultura y una figura geométrica. También me gustó cómo se explica que los catálogos matemáticos terminaron convirtiéndose en una especie de biblioteca de formas para artistas y arquitectos. Después aparecen Le Corbusier y Naum Gabo, que utilizaron estas ideas de maneras distintas: uno las llevó a sus proyectos arquitectónicos y el otro las transformó en esculturas usando líneas, hilos y estructuras. Me quedo con la idea de que las matemáticas no tienen por qué quedarse en cálculos o fórmulas, también pueden ser un punto de partida para experimentar, imaginar y crear nuevas formas.

"such techniques offered new ways of seeing, thinking, and making." — pág. 87

"The interplay between mathematical technique and intuition was Gabo’s field of creative invention." — pág. 92
