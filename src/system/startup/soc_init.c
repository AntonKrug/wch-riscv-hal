//
// Created by anton on 20/01/2025.
//

#include "soc_init.h"
#include "generated_startup_configuration.h"
// #include <stddef.h>

// void* memcpy(void* dest, const void* src, size_t n) {
//     unsigned char* d = (unsigned char*)dest;
//     const unsigned char* s = (const unsigned char*)src;
//     for (size_t i = 0; i < n; ++i) {
//         d[i] = s[i];
//     }
//     return dest;
// }

extern unsigned int __data_rom_start;
extern unsigned int __data_rom_end;
extern unsigned int __data_ram_start;
extern unsigned int __data_ram_end;

// https://gcc.gnu.org/onlinedocs/gcc-4.0.1/gcc/Optimize-Options.html

// inline
void
__attribute__ ((
    // always_inline,
    optimize("-O1"),
    optimize("-fno-strict-aliasing"),
    optimize("-fomit-frame-pointer"),
    optimize("-fexpensive-optimizations"),
    optimize("-fdefer-pop"),
    optimize("-fdelete-null-pointer-checks"),
    // optimize("-fconst-prop"),
    // optimize("-fstrength-reduce"),
    // optimize("-fregmove"),
    optimize("-fcaller-saves"),
    // optimize("-funroll-loops"),
    // optimize("-floop-block"),
    // optimize("-floop-interchange"),
    // optimize("-floop-optimize"),
    // optimize(""),
    optimize("-finline-functions"),
    optimize("-fpeephole"),
    optimize("-fpeephole2"),
    optimize("-fschedule-insns"),
    optimize("-fschedule-insns2")
    ))
prepare_system_for_main(void) {
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

void
// inline
__attribute__((
    // always_inline,
    section(".text.handle_reset"),
    naked,
    optimize("-O0"))) handle_reset() {
    asm volatile (
        ".option norelax\n"           // Disable relaxation for the next instruction
        "la gp, __global_pointer$\n"  // Load the address of the global pointer
        ".option relax\n"             // Enable relaxation again
    );
    asm volatile (
        ".option relax\n"             // Enable relaxation again
        "la sp, __stack_end\n"        // Load the address of the stack pointer
    );
    prepare_system_for_main();
}
