//
// Created by anton on 30/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"

namespace riscv::csr {

    template<auto Csr>
    requires riscv::concepts::is_qing_ke_csr_enum<Csr>
    constexpr auto toAddress() -> std::uint16_t {
        return static_cast<std::uint16_t>(Csr);
    }

}
