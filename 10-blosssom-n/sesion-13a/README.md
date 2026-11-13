# sesion-13a

## apuntes sesión

Hoy falté a clase pipipi, pero mi querido amado grupo me fue contando y de una selección de ideaa, bajamos dos para nuestro proyecto 3

### Fan Connect

Fan Connect sería un dispositivo para compartir música con personas que tienen gustos parecidos. la idea es que puedas elegir una canción o un pedacito de una biblioteca que ya viene cargada y mandárselo a otra persona. quien lo recibe puede escucharlo y responder con un emoji que se ve en la pantalla y tiene su propio sonido. esos emojis los diseñaríamos nosotros.

Lo que queremos probar es si mandar una canción puede ser una forma de empezar a hablar con alguien, incluso si todavía no lo conoces. es como cuando le mandas un tema a alguien porque crees que le va a gustar, o porque la canción expresa algo que te cuesta poner en palabras. acá la interacción sería con el objeto mismo, usando botones para navegar, mandar música y reaccionar.

Para mostrar el intercambio pensamos en dos dispositivos que se comuniquen por wifi, con un estilo muy tamagotchi. cada uno tendría una pantalla para ver las canciones, los emojis y las notificaciones, además de un parlante para escuchar los fragmentos y las reacciones. 



### Nido

Nido sería un dispositivo sonoro portátil para acompañar a niños cuando se sienten solos, asustados o extrañan a alguien. tendría una biblioteca de cuentos, música y sonidos tranquilos organizada por edades, y los cuidadores podrían elegir qué contenido queda disponible. el niño podría moverse entre las categorías con botones, escoger qué escuchar y reproducirlo o pausarlo.

También tendría un micrófono para que familiares o cuidadores puedan grabar mensajes y dejarlos guardados. por ejemplo, si un niño extraña a alguien antes de dormir, podría buscar un mensaje y escuchar esa voz, o elegir un cuento si prefiere distraerse un rato. la idea es que pueda hacerlo por su cuenta con una interfaz sencilla. Además, hoy entre tanta pantalla y estimulación para los niños, esta es una alternativa más amigable con su desarrollo.

Pensamos en una carcasa redondeada que sea cómoda de agarrar. la pantalla mostraría ilustraciones, personajes y lo que se está reproduciendo, así es más fácil cachar en qué parte estás y qué audio elegiste.





## encargo

### bom de Fan Connect

Esta lista es para un dispositivo. para probar que se puedan mandar canciones y reacciones entre dos personas, necesitamos armar dos.

| componente                           | cantidad | precio unitario | proveedor          |
| ------------------------------------ | -------: | --------------: | ------------------ |
| raspi pico reutilizada               |        1 |              $0 | universidad        |
| pantalla táctil tft de 2,4″ con spi  |        1 |         $10.000 | Afel               |
| mini parlante de 3 w / 4 Ω           |        1 |          $3.000 | Afel               |
| ampli pam8403 de 2 × 3 w             |        1 |          $1.000 | Afel               |
| kit de 5 pulsadores                  |        1 |            $500 | Afel               |
| microsd de 32 gb                     |        1 |          $6.000 | por definir        |
| power bank usb-c de 5 v / 3 a        |        1 |         $15.000 | por definir        |
| cable usb-c de alimentación          |        1 |          $1.500 | por definir        |
| cable y conexión de audio de 3,5 mm  |        1 |          $1.500 | por definir        |
| cables dupont y conectores           |   1 lote |          $3.000 | Afel / estimado    |
| carcasa impresa en 3d                |        1 |          $6.000 | fabricación propia |
| total estimado por dispositivo       |          |         $47.500 |                    |
| total estimado para dos dispositivos |          |         $95.000 |                    |

La microsd serviría para guardar los audios y los recursos gráficos. los fragmentos estarían cargados de antemano y las reacciones las haríamos nosotros. queda por confirmar que las raspi disponibles tengan wifi o cómo se lo vamos a agregar, además de revisar la salida de audio y cuánto aguanta la batería.

### bom de Nido

Para esta idea calculamos un solo dispositivo. el micrófono se suma porque acá sí queremos grabar mensajes, y la microsd serviría para guardar tanto la biblioteca de audios como las grabaciones.

| componente                          | cantidad | precio unitario | proveedor          |
| ----------------------------------- | -------: | --------------: | ------------------ |
| raspi pico reutilizada              |        1 |              $0 | universidad        |
| pantalla táctil tft de 2,4″ con spi |        1 |         $10.000 | Afel               |
| mini parlante de 3 w / 4 Ω          |        1 |          $3.000 | Afel               |
| ampli pam8403 de 2 × 3 w            |        1 |          $1.000 | Afel               |
| kit de 5 pulsadores                 |        1 |            $500 | Afel               |
| micrófono usb pequeño               |        1 |          $4.000 | por definir        |
| microsd de 32 gb                    |        1 |          $6.000 | por definir        |
| power bank usb-c de 5 v / 3 a       |        1 |         $15.000 | por definir        |
| cable usb-c de alimentación         |        1 |          $1.500 | por definir        |
| cable y conexión de audio de 3,5 mm |        1 |          $1.500 | por definir        |
| cables dupont y conectores          |   1 lote |          $2.500 | Afel / estimado    |
| carcasa impresa en 3d               |        1 |          $6.000 | fabricación propia |
| total estimado por dispositivo      |          |         $51.000 |                    |

Queda por probar si podemos grabar con el micrófono usb en la raspi que usemos y cómo vamos a mandar el audio al ampli. para la carcasa también tenemos que considerar cuánto espacio ocupan la pantalla, la raspi, el parlante y la batería.

### diagramas de flujo

**- Fan Connect**
  ```mermaid
flowchart TD

A@{shape: circ, label: "Se enciende<br>el dispositivo"}-->B(Suena sonido de bienvenida);
B-->C(Exploras la biblioteca musical);
C-->D(Eliges una canción o fragmento);
D-->E(Se envía a otra persona);
E-->F(La otra persona lo escucha);
F-->G(Responde con un emoji sonoro);
G-->H(Recibes su reacción);
H-->I(Descubren su afinidad musical);
I-->J{¿Siguen conversando?};
J--sí-->D;
J--no -->K@{shape: dbl-circ, label: "Termina la<br>interacción"};
```

**- Nido**

```mermaid
flowchart TD

A@{shape: circ, label: "Adulto prepara<br>el dispositivo"}-->B(Graba mensajes y sonidos);
B-->C(El niño lo lleva consigo);
C-->D{¿Qué necesita?};
D-->E(Cuento o música);
D-->F(Mensaje grabado);
D-->G(Sonido calmante);
E-->H{¿Se siente mejor?};
F-->H;
G-->H;
H--sí-->I@{shape: dbl-circ, label: "Sigue con su día o descansa"};
H--no -->J(Elige otra cosa);
J-->D;
´´´



## lectura
pags 138–143

En estas páginas el libro sigue con la poesía interactiva. En 4320, de Alan Sondheim, la persona usa un joystick para girar una figura en la pantalla. en la foto del libro se ve como un cubo de líneas, pero al moverlo la figura cambia y puede llegar a verse como un cuadrado. La captura muestra una sola posición de algo que se supone que vas probando.

En Voies de faits, de Jean-Marie Dutey, usas las flechas para mover una figura por una grilla de palabras. al principio solo se ve una parte, así que tienes que recorrerla para encontrar el resto del texto. en este caso, el orden en que vas leyendo depende de por dónde te mueves

>
>“The stimulating aspect of the work was interacting with the virtual object.” (p. 139)
>
>“the viewer uses arrow keys to explore the larger grid” (p. 141)
