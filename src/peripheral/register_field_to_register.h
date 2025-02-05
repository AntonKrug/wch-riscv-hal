//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"

#include "system/memory_mapped_register/concepts.h"

// TODO: own namespace, combine the CSR actions (registers are basic and CSR register on top)
//       clearing, bulk registers, able to provide isntance instead of baseAddress


// RCC


template<Peripheral::Rcc::IsAnyRegField RegField>
constexpr auto regFieldToPeripheralBaseAddr() -> std::uint32_t {
    return Peripheral::Rcc::baseAddr;
}


template<Peripheral::Rcc::Ctlr::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Ctrlr);
}


template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Cfgr0);
}



// ReSharper disable once CppUnnamedNamespaceInHeaderFile
namespace {
    template<auto RegField>
    requires SoC::MemMappedReg::Concept::RegisterFieldEnumWhichContainsFieldBitMask<decltype(RegField)>
    constexpr auto regFieldMask() -> std::uint32_t {
        return static_cast<std::uint32_t>(decltype(RegField)::fieldBitMask);
    }
}



template<std::uint32_t Addr>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
socReadRegister() -> std::uint32_t {
    return *reinterpret_cast<volatile std::uint32_t *>(Addr);
}


template<std::uint32_t BaseAddr, auto TestedRegField>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldEnumSet() -> bool {
    constexpr auto regMask   =  regFieldMask<TestedRegField>();
    constexpr auto regOffset =  regFieldTypeToRegMemOffset<decltype(TestedRegField)>();
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
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(ExpectedRegField)>();
    return isRegFieldEnumSet<baseAddr, ExpectedRegField>();
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


template<std::uint32_t BaseAddr, auto RegFieldValue>
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto regOffset =  regFieldTypeToRegMemOffset<decltype(RegFieldValue)>();
    socWriteRegister<BaseAddr + regOffset, static_cast<std::uint32_t>(RegFieldValue)>();
}


template<auto RegFieldValue>
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(RegFieldValue)>();
    return writeRegFieldEnum<baseAddr, RegFieldValue>();
}


template<std::uint32_t baseAddress, auto RegFieldValue>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnum() -> void {
    constexpr auto regOffset = regFieldTypeToRegMemOffset<decltype(RegFieldValue)>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue |= static_cast<std::uint32_t>(RegFieldValue);
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<auto RegFieldValue>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(RegFieldValue)>();
    return setRegFieldEnum<baseAddr, RegFieldValue>();
}


template<std::uint32_t baseAddress, auto RegFieldMask>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldEnum() -> void {
    constexpr auto regOffset = regFieldTypeToRegMemOffset<decltype(RegFieldMask)>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= static_cast<std::uint32_t>(RegFieldMask);
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<auto RegFieldMask>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(RegFieldMask)>();
    return clearRegFieldEnum<baseAddr, RegFieldMask>();
}


template<std::uint32_t baseAddress, auto RegFieldMask>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldEnum() -> void {
    constexpr auto regOffset = regFieldTypeToRegMemOffset<decltype(RegFieldMask)>();
    auto actualValue = socReadRegister<baseAddress + regOffset>();
    actualValue &= 0xffffffff ^ static_cast<std::uint32_t>(RegFieldMask);
    socWriteRegister<baseAddress + regOffset>(actualValue);
}


template<auto RegFieldMask>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldEnum() -> void {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(RegFieldMask)>();
    return keepRegFieldEnum<baseAddr, RegFieldMask>();
}