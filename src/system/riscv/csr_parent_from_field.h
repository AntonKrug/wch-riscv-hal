//
// Created by anton on 02/02/2025.
//

#pragma once


#include "concepts.h"


namespace Riscv::Csr::AccessCt {


    template<Riscv::Csr::Intsyscr::IsAnyField Field>
    constexpr auto getCsrFromField(Field field) {
        return Riscv::Csr::QingKeV2::intsyscr;
    }


    template<Riscv::Csr::Mtvec::IsAnyField... Field>
    constexpr auto getCsrFromField(Field... field) {
        return Riscv::Csr::QingKeV2::mtvec;
    }


    template<Riscv::Csr::Mstatus::IsAnyField... Field>
    constexpr auto getCsrFromField(Field... field) {
        return Riscv::Csr::QingKeV2::mstatus;
    }


}