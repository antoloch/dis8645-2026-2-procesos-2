# sesion-10b

## apuntes sesión

# TRAZO — FASE 01
## De la idea al primer funcionamiento

### 1. ¿De dónde partimos?

Para comenzar con TRAZO, primero tuvimos que definir qué queríamos construir y cómo nos imaginábamos que funcionaría. La idea era crear un dispositivo que permitiera dibujar digitalmente utilizando dos perillas, en vez de los controles tradicionales como un mouse o un lápiz digital.

El proyecto utiliza una Raspberry Pi Pico, dos potenciómetros y una pantalla OLED. Cada potenciómetro controla un eje diferente: uno modifica la posición horizontal (X) y el otro la vertical (Y).

Lo interesante de esta propuesta es que tenemos que coordinar ambos movimientos para conseguir un recorrido dentro de la pantalla. No es exactamente lo mismo que dibujar con un mouse, porque aquí el movimiento depende del giro de dos controles independientes.

También definimos tres acciones principales para el dispositivo:

- MOVER: desplazar el cursor sin dibujar.
- DIBUJAR: conectar las posiciones del cursor para generar líneas.
- BORRAR: limpiar la pantalla y comenzar nuevamente.

### 2. Nuestro referente: MacPaint

Antes de comenzar a programar, investigamos cómo habían funcionado algunos de los primeros programas de dibujo digital.

Uno de los referentes que más nos llamó la atención fue MacPaint, desarrollado por Bill Atkinson y lanzado en 1984 junto con el primer Macintosh.

Me interesó especialmente entender cómo algo tan cotidiano actualmente, como dibujar en un computador, necesitó desarrollar una nueva forma de interacción.

MacPaint permitía dibujar utilizando un mouse. El computador recibía información sobre su movimiento y, dependiendo de la herramienta seleccionada, transformaba ese recorrido en píxeles dentro de la pantalla.

De este referente rescatamos principalmente la relación entre movimiento físico y resultado digital. Queríamos experimentar con el mismo principio, pero cambiando el dispositivo de entrada: en vez de mover un mouse, utilizaríamos dos potenciómetros.

Esto también nos permitió entender que el dibujo digital no ocurre directamente por el movimiento de un objeto sobre una pantalla. Existe un proceso en el que el dispositivo recibe información, la interpreta y finalmente la representa visualmente.

### 3. ¿Cómo funcionan nuestros componentes?

Para entender cómo llevar la idea a la práctica, tuvimos que investigar qué hacía cada componente.

**Raspberry Pi Pico**

Es el microcontrolador principal. Se encarga de recibir las señales de los potenciómetros, procesar sus valores y enviar las instrucciones necesarias para mostrar la información en la pantalla.

**Potenciómetros**

Funcionan como resistencias variables. Al girarlos cambia el voltaje de salida, lo que permite obtener distintos valores mediante las entradas analógicas de la Pico.

En nuestro caso utilizamos dos:

- Potenciómetro X: controla el desplazamiento horizontal.
- Potenciómetro Y: controla el desplazamiento vertical.

La Raspberry Pi Pico tiene un conversor analógico-digital (ADC), que transforma esas señales en valores numéricos. Trabajamos con un rango de 0 a 4095, que corresponde a la resolución de 12 bits del ADC.

**Pantalla OLED SSD1306**

Es el medio donde representamos el movimiento y los dibujos. Trabajamos con una resolución de 128 × 64 píxeles, por lo que las coordenadas deben mantenerse dentro de ese espacio.

Por ejemplo, en el eje horizontal podemos utilizar posiciones desde 0 hasta 127, mientras que en el vertical podemos utilizar desde 0 hasta 63.

### 4. Primer acercamiento a la programación

Una vez definida la idea, comenzamos a desarrollar el código por partes. No quisimos programar todas las funciones de una sola vez, porque necesitábamos entender cómo se comunicaban los componentes y detectar los errores con mayor facilidad.

**Código 1 — Planificación (25/10/2026)**

Primero desarrollamos una estructura utilizando comentarios. Todavía no buscábamos que el dispositivo funcionara, sino ordenar las acciones que debía realizar el programa.

