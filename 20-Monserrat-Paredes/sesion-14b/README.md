# sesion-14b → 20/11/26

## apuntes sesión

Presentar proyecto-03

Documento de google sobre el tiempo → <https://docs.google.com/document/d/1sypus6ZvVz7lHq5YcEqVzASbmHK05lBOpeybLViu8UI/edit?tab=t.0#heading=h.sjclanj7cy8k>


Referencias: Rosa ayuda a explicar por qué sentimos que el tiempo no alcanza; Sharma cuestionar por qué no todos tenemos las mismas condiciones para vivir y administrar ese tiempo.

Hartmut Rosa

Sociólogo y teórico social alemán, nacido en 1965. Es profesor de sociología en la Universidad Friedrich Schiller de Jena.

Su investigación se centra en la aceleración social: cómo la sociedad moderna funciona cada vez más rápido y cómo esto afecta nuestra vida cotidiana. Rosa explica que vivimos bajo exigencias de hacer más cosas en menos tiempo, lo que puede generar presión, estrés y la sensación de que las horas no alcanzan.

PDF → <https://acceleratedclassroom.wordpress.com/wp-content/uploads/2011/03/social-acceleration.pdf>


Sarah Sharma

Académica canadiense, profesora de teoría de medios en la Universidad de Toronto. Investiga la relación entre el tiempo, la tecnología, el trabajo y las desigualdades sociales.

Su trabajo cuestiona la idea de que todas las personas viven el tiempo de la misma manera. Plantea que las experiencias temporales están condicionadas por las relaciones de poder, el trabajo y las distintas posiciones que ocupamos en la sociedad.

PDF → <https://api.pageplace.de/preview/DT0400.9780822378334_A35629120/preview-9780822378334_A35629120.pdf?ut>


---


## PROYECTO 03 – Videojuego de nave 

## 1. Las 10 clases

| N.º | Clase | Qué hace |
|:---:|---|---|
| 1 | **Boton** | Lee un pulsador con antirrebote de 30 ms y avisa una sola vez por pulsación. Se usa para disparo, inicio/pausa/reanudar y el interruptor. |
| 2 | **Perilla** | Lee el potenciómetro con el ADC y entrega el volumen de 0 a 100 %. |
| 3 | **Inclinacion** | Lee el MPU6050: si lo inclinas a un lado, la nave pasa a ese lado (izquierda o derecha). |
| 4 | **Pantalla** | Maneja la LCD 2004: vidas, nivel, puntos y las animaciones de inicio, level up y game over. |
| 5 | **MotorAudio** | Reproduce los 14 sonidos por el canal izquierdo, derecho o ambos, sin bloquear el juego. En el físico los envía por I2S al Pico Audio Pack. |
| 6 | **Nave** | Guarda el sector, las vidas (máx. 3) y los puntos, y da una vida extra cada 100 puntos. |
| 7 | **Dificultad** | Lleva el tiempo de juego (sin contar la pausa) y el nivel, que sube cada 15 s. |
| 8 | **Amenaza** | Es un enemigo o un obstáculo que aparece, se acerca y llega. Solo el enemigo se puede disparar. |
| 9 | **GestorAmenazas** | Crea amenazas al azar, las actualiza y resuelve el disparo. |
| 10 | **Juego** | Coordina todo con los estados apagado, espera, jugando, pausa y game over. |

## 2. BOM final (portátil)

