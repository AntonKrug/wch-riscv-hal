//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"
#include "csr.h"

namespace Riscv::Csr {


    template<auto Csr>
    constexpr auto toAddress() -> std::uint16_t requires Riscv::Concepts::IsCsrEnum<Csr> {
        return static_cast<std::uint16_t>(Csr);
    }

    //https://developer.arm.com/documentation/100748/0613/using-assembly-and-intrinsics-in-c-or-c---code/writing-inline-assembly-code

    template <auto Csr>
    requires Riscv::Concepts::IsCsrValid<Csr>
    constexpr auto readCsr() -> std::uint32_t {
        std::uint32_t result;

        __asm__ volatile(
            "csrr %0, %1"
            : "=r"(result)
            : "i"(static_cast<std::uint16_t>(Csr)));

        return result;
    }

    template <auto Csr, auto value>
    requires Riscv::Concepts::IsCsrValid<Csr>
    constexpr auto writeCsr() -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }

    template <auto Csr>
    requires Riscv::Concepts::IsCsrValid<Csr>
    constexpr auto writeCsr(const unsigned int value) -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }

    template <auto Csr>
    requires Riscv::Concepts::IsCsrValid<Csr>
    constexpr auto writeCsr(const std::uint32_t value) -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }

    template <auto Csr>
    requires Riscv::Concepts::IsCsrValid<Csr>
    constexpr auto writeCsr(const std::uint16_t value) -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }

}


