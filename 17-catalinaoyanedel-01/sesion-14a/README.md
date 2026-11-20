# sesion-14a
martes 17 de noviembre

## apuntes sesión

este día terminamos de corregir todos los aspectos técnicos de lo que necesitábamos: el bom, diagrama de flujo, pseudocodigo, prototipo en wokwi (que quedaron actualizados en las bitácoras anteriores y en [00-proyecto-3/grupo-03](https://github.com/catalinaoyanedel-01/dis8645-2026-2-procesos-2/tree/main/00-proyecto-3/grupo-03), y realización de la idea de la carcasa.

hicimos un boceto y luego la renderizamos con IA con el siguiente prompt:

> "Genera un render realista del diseño que adjunto, respetando su distribución y proporciones. Los materiales y elementos deben interpretarse de la siguiente manera: las superficies de color café claro corresponden a madera clara; las piezas burdeos, a filamento de impresión 3D; y las perillas verdes, también a filamento 3D. Los pequeños puntos burdeos corresponden a potenciómetros de menor tamaño, destinados a controlar el volumen de cada especie. Las líneas grabadas en la superficie representan las siluetas de distintas especies. En la parte superior se encuentran los orificios de salida del sonido. El botón ubicado en la esquina superior izquierda corresponde al encendido y apagado del dispositivo, mientras que en la zona inferior izquierda se incorpora una pequeña leyenda explicativa. Es importante mantener la estética, la distribución de los controles y la estructura del diseño original, sin agregar elementos que no estén contemplados."

![boceto](./imagenes/boceto.jpeg)

![render 1](./imagenes/render1.png)

![render 2](./imagenes/render2.png)

también nos dividimos las tareas para hacerlo más equitativo y ordenado. mi rol es cumplir con esto:

- Investigar sobre la idea de “no se puede cuidar lo que no se conoce”.
- Investigar a Carlos Cabezas como referente.
- Investigar la biodiversidad sonora de Chile.
- Seleccionar especies de las zonas norte, centro y sur (verificando que existan registros de audio disponibles).
- Justificar la selección territorial y ecológica.
- Investigar características de las vocalizaciones.
- Buscar y recopilar registros de audio.
- Investigar probabilidad y aleatoriedad desde una perspectiva teórica. (Me sirve Tortugas, termitas y atascos de trafico de Mitchel Resnik)
- Investigar The Nature of Code, especialmente Randomness y Perlin Noise.
- Investigar densidad y síntesis granular desde la perspectiva conceptual.
- Consolidar el documento final y su presentación (primero el documento y luego la presentación).

Entregable: marco teórico, catálogo de especies y bibliografía científica.

## encargos

### No se puede cuidar lo que no se conoce
La frase que inspira el proyecto se le atribuye al conservacionista senegalés Baba Dioum, quien la expresó en 1968 durante la reunión trienal de la Asamblea General de la Unión Internacional para la Conservación de la Naturaleza y los Recursos Naturales (UICN), realizada en Nueva Delhi. Su declaración fue: “Al final, solo conservaremos lo que amamos, solo amaremos lo que entendemos y solo entenderemos lo que nos enseñan” (Dioum, 1968, como se citó en Gray y Thomson, 2016).

Esta cita nos sirve como un punto de partida para definir el propósito del proyecto, ya que no basta con educar dando a conocer. Entendemos que conocer no implica, de forma directa, cuidar, y que incluso las especies más conocidas pueden ser las que se encuentran en mayor peligro (Andarríos, 2011c). Por lo que, conocer nombres, características y vocalizaciones no es suficiente y a partir de esto, definimos dos enfoques educativos que se complementan.

El primero consiste en profundizar la información mediante una ficha que acompañe a la máquina y que permita entender a cada especie como parte de un sistema. Según Andarríos (2011a), los ecosistemas pueden seguir existiendo tras la pérdida de diversas especies, aunque su funcionamiento se vea alterado. A pesar de esto, es importante entender que cada organismo cumple un rol bioindicador, entregando información sobre los cambios y la salud de su entorno (Andarríos, 2011a). Comprender esto nos permite valorar el papel de cada especie y leer mejor los ecosistemas de los que también somos parte.

El segundo enfoque apunta a una relación más afectiva con la naturaleza. Gray y Thomson (2016) sostienen que “la educación no solo requiere una mayor conciencia de nuestra realidad ecológica, sino también una mayor curiosidad y apego —e incluso amor, tal vez— por el mundo natural” (p. 241, traducción propia). Buscamos que la máquina sonora cumpla este propósito generando, a través de la escucha, un vínculo sensible con estas especies.

### Referentes (personas, conceptuales):
#### Carlos Cabezas

Carlos Cabezas es un músico, productor y artista sonoro chileno con más de 40 años de trayectoria. Su trabajo destaca por la experimentación electrónica, el autodidactismo y el cruce entre la música, las artes visuales, el cine y el teatro. Sobre su recorrido artístico, Arteaga (2025) señala: 

> “Desde los días fundacionales de Electrodomésticos, pasando por su carrera solista, proyectos como La Banda del Dolor, Cordillera y sus incursiones en el bolero y la experimentación sonora, su obra se ha transformado en un mapa sonoro que refleja tanto la historia personal del artista como la evolución cultural del país.” (párr. 1)

Como referentes para este proyecto, consideramos una obra de su autoría y otra en la que participó como compositor:

1. _Cordillera:_ álbum realizado en conjunto a Ángelo Pierattini

Respecto al enfoque conceptual de este disco, Carlos Cabezas explica:

> “Cordillera plantea un paisaje sonoro, por ejemplo, como lo dice el nombre, plantea un espacio, una sensación, un territorio. Y las sensaciones y reflexiones que pueden venir de ahí, de lo que puede ser la experiencia de vida en ese espacio.” (Arteaga, 2025, párr. 12)

Por su parte, Ángelo Pierattini complementa esta visión señalando que el álbum contiene “composiciones muy sensibles, mínimas en su vestidura, mucha atmósfera, una que lograra llevarnos a un espacio tan enigmático como la cordillera” (La Tercera, 2024, párr. 3).

De este álbum rescatamos su propuesta conceptual en torno al paisaje sonoro, ya que nos permite abordar la idea del espacio y las sensaciones propias de los distintos territorios que estamos abordando.

2. _Blanco_

Blanco es una obra de danza contemporánea sobre la vastedad de la naturaleza y la forma en que nos afecta y conecta, cocreada por tres figuras de las artes escénicas: Rodrigo Pérez, Andrés Poirot y Claudia Vicuña. La puesta en escena indaga en la sensación de inmensidad de un paisaje y en el silencio interior que se alcanza al contemplarlo (Centro Cultural Gabriela Mistral, s. f.).

La composición musical estuvo a cargo de Carlos Cabezas, quien construye un ambiente envolvente, cristalino y natural. Según explicó Claudia Vicuña, la obra se concibe como un paisaje en el que los elementos coexisten sin jerarquías, permitiendo que sea el propio espectador quien vincule sus componentes (Radio Universidad de Chile, 2019).

De esta propuesta rescatamos tanto la composición musical, que evoca al entorno natural, como la noción de un paisaje abierto, donde cada cada elemento pueda existir individualmente y que el usuario sea quien le da sentido a lo que escucha.

#### Felipe Otondo

Felipe Otondo es ingeniero acústico, con una maestría en Acústica en la Universidad de Aalborg (Dinamarca), y compositor: estudió composición en la Carl Nielsen Academy of Music y realizó un doctorado en composición musical en la Universidad de York. Actualmente dirige el Instituto de Acústica de la Universidad Austral de Chile y, desde 2019, lidera el proyecto Soundlapse, vinculado a un proyecto Fondecyt sobre una herramienta espacio-temporal para aplicaciones creativas y educativas de grabaciones de paisaje sonoro de humedales.

Soundlapse se organiza en tres líneas de investigación: detección de fauna, creación artística y educación ambiental. En la primera, usan inteligencia artificial para detectar de manera fidedigna las aves de Chile y generar un repositorio para escucharlas, que vincula la imagen del ave con su sonido y una breve descripción.

### Biodiversidad sonora de Chile

Existen diversos recursos en Chile que recopilan archivo sonoros de especies, los que utilizamos fueron los siguientes:

- **Museo Bioacústico** (https://museobioacustico.org): corporación sin fines de lucro e interdisciplinaria dedicada a la investigación, educación y divulgación científica del patrimonio acústico natural de Chile, promoviendo la conservación de sus ecosistemas mediante el acceso abierto a registros y experiencias sonoras.
  
- **iNaturalist** (https://www.inaturalist.org): plataforma global de ciencia ciudadana y red social de naturalistas dedicada al registro, identificación y mapeo colaborativo de la biodiversidad. Permite recopilar, geolocalizar y validar observaciones visuales y acústicas aportadas por usuarios e investigadores, generando datos abiertos para el estudio y conservación de especies en Chile y el mundo. (Iniciativa conjunta de la Academia de Ciencias de California y National Geographic Society). 

- **ONG Ranita de Darwin – Cantos de Anfibios** (https://www.ranitadedarwin.org/cantos): archivo digital de divulgación científica y educación ambiental dedicado a la recopilación de vocalizaciones de anfibios nativos de Chile. Desarrollado por la organización sin fines de lucro Ranita de Darwin, el recurso pone a disposición registros sonoros abiertos acompañados de información ecológica para promover el conocimiento, la valoración y la conservación de los anfibios amenazados del país.

Recursos internacionales:

- **Xeno-canto** (xeno-canto.org): base de datos colaborativa de acceso abierto dedicada a vocalizaciones de aves a nivel mundial. Permite filtrar registros por país y especie (nativa o endémica), ofreciendo descarga de audios junto con metadatos de geolocalización, fecha, hora y tipo de licencia.

### Selección de especies, justificación, características y audio

A partir de este prompt, le pedí a Gemini y Claude que me dieran un listado de posibles especies y fui verificando.
> “Necesito seleccionar especies de fauna nativa o endémica de Chile que estén clasificadas en estado de amenaza (En Peligro Crítico, En Peligro o Vulnerable según el MMA de Chile) para un proyecto de divulgación sobre biodiversidad. Dame 5 opciones de especies para cada cruce entre las siguientes categorías y zonas del país:
>
> Mamíferos: Norte / Centro / Sur
>
> Aves: Norte / Centro / Sur
> 
> Anfibios: Norte / Centro / Sur
> 
> Insectos: Norte / Centro / Sur
> 
> Criterios de selección y formato: prioriza especies que emitan sonidos, cantos o vocalizaciones reconocibles (especialmente en aves, anfibios e insectos). Indica para cada especie: nombre común, nombre científico, origen (endémica o nativa), categoría de conservación exacta, una breve referencia de su hábitat, de su vocalización y cómo actúa como bio indicador."

#### Mamíferos

| Zona | Nombre común | Nombre científico | Origen | Estado MMA | Hábitat, vocalización, rol bioindicador | Audio |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Norte | Ballena Azul | *Balaenoptera musculus* | Nativa | CR | Aguas oceánicas y zonas de surgencia costera (Chañaral); emite pulsos de muy baja frecuencia (infrasonidos). Indica productividad marina (krill) y baja contaminación acústica submarina. | [Escuchar](https://museobioacustico.org/especie/83e7b118-57f7-4e1b-81a4-0e19717e31dc) |
| Centro | Zorro culpeo | *Lycalopex culpaeus* | Nativa | VU (Zona Central) | Matorral y bosque esclerófilo; se comunica con aullidos y ladridos profundos. Indica equilibrio en las poblaciones de roedores nativos. | [Escuchar](https://xeno-canto.org/species/Lycalopex-culpaeus) |
| Sur | Zorro chilote | *Lycalopex fulvipes* | Endémica | EN | Bosque húmedo del sur. Indica bosques conservados | [Escuchar](https://xeno-canto.org/species/Lycalopex-fulvipes) |

#### Aves

| Zona | Nombre común | Nombre científico | Origen | Estado MMA | Hábitat, vocalización, rol bioindicador | Audio |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Norte | Gaviotín chico | *Sternula lorata* | Nativa | EN | Playas arenosas y dunas costeras; emite llamadas estridentes y nasales en vuelo. Indica playas libres de tránsito vehicular, perros y perturbación humana. | [Escuchar](https://xeno-canto.org/species/Sternula-lorata) |
| Centro | Hued-hued Castaño | *Pteroptochos castaneus* | Nativa | VU | Sotobosque húmedo y denso de bosques caducifolios y esclerófilos de transición de la Zona Centro-Sur (desde O'Higgins hasta el Biobío); emite un canto territorial profundo y resonante. Indica cobertura vegetal continua, quebradas húmedas con abundante hojarasca y baja fragmentación del bosque nativo. | [Escuchar](https://museobioacustico.org/especie/edc219c6-fa79-4b8f-8f5a-58077907969e) |
| Sur | Fardela Blanca | *Ardenna creatopus* | Nativa | EN | Aguas pelágicas e islas (nidifica en Isla Mocha y Juan Fernández); emite llamadas nocturnas nasales en colonias. Indica productividad marina y ausencia de depredadores introducidos o contaminación lumínica en sus islas. | [Escuchar](https://museobioacustico.org/especie/3c863387-d180-4f8e-922b-ffcdb0a98d79) |


#### Anfibios

| Zona | Nombre común | Nombre científico | Origen | Estado MMA | Hábitat, vocalización, rol bioindicador | Audio |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Norte | Sapo de Atacama | *Rhinella atacamensis* | Endémica | VU | Oasis, aguadas y quebradas costeras (desde Antofagasta a Coquimbo); emite un trino grave y continuo. Indica calidad del agua y conservación de humedales en zonas áridas. | [Escuchar](https://soundcloud.com/anfibios-de-chile/rhinella-arunco) |
| Centro | Sapito de cuatro ojos | *Pleurodema thaul* | Nativa | VU (poblaciones norte-centro) | Estero, charcos temporales y humedales desde Antofagasta hacia el sur; canto melódico y aflautado (*"pui-pui"*) fácil de hallar en archivos de audio. Indica disponibilidad de agua estacional y micro humedales sanos. | [Escuchar](https://xeno-canto.org/species/Pleurodema-thaul) |
| Sur | Ranita de Darwin | *Rhinoderma darwinii* | Nativa | EN | Requiere bosques templados lluviosos maduros y de alta humedad, por lo que su presencia funciona como un "termómetro" biológico (especie centinela) que certifica la buena salud y madurez del bosque nativo. | [Escuchar](https://www.ranitadedarwin.org/cantos) |

#### Insectos

| | Nombre común | Nombre científico | Origen | Estado MMA | Hábitat, vocalización, rol bioindicador | Audio |
|---|---|---|---|---|---|---|
| Norte | Moscardón o Abejorro nativo | *Bombus dahlbomii* | Nativa | EN | Llega hasta Coquimbo en su límite norte; zumbido grave y muy fácil de encontrar grabado. Indica estado de polinizadores nativos frente a especies invasoras (*B. terrestris*). | [Escuchar](https://www.youtube.com/watch?v=9bTr3HQUSP4)|
| Centro | Madre de la culebra | *Acanthinodera cumingii* | Endémica | VU | Bosque esclerófilo y caducifolio; emite un chillido estridente por estridulación al ser perturbada y un zumbido pesado al volar. Indica bosques con árboles maduros y madera en descomposición. | [Escuchar](https://www.inaturalist.org/observations/66860221) |
| Sur | Abeja Negra | *Centris nigerrima* | Nativa | VU | Bosques templados y matorrales (hasta Los Lagos); emite un zumbido grave al polinizar por vibración. Indica salud de la flora nativa, suelos sin compactar para nidificar y baja presión de especies invasoras. | [Escuchar](https://museobioacustico.org/especie/2c219b00-b16d-4a85-bee8-a56df40cd652) |


## bibliografía

- Andarríos. (2011a, 10 de enero). Bioindicadores: escuchando de nuevo a la Naturaleza (1/2). BlogSOStenible. https://blogsostenible.wordpress.com/2011/01/10/bioindicadores-escuchando-de-nuevo-a-la-naturaleza-12/ 
- Andarríos. (2011b, 11 de enero). Bioindicadores: escuchando de nuevo a la Naturaleza (2/2). BlogSOStenible. https://blogsostenible.wordpress.com/2011/01/11/bioindicadores-escuchando-de-nuevo-a-la-naturaleza-22/ 
- Andarríos. (2011c, 25 de febrero). ¿Conocer para proteger? BlogSOStenible. https://blogsostenible.wordpress.com/2011/02/25/%C2%BFconocer-para-proteger/ 
- Gray, T., y Thomson, C. (2016). Transforming environmental awareness of students through the arts and place-based pedagogies. Learning Landscapes, Vol. 9. No. 2, 239-260. https://www.learninglandscapes.ca/index.php/learnland/article/view/Transforming-Environmental-Awareness-of-Students-Through-the-Art/774 

_____

- Arteaga, M. (28 de noviembre de 2025). Carlos Cabezas: 40 años en una sola voz. Rockaxis. https://www.rockaxis.com/chile/entrevista/48812/carlos-cabezas-40-anos-en-una-sola-voz/  
- Centro Cultural Gabriela Mistral. (2 de agosto de 2019). Danza: Blanco [Video]. YouTube. http://www.youtube.com/watch?v=KTKLvBF6utY 
- Centro Cultural Gabriela Mistral. (2019). Blanco. https://gam.cl/es/que-hacer-en-gam/danza/blanco-2019/ 
- Centro Cultural Gabriela Mistral. (13 de agosto de 2019). Entrevista a Claudia Vicuña en Radio Universidad de Chile. https://gam.cl/es/sala-de-prensa/prensa/entrevista-claudia-vicuna-en-radio-universidad-de-/ 
- La Tercera. (24 de mayo de 2024). Cordillera, el proyecto de Carlos Cabezas y Ángelo Pierattini, estrena su primer disco. https://www.latercera.com/culto/2024/05/24/cordillera-el-proyecto-de-carlos-cabezas-y-angelo-pierattini-estrena-su-primer-disco/ 

_____

- Bustos Vaccia, V. (2024, 23 de diciembre). Sonidos de biodiversidad en ecosistemas del sur [Entrevista a Felipe Otondo]. Fundación Mar Adentro. https://fundacionmaradentro.cl/articulo/sonidos-de-biodiversidad-en-ecosistemas-del-sur/



## lectura
