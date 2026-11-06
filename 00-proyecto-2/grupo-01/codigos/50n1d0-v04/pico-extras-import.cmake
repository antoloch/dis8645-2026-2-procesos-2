if (DEFINED ENV{PICO_EXTRAS_PATH} AND (NOT PICO_EXTRAS_PATH))
    set(PICO_EXTRAS_PATH $ENV{PICO_EXTRAS_PATH})
endif ()

if (NOT PICO_EXTRAS_PATH)
    if (PICO_EXTRAS_FETCH_FROM_GIT)
        include(FetchContent)
        if (POLICY CMP0169)
            cmake_policy(SET CMP0169 OLD)
        endif ()
        FetchContent_Declare(
                pico_extras
                GIT_REPOSITORY https://github.com/raspberrypi/pico-extras
                GIT_TAG sdk-2.3.1
        )
        FetchContent_Populate(pico_extras)
        set(PICO_EXTRAS_PATH ${pico_extras_SOURCE_DIR})
    elseif (PICO_SDK_PATH AND EXISTS "${PICO_SDK_PATH}/../pico-extras")
        set(PICO_EXTRAS_PATH ${PICO_SDK_PATH}/../pico-extras)
    else ()
        message(FATAL_ERROR
                "PICO EXTRAS location was not specified. Set PICO_EXTRAS_PATH or enable PICO_EXTRAS_FETCH_FROM_GIT.")
    endif ()
endif ()

get_filename_component(PICO_EXTRAS_PATH "${PICO_EXTRAS_PATH}" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
if (NOT EXISTS "${PICO_EXTRAS_PATH}")
    message(FATAL_ERROR "Directory '${PICO_EXTRAS_PATH}' not found")
endif ()

set(PICO_EXTRAS_PATH "${PICO_EXTRAS_PATH}" CACHE PATH "Path to pico-extras" FORCE)
add_subdirectory("${PICO_EXTRAS_PATH}" pico_extras)
