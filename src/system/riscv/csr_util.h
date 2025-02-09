//
// Created by anton on 30/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"


namespace Riscv::Csr {


    template<auto Csr>
    constexpr auto toAddress() -> std::uint16_t requires Riscv::Concepts::IsQingKeCsrEnum<Csr> {
        return static_cast<std::uint16_t>(Csr);
    }


    // Accept any field enums that belong to the same CSR, but do not allow
    // field enums from different CSR to be mixed into them
    // TODO: depricate
    template<auto... Fields>
    constexpr auto combineFieldsToUint32() -> std::uint32_t requires Riscv::Concepts::SameCsrFieldEnums<Fields...> {
        return (static_cast<std::uint32_t>(Fields) | ...);
    }


    template <Riscv::Concepts::FieldEnumWhichContainsFieldBitMask EnumType>
    constexpr auto getMaskFromSingleFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(EnumType::fieldBitMask);
    }


    template <auto... EnumTypeValues>
    constexpr auto getMaskFromFieldEnumValues() -> std::uint32_t {
        return ( getMaskFromSingleFieldType<decltype(EnumTypeValues)>() | ...);
    }


}
