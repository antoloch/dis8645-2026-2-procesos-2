# sesion-11b

## apuntes sesión
Después de modificar las clases de los botones y sus funciones para que pudiesen abrir las carpetas de nuestra pantalla. Hicimos una mejora a la clase de las perillas para que su funcionamiento sea más fácil para el usuario, antes funcionaba de manera que si la perilla estaba movida hacía cualquier ángulo de la derecha, el cursor se movía hacia el extremo de su eje. Pero nosotros para que fuera más fácil tuvimos que a cada porcentaje de cada perilla, asignarle un valor que sea la posición del cursor en la pantalla respecto a cada eje.
Perilla X (GP27): Si está al 0% 
→
→ pixel 0 (extremo izquierdo). Si está al 50% 
→
→ pixel 61 (centro). Si está al 100% 
→
→ pixel 122 (extremo derecho).
Perilla Y (GP26): Si está al 0% 
→
→ pixel 9 (debajo de la barra de menú). Si está al 50% 
→
→ pixel 32 (centro vertical). Si está al 100% 
→
→ pixel 55 (fondo de pantalla).
Posición absoluta: El cursor se queda exactamente en el punto donde dejes la perilla, igual que en un Etch A Sketch. No se desplaza solo hacia los extremos ni acelera.
Sobremuestreo suave: Se promedian 4 lecturas consecutivas por vuelta para eliminar vibraciones en Wokwi.

## encargos

## lectura
