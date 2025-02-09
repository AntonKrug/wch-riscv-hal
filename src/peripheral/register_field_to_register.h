//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"
#include "ch32v00x/rcc/cfgr0.h"
#include "ch32v00x/rcc/intr.h"
#include "system/memory_mapped_register/concept.h"
// #include "system/riscv/csr_util.h"
#include "system/register/concept.h"

// TODO: own namespace, combine the CSR actions (registers are basic and CSR register on top)
//       able to provide isntance instead of baseAddress


// RCC

//TODO: make sure they are the same type
// match head to each single tail, and with empty Tail pack it will become true by default
// TODO: they need to match same peripheral base type
template <typename HeadType, typename... TailTypes>
concept HeadSameAsTail = sizeof...(TailTypes) == 0 || (std::is_same_v<HeadType, TailTypes> && ...);


template<Peripheral::Rcc::IsAnyRegField RegFields>
constexpr auto regFieldTypeToPeripheralBaseAddr() -> std::uint32_t {
    return Peripheral::Rcc::baseAddr;
}


template<auto RegFieldHead, auto... RegFieldTails>
// requires HeadSameAsTail<decltype(RegFieldHead), decltype(RegFieldTails)...>
constexpr auto regFieldToPeripheralBaseAddr() -> std::uint32_t {
    return regFieldTypeToPeripheralBaseAddr<decltype(RegFieldHead)>();
}


template<Peripheral::Rcc::Ctlr::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Ctrlr);
}


template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Cfgr0);
}

template<Peripheral::Rcc::Intr::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Intr);
}

template<auto RegFieldHead, auto... RegFieldTails>
// requires HeadSameAsTail<decltype(RegFieldHead), decltype(RegFieldTails)...>
constexpr auto regFieldToRegMemOffset() -> std::uint32_t {
    return regFieldTypeToRegMemOffset<decltype(RegFieldHead)>();
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


//TODO: improve safety and reduce duplication, can have two versions or 1 universal?
template<auto... Fields>
constexpr auto combineFieldValuesToUint32() -> std::uint32_t
// requires Riscv::Concepts::SameCsrFieldEnums<Fields...>
{
    return (static_cast<std::uint32_t>(Fields) | ...);
}


//TODO: depend on fields which belong to same register
template<auto... Fields>
requires (Soc::Reg::Concept::FieldEnumWhithMaskAndAccess<decltype(Fields)> && ...)
constexpr auto combineFieldMasksToUint32() -> std::uint32_t
// requires Riscv::Concepts::SameCsrFieldEnums<Fields...>
{
    return (static_cast<std::uint32_t>(decltype(Fields)::fieldBitMask) | ...);
}


template<std::uint32_t BaseAddr, auto TestedRegField>
requires Soc::Reg::Concept::FieldEnumWithFieldBitMask<decltype(TestedRegField)>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldEnumSet() -> bool {
    constexpr auto regMask   =  static_cast<std::uint32_t>(decltype(TestedRegField)::fieldBitMask);
    constexpr auto regOffset =  regFieldToRegMemOffset<TestedRegField>();
    const auto actualValue   =  socReadRegister<BaseAddr + regOffset>();
    return ( regMask & actualValue)  == static_cast<std::uint32_t>(TestedRegField);
}


template<auto ExpectedRegField>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldEnumSet() -> bool {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<ExpectedRegField>();
    return isRegFieldEnumSet<baseAddr, ExpectedRegField>();
}


template<std::uint32_t BaseAddr, auto... RegFieldValues>
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto regOffset =  regFieldToRegMemOffset<RegFieldValues...>();
    constexpr auto combinedValue = combineFieldValuesToUint32<RegFieldValues...>();
    socWriteRegister<BaseAddr + regOffset, combinedValue>();
}


template<auto... RegFieldValues>
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<RegFieldValues...>();
    return writeRegFieldEnum<baseAddr, RegFieldValues...>();
}


template<
    std::uint32_t baseAddress,
    auto... RegFieldValues>
requires (Soc::Reg::Concept::FieldEnumWithFieldBitMask<decltype(RegFieldValues)> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnumBaseAddr() -> void {
    constexpr auto regOffset     = regFieldToRegMemOffset<RegFieldValues...>();
    constexpr auto combinedValue = combineFieldValuesToUint32<RegFieldValues...>();
    constexpr auto combinedMask  = combineFieldMasksToUint32<RegFieldValues...>();

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


template<auto... RegFieldValues>
requires (Soc::Reg::Concept::FieldEnumWithFieldBitMask<decltype(RegFieldValues)> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<RegFieldValues...>();
    return setRegFieldEnumBaseAddr<baseAddr, RegFieldValues...>();
}


template<std::uint32_t baseAddress, auto... RegFieldMasks>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldEnumBaseAddr() -> void {
    constexpr auto regOffset = regFieldToRegMemOffset<RegFieldMasks...>();
    constexpr auto combinedValue = combineFieldValuesToUint32<RegFieldMasks...>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= combinedValue;
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<auto... RegFieldMasks>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<RegFieldMasks...>();
    return clearRegFieldEnumBaseAddr<baseAddr, RegFieldMasks...>();
}


template<std::uint32_t baseAddress, auto... RegFieldMasks>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldMaskEnumsBaseAddr() -> void {
    constexpr auto regOffset = regFieldToRegMemOffset<RegFieldMasks...>();
    constexpr auto combinedMask = combineFieldValuesToUint32<RegFieldMasks...>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= 0xffffffff ^ combinedMask;
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<auto... RegFieldMasks>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldMaskEnums() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<RegFieldMasks...>();
    return keepRegFieldMaskEnumsBaseAddr<baseAddr, RegFieldMasks...>();
}