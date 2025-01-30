//
// Created by anton on 30/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"


namespace Riscv::Csr {


    using namespace Riscv;


    template<auto Csr>
    constexpr auto toAddress() -> std::uint16_t requires Concepts::IsQingKeCsrEnum<Csr> {
        return static_cast<std::uint16_t>(Csr);
    }


    template<auto Enum>
    constexpr auto toValue() -> std::uint16_t requires Concepts::IsCsrMaskedEnums<Enum> {
        return static_cast<std::uint16_t>(Enum);
    }


    // Accept any field enums that belong to the same CSR, but do not allow
    // field enums from different CSR to be mixed into them
    template<auto... Args>
    constexpr auto combine() -> std::uint32_t requires Concepts::SameCsrFieldEnum<Args...> {
        return (static_cast<std::uint32_t>(Args) | ...);
    }


    template <Concepts::EnumWithMask EnumType>
    constexpr auto getSingleMaskValue() -> std::uint32_t {
        return static_cast<std::uint32_t>(EnumType::fieldBitMask);
    }


    template <auto... EnumTypeValue>
    constexpr auto getMaskFromFieldEnumValues() -> std::uint32_t {
        return ( getSingleMaskValue<decltype(EnumTypeValue)>() | ...);
    }

}
