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

namespace Peripheral::RegFieldTuple {

    template<Rcc::Concept::IsAnyRegField RegFieldType>
    constexpr auto fromPeripheralRegFieldType() { return Rcc::RegFieldTuple::fromRegFieldType<RegFieldType>(); }

    template<typename RegFieldType>
    constexpr auto fromRegFieldType() { return fromPeripheralRegFieldType<RegFieldType>(); }

    template<auto RegField>
    constexpr auto fromRegField() { return fromRegFieldType<decltype(RegField)>(); }

}

namespace Peripheral::RegMemOffset {

    template<Rcc::Concept::IsAnyRegField RegFieldType>
    constexpr auto fromPeripheralRegFieldType() { return Rcc::RegMemOffset::fromRegFieldType<RegFieldType>(); }

    template<typename RegField>
    constexpr auto fromRegFieldType() -> std::uint32_t { return fromPeripheralRegFieldType<RegField>(); }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t { return fromRegFieldType<decltype(RegField)>(); }

}
