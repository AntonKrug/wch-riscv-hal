# https://developer.arm.com/documentation/100066/0611/assembling-assembly-code/preprocessing-assembly-code

cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

#set(SHARED_NAME wch-riscv-hal)
#set(FIRST_PASS_BINARY ${SHARED_NAME}-first-pass)
#set(SECOND_PASS_BINARY ${SHARED_NAME}-second-pass)
project(wch-riscv-hal C CXX ASM)
set(BINARY_FIRST_PASS  ${PROJECT_NAME}-pass-1.elf)
set(BINARY_SECOND_PASS ${PROJECT_NAME}-pass-2.elf)

if (CMAKE_HOST_WIN32)
    # detect the host win32 and not the target win32:
    # https://stackoverflow.com/a/70382484/4535300
    # the variable CMAKE_EXECUTABLE_SUFFIX will get
    # overriden when calling project, so use the
    # CMAKE_HOST_EXECUTABLE_SUFFIX instead
    set(CMAKE_HOST_EXECUTABLE_SUFFIX ".exe")
else()
    set(CMAKE_HOST_EXECUTABLE_SUFFIX "")
endif()

#message(FATAL_ERROR "suffix:${CMAKE_HOST_EXECUTABLE_SUFFIX}")



#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

#set(TOOLCHAIN_PATH "C:/progs/wch-gcc-8")
set(TOOLCHAIN_PATH "C:/progs/wch-gcc-12")
#set(TOOLCHAIN_PATH "C:/progs/xpack-riscv-none-elf-gcc-14.2.0-3")
#set(TOOLCHAIN_PATH "C:/progs/xpack-riscv-none-elf-gcc-13.3.0-2")

#message(FATAL_ERROR "suffix:${CMAKE_HOST_EXECUTABLE_SUFFIX}")


#https://cmake.org/cmake/help/latest/variable/CMAKE_PROGRAM_PATH.html
#https://gitlab.kitware.com/cmake/cmake/-/issues/21736
set(TOOLCHAIN_PATH_BIN "${TOOLCHAIN_PATH}/bin")
#set(CMAKE_PROGRAM_PATH "${TOOLCHAIN_PATH_BIN}" "${CMAKE_PROGRAM_PATH}")
#set(MY_TOOLCHAIN_CROSS_PREFIX "riscv-none-elf-")
#set(MY_TOOLCHAIN_CROSS_PREFIX "riscv-none-embed-")
set(MY_TOOLCHAIN_CROSS_PREFIX "riscv-wch-elf-")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}gcc${CMAKE_HOST_EXECUTABLE_SUFFIX}")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}g++${CMAKE_HOST_EXECUTABLE_SUFFIX}")
set(CMAKE_SIZE         "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}size${CMAKE_HOST_EXECUTABLE_SUFFIX}")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}objcopy${CMAKE_HOST_EXECUTABLE_SUFFIX}")
set(CMAKE_OBJDUMP      "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}objdump${CMAKE_HOST_EXECUTABLE_SUFFIX}")
set(CMAKE_STRIP        "${TOOLCHAIN_PATH_BIN}/${MY_TOOLCHAIN_CROSS_PREFIX}strip${CMAKE_HOST_EXECUTABLE_SUFFIX}")

#https://cmake.org/cmake/help/latest/command/find_program.html
#find_program(CMAKE_SIZE    size    PATHS "${TOOLCHAIN_PATH_BIN}" NO_CACHE NO_DEFAULT_PATH)
#find_program(CMAKE_OBJCOPY objcopy PATHS "${TOOLCHAIN_PATH_BIN}" NO_CACHE NO_DEFAULT_PATH)
#find_program(CMAKE_OBJDUMP objdump PATHS "${TOOLCHAIN_PATH_BIN}" NO_CACHE NO_DEFAULT_PATH)
#find_program(CMAKE_NM      nm      PATHS "${TOOLCHAIN_PATH_BIN}" NO_CACHE NO_DEFAULT_PATH)
##find_program(CMAKE_NM ${MY_TOOLCHAIN_CROSS_PREFIX}size${CMAKE_HOST_EXECUTABLE_SUFFIX})
#message(FATAL_ERROR "suffix:${CMAKE_HOST_EXECUTABLE_SUFFIX} ${CMAKE_PROGRAM_PATH} ${CMAKE_SIZE} ${CMAKE_OBJCOPY} ${CMAKE_OBJDUMP} ${CMAKE_NM}")
#
#if(NOT CMAKE_NM)
#    message(FATAL_ERROR "nm not found!")
#endif()

#set(CMAKE_C_COMPILER ${TOOLCHAIN}/${MY_CROSS_PREFIX}gcc${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_CXX_COMPILER ${TOOLCHAIN}/${MY_CROSS_PREFIX}g++${CMAKE_EXECUTABLE_SUFFIX})
#SET(CMAKE_LINKER ${TOOLCHAIN}/${MY_CROSS_PREFIX}ld${CMAKE_EXECUTABLE_SUFFIX})
#set(CMAKE_AR ${TOOLCHAIN}/${MY_CROSS_PREFIX}ar${CMAKE_EXECUTABLE_SUFFIX})

#set(CMAKE_C_COMPILER_ABI_INFO 1)
#set(CMAKE_CXX_COMPILER_ABI_INFO 1)

set(TARGET_ARCH_AND_ABI "-march=rv32ic -mabi=ilp32")
#For WCH toolchain and CHV32V003
#set(TARGET_ARCH_AND_ABI "-march=rv32ecxw -mabi=ilp32e")


