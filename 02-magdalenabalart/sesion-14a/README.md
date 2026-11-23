# sesion-14a

## apuntes sesión

Durante el martes nos organizamos para distribuir las tareas de cada una y avanzar en paralelo con las distintas partes de la máquina. También estuvimos revisando cómo funcionaría la interacción y cómo lograr que los sonidos de las especies pudieran construir un paisaje sonoro y no simplemente reproducirse de manera individual

### Distribución de tareas

Nos organizamos para que cada una pudiera avanzar en distintas partes del proyecto y después ir juntando todo.

- **Yo:** trabajar en Wokwi, armar las conexiones de los componentes y avanzar con el código en C/C++. También investigar cómo incorporar la probabilidad y el tempo, hacer las pruebas y subir los avances a GitHub.
- **Cata:** investigar sobre probabilidad, densidad sonora, síntesis granular y *The Nature of Code*, además de buscar bibliografía para entender mejor cómo aplicar estos conceptos a la máquina.
- **Marce:** avanzar con la carcasa, viendo cómo resolverla con impresión 3D, corte láser y los materiales que vamos a utilizar. También desarrollar el pseudocódigo para ordenar cómo debería funcionar la máquina.
- **Yai:** trabajar en las conexiones y el esquema electrónico en KiCad, revisar los componentes que necesitamos y sus requerimientos. También buscar referentes de instrumentos sonoros y apoyar con la documentación.

Cambios en la interacción de la máquina

Al principio habíamos pensado en utilizar ocho potenciómetros: cuatro para seleccionar las especies de cada categoría (mamíferos, aves, insectos y anfibios) y otros cuatro para controlar el volumen de cada una.

Pero conversando entre nosotras nos surgió la duda de cómo esta interacción realmente iba a generar un paisaje sonoro y no simplemente reproducir sonidos de animales. Porque claro, podíamos seleccionar distintas especies y mezclar sus volúmenes, pero al final los audios sonarían de manera bastante constante y el resultado podía terminar siendo medio monótono. Era más un reproductor de sonidos que una máquina que permitiera experimentar con ellos.

Entonces hablamos con los profes sobre esta inquietud y nos recomendaron investigar algunas formas de trabajar con la aleatoriedad, para que los sonidos no dependieran únicamente de lo que seleccionara el usuario, sino que también pudieran ir apareciendo y cambiando por sí solos.

De esa conversación salieron cinco conceptos que tenemos que profundizar: 

-Probabilidad  
-Densidad  
-Síntesis granular  
-The nature of code  
-Tempo, cada cuánto quieres que suene esa probabilidad  

¿En qué quedamos?

Con todo esto decidimos cambiar un poco la propuesta inicial. Los cuatro potenciómetros grandes seguirán siendo para seleccionar las especies, pero los cuatro pequeños ya no controlarán el volumen, sino la probabilidad de aparición de cada categoría.

Además, vamos a incorporar un noveno potenciómetro para manejar el tempo general de la máquina.

También queremos probar que cada reproducción pueda tener pequeñas variaciones de velocidad, duración y volumen, para que una misma especie no suene siempre igual.

La idea es que el usuario pueda elegir las especies y modificar las condiciones en que aparecen, pero sin controlar completamente lo que va a escuchar. Que el paisaje sonoro se vaya construyendo a partir de esas decisiones y también de lo que la propia máquina va generando.


## encargos

## lectura

### Sangre negra, Paula Carrasco 

Libro que me leí la semana pasada: 

Ya me había encontrado antes con su escritura en Algo temporal y ahora, con Sangre negra, vuelvo a rendirme ante su manera de escribir, en el tacto y el cariño con el que construye a sus personajes, que hace que estos dejen de ser simples personajes cuando una termina de leer sus historias. Ambas novelas comparten algo que me gusta muchísimo: ese tono de thriller, pero de un thriller que se siente profundamente cotidiano. Que no necesita de grandes acontecimientos para inquietarnos, porque encuentra en las relaciones humanas, en sus contradicciones y en todo aquello que preferimos no decir, suficiente material para mantenernos con la sensación de que hay gato encerrado.
 
Y quizás no es tan extraño si pensamos en que Paula también es psicóloga y que, como ella misma agradece, parte de su escritura se nutre de las experiencias que ha conocido a través de sus pacientes. Me parece preciosa esa capacidad de recoger algo de lo cotidiano, de las complejidades que muchas veces pasan desapercibidas en la vida de las personas y transformarlo en ficción. No porque esas vidas necesiten ser extraordinarias para convertirse en literatura sino porque quizás nunca fueron tan simples como parecían. De esta manera el libro me mantiene con la sensación de que hay algo escondido. De que algo no termina de calzar, de que nos falta una parte de la historia, incluso cuando creemos que ya entendimos todo. Me encanta esa capacidad que tiene de hacernos desconfiar de lo que estamos leyendo, de llevarnos a sacar nuestras propias conclusiones para después obligarnos a cuestionarlas. 
 
Pero creo que lo que más me gusta de Paula es cómo construye a sus personajes. Porque una sabe que hay cosas que no están bien, decisiones que no corresponden, acciones que duelen, pero que tampoco parecen lo suficientemente malas como para juzgarlas con tanta facilidad, y qué difícil es eso, porque una termina entendiendo cosas que quizás no debería justificar (y que tampoco quiere justificar), pero ¿cómo no hacerlo cuando conocemos todo lo que hay detrás?

Me gusta esa incomodidad de no saber muy bien de qué lado ponerse. De querer a personajes con los que no necesariamente estamos de acuerdo, de sentir pena por quienes también han hecho daño, de comprender sin perdonar del todo. Porque al final las personas tampoco somos tan fáciles de separar entre buenas y malas, y Paula parece entenderlo muy bien.

Y creo que ahí está una de las cosas más bonitas de su escritura: en que no nos entrega personajes para juzgarlos, sino para conocerlos. Aunque conocerlos tampoco signifique que tengamos que estar de acuerdo con ellos.
