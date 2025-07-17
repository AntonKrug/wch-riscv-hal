# https://discourse.cmake.org/t/add-custom-command-with-target-dependency-doesnt-rebuild/7029/8
# https://stackoverflow.com/questions/2410976/how-can-i-define-a-string-literal-on-the-gcc-command-line

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_SYSTEM_NAME Generic)
project(wch-riscv-hal C CXX ASM)
#set(BINARY_FIRST_PASS ${PROJECT_NAME}-pass-1.elf)
set(BINARY_FIRST_PASS_HEX ${PROJECT_NAME}-pass-1.hex)
set(BINARY_FIRST_PASS_LST ${PROJECT_NAME}-pass-1.lst)
#include_directories(./)

# Force to rebuild this file on each compilation, making sure the date and time info in the file are always up to date
set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")


# Generate list file output for each source file
# https://stackoverflow.com/questions/6666805/what-does-each-column-of-objdumps-symbol-table-mean
#foreach(source_file IN LISTS sources)
##    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa, -x -C -adhln=${source_file}.lst")
#    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${source_file}.lst")
#endforeach()
#set_source_files_properties(${CMAKE_SOURCE_DIR}/src/user_src/main.cpp PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${source_file}.lst")



# Add assemblies to be compiled
foreach(asm_file IN LISTS assemblies)
    # https://developer.arm.com/documentation/101754/0623/armclang-Reference/armclang-Integrated-Assembler/Section-directives
    # https://stackoverflow.com/questions/15132185/mixing-c-and-assembly-sources-and-build-with-cmake
    set_property(SOURCE ${asm_file} APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
#    set_property(SOURCE foo.s APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
#    set_source_files_properties(${asm_file} PROPERTIES COMPILE_OPTIONS "-x assembler-with-cpp")
endforeach()


# The first startup function copy memory and prepare the stacks and register,
# these can't get optimized away with builtin functions like memcpy, so for this
# file the built-ins are disables,
# HOWEVER it doesn't work the way i want, so i had to disable it for the whole C compiler
# but this is the only C file anyway and rest is C++
# it is applied in the command line when i run cmake with -DCMAKE_VERBOSE_MAKEFILE=ON but with
# higher optimizations like -O2 it's ignored, i can do -O1 and manually add optimizations
#set_source_files_properties(${CMAKE_SOURCE_DIR}/src/system/startup/soc_init.c PROPERTIES COMPILE_FLAGS "-fno-builtin -ffreestanding")
# https://stackoverflow.com/questions/54281780/what-exactly-is-fno-builtin-doing-here
# https://gcc.gnu.org/onlinedocs/gcc/Attribute-Syntax.html
#set_source_files_properties(${CMAKE_SOURCE_DIR}/src/system/startup/soc_init.c PROPERTIES COMPILE_FLAGS "-fno-builtin -ffreestanding -fno-builtin-memcpy")


# Stating what binary will be produced
add_executable(${BINARY_FIRST_PASS} ${assemblies} ${sources} ${headers})

#target_link_libraries(${PROJECT_NAME}-first-pass m)  # For math library or others


# Bind the include headers
target_include_directories(${BINARY_FIRST_PASS} PRIVATE ${PROJECT_SOURCE_DIR}/)


# Generate custom linker script on each built by running C pre-processor on a template
# file and then use its result linker script to link the binary with

add_custom_command(
        OUTPUT ${LINKER_SCRIPT_FINAL}
        COMMAND ${CMAKE_C_COMPILER} -E -P -D __PREPROCESSOR_RUN_FOR_LINKER_SCRIPT__ -I${PROJECT_SOURCE_DIR}/ -x c ${LINKER_SCRIPT_TEMPLATE} -o ${LINKER_SCRIPT_FINAL}
        DEPENDS ${LINKER_SCRIPT_TEMPLATE}

        # We depend on user referencing in the user_src/system.h file which SoC he is
        # using for this project. Then the final generated liner script will have adjusted
        # all the ROM, slow-ROM (the less documented non-zero-wait flash), RAM sizes and addresses.
        # It will even consider that some chips have dynamically configurable ROM/RAM sizes like:
        # CH32V303xCxx, CH32V307xCxx, CH32V317xCxx, CH32V203RBT6 and CH32V208xBxx .
        # With this approach you can swap between all the SoCs in WCH family while
        # having to maintain just single generic data-driven linker script template.
        DEPENDS user_src/platform_configuration/system.h
        COMMENT "Pre-processing the linker script"
)

add_custom_target(pre_processed_linker_script DEPENDS ${LINKER_SCRIPT_FINAL} user_src/platform_configuration/system.h)
add_dependencies(${BINARY_FIRST_PASS} pre_processed_linker_script)
target_link_options(${BINARY_FIRST_PASS} PRIVATE -T ${LINKER_SCRIPT_FINAL})
target_compile_options(${BINARY_FIRST_PASS} PRIVATE -Wno-packed-bitfield-compat)

# Getting __data_size__ and __bss_size__ from the binary
#add_custom_command(
#        OUTPUT ${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h
#        COMMAND ${CMAKE_COMMAND} -E echo "#define WCH_STARTUP_DATA_SECTION_COPY" > ${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h
#)

#target_sources(${PROJECT_NAME}-first-pass PRIVATE ${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h)

#set_source_files_properties(${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h PROPERTIES GENERATED TRUE)

#add_custom_target(bar DEPENDS ${CMAKE_SOURCE_DIR}/src/system/startup/generated_startup_configuration.h)
#
#add_dependencies(${PROJECT_NAME}-first-pass bar)

# Copy so we can strip debug symbols
add_custom_command(
        TARGET ${BINARY_FIRST_PASS} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_FIRST_PASS} ${BINARY_FIRST_PASS}-stripped
)


