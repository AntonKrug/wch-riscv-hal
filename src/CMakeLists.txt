# https://discourse.cmake.org/t/add-custom-command-with-target-dependency-doesnt-rebuild/7029/8
# https://stackoverflow.com/questions/2410976/how-can-i-define-a-string-literal-on-the-gcc-command-line

cmake_minimum_required(VERSION 3.28)
set(CMAKE_SYSTEM_NAME Generic)
project(wch-riscv-hal)
#include_directories(./)

# Force to rebuild this file on each compilation, making sure the date and time info in the file are always up to date
set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")


# Generate list file output for each source file
# https://stackoverflow.com/questions/6666805/what-does-each-column-of-objdumps-symbol-table-mean
foreach(source_file IN LISTS sources)
#    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa, -x -C -adhln=${source_file}.lst")
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${source_file}.lst")
endforeach()


# Stating what binary will be produced
add_executable(${PROJECT_NAME}-first-pass ${sources} ${headers})


# Bind the include headers
target_include_directories(${PROJECT_NAME}-first-pass PRIVATE ${PROJECT_SOURCE_DIR}/)


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
        DEPENDS user_src/system.h
        COMMENT "Pre-processing the linker script"
)

add_custom_target(pre_processed_linker_script DEPENDS ${LINKER_SCRIPT_FINAL} user_src/system.h)
add_dependencies(${PROJECT_NAME}-first-pass pre_processed_linker_script)
target_link_options(${PROJECT_NAME}-first-pass PRIVATE -T ${LINKER_SCRIPT_FINAL})
target_compile_options(${PROJECT_NAME}-first-pass PRIVATE -Wno-packed-bitfield-compat)

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

if(NOT DEFINED ${SKIP_SECOND_PASS_PREPARATION})
    #find_program(NM_TOOL nm)

    add_custom_command(
            TARGET ${PROJECT_NAME}-first-pass POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "First pass binary finished, getting symbols from it"
    )

    add_custom_command(
            TARGET ${PROJECT_NAME}-first-pass POST_BUILD
            COMMAND ${CMAKE_NM} ${PROJECT_NAME}-first-pass${CMAKE_EXECUTABLE_SUFFIX} -C > ${CMAKE_BINARY_DIR}/first_pass_symbols.txt
            COMMENT "Getting all symbols"
    )

    add_custom_command(
            TARGET ${PROJECT_NAME}-first-pass POST_BUILD
            COMMAND ${BUSYBOX_LOCATION} awk \"$$3==\\"__data_end__\\" {print $$1}\" ${CMAKE_BINARY_DIR}/first_pass_symbols.txt > ${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt
            COMMENT "Parsing out symbol __data_size__"
    )

    add_custom_command(
            TARGET ${PROJECT_NAME}-first-pass POST_BUILD
            COMMAND ${BUSYBOX_LOCATION} awk \"$$3==\\"__bss_end__\\" {print $$1}\" ${CMAKE_BINARY_DIR}/first_pass_symbols.txt > ${CMAKE_BINARY_DIR}/first_pass_symbols_bss.txt
            COMMENT "Parsing out symbol __bss_size__"
    )

endif ()
