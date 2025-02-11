//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"
#include "ch32v00x/rcc/cfgr0.h"
#include "ch32v00x/rcc/intr.h"
// #include "system/riscv/csr_util.h"
#include "system/register/concept.h"

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

namespace RegMemOffset {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Ctrlr);
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Cfgr0);
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Intr);
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


#pragma region Primitives

template<std::uint32_t Addr>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
socReadRegister() -> std::uint32_t {
    return *reinterpret_cast<volatile std::uint32_t *>(Addr);
}


template<std::uint32_t Addr, std::uint32_t Value>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
socWriteRegister() -> void {
    *(reinterpret_cast<volatile std::uint32_t *>(Addr))=Value;
}


template<std::uint32_t Addr>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
socWriteRegister(const std::uint32_t Value) -> void {
    *(reinterpret_cast<volatile std::uint32_t *>(Addr))=Value;
}

#pragma endregion

#pragma region isReg

template<std::uint32_t BaseAddr, auto TestedRegFieldHead, auto... TestedRegFieldTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<TestedRegFieldHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<TestedRegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldSetMip() -> bool {
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
    constexpr auto combinedMask  = Soc::Reg::combineFieldMasksToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
    constexpr auto regOffset     = RegMemOffset::fromRegField<TestedRegFieldHead>();
    const     auto actualValue   = socReadRegister<BaseAddr + regOffset>();

    return (actualValue & combinedMask)  == combinedValue;
}


template<auto TestedRegFieldHead, auto... TestedRegFieldTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<TestedRegFieldHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<TestedRegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldSetSip() -> bool {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<TestedRegFieldHead>();
    return isRegFieldSetMip<baseAddr, TestedRegFieldHead, TestedRegFieldTails...>();
}

#pragma endregion

#pragma region writeReg

template<std::uint32_t BaseAddr, auto RegFieldValueHead, auto... RegFieldValueTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueTails> && ...)
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto regOffset =  RegMemOffset::fromRegField<RegFieldValueHead>();
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<RegFieldValueHead, RegFieldValueTails...>();
    socWriteRegister<BaseAddr + regOffset, combinedValue>();
}


template<auto RegFieldValueHead, auto... RegFieldValueTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueTails> && ...)
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<RegFieldValueHead>();
    return writeRegFieldEnum<baseAddr, RegFieldValueHead, RegFieldValueTails...>();
}

#pragma endregion

#pragma region setReg

template<
    std::uint32_t baseAddress,
    auto RegFieldValueHead,
    auto... RegFieldValueTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnumsMip() -> void {
    constexpr auto regOffset     = RegMemOffset::fromRegField<RegFieldValueHead>();
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<RegFieldValueHead, RegFieldValueTails...>();
    constexpr auto combinedMask  = Soc::Reg::combineFieldMasksToUint32<RegFieldValueHead, RegFieldValueTails...>();

    // TODO: detect when full 0xffffffff mask and replace the clear with write,
    //       also detect the cases where all writable fields are already masked and
    //       act as the full clear and do write instead

    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= combinedMask;
    if constexpr (combinedValue > 0) {
        actualValue |= combinedValue;
    }
    socWriteRegister<baseAddress + regOffset>(actualValue);
}

template<auto RegFieldValueHead, auto... RegFieldValueTails>
requires
    Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueHead> &&
    (Soc::Reg::Concept::FieldWithFieldBitMask<RegFieldValueTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnumsSip() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<RegFieldValueHead>();
    return setRegFieldEnumsMip<baseAddr, RegFieldValueHead, RegFieldValueTails...>();
}

#pragma endregion

#pragma region ClearReg

template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldTypesMip() -> void {
    constexpr auto regOffset    = RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
    constexpr auto combinedMask = Soc::Reg::combineFieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
    const     auto actualValue  = socReadRegister<baseAddress + regOffset>();

    socWriteRegister<baseAddress + regOffset>(actualValue & combinedMask);
}


template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldTypesSip() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegFieldType<RegFieldTypeHead>();
    return clearRegFieldTypesMip<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
}

#pragma endregion

#pragma region KeepReg

template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldTypesMip() -> void {
    constexpr auto regOffset = RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
    constexpr auto combinedMask = Soc::Reg::combineFieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= 0xffffffff ^ combinedMask;
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldTypesSip() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegFieldType<RegFieldTypeHead>();
    return keepRegFieldTypesMip<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
}

#pragma endregion