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
        system/ch32v003/soc.h
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
)

set (sources
        user_src/main.cpp
)

#set_source_files_properties(${CMAKE_BINARY_DIR}/my_file.h PROPERTIES GENERATED TRUE)
#set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES GENERATED TRUE)

# Touch the file to force it to change
#add_custom_command(
#        OUTPUT ${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h
#        COMMAND ${CMAKE_COMMAND} -E touch ${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h
#        COMMENT "Forcing recompile by touching ${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h"
#)

# Force to rebuild this file on each compilation, making sure the date
# and time info in the file are always up to date
set_source_files_properties(${PROJECT_SOURCE_DIR}/utils/firmware_build_info.h PROPERTIES COMPILE_FLAGS "-DFORCE_REBUILD")

# generate list files
foreach(source_file IN LISTS sources)
    set_source_files_properties(${source_file} PROPERTIES COMPILE_OPTIONS "-Wa,-adhln=${CMAKE_CURRENT_SOURCE_DIR}/${source_file}.lst")
endforeach()

# binary to be produced
add_executable(${PROJECT_NAME} ${sources} ${headers})

# provide include headers
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/)

# Adding custom linker
set(LINKER_SCRIPT_TEMPLATE ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-universal-boot0.tpl.ld)
set(LINKER_SCRIPT_FINAL ${CMAKE_SOURCE_DIR}/linker/WCH32Vxxx-for-current-soc-boot0.ld)

add_custom_command(
        OUTPUT ${LINKER_SCRIPT_FINAL}
        COMMAND ${CMAKE_C_COMPILER} -E -P -D __PREPROCESSOR_RUN_FOR_LINKER_SCRIPT__ -I ${PROJECT_SOURCE_DIR}/ -x c ${LINKER_SCRIPT_TEMPLATE} -o ${LINKER_SCRIPT_FINAL}
        DEPENDS ${LINKER_SCRIPT_TEMPLATE}
        DEPENDS user_src/system.h
        COMMENT "Pre-processing the linker script"
)

add_custom_target(pre_processed_linker_script DEPENDS ${LINKER_SCRIPT_FINAL} user_src/system.h)

add_dependencies(${PROJECT_NAME} pre_processed_linker_script)
target_link_options(${PROJECT_NAME} PRIVATE -T ${LINKER_SCRIPT_FINAL})
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT_FINAL}")
