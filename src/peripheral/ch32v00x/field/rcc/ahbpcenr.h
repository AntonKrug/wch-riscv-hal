//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Ahbpcenr {
        // Advanced High-performance Bus peripheral clock enable

        constexpr static std::uint32_t regMemOffset = 0x14u;

        enum class DMA1EN_RW_DirectMemoryAccess1Enable: std::uint32_t {
            fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0u,
            clockEnable  = fieldBitMask
        };

        enum class SRAMEN_RW_SramClockDuringSleepEnable: std::uint32_t {
            fieldBitMask = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0u,          // The SRAM clock disabled during sleep
            clockEnable  = fieldBitMask // The SRAM clock enabled during sleep
        };

        constexpr static std::tuple<
            DMA1EN_RW_DirectMemoryAccess1Enable,
            SRAMEN_RW_SramClockDuringSleepEnable
        > regFields = {};
    };

}
