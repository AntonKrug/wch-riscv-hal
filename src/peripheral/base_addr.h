//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "ch32v00x/rcc.h"

namespace Peripheral::BaseAddr {

    template<Peripheral::Rcc::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return Peripheral::Rcc::baseAddr;
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}
