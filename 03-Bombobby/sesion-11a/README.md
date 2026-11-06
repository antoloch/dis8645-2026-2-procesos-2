# sesion-11a

## apuntes sesión

Para no perder tiempo haciendo el modelado del computador elegido, utilicé un stl que ya existiera y se me hiciera fácil de modificar.
Al importarlo al Rhino noté el primero de mis problemas, la malla principal de este stl venía abierta, lo cual me complicaría las cosas ya sea al momento de modificarlo o de imprimirlo. Por eso intenté convertir la malla a una polisuperficie, pero la cantidad de polígonos que traía esta malla era un crimen; 500 mil polígonos… intenté reducirlos pero se veía bastante deplorable el modelado, intenté 1000 cosas más para que se viera decente. Pero desaparecía un problema y aparecían 2 más.
Me rendí y empecé desde cero con la malla, empecé a tapar los bordes desnudos a mano y a usar el asombroso “ShrinkWrap” lo cual permitió que mi malla se vea bastante lisa y sin esa brutal cantidad de polígonos.

La malla trasera fue una maravilla, no tuve que cambiar casi nada de ella, ojalá así hubiera sido la primera, pero weno… el primero siempre sale feo dicen por ahí, yo intento negar ese dicho porque soy la primera hija de mis padres, pero que le voy a hacer.

Modelé la pantalla, los botones, potenciometros y protoboard a usar para lograr un encaje lo más perfecto posible. Todo fue medido con pie de metro.

Solo tuve que hacer diferencia booleana de malla, dejando un margen de 0.2 mm por pieza para que efectivamente entren en la carcasa sin dificultad.

## encargos

## lectura
