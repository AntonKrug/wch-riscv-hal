//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Ahbpcenr { //NOLINT
        // Advanced High-performance Bus peripheral clock enable

        constexpr static std::uint32_t reg_mem_offset = 0x14U;

        enum class DMA1EN_RW_DirectMemoryAccess1Enable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,
            clock_enable   = field_bit_mask
        };

        enum class SRAMEN_RW_SramClockDuringSleepEnable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,          // The SRAM clock disabled during sleep
            clock_enable   = field_bit_mask // The SRAM clock enabled during sleep
        };

        constexpr static std::tuple<
            DMA1EN_RW_DirectMemoryAccess1Enable,
            SRAMEN_RW_SramClockDuringSleepEnable
        > reg_fields = {};
    };

}
