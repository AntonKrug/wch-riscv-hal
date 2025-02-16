//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"
#include "ch32v00x/rcc/cfgr0.h"
#include "ch32v00x/rcc/intr.h"
#include "system/register/concept.h"
#include "system/register/combine.h"
#include "system/register/access_primitives_ct.h"

// TODO: own namespace, combine the CSR actions (registers are basic and CSR register on top)
//       able to provide isntance instead of baseAddress
//       constrains on high level functions/public ones, not the lower access level ones


// RCC

//TODO: make sure they are the same type
// match head to each single tail, and with empty Tail pack it will become true by default
// TODO: they need to match same peripheral base type
// template <typename HeadType, typename... TailTypes>
// concept HeadSameAsTail = sizeof...(TailTypes) == 0 || (std::is_same_v<HeadType, TailTypes> && ...);

namespace PeripheralBaseAddr {

    template<Peripheral::Rcc::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return Peripheral::Rcc::baseAddr;
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}

namespace RegFieldTuple {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Ctlr::metadata::fields;
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Cfgr0::metadata::fields;
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Intr::metadata::fields;
    }

    template<auto RegField>
    constexpr auto fromRegField() {
        return fromRegFieldType<decltype(RegField)>();
    }

}

namespace RegMetadata {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Ctlr::metadata::getMetadata();
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Cfgr0::metadata::getMetadata();
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() {
        return Peripheral::Rcc::Intr::metadata::getMetadata();
    }

    template<auto RegField>
    constexpr auto fromRegField() {
        return fromRegFieldType<decltype(RegField)>();
    }


}

namespace RegMemOffset {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Ctlr::metadata::offset);
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Cfgr0::metadata::offset);
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::Intr::metadata::offset);
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}



// ReSharper disable once CppUnnamedNamespaceInHeaderFile
// namespace {
//     template<auto RegField>
//     requires Soc::MemMappedReg::Concept::RegisterFieldEnumWhichContainsFieldBitMask<decltype(RegField)>
//     constexpr auto regFieldMask() -> std::uint32_t {
//         return static_cast<std::uint32_t>(decltype(RegField)::fieldBitMask);
//     }
// }


