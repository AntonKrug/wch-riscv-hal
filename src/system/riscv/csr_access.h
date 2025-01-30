//
// Created by anton on 27/01/2025.
//

//https://developer.arm.com/documentation/100748/0623/Using-Assembly-and-Intrinsics-in-C-or-C---Code/Writing-inline-assembly-code?lang=en
//https://developer.arm.com/documentation/102284/6-16-2LTS/armclang-Reference/armclang-Inline-Assembler/Inline-assembly-statements-within-a-function
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Extended-Asm.html
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Simple-Constraints.html
//https://gcc.gnu.org/onlinedocs/gcc-12.4.0/gcc/Machine-Constraints.html
//https://msyksphinz-self.github.io/riscv-isadoc/html/rvi.html#csrrwi
//https://uim.fei.stuba.sk/wp-content/uploads/2018/02/riscv-spec-2022.pdf

#pragma once

#include <cstdint>

#include "concepts.h"
#include "csr_utils.h"


namespace Riscv::Csr::Access {


    using namespace Riscv;


    template <auto Csr>
    requires Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    read() -> std::uint32_t {
        std::uint32_t result;

        __asm__ volatile(
            "csrr %0, %1"
            : "=r"(result)
            : "i"(static_cast<std::uint16_t>(Csr)));

        return result;
    }


    template <auto Csr, auto Clear, auto Set>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::IsCsrValueCorrectRegisterType<Clear> &&
             Concepts::IsCsrValueCorrectRegisterType<Set>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clearAndSet() -> void {
        // ReSharper disable CppTooWideScopeInitStatement
        constexpr bool isSmallClear = Clear < (1u<<5);
        constexpr bool isSmallSet   = Set   < (1u<<5);
        // ReSharper restore CppTooWideScopeInitStatement

        // For all possible combinations use the smallest instructions
        if (isSmallClear && isSmallSet) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrsi %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(Clear),
                  "K"(Set)
            );
        } else if (isSmallClear) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrs  %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(Clear),
                  "i"(Set)
            );
        } else if (isSmallSet) {
            __asm__ volatile(
                "csrc  %0, %1\n"
                "csrsi %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "i"(Clear),
                  "K"(Set)
            );
        } else {
            __asm__ volatile(
                "csrc %0, %1\n"
                "csrs %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "i"(Clear),
                  "i"(Set)
            );
        }
    }


    template <auto Csr, auto ClearValue>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::IsCsrValueCorrectRegisterType<ClearValue>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
        if (ClearValue < (1u<<5)) {
            // is small enough, use the 5-bit immediate instruction instead
            __asm__ volatile(
                "csrci %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(ClearValue)
            );
        } else {
            __asm__ volatile(
                "csrc %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(ClearValue)
            );
        }
    }


    // All the Sets for various types
    template <auto Csr, auto SetValue>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::IsCsrValueCorrectRegisterType<SetValue>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        if (SetValue < (1u<<5)) {
            // is small enough, use the 5-bit imediate instruction instead
            __asm__ volatile(
                "csrsi %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(SetValue)
            );
        } else {
            __asm__ volatile(
                "csrs %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(SetValue)
            );
        }
    }


    // All the writes for various types
    template <auto Csr, auto Value>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::IsCsrValueCorrectRegisterType<Value>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html

        if (Value == 0) {
            // if writtin zero we can use r0 aka x0 register
            __asm__ volatile(
                "csrw %0, x0"  // x0(zero) register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr))
            );
        } else if (Value<(1u<<5)) {
            // csrwI instruction can take 5-bit imediate value
            __asm__ volatile(
                "csrwi %0, %1" // 5-bit immediate values only
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(Value)
            );
        } else {
            __asm__ volatile(
                "csrw %0, %1" // value from a register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(Value)
            );
        }
    }


    // All the writes for various types
    template <auto Csr, auto... Values>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::CsrFieldEnumMatchingCsr<false, Csr, Values...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        constexpr std::uint32_t value = (static_cast<std::uint32_t>(Values) | ...);
        write<Csr, value>();
    }


    // When only specific fields needs to be updated (it will for various types
    template <auto Csr, auto... Values>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::CsrFieldEnumMatchingCsr<false, Csr, Values...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr std::uint32_t clearValue = Csr::getMaskFromFieldEnumValues<Values...>();
        constexpr std::uint32_t setValue   = (static_cast<std::uint32_t>(Values) | ...);

        constexpr bool clearHasValue = clearValue != 0;
        constexpr bool setHasValue   = setValue   != 0;

        // simplify the operation depending if the set and clear are empty or if they are the same value
        if (clearHasValue) {
            // Mask is not empty

            if (setHasValue) {
                if (clearValue==setValue) {
                    // if clearning and setting is the same, simplify just to setting
                    set<Csr, setValue>();
                } else {
                    clearAndSet<Csr, clearValue, setValue>();
                }
            } else {
                clear<Csr, clearValue>();
            }
        } else {
            // Mask empty

            if (setHasValue) {
                set<Csr, setValue>();
            } else {
                // mask and value empty, do nothing
            }
        }
    }


}


