//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

#include "csrs_qingke.h"
#include "csr_register/intsyscr.h"
#include "csr_register/mtvec.h"
#include "csr_register/mstatus.h"

namespace Riscv::Concepts {

    using namespace Riscv;


    // If any of the masking enum is used
    template<auto Mask>
    concept IsCsrMaskedEnums =
        std::is_same_v<decltype(Mask), Csr::Priviledge> ||
        std::is_same_v<decltype(Mask), Csr::ReadWrite>;


    // A type which is all QingKeV2, QingKeV3 and QingKeV4 at the same time
    template<typename CsrType>
    concept QingKeCsrEnumType =
        std::is_same_v<CsrType, Csr::QingKeV2> ||
        std::is_same_v<CsrType, Csr::QingKeV3> ||
        std::is_same_v<CsrType, Csr::QingKeV4>;


    // To confirm the CSR enum value belongs to one of the QingKe enum types
    template<auto CsrEnum>
    concept IsQingKeCsrEnum =
        QingKeCsrEnumType<decltype(CsrEnum)>;


    // For writting a CSR value many literal number types can be used
    template<auto CsrValueType>
    concept IsCsrValueCorrectRegisterType =
        std::is_same_v<decltype(CsrValueType), int>                ||
        std::is_same_v<decltype(CsrValueType), unsigned int>       ||
        std::is_same_v<decltype(CsrValueType), std::uint32_t>      ||
        std::is_same_v<decltype(CsrValueType), std::uint16_t>      ||
        std::is_same_v<decltype(CsrValueType), std::uint8_t>;


    // Confirm the CSR's address is within the 12-bit range
    template<auto csrAddress>
    concept IsCsrValidAddressRange = (csrAddress >= 0 && csrAddress < (1u<<12));


    // Confirm one of the QingKe CSRs was used and it's address
    // is withing correct range
    template<auto Csr>
    concept IsCsrEnumValid =
        IsQingKeCsrEnum<Csr> &&
        IsCsrValidAddressRange<static_cast<std::uint16_t>(Csr)>;


    template<auto CsrAddress>
    concept IsCsrWritableAddress = (
        (CsrAddress & Csr::maskReadWrite) != static_cast<std::uint16_t>(Csr::ReadWrite::readOnly) );


    template<auto Csr>
    concept IsCsrWritable =
        IsQingKeCsrEnum<Csr> &&
        IsCsrWritableAddress<static_cast<std::uint16_t>(Csr)>;


    template<auto CsrAddress>
    concept IsCsrMachinePriviledgeAddress = (
        (CsrAddress & Csr::maskPriviledge) == static_cast<std::uint16_t>(Csr::Priviledge::machine) );


    template<auto Csr>
    concept IsCsrMachinePriviledge =
        IsQingKeCsrEnum<Csr> &&
        IsCsrMachinePriviledgeAddress<static_cast<std::uint16_t>(Csr)>;


    template<typename CsrField>
    concept EnumWithMask = requires
        { { CsrField::fieldBitMask }; };

    // template<typename CsrField>
    // concept EnumWithMask = requires
    // { { CsrField::fieldBitMask }; } &&
    // std::is_same_v<typename std::underlying_type_t<CsrField>, decltype(CsrField::fieldBitMask)>;


    // The CSRs can come different enums, but must resolve to the same enum value
    // CheckSameCsrValue<QingKeV2::intsyscr, QingKeV4::intsyscr> true
    template<bool OmmitCheck, auto Left, auto Right>
    concept CheckSameCsrValue =
        (OmmitCheck || static_cast<uint16_t>(Left)==static_cast<uint16_t>(Right));


    // The CsrField enums need to belong to the same CSR, and when OmmitCheck is false
    // then also the Parent is check if the CsrField enums belong to the parent CSR enum
    template<bool OmmitCheck, auto Parent, auto... CsrField>
    concept CsrFieldEnumMatchingCsr =
        ( CheckSameCsrValue<OmmitCheck, Parent, Csr::QingKeV2::intsyscr> && (Csr::Intsyscr::IsAnyField<decltype(CsrField)> && ...) ) ||
        ( CheckSameCsrValue<OmmitCheck, Parent, Csr::QingKeV2::mtvec>    && (Csr::Mtvec::IsAnyField<decltype(CsrField)>    && ...) );


    // Omit check and provide any CSR as it will be omitted anyway, this
    // makes sure the fields belong to one specidic CSR only, but we
    // do not care which exact CSR it is
    // Note: intsyscr was entered as arbitary CSR, as the OmmitCheck is set to true
    template<auto... CsrField>
    concept SameCsrFieldEnum =
        CsrFieldEnumMatchingCsr<true, Csr::QingKeV2::intsyscr, CsrField...>;


};


namespace Riscv::Csr {

    template<Riscv::Csr::Intsyscr::IsAnyField Field>
    constexpr auto getCsrFromField(Field field) {
        return QingKeV2::intsyscr;
    }

    template<Riscv::Csr::Mtvec::IsAnyField... Field>
    constexpr auto getCsrFromField(Field... field) {
        return QingKeV2::mtvec;
    }


}
