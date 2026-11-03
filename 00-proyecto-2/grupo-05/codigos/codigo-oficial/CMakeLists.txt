cmake_minimum_required(VERSION 3.13)

# La placa y arquitectura son las mismas del montaje fisico.
set(PICO_BOARD pico2_w CACHE STRING "Raspberry Pi Pico 2 W")
set(PICO_PLATFORM rp2350-arm-s CACHE STRING "Arquitectura ARM")

# Usar el importador que ya viene con Pico SDK, sin copiarlo al proyecto.
if (NOT PICO_SDK_PATH AND DEFINED ENV{PICO_SDK_PATH})
    set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
endif()
if (NOT EXISTS "${PICO_SDK_PATH}/external/pico_sdk_import.cmake")
    message(FATAL_ERROR "Indicar la carpeta de Pico SDK con PICO_SDK_PATH.")
endif()
include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")

project(altiro_hero C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(altiro_hero
    main.cpp
    Boton.cpp
    Perilla.cpp
    Matriz.cpp
    Sonido.cpp
)

target_include_directories(altiro_hero PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(altiro_hero
    pico_stdlib
    hardware_adc
    hardware_pio
    hardware_clocks
    hardware_dma
    pico_multicore
)

pico_enable_stdio_usb(altiro_hero 1)
pico_enable_stdio_uart(altiro_hero 0)
pico_add_extra_outputs(altiro_hero)
