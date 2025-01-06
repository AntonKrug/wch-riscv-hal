cmake_minimum_required(VERSION 3.28)
project(wch-riscv-hal)

set(CMAKE_CXX_STANDARD 23)

#include_directories(hdr)

set (headers
        peripheral/ch32v00x/gpio.h
        peripheral/ch32v00x/rcc.h
        peripheral/ch32v00x/timer.h
        peripheral/ch32v00x/usart.h
        system/ch32v003/soc.h
        system/soc_types.h
        utils/delay.h
        utils/literals/delay.h
        utils/literals/helper_templates.h
        utils/literals/timer.h
)

set (sources
        main.cpp
)

foreach(source_file IN LISTS sources)
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.lst")
endforeach()

add_executable(${PROJECT_NAME} ${sources} ${headers})
