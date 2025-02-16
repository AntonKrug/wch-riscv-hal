//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"

// TODO: combine the CSR actions (registers are basic and CSR register on top)
// TODO: able to provide isntance instead of baseAddress
// TODO: constrains on high level functions/public ones, not the lower access level ones
//       make sure they are the same type
// TODO: match head to each single tail, and with empty Tail pack it will become true by default
// TODO: they need to match same peripheral base type
// template <typename HeadType, typename... TailTypes>
// concept HeadSameAsTail = sizeof...(TailTypes) == 0 || (std::is_same_v<HeadType, TailTypes> && ...);

namespace Peripheral::WholeRegFieldsTuple {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Ctlr::regFields;
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Cfgr0::regFields;
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Intr::regFields;
    }

    template<auto RegField>
    constexpr auto fromRegField() {
        return fromRegFieldType<decltype(RegField)>();
    }

}

namespace Peripheral::RegMemOffset {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Ctlr::regMemOffset);
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Cfgr0::regMemOffset);
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Intr::regMemOffset);
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}
