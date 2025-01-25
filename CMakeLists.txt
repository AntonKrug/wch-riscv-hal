cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

#set(SHARED_NAME wch-riscv-hal)
#set(FIRST_PASS_BINARY ${SHARED_NAME}-first-pass)
#set(SECOND_PASS_BINARY ${SHARED_NAME}-second-pass)
project(wch-riscv-hal)

if(WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")
elseif(UNIX)
    set(CMAKE_EXECUTABLE_SUFFIX "")
endif()


#set(CMAKE_C_COMPILER_WORKS)
#set(CMAKE_C_ABI_COMPILED)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

#set(CMAKE_SYSTEM_NAME Linux)

set(TOOLCHAIN C:/progs/xpack-riscv-none-elf-gcc-14.2.0-3/bin)
set(MY_CROSS_PREFIX "riscv-none-elf-")
set(CMAKE_NM ${TOOLCHAIN}/${MY_CROSS_PREFIX}nm${CMAKE_EXECUTABLE_SUFFIX})

#set(CMAKE_C_COMPILER ${TOOLCHAIN}/${MY_CROSS_PREFIX}gcc${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/${MY_CROSS_PREFIX}g++${CMAKE_EXECUTABLE_SUFFIX})
#SET(CMAKE_LINKER ${TOOLCHAIN}/${MY_CROSS_PREFIX}ld${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_AR ${TOOLCHAIN}/${MY_CROSS_PREFIX}ar${CMAKE_EXECUTABLE_SUFFIX})

#set(CMAKE_C_COMPILER_ABI_INFO 1)
#set(CMAKE_CXX_COMPILER_ABI_INFO 1)


#set(CMAKE_C_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
#set(CMAKE_CXX_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0 -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti")
#_GLIBCXX_USE_CXX11_ABI=1 or _GLIBCXX_USE_CXX11_ABI=2

set(CMAKE_C_FLAGS "-march=rv32i -mabi=ilp32 -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
set(CMAKE_CXX_FLAGS "-march=rv32i -mabi=ilp32 -msave-restore -Wabi-tag -D_GLIBCXX_USE_CXX11_ABI=1 -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0 -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti")

#TARGET_C_FLAGS="-O3 -march=rv32g -g"

#set(CMAKE_EXE_LINKER_FLAGS "-march=rv32i -mabi=ilp32 -nostartfiles -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs")
set(CMAKE_EXE_LINKER_FLAGS "-march=rv32i -mabi=ilp32 -nostartfiles -nostdlib -Xlinker --gc-sections --specs=nano.specs")


## Set the ABI and target architecture (for 32-bit RISC-V)
#set(CMAKE_C_FLAGS "-m32 -march=rv32imaf -mabi=ilp32")
#set(CMAKE_CXX_FLAGS "-m32 -march=rv32imaf -mabi=ilp32")
#
##TARGET_C_FLAGS="-O3 -march=rv32g -g"
#
#set(CMAKE_EXE_LINKER_FLAGS "")
#set(CMAKE_EXE_LINKER_FLAGS "-march=rv32imaf -mabi=ilp32")



# Optionally, set other toolchain flags depending on your requirements
# E.g., where the cross-compiling tools are located
#set(CMAKE_FIND_ROOT_PATH /path/to/riscv32-toolchain)


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
        ${CMAKE_SOURCE_DIR}/src/system/startup/startup_configuration.h
)


set (sources
        ${CMAKE_SOURCE_DIR}/src/user_src/main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(./src)


execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo "hiiiiiiiiiiiiiiiiiffffffffffffffffffffffffffffff"
)


#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt" FIRST_PASS_DATA_SIZE)
#message("FIRST_PASS_DATA_SIZE: ${FIRST_PASS_DATA_SIZE}")
#
#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_bss.txt" FIRST_PASS_BSS_SIZE)
#message("FIRST_PASS_BSS_SIZE: ${FIRST_PASS_BSS_SIZE}")


#target_include_directories(${PROJECT_NAME} PRIVATE ./src)

add_executable(${PROJECT_NAME}-second-pass ${sources} ${headers})

add_dependencies(${PROJECT_NAME}-second-pass ${PROJECT_NAME}-first-pass)
#add_dependencies(${PROJECT_NAME}-second-pass "${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt")


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
