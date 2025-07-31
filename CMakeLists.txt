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

#set(TARGET_ARCH_AND_ABI "-march=rv32ic -mabi=ilp32")
#For WCH toolchain and CHV32V003
# assembly was using gcc.exe (not g++ like the rest) for ASM compilation on few files, but then no extra CMAKE_ASM_FLAGS
# were set so that was default to rv32i and then switching to rv32e was causing mismatch between object files and
# linker was failing
# gcc 8?
#set(TARGET_ARCH_AND_ABI "-march=rv32ecxw -mabi=ilp32e")
# gcc 12
#set(TARGET_ARCH_AND_ABI "-march=rv32ec_xw -mabi=ilp32e")


# without XW the assembler gdb and tools work better as they recognize instructions
set(TARGET_ARCH_AND_ABI "-march=rv32ec_zicsr_zbb -mabi=ilp32e")

# WCH GCC-12 toolchain multilibs riscv-wch-elf-gcc.exe --print-multi-lib
#rv32e/ilp32e;@march=rv32e@mabi=ilp32e
#rv32ec/ilp32e;@march=rv32ec@mabi=ilp32e
#rv32ec_xw/ilp32e;@march=rv32ec_xw@mabi=ilp32e
#rv32emac/ilp32e;@march=rv32emac@mabi=ilp32e
#rv32ec_zmmul/ilp32e;@march=rv32ec_zmmul@mabi=ilp32e
#rv32ec_zmmul_xw/ilp32e;@march=rv32ec_zmmul_xw@mabi=ilp32e
#rv32i/ilp32;@march=rv32i@mabi=ilp32
#rv32ia/ilp32;@march=rv32ia@mabi=ilp32
#rv32iac/ilp32;@march=rv32iac@mabi=ilp32
#rv32iaf/ilp32f;@march=rv32iaf@mabi=ilp32f
#rv32iafc/ilp32f;@march=rv32iafc@mabi=ilp32f
#rv32iafd_zicsr/ilp32d;@march=rv32iafd_zicsr@mabi=ilp32d
#rv32iafdc_zicsr/ilp32d;@march=rv32iafdc_zicsr@mabi=ilp32d
#rv32ic/ilp32;@march=rv32ic@mabi=ilp32
#rv32if/ilp32f;@march=rv32if@mabi=ilp32f
#rv32ifc/ilp32f;@march=rv32ifc@mabi=ilp32f
#rv32ifd_zicsr/ilp32d;@march=rv32ifd_zicsr@mabi=ilp32d
#rv32ifdc_zicsr/ilp32d;@march=rv32ifdc_zicsr@mabi=ilp32d
#rv32im/ilp32;@march=rv32im@mabi=ilp32
#rv32ima/ilp32;@march=rv32ima@mabi=ilp32
#rv32imc_zba_zbb_zbc_zbs/ilp32;@march=rv32imc_zba_zbb_zbc_zbs@mabi=ilp32
#rv32imac_zba_zbb_zbc_zbs/ilp32;@march=rv32imac_zba_zbb_zbc_zbs@mabi=ilp32
#rv32imac_xw/ilp32;@march=rv32imac_xw@mabi=ilp32
#rv32imc_zba_zbb_zbc_zbs_xw/ilp32;@march=rv32imc_zba_zbb_zbc_zbs_xw@mabi=ilp32
#rv32imac_zba_zbb_zbc_zbs_xw/ilp32;@march=rv32imac_zba_zbb_zbc_zbs_xw@mabi=ilp32
#rv32imaf/ilp32f;@march=rv32imaf@mabi=ilp32f
#rv32imafc/ilp32f;@march=rv32imafc@mabi=ilp32f
#rv32imafc_zba_zbb_zbc_zbs/ilp32f;@march=rv32imafc_zba_zbb_zbc_zbs@mabi=ilp32f
#rv32imafc_xw/ilp32f;@march=rv32imafc_xw@mabi=ilp32f
#rv32imafc_zba_zbb_zbc_zbs_xw/ilp32f;@march=rv32imafc_zba_zbb_zbc_zbs_xw@mabi=ilp32f
#rv32imafd_zicsr/ilp32d;@march=rv32imafd_zicsr@mabi=ilp32d
#rv32imafdc_zicsr/ilp32d;@march=rv32imafdc_zicsr@mabi=ilp32d
#rv32imc/ilp32;@march=rv32imc@mabi=ilp32
#rv32imf/ilp32f;@march=rv32imf@mabi=ilp32f
#rv32imfc/ilp32f;@march=rv32imfc@mabi=ilp32f
#rv32imfd_zicsr/ilp32d;@march=rv32imfd_zicsr@mabi=ilp32d
#rv32imfdc_zicsr/ilp32d;@march=rv32imfdc_zicsr@mabi=ilp32d
#rv64im/lp64;@march=rv64im@mabi=lp64
#rv64ima/lp64;@march=rv64ima@mabi=lp64
#rv64imac/lp64;@march=rv64imac@mabi=lp64
#rv64imaf/lp64f;@march=rv64imaf@mabi=lp64f
#rv64imafc/lp64f;@march=rv64imafc@mabi=lp64f


