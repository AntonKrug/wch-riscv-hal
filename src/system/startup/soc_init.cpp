//
// Created by anton on 28/01/2025.
//

#include "generated_startup_configuration.h"
#include "system/riscv/csr_access.h"
#include "system/riscv/csr_register/intsyscr.h"
#include "user_src/system.h"
#include "system/riscv/concepts.h"

extern "C" {
    // DATA section
    extern unsigned int __data_rom_start; // NOLINT(*-reserved-identifier)
    extern unsigned int __data_rom_end;   // NOLINT(*-reserved-identifier)
    extern unsigned int __data_ram_start; // NOLINT(*-reserved-identifier)
    extern unsigned int __data_ram_end;   // NOLINT(*-reserved-identifier)


    // BSS section
    extern unsigned int __block_started_by_symbol;     // NOLINT(*-reserved-identifier)
    extern unsigned int __block_started_by_symbol_end; // NOLINT(*-reserved-identifier)


    // https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Optimize-Options.html
    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
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

        using namespace Riscv::Csr;

        Access::write<
            QingKeV2::intsyscr,
            Intsyscr::Hwstken::hpeEnable,                  //HW preamble and epilogue
            Intsyscr::Inesten::interuptNestingEnable>();

        constexpr std::uint32_t irqVectorTableAddressSanitized =
            Mtvec::CheckVectorBaseAddressAlignment<SYSTEM_WCH_VECTOR_TABLE_ADDRESS>();

        constexpr auto mtvecValue =
            combine<Mtvec::Mode0::vectorizedInterupts,Mtvec::Mode1::absoluteJumpAddresses>() +
            irqVectorTableAddressSanitized;

        Access::write<QingKeV2::mtvec, mtvecValue>();

        // constexpr auto a = Riscv::Csr::getCsrFromField(Mtvec::Mode0::vectorizedInterupts, Mtvec::Mode1::executeInstructions);
        constexpr auto a = Riscv::Csr::getCsrFromField(Mtvec::Mode0::vectorizedInterupts);
        Access::write<a, mtvecValue>();

        // readCsr<QingKeV2::intsyscr>();
        // writeCsr<QingKeV2::intsyscr, 0>();
        // writeCsr<QingKeV2::intsyscr, 0x20000000>();
        // writeCsr<QingKeV2::intsyscr, 0xffffff>();

        // constexpr auto interuptSettings = combine<
        //     Intsyscr::Hwstken::hpeEnable,
        //     Intsyscr::Inesten::interuptNestingEnable,
        //     Intsyscr::Eabien::eabiDisable>();

        // constexpr auto sss = Csr::getMaskFromFieldEnumValues<Intsyscr::Eabien::eabiDisable>();

        // writeCsr<QingKeV2::intsyscr, interuptSettings>();
        //writeCsr<QingKeV2::intsyscr, 31>();

        // writeCsr<QingKeV2::intsyscr, Intsyscr::Hwstken::hpeEnable, Intsyscr::Eabien::eabiEnable>();
        // writeCsr<QingKeV2::dcsr, A::B::b, A::A::a>();
        //writeCsr<QingKeV2::dcsr, A::B::b, A::A::a, Intsyscr::Hwstken::hpeEnable>();
        // riscv_qingke2_write_intsyscr(0x10);
        // Riscv::Csr::readCSR<>()
    }

    void
    __attribute__((
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
}

