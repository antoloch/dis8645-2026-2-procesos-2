cmake_minimum_required(VERSION 3.13)

# placa: Raspberry Pi Pico 2 W
# esto va ANTES de incluir pico_sdk_import.cmake
set(PICO_BOARD pico2_w)

# importa el Pico SDK (este archivo ya viene en el proyecto de wokwi)
include(pico_sdk_import.cmake)

project(proyecto3 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# main.cpp reemplaza al main.c (borrar main.c del proyecto)
# ili934x.cpp es la biblioteca de Darren Horrocks (sin modificar);
# sus fuentes FreeMono son solo .h y las incluye Pantallita.cpp
add_executable(proyecto3
  main.cpp
  BotonGrupo1.cpp
  Lucecita.cpp
  Pantallita.cpp
  ili934x.cpp
)

# pico_stdlib trae printf y gpio; hardware_gpio por claridad
# hardware_spi es para la pantalla (spi0)
target_link_libraries(proyecto3 pico_stdlib hardware_gpio hardware_spi)

# monitor serial por UART0 (GP0 = TX, GP1 = RX), no por USB
pico_enable_stdio_uart(proyecto3 1)
pico_enable_stdio_usb(proyecto3 0)

pico_add_extra_outputs(proyecto3)
