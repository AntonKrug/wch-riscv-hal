//
// Created by Fredy on 7/13/2025.
//

#include <array>

namespace Soc::Irq {

    // Not going to try save stack ((naked)), not going to intend to return
    // from this [[noreturn]]. It is meant to be small infinite loop to intentionally
    // get the CPU stuck when any problems or unimplemented IRQ occurs.
    extern "C" [[noreturn]] void __attribute__((naked)) handlerDefault(void) {
        while (true) {
        }
    }

    void __attribute__((weak, alias("handlerDefault"))) handlerNonMaskable();            // NMI
    void __attribute__((weak, alias("handlerDefault"))) handlerHardFault();              // Abnormal events
    void __attribute__((weak, alias("handlerDefault"))) handlerSystemTimer();            // SysTick
    void __attribute__((weak, alias("handlerDefault"))) handlerSoftware();               // SW
    void __attribute__((weak, alias("handlerDefault"))) handlerWindowTimer();            // WWDG
    void __attribute__((weak, alias("handlerDefault"))) handlerSupplyVoltageDetection(); // PVD
    void __attribute__((weak, alias("handlerDefault"))) handlerFlashGlobal();            // FLASH
    void __attribute__((weak, alias("handlerDefault"))) handlerResetAndClockControl();   // RCC
    void __attribute__((weak, alias("handlerDefault"))) handlerExtiLine0to7();           // EXTI7_0
    void __attribute__((weak, alias("handlerDefault"))) handlerWakeUpHandler();          // AWU

    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch1();          // DMA1_CH1
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch2();          // DMA1_CH2
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch3();          // DMA1_CH3
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch4();          // DMA1_CH4
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch5();          // DMA1_CH5
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch6();          // DMA1_CH6
    void __attribute__((weak, alias("handlerDefault"))) handlerGlobalDma1Ch7();          // DMA1_CH7

    void __attribute__((weak, alias("handlerDefault"))) handlerAnalogDigitalConverter(); // ADC

    void __attribute__((weak, alias("handlerDefault"))) handlerI2C1event();              // I2C1_EV
    void __attribute__((weak, alias("handlerDefault"))) handlerI2C1error();              // I2C1_ER

    void __attribute__((weak, alias("handlerDefault"))) handlerUsart1();                 // USART1
    void __attribute__((weak, alias("handlerDefault"))) handlerSpi1();                   // SPI1

    void __attribute__((weak, alias("handlerDefault"))) handlerTim1brake();              // TIM1BRK
    void __attribute__((weak, alias("handlerDefault"))) handlerTim1update();             // TIM1UP
    void __attribute__((weak, alias("handlerDefault"))) handlerTim1triggers();           // TIM1TRG
    void __attribute__((weak, alias("handlerDefault"))) handlerTim1compare();            // TIM1CC

    void __attribute__((weak, alias("handlerDefault"))) handlerTim2();                   // TIM2


    // C style would be:
    // constexpr void (* const irqhandlersVectorTable[])(void) =
    // `irqHandlersVectorTable[]` the vector table is array
    // `* const` function pointers, the pointers can't be changed later on runtime
    // `void (* ... )(void)` function pointer with no arguments and no return
    // `constexpr` content of the array must be compile-time known so we can put it into the flash
    using IrqHandler = void(* const)();

    __attribute__ ((retain, used, section(".init.irq_vector_table_plus_one")))
    constexpr std::array<IrqHandler, 38U> handlersVectorTable = {
        &handlerDefault,  // N/A
        &handlerNonMaskable,
        &handlerHardFault,
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerDefault,  // reserved
        &handlerSystemTimer,
        &handlerDefault,  // reserved
        &handlerSoftware,
        &handlerDefault,  // reserved
        &handlerWindowTimer,
        &handlerSupplyVoltageDetection,
        &handlerFlashGlobal,
        &handlerResetAndClockControl,
        &handlerExtiLine0to7,
        &handlerWakeUpHandler,
        &handlerGlobalDma1Ch1,
        &handlerGlobalDma1Ch2,
        &handlerGlobalDma1Ch3,
        &handlerGlobalDma1Ch4,
        &handlerGlobalDma1Ch5,
        &handlerGlobalDma1Ch6,
        &handlerGlobalDma1Ch7,
        &handlerAnalogDigitalConverter,
        &handlerI2C1event,
        &handlerI2C1error,
        &handlerUsart1,
        &handlerSpi1,
        &handlerTim1brake,
        &handlerTim1update,
        &handlerTim1triggers,
        &handlerTim1compare,
        &handlerTim2
    };
}
