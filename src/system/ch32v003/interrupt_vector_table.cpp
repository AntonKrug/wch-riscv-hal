//
// Created by Fredy on 7/13/2025.
//

extern "C" {
    // Not going to try save stack, not going to intend to return
    // from this. It is meant to be small infinite loop to intentionally
    // get the CPU stuck on any problems or unimplemented IRQ handlers.
    [[noreturn]] void __attribute__((naked, used)) systemGenericCommonHalt(void) {
        while (true) {
        }
    }

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerNonMaskable(void);            // NMI
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerHardFault(void);              // Abnormal events
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerSystemTimer(void);            // SysTick
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerSoftware(void);               // SW
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerWindowTimer(void);            // WWDG
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerSupplyVoltageDetection(void); // PVD
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerFlashGlobal(void);            // FLASH
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerResetAndClockControl(void);   // RCC
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerExtiLine0to7(void);           // EXTI7_0
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerWakeUpHandler(void);          // AWU

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch1(void);          // DMA1_CH1
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch2(void);          // DMA1_CH2
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch3(void);          // DMA1_CH3
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch4(void);          // DMA1_CH4
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch5(void);          // DMA1_CH5
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch6(void);          // DMA1_CH6
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerGlobalDma1Ch7(void);          // DMA1_CH7

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerAnalogDigitalConverter(void); // ADC

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerI2C1event(void);              // I2C1_EV
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerI2C1error(void);              // I2C1_ER

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerUsart1(void);                 // USART1
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerSpi1(void);                   // SPI1

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerTim1brake(void);              // TIM1BRK
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerTim1update(void);             // TIM1UP
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerTim1triggers(void);           // TIM1TRG
    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerTim1compare(void);            // TIM1CC

    void __attribute__((weak, alias("systemGenericCommonHalt"))) irqHandlerTim2();                       // TIM2


    __attribute__ ((used, section(".init.irq_vector_table_plus_one")))
    constexpr void (* const irqHandlersVectorTable[])(void) =
    {
        &systemGenericCommonHalt,  // N/A
        &irqHandlerNonMaskable,
        &irqHandlerHardFault,
        &systemGenericCommonHalt,  // reserved
        &irqHandlerSystemTimer,
        &systemGenericCommonHalt,  // reserved
        &irqHandlerSoftware,
        &systemGenericCommonHalt,  // reserved
        &irqHandlerWindowTimer,
        &irqHandlerSupplyVoltageDetection,
        &irqHandlerFlashGlobal,
        &irqHandlerResetAndClockControl,
        &irqHandlerExtiLine0to7,
        &irqHandlerWakeUpHandler,
        &irqHandlerGlobalDma1Ch1,
        &irqHandlerGlobalDma1Ch2,
        &irqHandlerGlobalDma1Ch3,
        &irqHandlerGlobalDma1Ch4,
        &irqHandlerGlobalDma1Ch5,
        &irqHandlerGlobalDma1Ch6,
        &irqHandlerGlobalDma1Ch7,
        &irqHandlerAnalogDigitalConverter,
        &irqHandlerI2C1event,
        &irqHandlerI2C1error,
        &irqHandlerUsart1,
        &irqHandlerSpi1,
        &irqHandlerTim1brake,
        &irqHandlerTim1update,
        &irqHandlerTim1triggers,
        &irqHandlerTim1compare,
        &irqHandlerTim2,
    };
}
