# sesion-11a

## apuntes sesión

La clase anterior pudimos dejar el proyecto definido, el concepto y como imaginábamos el resultado final. También pudimos ordenar como queríamos que funcionara el código, acá es donde más surgieron las dudad y la verdad estábamos cayendo en el colapso, hasta que Aarón nos ayudo con un borrador y explicarnos la idea base de como sería nuestro código y que a la par también como teníamos que crear nuestro código para el otro grupo. Primero iniciamos creando las clases perilla.h y perilla.cpp. Para desarrollar el funcionamiento de la máquina comenzamos a organizar el código mediante clases. Esto nos permitió dividir el proyecto en diferentes responsabilidades, evitando concentrar todas las instrucciones dentro de un único archivo.

+ Una clase funciona como una estructura que agrupa atributos y métodos relacionados con un elemento o función del proyecto.

De esta manera, cada clase se encarga de una tarea específica y se comunica con las demás cuando es necesario.

En nuestro desarrollo distinguimos entre las clases creadas por nuestro grupo y las clases desarrolladas por otro grupo, que incorporamos como apoyo para el funcionamiento del dispositivo.

```
codigo/
├── main.cpp
  └── Boton.h              // clases de componentes
  └── Boton.cpp
  └── Perilla.h
  └── Perilla.cpp
  └── Led.h
  └── Led.cpp
  └── Pantalla.h
  └── Pantalla.cpp
  └── Emociones.h         // clases de modos de pantalla
  └── Emociones.cpp
  └── Mensajes.h
  └── Mensajes.cpp
  └── Juego.h
  └── Juego.cpp
  └── Rostros.h          // graficas de emociones

```

# Experiencia de uso

Encender el dispositivo: la persona presiona el botón de encendido para iniciar la máquina y activar la pantalla.

Seleccionar un modo: gira la perilla de navegación para recorrer los tres modos disponibles: Expresiones, Mensajes y Minijuego.
Explorar las emociones (Modo 01): gira la perilla de emociones para seleccionar una de las cuatro expresiones disponibles. El rostro de la pantalla cambia según la emoción seleccionada.
Explorar los mensajes (Modo 02): gira la perilla de navegación hasta el segundo modo. La pantalla muestra mensajes asociados a la emoción seleccionada anteriormente.
Iniciar el minijuego (Modo 03): gira la perilla de navegación hasta el tercer modo. La pantalla presenta el juego y se enciende una luz LED que indica que el botón de salto está habilitado.
Interactuar con el minijuego: presiona el botón de salto para que el personaje esquive los obstáculos. La dificultad del juego varía dependiendo de la emoción seleccionada en el Modo 01.
Cambiar de modo: gira nuevamente la perilla de navegación para acceder a cualquiera de los tres modos. La máquina conserva la última emoción seleccionada para utilizarla en los mensajes y el minijuego.
Apagar el dispositivo: presiona nuevamente el botón de encendido para desactivar la pantalla y finalizar la interacción.

## Clases desarrolladas por nuestro grupo

### 1. Perilla

Representa los potenciómetros físicos. Lee sus valores analógicos (0 a 4095) y los transforma en opciones para seleccionar modos, emociones y mensajes.

Sus funciones principales son leer el valor, calcular la opción seleccionada y detectar movimientos. También utiliza un umbral para evitar que pequeñas fluctuaciones eléctricas generen cambios involuntarios.

### 2. Led

Controla la luz indicadora del dispositivo. Cuenta con métodos para encenderla, apagarla y actualizar su estado según el modo seleccionado.

El LED permanece encendido únicamente durante el modo 03 (Juego).

### 3. Pantalla

Administra la pantalla OLED de 128 × 64 píxeles. Utiliza un espacio de memoria llamado buffer, donde se construyen los elementos gráficos antes de enviarlos a la pantalla.

Permite dibujar píxeles, rectángulos, imágenes y textos. Es utilizada por los tres modos para mostrar su contenido.

### 4. Emociones

Gestiona las cuatro emociones disponibles: felicidad, tristeza, enojo y calma.

Cada emoción tiene un nombre, un rostro pixelado y una dificultad asociada al minijuego. Esta clase permite seleccionar y representar las emociones en pantalla, además de entregar su dificultad al juego.

### 5. Mensajes

Administra los cinco mensajes motivacionales del modo 01. Permite seleccionarlos mediante la perilla, mostrarlos en pantalla y visualizar un contador de posición (por ejemplo, 2/5).

Los mensajes funcionan de manera independiente de las emociones.

### 6. Juego

Contiene la lógica del minijuego del modo 03. El personaje debe saltar obstáculos que avanzan desde la derecha de la pantalla.

Se encarga de controlar los saltos, movimientos, colisiones y puntuación. Además, adapta la velocidad y frecuencia de los obstáculos según la emoción seleccionada anteriormente.
Clases desarrolladas por otro grupo (Grupo 04)

### 7. Boton

Esta clase permite detectar las pulsaciones de los botones físicos y reconocer acciones como clic simple, doble clic o pulsación larga.

Incorpora un sistema de antirrebote (debouncing), que evita interpretar pequeñas variaciones eléctricas como múltiples pulsaciones.

En nuestro proyecto utilizamos:
- Botón 01 (GP14): enciende y apaga la máquina.
- Botón 02 (GP15): permite saltar durante el minijuego.



## encargos

## lectura

En este capítulo, Debord explica cómo el capitalismo transforma los espacios para adaptarlos a las necesidades del mercado y el consumo.

- Los lugares pierden sus características propias y se vuelven cada vez más similares. Esto también ocurre con el turismo, donde viajar se convierte en una experiencia de consumo.
- “La misma modernización que ha retirado del viaje el tiempo le ha retirado también la realidad del espacio”.
- El autor critica cómo el urbanismo moderno funciona como una herramienta de control y separación social.
- Las ciudades se organizan alrededor del trabajo, el automóvil y el consumo, mientras las personas pierden espacios de encuentro y relaciones sociales reales.
- “El urbanismo es la realización moderna de la tarea ininterrumpida que salvaguarda el poder de clase”.
Finalmente, plantea que es necesario recuperar el territorio como un espacio de construcción colectiva.
- Las personas deberían tener la posibilidad de decidir cómo habitar y transformar sus espacios, en lugar de que estos respondan únicamente a las necesidades del capitalismo.
- “La mayor idea revolucionaria referente al urbanismo no es ella misma urbanística, tecnológica o estética. Es la decisión de reconstruir íntegramente el territorio”.

1. “La dictadura del automóvil, producto-piloto de la primera fase de la abundancia mercantil, se ha inscrito en el terreno con la dominación de la autopista”.

2. “La mayor idea revolucionaria referente al urbanismo no es ella misma urbanística, tecnológica o estética. Es la decisión de reconstruir íntegramente el territorio”.
