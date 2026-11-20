# sesion-13b

## apuntes sesión

sesión-13b (viernes 13):

- descripción total del proyecto
- prototipos
- pseudocódigo
- esqueleto en wokwi

---

## Texto descriptivo del proyecto

Nuestro proyecto n°3 se basará en crear un juego donde el jugador debe interactuar con el sistema mediante botones pulsadores y utilizar el sonido como principal medio de comunicación para comprender lo que sucede durante la partida.

El juego consiste en controlar una nave espacial que debe sobrevivir a diferentes ataques enemigos, esquivando obstáculos y eliminando adversarios mediante disparos. A medida que transcurre el tiempo, la dificultad aumenta progresivamente, generando nuevos desafíos que exigen mayor atención y capacidad de reacción. A través de diferentes señales sonoras, el jugador podrá identificar la ubicación de los enemigos, anticipar ataques, reconocer daños en su nave y recibir información sobre su desempeño.

El jugador deberá interpretar las señales auditivas para tomar decisiones, desplazarse entre diferentes posiciones y disparar contra los enemigos antes de recibir un ataque.  La partida termina cuando la nave pierde todas sus vidas. El sistema comunica el resultado mediante una secuencia sonora y permite iniciar una nueva partida. 


### 1. **Mecánica y funcionamiento** 

El videojuego funciona mediante tres posiciones imaginarias: izquierda, centro y derecha. Estas posiciones representan los diferentes sectores del espacio por los que pueden aparecer los enemigos y desplazarse la nave del jugador. Durante la partida, los enemigos aparecen aleatoriamente en cualquiera de estos sectores. El jugador debe reconocer su ubicación mediante las señales emitidas por el parlante y utilizar los botones para desplazarse o disparar. 


### El funcionamiento general se desarrolla de la siguiente manera:

1. **Inicio:** el jugador presiona el botón de inicio y el sistema reproduce una señal sonora que indica que la partida ha comenzado.  
2. **Desarrollo:** aparecen enemigos y obstáculos en diferentes posiciones.  
3. **Identificación:** el parlante reproduce señales específicas que permiten reconocer la ubicación y proximidad de los enemigos.  
4. **Interacción:** el jugador utiliza los botones para desplazarse y disparar.  
5. **Supervivencia:** debe evitar los ataques enemigos y eliminar adversarios para conseguir puntos.  
6. **Dificultad progresiva:** a medida que aumenta el tiempo de juego, los enemigos aparecen con mayor frecuencia y velocidad.  
7. **Finalización:** cuando el jugador pierde todas sus vidas, se reproduce una señal de derrota y se comunica la puntuación final.


### Cada sonido tendrá un significado específico:

* **Pitido hacia la izquierda:** indica la presencia de un enemigo en el sector izquierdo.  
* **Pitido hacia la derecha:** indica la presencia de un enemigo en el sector derecho.  
* **Dos pitidos rápidos:** advierten que un enemigo se está aproximando.  
* **Sonido grave:** indica un ataque enemigo.  
* **Sonido de explosión:** comunica que un enemigo ha sido eliminado.  
* **Sonido de impacto:** indica que la nave recibió un daño.  
* **Alerta repetitiva:** advierte que el jugador se encuentra en su última vida.  
* **Melodía de recompensa:** indica que se consiguió una puntuación adicional o una mejora.  
* **Secuencia de derrota:** comunica que la partida ha terminado.

**Botón 1 – Movimiento hacia la izquierda:** permite desplazar la nave desde el centro hacia el sector izquierdo.

**Botón 2 – Movimiento hacia la derecha:** permite desplazar la nave desde el centro hacia el sector derecho.

**Botón 3 – Disparo:** permite atacar a los enemigos que aparecen durante la partida.

**Botón 4 – Inicio y pausa:** permite comenzar el videojuego, pausar la partida y reiniciarla cuando corresponda.


Estados del juego = ESPERA → JUGANDO ⇄ PAUSA → GAME OVER → (nueva partida)
