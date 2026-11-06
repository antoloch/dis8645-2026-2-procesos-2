# DORE 8 - Version modularizada para la Raspberry Pi Pico 2 W
# Abrir la carpeta Codigos en Visual Studio Code con Pico SDK instalado.
cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico2_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
project(Dore8 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(Dore8
    main.cpp
    AdafruitGFX_Pico.cpp
    PantallaUI.cpp
    Perilla.cpp
    Boton2.cpp
)

target_include_directories(Dore8 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Dore8
    pico_stdlib
    hardware_spi
    hardware_adc
    hardware_gpio
)
pico_enable_stdio_usb(Dore8 1)
pico_enable_stdio_uart(Dore8 0)
pico_add_extra_outputs(Dore8)
