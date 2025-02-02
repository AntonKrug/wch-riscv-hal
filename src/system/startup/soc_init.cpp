//
// Created by anton on 28/01/2025.
//

#include "generated_startup_configuration.h"
#include "system/riscv/csr_access_ct.h"
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
        using namespace Riscv;

        // In case we are in soft-reset, disable (probably) pre-existing global interupt,
        // and prepare CSR fields so when "return from interupt" (which will do forcefully
        // at end of this method by invoking mret), then it will restore expected priviledge
        // mode and expected MIE state
        Csr::AccessCt::write<
            Csr::Mstatus::MieMachineInteruptEnable::disable,
            Csr::Mstatus::MppMachinePreviousPriviledge::machine,
            Csr::Mstatus::MpieMachinePreviousInteruptEnabled::enabled>();


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

        // Configure CPU behaviour
        Csr::AccessCt::write<
            Csr::QingKeV2::intsyscr,
            Csr::Intsyscr::HwstkenHardwarePrologueEpilogue::enable,                  //HW preamble and epilogue
            Csr::Intsyscr::InestenInteruptNesting::enable>();

        // Configure trap/interupt behaviour
        constexpr std::uint32_t irqVectorTableAddressSanitized =
            Csr::Mtvec::CheckVectorBaseAddressAlignment<SYSTEM_WCH_VECTOR_TABLE_ADDRESS>();

        constexpr auto mtvecValue =
            Csr::combineFieldsToUint32<
                Csr::Mtvec::Mode0VectorizationEnable::vectorizedInterupts,
                Csr::Mtvec::Mode1VectorizedBehaviour::absoluteJumpAddresses>() +
                irqVectorTableAddressSanitized;

        Csr::AccessCt::writeUint32<Csr::QingKeV2::mtvec, mtvecValue>();



        // constexpr auto a = Riscv::Csr::getCsrFromField(Mtvec::Mode0::vectorizedInterupts, Mtvec::Mode1::executeInstructions);
        // constexpr auto a = Riscv::Csr::AccessCt::getCsrFromField(Csr::Mtvec::Mode0::singleUnifiedTrapHandler);
        // Csr::AccessCt::setWithAutoClear<
        //     Csr::Mstatus::Mie::machineIrqEnable,
        //     Csr::Mstatus::Mpie::machinePreviousIrqEnable>();
        // Csr::AccessCt::writeUint32<a, 0>();

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

