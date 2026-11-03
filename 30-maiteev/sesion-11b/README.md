# sesion-11b
- Dandole los ultimos detalles al proyecto, se nos presento el problema de que nuestras animaciones eran bastante grandes para la pantalla y tuvimos que cambiar todas las animaciones ya que tambien se volvia muy facil perder.
- Se arreglo el json ya que habían unos problemas.
 ```cpp

{
  "version": 1,
  "author": "Juego Pico",
  "editor": "wokwi",
  "parts": [
    { "type": "wokwi-pi-pico", "id": "pico", "top": 0, "left": 0, "attrs": {} },
    {
      "type": "board-ssd1306",
      "id": "oled",
      "top": -160,
      "left": -60,
      "attrs": { "i2cAddress": "0x3c" }
    },
    { "type": "wokwi-breadboard-half", "id": "bb1", "top": 30, "left": -470, "attrs": {} },
    {
      "type": "wokwi-pushbutton",
      "id": "btnSelect",
      "top": 260,
      "left": -400,
      "attrs": { "color": "red", "label": "Seleccionar" }
    },
    {
      "type": "wokwi-pushbutton",
      "id": "btnFly",
      "top": 260,
      "left": -290,
      "attrs": { "color": "green", "label": "Volar" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r1",
      "top": -30,
      "left": -430,
      "attrs": { "value": "10000" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r2",
      "top": -30,
      "left": -320,
      "attrs": { "value": "10000" }
    },
    {
      "type": "wokwi-potentiometer",
      "id": "knobSpeed",
      "top": -97.3,
      "left": 307,
      "attrs": { "label": "Velocidad" }
    },
    {
      "type": "wokwi-potentiometer",
      "id": "knobChar",
      "top": -106.9,
      "left": 182.2,
      "attrs": { "label": "Personaje" }
    }
  ],
  "connections": [
    [ "pico:GND.1", "oled:GND", "black", [ "h-102", "v-195.15", "h74.17" ] ],
    [ "pico:3V3", "oled:VCC", "red", [ "v0" ] ],
    [ "pico:GP4", "oled:SDA", "blue", [ "h-121.2", "v-233.55", "h122.1" ] ],
    [ "pico:GP5", "oled:SCL", "yellow", [ "h-130.8", "v-252.75", "h121.87" ] ],
    [ "pico:3V3", "bb1:tp.1", "red", [ "v0" ] ],
    [ "pico:GND.2", "bb1:tn.1", "black", [ "v0" ] ],
    [ "r1:1", "bb1:tp.2", "red", [ "v0" ] ],
    [ "r1:2", "bb1:3t.a", "gray", [ "v0" ] ],
    [ "btnSelect:1.l", "bb1:3t.b", "green", [ "v0" ] ],
    [ "btnSelect:2.r", "bb1:tn.2", "black", [ "v0" ] ],
    [ "pico:GP14", "bb1:3t.c", "green", [ "v0" ] ],
    [ "r2:1", "bb1:tp.3", "red", [ "v0" ] ],
    [ "r2:2", "bb1:8t.a", "gray", [ "v0" ] ],
    [ "btnFly:1.l", "bb1:8t.b", "green", [ "v0" ] ],
    [ "btnFly:2.r", "bb1:tn.3", "black", [ "v0" ] ],
    [ "pico:GP15", "bb1:8t.c", "green", [ "v0" ] ],
    [ "knobSpeed:GND", "pico:GND.3", "black", [ "v0" ] ],
    [ "knobSpeed:VCC", "pico:3V3", "red", [ "v0" ] ],
    [ "knobSpeed:SIG", "pico:GP26", "orange", [ "v0" ] ],
    [ "knobChar:GND", "pico:GND.3", "black", [ "v0" ] ],
    [ "knobChar:VCC", "pico:3V3", "red", [ "v0" ] ],
    [ "knobChar:SIG", "pico:GP27", "purple", [ "v0" ] ]
  ],
  "dependencies": {}
 ```
  - Me confundi con unos cables y no me funcionaban las perillas :(( pero se arreglo jeje
  - Ademas el grupo 05 nos paso las perillas definitivas ouyeah
  - Después estuve agregando imágenes e información del proceso al readme y el flujo de trabajo que en un momento el readme no lo leía tan bien, y terminamos cosas que teníamos pendiente de la clase pasada.  
## apuntes sesión

## encargos

## lectura
