# sesion-11b

## apuntes sesión

# Actualización del proceso — 30 de octubre de 2026

## 14. Registro de pruebas y correcciones

Además de desarrollar las distintas versiones del código, comenzamos a llevar un registro en Excel para ordenar los avances, las pruebas y los errores que íbamos encontrando.

Esto nos sirvió para comparar versiones y entender mejor por qué algunas partes del código no funcionaban como esperábamos.

### Prueba E01 — Conversión del ADC

Quisimos comprobar qué pasaba si dividíamos antes de multiplicar al convertir las lecturas de las perillas en coordenadas.

Al trabajar con números enteros, la división eliminaba la parte decimal y muchas posiciones terminaban en cero.

Probamos con una lectura de 2048 y un máximo de 127. La versión incorrecta entregó 0, mientras que la corregida entregó 64, que era el resultado esperado.

Por eso, en el Código 4 dejamos la multiplicación antes de la división y agregamos redondeo.

### Prueba E02 — Selección del canal

También revisamos que cada potenciómetro utilizara su propio canal ADC.

En una de las variantes, la coordenada Y terminaba utilizando la lectura de X.

Para corregirlo, hicimos que cada instancia de Perilla seleccionara el canal correspondiente según su pin.

Probamos utilizando ADC0 = 0 y ADC1 = 4095. Con la corrección obtuvimos los valores esperados para ambos ejes.

### Prueba E03 — Retroceso del encoder

Durante el intercambio con el Grupo 1 también revisamos el funcionamiento del encoder.

Al probar cómo retroceder desde la primera opción de un menú, encontramos que una variante entregaba -1 en lugar de volver a la última opción.

Corregimos el cálculo sumando la cantidad total de opciones antes de aplicar el módulo.

Con esto conseguimos que el menú pudiera recorrer sus cinco opciones de manera circular.

Aunque TRAZO no utiliza encoder, esta prueba formó parte del desarrollo de la clase que trabajamos para compartir con el otro grupo.

### Organización de versiones

Fuimos guardando las diferentes versiones del código, los archivos de prueba y respaldos en Drive.

También utilizamos programas de prueba en C++ para revisar cálculos antes de trabajar con el circuito completo.

Así podíamos comprobar partes específicas de la lógica y conservar los códigos anteriores para comparar los cambios.

## 15. Código 5 — Integración final

En este avance logramos reunir las distintas funciones que habíamos estado desarrollando por separado.

Ya teníamos trabajada la lectura de las perillas y el movimiento del cursor, pero nos faltaba incorporar los botones del otro grupo.

Ellos nos compartieron su clase `BotonGrupo1`, que integramos con nuestra clase `Perilla` dentro de `main.cpp`.

Ahora el programa reúne el movimiento del cursor, los modos MOVER y DIBUJAR, el borrado y la representación de los trazos en pantalla.

### Integración de los botones

Utilizamos dos botones con funciones diferentes.

**Botón TRAZO (GP14):** permite alternar entre MOVER y DIBUJAR con cada pulsación, sin tener que mantenerlo presionado.

**Botón BORRAR (GP15):** limpia el dibujo y devuelve el programa al modo MOVER.

La clase del otro grupo también incorpora un filtro de rebote de 50 milisegundos, para evitar que una misma pulsación se registre varias veces.

### Ajustes de las perillas

Seguimos utilizando nuestra clase `Perilla` para controlar los ejes X e Y.

Mantuvimos el promedio de cuatro lecturas para suavizar los valores del ADC y la fórmula de conversión que corregimos en el Código 4.

Las perillas están conectadas a GP26 y GP27.

### Cambios en la pantalla

También organizamos mejor la pantalla OLED de 128 × 64 píxeles.

Reservamos las primeras 8 filas para mostrar si estamos en MOVER o DIBUJAR, dejando el resto disponible para los trazos.

Además, utilizamos un cursor con forma de cruz para identificar su posición.

### Cómo conservamos el dibujo

Para evitar que el cursor dejara marcas cuando solamente queremos movernos, trabajamos con dos espacios de memoria.

Uno guarda los trazos que dibujamos y el otro prepara la imagen que se muestra en pantalla, incluyendo el cursor y el estado actual.

Así podemos mover el cursor sin modificar las líneas que ya están dibujadas.

También incorporamos una función que conecta la posición anterior con la actual, permitiendo generar líneas continuas cuando movemos las perillas.

### Actualización del programa

Organizamos las lecturas para que los botones se revisen constantemente y las perillas cada 20 milisegundos.

La pantalla se actualiza cuando hay cambios que mostrar.

Además, utilizamos el monitor serial para revisar las coordenadas, las lecturas de los potenciómetros y el estado del dibujo.

## 16. Archivos y simulación

Para mantener el código ordenado, utilizamos archivos separados según su función.

- `main.cpp`: reúne el funcionamiento principal.
- `Perilla.h` y `Perilla.cpp`: contienen nuestra clase de potenciómetros.
- `BotonGrupo1.h` y `BotonGrupo1.cpp`: contienen la clase de botones del otro grupo.
- `ssd1306.h`, `ssd1306.c` y `font.h`: permiten trabajar con la pantalla.
- `diagram.json`: contiene las conexiones del circuito en Wokwi.
- `CMakeLists.txt`: contiene la configuración de compilación.

## encargos

## lectura
