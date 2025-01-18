//
// Created by anton on 18/01/2025.
//

#pragma once

#include "soc_types.h"

namespace SoC::PeripheralAddreses {
    constexpr static Peripheral::Gpio::BaseAddress  gpioA = 0x4001'0800;
    constexpr static Peripheral::Gpio::BaseAddress  gpioC = 0x4001'1000;
    constexpr static Peripheral::Gpio::BaseAddress  gpioD = 0x4001'1400;

    constexpr static auto usart1 = Peripheral::Usart::MakeBaseAddress<0x4001'3800>();

}
