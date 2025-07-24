//
// Created by anton on 26/12/2024.
//

#pragma once

#define SYSTEM_WCH
#define SYSTEM_WCH_CH32V00x
#define SYSTEM_WCH_CH32V003
#define SYSTEM_WCH_QingKeV2

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

#include "system/soc_types.h"
#include "system/memory_map/peripheral_addresses.h"
#include "peripheral/ch32v00x/dma_channels.h"


namespace soc {



    static constexpr types::GpioACD<
        soc::peripheral_addresses::gpio_a,
        soc::peripheral_addresses::gpio_c,
        soc::peripheral_addresses::gpio_d>
    GpioPort = {};


    // NOLINTBEGIN(readability-static-accessed-through-instance)
    namespace usart {


        static constexpr types::UartInstance<
            soc::peripheral_addresses::usart1,
            0b00U,             // Pin mapping variant
            GpioPort.d.get_pin<4U>(), // D4 = Ck
            GpioPort.d.get_pin<5U>(), // D5 = Tx
            GpioPort.d.get_pin<6U>(), // D6 = Rx
            GpioPort.d.get_pin<3U>(), // D3 = Cts
            GpioPort.c.get_pin<2U>()> // C2 = Rts
        channel1_mapping0 = {};


        static constexpr types::UartInstance<
            soc::peripheral_addresses::usart1,
            0b01U,             // Pin mapping variant
            GpioPort.d.get_pin<7U>(), // D7 = Ck
            GpioPort.d.get_pin<0U>(), // D0 = Tx
            GpioPort.d.get_pin<1U>(), // D1 = Rx
            GpioPort.c.get_pin<3U>(), // C3 = Cts
            GpioPort.c.get_pin<2U>()> // C2 = Rts
        channel1_mapping1 = {};


        static constexpr types::UartInstance<
            soc::peripheral_addresses::usart1,
            0b01U,             // Pin mapping variant
            GpioPort.d.get_pin<7U>(), // D7 = Ck
            GpioPort.d.get_pin<6U>(), // D6 = Tx
            GpioPort.d.get_pin<5U>(), // D5 = Rx
            GpioPort.c.get_pin<6U>(), // C6 = Cts
            GpioPort.c.get_pin<7U>()> // C7 = Rts
        channel1_mapping2 = {};


        static constexpr types::UartInstance<
            soc::peripheral_addresses::usart1,
            0b01U,             // Pin mapping variant
            GpioPort.c.get_pin<5U>(), // C5 = Ck
            GpioPort.c.get_pin<0U>(), // C0 = Tx
            GpioPort.c.get_pin<1U>(), // C1 = Rx
            GpioPort.c.get_pin<6U>(), // C6 = Cts
            GpioPort.c.get_pin<7U>()> // C7 = Rts
        channel1_mapping3 = {};


        // struct Channel1 {
        //     // operator[] overload
        // }

    }
    // NOLINTEND(readability-static-accessed-through-instance)


}


#endif
