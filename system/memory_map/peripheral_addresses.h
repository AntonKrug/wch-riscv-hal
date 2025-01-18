//
// Created by anton on 18/01/2025.
//

#pragma once

#include "system/soc_types.h"

namespace SoC::PeripheralAddreses {
    constexpr static auto gpioA = Peripheral::Gpio::MakeBaseAddress<0x4001'0800>();;
    constexpr static auto gpioB = Peripheral::Gpio::MakeBaseAddress<0x4001'0C00>();;
    constexpr static auto gpioC = Peripheral::Gpio::MakeBaseAddress<0x4001'1000>();;
    constexpr static auto gpioD = Peripheral::Gpio::MakeBaseAddress<0x4001'1400>();
    constexpr static auto gpioE = Peripheral::Gpio::MakeBaseAddress<0x4001'1800>();;

    constexpr static auto usart1 = Peripheral::Usart::MakeBaseAddress<0x4001'3800>();
    constexpr static auto usart2 = Peripheral::Usart::MakeBaseAddress<0x4001'4400>();
    constexpr static auto usart3 = Peripheral::Usart::MakeBaseAddress<0x4001'4800>();

    constexpr static auto uart4  = Peripheral::Usart::MakeBaseAddress<0x4001'4C00>();
    constexpr static auto uart5  = Peripheral::Usart::MakeBaseAddress<0x4000'5000>();
    constexpr static auto uart6  = Peripheral::Usart::MakeBaseAddress<0x4000'1800>();
    constexpr static auto uart7  = Peripheral::Usart::MakeBaseAddress<0x4000'1C00>();
    constexpr static auto uart8  = Peripheral::Usart::MakeBaseAddress<0x4000'2000>();
}
