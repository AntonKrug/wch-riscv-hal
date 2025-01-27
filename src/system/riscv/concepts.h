//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

#include "csr.h"

namespace Riscv::Concepts {


    template<typename CsrType>
    concept IsCsrEnumType =
        std::is_same_v<CsrType, Riscv::Csr::CsrQingKeV2> ||
        std::is_same_v<CsrType, Riscv::Csr::CsrQingKeV3> ||
        std::is_same_v<CsrType, Riscv::Csr::CsrQingKeV4>;

    template<auto Csr>
    concept IsCsrEnum = IsCsrEnumType<decltype(Csr)>;

    template<long long int csrAddress>
    concept IsCsrValidAddress = (csrAddress >= 0 && csrAddress < (1u<<12));


    template<auto Csr>
    concept IsCsrValid =
        IsCsrEnum<Csr> &&
        IsCsrValidAddress<static_cast<std::uint16_t>(Csr)>;


    template<long long int CsrAddress>
    concept IsCsrWritableAddress = (
        (CsrAddress  & Riscv::Csr::readWriteMask) != static_cast<std::uint16_t>(Riscv::Csr::ReadWrite::readOnly) );


    template<auto Csr>
    concept IsCsrWritable =
            IsCsrEnumType<decltype(Csr)> &&
            IsCsrWritableAddress<static_cast<std::uint16_t>(Csr)>;


    template<long long int CsrAddress>
    concept IsCsrMachinePriviledgeAddress = (
        (CsrAddress  & Riscv::Csr::priviledgeMask) == static_cast<std::uint16_t>(Riscv::Csr::Priviledge::machine) );


    template<auto Csr>
    concept IsCsrMachinePriviledge =
        IsCsrEnumType<decltype(Csr)> &&
        IsCsrMachinePriviledgeAddress<static_cast<std::uint16_t>(Csr)>;


}
