# sesion-15a

martes 2026-11-24

## apuntes sesión

- hacer listado de materiales excel 
- tomar todo el README y pasárselo a claude o IA, preguntar por factibilidad, tarjeta interna, viabilidad
- subir la factibilidad que dio la IA al discord

### conversación con claude
---

Veredicto: el proyecto es viable y factible, con un riesgo técnico principal: el audio real por I2S, que Wokwi no puede probar. Dejé fuera lo que no pude comprobar (carcasa, código, fotos).

Una observación previa: la fecha de entrega (20-11) ya pasó. Te evalúo el proyecto tal como está en el README, y si quieres seguir mejorándolo, lo que sigue sirve de lista de pendientes.

### Por qué es viable

 - **El concepto es coherente.** Juego tipo Asteroids, sonido como canal principal y una sola pantalla de apoyo. La restricción se entiende y se puede explicar.

 - **El alcance es modesto.** Tienes 10 clases bien separadas, 14 sonidos definidos y un sistema de niveles simple (cada 15 s). Se puede jugar sin ser sofisticado.

 - **El diseño sonoro está pensado.** Graves para peligro, agudos para información, tonos que suben para premio y que bajan para pérdida. Eso es lo que hace funcionar un juego solo con audio.

### El costo es bajo.

El BOM suma unos CLP $63.000 sin contar audífonos, cables ni soldador. El Pico Audio Pack es lo más caro, y no sé cuánto suman el envío e impuestos de importarlo.

### Riesgos de factibilidad (de mayor a menor)

 1. **Audio I2S sin simulación.** Wokwi no emula el Pico Audio Pack, así que el MotorAudio real (14 sonidos, canal izquierdo/derecho/ambos, sin bloquear el juego) está sin probar en hardware. Lo más seguro es generar el audio por PIO/DMA con una librería I2S para Pico (pico-extras o la de Pimoroni), y probar eso primero, antes de armar el resto.

 2. **Acceso a los pines con el Pico enchufado al pack.** El Pico va con pines hacia abajo para insertarse en el Audio Pack. Verifica cómo llegas a GP2, GP3, GP4, GP6, GP7 y GP26 sin que el pack estorbe, y si el protoboard sirve para eso.

 3. **Alimentación.** Si alimentas VSYS con el boost, necesitas un diodo Schottky, porque si no el USB y la batería se pelean al conectar el cable. Ajusta el boost a 5 V antes de conectar nada. El interruptor que ya pusiste entre el TP4056 y el boost es una buena decisión.

 4. **Inconsistencia entre esquemático y BOM.** El BOM compra el módulo TP4056, pero si tu esquemático en KiCad dibuja el chip TP4056 (ESOP8), no coinciden. Conviene dibujar el módulo como un símbolo de pocos pines (B+, B−, OUT+, OUT−), o comprar el chip y diseñar la PCB, lo que sería otro proyecto.

 5. **Control con giroscopio.** Inclinar en X para cambiar de sector funciona, pero necesita calibración y un umbral con zona muerta, o la nave va a temblar. El README no dice cuántos sectores hay (¿izquierda, centro, derecha?).

### Observaciones de concepto

 - "Binaural" no es lo que hacen. Reproducir un sonido por el canal izquierdo, el derecho o ambos es paneo estéreo. El audio binaural simula la posición con diferencias de tiempo y filtrado de la cabeza (HRTF), y con tu hardware no es realista. Te sugiero llamarlo "paneo estéreo" o "espacialización por canales". Para dar distancia puedes variar volumen o tono según qué tan cerca está la amenaza.

 - Christine Sun Kim. Su obra parte de la sordera. Tu juego quita la vista y deja el oído, que es casi lo inverso. El vínculo funciona si lo dices así: "invertimos la restricción para entender lo que implica depender de un solo sentido".

 - Parlantes de 0,5 W. La tabla habla de ellos, pero el BOM solo tiene audífonos. Aclara cuál usan, porque eso afecta qué graves se oyen.

### Qué haría ahora

 - Probar un tono por I2S en el Pico real, izquierda y derecha por separado.
 - Armar solo botón + giroscopio + audio y jugar una versión mínima sin LCD ni batería.
 - Sumar LCD y alimentación a batería al final, que es lo más fácil de resolver.
 
 ---

### decisiones para llevar acabo el examen
-  color de filamento para carcasa (blanco o azul transparente/ambos filamento PLA)
-  audífonos con entrada jack, no es un componente
-  cambiar potenciometro de B10k por uno de B100k (todos los grupos usara el mismo)
-  cambiar switch mini por uno mas grande
-  **Pico Audio Pack (Pimoroni PIM544)** ya que no se encuentra en ninguna de las tiendas, alternativa utilizar **PCM5102**
-  el proyecto no es necesario que sea inalambrico


## encargos

## lectura
nos dejaron elegir un libro para leer durante el semestre en el cual debemos dejar 2 citas por clase y leer mínimo 100 paginas durante el semestre

libro escogido **La Música electroacústica en Chile** de Federico Schumacher

### capítulo leído del libro 
- Los años negros
   - los 80 fueron la época más floja: casi nadie seguía haciendo electroacústica después del ramo obligatorio
   - hubo muy pocos conciertos y el ambiente seguía súper tenso por la dictadura
   - cerraron composición por "poco rentable" y el ramo de electrónica pasó a ser pura teoría
   - igual llegó gente de la música popular que cachaba de tecnología y salvó el futuro del género



### citas del libro

**cita 1**: 

**_"...ciertamente no hay otro período en nuestra historia más anémico..."_**

página 98

**opinión:** estaba más pa allá que pa acá 


**cita 2**: 

**_"...con un mínimo apoyo financiero y con un máximo de buena voluntad..."_**

página 101

**opinión:** me sonó como la frase "todos para uno y uno para todos" xd
