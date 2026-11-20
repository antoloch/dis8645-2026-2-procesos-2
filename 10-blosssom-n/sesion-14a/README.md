# sesion-14a

## apuntes sesión

Seguimos aterrizando cómo va a funcionar nuestro proyecto, que todavía necesita otro nombre!! esta vez nos enfocamos harto en resolver qué pasa con cada botón y cómo dejar la grabación en manos de la persona cuidadora.

Acotamos el uso a niños de 3 a 4 años y decidimos que la máquina se quede en un lugar fijo, como el velador. con eso sacamos la batería y el power bank. También dejamos de separar el contenido entre día y noche, porque los cuentos y la música podrían usarse en distintos momentos. por ahora vamos a organizar todo por categorías.

### los botones

Pensamos en tres botones principales, uno para cuentos, otro para música y otro para el mensaje grabado. al apretar cuentos o música empieza a sonar un audio y, si apretamos de nuevo el mismo botón, pasa al siguiente dentro de esa categoría. el tercer botón reproduce el mensaje que dejó la persona cuidadora.

Queremos que esto se pueda entender aunque el niño todavía no lea. cada botón tendría un personaje o icono que también aparezca en pantalla cuando suena el audio. todavía tenemos que diseñarlos, pero la relación tiene que mantenerse para que se pueda aprender qué hace cada uno al usarlo.

También revisamos que el programa tiene que distinguir entre apretar una vez, dejar apretado y tocar varias veces seguidas. para eso vamos a registrar el estado anterior del botón y los tiempos de pulsación. queda pendiente definir cómo se pausa o detiene el audio.

### grabación y control de la persona cuidadora

Para que el niño no borre el mensaje sin querer, pensamos dejar la grabación en un módulo separado, con micrófono y su propio botón. la persona cuidadora lo conecta, mantiene apretado para grabar y lo suelta para terminar. mientras graba queda un led encendido y, al guardar, la pantalla muestra «Mensaje guardado».

Decidimos guardar un solo mensaje de hasta un minuto. cuando se graba otro, reemplaza al anterior en la sd. también pensamos qué debería pasar si algo falla, como que no se detecte el micrófono o no se pueda guardar el archivo. ahí la pantalla tendría que mostrar un error.

Para el proyecto 3 vamos a mantener el módulo desmontable y simular primero cómo funciona. después tenemos que revisar si conviene dejarlo conectado por un cable que permita acercar el micrófono, porque grabar agachado al lado del velador podría ser incómodo.

### qué vamos a probar primero

Dejamos una biblioteca inicial de tres cuentos de hasta cinco minutos, cuatro piezas de música infantil de hasta un minuto y el mensaje grabado. con eso queremos probar la navegación, la reproducción y la grabación antes de sumar más cosas.

Para elegir los cuentos vamos a revisar si se entienden solo escuchándolos. en la música, si la letra se entiende o si hay una melodía o ritmo que se pueda seguir. cada archivo tiene que quedar registrado con su título, autor o intérprete, fuente y licencia.

Los botones deberían ser grandes y distinguirse por su posición, símbolo y forma. todavía falta definir el tamaño, la separación y cuánta fuerza necesitan para que al apretarlos no se mueva toda la máquina. también tenemos que pensar cómo se ven con poca luz y cómo dejar las conexiones y piezas pequeñas protegidas dentro de la carcasa.


## encargos



## lectura

págs. 144–149

Por fin llegué al final del segundo capítulo!! me ha costado avanzar porque tengo muchas cosas que hacer y, como ya he dicho, este no es mucho mi estilo de lectura pipipii.

En estas últimas páginas aparece un poema donde las palabras forman una esfera que gira. después el autor repasa cómo el movimiento y la forma del texto cambian la manera de leerlo. también habla de que a veces hay que aprender a usar la obra para poder seguir lo que dice.


>
> “The emphasis is clearly on kinetic attributes of language.” (p. 146)
>
> “a viewer usually has to learn how to interact with digital works and discern their boundaries.” (p. 148)
>
