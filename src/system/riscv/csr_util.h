//
// Created by anton on 30/01/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"

namespace Riscv::Csr {

    template<auto Csr>
    requires Riscv::Concepts::IsQingKeCsrEnum<Csr>
    constexpr auto toAddress() -> std::uint16_t {
        return static_cast<std::uint16_t>(Csr);
    }

}
