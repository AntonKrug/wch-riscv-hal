//
// Created by anton on 20/01/2025.
//

#include "soc_init.h"
#include "generated_startup_configuration.h"
#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

extern unsigned int __data_rom_start;
extern unsigned int __data_rom_end;
extern unsigned int __data_ram_start;
extern unsigned int __data_ram_end;

__attribute__ ((optimize("-Os"))) void prepare_system_for_main(void) {
#ifndef WCH_STARTUP_SKIP_DATA_SECTION_COPY
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

__attribute__((section(".text.handle_reset"))) void handle_reset() {
    prepare_system_for_main();
}
