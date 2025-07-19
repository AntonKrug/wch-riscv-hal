//
// Created by anton on 18/01/2025.
//

#pragma once

#include "system/soc_types.h"

namespace soc::peripheral_addreses {


    // General Purpose Input/Output
    constexpr static auto gpio_a  = peripheral::gpio::make_base_address<0x4001'0800>();;
    constexpr static auto gpio_b  = peripheral::gpio::make_base_address<0x4001'0C00>();;
    constexpr static auto gpio_c  = peripheral::gpio::make_base_address<0x4001'1000>();;
    constexpr static auto gpio_d  = peripheral::gpio::make_base_address<0x4001'1400>();
    constexpr static auto gpio_e  = peripheral::gpio::make_base_address<0x4001'1800>();;


    // Universal Synchronous Asynchronous Receiver Transmitter
    constexpr static auto usart1 = peripheral::usart::MakeBaseAddress<0x4001'3800>();
    constexpr static auto usart2 = peripheral::usart::MakeBaseAddress<0x4001'4400>();
    constexpr static auto usart3 = peripheral::usart::MakeBaseAddress<0x4001'4800>();


    // Universal Asynchronous Receiver Transmitter
    constexpr static auto uart4  = peripheral::usart::MakeBaseAddress<0x4001'4C00>();
    constexpr static auto uart5  = peripheral::usart::MakeBaseAddress<0x4000'5000>();
    constexpr static auto uart6  = peripheral::usart::MakeBaseAddress<0x4000'1800>();
    constexpr static auto uart7  = peripheral::usart::MakeBaseAddress<0x4000'1C00>();
    constexpr static auto uart8  = peripheral::usart::MakeBaseAddress<0x4000'2000>();


}
