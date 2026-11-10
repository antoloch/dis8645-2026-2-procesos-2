# sesion-11b
Viernes 30 de Octubre

## apuntes sesión

### Avances

1. **Entendí cómo funciona una clase.** Partí de un código de `Perilla` que no entendía bien. Pedí a Claude que me explicara la diferencia entre clase y objeto, el constructor, `this->`, y cómo se relacionan el `.h` (declaración) y el `.cpp` (implementación).

2. **Corregí el constructor yo misma.** Cambié los parámetros a `nuevoPin` y `nuevaCantidadOpciones` para que no se llamen igual que los atributos y no haga falta `this->`. La primera vez lo cambié solo dentro del constructor y no en la cabecera, y eso daba error. Lo corregí en los dos lugares.

3. **Codigo funcional para ambos grupos.**
   - Conseguí el documento del grupo 04 con lo que ellos necesitan (`muestrear()`, `leer()`, `posicion(cantidad)`...).
   - Comparé con lo que necesita mi grupo (`leerValor()`, `seleccionarOpcion()`, `perillaEmociones`, `perillaNavegacion`).
   - Noté que se habían mezclado los dos documentos y lo corregí: separé qué era de cada grupo.
   - Aclaré que ambos grupos usan 2 perillas, así que bastaba una sola clase `Perilla`.

4. **Busqué referencias de estilo.** Conseguí ejemplos de clases hechas por otros (`Boton`, `BotonGrupo1`, `Boton03`) y los usé como guía para ordenar los comentarios y el encabezado.

5. **Busqué un minijuego.**
   - Descarté el del dinosaurio y decidimos hacer uno más simple, solo con cuadrados.
   - Busqué repositorios y encontré varios en Arduino, pero ninguno para Pico SDK en C++.
   - Analicé un código de Arduino con LCD 16x2 y vi qué ideas servían y cuáles no.

6. **Identifiqué la pantalla.** Averigüé que nuestra pantalla es una OLED de 1.3" 128x64 con chip **SH1106**, que es de píxeles y no de caracteres, y que eso cambia la librería que se puede usar.

## Qué falta

- [ ] Probar la `Perilla` en Wokwi
- [ ] Probar el juego en Wokwi y en la pantalla real
- [ ] Integrar el botón y las perillas con el juego

## Qué aprendí

- Cada objeto guarda sus propios datos aunque use la misma clase.
- Un `.h` solo declara y un `.cpp` implementa; mezclarlos causa errores.
- Si atributo y parámetro tienen el mismo nombre hay que usar `this->`; si son distintos no hace falta.
- Una clase para varios proyectos recibe como parámetros lo que cambia entre ellos.

## encargos

## lectura
