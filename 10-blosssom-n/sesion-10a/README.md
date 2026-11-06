# sesion-10a

## apuntes sesión

No nos anotamos para corregir en esta sesión, pero aun asi nos reunimos con mi grupito 07, y concretamos más o menos nestra idea!!! 
# TRAZO

## idea del proyecto

La idea es hacer una máquina de dibujo digital que se controle con dos perillas y dos botones.

En la pantalla aparece un punto que podemos mover de izquierda a derecha y de arriba abajo. Después, con un botón, decidimos si queremos que ese movimiento deje una línea o si solamente queremos cambiar el punto de lugar.

O sea, podemos movernos por la pantalla sin dibujar y activar el trazo cuando lleguemos a donde queremos empezar.

Le pusimos TRAZO porque la propuesta gira en torno a eso: mover un punto y decidir cuándo ese movimiento se convierte en dibujo.

## referente

Nuestro referente es MacPaint de 1984.

Tomamos el dibujo con píxeles, el blanco y negro y la idea de tener herramientas simples que permitan ver inmediatamente lo que estamos haciendo.

En nuestro caso vamos a llevar esa experiencia a controles físicos. Una perilla mueve el punto horizontalmente y la otra lo mueve verticalmente.

No vamos a reproducir todas las herramientas de MacPaint, porque para esta primera versión queremos trabajar con dos acciones principales: activar o desactivar el trazo y borrar.

