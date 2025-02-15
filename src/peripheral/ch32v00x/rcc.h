//
// Created by anton on 04/01/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "rcc/ctlr.h"
#include "rcc/cfgr0.h"
#include "rcc/intr.h"

namespace Peripheral::Rcc {

    constexpr std::uint32_t baseAddr = 0x4002'1000;

    // make it work with relative addresses or multiple instances
    enum class RegOffset: std::uint32_t {
        Ctrlr     = 0x00,
        Cfgr0     = 0x04,
        Intr      = 0x08,
        Apb2prstr = 0x0C,
        Apb1prstr = 0x10,
        Ahbpcenr  = 0x14,
        Apb2pcenr = 0x18,
        Apb1pcenr = 0x1C,
        Rstsckr   = 0x24
    };


    enum class Reset: std::uint8_t {
        nop   = 0,
        reset = 1
    };

    // struct PeripheralBus2Reset {
    //     Reset portD:2;
    //     Reset portC:2;
    //     std::uint8_t  reserved2:1;
    //     Reset portA:2;
    //     std::uint8_t  reserved1:1;
    //     Reset ioAuxiliary:2;
    // }__attribute((packed));

    enum class ModuleClock: std::uint8_t {
        off = 0,
        on  = 1
    };

    // struct PeripheralBus2ClockEnable {
    //     std::uint16_t reserved6;
    //     ModuleClock   Usart1:2;
    //     std::uint8_t  reserved5:1;
    //     ModuleClock   Spi1:2;
    //     ModuleClock   Tim1:2;
    //     std::uint8_t  reserved4:1;
    //     ModuleClock   Adc1:2;
    //     std::uint8_t  reserved3:3;
    //     ModuleClock   portD:2;
    //     ModuleClock   portC:2;
    //     std::uint8_t  reserved2:1;
    //     ModuleClock   portA:2;
    //     std::uint8_t  reserved1:1;
    //     ModuleClock   ioAuxiliary:2;
    // }__attribute((packed));

    template<typename Reg>
    concept IsAnyRegField =
        Peripheral::Rcc::Ctlr::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Cfgr0::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Intr::IsAnyRegField<Reg>;

    constexpr std::tuple<
        decltype(Peripheral::Rcc::Ctlr::fields),
        decltype(Peripheral::Rcc::Cfgr0::fields),
        decltype(Peripheral::Rcc::Intr::fields)> registerFields;

}

