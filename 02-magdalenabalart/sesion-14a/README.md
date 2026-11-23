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