Esto nos permitió pensar en el orden de inicialización, lectura de controles, cálculo de coordenadas, dibujo y borrado antes de comenzar a escribir las instrucciones.

**Código 2 — Estructura inicial (26/10/2026)**

Después pasamos a una estructura básica de programación. Trabajamos con un archivo `main.cpp` y un `CMakeLists.txt`.

También comenzamos a configurar el ADC, que necesitábamos para obtener las lecturas de los potenciómetros.

En esta versión todavía no incorporábamos el movimiento del cursor. El objetivo era tener una base desde donde seguir construyendo.

**Código 3 — Lectura de las perillas**

En esta etapa comenzamos a trabajar con la clase `Perilla`, que nos permitiría leer los valores de los potenciómetros y convertirlos en coordenadas.

La intención era que al girar las perillas pudiéramos observar cómo cambiaba la posición de un punto en la pantalla.

Aquí apareció un error importante en la conversión matemática.

Estábamos dividiendo el valor leído por 4095 antes de multiplicarlo por el límite de la pantalla. Como trabajábamos con números enteros, la división descartaba los decimales y entregaba cero para casi todas las lecturas.

Por ejemplo:

2048 / 4095 = 0

Aunque matemáticamente el resultado está cerca de 0,5, al realizar la operación con enteros se pierde esa parte decimal.

Esto provocaba que el punto permaneciera en la posición inicial casi todo el tiempo, hasta que la lectura alcanzaba su valor máximo.

**Código 4 — Corrección de la conversión**

Para solucionar el problema cambiamos el orden de las operaciones: primero multiplicamos y después dividimos.

La fórmula quedó de esta manera:

`(valor * limite + 2047) / 4095`

También incorporamos el redondeo, utilizando 2047, que corresponde a la mitad entera de 4095.

Con esto conseguimos que la conversión fuera proporcional y que los valores intermedios pudieran representarse correctamente dentro de la pantalla.

Esta corrección me ayudó a entender algo importante: a veces el problema no está en la lectura del componente, sino en cómo estamos procesando matemáticamente la información que recibimos.

### 5. ¿Por qué trabajamos con una clase Perilla?

Durante esta etapa comenzamos a separar las funciones del código mediante una clase propia llamada `Perilla`.

La idea era evitar que toda la lectura de los potenciómetros estuviera escrita directamente dentro del archivo principal.

La clase se encarga de trabajar con el pin correspondiente, leer el valor del ADC, convertirlo a una posición y detectar si esa posición cambió.

También incorporamos un promedio de cuatro lecturas para suavizar pequeñas variaciones que podían aparecer al leer los potenciómetros.

Esto es útil porque las señales analógicas pueden presentar pequeñas fluctuaciones. Al promediar varias lecturas reducimos parte de esas variaciones, aunque no eliminamos completamente el ruido.

### 6. Bibliotecas y herramientas

Durante esta primera fase también investigamos cómo utilizar bibliotecas externas.

Las bibliotecas son conjuntos de funciones y herramientas de programación que permiten incorporar funcionalidades desarrolladas previamente. Esto evita que tengamos que construir absolutamente todo desde cero.

En nuestro caso, investigamos alternativas gratuitas disponibles en GitHub, especialmente para controlar la pantalla OLED.

Es importante diferenciar las herramientas que investigamos de las que realmente utilizamos:

- **Pico SDK:** entorno de desarrollo utilizado para programar la Raspberry Pi Pico en C/C++.
- **hardware_adc:** módulo del Pico SDK que permite configurar y leer las entradas analógicas.
- **SSD1306 de David Schramm:** implementación en C utilizada para controlar la pantalla OLED mediante los archivos `ssd1306.c` y `ssd1306.h`.
- **font.h:** archivo de fuentes utilizado por la implementación de pantalla.

También revisamos alternativas como Adafruit GFX y Adafruit SSD1306, que ofrecen herramientas para dibujar y controlar pantallas gráficas, pero no debemos confundirlas con la implementación que quedó en el código revisado.


## encargos

## lectura
