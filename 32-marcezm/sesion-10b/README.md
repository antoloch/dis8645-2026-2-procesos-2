# sesion-10b
Viernes 23 de Octubre

## apuntes sesión

Durante esta clase pude comprender mejor cómo se estaban definiendo los aspectos técnicos y visuales de nuestro proyecto 02. 

Identificamos los componentes electrónicos necesarios y entender qué función tendría cada uno dentro del dispositivo, considerando la interacción que buscamos generar con el usuario.

También se avanzó en la definición de su apariencia física. Pensamos que lo mejor seria fabricar una carcasa mediante impresión 3D, tomando como referencia los computadores de juguete, con el propósito de mantener una estética reconocible y cercana al concepto inicial.

En la programación, se trabajó con las perillas y su funcionamiento en la Raspberry Pi Pico 2 W. Mediante pruebas con potenciómetros, se exploró cómo interpretar sus valores analógicos para convertirlos en selecciones dentro del dispositivo. Esto permitiría controlar distintas funciones, como elegir emociones o desplazarse entre los modos de pantalla.

Un aprendizaje importante de esta sesión fue comprender cómo una misma estructura de código puede adaptarse a diferentes necesidades, dependiendo de la cantidad de opciones que se quieran controlar. Además se revisó la detección de cambios en las posiciones para evitar que el programa repitiera información innecesariamente.

### Aprendizajes técnicos

Durante el desarrollo pude entender mejor cómo funciona la lectura analógica del potenciómetro en la Raspberry Pi Pico 2 W. Al girar la perilla, cambian los valores que recibe la placa y estos se transforman en información que el programa puede utilizar para realizar distintas acciones.

También comprendí que la cantidad de opciones influye en la precisión de la perilla. No es lo mismo seleccionar entre cuatro emociones que recorrer 38 caracteres o 81 posiciones. Por eso, es importante ajustar bien los valores para que cada selección funcione correctamente.

### Organización y reutilización del código

Algo que me pareció interesante fue poder utilizar una misma clase para diferentes proyectos. En este caso, la clase `Perilla` permite adaptar su funcionamiento según lo que necesita cada grupo, sin tener que modificar todo el código.

Esto me ayudó a entender la importancia de mantener una programación ordenada y que pueda reutilizarse en otras partes del proyecto, facilitando los cambios y las pruebas.

## encargos

## lectura