# xpack toolchain xpack-riscv-none-elf-gcc-14.2.0-3\bin>riscv-none-elf-gcc --print-multi-lib
#rv32e/ilp32e;@march=rv32e@mabi=ilp32e
#rv32ec/ilp32e;@march=rv32ec@mabi=ilp32e
#rv32ea/ilp32e;@march=rv32ea@mabi=ilp32e
#rv32em/ilp32e;@march=rv32em@mabi=ilp32e
#rv32eac/ilp32e;@march=rv32eac@mabi=ilp32e
#rv32emac/ilp32e;@march=rv32emac@mabi=ilp32e
#rv32i/ilp32;@march=rv32i@mabi=ilp32
#rv32ia/ilp32;@march=rv32ia@mabi=ilp32
#rv32im/ilp32;@march=rv32im@mabi=ilp32
#rv32imc/ilp32;@march=rv32imc@mabi=ilp32
#rv32if_zicsr/ilp32f;@march=rv32if_zicsr@mabi=ilp32f
#rv32ifd_zicsr/ilp32d;@march=rv32ifd_zicsr@mabi=ilp32d
#rv32iaf_zicsr/ilp32f;@march=rv32iaf_zicsr@mabi=ilp32f
#rv32iafd_zicsr/ilp32d;@march=rv32iafd_zicsr@mabi=ilp32d
#rv32imf_zicsr/ilp32f;@march=rv32imf_zicsr@mabi=ilp32f
#rv32imfd_zicsr/ilp32d;@march=rv32imfd_zicsr@mabi=ilp32d
#rv32iac/ilp32;@march=rv32iac@mabi=ilp32
#rv32imafc_zicsr/ilp32f;@march=rv32imafc_zicsr@mabi=ilp32f
#rv32imafdc_zicsr/ilp32d;@march=rv32imafdc_zicsr@mabi=ilp32d
#rv64i/lp64;@march=rv64i@mabi=lp64
#rv64ia/lp64;@march=rv64ia@mabi=lp64
#rv64im/lp64;@march=rv64im@mabi=lp64
#rv64if_zicsr/lp64f;@march=rv64if_zicsr@mabi=lp64f
#rv64ifd_zicsr/lp64d;@march=rv64ifd_zicsr@mabi=lp64d
#rv64iaf_zicsr/lp64f;@march=rv64iaf_zicsr@mabi=lp64f
#rv64iafd_zicsr/lp64d;@march=rv64iafd_zicsr@mabi=lp64d
#rv64imf_zicsr/lp64f;@march=rv64imf_zicsr@mabi=lp64f
#rv64iac/lp64;@march=rv64iac@mabi=lp64
#rv64imac/lp64;@march=rv64imac@mabi=lp64
#rv64imafc_zicsr/lp64f;@march=rv64imafc_zicsr@mabi=lp64f
#rv64imafdc_zicsr/lp64d;@march=rv64imafdc_zicsr@mabi=lp64d



#set(CMAKE_C_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
#set(CMAKE_CXX_FLAGS "-march=rv32i -mabi=ilp32 -v -msave-restore -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0 -fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti")
#_GLIBCXX_USE_CXX11_ABI=1 or _GLIBCXX_USE_CXX11_ABI=2