#set(CMAKE_C_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
#set(CMAKE_CXX_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0 -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti")
#_GLIBCXX_USE_CXX11_ABI=1 or _GLIBCXX_USE_CXX11_ABI=2

set(MY_COMPILER_FLAGS_COMMON "-Wunused -Wuninitialized -msave-restore -msmall-data-limit=0 -fmax-errors=10 -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
set(MY_COMPILER_FLAGS_CPP    "-fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti -Wabi-tag -D_GLIBCXX_USE_CXX11_ABI=2")
set(MY_LINKER_FLAGS          "-nostartfiles -flto -mrelax -nostdlib -fno-builtin-memcpy -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs")

set(CMAKE_C_FLAGS   "${TARGET_ARCH_AND_ABI} ${MY_COMPILER_FLAGS_COMMON} -mrelax -flto -fno-builtin -fno-builtin-memcpy -ffreestanding") # C compiler used only for soc_init.c disabling builtin so memcpy is not used
set(CMAKE_CXX_FLAGS "${TARGET_ARCH_AND_ABI} ${MY_COMPILER_FLAGS_COMMON} ${MY_COMPILER_FLAGS_CPP}")


#@	riscv-none-embed-objcopy -O ihex "CH32V003F4P6-2.elf" "CH32V003F4P6-2.hex"
#CH32V003F4P6-2.lst: CH32V003F4P6-2.elf
#@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "CH32V003F4P6-2.elf" > "CH32V003F4P6-2.lst"
#CH32V003F4P6-2.siz: CH32V003F4P6-2.elf
#riscv-none-embed-size --format=sysv --totals "CH32V003F4P6-2.elf"

#TARGET_C_FLAGS="-O3 -march=rv32g -g"

#https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
#set(CMAKE_EXE_LINKER_FLAGS "-march=rv32i -mabi=ilp32 -nostartfiles -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs")
#set(CMAKE_EXE_LINKER_FLAGS "-march=rv32ic -mabi=ilp32 -nostartfiles -nostdlib -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs")
set(CMAKE_EXE_LINKER_FLAGS "${TARGET_ARCH_AND_ABI} ${MY_LINKER_FLAGS}")


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
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(BUSYBOX_LOCATION "c:/progs/busybox.exe")


set(LINKER_SCRIPT_TEMPLATE ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-universal-boot0.tpl.ld)
set(LINKER_SCRIPT_FINAL ${CMAKE_BINARY_DIR}/linker/WCH32Vxxx-for-current-soc-boot0.ld)


set (headers
        ${CMAKE_SOURCE_DIR}/src/peripheral/base_addr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/gpio.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/rcc.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/cfgr0.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/ctlr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/intr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/timer.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/usart.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/concept.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/register_field_to_register.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/util.h
        ${CMAKE_SOURCE_DIR}/src/system/CH32V003xxxx/soc_template.h
        ${CMAKE_SOURCE_DIR}/src/system/ch32v003/clocks.h
        ${CMAKE_SOURCE_DIR}/src/system/linker_utils.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/peripheral_addresses.h
        ${CMAKE_SOURCE_DIR}/src/system/register/concept.h
        ${CMAKE_SOURCE_DIR}/src/system/register/util.h
        ${CMAKE_SOURCE_DIR}/src/system/register/field_access_privilege.h
        ${CMAKE_SOURCE_DIR}/src/system/register/combine.h
        ${CMAKE_SOURCE_DIR}/src/system/register/access_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/register/access_primitives_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_access_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_access_primitives_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_field_to_csr.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_register/intsyscr.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_register/mstatus.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_register/mtvec.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_util.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csrs_qingke.h
        ${CMAKE_SOURCE_DIR}/src/system/soc_types.h
        ${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h
        ${CMAKE_SOURCE_DIR}/src/user_src/firmware_build_info.h
        ${CMAKE_SOURCE_DIR}/src/user_src/system.h
        ${CMAKE_SOURCE_DIR}/src/user_src/system_clock.h
        ${CMAKE_SOURCE_DIR}/src/user_src/system_defines.h
        ${CMAKE_SOURCE_DIR}/src/user_src/system_soc.h
        ${CMAKE_SOURCE_DIR}/src/utils/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/firmware_build_info.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/all.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/helper_templates.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/timer.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/usart.h
)


set (sources
        ${CMAKE_SOURCE_DIR}/src/user_src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/system/startup/soc_init.cpp
)

set (assemblies
        ${CMAKE_SOURCE_DIR}/src/system/startup/entry_after_power_up.s
)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(./src)


#execute_process(
#        COMMAND ${CMAKE_COMMAND} -E echo "hifffffffffff"
#)


#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt" FIRST_PASS_DATA_SIZE)
#message("FIRST_PASS_DATA_SIZE: ${FIRST_PASS_DATA_SIZE}")
#
#file(READ "${CMAKE_BINARY_DIR}/first_pass_symbols_bss.txt" FIRST_PASS_BSS_SIZE)
#message("FIRST_PASS_BSS_SIZE: ${FIRST_PASS_BSS_SIZE}")


#target_include_directories(${PROJECT_NAME} PRIVATE ./src)

add_executable(${BINARY_SECOND_PASS} ${sources} ${headers})

add_dependencies(${BINARY_SECOND_PASS} ${BINARY_FIRST_PASS})
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
