//
// Created by anton on 02/02/2025.
//

#pragma once

//TODO: runtime versions, IRQ safe versions

//https://developer.arm.com/documentation/100748/0623/Using-Assembly-and-Intrinsics-in-C-or-C---Code/Writing-inline-assembly-code?lang=en
//https://developer.arm.com/documentation/102284/6-16-2LTS/armclang-Reference/armclang-Inline-Assembler/Inline-assembly-statements-within-a-function
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Extended-Asm.html
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Simple-Constraints.html
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Machine-Constraints.html
//https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html#csrrwi
//https://uim.fei.stuba.sk/wp-content/uploads/2018/02/riscv-spec-2022.pdf
//https://stackoverflow.com/questions/14449141/the-difference-between-asm-asm-volatile-and-clobbering-memory

#include <cstdint>

#include "concepts.h"


namespace riscv::csr::access_ct {
    // TODO: replace with static assert that this function should be used from compile time section
    template<auto Csr>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    auto
    __attribute__ ((always_inline))
    read_uint32() -> std::uint32_t {
        std::uint32_t result_uin32_t;

        __asm__ volatile(
            "csrr %0, %1"
            : "=r"(result_uin32_t)
            : "i"(static_cast<std::uint16_t>(Csr))
            : "memory"
        );
        return result_uin32_t;
    }


    // TODO: check if IsCsrValueCorrectRegisterType could be needed here, or can be avoided
    template<auto Csr, std::uint32_t ClearValueUint32>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear_uint32() -> void {
        if constexpr (ClearValueUint32 == 0U) {
            // nothing to clear, nothing has to be done here
        } else if constexpr (ClearValueUint32 == 0xffffffffU) {
            // if the clear is full, meaning to clear whole register, it's
            // faster to do a write with 0 instead
            __asm__ volatile(
                "csrw %0, x0" // write 0 is the same as clear all
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr))
                : "memory"
            );
        } else if constexpr (ClearValueUint32 < (1U << 5U)) {
            // is small enough, use the 5-bit immediate instruction instead
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrci %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "K"(ClearValueUint32)
                : "memory"
            );
        } else {
            // use temporary register instead of immediate
            __asm__ volatile(
                "csrc %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "r"(ClearValueUint32)
                : "memory"
            );
        }
    }


    // All the Sets for various types
    template<auto Csr, std::uint32_t SetValueUint32>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set_uint32() -> void {
        if constexpr (SetValueUint32 == 0U) {
            // nothing to set, nothing has to be done here
        } else if constexpr (SetValueUint32 < (1U << 5U)) {
            // is small enough, use the 5-bit immediate instruction instead
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrsi %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "K"(SetValueUint32)
                : "memory"
            );
        } else {
            // use temporary register instead of immediate
            __asm__ volatile(
                "csrs %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "r"(SetValueUint32)
                : "memory"
            );
        }
    }


    // All the writes for various types
    template<auto Csr, std::uint32_t ValueUint32>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write_uint32() -> void {
        if constexpr (ValueUint32 == 0U) {
            // if writtin zero we can use r0 aka x0 register
            __asm__ volatile(
                "csrw %0, x0" // x0(zero) register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr))
                : "memory"
            );
        } else if constexpr (ValueUint32 < (1U << 5U)) {
            // csrwI instruction can take 5-bit imediate value
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrwi %0, %1" // 5-bit immediate values only
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "K"(ValueUint32)
                : "memory"
            );
        } else {
            __asm__ volatile(
                "csrw %0, %1" // value from a register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "r"(ValueUint32)
                : "memory"
            );
        }
    }

    // TODO: at higher level detect if all the writable fields were used and inject
    // the isClearFull to produce write instead
    template<auto Csr, std::uint32_t ClearValueUint32, std::uint32_t SetValueUint32>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear_and_set_uint32() -> void {
        // ReSharper disable CppTooWideScopeInitStatement
        constexpr bool is_clear_small = ClearValueUint32 < (1U << 5U);
        constexpr bool is_clear_zero  = ClearValueUint32 == 0U;
        constexpr bool is_clear_full  = ClearValueUint32 == 0xffffffffU;

        constexpr bool is_set_small = SetValueUint32 < (1U << 5U);
        constexpr bool is_set_zero  = SetValueUint32 == 0U;

        constexpr bool is_clear_set_same = ClearValueUint32 == SetValueUint32;
        // ReSharper restore CppTooWideScopeInitStatement

        if constexpr (is_clear_zero && is_set_zero) {
            // nothing to do, abort
            return;
        }

        if constexpr (is_clear_zero || is_clear_set_same) {
            // nothing to clear, or setting the same value after clearning it:
            // then execute just the set
            set_uint32<Csr, SetValueUint32>();
            return;
        }

        if constexpr (is_set_zero) {
            // nothing to set, execute just the clear part.
            clear_uint32<Csr, ClearValueUint32>();
            return;
        }

        if constexpr (is_clear_full) {
            // no need to clear everything just to set it later, we can use write instead
            write_uint32<Csr, SetValueUint32>();
            return;
        }

        // Both clearing and setting has to be done, but do the minimum amount of instructions
        if constexpr (is_clear_small && is_set_small) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrsi %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "K"(ClearValueUint32),
                "K"(SetValueUint32)
            );
        } else if constexpr (is_clear_small) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrs  %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "K"(ClearValueUint32),
                "r"(SetValueUint32)
            );
        } else if constexpr (is_set_small) {
            __asm__ volatile(
                "csrc  %0, %1\n"
                "csrsi %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "r"(ClearValueUint32),
                "K"(SetValueUint32)
            );
        } else {
            __asm__ volatile(
                "csrc %0, %1\n"
                "csrs %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                "r"(ClearValueUint32),
                "r"(SetValueUint32)
            );
        }
    }
}
