cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(mi_proyecto C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(mi_proyecto
    main.cpp
    Boton.cpp
    Perilla.cpp
    Mapa.cpp
    Juego.cpp
    Pantalla.cpp
)

# hardware_i2c = la pantalla, hardware_adc = las perillas
target_link_libraries(mi_proyecto
    pico_stdlib
    hardware_i2c
    hardware_adc
)

pico_add_extra_outputs(mi_proyecto)