# Strip debug symbols so then the display size will not confuse us with debug info
add_custom_command(
        TARGET ${BINARY_FIRST_PASS} POST_BUILD
        COMMAND ${CMAKE_STRIP} ${BINARY_FIRST_PASS}-stripped
)


# Display sizes
add_custom_command(
    TARGET ${BINARY_FIRST_PASS} POST_BUILD
    # https://sourceware.org/binutils/docs/binutils/size.html
    COMMAND ${CMAKE_SIZE} --format=sysv -x --totals "${BINARY_FIRST_PASS}-stripped"
)


# Generate programing HEX
add_custom_command(
    TARGET ${BINARY_FIRST_PASS} POST_BUILD
    # https://man7.org/linux/man-pages/man1/objcopy.1.html
    COMMAND ${CMAKE_OBJCOPY} -O ihex "${BINARY_FIRST_PASS}" "${BINARY_FIRST_PASS_HEX}"
)


# Disassemble from final binary to a listing file
add_custom_command(
        TARGET ${BINARY_FIRST_PASS} POST_BUILD
        # https://man7.org/linux/man-pages/man1/objdump.1.html
        # https://forums.sifive.com/t/16-bit-instructions-from-objdump/979/2
        COMMAND ${CMAKE_OBJDUMP} --disassemble --demangle --all-headers "${BINARY_FIRST_PASS}" > "${BINARY_FIRST_PASS_LST}"
)


if(NOT DEFINED ${SKIP_SECOND_PASS_PREPARATION})

    add_custom_command(
            TARGET ${BINARY_FIRST_PASS} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "First pass binary finished, getting symbols from it"
    )

    add_custom_command(
            TARGET ${BINARY_FIRST_PASS} POST_BUILD
            COMMAND ${CMAKE_NM} ${BINARY_FIRST_PASS} -C > ${CMAKE_BINARY_DIR}/first_pass_symbols.txt
            COMMENT "Getting all symbols"
    )

    add_custom_command(
            TARGET ${BINARY_FIRST_PASS} POST_BUILD
            COMMAND ${BUSYBOX_LOCATION} awk \"$$3==\\"__data_end__\\" {print $$1}\" ${CMAKE_BINARY_DIR}/first_pass_symbols.txt > ${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt
            COMMENT "Parsing out symbol __data_size__"
    )

    add_custom_command(
            TARGET ${BINARY_FIRST_PASS} POST_BUILD
            COMMAND ${BUSYBOX_LOCATION} awk \"$$3==\\"__bss_end__\\" {print $$1}\" ${CMAKE_BINARY_DIR}/first_pass_symbols.txt > ${CMAKE_BINARY_DIR}/first_pass_symbols_bss.txt
            COMMENT "Parsing out symbol __bss_size__"
    )

endif ()
