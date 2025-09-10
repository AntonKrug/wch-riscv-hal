//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Apb1prstr { //NOLINT
        // Advanced Peripheral Bus 1 (low speed) peripheral reset register

        constexpr static std::uint32_t reg_mem_offset = 0x10U;

        enum class TIM2RST_RW_Timer2Reset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class WWDGRST_RW_WindowWatchdogReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class I2C1RST_RW_WindowWatchdogReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 21U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class PWRRST_RW_PowerInterfaceModuleReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 28U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        constexpr static std::tuple<
            TIM2RST_RW_Timer2Reset,
            WWDGRST_RW_WindowWatchdogReset,
            I2C1RST_RW_WindowWatchdogReset,
            PWRRST_RW_PowerInterfaceModuleReset
        > reg_fields = {};

    };

}
