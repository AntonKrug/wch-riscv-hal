//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"
#include "csr.h"

namespace Riscv::Csr {


    template<auto Csr>
    constexpr auto toAddress() -> std::uint16_t requires Riscv::Concepts::IsCsrEnumType<Csr> {
        return static_cast<std::uint16_t>(Csr);
    }


    template<auto Enum>
    constexpr auto toValue() -> std::uint16_t requires Riscv::Concepts::IsCsrMaskedEnums<Enum> {
        return static_cast<std::uint16_t>(Enum);
    }


    //https://developer.arm.com/documentation/100748/0613/using-assembly-and-intrinsics-in-c-or-c---code/writing-inline-assembly-code
    template <auto Csr>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    readCsr() -> std::uint32_t {
        std::uint32_t result;

        __asm__ volatile(
            "csrr %0, %1"
            : "=r"(result)
            : "i"(static_cast<std::uint16_t>(Csr)));

        return result;
    }

    // All the writes for various types
    template <auto Csr, auto value>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::IsCsrValueCorrectType<value>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    writeCsr() -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }


    // For zeroing writes
    template <auto Csr, auto value>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::IsCsrValueCorrectType<value> &&
             Riscv::Concepts::IsCsrValueZero<value>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    writeCsr() -> void {
        __asm__ volatile(
            "csrw %0, x0"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr))
        );
    }

    // template <auto Csr, auto value>
    // requires Riscv::Concepts::IsCsrEnumValid<Csr>
    // inline
    // constexpr auto
    // __attribute__ ((always_inline))
    // writeCsr() -> void {
    //     __asm__ volatile(
    //         "csrw %0, %1"
    //         : // no output
    //         : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
    //     );
    // }
    //
    // template <auto Csr>
    // requires Riscv::Concepts::IsCsrEnumValid<Csr>
    // inline
    // constexpr auto
    // __attribute__ ((always_inline))
    // writeCsr(const unsigned int value) -> void {
    //     __asm__ volatile(
    //         "csrw %0, %1"
    //         : // no output
    //         : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
    //     );
    // }
    //
    // template <auto Csr>
    // requires Riscv::Concepts::IsCsrEnumValid<Csr>
    // inline
    // constexpr auto
    // __attribute__ ((always_inline))
    // writeCsr(const std::uint32_t value) -> void {
    //     __asm__ volatile(
    //         "csrw %0, %1"
    //         : // no output
    //         : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
    //     );
    // }

    template <auto Csr>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    writeCsr(const std::uint16_t value) -> void {
        __asm__ volatile(
            "csrw %0, %1"
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)), "r"(value)
        );
    }

}


