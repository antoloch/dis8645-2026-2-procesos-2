# Compilamos con Pico SDK, como en clase. Aquí incluimos los archivos que necesita el ejemplo.
cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(trazo C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
# Código 7: agregamos Pantallita.cpp a la lista de archivos que se compilan.
add_executable(trazo main.cpp Perilla.cpp BotonGrupo1.cpp Pantallita.cpp ssd1306.c)
target_include_directories(trazo PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(trazo pico_stdlib hardware_adc hardware_i2c)
pico_enable_stdio_uart(trazo 1)
pico_enable_stdio_usb(trazo 0)
pico_add_extra_outputs(trazo)
