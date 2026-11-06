#include "AdafruitGFXPico.h"
#include "FuenteRetro.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>

// DORE 8 - Implementación para ST7789V físico (ILI9341 opcional en Wokwi).
// ========================================================================
// Controlador pantalla: ST7789V y funciones graficas
// ========================================================================

// 05 · IMPLEMENTACIÓN DEL CONTROLADOR TFT
// Se guardan los GPIO que manejarán la comunicación con la pantalla.
AdafruitGFX_Pico::AdafruitGFX_Pico(uint8_t cs, uint8_t dc, int8_t rst)
    : cs_(cs), dc_(dc), rst_(rst) {}

// Envía un comando al controlador: DC en 0 y CS activo durante el envío SPI.
void AdafruitGFX_Pico::comando(uint8_t valor) {
    gpio_put(dc_, false);
    gpio_put(cs_, false);
    spi_write_blocking(spi0, &valor, 1);
    gpio_put(cs_, true);
}
// Envía datos asociados a un comando (coordenadas, colores o configuración).
void AdafruitGFX_Pico::datos(const uint8_t *buffer, std::size_t n) {
    if (!n) return;
    gpio_put(dc_, true);
    gpio_put(cs_, false);
    spi_write_blocking(spi0, buffer, n);
    gpio_put(cs_, true);
}
// Esta clase conserva las primitivas de dibujo ya usadas por DORE 8.
// Solo cambia la inicializacion/orientacion segun el controlador.
// NO utiliza las bibliotecas de Arduino.
// Inicialización del bus y del panel; el modo físico usa ST7789V y Wokwi puede usar ILI9341.
void AdafruitGFX_Pico::begin() {
    // 12 MHz es un punto de partida prudente con cables Dupont/protoboard.
    // Se puede aumentar tras verificar el funcionamiento fisico.
    // Se inicia SPI0 a 12 MHz, velocidad conservadora para cables del prototipo.
    spi_init(spi0, 12000000);
    // GP18 y GP19 se reservan para el reloj y los datos de pantalla.
    gpio_set_function(18, GPIO_FUNC_SPI); // SCK
    gpio_set_function(19, GPIO_FUNC_SPI); // MOSI
    // Se configuran CS y DC como salidas, sin seleccionar aún la pantalla.
    gpio_init(cs_); gpio_set_dir(cs_, GPIO_OUT); gpio_put(cs_, true);
    gpio_init(dc_); gpio_set_dir(dc_, GPIO_OUT); gpio_put(dc_, true);

// En hardware físico se usa RST (GP20) para reiniciar el controlador ST7789V.
#if !defined(DORE8_PANTALLA_ILI9341)
    // El GMT020-02-8P fisico tiene RST independiente, conectado a GP20.
    if (rst_ >= 0) {
        gpio_init(static_cast<uint>(rst_));
        gpio_set_dir(static_cast<uint>(rst_), GPIO_OUT);
        gpio_put(static_cast<uint>(rst_), true);
        sleep_ms(10);
        gpio_put(static_cast<uint>(rst_), false);
        sleep_ms(20);
        gpio_put(static_cast<uint>(rst_), true);
        sleep_ms(150);
    }
#endif

    // Después del reinicio físico (si corresponde), se reinicia el controlador por software.
    comando(0x01); sleep_ms(150); // SWRESET (ambos controladores).

// Si la compilación define DORE8_PANTALLA_ILI9341, se elige la secuencia de Wokwi.
#if defined(DORE8_PANTALLA_ILI9341)
    // Mantener comportamiento de la pantalla ILI9341 simulada en Wokwi.
    comando(0x28);               // DISPOFF
    comando(0x3A);               // COLMOD: RGB565
    const uint8_t formato = 0x55; datos(&formato, 1);
    setRotation(1);
    comando(0x11); sleep_ms(150);
    comando(0x29); sleep_ms(30);
#else
    // ST7789V del panel fisico GMT020-02-8P (240 x 320).
    // En la ruta física ST7789V, se sale del modo reposo antes de dibujar.
    comando(0x11); sleep_ms(120);  // SLPOUT
    comando(0x3A);                 // COLMOD: 16 bits por pixel
    const uint8_t formato = 0x55; datos(&formato, 1);
    comando(0x21);                 // INVON: suele ser necesario en ST7789
    comando(0x13);                 // NORON: modo normal
    setRotation(1);                // Horizontal: 320 x 240
    comando(0x29); sleep_ms(80);   // DISPON
#endif
}

