# https://discourse.cmake.org/t/add-custom-command-with-target-dependency-doesnt-rebuild/7029/8
# https://stackoverflow.com/questions/2410976/how-can-i-define-a-string-literal-on-the-gcc-command-line

cmake_minimum_required(VERSION 3.28)
project(wch-riscv-hal)
set(CMAKE_CXX_STANDARD 23)

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
)

set (sources
        user_src/main.cpp
)

# Force to rebuild this file on each compilation, making sure the date and time info in the file are always up to date
set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")

# Generate list file output for each source file
foreach(source_file IN LISTS sources)
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.lst")
endforeach()

# Stating what binary will be produced
add_executable(${PROJECT_NAME} ${sources} ${headers})

# Bind the include headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/)

# Generate custom linker script on each built by running C pre-processor on a template
# file and then use its result linker script to link the binary with
set(LINKER_SCRIPT_TEMPLATE ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-universal-boot0.tpl.ld)
set(LINKER_SCRIPT_FINAL ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-for-current-soc-boot0.ld)

add_custom_command(
        OUTPUT ${LINKER_SCRIPT_FINAL}
        COMMAND ${CMAKE_C_COMPILER} -E -P -D __PREPROCESSOR_RUN_FOR_LINKER_SCRIPT__ -I ${PROJECT_SOURCE_DIR}/ -x c ${LINKER_SCRIPT_TEMPLATE} -o ${LINKER_SCRIPT_FINAL}
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
add_dependencies(${PROJECT_NAME} pre_processed_linker_script)
#target_link_options(${PROJECT_NAME} PRIVATE -T ${LINKER_SCRIPT_FINAL})
