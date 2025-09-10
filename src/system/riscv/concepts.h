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

namespace riscv::concepts {
        // A type which is all QingKeV2, QingKeV3 and QingKeV4 at the same time
        template<typename CsrType>
        concept qing_ke_csr_enum_type =
                        std::is_same_v<CsrType, riscv::csr::QingKeV2> ||
                        std::is_same_v<CsrType, riscv::csr::QingKeV3> ||
                        std::is_same_v<CsrType, riscv::csr::QingKeV4>;

        // To confirm the CSR enum value belongs to one of the QingKe enum types
        template<auto CsrEnum>
        concept is_qing_ke_csr_enum =
                        qing_ke_csr_enum_type<decltype(CsrEnum)>;

        // Confirm the CSR's address is within the 12-bit range
        template<auto CsrAddress>
        concept is_csr_valid_address_range = (CsrAddress >= 0U && CsrAddress < (1U << 12U));

        // Confirm one of the QingKe CSRs was used and it's address
        // is withing correct range
        template<auto CsrValue>
        concept is_csr_enum_valid =
                        is_qing_ke_csr_enum<CsrValue> &&
                        is_csr_valid_address_range<static_cast<std::uint16_t>(CsrValue)>;

        template<auto CsrAddress>
        concept is_csr_writable_address = (
                (CsrAddress & riscv::csr::mask_read_write) != static_cast<std::uint16_t>(riscv::csr::ReadWrite::read_only));


        template<auto CsrValue>
        concept is_csr_writable =
                        is_qing_ke_csr_enum<CsrValue> &&
                        is_csr_writable_address<static_cast<std::uint16_t>(CsrValue)>;


        template<typename CsrField>
        concept field_enum_which_contains_field_bit_mask = requires
        {
                { CsrField::field_bit_mask };
        };


        namespace _internal {
                // The CSRs can come different enums, but must resolve to the same enum value
                // CheckSameCsrValue<QingKeV2::intsyscr, QingKeV4::intsyscr> true
                template<bool OmmitCheck, auto LeftValue, auto RightValue>
                concept check_same_csr_value =
                    (OmmitCheck || static_cast<uint16_t>(LeftValue) == static_cast<uint16_t>(RightValue));


                // Internal more generic concept to cover both cases below
                // The CsrField enums need to belong to the same CSR, and when OmmitCheck is false
                // then also the Parent is check if the CsrField enums belong to the parent CSR enum
                template<bool OmmitCheck, auto ParentCsr, auto... CsrFields>
                concept csr_field_enum_matching_csr_generic =
                    (check_same_csr_value<OmmitCheck, ParentCsr, riscv::csr::QingKeV2::intsyscr> && (
                        riscv::csr::intsyscr::is_any_field<decltype(CsrFields)> && ...)) ||
                    (check_same_csr_value<OmmitCheck, ParentCsr, riscv::csr::QingKeV2::mtvec> && (
                        riscv::csr::mtvec::is_any_field<decltype(CsrFields)> && ...)) ||
                    (check_same_csr_value<OmmitCheck, ParentCsr, riscv::csr::QingKeV2::mstatus> && (
                        riscv::csr::mstatus::is_any_field<decltype(CsrFields)> && ...));
        }


        // CsrField(s) need to belong to the same ParentCsr
        template<auto ParentCsr, auto... CsrFields>
        concept same_csr_field_enums_and_matching_parent_csr =
            _internal::csr_field_enum_matching_csr_generic<false, ParentCsr, CsrFields...>;


        // Omit check and provide any CSR as it will be omitted anyway, this
        // makes sure the fields belong to one specidic CSR only, but we
        // do not care which exact CSR it is
        // Note: intsyscr was entered as arbitary CSR, as the OmmitCheck is set to true
        template<auto... CsrFields>
        concept same_csr_field_enums =
            _internal::csr_field_enum_matching_csr_generic<true, riscv::csr::QingKeV2::intsyscr, CsrFields...>;
};
