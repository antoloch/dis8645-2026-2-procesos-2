# sesion-14a

## apuntes sesión

hoy seguimos avanzando en nuestro ```proyecto-03```, el cual ahora tenemos la idea definida de este.

### descripción proyecto
El proyecto consiste en desarrollar un muestrario interactivo de sonidos cotidianos, que permita a los usuarios escuchar y reconocer diferentes sonidos de un entorno de manera sencilla.

El dispositivo contará con tres botones físicos, cada uno identificado con una categoría, como ejemplo: naturaleza, herramientas y sonidos de la feria.

Cada botón tendrá asignados tres sonidos diferentes.

Al presionarlo, se reproducirá un sonido de su categoría y, con cada nueva pulsación, se avanzará al siguiente, hasta completar los tres y volver al primero.

En total, el dispositivo contará con nueve sonidos, que se reproducirán individualmente mediante un parlante.

Para su funcionamiento se utilizará una Raspberry Pi Pico 2 W, botones y un sistema de reproducción de audio, integrados en una carcasa con etiquetas o íconos que faciliten su uso.

Como extra, estamos barajando la opción de incorporar una perilla para modificar el tono, siempre que sea técnicamente viable.

### bom
|Componente|Cantidad|Precio|Link|
|---|---|---|---|
|Raspberry Pi Pico|1|$14.990|<https://raspberrypi.cl/products/raspberry-pi-pico-2-w-con-headers>|
|Reproductor de Audio (DFPlayer Mini)| 1 |$2.990|<https://afel.cl/products/modulo-reproductor-mp3-tf-16p-v3-0>|
|Salida de Audio (Altavoz 3W, 8 Ohm)| 1 |$3.000|<https://afel.cl/products/mini-parlante-altavoz-de-3w>|
|Protoboard|1|$1.500|<https://afel.cl/products/mini-protoboard-400-puntos>|
|Cables dupont pack 20|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-macho>|
|Kit 10 Cables Caiman a Dupont Hembra|x|$2.500|<https://afel.cl/products/kit-10-cables-caiman-a-dupont-hembra?_pos=2&_sid=aec538813&_ss=r>|
|Pack 20 Cables de Conexión Macho Hembra|x|$1.000|<https://afel.cl/products/pack-20-cables-de-conexion-macho-hembra?_pos=1&_sid=bff72fae6&_ss=r>
|Potenciómetro B100k|2|$500|<https://afel.cl/products/potenciometro-10k-ohm>|
|Cable micro usb |1| $x|<https://www.masterg.cl/mgcamic/p?idsku=6>|
|Almacenamiento (Tarjeta MicroSD) | 1 | $x | x |
|Botones| 3 | $300 | <https://afel.cl/products/boton-tactil-tapa-12x12x7-3-interruptor?_pos=1&_sid=2ce280b32&_ss=r> |

### wokwi

### diagrama flujo
![imagen](imagenes/diagrama-proyecto03.png)

### pseudocódigo 
```
INICIO

  El soundboard reproduce nueve grabaciones propias.

  Hay tres botones y cada botón corresponde a una categoría con tres grabaciones.

  Cada grabación tiene tres versiones: grave, original y aguda.
  Una perilla regula el volumen y la otra elige la versión del sonido.

  Guardar el número de grabación que esté y avanzar al siguiente en la categoría.
  Al encender, las tres categorías empiezan en la grabación 1.
  Mantener el parlante en silencio hasta recibir una pulsación.

  REPETIR MIENTRAS LA CONSOLA ESTÁ ENCENDIDA

    Leer la perilla de volumen y ajustar la salida.
    El mínimo deja el parlante en silencio, incluso durante una pista.

    Detectar nuevas pulsaciones de los botones.

    SI se presionan los tres botones juntos
      Interrumpir la pista actual, si hay una en reproducción.
      Reproducir la grabación sorpresa.
      Conservar el número de la próxima grabación de cada categoría.
      Esperar a que se suelten los tres botones para aceptar otra pulsación.

    SI se registra una pulsación de un solo botón
      Identificar la categoría de ese botón.
      Consultar qué grabación sigue en esa categoría.
      Leer la perilla de tono y elegir grave, original o aguda.
      Interrumpir la pista actual, si hay una en reproducción.
      Dejar una pausa breve e iniciar la grabación elegida desde el comienzo.
      Avanzar la secuencia de esa categoría: 1, 2, 3 y nuevamente 1.

    SI termina la reproducción
      Volver a la espera silenciosa.

  FIN REPETIR

FIN
```
## encargos

[ensayo tiempo](https://docs.google.com/document/d/1ldegBF9RuInPusXr5iVuPF36Hbv8sV4ZaqrlZWKM800/edit?usp=sharing)

## lectura
### Mindstorms: Children, Computers and Powerful Ideas - Seymour Papert
- me encuentro aún en el capítulo 7 llamado "Logo´s Roots: Piaget and AI", terminé este capítulo.

*apuntes lectura*
