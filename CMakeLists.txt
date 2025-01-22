# https://discourse.cmake.org/t/add-custom-command-with-target-dependency-doesnt-rebuild/7029/8
# https://stackoverflow.com/questions/2410976/how-can-i-define-a-string-literal-on-the-gcc-command-line

cmake_minimum_required(VERSION 3.28)
project(wch-riscv-hal)
set(CMAKE_CXX_STANDARD 23)
set(INTERMEDIATE_BINARY "first_pass_binary")
find_program(NM_TOOL nm)

function(read_file_into_variable filename var)
    file(READ ${filename} ${var})
endfunction()

include_directories(./)

set (headers
        peripheral/ch32v00x/gpio.h
        peripheral/ch32v00x/rcc.h
        peripheral/ch32v00x/timer.h
        peripheral/ch32v00x/usart.h
        system/CH32V003xxxx/soc_template.h
        system/soc_types.h
        user_src/firmware_build_info.h
        user_src/system.h
        utils/delay.h
        utils/firmware_build_info.h
        utils/literals/all.h
        utils/literals/delay.h
        utils/literals/helper_templates.h
        utils/literals/timer.h
        utils/literals/usart.h
        system/linker_utils.h
        system/memory_map/peripheral_addresses.h
        system/memory_map/concepts.h
        system/startup/soc_init.h
)

set (sources
        user_src/main.cpp
)

# Force to rebuild this file on each compilation, making sure the date and time info in the file are always up to date
set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")

# Generate list file output for each source file
# https://stackoverflow.com/questions/6666805/what-does-each-column-of-objdumps-symbol-table-mean
foreach(source_file IN LISTS sources)
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.lst")
endforeach()

# Stating what binary will be produced
add_executable(${PROJECT_NAME} ${sources} ${headers})
add_executable(${INTERMEDIATE_BINARY} ${sources} ${headers})

# Bind the include headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/)
target_include_directories(${INTERMEDIATE_BINARY} PRIVATE ${PROJECT_SOURCE_DIR}/)

# Generate custom linker script on each built by running C pre-processor on a template
# file and then use its result linker script to link the binary with
set(LINKER_SCRIPT_TEMPLATE ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-universal-boot0.tpl.ld)
set(LINKER_SCRIPT_FINAL ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-for-current-soc-boot0.ld)

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
add_dependencies(${PROJECT_NAME} first_pass_post_build_target3 pre_processed_linker_script)
add_dependencies(${INTERMEDIATE_BINARY} pre_processed_linker_script)
#target_link_options(${PROJECT_NAME} PRIVATE -T ${LINKER_SCRIPT_FINAL})

# Force a first pass to be compiled first so we can fetch the symbols from it
#add_custom_command(
#        TARGET ${INTERMEDIATE_BINARY} POST_BUILD
##        OUTPUT SECOND_PASS
#        COMMAND ${CMAKE_COMMAND} -E echo "Executing post-build steps for my_program"
#        COMMENT "Running post-build steps"
#)


#add_custom_target(
#        first_pass_post_build_target1_get_symbols
##        COMMAND ${NM_TOOL} ${INTERMEDIATE_BINARY} | grep "__data_size" > ${TEMPORARY_EXTRACTED_DATA_SIZE}
#        # TODO fix the binary, make it portable
##        COMMAND "c:/progs/busybox.exe" echo '__data_start__'
##        COMMAND "c:/progs/busybox.exe" cat first_pass_symbols.txt
#        COMMAND ${NM_TOOL} ${INTERMEDIATE_BINARY}.exe -C > first_pass_symbols.txt
##        COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_COMMAND}"
#        COMMAND "c:/progs/busybox.exe" grep "__data_start__" first_pass_symbols.txt > __data.txt
#        COMMAND "c:/progs/busybox.exe" awk "{print $1}" __data.txt
#
#        COMMENT "Extracting symbol __data_size"
#)

# TODO fix the binary, make it portable
add_custom_target(
        first_pass_post_build_target1_get_symbols
        COMMAND ${NM_TOOL} ${INTERMEDIATE_BINARY}.exe -C > first_pass_symbols.txt
        COMMENT "Getting all symbols"
)


add_custom_target(
        first_pass_post_build_target2_get_data_size
        COMMAND "c:/progs/busybox.exe" awk \"$$3==\\"__data_end__\\" {print $$1}\" first_pass_symbols.txt > first_pass_symbols_data.txt
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CmakeReadDataSizeSymbol.cmake
        COMMENT "Extracting symbol __data_size"
#        message(STATUS "File contentsssssssssssssss: ${file_contents}")
)

add_custom_target(
        first_pass_post_build_target3
        COMMAND ${CMAKE_COMMAND} -E echo "${FILE_CONTENTS}"
        #get_property(file_contents GLOBAL PROPERTY FILE_CONTENTS)
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CmakeReadDataSizeSymbol.cmake
        get_property(file_contents CACHE PROPERTY CRYPTOBACKEND)
        COMMAND ${CMAKE_COMMAND} -E echo "jjjjjjjjjjjjjjjjj ${FILE_CONTENTS}"
        COMMENT "Extracting symbol contentsssssssssssssss: ${file_contents}"
)

#add_custom_command(
#        TARGET first_pass_post_build_target3 POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E echo "Reading content from the file"
#        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CmakeReadDataSizeSymbol.cmake
#)

add_dependencies(first_pass_post_build_target1_get_symbols ${INTERMEDIATE_BINARY})
add_dependencies(first_pass_post_build_target2_get_data_size first_pass_post_build_target1_get_symbols)
add_dependencies(first_pass_post_build_target3               first_pass_post_build_target2_get_data_size)

#file(READ ${CMAKE_BINARY_DIR}/first_pass_symbols_data.txt aaaa)
#message(STATUS "Address of __data_end__: ${aaaa}")


## First pass: Compile and link using the linker script
#add_custom_command(
#        OUTPUT ${INTERMEDIATE_BINARY}
#        COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -T ${LINKER_SCRIPT_FINAL} -I${PROJECT_SOURCE_DIR}/ -o ${INTERMEDIATE_BINARY} ${CMAKE_SOURCE_DIR}/${sources}
#        COMMENT "Running first pass linking"
#        DEPENDS ${sources} ${headers} ${LINKER_SCRIPT_FINAL}
#)

# Extract the symbol (_MY_SYMBOL) from the binary
set(TEMPORARY_EXTRACTED_DATA_SIZE "temporary_extracted_data_size.txt")
add_custom_command(
        OUTPUT ${TEMPORARY_EXTRACTED_DATA_SIZE}
        DEPENDS ${INTERMEDIATE_BINARY}
)