| N.º | Componente | Cantidad | Precio (CLP) | Enlace |
|:---:|---|:---:|---:|---|
| 1 | Raspberry Pi Pico W con pines soldados (hacia abajo) | 1 | $11.290 | [Comprar](https://raspberrypi.cl/products/raspberry-pi-pico-w-con-conexion-wifi) |
| 2 | Pico Audio Pack (Pimoroni PIM544) | 1 | $20.000 | [Comprar](https://thepihut.com/products/pico-audio-pack-line-out-and-headphone-amp?srsltid=AYCX9vK1tTY8zKseWunwPfP0f0sHdsMd8y_xIhHtjg09X2NUH4LuyGxf) |
| 3 | Audífonos estéreo con jack 3,5 mm | 1 | - | - |
| 4 | Botones pulsadores | 2 | $500 | [Comprar](https://afel.cl/products/kit-boton-tactil-6x6x5-interruptor-5-unidades) |
| 5 | Interruptor deslizante | 1 | $300 | [Comprar](https://www.mechatronicstore.cl/mini-switch-interruptor-proto-3-pin/?attribute_tamano=5.7x19.8mm&gad_source=1&gad_campaignid=22963321414&gbraid=0AAAAAqH0PVPTVAN6fsocfXnAEohDuLlhg&gclid=Cj0KCQiA2frXBhC_ARIsAPTFcdMu42zb8HK8XkSLshWaRKeFXZTj4N4wuSPT2Hx-EFNbEUKIZeBDWowaAt3vEALw_wcB) |
| 6 | Potenciómetro 10 kΩ | 1 | $500 | [Comprar](https://afel.cl/products/potenciometro-10k-ohm) |
| 7 | Sensor MPU6050 (GY-521) | 1 | $4.000 | [Comprar](https://afel.cl/products/sensor-giroscopio-acelerometro-gy-521-mpu6050?variant=45125238915224&country=CL&currency=CLP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&utm_term=&utm_campaign=@+Smart+Shopping+2&utm_source=adwords&utm_medium=ppc&hsa_acc=1808722794&hsa_cam=19569174895&hsa_grp=&hsa_ad=&hsa_src=x&hsa_tgt=&hsa_kw=&hsa_mt=&hsa_net=adwords&hsa_ver=3&gad_source=1&gad_campaignid=19560974580&gbraid=0AAAAADBMsFRk1PsXMSeyznFKAyCuN4hvy&gclid=Cj0KCQiA2frXBhC_ARIsAPTFcdPDwm8yk8GtXc9UsI5vPKAeW25GfOHtz_r5iiWCksDefua17uIzdc0aAh9LEALw_wcB) |
| 8 | LCD 2004 (20x4) con módulo I2C | 1 | $7.853 | [Comprar](https://www.mercadolibre.cl/modulo-lcd-20x4-con-interfaz-2004-para-display/p/MLC2035293542) |
| 9 | Conversor de nivel lógico (traductor) I2C 5V a 3.3V | 1 | $2.000 | [Comprar](https://afel.cl/products/conversor-de-nivel-logico-i2c-5v-a-3-3v?srsltid=AYCX9vJUmmicTz9JrkdNi7dlYMKtfUp1EhtzUKM3RpkZJUr4pLrNj4Cg) |
| 10 | Módulo de carga TP4056 | 1 | $1.990 | [Comprar](https://mcielectronics.cl/shop/product/modulo-de-cargador-tipo-c-tp4056-5v-1a-tipo-c-usb-placa-de-proteccion-de-bateria-de-litio/?srsltid=AYCX9vIyVNvNyB02mgALR-N4b41AijCS616Gkl80tmAXan_sYdMf-AiB) |
| 11 | Batería LiPo 3,7 V, 2000 mAh | 1 | $7.990 | [Comprar](https://maxelectronica.cl/fuentes-de-alimentacion/910-bateria-li-po-37v-74wh-2000mah-modelo-103450-con-conector-jst-ph20.html) |
| 12 | Regulador de voltaje (boost) a 5 V, ajustado a 5 V | 1 | $1.490 | [Comprar](https://www.mechatronicstore.cl/convertidor-dc-dc-step-up-mt3608-2a-regulable-boost/?gad_source=1&gad_campaignid=22963321414&gbraid=0AAAAAqH0PVPTVAN6fsocfXnAEohDuLlhg&gclid=Cj0KCQiA2frXBhC_ARIsAPTFcdPPmZqqtpm79VzlI_cAvMVh0R7eOpORpOEZA6IM7ZCAg5Pc-ZisR5waAuDeEALw_wcB) |
| 13 | Protoboard de 830 puntos | 1 | $2.930 | [Comprar](https://altronics.cl/breadboard-830) |
| 14 | Cables Dupont | 1 | $2.000 | [Comprar](https://afel.cl/products/pack-60-cables-de-conexion) |
| 15 | Cable micro-USB de datos | 1 | - | - |
| 16 | Cable USB-C (cargar la batería por el TP4056) | 1 | - | - |
| 17 | Soldador, estaño y cables finos | 1 | - | - |


## 3. Conexiones físicas

Los números entre paréntesis son los pines físicos de la Pico (1 a 40).

| Componente | Pin del componente | Va conectado a |
|---|---|---|
| Botón disparo | Terminal 1 / terminal 2 | GP2 (pin 4) / GND |
| Botón inicio | Terminal 1 / terminal 2 | GP3 (pin 5) / GND |
| Interruptor | Dos terminales | En serie con el positivo, entre TP4056 y boost |
| (Para el interruptor) | Cable fijo | GP4 (pin 6) a GND |
| Potenciómetro | Extremo 1 / centro / extremo 3 | 3V3 (pin 36) / GP26 (pin 31) / AGND (pin 33) |
| MPU6050 | VCC / GND | 3V3 (pin 36) / GND |
| MPU6050 | SDA / SCL | GP6 (pin 9) / GP7 (pin 10) |
| Convertidor de nivel | LV / HV / GND | 3V3 / 5 V del boost / GND |
| Convertidor de nivel | LV1 / LV2 | GP6 / GP7 |
| LCD 2004 | VCC / GND | 5 V del boost / GND |
| LCD 2004 | SDA / SCL | HV1 / HV2 del convertidor |
| Pico Audio Pack | Datos / BCK / LRCK | GP9 (pin 12) / GP10 (pin 14) / GP11 (pin 15) |
| Pico Audio Pack | Mute | GP22 (pin 29), por verificar |
| Pico Audio Pack | 3V3 / GND | 3V3 (pin 36) / GND, alimentación por verificar |
| Audífonos | Jack 3,5 mm | Salida de audífonos del Audio Pack (interruptor de ganancia en bajo) |
| LiPo | + / − | B+ / B− del TP4056 |
| TP4056 | OUT+ / OUT− | Interruptor, luego boost (IN+ / IN−) |
| Boost | OUT 5 V | VSYS (pin 39), LCD y convertidor de nivel |
| Boost | GND | GND común de todo |


## encargos

## lectura

Libro: A New Program for Graphic Design

Autor: David Reinfurt

El libro está dividido en 3 grandes capítulos.

I. T--Y-P-O-G-R-A-P-H-Y

II. G-E-S-T-A-L-T

III. I-N-T-E-R-F-A-C-E

El autor plantea las bases de lo que significa enseñar diseño gráfico hoy. Introduce la idea de que el diseño no se trata de "estilo" o decoración, sino de sistemas, reglas y tecnología aplicadas a la comunicación.
