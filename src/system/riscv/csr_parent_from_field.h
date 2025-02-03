//
// Created by anton on 02/02/2025.
//

#pragma once


#include "concepts.h"


namespace Riscv::Csr::AccessCt {


    template<Riscv::Csr::Intsyscr::IsAnyField... CsrFields>
    constexpr auto getCsrFromField(CsrFields... csrFields) {
        return Riscv::Csr::QingKeV2::intsyscr;
    }


    template<Riscv::Csr::Mtvec::IsAnyField... CsrFields>
    constexpr auto getCsrFromField(CsrFields... csrFields) {
        return Riscv::Csr::QingKeV2::mtvec;
    }


    template<Riscv::Csr::Mstatus::IsAnyField... CsrFields>
    constexpr auto getCsrFromField(CsrFields... csrFields) {
        return Riscv::Csr::QingKeV2::mstatus;
    }


}