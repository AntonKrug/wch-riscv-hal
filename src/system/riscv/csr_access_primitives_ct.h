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

#pragma once

#include <cstdint>

#include "concepts.h"


namespace Riscv::Csr::AccessCt {


    template <auto Csr>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    readUint32() -> std::uint32_t {
        std::uint32_t resultUin32t;

        __asm__ volatile(
            "csrr %0, %1"
            : "=r"(resultUin32t)
            : "i"(static_cast<std::uint16_t>(Csr)));

        return resultUin32t;
    }


    // TODO: check if IsCsrValueCorrectRegisterType could be needed here, or can be avoided
    template <auto Csr, std::uint32_t ClearValueUint32>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clearUint32() -> void {
        if (ClearValueUint32 == 0) {
            // nothing to clear, nothing has to be done here
        } else if (ClearValueUint32 < (1u<<5)) {
            // is small enough, use the 5-bit immediate instruction instead
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrci %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(ClearValueUint32)
            );
        } else {
            // use temporary registor instead of immediate
            __asm__ volatile(
                "csrc %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(ClearValueUint32)
            );
        }
    }


    // All the Sets for various types
    template <auto Csr, std::uint32_t SetValueUint32>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setUint32() -> void {
        if (SetValueUint32 == 0) {
            // nothing to set, nothing has to be done here
        } else if (SetValueUint32 < (1u<<5)) {
            // is small enough, use the 5-bit immediate instruction instead
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrsi %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(SetValueUint32)
            );
        } else {
            // use temporary registor instead of immediate
            __asm__ volatile(
                "csrs %0, %1"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(SetValueUint32)
            );
        }
    }


    // All the writes for various types
    template <auto Csr, auto ValueUint32>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    writeUint32() -> void {
        if (ValueUint32 == 0) {
            // if writtin zero we can use r0 aka x0 register
            __asm__ volatile(
                "csrw %0, x0"  // x0(zero) register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr))
            );
        } else if (ValueUint32<(1u<<5)) {
            // csrwI instruction can take 5-bit imediate value
            // https://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html
            __asm__ volatile(
                "csrwi %0, %1" // 5-bit immediate values only
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(ValueUint32)
            );
        } else {
            __asm__ volatile(
                "csrw %0, %1" // value from a register
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "r"(ValueUint32)
            );
        }
    }


    // TODO: at higher level detect if all the writable fields were used and inject
    // the isClearFull to produce write instead
    template <auto Csr, std::uint32_t ClearValueUint32, std::uint32_t SetValueUint32>
    requires Riscv::Concepts::IsCsrEnumValid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clearAndSetUint32() -> void {
        // ReSharper disable CppTooWideScopeInitStatement
        constexpr bool isClearSmall = ClearValueUint32 <  (1u<<5);
        constexpr bool isClearZero  = ClearValueUint32 == 0;
        constexpr bool isClearFull  = ClearValueUint32 == 0xffffffff;

        constexpr bool isSetSmall   = SetValueUint32   <  (1u<<5);
        constexpr bool isSetZero    = SetValueUint32   == 0;
        // ReSharper restore CppTooWideScopeInitStatement

        if (isClearZero && isSetZero) {
            // nothing to do, abort
            return;
        }

        if (isClearZero) {
            // nothing to clear, execute just the set
            setUint32<Csr, SetValueUint32>();
            return;
        }

        if (isSetZero) {
            // nothing to set, execute just the clear
            clearUint32<Csr, ClearValueUint32>();
            return;
        }

        if (isClearFull) {
            // no need to clear everything just to set it later, we can use write instead
            writeUint32<Csr, SetValueUint32>();
            return;
        }

        // Both clearing and setting has to be done, but do the minimum amount of instructions
        if (isClearSmall && isSetSmall) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrsi %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(ClearValueUint32),
                  "K"(SetValueUint32)
            );
        } else if (isClearSmall) {
            __asm__ volatile(
                "csrci %0, %1\n"
                "csrs  %0, %2"
                : // no output
                : "i"(static_cast<std::uint16_t>(Csr)),
                  "K"(ClearValueUint32),
                  "r"(SetValueUint32)
            );
        } else if (isSetSmall) {
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