// Configura la orientación mediante MADCTL y actualiza ancho/alto en memoria.
void AdafruitGFX_Pico::setRotation(uint8_t r) {
    rotacion_ = r & 3;
// Si la compilación define DORE8_PANTALLA_ILI9341, se elige la secuencia de Wokwi.
#if defined(DORE8_PANTALLA_ILI9341)
    constexpr uint8_t direcciones[4] = {0x48, 0x28, 0x88, 0xE8};
#else
    // ST7789: MY, MX, MV. Si la imagen aparece girada o reflejada,
    // ajustar esta tabla despues de la primera prueba fisica.
    constexpr uint8_t direcciones[4] = {0xC0, 0xA0, 0x00, 0x60};
#endif
    comando(0x36); datos(&direcciones[rotacion_], 1); // MADCTL
    ancho_ = (rotacion_ & 1) ? 320 : 240;
    alto_ = (rotacion_ & 1) ? 240 : 320;
}
// Delimita el rectángulo de píxeles que recibirá la pantalla mediante comandos SPI.
void AdafruitGFX_Pico::ventana(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    comando(0x2A);
    const uint8_t xs[] = {uint8_t(x0 >> 8), uint8_t(x0), uint8_t(x1 >> 8), uint8_t(x1)};
    datos(xs, 4);
    comando(0x2B);
    const uint8_t ys[] = {uint8_t(y0 >> 8), uint8_t(y0), uint8_t(y1 >> 8), uint8_t(y1)};
    datos(ys, 4);
    comando(0x2C);
}
// Dibuja rectángulos rellenos; recorta lo que quede fuera de la pantalla.
void AdafruitGFX_Pico::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t color) {
    if (w <= 0 || h <= 0 || x >= ancho_ || y >= alto_ || x + w <= 0 || y + h <= 0) return;
    int x0 = std::max<int>(0,x), y0 = std::max<int>(0,y);
    int x1 = std::min<int>(ancho_,x+w), y1 = std::min<int>(alto_,y+h);
    if (x1 <= x0 || y1 <= y0) return;
    ventana(x0,y0,x1-1,y1-1);
    // Se utiliza un bloque pequeño de 128 bytes en lugar de guardar la pantalla completa en RAM.
    uint8_t bloque[128];
    for (int i=0;i<128;i+=2) { bloque[i]=uint8_t(color>>8); bloque[i+1]=uint8_t(color); }
    uint32_t pixeles = static_cast<uint32_t>(x1-x0)*(y1-y0);
    gpio_put(dc_,true); gpio_put(cs_,false);
    while (pixeles) {
        uint32_t n=std::min<uint32_t>(64,pixeles);
        spi_write_blocking(spi0,bloque,n*2);
        pixeles-=n;
    }
    gpio_put(cs_,true);
}
// Limpia la pantalla llamando al dibujo de un rectángulo del tamaño completo.
void AdafruitGFX_Pico::fillScreen(uint16_t color) { fillRect(0,0,ancho_,alto_,color); }
void AdafruitGFX_Pico::drawPixel(int16_t x,int16_t y,uint16_t c) { fillRect(x,y,1,1,c); }
void AdafruitGFX_Pico::drawFastHLine(int16_t x,int16_t y,int16_t w,uint16_t c) { fillRect(x,y,w,1,c); }
void AdafruitGFX_Pico::drawRect(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t c) {
    if (w <= 0 || h <= 0) return;
    fillRect(x,y,w,1,c);fillRect(x,y+h-1,w,1,c);
    fillRect(x,y,1,h,c);fillRect(x+w-1,y,1,h,c);
}
// Traza líneas; optimiza las horizontales/verticales y calcula los píxeles de diagonales.
void AdafruitGFX_Pico::drawLine(int16_t x0,int16_t y0,int16_t x1,int16_t y1,uint16_t c) {
    if(y0==y1){fillRect(std::min(x0,x1),y0,std::abs(x1-x0)+1,1,c);return;}
    if(x0==x1){fillRect(x0,std::min(y0,y1),1,std::abs(y1-y0)+1,c);return;}
    int dx=std::abs(x1-x0),sx=x0<x1?1:-1;
    int dy=-std::abs(y1-y0),sy=y0<y1?1:-1;
    int error=dx+dy;
    while(true){
        drawPixel(x0,y0,c);
        if(x0==x1 && y0==y1)break;
        const int doble=2*error;
        if(doble>=dy){error+=dy;x0+=sx;}
        if(doble<=dx){error+=dx;y0+=sy;}
    }
}
void AdafruitGFX_Pico::circuloPuntos(int16_t cx,int16_t cy,int16_t x,int16_t y,uint16_t c) {
    drawPixel(cx+x,cy+y,c);drawPixel(cx-x,cy+y,c);drawPixel(cx+x,cy-y,c);drawPixel(cx-x,cy-y,c);
    drawPixel(cx+y,cy+x,c);drawPixel(cx-y,cy+x,c);drawPixel(cx+y,cy-x,c);drawPixel(cx-y,cy-x,c);
}
// Dibuja círculos con simetría; se usan en cabezas y ruedas.
void AdafruitGFX_Pico::drawCircle(int16_t cx,int16_t cy,int16_t r,uint16_t c) {
    if(r<0)return;
    int x=0,y=r,d=1-r;
    while(x<=y){circuloPuntos(cx,cy,x,y,c);if(d<0)d+=2*x+3;else{d+=2*(x-y)+5;--y;}++x;}
}
// Rellena círculos para detalles sólidos sin emplear un bitmap.
void AdafruitGFX_Pico::fillCircle(int16_t cx,int16_t cy,int16_t r,uint16_t c) {
    if(r<0)return;
    int x=0,y=r,d=1-r;
    while(x<=y){
        fillRect(cx-x,cy+y,2*x+1,1,c);fillRect(cx-x,cy-y,2*x+1,1,c);
        fillRect(cx-y,cy+x,2*y+1,1,c);fillRect(cx-y,cy-x,2*y+1,1,c);
        if(d<0)d+=2*x+3;else{d+=2*(x-y)+5;--y;}++x;
    }
}
// Dibuja marcos con esquinas redondeadas como parte de la estética retro.
void AdafruitGFX_Pico::drawRoundRect(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c) {
    if(w<=0||h<=0)return;
    r=std::max<int>(0,std::min<int>(r,std::min(w,h)/2));
    drawFastHLine(x+r,y,w-2*r,c);drawFastHLine(x+r,y+h-1,w-2*r,c);
    fillRect(x,y+r,1,h-2*r,c);fillRect(x+w-1,y+r,1,h-2*r,c);
    for(int i=0;i<=r;i++){
        // Esquina usando circulo discretizado con radio r.
        int j=0;
        while(j<r && i*i+j*j<r*r)++j;
        drawPixel(x+r-j,y+r-i,c);drawPixel(x+w-r-1+j,y+r-i,c);
        drawPixel(x+r-j,y+h-r-1+i,c);drawPixel(x+w-r-1+j,y+h-r-1+i,c);
    }
}
// Rellena las carrocerías de los vehículos usando geometría.
void AdafruitGFX_Pico::fillRoundRect(int16_t x,int16_t y,int16_t w,int16_t h,int16_t r,uint16_t c) {
    if(w<=0||h<=0)return;
    r=std::max<int>(0,std::min<int>(r,std::min(w,h)/2));
    fillRect(x+r,y,w-2*r,h,c);
    for(int i=0;i<r;i++){
        int j=0;
        while(j<r && i*i+j*j<r*r)++j;
        int xx=r-j;
        fillRect(x+xx,y+r-i,w-2*xx,1,c);
        fillRect(x+xx,y+h-r-1+i,w-2*xx,1,c);
    }
}
// Rasteriza triángulos mediante franjas horizontales, útil para flechas e indicadores.
void AdafruitGFX_Pico::fillTriangle(int16_t x0,int16_t y0,int16_t x1,int16_t y1,
                                   int16_t x2,int16_t y2,uint16_t c) {
    const int minY=std::min<int>({y0,y1,y2}),maxY=std::max<int>({y0,y1,y2});
    const int xs[]={x0,x1,x2},ys[]={y0,y1,y2};
    for(int y=minY;y<=maxY;++y){
        int interceptos[6],num=0;
        for(int i=0;i<3;++i){
            const int j=(i+1)%3;
            if(ys[i]==ys[j])continue;
            if(y>=std::min(ys[i],ys[j]) && y<=std::max(ys[i],ys[j]))
                interceptos[num++]=xs[i]+(y-ys[i])*(xs[j]-xs[i])/(ys[j]-ys[i]);
        }
        if(num>=2){
            int a=interceptos[0],b=interceptos[1];
            for(int k=2;k<num;k++){a=std::min(a,interceptos[k]);b=std::max(b,interceptos[k]);}
            fillRect(a,y,b-a+1,1,c);
        }
    }
}

