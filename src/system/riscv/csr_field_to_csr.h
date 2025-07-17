//
// Created by anton on 02/02/2025.
//

#pragma once


#include "concepts.h"


namespace riscv::csr::access_ct {


    template<riscv::csr::intsyscr::is_any_field... CsrFields>
    constexpr auto get_csr_from_field(CsrFields... csrFields) {
        return riscv::csr::QingKeV2::intsyscr;
    }


    template<riscv::csr::mtvec::is_any_field... CsrFields>
    constexpr auto get_csr_from_field(CsrFields... csrFields) {
        return riscv::csr::QingKeV2::mtvec;
    }


    template<riscv::csr::mstatus::is_any_field... CsrFields>
    constexpr auto get_csr_from_field(CsrFields... csrFields) {
        return riscv::csr::QingKeV2::mstatus;
    }


}