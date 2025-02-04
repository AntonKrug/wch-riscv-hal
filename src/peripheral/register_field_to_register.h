//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"

#include "system/memory_mapped_register/concepts.h"

// RCC


template<Peripheral::Rcc::IsAnyRegField RegField>
constexpr auto regFieldToPeripheralBaseAddr() -> std::uint32_t {
    return Peripheral::Rcc::baseAddr;
}


template<Peripheral::Rcc::Ctlr::IsAnyRegField RegField>
constexpr auto regFieldTypeToRegMemOffset() -> std::uint32_t {
    return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Ctrlr);
}




namespace {
    template<auto RegField>
    requires SoC::MemMappedReg::Concept::RegisterFieldEnumWhichContainsFieldBitMask<decltype(RegField)>
    constexpr auto regFieldMask() -> std::uint32_t {
        return static_cast<std::uint32_t>(decltype(RegField)::fieldBitMask);
    }
}



template<std::uint32_t Address>
inline
auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
socReadRegister() -> std::uint32_t {
    return *reinterpret_cast<volatile std::uint32_t *>(Address);
}

template<std::uint32_t baseAddress, auto TestedRegField>
inline
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto isRegFieldEnumSet() -> bool {
    constexpr auto regMask = regFieldMask<TestedRegField>();
    constexpr auto regOffset = regFieldTypeToRegMemOffset<decltype(TestedRegField)>();
    const auto actualValue = socReadRegister<baseAddress + regOffset>();
    return ( regMask & actualValue) == static_cast<std::uint32_t>(TestedRegField);
}


template<auto ExpectedRegField>
inline
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto isRegFieldEnumSet() -> bool {
    constexpr auto baseAddr = regFieldToPeripheralBaseAddr<decltype(ExpectedRegField)>();
    return isRegFieldEnumSet<baseAddr, ExpectedRegField>();
}