//
// Created by anton on 04/01/2025.
//

#pragma once

#include <cstdint>

namespace Peripheral::Rcc {

    enum class Reset: std::uint8_t {
        nop   = 0,
        reset = 1
    };

    struct PeripheralBus2Reset {
        Reset portD:2;
        Reset portC:2;
        std::uint8_t  reserved2:1;
        Reset portA:2;
        std::uint8_t  reserved1:1;
        Reset ioAuxiliary:2;
    }__attribute((packed));

    enum class ModuleClock: std::uint8_t {
        off = 0,
        on  = 1
    };

    struct PeripheralBus2ClockEnable {
        std::uint16_t reserved6;
        ModuleClock   Usart1:2;
        std::uint8_t  reserved5:1;
        ModuleClock   Spi1:2;
        ModuleClock   Tim1:2;
        std::uint8_t  reserved4:1;
        ModuleClock   Adc1:2;
        std::uint8_t  reserved3:3;
        ModuleClock   portD:2;
        ModuleClock   portC:2;
        std::uint8_t  reserved2:1;
        ModuleClock   portA:2;
        std::uint8_t  reserved1:1;
        ModuleClock   ioAuxiliary:2;
    }__attribute((packed));
}

