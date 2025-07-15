//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "ch32v00x/rcc.h"

namespace peripheral::base_addr {

    template<peripheral::rcc::concepts::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return peripheral::rcc::base_addr;
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}
