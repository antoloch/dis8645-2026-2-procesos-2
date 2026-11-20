# grupo-07

## proyecto-03

### Integrantes

- Isidora Díaz [@isiidoraadiiaz](https://github.com/isiidoraadiiaz)
- Natalia Gutiérrez [@blosssom-n](https://github.com/blosssom-n)
- Carlo Martínez [@zaaaiko](https://github.com/zaaaiko)
- Natalia Pilar [@sz-mada](https://github.com/sz-mada)

### Dispositivo sonoro para acompañar a infancias

El proyecto consiste en un dispositivo sonoro pensado para acompañar a infancias de 3 a 4 años durante momentos específicos del día, especialmente cuando están en su habitación, descansando o preparándose para dormir. Esta primera versión no es portátil: el dispositivo está pensado para permanecer en un lugar fijo, como un velador.

El prototipo tiene tres botones físicos principales: uno para cuentos, otro para música y un tercero para el mensaje grabado por el cuidador. Al presionar uno de los botones en Wokwi, el sistema activa la categoría correspondiente. En cuentos y música, volver a presionar el mismo botón permite pasar al siguiente contenido en la simulación. Cuando se implemente el audio real, cada botón iniciará la reproducción de un audio de su categoría.

La interfaz no depende de que la infancia sepa leer: los botones permiten seleccionar una categoría sin navegar por un menú y la pantalla muestra la acción que se realiza mediante símbolos provisorios. Cada botón tendrá un personaje o ícono asociado a su categoría. La idea es mantener la misma relación entre el botón físico, el símbolo en pantalla y la respuesta sonora, que todavía está pendiente de implementar.

En esta entrega probamos la interacción en Wokwi y dejamos la carcasa para más adelante. La sugerencia del profe es dejar una sola categoría además del mensaje. Para el examen todavía tenemos que decidir si nos quedamos con cuentos y mensaje. Por ahora mantenemos las tres categorías en la simulación y la selección de los audios sigue pendiente.

### Investigación y contexto de uso

#### Contexto e inspiración

Partimos de una pregunta: ¿qué puede tener a mano une infante de 3 a 4 años cuando se siente sole o extraña a alguien que le cuida? La Organización Mundial de la Salud (2025, p. 4) explica que la soledad puede aparecer cuando la conexión que una persona siente queda por debajo de la que desea o necesita. Imaginamos, por ejemplo, a une infante que juega en su pieza una tarde y echa de menos a su papá mientras su abuela le cuida.

También revisamos el abandono infantil. En una guía de UNICEF Chile, Arriagada y Valdebenito (2011, p. 47) lo describen como una forma extrema de negligencia. La investigación sobre negligencia destaca la importancia de contar con personas que respondan de manera estable a las necesidades del infante (Center on the Developing Child at Harvard University, s.f.). Eso nos ayuda a precisar el punto de partida del dispositivo: ya existe un vínculo de cuidado y la propuesta busca que alguien cercano pueda dejarle un mensaje para escuchar mientras juega, descansa o se prepara para dormir.

#### Usuario objetivo y contexto de uso

El público objetivo corresponde a infancias de 3 a 4 años que pueden necesitar acompañamiento durante momentos de soledad, miedo o incertidumbre dentro de su habitación.

El dispositivo está pensado para utilizarse en un contexto doméstico y fijo. En el uso previsto, el cuidador lo deja preparado en el velador y la infancia escoge entre cuentos, música o el mensaje grabado sin tener que utilizar un celular ni navegar por un menú complejo.

No asumimos que la infancia sabe leer o que comprende una interfaz basada únicamente en texto. En la simulación, las categorías se distinguen por sus botones y por las señales visuales de la pantalla. Los personajes definitivos y las respuestas de audio todavía están pendientes. La pantalla sirve de apoyo, pero la acción principal se tiene que entender a partir de los botones físicos.

Nos enfocamos en los 3 a 4 años porque en esa etapa ya aparecen habilidades que podemos aprovechar. A los 3 años, muches niñes pueden reconocer qué está pasando en una imagen. A los 4, muches pueden anticipar qué viene en un cuento que ya conocen o inventar situaciones con personajes durante el juego. Como estas habilidades se desarrollan a ritmos distintos, queremos que la interacción se pueda entender a través de señales que se puedan ver, tocar y escuchar.

#### Por qué incluimos una voz familiar

Pensamos en une infante que extraña a una persona que participa en su cuidado cotidiano. Queremos que pueda elegir escuchar un saludo que esa persona le dejó. Nuestra hipótesis es que volver a oír una voz conocida puede darle una sensación de compañía en ese momento.

#### ¿Qué aporta escuchar una voz conocida?

Sesame Workshop (s.f.) propone dejar en cada hogar un retrato de la persona que está en el otro, para que el infante pueda mirarlo cuando la extrañe. Encontramos un ejemplo parecido en sonido: Easterseals Vermont (2024) describe un caso de acogimiento familiar en el que su equipo graba a una mamá leyendo un cuento y reproduce su voz durante el trayecto a la guardería después de las visitas. Ese caso inspira la decisión de darle un botón propio al mensaje, junto a los de cuentos y música. Cuando el audio esté implementado, cada infante podrá elegir escuchar directamente un saludo, una frase o un pedacito de canción grabado por una persona de confianza.

En nuestro proyecto queda por explorar si la grabación puede dar una sensación de compañía en un momento cotidiano de soledad. Con el primer prototipo queremos comprobar si es fácil dejar, encontrar y reproducir el mensaje.

### Investigación de referentes

#### 01. Yoto Player — La pantalla confirma qué elegimos

**Contexto.** El Yoto Player de cuarta generación es un reproductor infantil que permite elegir audios mediante tarjetas o un control físico. La pantalla muestra imágenes de los títulos al recorrerlos e íconos para identificar capítulos o pistas durante la escucha.

**Qué rescatamos.** Nos interesa que la pantalla confirme lo que la infancia acaba de hacer. Si aprieta el botón de cuentos, queremos que aparezca el mismo símbolo del botón cuando comienza la narración.

![Yoto Player](./imagenes/01-yoto-player.png)

*Yoto Newsroom, 2026. Imagen de prensa del Yoto Player 4th Gen y Yoto Mini 4th Gen.* [Yoto Newsroom](https://us.yotoplay.com/press/en-US/270385-yoto-launches-yoto-player-4th-gen-and-yoto-mini-4th-gen-its-biggest-hardware-upgrade-yet/).

#### 02. Toniebox — Una pista antes de escuchar

**Contexto.** La Toniebox reproduce el audio asociado a una figura cuando se coloca sobre el equipo. Por ejemplo, una figura de Pikachu da acceso a historias de Pokémon.

**Qué rescatamos.** La figura da una pista sobre lo que va a sonar antes de que empiece el audio. Queremos trasladar esa idea a los dibujos de los tres botones.

![Toniebox con Pikachu](./imagenes/02-toniebox-pikachu.png)

*tonies, s.f. Imagen de producto de Toniebox 2 con figura de Pikachu.* [tonies](https://us.tonies.com/products/pokemon-pikachu-tonie).

#### 03. BIGmack — Un botón para un mensaje

**Contexto.** BIGmack es un dispositivo de apoyo a la comunicación. Al presionar su superficie grande reproduce un único mensaje grabado.

**Qué rescatamos.** El dibujo está justo donde ocurre la acción. Aplicamos esa idea al botón del mensaje, cuyo símbolo estará en la superficie que se aprieta.

![BIGmack](./imagenes/03-bigmack.jpg)

*BoundlessAT, s.f. Fotografía de BIGmack, dispositivo de comunicación de un solo mensaje.* [BoundlessAT](https://www.boundlessat.com/Communication/Basic-Communicators/LITTLEmack-BIGmack-Communicators).

#### 04. Lovebox — Dejar un mensaje para más tarde

**Contexto.** Lovebox Photo Messenger es una caja que recibe mensajes, fotos y dibujos enviados desde una aplicación. Cuando llega un mensaje, el corazón exterior gira.

**Qué rescatamos.** Nos interesa que preparar un mensaje y encontrarlo puedan ocurrir en momentos distintos. Alguien cercano puede grabar un saludo antes de salir y la infancia puede volver a escucharlo después.

![Lovebox Photo Messenger](./imagenes/04-lovebox-photo-messenger.png)

*Lovebox, s.f. Fotografía de Lovebox Photo Messenger.* [Lovebox](https://en.lovebox.love/products/lovebox-color-photo).

#### 05. Whispering Table — El lugar también forma parte de la escucha

**Contexto.** Whispering Table es una instalación creada por TheGreenEyl para una exposición en el Museo Judío de Berlín en 2009. Las piezas de cerámica cuentan relatos cuando se acercan al oído.

**Qué rescatamos.** La obra sitúa la escucha en un lugar reconocible y la activa con un gesto físico. Eso nos hizo mirar el velador como parte del uso del dispositivo.

![Whispering Table](./imagenes/05-whispering-table.jpg)

*TheGreenEyl, 2009. Fotografía de la instalación Whispering Table.* [TheGreenEyl](https://www.thegreeneyl.com/whispering-table).

#### 06. Under the Same Moon — Qué hace personal a una grabación

**Contexto.** Under the Same Moon es un libro grabable de Hallmark que permite registrar la voz de una persona cercana para que une infante pueda volver a escucharla.

**Qué rescatamos.** Este referente nos ayudó a pensar qué grabar. Puede ser un saludo, una frase que se dicen siempre o parte de una canción, algo que la infancia pueda reconocer como propio de esa persona.

![Under the Same Moon](./imagenes/06-under-the-same-moon.jpg)

*Hallmark, s.f. Imagen de producto de Under the Same Moon, libro grabable.* [Hallmark](https://www.hallmark.com/baby-and-kids/kids-books/recordable-storybooks/under-the-same-moon-recordable-storybook-1KOB1228.html).

#### 07. Matriz de voz — Mostrar que la máquina está grabando

**Contexto.** Voice Array, de Rafael Lozano-Hemmer, transforma la voz captada por un intercomunicador en destellos de luz. Cada grabación nueva se incorpora a una secuencia de luces.

**Qué rescatamos.** La luz permite ver que la máquina está captando la voz. En nuestro módulo, el LED encendido indicará que la grabación está en curso.

![Voice Array](./imagenes/07-voice-array.jpg)

*Rafael Lozano-Hemmer, 2011. Imagen de la instalación Voice Array.* [Atelier Lozano-Hemmer](https://www.lozano-hemmer.com/voice_array.php).

#### 08. I Like Big Buttons — Distinguir grabar de reproducir

**Contexto.** I Like Big Buttons es una grabadora con botones grandes, uno para grabar y otro para reproducir.

**Qué rescatamos.** Grabar y escuchar son acciones distintas y cada una tiene su propio control. La persona cuidadora mantiene apretado el botón para grabar y lo suelta al terminar.

![I Like Big Buttons](./imagenes/08-i-like-big-buttons.jpg)

*BrandNewNoise, s.f. Fotografía de I Like Big Buttons, grabadora de botones grandes.* [BrandNewNoise](https://www.brandnewnoise.com/products/i-like-big-buttons).

#### 09. Wall-mounted CD Player — Una acción para empezar a escuchar

**Contexto.** Naoto Fukasawa diseñó este reproductor de CD para MUJI en 1999. Se instala en la pared y usa el cable que cuelga del equipo como interruptor.

**Qué rescatamos.** El uso parte de un gesto concreto sobre el objeto. Para nuestros botones, eso se traduce en que la primera pulsación inicia un audio y la segunda permite pasar al siguiente.

![Wall-mounted CD Player](./imagenes/09-wall-mounted-cd-player.jpg)

*Cooper Hewitt, Smithsonian Design Museum, s.f. Fotografía del reproductor de CD mural diseñado por Naoto Fukasawa para MUJI en 1999.* [Smithsonian Institution](https://www.si.edu/object/wall-mounted-cd-player%3Achndm_2013-35-1).

#### 10. Playdate — Dibujos legibles en una pantalla pequeña

**Contexto.** Playdate es una consola portátil de Panic con una pantalla reflectante en blanco y negro, botones físicos y una manivela lateral.

**Qué rescatamos.** Nos interesa cómo construye una identidad visual con pocos elementos. Para nuestros íconos queremos probar primero siluetas simples y comprobar que se distingan al tamaño real de la pantalla.

![Playdate](./imagenes/10-playdate.png)

*Panic, s.f. Fotografía de la consola Playdate.* [Panic](https://play.date/).


### Contenido y decisiones de diseño

#### Contenido y alcance inicial

La biblioteca inicial estará compuesta por:

- tres cuentos de una duración máxima de cinco minutos
- cuatro piezas de música infantil de una duración máxima de un minuto
- un mensaje grabado por el cuidador de una duración máxima de un minuto

Para seleccionar los cuentos, queremos evaluar si se puede seguir la historia solo escuchándola. Nos interesa que se entienda quién es el personaje, qué le pasa, qué hace y cómo termina la historia. Por eso, el criterio al revisar cada versión es si la narración cuenta lo necesario o si para entenderla hay que estar mirando el libro.

Para seleccionar la música, consideraremos
+qué se puede reconocer y seguir al escucharla. Si una canción tiene letra, nos fijamos en las palabras del estribillo y en si la acción o la idea principal se entiende. Si es instrumental, nos interesa si hay un ritmo o una melodía que vuelve a aparecer. Una canción puede ser fácil de tararear aunque parte de la letra todavía sea difícil de comprender.

Los cuentos se obtendrán desde bibliotecas de audiolibros o repositorios que permitan su uso gratuito y que tengan una licencia compatible con el proyecto. La música infantil deberá cumplir con la misma condición. Para cada audio se registrará el título, autor o intérprete, fuente y tipo de licencia.

Descartamos la organización de los contenidos según la hora del día. El planteamiento inicial separa los cuentos entre día y noche, pero la propuesta actual no limita su reproducción a un horario específico: los tres cuentos se podrán escuchar en cualquier momento. En la simulación, los contenidos se organizan por categoría y no por momento del día.

#### Personajes, colores y forma de la máquina

Queremos que cada categoría tenga su propio ícono. Para el diseño distinguimos dos pasos: reconocer el dibujo y entender qué activa el botón. Alguien puede ver un libro y decir «libro», pero la asociación con escuchar un cuento se aprende cuando presiona el botón y empieza a sonar. Por eso queremos mantener la misma relación entre el dibujo del botón, lo que aparece en pantalla y el audio que se activa. Los personajes todavía están por definir.

Tomamos como referencia la pauta del World Wide Web Consortium (2025) de acompañar el color con otras señales visuales. En el prototipo, cada botón ocupa una posición fija. Los botones de la versión física tendrán, además, un símbolo y un color propios. Para elegir la paleta, revisaremos cuánto se distingue el dibujo sobre el botón y el botón sobre la carcasa, tanto con luz natural como con luz más baja.

El primer planteamiento describe botones grandes, rígidos y resistentes a golpes. La nota más reciente propone botones blandos al tacto y resistentes al uso. Queremos distinguirlos por forma, posición y símbolo, y considerar el relieve para reconocerlos al tacto. Queda por definir la materialidad, el tamaño, la separación y la fuerza necesaria para presionarlos sin mover la máquina.

Al elegir la paleta, revisaremos qué tan bien se distingue cada control de la carcasa y de los demás botones, sobre todo con la luz que puede haber en la habitación a la hora de dormir. La paleta también definirá cómo se ve y se siente el objeto, y eso sigue siendo una decisión de diseño.

La carcasa tiene que quedar estable en el velador y proteger la pantalla, el parlante, la tarjeta SD y las conexiones. Los botones estarán integrados a la carcasa. Queremos que los cables y las piezas pequeñas queden fuera del alcance de les niñes durante el uso cotidiano.

### Funcionamiento

#### Funcionamiento principal

El dispositivo está pensado para permanecer en un lugar fijo y para que lo use principalmente la infancia. El prototipo de Wokwi tiene tres botones asociados a cuentos, música y mensajes del cuidador.

Al presionar el botón de cuentos, el programa selecciona uno de los tres cuentos en la simulación. Una nueva pulsación válida sobre el mismo botón cambia al siguiente. Lo mismo ocurre con las piezas de música, que están organizadas en una categoría independiente. La reproducción de audio real todavía no está implementada.

Cada botón de cuentos, música y mensaje acepta una pulsación con el filtro de rebote de más de 50 ms y espera al menos 3 segundos desde esa pulsación aceptada para contar otra. Cada botón lleva su propio tiempo. Los toques dentro de la espera se ignoran, no quedan pendientes ni reinician los 3 segundos. Mantener el botón apretado cuenta una sola vez; para aceptar otra pulsación hay que soltar y volver a presionar después de la espera. El primer toque al encender está disponible sin esperar. No usamos doble clic ni pulsaciones largas para pausar o detener. El botón de grabación es otra instancia de la misma clase, con el filtro de rebote y sin la espera extra de 3 segundos.

El tercer botón activa la reproducción simulada del único mensaje de la persona cuidadora. Si se vuelve a presionar mientras el mensaje está activo, no hace nada: no lo reinicia, pausa ni deja una repetición pendiente. El mensaje sigue activo hasta terminar, salvo que se elija cuentos o música; en ese caso se interrumpe y cambia a la categoría elegida. Cuando termina, una nueva pulsación válida permite activarlo otra vez desde el principio. Esta lógica evita que los toques repetidos reinicien el mensaje una y otra vez. En la versión física, el mensaje quedará en la tarjeta SD y solo se reemplazará cuando una grabación nueva se guarde correctamente. Si falla la grabación o el guardado, se conservará el anterior. No se borrará después de un día ni por tiempo. En el primer código ejecutable, la grabación y el guardado se representan mediante mensajes de simulación.

La pantalla acompaña la interacción mostrando la categoría seleccionada y los estados implementados en la simulación. La indicación del contenido que efectivamente suena y los avisos de guardado o error asociados a un archivo real quedan pendientes. La navegación no depende de la pantalla: la selección se hace directamente con los botones físicos, que tienen que ser comprensibles por sí mismos.

#### Sistema de grabación

La grabación está destinada al cuidador y no al uso cotidiano de la infancia. El programa contempla un solo mensaje a la vez. En la versión física, la función de grabación estará ubicada en un módulo separado del dispositivo principal. Este módulo incluirá el micrófono y el botón para comenzar y terminar la grabación, y se conectará mediante conectores JST.

En Wokwi, la persona cuidadora mantiene presionado el botón para iniciar la grabación simulada y lo suelta para terminar, con un máximo de un minuto. El LED se enciende durante la grabación simulada y se apaga al terminar. Si el conteo llega al minuto, no empieza otra grabación hasta soltar y volver a presionar. En la versión física, solo se confirmará «mensaje guardado» si el archivo se guarda bien; si falla, se mostrará el error y se conservará el mensaje anterior. En el Wokwi actual no hay micrófono conectado: por ahora probamos el botón, el tiempo y el LED y representamos la grabación con mensajes, sin capturar voz ni guardar un archivo real. Mientras se mantiene presionado el botón, la pantalla muestra «grabando…», el conteo de segundos transcurridos hasta 60 y las indicaciones «puedes grabar máximo 60 segundos» y «suelta el botón para guardar tu mensaje». El conteo usa el tiempo transcurrido de la raspi y se detiene al soltar o llegar al minuto. En el código actual de Wokwi, esta visualización ya está implementada como parte de la simulación y no confirma captura de voz ni guardado real.

La separación física del módulo funcionará como una restricción para que la infancia no pueda iniciar una grabación ni eliminar accidentalmente el mensaje del cuidador.

#### Revisión del módulo de grabación

Como el dispositivo ya no es portátil, debemos revisar si el módulo desmontable sigue siendo la forma más cómoda para el cuidador. La persona que graba es más alta que la infancia y puede necesitar acercar el módulo a su posición sin mover todo el dispositivo del velador.

La conexión y la forma del módulo se resolverán al desarrollar la carcasa. En la versión física el sistema deberá reconocer si el módulo está unido y permitir grabar solo cuando está conectado. Si se separa durante la grabación, deberá cancelarla sin reemplazar el mensaje anterior. Esto sigue siendo un requisito, aunque en esta entrega no hacemos carcasa ni implementamos la detección física del módulo; se mantiene declarado en el código.

### Proceso de código y pruebas

#### Código 1 y 2: declaraciones

El primer código es preliminar. Ordena las entradas, salidas y estados del dispositivo, pero todavía no define los pines ni contiene un programa ejecutable.

En el segundo código tomamos decisiones más concretas: tres cuentos de hasta cinco minutos, cuatro piezas de música de hasta un minuto y un mensaje de la persona cuidadora de hasta un minuto. También definimos una posición propia para cada categoría, el reemplazo del mensaje solo si la grabación nueva se guarda correctamente y la posibilidad de escuchar los contenidos a cualquier hora. Esta versión todavía no resuelve los tiempos entre pulsaciones, los botones presionados al mismo tiempo ni la forma de detener los audios.

#### Código 3: primera prueba ejecutable

En esta versión empezamos a trabajar en C++ con el Pico SDK y Wokwi. El `main` coordina las categorías, los estados y la grabación simulada.

Usamos `BotonGrupo1` para los botones de cuentos, música, mensaje y grabación. Los botones de categorías tienen filtro de rebote y una espera de tres segundos entre pulsaciones aceptadas. El botón de grabación usa la misma clase, pero sin esa espera adicional.

También incorporamos `Lucecita` para controlar el LED. Al mantener presionado el botón de grabación, el LED se enciende y comienza un conteo de hasta un minuto. Al soltar el botón, termina la grabación simulada. No se captura voz ni se guardan archivos: todo se representa mediante mensajes en el monitor serial.

#### Código 4: pantalla y correcciones

En la cuarta versión integramos la clase `Pantallita`, usando la biblioteca ILI934X. El `main` coordina las acciones y la pantalla muestra los estados.

Agregamos una pantalla inicial, símbolos provisorios para cuentos y música, una carita cuando no existe mensaje, un corazón mientras el mensaje está activo y un contador durante la grabación simulada.

También corregimos las pulsaciones simultáneas: si se presionan varios botones de categoría, ninguno ejecuta una acción. Además, el botón del mensaje no lo reinicia mientras está activo, mientras que cuentos y música sí pueden interrumpirlo.

Finalmente agregamos un reposo por software después de tres minutos sin uso. La pantalla se apaga, pero la placa sigue encendida. La primera pulsación solo despierta el dispositivo y hay que volver a presionar para realizar una acción.

La entrega compila en Wokwi y permite probar la pantalla, los símbolos, el LED, el contador de grabación y el bloqueo de pulsaciones simultáneas. Todavía falta implementar el audio real, el micrófono, el almacenamiento en la tarjeta SD y la detección física del módulo de grabación.

#### Diagrama de flujo de usuario

El diagrama muestra el uso previsto del dispositivo. En Wokwi, los pasos que implican reproducir audio o grabar voz se representan mediante estados simulados.

```mermaid
%%{ init : { "theme" : "redux-color", "flowchart" : { "curve" : "linear" }}}%%
%%                                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
flowchart TD
%%==============================================================================

A@{shape: circ, label: "Se enciende el dispositivo"}-->B(Alguien presiona un botón);
B-->C(Elegir un cuento);
B-->D(Elegir música);
B-->E(Escuchar el mensaje);
B-->F(Grabar mensaje);
C-->G(Suena un cuento);
G--presionado de nuevo -->K(Pasa al siguiente)-->O;
D-->H(Suena música);
H--presionado de nuevo -->K(Pasa al siguiente);
E-->I{¿Hay mensaje?};
I--no -->M(Aparece una carita triste en pantalla)-->O;
I--sí -->N(Suena el mensaje)-->O;
F-->J(Graba la voz);
J--deja de presionar o excede el tiempo -->L(Termina de grabar)-->O;
O(3 minutos sin uso)-->P@{shape: dbl-circ, label: "Pantalla se apaga"};
P-->B;
```

#### Diagrama de flujo técnico

```mermaid
%%{ init : { "theme" : "redux-color", "flowchart" : { "curve" : "linear" }}}%%
%%                                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
flowchart TD
%%==============================================================================

A@{shape: circ, label: "**Inicializar**<br>serial, botones, LED, pantalla"}-->B(Inicio de la vuelta);
B-->C(<b>Leer botón grabar</b><br>aceptarPulsaciones);
C-->D{¿no está grabando y la pulsación de grabar fue aceptada?};
D--sí -->S(<b>iniciarGrabacion</b><br>detiene audio<br>LED on)-->H;
D--no -->E{¿Está grabando y soltaron el botón?};
E--sí -->T(<b>terminarGrabacion</b><br>mensaje = true<br>LED off)-->H;
E--no -->F{¿Está grabando y llegó a 60s?};
F--sí -->U(<b>Terminar grabación</b><br>avisoSoltar = true)-->H;
F--no -->G{¿Sigue grabando?};
G--sí -->V(actualizarConteo)-->H;
G--no -->H{¿avisoSoltar y el botón grabar ya se soltó?};
H--sí -->W(Quitar aviso)-->I;
H--no -->I(<b>Leer pines crudos</b><br>cuentos, música y mensaje);
I-->J(Leer los 3 botones);
J-->K{¿En reposo?};
K--sí -->X{¿Algún botón recién presionado?};
X--sí -->Y(Despertar)-->R;
X--no -->R;
K--no -->L{¿Cuentos o música fue aceptado?};
L--sí -->Z("<b>cambiarA(categoria)</b>"<br>empieza, avanza, o cambia)-->M;
L--no -->M{¿Mensaje aceptado y<br>existe mensaje?};
M--sí -->AA(Activar mensaje)-->N;
M--no -->N{¿Mensaje aceptado y no existe mensaje?};
N--sí -->AB(<b>Carita triste</b><br>no cambia la categoría)-->O;
N--no -->O(pantalla.actualizar);
O-->P{¿Pulsación aceptada, grabando o audio activo?};
P--sí -->AC(<b>ultimoUso = ahora</b><br>reinicia el contador de reposo)-->Q;
P--no -->Q{¿Pasaron 180 seg<br>sin uso?};
Q--sí -->AD(<b>Entrar en reposo</b><br>dormido = true<br>apaga pantalla)-->R;
Q--no -->R@{shape: dbl-circ, label: "Fin de la vuelta"}-->B;
```

### Materiales y simulación

#### Lista de materiales (BOM)

| Componente | Cantidad | Precio unitario | Proveedor |
| --- | ---: | ---: | --- |
| Raspberry Pi Pico 2 W | 1 | $0 | Universidad |
| Pantalla TFT LCD Touch 3,5” SPI 480x320 | 1 | $14.000 | Afel |
| Mini parlante 3 W / 4 Ω | 1 | $3.000 | Afel |
| Amplificador PAM8403 2x3 W | 1 | $1.000 | Afel |
| Botón táctil con tapa 12x12x7,3 mm | 5 | $100 | Afel |
| Módulo micrófono amplificador MAX4466 | 1 | $2.990 | Afel |
| Lector Micro SD SPI | 1 | $2.500 | Afel |
| Tarjeta microSD 4 GB | 1 | por definir | no hay en afeel |
| Pack de 60 cables de conexión Dupont | 1 | $2.500 | Afel |
| Kit de conectores JST XH2.54 | 1 | $6.000 | Afel |
| LED | 2 | 0 | PROPIOS |
| Cable USB | 1 | 0 | PROPIO |
| **Total sin la tarjeta microSD** | | **$32.490** | |

- Eliminamos la batería y el power bank porque el dispositivo no es portátil y está pensado para permanecer instalado en un lugar fijo.
- La pantalla elegida incluye función touch, pero en el proyecto funciona solo como pantalla de visualización. De los 5 pulsadores del BOM, 4 son para uso y 1 es repuesto por precaución; de los 2 LED, 1 indica grabación y 1 es repuesto.

#### Circuito

(<https://wokwi.com/projects/481143276644147201>)

En el circuito se ven la Pico 2 W, la pantalla ILI9341, cuatro pulsadores, el LED de grabación, el lector microSD, el modelo del PAM8403 y un buzzer que representa el parlante. El código usa GP2 para cuentos, GP3 para música, GP4 para mensaje, GP5 para grabar y GP6 para el LED. GP0 y GP1 se usan para el monitor serial UART0.

#### Bibliotecas, clases y créditos

| Recurso | Función en el proyecto | Crédito o archivo |
| --- | --- | --- |
| [Pico SDK](https://github.com/raspberrypi/pico-sdk) | Base del programa en C++, lectura del tiempo y comunicación serial. | Raspberry Pi. El programa incluye `pico/stdlib.h` y el proyecto contiene `CMakeLists.txt`. |
| ILI934X | Control de la pantalla ILI9341 del simulador. | [Darren Horrocks](https://www.darrenhorrocks.co.uk/native-pico-sdk-ili9341-ili9342-for-raspberry-pi-pico/). `ili934x.h`, `ili934x.cpp` y `LICENSE-ILI934X.txt` (BSD de 3 cláusulas). |
| FreeMono y `gfxfont.h` | Fuentes y estructuras de fuente usadas para los textos. | `FreeMono12pt7b.h`, `FreeMono18pt7b.h`, `FreeMono24pt7b.h`. |
| `BotonGrupo1` | Lectura de los pulsadores, filtro de rebote y espera por botón. | Camila Ramírez, Bianka Vilchez y Dayana Pañitrur, Grupo 1 del Proyecto 2. |
| `Lucecita` | Enciende y apaga el LED de grabación. | Material docente de la sesión 08a. |
| `Pantallita` | Dibuja las vistas del dispositivo. `main.cpp` coordina las categorías y los estados. | `Pantallita.h` y `Pantallita.cpp`. |
| `pam8403.chip.c` y `pam8403.chip.json` | Modelo del amplificador representado en el circuito. | Archivos del proyecto en Wokwi. |

### Bibliografía

#### Investigación de contexto

- Arriagada, P., & Valdebenito, L. (2011). Para reconstruir la vida de los niños y niñas: Guía para apoyar intervenciones psicosociales en emergencias y desastres. UNICEF. <https://www.unicef.org/chile/media/1666/file>
- Center on the Developing Child at Harvard University. (s.f.). A guide to neglect. <https://developingchild.harvard.edu/resource-guides/guide-neglect/>
- Easterseals Vermont. (2024, 31 de mayo). A sound that soothes. <https://eastersealsvt.org/news-and-stories/a-sound-that-soothes/>
- Organización Mundial de la Salud. (2025). De la soledad a la conexión social: El camino hacia sociedades más saludables [Resumen del informe de la Comisión de la OMS sobre Conexión Social]. <https://www.who.int/es/publications/m/item/from-loneliness-to-social-connection-summary>
- Sesame Workshop. (s.f.). Cuando los niños extrañan al otro padre. <https://sesameworkshop.org/resources/cuando-los-ninos-extranan-al-otro-padre/>
- World Wide Web Consortium. (2025, 16 de septiembre). Understanding SC 1.4.1: Use of color (Level A). <https://www.w3.org/WAI/WCAG22/Understanding/use-of-color>

#### Referentes de diseño

- AbleNet. (s.f.). BIGmack. <https://www.ablenetinc.com/bigmack/>
- Boundless Assistive Technology (s.f.). *LITTLEmack & BIGmack Communicators* [Fotografía]. BoundlessAT. <https://www.boundlessat.com/Communication/Basic-Communicators/LITTLEmack-BIGmack-Communicators>
- BrandNewNoise. (s.f.). I LIKE BIG BUTTONS [Fotografía]. <https://www.brandnewnoise.com/products/i-like-big-buttons>
- Cooper Hewitt, Smithsonian Design Museum. (s.f.). Wall-Mounted CD Player. Smithsonian Institution. <https://www.si.edu/object/wall-mountedcdplayer%3Achndm_2013-35-1>
- Hallmark. (s.f.). Recordable storybooks. <https://www.hallmark.com/baby-and-kids/kids-books/recordable-storybooks/>
- Hallmark. (s.f.). *Under the Same Moon Recordable Storybook* [Fotografía]. <https://www.hallmark.com/baby-and-kids/kids-books/recordable-storybooks/under-the-same-moon-recordable-storybook-1KOB1228.html>
- Lovebox. (s.f.). Lovebox Photo Messenger. <https://en.lovebox.love/products/lovebox-color-photo>
- Lovebox. (s.f.). Lovebox Photo Messenger [Fotografía]. <https://buy.lovebox.love/products/lovebox>
- Lozano-Hemmer, R. (2011). Voice array (Subsculpture 13) [Instalación interactiva]. Atelier Lozano-Hemmer. <https://www.lozano-hemmer.com/voice_array.php>
- Panic. (2022). Playdate. <https://play.date/>
- TheGreenEyl. (s.f.). Whispering table (Tischgeflüster). <https://www.thegreeneyl.com/whispering-table>
- tonies. (s.f.). *Pokémon: Pikachu Tonie*. <https://us.tonies.com/products/pokemon-pikachu-tonie>
- Upchurch, R. (2019). Instructions for how it works – BrandNewNoise by Richard Upchurch. BrandNewNoise. <https://www.brandnewnoise.com/blogs/news/instructions-for-how-it-works-by-richard-upchurch>
- Yoto. (s.f.). Yoto Player 4th Gen. <https://us.yotoplay.com/players/yoto-player-4>
- Yoto. (2026). *Yoto 4th Gen* [Fotografía]. Yoto Newsroom. <https://us.yotoplay.com/press/en-US/270385-yoto-launches-yoto-player-4th-gen-and-yoto-mini-4th-gen-its-biggest-hardware-upgrade-yet/>
