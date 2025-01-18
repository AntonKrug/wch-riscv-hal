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

//#define SYSTEM_WCH_CHIP_EEPROM_SIZE 480k
//#define SYSTEM_WCH_CHIP_EEPROM_SIZE 224k

/* TODO: for larger SoCs
 *   Devices with 256k (CH32V303xCxx, CH32V307xCxx, CH32V317xCxx) flash can their sizes tweaked
 *   ROM  RAM   SRAM_CODE_MODE
 *   288K 32K   111
 *   256K 64K   10x  - default for 305/307 (but not 317)
 *   224K 96K   01x
 *   192K 128K  00x
 *   128K 192K  110  - Not supported as datasheet (CH32V303_305_307) section 2.3 stating the 128K RAM is max
 *                     for 305/307. While the coresponding reference manual (CH32FV2x_V3x) stating in
 *                     section 32.4.6 that it's possible and software confirming that 317 has this option and
 *                     for 317 it's default
 *
 *   Some devices with 128k flash (CH32V203RBT6 and CH32V208xBxx) can have their size tweaked too:
 *   ROM  RAM   SRAM_CODE_MODE
 *   128K 64K   00x - default
 *   144K 48K   01x
 *   160K 32K   1xx
 */

#ifndef __PREPROCESSOR_RUN_FOR_LINKER_SCRIPT__
#include "../soc_types.h"
#include "../peripheral_addresses.h"


namespace Soc {


    static constexpr Types::GpioACD<
        SoC::PeripheralAddreses::gpioA,
        SoC::PeripheralAddreses::gpioC,
        SoC::PeripheralAddreses::gpioD> Gpio = {};


    // NOLINTBEGIN(readability-static-accessed-through-instance)
    namespace Usart {
        static constexpr Types::UartInstance<
            SoC::PeripheralAddreses::usart1,
            0b00,             // Pin mapping variant
            Gpio.D.GetPin(4), // D4 = Ck
            Gpio.D.GetPin(5), // D5 = Tx
            Gpio.D.GetPin(6), // D6 = Rx
            Gpio.D.GetPin(3), // D3 = Cts
            Gpio.C.GetPin(2)> // C2 = Rts
        Channel1Mapping0 = {};

        static constexpr Types::UartInstance<
            SoC::PeripheralAddreses::usart1,
            0b01,             // Pin mapping variant
            Gpio.D.GetPin(7), // D7 = Ck
            Gpio.D.GetPin(0), // D0 = Tx
            Gpio.D.GetPin(1), // D1 = Rx
            Gpio.C.GetPin(3), // C3 = Cts
            Gpio.C.GetPin(2)> // C2 = Rts
        Channel1Mapping1 = {};

        static constexpr Types::UartInstance<
            SoC::PeripheralAddreses::usart1,
            0b01,             // Pin mapping variant
            Gpio.D.GetPin(7), // D7 = Ck
            Gpio.D.GetPin(6), // D6 = Tx
            Gpio.D.GetPin(5), // D5 = Rx
            Gpio.C.GetPin(6), // C6 = Cts
            Gpio.C.GetPin(7)> // C7 = Rts
        Channel1Mapping2 = {};

        static constexpr Types::UartInstance<
            SoC::PeripheralAddreses::usart1,
            0b01,             // Pin mapping variant
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