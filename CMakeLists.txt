cmake_minimum_required(VERSION 3.28)

#set(SHARED_NAME wch-riscv-hal)
#set(FIRST_PASS_BINARY ${SHARED_NAME}-first-pass)
#set(SECOND_PASS_BINARY ${SHARED_NAME}-second-pass)
project(wch-riscv-hal-second-pass)

set(CMAKE_CXX_STANDARD 23)
set(BUSYBOX_LOCATION "c:/progs/busybox.exe")


set(LINKER_SCRIPT_TEMPLATE ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-universal-boot0.tpl.ld)
set(LINKER_SCRIPT_FINAL ${CMAKE_BINARY_DIR}/linker/WCH32Vxxx-for-current-soc-boot0.ld)


set (headers
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/gpio.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/rcc.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/timer.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/usart.h
        ${CMAKE_SOURCE_DIR}/src/system/CH32V003xxxx/soc_template.h
        ${CMAKE_SOURCE_DIR}/src/system/soc_types.h
        ${CMAKE_SOURCE_DIR}/src/user_src/firmware_build_info.h
        ${CMAKE_SOURCE_DIR}/src/user_src/system.h
        ${CMAKE_SOURCE_DIR}/src/utils/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/firmware_build_info.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/all.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/helper_templates.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/timer.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/usart.h
        ${CMAKE_SOURCE_DIR}/src/system/linker_utils.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/peripheral_addresses.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/startup/soc_init.h
)


set (sources
        ${CMAKE_SOURCE_DIR}/src/user_src/main.cpp
)


#add_subdirectory(./src)


execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo "hiiiiiiiiiiiiiiiiiffffffffffffffffffffffffffffff"
)


#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt" FIRST_PASS_DATA_SIZE)
#message("FIRST_PASS_DATA_SIZE: ${FIRST_PASS_DATA_SIZE}")
#
#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_bss.txt" FIRST_PASS_BSS_SIZE)
#message("FIRST_PASS_BSS_SIZE: ${FIRST_PASS_BSS_SIZE}")

include_directories(./src)

#target_include_directories(${PROJECT_NAME} PRIVATE ./src)

add_executable(${PROJECT_NAME} ${sources} ${headers})

#add_dependencies(${PROJECT_NAME} wch-riscv-hal-first-pass)


#add_executable(main_app src/main.cpp)
#add_dependencies(main_app wch-riscv-hal-first-pass)
#target_link_libraries(main_app PRIVATE wch-riscv-hal-first-pass)  # Example of linking src target

#SET(OUTPUT_BINARY wch-riscv-hal-first-pass)
#
#
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