[Referencia: MacPaint y QuickDraw — Computer History Museum](https://computerhistory.org/blog/macpaint-and-quickdraw-source-code/)

## qué queremos que pase

La persona puede empezar haciendo líneas rectas y después ir probando qué pasa cuando mueve las dos perillas al mismo tiempo.

Una mano controla el movimiento horizontal y la otra, el vertical. Entonces hay que ir coordinando los dos movimientos para construir diagonales, curvas o figuras.

La idea es aprender usándolo, porque probablemente al principio cueste un poco hacer la forma que una tiene en la cabeza jiji.

No hay una figura que tengamos que copiar, ni puntaje, ni tiempo límite. Podemos dibujar libremente y dejar el resultado en la pantalla hasta que decidamos borrarlo.

## qué hace cada cosa

| Elemento | Función |
| --- | --- |
| Perilla horizontal | Controla la posición del punto de izquierda a derecha. |
| Perilla vertical | Controla la posición del punto de arriba abajo. |
| Botón de trazo | Activa o desactiva el dibujo. |
| Botón de borrar | Borra todo el dibujo y deja el trazo desactivado. |
| Pantalla | Muestra el punto, las líneas y si estamos moviendo o dibujando. |

## las perillas

Las perillas controlan la posición del punto.

Esto significa que cada posición de la perilla corresponde a un lugar de la pantalla. Si giro una perilla hasta la mitad y la dejo ahí, el punto llega a esa posición y se queda ahí también.

No sigue avanzando solo.

### movimiento horizontal

Una perilla controla la coordenada horizontal, que sería la posición en el eje X.

- En el mínimo de su recorrido, el punto queda en el borde izquierdo.
- En el máximo, queda en el borde derecho.
- Aproximadamente a la mitad, queda al centro horizontal del área de dibujo.

### movimiento vertical

La otra perilla controla la coordenada vertical, que sería la posición en el eje Y.

- En el mínimo de su recorrido, el punto queda arriba.
- En el máximo, queda abajo.
- Aproximadamente a la mitad, queda al centro vertical del área de dibujo.

Entonces, si las dos perillas están a la mitad, el punto queda cerca del centro.

Si movemos solo una, podemos hacer una línea horizontal o vertical. Si movemos ambas, podemos ir construyendo diagonales o curvas, dependiendo de cómo coordinemos los giros.

El punto tampoco puede salir del área de dibujo ni aparecer al otro lado de la pantalla. Cuando llega al borde, ese es su límite.

## mover y dibujar

La máquina tiene dos estados: `MOVER` y `DIBUJAR`.

### MOVER

El punto cambia de posición, pero no deja líneas nuevas.

Lo que ya dibujamos se mantiene en la pantalla. Solamente estamos cambiando el lugar desde donde queremos seguir.

Es como levantar el lápiz del papel para llevarlo a otra parte, sin marcar todo el camino.

### DIBUJAR

El punto cambia de posición y va dejando una línea.

Acá sería como apoyar otra vez el lápiz y empezar a dibujar desde donde estamos.

| Estado | Qué pasa cuando movemos las perillas |
| --- | --- |
| `MOVER` | El punto se desplaza sin dejar un trazo permanente. |
| `DIBUJAR` | El punto se desplaza dejando una línea. |

## botón de trazo

El botón de trazo cambia entre esos dos estados.

Si estamos en `MOVER` y lo presionamos, pasamos a `DIBUJAR`.

Si lo volvemos a presionar, regresamos a `MOVER`.

No hay que mantenerlo apretado. Una pulsación cambia el estado y ese estado se mantiene hasta que volvamos a presionar.

Esto nos deja las dos manos disponibles para mover las perillas (porque si tuviéramos que mantener el botón apretado sería un poco complicado hacer todo a la vez).

Algo que tenemos que cuidar es que cada pulsación produzca un solo cambio de estado. Mantenerlo presionado no debería hacer que cambie muchas veces seguidas.

### dónde comienza cada trazo

Cada vez que activamos el dibujo, el trazo comienza en la posición actual del punto.

Por ejemplo, si dibujamos algo a la izquierda, desactivamos el trazo y nos movemos a la derecha, al volver a dibujar tenemos que comenzar ahí.

No debería aparecer una línea atravesando la pantalla para unir el dibujo anterior con el nuevo.

## cómo se forma el dibujo

Mientras estamos en `DIBUJAR`, el programa une la posición anterior del punto con la posición nueva.

Así buscamos que se vea una línea continua, incluso cuando movemos las perillas rápido, en vez de que queden puros puntos separados.

Para la primera versión vamos a trabajar con:

- Un solo grosor de línea.
- Trazos blancos sobre fondo negro, si usamos una pantalla OLED monocromática.
- Líneas que se van acumulando.
- Trazos que pueden pasar por encima de otros sin borrarlos.

O sea, si dibujo un círculo y después hago otra figura, el círculo sigue ahí. Y si paso por encima de una línea, eso no funciona como goma.

### el punto y el dibujo

El punto que indica dónde estamos tiene que verse también cuando el trazo está desactivado.

Pero ojo, que se vea no significa que tenga que dejar una marca.

Tenemos que separar el cursor del dibujo: el cursor se mueve, mientras que las líneas realizadas permanecen.

La forma exacta de ese cursor la vamos a ajustar cuando probemos la pantalla, para ver cómo se distingue mejor.

## botón de borrar

El segundo botón borra todo el dibujo.

Además, deja la máquina en `MOVER`, para que podamos ubicarnos de nuevo antes de empezar otro trazo.

Para esta primera versión el borrado será inmediato, sin una pregunta de confirmación. El botón tiene que estar identificado como `BORRAR`.

### qué pasa con la posición del punto

Borrar no devuelve el punto al centro.

Como su posición depende de las perillas, después del borrado sigue en el lugar que ellas indiquen.

Por ejemplo, si las perillas tienen el punto abajo a la derecha y presionamos borrar, el dibujo desaparece, pero el punto sigue abajo a la derecha.

Lo que se reinicia es el dibujo y el estado del trazo, no la posición de los controles.

## cómo se divide la pantalla

Vamos a separar la pantalla en dos partes:

| Zona | Contenido |
| --- | --- |
| Franja de estado | Muestra `MOVER` o `DIBUJAR`. |
| Área de dibujo | Muestra el punto y las líneas que hacemos. |

Las perillas solamente recorren el área de dibujo.

La franja de estado queda fuera de ese espacio, porque si pudiéramos dibujar encima terminaríamos tapando la información.

Todavía tenemos que confirmar el modelo y la resolución de la pantalla para definir cuánto espacio ocupa cada parte.

## qué pasa al iniciar

Cuando comienza la simulación:

1. El área de dibujo queda vacía.
2. La máquina parte en `MOVER`.
3. Se leen las posiciones de ambas perillas.
4. El punto aparece donde corresponde según esas posiciones.
5. Podemos moverlo y activar el trazo cuando queramos.

No tiene que partir siempre al centro, porque eso depende de cómo estén ubicadas las perillas.

En esta primera versión tampoco vamos a guardar los dibujos entre reinicios. Si reiniciamos la simulación, empezamos otra vez con el área vacía.

## ejemplo de uso

Supongamos que queremos dibujar dos figuras separadas.

Primero, con el trazo desactivado, movemos el punto hasta donde queremos empezar.

Presionamos el botón de trazo y hacemos la primera figura usando las perillas.

Después volvemos a presionar el botón para pasar a `MOVER`. Así podemos llevar el punto a otra parte sin dejar una línea entre las dos figuras.

Cuando llegamos al segundo lugar, activamos otra vez el trazo y seguimos dibujando.

Si queremos empezar todo de nuevo, presionamos `BORRAR`.

El recorrido sería:

1. Ubicar el punto.
2. Activar el trazo.
3. Dibujar.
4. Desactivar el trazo para cambiar de lugar.
5. Volver a activarlo para seguir, o borrar para empezar de nuevo.

## cosas por probar

- [ ] Confirmar qué pantalla vamos a usar y su resolución.
- [ ] Definir el espacio del dibujo y de la franja de estado.
- [ ] Probar que el recorrido de las perillas corresponda con el área de dibujo.
- [ ] Revisar cómo se ve el cursor.
- [ ] Comprobar que en `MOVER` el punto no deje rastros ni borre líneas existentes.
- [ ] Revisar que una pulsación cambie el estado una sola vez.
- [ ] Comprobar que al activar el trazo no se conecte con el dibujo anterior.
- [ ] Probar movimientos rápidos para ver si las líneas quedan continuas.
- [ ] Revisar que borrar elimine todo y deje el trazo desactivado.
- [ ] Identificar los controles para que se entienda qué hace cada uno.


## encargos

## lectura

### PAG 104-115

En estas páginas aparecen poemas donde las letras forman imágenes o están repartidas por la página. Entonces no importa solamente lo que dicen las palabras, sino también cómo se ven.

Después habla de poemas que se mueven y se van transformando en la pantalla. Me llamó la atención que el movimiento también pueda ser parte de lo que dice el poema, porque cambia cómo vamos leyendo las palabras.

>Citas
>
> “The space on the screen concretely represents a physical action.” (p. 113)
>
> “Mutation (instead of permutation) is the operative principle in most dynamic works” (p. 111).
