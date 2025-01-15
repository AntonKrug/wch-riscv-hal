//
// Created by anton on 26/12/2024.
//

#pragma once

#define SYSTEM_WCH
#define SYSTEM_WCH_CH32V00x
#define SYSTEM_WCH_CH32V003

/* keep them as defines, even when they could be changed to consexpr, because
 * this file will be used from linker script when doing linker-pre-processing
 * hack to support many platforms with one script */
#define SYSTEM_WCH_CHIP_ROM_SIZE 16k // Flash size of the chip
#define SYSTEM_WCH_CHIP_RAM_SIZE 2k  // RAM so small that GP's 12-bit (4k) can fully cover it to offer relaxation optimizations

#ifndef __PREPROCESSOR_RUN_FOR_LINKER_SCRIPT__
#include "../soc_types.h"


namespace Soc {


    static constexpr Types::GpioACD<
        0x40010800,
        0x40011000,
        0x40011400> Gpio = {};


    // NOLINTBEGIN(readability-static-accessed-through-instance)
    namespace Usart {
        static constexpr Types::UartInstance<
            Peripheral::Usart::BaseAddress(0x40013800),
            0b00,             // Mapping
            Gpio.D.GetPin(4), // D4 = Ck
            Gpio.D.GetPin(5), // D5 = Tx
            Gpio.D.GetPin(6), // D6 = Rx
            Gpio.D.GetPin(3), // D3 = Cts
            Gpio.C.GetPin(2)> // C2 = Rts
        Channel1Mapping0 = {};

        static constexpr Types::UartInstance<
            Peripheral::Usart::BaseAddress(0x40013800),
            0b01,             // Mapping
            Gpio.D.GetPin(7), // D7 = Ck
            Gpio.D.GetPin(0), // D0 = Tx
            Gpio.D.GetPin(1), // D1 = Rx
            Gpio.C.GetPin(3), // C3 = Cts
            Gpio.C.GetPin(2)> // C2 = Rts
        Channel1Mapping1 = {};

        static constexpr Types::UartInstance<
            Peripheral::Usart::BaseAddress(0x40013800),
            0b01,             // Mapping
            Gpio.D.GetPin(7), // D7 = Ck
            Gpio.D.GetPin(6), // D6 = Tx
            Gpio.D.GetPin(5), // D5 = Rx
            Gpio.C.GetPin(6), // C6 = Cts
            Gpio.C.GetPin(7)> // C7 = Rts
        Channel1Mapping2 = {};

        static constexpr Types::UartInstance<
            Peripheral::Usart::BaseAddress(0x40013800),
            0b01,             // Mapping
            Gpio.C.GetPin(5), // C5 = Ck
            Gpio.C.GetPin(0), // C0 = Tx
            Gpio.C.GetPin(1), // C1 = Rx
            Gpio.C.GetPin(6), // C6 = Cts
            Gpio.C.GetPin(7)> // C7 = Rts
        Channel1Mapping3 = {};

        // struct Channel1 {
        //     // operator[] overload
        // }

    }
    // NOLINTEND(readability-static-accessed-through-instance)


}

// static const struct Soc = {
//     int a;
//
//     // static const struct Gpio {
//     //     constexpr static Peripheral::Gpio::Port<0x40010800> A = {};
//     //     constexpr static Peripheral::Gpio::Port<0x40011000> B = {};
//     //     constexpr static Peripheral::Gpio::Port<0x40011400> C = {};
//     // };
//     // static const struct Uart {
//     //     static const struct Instance1 {
//     //         constexpr static Soc::Gpio::
//     //     };
//     // };
// };

#endif