//
// Created by anton on 18/01/2025.
//

#pragma once

#include "../../soc_types.h"

namespace SoC::PeripheralAddreses {
    constexpr static Peripheral::Gpio::BaseAddress  gpioA = 0x40010800;
    constexpr static Peripheral::Gpio::BaseAddress  gpioC = 0x40011000;
    constexpr static Peripheral::Gpio::BaseAddress  gpioD = 0x40011400;

    constexpr static auto usart1 = Peripheral::Usart::MakeBaseAddress(0x40013800);

}
