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
add_executable(proyecto3
  main.cpp
  BotonGrupo1.cpp
  Lucecita.cpp
)

# pico_stdlib trae printf y gpio; hardware_gpio por claridad
target_link_libraries(proyecto3 pico_stdlib hardware_gpio)

# monitor serial por UART0 (GP0 = TX, GP1 = RX), no por USB
pico_enable_stdio_uart(proyecto3 1)
pico_enable_stdio_usb(proyecto3 0)

pico_add_extra_outputs(proyecto3)
