//
// Created by anton on 20/01/2025.
//

#pragma once

#include "startup_configuration.h"

extern unsigned int __data_rom_start;
extern unsigned int __data_rom_end;
extern unsigned int __data_ram_start;
extern unsigned int __data_ram_end;

__attribute__ ((optimize("-Os"))) void prepare_system_for_main(void) {

#ifdef WCH_STARTUP_DATA_SECTION_COPY
    // Get the address locations from the linker script
    //const unsigned int* rom_end = &__data_rom_end;
    const unsigned int* ram_end = &__data_ram_end;
    const unsigned int* rom_ptr = &__data_rom_start;
    unsigned int*       ram_ptr = &__data_ram_start;

    // Copy data from ROM to RAM, word (4 bytes) at a time
    do {
        *ram_ptr++ = *rom_ptr++;
    } while (ram_ptr < ram_end);
#endif

}