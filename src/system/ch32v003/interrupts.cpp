//
// Created by Fredy on 7/13/2025.
//

#include <array>

#include "user_src/platform_configuration/system_defines.h"
#include "user_src/platform_configuration/system_irq.h"

namespace soc::irq {

    namespace handler {

        // C style would be:
        // constexpr void (* const irqhandlersVectorTable[])(void) =
        // `irqHandlersVectorTable[]` the vector table is array
        // `* const` function pointers, the pointers can't be changed later on runtime
        // `void (* ... )(void)` function pointer with no arguments and no return
        // `constexpr` content of the array must be compile-time known so we can put it into the flash
        using HandlerType = void(* const)();

        // Not going to try save stack ((naked)), not going to intend to return
        // from this [[noreturn]]. It is meant to be small (single instruction)
        // infinite loop to intentionally get the CPU stuck when any problems
        // or unimplemented IRQ occurs. Without wasting footprint on prologue
        // or epilogue.
        extern "C" [[noreturn]] void __attribute__((naked)) infinite_loop(void) {
            while (true) {
            }
        }

#ifdef SYSTEM_WCH_IRQ_VECTORIZED

        void __attribute__((weak, alias("infinite_loop"))) non_maskable();             // NMI
        void __attribute__((weak, alias("infinite_loop"))) hard_fault();               // Abnormal events
        void __attribute__((weak, alias("infinite_loop"))) system_timer();             // SysTick
        void __attribute__((weak, alias("infinite_loop"))) software();                 // SW
        void __attribute__((weak, alias("infinite_loop"))) window_timer();             // WWDG
        void __attribute__((weak, alias("infinite_loop"))) supply_voltage_detection(); // PVD
        void __attribute__((weak, alias("infinite_loop"))) flash_global();             // FLASH
        void __attribute__((weak, alias("infinite_loop"))) reset_and_clock_control();  // RCC
        void __attribute__((weak, alias("infinite_loop"))) exti_line0_to7();           // EXTI7_0
        void __attribute__((weak, alias("infinite_loop"))) wake_up_handler();          // AWU

        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch1();          // DMA1_CH1
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch2();          // DMA1_CH2
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch3();          // DMA1_CH3
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch4();          // DMA1_CH4
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch5();          // DMA1_CH5
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch6();          // DMA1_CH6
        void __attribute__((weak, alias("infinite_loop"))) global_dma1_ch7();          // DMA1_CH7

        void __attribute__((weak, alias("infinite_loop"))) analog_digital_converter(); // ADC

        void __attribute__((weak, alias("infinite_loop"))) i2c1_event();               // I2C1_EV
        void __attribute__((weak, alias("infinite_loop"))) i2c1_error();               // I2C1_ER

        void __attribute__((weak, alias("infinite_loop"))) usart1();                   // USART1
        void __attribute__((weak, alias("infinite_loop"))) spi1();                     // SPI1

        void __attribute__((weak, alias("infinite_loop"))) tim1_brake();               // TIM1BRK
        void __attribute__((weak, alias("infinite_loop"))) tim1_update();              // TIM1UP
        void __attribute__((weak, alias("infinite_loop"))) tim1_triggers();            // TIM1TRG
        void __attribute__((weak, alias("infinite_loop"))) tim1_compare();             // TIM1CC

        void __attribute__((weak, alias("infinite_loop"))) tim2();                     // TIM2
#else

        void __attribute__((weak, alias("infinite_loop"))) unified();             // NMI

#endif

    }

    __attribute__ ((retain, used, section(".init.irq_vector_table_plus_one")))
    constexpr std::array<handler::HandlerType, handlers_vector_table_size> handlers_vector_table = {
        &handler::infinite_loop,  // N/A
        &handler::non_maskable,
        &handler::hard_fault,
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::infinite_loop,  // reserved
        &handler::system_timer,
        &handler::infinite_loop,  // reserved
        &handler::software,
        &handler::infinite_loop,  // reserved
        &handler::window_timer,
        &handler::supply_voltage_detection,
        &handler::flash_global,
        &handler::reset_and_clock_control,
        &handler::exti_line0_to7,
        &handler::wake_up_handler,
        &handler::global_dma1_ch1,
        &handler::global_dma1_ch2,
        &handler::global_dma1_ch3,
        &handler::global_dma1_ch4,
        &handler::global_dma1_ch5,
        &handler::global_dma1_ch6,
        &handler::global_dma1_ch7,
        &handler::analog_digital_converter,
        &handler::i2c1_event,
        &handler::i2c1_error,
        &handler::usart1,
        &handler::spi1,
        &handler::tim1_brake,
        &handler::tim1_update,
        &handler::tim1_triggers,
        &handler::tim1_compare,
        &handler::tim2
    };

}
