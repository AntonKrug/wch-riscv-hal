//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

#include "csr.h"

namespace Riscv::Concepts {

    template<auto Mask>
    concept IsCsrMaskedEnums =
        std::is_same_v<decltype(Mask), Riscv::Csr::Priviledge> ||
        std::is_same_v<decltype(Mask), Riscv::Csr::ReadWrite>;


    template<auto CsrEnum>
    concept IsCsrEnumType =
        std::is_same_v<decltype(CsrEnum), Riscv::Csr::QingKeV2> ||
        std::is_same_v<decltype(CsrEnum), Riscv::Csr::QingKeV3> ||
        std::is_same_v<decltype(CsrEnum), Riscv::Csr::QingKeV4>;


    template<auto CsrValueType>
    concept IsCsrValueCorrectType =
        std::is_same_v<decltype(CsrValueType), int>                ||
        std::is_same_v<decltype(CsrValueType), unsigned int>       ||
        std::is_same_v<decltype(CsrValueType), std::uint32_t>      ||
        std::is_same_v<decltype(CsrValueType), std::uint16_t>      ||
        std::is_same_v<decltype(CsrValueType), std::uint8_t>;


    template<auto CsrValueType>
    concept IsCsrValueZero = CsrValueType == 0u;


    template<auto csrAddress>
    concept IsCsrValidAddressRange = (csrAddress >= 0 && csrAddress < (1u<<12));


    template<auto Csr>
    concept IsCsrEnumValid =
        IsCsrEnumType<Csr> &&
        IsCsrValidAddressRange<static_cast<std::uint16_t>(Csr)>;


    template<auto CsrAddress>
    concept IsCsrWritableAddress = (
        (CsrAddress  & Riscv::Csr::maskReadWrite) != static_cast<std::uint16_t>(Riscv::Csr::ReadWrite::readOnly) );


    template<auto Csr>
    concept IsCsrWritable =
        IsCsrEnumType<Csr> &&
        IsCsrWritableAddress<static_cast<std::uint16_t>(Csr)>;


    template<auto CsrAddress>
    concept IsCsrMachinePriviledgeAddress = (
        (CsrAddress  & Riscv::Csr::maskPriviledge) == static_cast<std::uint16_t>(Riscv::Csr::Priviledge::machine) );


    template<auto Csr>
    concept IsCsrMachinePriviledge =
        IsCsrEnumType<Csr> &&
        IsCsrMachinePriviledgeAddress<static_cast<std::uint16_t>(Csr)>;


}
