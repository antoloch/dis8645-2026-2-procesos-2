# sesion-11a

## apuntes sesión
trabajando en el prooyecto 2 
El profe cambio la fecha de la entrega al martes 3 de noviembre yei
- conversamos con el grupo del nico de los que nos pidieron con sus botones y se llego a esto:

Botón 1: su función es cambiar de nivel 1>2>3 y seleccionar con un doble clic con un tiempo límite entre clicks de 0.6ms —--> su función es cambiar de nivel 1>2>3 y al recibir un cuarto clic, se reinicia el ciclo volviendo al nivel 1 

Botón 2: su función es atrapar notas —---> su función atrapar notas y se le suma la función de seleccionar nivel. El botón comienza actuando con la función de seleccionar nivel y una vez seleccionado, la función del botón cambia a atrapar para su uso en el juego. 

Yo soy la encarga de subir todo al readme y que no explote github con información, esa clase agregue todo lo que era el computador, interfaz de usuario, porque los elegimos y los metodós, atributos y constructores

 ```cpp

//constructor del botón
//boton1:
//botón seleccionador
//boton2: 
//botón de Movimiento

//métodos del botón
//boton1:
//puede encender
//puede apagar
//puede seleccionar
//tiempo de antirrebote 
//boton2:
//puede subir - se mueve en el eje y
// mientras no se aprieta, el personaje cae 

//atributos del botón
//boton1:
// noPresionado a estaSiendoPresionado
//está presionado por 2sg
//tiempoParaMantener (2000ms) 
//está encendido
//está apagado
//está presionado por menos de 2sg
//está seleccionando una opción

//boton2:
// noPresionado a estaSiendoPresionado
//está presionado 
//constructor de las perillas
//perilla1:
//perilla seleccionadora de 4 personajes
//número de patita, deduce a qué canal del lector analógico 
//perilla2: 
//Encargada de regular la velocidad del mapa / juego 
//Recibe el número del pin físico al que está conectada esta perilla 


//métodos de la perilla
//perilla1:
//Divide el rango total del potenciómetro entre el número de personajes. 
//calcular el tamaño que le toca a cada personaje en el giro. 
//Determina en qué "casilla" cae la lectura filtrada actual. 
//perilla2:
//Toma la lectura filtrada interna 
//Cuando la perilla está al mínimo (0.0), el mapa avanza a la velocidad mínima (por ejemplo, 20 píxeles por segundo). 
//Cuando la perilla está al máximo (1.0), el mapa avanza a la velocidad máxima (por ejemplo, 90 píxeles por segundo).
//En cualquier punto intermedio, calcula la velocidad proporcional exacta para acelerar o frenar el avance del escenario suavemente.

//atributos de la perilla
//perilla1:
//Para recordar en qué patita físicamente está conectada la perilla y qué canal del lector analógico le toca consultar. 
//Un entero que recuerde qué opción (0, 1, 2 o 3) estaba seleccionada en la última comprobación.    
//perilla2:
//Un número entero que almacena la posición física limpia de la perilla tras procesar la señal del convertidor 


 ```



## encargos

## lectura
lectura pag 23 a la pagina 35 
- en el mundo en linea ya no funciona el concepto de contexto fisico de la galeria/museo
- esta la mala opinión de que ideas se presenten en internet significa que "pertenecen a internet" eso se vuelve una opinión errónea
- coleccionar arte
- el arte digital en cierta parte es efímero e inestable 
- Efímero es un adjetivo que describe algo pasajero, de corta duración o que dura un solo día


**Capitulo 1**

"la presencia de una obra de arte en el tiempo y el espacio, 'su existencia única en el lugar donde se encuentra', constituía la autenticidad, la autoridad y el 'aura' del objeto artístico, todo lo cual parecía estar en peligro por las posibilidades de la reproducción mecánica y la creación de copias idénticas" (pag 28)
- composición de diferentes formas de imagenes, la superposición o la mezcla de elementos visuales
- Nancy Burson pionera en el campo de las fotografias generadas por computador
- Morphing
- manipulación de imagenes

"composición y collage que permite el procesamiento digital, un hecho al que se hace referencia con frecuencia en el arte" (pag 33)
- kiding= empresa que traspasaba los limites entre el arte y la estetica publicitaria 
