//
// Created by anton on 20/01/2025.
//

#include "soc_init.h"
#include "generated_startup_configuration.h"
// #include "system/riscv/csr_access_c.h"

extern int riscv_qingke2_read_intsyscr();
extern void riscv_qingke2_write_intsyscr(unsigned int value);


extern unsigned int __data_rom_start;
extern unsigned int __data_rom_end;
extern unsigned int __data_ram_start;
extern unsigned int __data_ram_end;

extern unsigned int __block_started_by_symbol;
extern unsigned int __block_started_by_symbol_end;

// https://gcc.gnu.org/onlinedocs/gcc-4.0.1/gcc/Optimize-Options.html

inline
void
__attribute__ ((
    always_inline,
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
    // Data ROM -> RAM copy
    #ifndef WCH_STARTUP_SKIP_DATA_SECTION_COPY
        // Get the address locations from the linker script
        //const unsigned int* rom_end = &__data_rom_end;
        const unsigned int* data_ram_end = &__data_ram_end;
        const unsigned int* data_rom_ptr = &__data_rom_start;
        unsigned int*       data_ram_ptr = &__data_ram_start;

        // Copy data from ROM to RAM, word (4 bytes) at a time
        do {
            *data_ram_ptr++ = *data_rom_ptr++;
        } while (data_ram_ptr < data_ram_end);
    #endif

    // BSS RAM zeroizing
    #ifndef WCH_STARTUP_SKIP_BSS_SECTION_ZEROIZING
        // Get the address locations from the linker script
        const unsigned int* zero_ram_end = &__block_started_by_symbol_end;
        unsigned int*       zero_ram_ptr = &__block_started_by_symbol;

        // Zero data in RAM, word (4 bytes) at a time
        do {
            *zero_ram_ptr++ = 0;
        } while (zero_ram_ptr < zero_ram_end);
    #endif

    // riscv_qingke2_write_intsyscr(0x10);
    // Riscv::Csr::readCSR<>()
}

void
// inline
__attribute__((
    // always_inline,
    section(".text.handle_reset"),
    naked,
    optimize("-O1"))) handle_reset() {
    asm volatile (
        ".option norelax\n"           // Disable relaxation for the next instruction
        "la gp, __global_pointer$\n"  // Load the address of the global pointer
        ".option relax\n"             // Enable relaxation again
        "la sp, __stack_end\n"        // Load the address of the stack pointer
    );
    prepare_system_for_main();
}
