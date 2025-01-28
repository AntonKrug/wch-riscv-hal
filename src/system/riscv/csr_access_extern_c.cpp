//
// Created by anton on 27/01/2025.
//

// #pragma once

// #include "csr_access_for_c.h"
#include "csr_access.h"
#include "csr.h"

// template auto Riscv::Csr::readCsr<Riscv::Csr::QingKeV2::intsyscr>() -> std::uint32_t;
// template auto Riscv::Csr::writeCsr<Riscv::Csr::QingKeV2::intsyscr>(std::uint32_t value) -> void;
// template auto Riscv::Csr::writeCsr<Riscv::Csr::QingKeV2::intsyscr>(const unsigned int value) -> void;


extern "C" {

    __attribute__((always_inline)) inline int riscv_qingke2_read_intsyscr() {
        return Riscv::Csr::readCsr<Riscv::Csr::QingKeV2::intsyscr>();
    }

    __attribute__((always_inline)) inline void riscv_qingke2_write_intsyscr(const unsigned int value) {
        return Riscv::Csr::writeCsr<Riscv::Csr::QingKeV2::intsyscr>(value);
    }

}