# -msave-restore vs  -mno-save-restore
# https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html
#Do or don’t use smaller but slower prologue and epilogue code that uses library function calls. The default is to use fast inline prologues and epilogues.
#set(MY_COMPILER_FLAGS_COMMON "-Wunused -Wuninitialized -msave-restore -msmall-data-limit=0 -fmax-errors=10 -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
set(MY_COMPILER_FLAGS_COMMON "-Wunused -Wuninitialized -mno-save-restore -msmall-data-limit=0 -fmax-errors=10 -fno-common -fdata-sections -ffunction-sections -fsigned-char -fmessage-length=0")
set(MY_COMPILER_FLAGS_CPP    "-fno-exceptions -fno-threadsafe-statics -fno-use-cxa-atexit -fno-rtti -Wabi-tag -D_GLIBCXX_USE_CXX11_ABI=2")
set(MY_LINKER_FLAGS          "-nostartfiles -flto -mrelax -nostdlib -fno-builtin-memcpy -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs")

set(CMAKE_C_FLAGS   "${TARGET_ARCH_AND_ABI} ${MY_COMPILER_FLAGS_COMMON} -mrelax -flto -fno-builtin -fno-builtin-memcpy -ffreestanding") # C compiler used only for soc_init.c disabling builtin so memcpy is not used
set(CMAKE_CXX_FLAGS "${TARGET_ARCH_AND_ABI} ${MY_COMPILER_FLAGS_COMMON} ${MY_COMPILER_FLAGS_CPP}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${TARGET_ARCH_AND_ABI}")


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
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/afio.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/dma_channels.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/afio/pcfr1.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/ahbpcenr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/apb1pcenr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/apb1prstr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/apb2pcenr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/apb2prstr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/cfgr0.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/ctlr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/intr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/field/rcc/rstsckr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/rcc.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/timer.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/ch32v00x/usart.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/dma.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/dma/field/cfgr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/dma/field/intfcr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/dma/field/intfr.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/gpio_port.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/gpio_pin.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/common/gpio_common.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/concepts.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/obtain_reg_metadata.h
        ${CMAKE_SOURCE_DIR}/src/peripheral/util.h
        ${CMAKE_SOURCE_DIR}/src/system/CH32V003xxxx/soc_template.h
        ${CMAKE_SOURCE_DIR}/src/system/ch32v003/clocks.h
        ${CMAKE_SOURCE_DIR}/src/system/gpio/ops_fusion.h
        ${CMAKE_SOURCE_DIR}/src/system/gpio/op.h
        ${CMAKE_SOURCE_DIR}/src/system/gpio/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/linker_utils.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/memory_map/peripheral_addresses.h
        ${CMAKE_SOURCE_DIR}/src/system/register/access_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/register/access_primitives.h
        ${CMAKE_SOURCE_DIR}/src/system/register/combine.h
        ${CMAKE_SOURCE_DIR}/src/system/register/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/register/field_access_privilege.h
        ${CMAKE_SOURCE_DIR}/src/system/register/util.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/concepts.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_access_ct.h
        ${CMAKE_SOURCE_DIR}/src/system/riscv/csr_access_primitives.h
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
        ${CMAKE_SOURCE_DIR}/src/user_src/platform_configuration/system.h
        ${CMAKE_SOURCE_DIR}/src/user_src/platform_configuration/system_clock.h
        ${CMAKE_SOURCE_DIR}/src/user_src/platform_configuration/system_defines.h
        ${CMAKE_SOURCE_DIR}/src/user_src/platform_configuration/system_irq.h
        ${CMAKE_SOURCE_DIR}/src/user_src/platform_configuration/system_soc.h
        ${CMAKE_SOURCE_DIR}/src/utils/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/firmware_build_info.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/all.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/delay.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/helper_templates.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/timer.h
        ${CMAKE_SOURCE_DIR}/src/utils/literals/usart.h
)


set (sources
        ${CMAKE_SOURCE_DIR}/src/system/ch32v003/interrupts.cpp
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
