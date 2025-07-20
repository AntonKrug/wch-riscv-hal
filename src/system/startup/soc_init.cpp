//
// Created by anton on 28/01/2025.
//

#include "generated_startup_configuration.h"
#include "system/riscv/csr_access_primitives.h"
#include "system/riscv/csr_access_ct.h"
#include "system/riscv/csr_register/intsyscr.h"
#include "user_src/platform_configuration/system.h"
#include "user_src/platform_configuration/system_clock.h"
#include "user_src/platform_configuration/system_defines.h"
#include "user_src/platform_configuration/system_irq.h"
#include "system/riscv/concepts.h"
#include "peripheral/ch32v00x/rcc.h"
#include "utils/literals/timer.h"
#include "system/register/access_ct.h"
#include "peripheral/common/dma.h"
// #include "dma_experiment_buffer.h"

extern "C" {

    // DATA section symbols
    extern const unsigned int __data_rom_start; // NOLINT(*-reserved-identifier)
    extern const unsigned int __data_rom_end;   // NOLINT(*-reserved-identifier)
    extern const unsigned int __data_ram_start; // NOLINT(*-reserved-identifier)
    extern const unsigned int __data_ram_end;   // NOLINT(*-reserved-identifier)


    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    copy_data_from_rom_to_ram() {
        // Data ROM -> RAM copy
        #ifndef WCH_STARTUP_SKIP_DATA_SECTION_COPY
            // Get the address locations from the linker script
            //const unsigned int* rom_end = &__data_rom_end;
            const unsigned int* data_ram_end = &__data_ram_end;
            const unsigned int* data_rom_ptr = &__data_rom_start;
            auto                data_ram_ptr = const_cast<unsigned int*>(&__data_ram_start);

            // Copy data from ROM to RAM, word (4 bytes) at a time
            do {
                *data_ram_ptr++ = *data_rom_ptr++;
            } while (data_ram_ptr < data_ram_end);
        #endif
    }


    // BSS section symbols
    extern const unsigned int __block_started_by_symbol;     // NOLINT(*-reserved-identifier)
    extern const unsigned int __block_started_by_symbol_end; // NOLINT(*-reserved-identifier)


    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    zeroize_bss() {
        // BSS RAM zeroizing
        #ifndef WCH_STARTUP_SKIP_BSS_SECTION_ZEROIZING
            // Get the address locations from the linker script
            const unsigned int* zero_ram_end = &__block_started_by_symbol_end;
            auto                zero_ram_ptr = const_cast<unsigned int*>(&__block_started_by_symbol);

            // Zero data in RAM, word (4 bytes) at a time
            do {
                *zero_ram_ptr++ = 0U;
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
    reset_and_stabilize_clocks_to_good_known_state() {
        using namespace peripheral::rcc;
        using namespace soc::reg;

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

        // peripheral::dma::noDuplicateId<
        //     peripheral::dma::Id::Spi1RxHwTrigger,
        //     peripheral::dma::Id::Tim2Ch1HwTrigger,
        //     peripheral::dma::Id::Dma1Ch7SwTrigger
        // >();
        //
        // uint32_t something;
        //
        // peripheral::dma::initPeripheralToMemoryCt<
        //     peripheral::dma::Id::Tim1Ch1HwTrigger,
        //     0x100U,
        //     peripheral::dma::PeripheralAlignment::bit32,
        //     false,
        //     true,
        //     16U,
        //     false,
        //     peripheral::dma::Priority::low,
        //     true,
        //     false,
        //     true,
        //     true>(&something);
    }

    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    trim_hsi_clock_calibration() {
        using namespace peripheral::rcc;
        using namespace soc::reg;

        constexpr auto raw_value = Ctlr::produceRawTrimValueCt<user_config::hsi_trim>();
        setRegFieldsWithRawValueSipCt<raw_value, Ctlr::HSITRIM_RW_InternalHighSpeedClockTrim>();
    }

    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    configure_swio_pin_d1() {
        soc::reg::setRegFieldsSipCt<
            peripheral::rcc::Apb2pcenr::IOPDEN_RW_InputOutputPortDClockEnable::clockEnable>();

        constexpr auto swio  = soc::Gpio.D.get_pin<1U>();
        swio.mode_input_ct<peripheral::gpio::PinInputDrive::pull_up_pull_down>();
        swio = 0U;

        // TODO: user controled GPIO setup
        // Enable SWIO support on RCC
        // PORT D1 as input
        // clear GPIOD_CFGLR 7-6 CNF1 mode (pullup/down...) and 5-4 MODE1 (input/output + speed)
        // then set INPUT & pull-up/down mode
        // GPIOD_BSHR -> write BS1=1 to clear OUTDR bit for BS1
    }

    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    configure_new_clocks() {
        using namespace peripheral::rcc;
        using namespace soc::reg;

        setRegFieldsSipCt<
            Cfgr0::get_hb_prescaler_enum<
                soc::clocks::hsi,
                user_config::system_clock
            >()
        >();

        // clearRegFieldTypesSipCt<
        //     Ahbpcenr::DMA1EN_RW_DirectMemoryAccess1Enable>();

    }

    [[noreturn]] extern void main_user(void);

    // the provided linker script is not compile time known, use calculated value instead
    // extern const unsigned int __main_user;   // NOLINT(*-reserved-identifier)


    // https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Optimize-Options.html
    inline
    void
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    prepareSystemForMain() {
        using namespace riscv;
        using namespace peripheral;

        // In case we are in soft-reset, disable (probably) pre-existing global interupt,
        // and prepare CSR fields so when "return from interupt" (which will do forcefully
        // at end of this method by invoking mret), then it will restore expected priviledge
        // mode and expected MIE state. Also returning with mret will allow us to have
        // shallower(less demanding on RAM) and cleaner callstack.
        csr::access_ct::write<
            csr::mstatus::Mie_MRW_MachineInteruptEnable::disable,
            csr::mstatus::Mpp_MRW_MachinePreviousPriviledge::machine,
            csr::mstatus::Mpie_MRW_MachinePreviousInteruptWasEnabled::enabled>();

        // Initialize the bss and data sections
        zeroize_bss();
        copy_data_from_rom_to_ram();

        // Configure CPU behaviour
        csr::access_ct::write<
            csr::intsyscr::Hwstken_MRW_HardwarePrologueEpilogue::enable, //HW preamble and epilogue
            csr::intsyscr::Inesten_MRW_InteruptNesting::enable>();

        // Configure trap/interupt behaviour
#ifdef SYSTEM_WCH_IRQ_VECTORIZED
        constexpr auto mtvec_value = csr::mtvec::calculate_mtvec_raw_value<
            SYSTEM_WCH_VECTOR_TABLE_ADDRESS,
            csr::mtvec::Mode0_RW_VectorizationEnable::vectorizedInterupts,
            csr::mtvec::Mode1_RW_VectorizedBehaviour::absoluteJumpAddresses>();
#else
        constexpr auto mtvec_value = csr::mtvec::CalculateMtvecRawValue<
            SYSTEM_WCH_VECTOR_TABLE_ADDRESS,
            csr::mtvec::Mode0_RW_VectorizationEnable::singleUnifiedTrapHandler,
            csr::mtvec::Mode1_RW_VectorizedBehaviour::executeInstructions>();
#endif

        csr::access_ct::write_uint32<csr::QingKeV2::mtvec, mtvec_value>();

        // System clock configuration
        reset_and_stabilize_clocks_to_good_known_state();
        trim_hsi_clock_calibration();
        configure_new_clocks();
        configure_swio_pin_d1();


        // Enter the end-user main function by setting up the RA
        // and just exit function instead calling it and deepening
        // the callstack

        // auto address = static_cast<std::uint8_t>(__main_user);
        // we do not know the value of __main_user yet, and it's not consexpr
        // https://stackoverflow.com/questions/30208685/how-to-declare-constexpr-extern
        // even if it's not runtime value but doing it the official way wouldn't work
        // csr::access_ct::writeUint32<csr::QingKeV2::mepc, __main_user>();
        // Nor:
        // void (*main_ptr)() = &main_user;
        // csr::access_ct::writeUint32<csr::QingKeV2::mepc, reinterpret_cast<std::uint32_t>(main_ptr)>();

        // Setting up machine exception program counter to point to our main user function.
        // Returning like from interupt/excerption, similar as jumping but this makes
        // class stack to not show the system init functions and allows smaller SP usage
        // If the __main_user 2046 bytes away from the begining of the memory (0), then
        // it can be optimized and the __main_user address loaded directly with a
        // LW instruction and 12-bit imediate operand (-2048 to 2047) and save
        // one opcode. If the __main_user would be max 28 then it would fit
        // into 5-bit operand and the imedetiate variant of CSR write, saving
        // yet another instruction.
        // csr::access::write_uint32<csr::QingKeV2::mepc>(__main_user);
        // csr::access::write_uint32<>(__main_user);
        csr::access_ct::write_uint32<csr::QingKeV2::mepc, soc::irq::memory_after_vector_table>();
        // csr::access_ct::writeUint32<csr::QingKeV2::mepc, 0x1c>();

        __asm__ volatile("mret\n");

        // Optionally we could do extra safty that if main ever exits, we could then
        // halt the system permanently but use [[noreturn]] on main and make sure
        // that's achieved within the main itself. Also then we do not need to
        // call main() directly and already shrink the callstack, instead we can
        // RA pointer trick and just return from init function
        // systemGenericCommonHalt();
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

