# sesion-11a

Durante esta etapa comenzamos a reunir los componentes que habíamos programado por separado para probarlos en conjunto en la Raspberry Pi Pico 2 W.

Hasta ese momento habíamos trabajado principalmente en las clases individuales, por lo que una de las principales dudas era cómo se comportarían al ejecutarse todas desde un mismo `main.cpp`.

Nos enfocamos en organizar la lógica general de la máquina de emociones y definir cómo debían relacionarse los botones, las perillas, el LED y la pantalla OLED.

### Lógica de funcionamiento

| Componente | Función | Condición |
|---|---|---|
| Botón de encendido | Encender y apagar la máquina | Disponible en todo momento |
| Perilla de navegación | Seleccionar entre los tres modos | Máquina encendida |
| Perilla de emociones | Seleccionar mensajes o emociones | Modos 01 y 02 |
| Pantalla OLED | Mostrar mensajes, rostros y juego | Según el modo seleccionado |
| LED | Indicar que el juego está disponible | Solo en modo 03 |
| Botón de salto | Controlar el salto del personaje | Solo en modo 03 |

Una de las cosas que tuvimos que resolver fue cómo evitar que todos los componentes funcionaran al mismo tiempo. Por ejemplo, el botón de salto no debía intervenir en los modos de mensajes o emociones, mientras que el LED tenía que permanecer apagado fuera del juego.

## 2. Primeros problemas y dudas

Al comenzar la integración aparecieron algunas dificultades que no habíamos considerado cuando programábamos los componentes de manera individual.

- **Organización del `main.cpp`:** tuvimos que ordenar las funciones para que la máquina identificara primero si estaba encendida, después el modo seleccionado y finalmente qué componentes debían funcionar.
- **Cambios de modo:** nos preocupaba que al girar la perilla quedaran activos elementos del modo anterior, especialmente el LED o el juego.
- **Lecturas de los botones:** necesitábamos diferenciar una pulsación de un botón mantenido presionado, para evitar ejecutar acciones más de una vez.
- **Pantalla OLED:** teníamos dudas sobre cómo actualizar las imágenes sin estar borrando y dibujando constantemente el mismo contenido.
- **Compatibilidad de los componentes:** aunque las clases estaban preparadas individualmente, todavía faltaba comprobar que funcionaran juntas en la placa física.

## 3. Primeras compilaciones y errores

Al trasladar el código al proyecto de Raspberry comenzamos a encontrarnos con errores de configuración y compilación.

Uno de los primeros problemas fue organizar todos los archivos `.h` y `.cpp` dentro del proyecto y agregarlos correctamente al `CMakeLists.txt`.

También apareció un error relacionado con la clase `Emociones`, porque la función `dibujarEmocion()` estaba declarada sin parámetros en `Emociones.h`, pero en `Emociones.cpp` necesitaba recibir un objeto de la clase `Pantalla`.

Para solucionarlo revisamos la relación entre ambas clases y corregimos su declaración:

```cpp
void dibujarEmocion(Pantalla &pantalla);
```

Esto nos permitió comprender mejor la importancia de mantener las mismas declaraciones entre los archivos `.h` y `.cpp`, especialmente cuando un componente necesita utilizar funciones de otra clase.

## 4. Paso del código a la Raspberry

Una vez corregidos los errores de compilación, comenzamos a probar el programa directamente en la Raspberry Pi Pico 2 W.

Este proceso también presentó dificultades, principalmente al generar y transferir el archivo `.uf2` desde VS Code. Tuvimos que revisar la estructura del proyecto, volver a configurar CMake y generar una compilación limpia.

Después logramos cargar el programa mediante BOOTSEL y comprobar su ejecución utilizando el monitor serial.

Durante las primeras pruebas aparecieron otros problemas:

- La pantalla OLED no respondía porque sus conexiones físicas SDA y SCK no coincidían con los GPIO configurados en el código.
- Los botones de encendido y salto estaban conectados físicamente al revés.
- El LED no encendía inicialmente, por lo que revisamos su conexión y agregamos mensajes de diagnóstico.
- El botón de salto no siempre respondía con una sola pulsación, lo que dejó pendiente revisar su detección dentro del juego.

## 5. Aprendizajes y siguientes ajustes

Estas pruebas nos permitieron identificar diferencias entre tener un código que compila correctamente y conseguir que funcione como esperamos en el circuito físico.

También comenzamos a utilizar el monitor serial como una herramienta de diagnóstico, incorporando mensajes para comprobar cuándo se activaban los modos y si el programa estaba llegando a determinadas funciones.

Los principales puntos pendientes fueron:

- Revisar la respuesta del botón de salto para conseguir una acción por pulsación.
- Comprobar el funcionamiento completo de los tres modos en la pantalla.
- Verificar que los componentes respondan correctamente después de cambiar entre modos.
- Continuar ajustando las conexiones físicas y la comunicación entre componentes.

En general, este avance estuvo marcado por las primeras pruebas de integración y la resolución de errores. Nos permitió pasar de tener componentes programados por separado a comenzar a probar el funcionamiento de la máquina como un sistema completo, identificando qué partes ya funcionaban y cuáles necesitaban ajustes.

## apuntes sesión

## encargos

## lectura
