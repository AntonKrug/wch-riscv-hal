//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Ahbpcenr {
        // Advanced High-performance Bus peripheral clock enable

        constexpr static std::uint32_t reg_mem_offset = 0x14U;

        enum class DMA1EN_RW_DirectMemoryAccess1Enable: std::uint32_t {
            fieldBitMask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class SRAMEN_RW_SramClockDuringSleepEnable: std::uint32_t {
            fieldBitMask = 0b1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,          // The SRAM clock disabled during sleep
            clockEnable  = fieldBitMask // The SRAM clock enabled during sleep
        };

        constexpr static std::tuple<
            DMA1EN_RW_DirectMemoryAccess1Enable,
            SRAMEN_RW_SramClockDuringSleepEnable
        > reg_fields = {};
    };

}
