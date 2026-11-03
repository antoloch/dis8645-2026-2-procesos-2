# sesion-11a
Martes 27 de Octubre

## apuntes sesión

### Trabajo realizado

- Se trabajó en Wokwi para simular el funcionamiento del circuito.
- Se establecieron las conexiones de las perillas:
  - **GP26 (ADC0):** selección de emociones.
  - **GP27 (ADC1):** navegación entre pantallas.
- Se revisó cómo funcionan las entradas analógicas y cómo leer sus valores.
- Se comenzó a organizar el código utilizando programación orientada a objetos.
- Se decidió separar el código en varios archivos.
- Se definió que el proyecto tendría cinco emociones y tres pantallas.
- Se revisaron las conexiones GND de la Raspberry Pi Pico, incluyendo GND1 y GND8.

### Organización del código

Se comenzó a definir la estructura de las clases:

| Clase | Función |
|---|---|
| `Perilla` | Leer los valores analógicos de los potenciómetros. |
| `Emocion` | Almacenar el nombre, mensaje y dificultad de cada emoción. |
| `Modo` | Organizar los diferentes modos de funcionamiento. |
| `Juego` | Controlar la lógica principal del juego. |
| `Boton` | Controlar los botones del proyecto (desarrollado por el otro grupo). |

### Dificultades y soluciones

- **Organización del código:** Se decidió separar las clases en archivos para evitar que `main.cpp` quedara demasiado largo.
- **Lectura de potenciómetros:** Se revisó cómo funcionan las entradas analógicas de la Raspberry Pi Pico y cómo utilizarlas para seleccionar emociones y navegar.
- **Conexiones:** Se aclaró que los pines GND de la placa comparten la misma conexión a tierra.

### Resultado

Se estableció la estructura inicial del proyecto y se definió cómo funcionarían las dos perillas. También se comenzó a organizar el código en clases independientes para facilitar su desarrollo y mantenerlo ordenado.

### Próximos pasos

- Terminar de desarrollar las clases.
- Comprobar el funcionamiento de las dos perillas.
- Integrar el código de los botones desarrollado por el otro grupo.
- Organizar y simplificar el `main.cpp`.
- Continuar con la implementación de la pantalla y la lógica del juego.

## encargos

## lectura
