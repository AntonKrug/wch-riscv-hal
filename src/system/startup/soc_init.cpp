//
// Created by anton on 28/01/2025.
//

#include "generated_startup_configuration.h"
#include "system/riscv/csr_access_ct.h"
#include "system/riscv/csr_register/intsyscr.h"
#include "user_src/system.h"
#include "user_src/system_clock.h"
#include "system/riscv/concepts.h"
#include "peripheral/ch32v00x/rcc.h"
#include "utils/literals/timer.h"
#include "system/register/access_ct.h"
#include "peripheral/common/dma.h"
// #include "dma_experiment_buffer.h"

extern "C" {

    // DATA section symbols
    extern unsigned int __data_rom_start; // NOLINT(*-reserved-identifier)
    extern unsigned int __data_rom_end;   // NOLINT(*-reserved-identifier)
    extern unsigned int __data_ram_start; // NOLINT(*-reserved-identifier)
    extern unsigned int __data_ram_end;   // NOLINT(*-reserved-identifier)


    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    copyDataFromRomToRam() {
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
    }


    // BSS section symbols
    extern unsigned int __block_started_by_symbol;     // NOLINT(*-reserved-identifier)
    extern unsigned int __block_started_by_symbol_end; // NOLINT(*-reserved-identifier)


    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    zeroizeBss() {
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
    }


    // TODO: check if other SoCs might preserve more
    // TODO: possibly other SoCs clear Intr bit 17 too (or maybe more), but on CH32V003 it would be unecesary
    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    resetAndStabilizeClocksToGoodKnownState() {
        using namespace Peripheral::Rcc;
        using namespace Soc::Reg;

        // Enable internal clock HSI on top of existing state -> it will need 6 clock cycles to apply
        setRegFieldsSipCt<
            Ctlr::HSION_RW_InternalHighSpeedClockEnable::enable>();

        // CLear everything, but preserve PLLSRC
        // NOTE: The SW_RW_SystemClockSource will get defaulted to HSI source in this step as well
        keepRegFieldTypesSipCt<
            Cfgr0::PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator>();

        // Disabling clocks and clearing settings in a safe order in a 3 separate steps
        clearRegFieldTypesSipCt<
            Ctlr::PLLON_RW_PhaseLockedLoopEnable,
            Ctlr::CSSON_RW_ClockSafety,
            Ctlr::HSEON_RW_ExternalHighSpeedClockEnable>();

        clearRegFieldTypesSipCt<
            Ctlr::HSEBYP_RW_ExternalHighSpeedClockBypass>();

        clearRegFieldTypesSipCt<
            Cfgr0::PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator>();

        // Clear the possibly-previously-set ready flags
        clearRegFieldTypesSipCt<
            Intr::CSSC_WO_ExternalHighSpeedSecurityClear,
            Intr::PLLRDYC_WO_PhaseLockedLoopReadyClear,
            Intr::HSERDYC_WO_ExternalHighSpeedReadyClear,
            Intr::HSIRDYC_WO_InternalHighSpeedReadyClear,
            Intr::LSIRDYC_WO_InternalLowSpeedReadyClear>();

        Peripheral::Dma::noDuplicateId<
            Peripheral::Dma::Id::Spi1RxHwTrigger,
            Peripheral::Dma::Id::Tim2Ch1HwTrigger,
            Peripheral::Dma::Id::Dma1Ch7SwTrigger
        >();

        uint32_t something;

        Peripheral::Dma::initPeripheralToMemoryCt<
            Peripheral::Dma::Id::Tim1Ch1HwTrigger,
            0x100U,
            Peripheral::Dma::PeripheralAlignment::bit32,
            false,
            true,
            16U,
            false,
            Peripheral::Dma::Priority::low,
            true,
            false,
            true,
            true>(&something);
    }

    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    trimHsiClockCalibration() {
        using namespace Peripheral::Rcc;
        using namespace Soc::Reg;

        constexpr auto rawValue = Ctlr::produceRawTrimValueCt<UserConfig::hsiTrim>();
        setRegFieldsWithRawValueSipCt<rawValue, Ctlr::HSITRIM_RW_InternalHighSpeedClockTrim>();
    }

    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    configureNewClocks() {
        using namespace Peripheral::Rcc;
        //using namespace Literals::Timer;
        using namespace Soc::Reg;

        setRegFieldsSipCt<
            Cfgr0::getHbPrescalerEnum<Soc::Clocks::Hsi, UserConfig::systemClock>()>();

        // clearRegFieldTypesSipCt<
        //     Ahbpcenr::DMA1EN_RW_DirectMemoryAccess1Enable>();

    }

    // https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Optimize-Options.html
    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    prepareSystemForMain() {
        using namespace Riscv;
        using namespace Peripheral;

        // In case we are in soft-reset, disable (probably) pre-existing global interupt,
        // and prepare CSR fields so when "return from interupt" (which will do forcefully
        // at end of this method by invoking mret), then it will restore expected priviledge
        // mode and expected MIE state. Also returning with mret will allow us to have
        // shallower(less demanding on RAM) and cleaner callstack.
        Csr::AccessCt::write<
            Csr::Mstatus::Mie_MRW_MachineInteruptEnable::disable,
            Csr::Mstatus::Mpp_MRW_MachinePreviousPriviledge::machine,
            Csr::Mstatus::Mpie_MRW_MachinePreviousInteruptWasEnabled::enabled>();

        // Initialize the bss and data sections
        zeroizeBss();
        copyDataFromRomToRam();

        // Configure CPU behaviour
        Csr::AccessCt::write<
            Csr::Intsyscr::Hwstken_MRW_HardwarePrologueEpilogue::enable, //HW preamble and epilogue
            Csr::Intsyscr::Inesten_MRW_InteruptNesting::enable>();

        // Configure trap/interupt behaviour
        constexpr auto mtvecValue = Csr::Mtvec::CalculateMtvecRawValue<
            SYSTEM_WCH_VECTOR_TABLE_ADDRESS,
            Csr::Mtvec::Mode0_RW_VectorizationEnable::vectorizedInterupts,
            Csr::Mtvec::Mode1_RW_VectorizedBehaviour::absoluteJumpAddresses>();

        Csr::AccessCt::writeUint32<Csr::QingKeV2::mtvec, mtvecValue>();

        // System clock configuration
        resetAndStabilizeClocksToGoodKnownState();
        trimHsiClockCalibration();
        // configureNewClocks();
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
        prepareSystemForMain();
    }


}