// Convierte la matriz 5x7 de una letra en pequeños rectángulos de píxeles.
void AdafruitGFX_Pico::dibujarCaracter(uint8_t codigo,int16_t x,int16_t y){
    if(x>=ancho_||y>=alto_||x+6*tam_<=0||y+8*tam_<=0)return;
    const uint8_t *bits=FUENTE_RETRO[codigo];
    for(int row=0;row<7;++row){
        int run=-1;
        for(int col=0;col<=5;++col){
            const bool on=(col<5) && ((bits[row]>>(4-col))&1);
            if(on && run<0)run=col;
            if(!on && run>=0){fillRect(x+run*tam_,y+row*tam_,(col-run)*tam_,tam_,textoColor_);run=-1;}
        }
    }
}
// Escribe el texto carácter a carácter; también respeta salto de línea y salto automático.
std::size_t AdafruitGFX_Pico::print(const char *txt){
    if(!txt)return 0;
    std::size_t count=0;
    for(const uint8_t *p=reinterpret_cast<const uint8_t*>(txt);*p;++p){
        if(*p=='\n'){cursorX_=0;cursorY_+=8*tam_;continue;}
        if(wrap_ && cursorX_+6*tam_>ancho_){cursorX_=0;cursorY_+=8*tam_;}
        dibujarCaracter(*p,cursorX_,cursorY_);cursorX_+=6*tam_;++count;
    }
    return count;
}
std::size_t AdafruitGFX_Pico::print(int valor){char buffer[20];std::snprintf(buffer,sizeof(buffer),"%d",valor);return print(buffer);}
// Calcula el espacio de un texto para poder centrarlo en la pantalla.
void AdafruitGFX_Pico::getTextBounds(const char *txt,int16_t x,int16_t y,
                                     int16_t *x1,int16_t *y1,uint16_t *w,uint16_t *h) const {
    if(x1)*x1=x;
    if(y1)*y1=y;
    uint16_t cantidad=0;
    if(txt)for(const char *p=txt;*p;p++)if(*p!='\n')++cantidad;
    if(w)*w=cantidad*6*tam_;
    if(h)*h=8*tam_;
}
