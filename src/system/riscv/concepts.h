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


    // If any of the masking enum is used
    template<auto Mask>
    concept IsCsrMaskedEnums =
        std::is_same_v<decltype(Mask), Riscv::Csr::Priviledge> ||
        std::is_same_v<decltype(Mask), Riscv::Csr::ReadWrite>;


    // A type which is all QingKeV2, QingKeV3 and QingKeV4 at the same time
    template<typename CsrType>
    concept QingKeCsrEnumType =
        std::is_same_v<CsrType, Riscv::Csr::QingKeV2> ||
        std::is_same_v<CsrType, Riscv::Csr::QingKeV3> ||
        std::is_same_v<CsrType, Riscv::Csr::QingKeV4>;


    // To confirm the CSR enum value belongs to one of the QingKe enum types
    template<auto CsrEnum>
    concept IsQingKeCsrEnum =
        QingKeCsrEnumType<decltype(CsrEnum)>;


    // Confirm the CSR's address is within the 12-bit range
    template<auto CsrAddress>
    concept IsCsrValidAddressRange = (CsrAddress >= 0 && CsrAddress < (1u<<12));


    // Confirm one of the QingKe CSRs was used and it's address
    // is withing correct range
    template<auto CsrValue>
    concept IsCsrEnumValid =
        IsQingKeCsrEnum<CsrValue> &&
        IsCsrValidAddressRange<static_cast<std::uint16_t>(CsrValue)>;


    template<auto CsrAddress>
    concept IsCsrWritableAddress = (
        (CsrAddress & Riscv::Csr::maskReadWrite) != static_cast<std::uint16_t>(Riscv::Csr::ReadWrite::readOnly) );


    template<auto CsrValue>
    concept IsCsrWritable =
        IsQingKeCsrEnum<CsrValue> &&
        IsCsrWritableAddress<static_cast<std::uint16_t>(CsrValue)>;


    template<auto CsrAddress>
    concept IsCsrMachinePriviledgeAddress = (
        (CsrAddress & Riscv::Csr::maskPriviledge) == static_cast<std::uint16_t>(Riscv::Csr::Priviledge::machine) );


    template<auto CsrValue>
    concept IsCsrMachinePriviledge =
        IsQingKeCsrEnum<CsrValue> &&
        IsCsrMachinePriviledgeAddress<static_cast<std::uint16_t>(CsrValue)>;


    template<typename CsrField>
    concept FieldEnumWhichContainsFieldBitMask = requires
        { { CsrField::fieldBitMask }; };


    namespace {

        // The CSRs can come different enums, but must resolve to the same enum value
        // CheckSameCsrValue<QingKeV2::intsyscr, QingKeV4::intsyscr> true
        template<bool OmmitCheck, auto LeftValue, auto RightValue>
        concept CheckSameCsrValue =
            (OmmitCheck || static_cast<uint16_t>(LeftValue)==static_cast<uint16_t>(RightValue));


        // Internal more generic concept to cover both cases below
        // The CsrField enums need to belong to the same CSR, and when OmmitCheck is false
        // then also the Parent is check if the CsrField enums belong to the parent CSR enum
        template<bool OmmitCheck, auto ParentCsr, auto... CsrFields>
        concept CsrFieldEnumMatchingCsrGeneric =
            ( CheckSameCsrValue<OmmitCheck, ParentCsr, Riscv::Csr::QingKeV2::intsyscr> && (Riscv::Csr::Intsyscr::IsAnyField<decltype(CsrFields)> && ...) ) ||
            ( CheckSameCsrValue<OmmitCheck, ParentCsr, Riscv::Csr::QingKeV2::mtvec>    && (Riscv::Csr::Mtvec::IsAnyField<   decltype(CsrFields)> && ...) ) ||
            ( CheckSameCsrValue<OmmitCheck, ParentCsr, Riscv::Csr::QingKeV2::mstatus>  && (Riscv::Csr::Mstatus::IsAnyField< decltype(CsrFields)> && ...) );

    }


    // CsrField(s) need to belong to the same ParentCsr
    template<auto ParentCsr, auto... CsrFields>
    concept SameCsrFieldEnumsAndMatchingParentCsr =
        CsrFieldEnumMatchingCsrGeneric<false, ParentCsr, CsrFields...>;


    // Omit check and provide any CSR as it will be omitted anyway, this
    // makes sure the fields belong to one specidic CSR only, but we
    // do not care which exact CSR it is
    // Note: intsyscr was entered as arbitary CSR, as the OmmitCheck is set to true
    template<auto... CsrFields>
    concept SameCsrFieldEnums =
        CsrFieldEnumMatchingCsrGeneric<true, Riscv::Csr::QingKeV2::intsyscr, CsrFields...>;


};
