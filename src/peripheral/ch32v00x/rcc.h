//
// Created by anton on 04/01/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "field/rcc/apb1prstr.h"
#include "field/rcc/apb2prstr.h"
#include "field/rcc/ctlr.h"
#include "field/rcc/cfgr0.h"
#include "field/rcc/intr.h"

namespace Peripheral::Rcc {

    constexpr std::uint32_t baseAddr = 0x4002'1000;

    // make it work with relative addresses or multiple instances
    enum class RegOffset: std::uint32_t {
        Ahbpcenr  = 0x14,
        Apb2pcenr = 0x18,
        Apb1pcenr = 0x1C,
        Rstsckr   = 0x24
    };

    template<typename Reg>
    concept IsAnyRegField =
        Peripheral::Rcc::Ctlr::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Cfgr0::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Intr::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Apb2prstr::IsAnyRegField<Reg> ||
        Peripheral::Rcc::Apb1prstr::IsAnyRegField<Reg>;

    constexpr std::tuple<
        decltype(Peripheral::Rcc::Ctlr::regFields),
        decltype(Peripheral::Rcc::Cfgr0::regFields),
        decltype(Peripheral::Rcc::Intr::regFields),
        decltype(Peripheral::Rcc::Apb2prstr::regFields),
        decltype(Peripheral::Rcc::Apb1prstr::regFields)
    > registerFields;

}

