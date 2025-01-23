cmake_minimum_required(VERSION 3.28)
project(wch-riscv-hal-second)
add_subdirectory(src)

#add_executable(main_app src/main.cpp)
#add_dependencies(main_app wch-riscv-hal-first-pass)
#target_link_libraries(main_app PRIVATE wch-riscv-hal-first-pass)  # Example of linking src target

#SET(OUTPUT_BINARY wch-riscv-hal-first-pass)
#
#include_directories(./)
#
#set (headers
#        peripheral/ch32v00x/gpio.h
#        peripheral/ch32v00x/rcc.h
#        peripheral/ch32v00x/timer.h
#        peripheral/ch32v00x/usart.h
#        system/CH32V003xxxx/soc_template.h
#        system/soc_types.h
#        user_src/firmware_build_info.h
#        user_src/system.h
#        utils/delay.h
#        utils/firmware_build_info.h
#        utils/literals/all.h
#        utils/literals/delay.h
#        utils/literals/helper_templates.h
#        utils/literals/timer.h
#        utils/literals/usart.h
#        system/linker_utils.h
#        system/memory_map/peripheral_addresses.h
#        system/memory_map/concepts.h
#        system/startup/soc_init.h
#)
#
#set (sources
#        user_src/main.cpp
#)
#
## Force to rebuild this file on each compilation, making sure the date and time info in the file are always up to date
#set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")
#
## Generate list file output for each source file
## https://stackoverflow.com/questions/6666805/what-does-each-column-of-objdumps-symbol-table-mean
#foreach(source_file IN LISTS sources)
#    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.lst")
#endforeach()
#
#
#include(CmakeCommon.cmake)
