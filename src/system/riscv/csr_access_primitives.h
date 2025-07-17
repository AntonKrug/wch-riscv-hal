//
// Created by anton on 17/07/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"

namespace riscv::csr::access {

    template<auto Csr>
        requires riscv::concepts::is_csr_enum_valid<Csr>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write_uint32(const std::uint32_t value_uint32) -> void {
        __asm__ volatile(
            "csrw %0, %1" // value from a register
            : // no output
            : "i"(static_cast<std::uint16_t>(Csr)),
            "r"(value_uint32)
        );
    }

}

