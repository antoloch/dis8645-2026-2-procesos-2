# Prompts del desarrollo del muestrario de sonidos en Wokwi

## 01 · Primer funcionamiento del prototipo

Quiero representar en Wokwi la idea de un muestrario interactivo de sonidos cotidianos. El dispositivo utilizará una Raspberry Pi Pico 2 W, un parlante y tres botones físicos, cada uno con un tema. Cada botón tendrá tres sonidos diferentes: al presionarlo, se reproducirá uno y, con cada pulsación nueva, se avanzará al siguiente hasta volver al primero. En total serán nueve sonidos. También estamos considerando una perilla para variar el tono.

## 02 · Archivos necesarios para Wokwi

¿Cuáles de los archivos del prototipo tengo que colocar en Wokwi y cómo debo hacerlo?

## 03 · Metodología de desarrollo

Este prototipo es un borrador por ahora. Después vamos a trabajar completamente en C++ y con la Raspberry Pi Pico 2 W, como en el proyecto anterior, utilizando la misma metodología que seguimos con DORE 8.

## 04 · Duración de los sonidos

Quiero ajustar el funcionamiento para que cada sonido tenga un tiempo máximo de reproducción. ¿Cómo podemos incorporar ese límite al prototipo?

## 05 · Selección de sonidos y perillas

Quiero considerar dos perillas: una para modificar el tono y otra para elegir entre AUTO, 1, 2 y 3 antes de presionar un botón. En AUTO, cada pulsación debe avanzar por los tres sonidos de su categoría; en las otras posiciones, debe poder seleccionarse directamente el sonido indicado.

## 06 · Interrupciones y secuencia de reproducción

Si se presiona otro botón mientras suena un audio, quiero que el sonido anterior se detenga y que haya una pausa breve antes de reproducir el nuevo. Cada botón debe conservar su propia secuencia de sonidos del 1 al 3 y volver al primero cuando corresponda.

## 07 · Reproducción en bucle, espera y sonido oculto

Quiero revisar cómo incorporar la reproducción en bucle, un estado de espera cuando no se está utilizando el dispositivo y un sonido oculto que se active con una combinación de los tres botones. ¿Cómo se puede representar esto en el prototipo?

## 08 · Cambio a grabaciones propias

Ahora queremos que el proyecto sea un repertorio de sonidos propios, grabados o elegidos por nosotros dentro de la universidad, como un soundboard. Así podremos definir un tema más específico y trabajar con sonidos que correspondan a ese entorno, sin depender de grabaciones ajenas.

## 09 · Reproducción de audio real

Quiero mantener los tres botones y los nueve sonidos, pero utilizar grabaciones reales en lugar de los tonos de prueba. Revisemos cómo incorporar un DFPlayer Mini, una tarjeta microSD y un parlante. Para esta versión también estamos considerando perillas de volumen y tono.

## 10 · Revisión del Wokwi anterior

¿El Wokwi anterior permite expresar todo esto del nuevo proyecto o tengo que hacer uno nuevo que permita representar los cambios que planteamos?

## 11 · Actualización de la simulación

Sí, hagámoslo para que esté lo más fiel y cercano posible a los últimos ajustes y para que ya no tenga los nombres de los videojuegos, porque el proyecto dejó de tratarse de eso.

## 12 · Prueba de los botones

¿Cómo hago para probar los tres botones en Wokwi?

## 13 · Revisión de la lógica y el pseudocódigo

¿Todo esto nuevo incorpora las correcciones que nos hicieron? El lenguaje debe estar en presente, no en futuro. Además, el inicio del pseudocódigo debe tener el contexto necesario para comprender por sí solo qué hace el proyecto, sin presuponer que quien lo lea ya conoce su funcionamiento.

## 14 · Conexiones para el esquemático

Necesito identificar claramente las conexiones del proyecto en Wokwi para poder realizar el esquemático. ¿Me puedes indicar qué componentes se conectan a cada pin de la Raspberry Pi Pico 2 W, incluidos los botones, la salida de sonido y las perillas?
