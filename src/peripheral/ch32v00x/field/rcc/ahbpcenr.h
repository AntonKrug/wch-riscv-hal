//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Ahbpcenr {
        // Advanced High-performance Bus peripheral clock enable

        enum class DMA1EN_RW_DirectMemoryAccess1Enable: std::uint32_t {
            fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0u,
            clockEnable  = fieldBitMask
        };

        enum class SRAMEN_RW_SRAM_Clock_Durning_Sleep_Enable: std::uint32_t {
            fieldBitMask = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0u,          // The SRAM clock disabled durning sleep
            clockEnable  = fieldBitMask // The SRAM clock enabled durning sleep
        };

        constexpr static std::uint32_t regMemOffset = 0x14u;

        constexpr static std::tuple<
            DMA1EN_RW_DirectMemoryAccess1Enable,
            SRAMEN_RW_SRAM_Clock_Durning_Sleep_Enable
        > regFields = {};
    };

}
