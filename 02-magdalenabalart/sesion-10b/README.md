# sesion-10b

Continuamos trabajando en la clase `Perilla`, esta vez enfocándonos en cómo utilizarla desde el `main.cpp` y comprobar que respondiera a las necesidades de ambos grupos.

Durante esta etapa avanzamos en:

- Implementar la lectura analógica mediante el ADC de la Raspberry Pi Pico 2 W.
- Configurar la cantidad de opciones que puede seleccionar cada perilla.
- Incorporar dos formas de utilizar la clase, según las necesidades de cada proyecto.
- Detectar cuándo cambia la posición del potenciómetro para evitar imprimir constantemente los mismos valores.

## 2. Pruebas para nuestro proyecto

Preparamos un código de prueba utilizando dos potenciómetros, uno para seleccionar emociones y otro para cambiar entre los modos de pantalla de nuestra máquina.

| Componente | GPIO utilizado en la prueba | Función | Opciones |
|---|---|---|---|
| Perilla de emociones | GP26 | Seleccionar entre las emociones | 4 |
| Perilla de navegación | GP27 | Cambiar entre los modos de pantalla | 3 |

Para comprobar su funcionamiento, utilizamos el método `seleccionarOpcion()` y agregamos una comparación con el valor anterior. De esta manera, la consola solo mostraría un nuevo mensaje cuando la persona cambiara de opción.

## 3. Adaptación para el grupo 04

También continuamos ajustando la clase para que el grupo 04 pudiera utilizarla sin modificar su estructura interna.

La principal diferencia era que ellos necesitaban trabajar con una cantidad mayor de posiciones, por lo que incorporamos la posibilidad de indicar ese número directamente mediante el método `posicion()`.

| Componente | Función | Posiciones |
|---|---|---|
| Perilla de caracteres | Recorrer los caracteres disponibles | 38 |
| Perilla de cursor | Seleccionar la posición del cursor | 81 |

Para este funcionamiento utilizamos:

- `leer()`: actualiza el valor del potenciómetro.
- `movio`: permite identificar si hubo un cambio en la lectura.
- `posicion()`: transforma el valor analógico en una posición según la cantidad indicada.

Esto permitió mantener una misma clase para ambos proyectos, aunque cada uno utilizara las perillas de manera diferente.

## 4. Desarrollo del main de prueba

Creamos un `main.cpp` para reunir ambos estilos de funcionamiento y revisar los resultados desde el monitor serial.

En este archivo:

- Inicializamos el ADC antes de crear los objetos `Perilla`.
- Configuramos cuatro objetos para representar las funciones de ambos grupos.
- Utilizamos GP26 y GP27 como conexiones físicas compartidas durante las pruebas.
- Incorporamos mensajes en consola para diferenciar las selecciones de nuestro grupo y del grupo 04.
- Agregamos una pausa de 50 ms entre las lecturas para controlar la frecuencia de actualización.

También revisamos cómo se identifican los pines en el código, aclarando la diferencia entre los números GPIO y los pines físicos de la Raspberry.

## 5. Resultados y pendientes

Con estos ajustes dejamos preparado un código de prueba que reúne las necesidades de ambos proyectos. La clase permite trabajar con una cantidad fija de opciones o indicar la cantidad de posiciones directamente al utilizarla.

Como siguiente paso, quedó pendiente comprobar el funcionamiento completo con los potenciómetros físicos, especialmente la precisión de las 38 y 81 posiciones solicitadas por el grupo 04, ya que al aumentar la cantidad de opciones disminuye el recorrido disponible para seleccionar cada una.

## encargos

## lectura
