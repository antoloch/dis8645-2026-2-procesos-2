# sesion-10a

## apuntes sesión

## Desarrollo del componente Perilla

Durante esta jornada trabajé en la estructura y programación de la clase `Perilla` para la Raspberry Pi Pico 2 W. La idea era desarrollar un componente que permitiera leer los valores de un potenciómetro y utilizarlos para seleccionar distintas opciones.

Además de las necesidades de nuestra máquina de emociones, trabajamos en adaptar la clase para el **grupo 04**, que necesitaba utilizar dos perillas con una mayor cantidad de posiciones.

### 1. Requerimientos del grupo 04

| Componente | Función | Cantidad de posiciones |
|---|---|---|
| Perilla de caracteres | Recorrer y seleccionar caracteres | 38 |
| Perilla de cursor | Cambiar la posición del cursor | 81 |
| Detección de movimiento | Actualizar la selección cuando se mueve la perilla | Variable |
| Configuración de rangos | Definir la cantidad de opciones desde el `main.cpp` | Variable |

### 2. Trabajo realizado

- Organizamos la clase en `Perilla.h` y `Perilla.cpp`, separando los atributos y métodos de su implementación.
- Trabajamos en la lectura analógica del potenciómetro y en la conversión de sus valores a distintas posiciones.
- Incorporamos la posibilidad de definir la cantidad de opciones al utilizar la perilla, sin tener que modificar la clase.
- Utilizamos el atributo `movio` para detectar cambios en la posición y evitar actualizaciones innecesarias.
- Preparamos un ejemplo en `main.cpp` para comprobar cómo funcionaría con los 38 caracteres y las 81 posiciones solicitadas.

### 3. Adaptación para ambos proyectos

Una de las cosas que tuvimos que resolver fue que ambos grupos necesitábamos usar el mismo componente de formas distintas.

En nuestra máquina de emociones utilizamos una perilla para seleccionar entre **3 modos de pantalla** y otra para recorrer **4 emociones**. En cambio, el grupo 04 necesitaba dividir el recorrido del potenciómetro en 38 y 81 posiciones.

Por eso trabajamos en una estructura más flexible, donde cada grupo pudiera indicar cuántas opciones necesitaba al llamar a la función `posicion()`.

```cpp
perillaSelector.posicion(38);
perillaCursor.posicion(81);
```

## encargos

## lectura
