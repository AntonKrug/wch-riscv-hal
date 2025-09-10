//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Apb1pcenr { //NOLINT
        // Advanced Peripheral Bus 1 (low speed) peripheral clock enable

        constexpr static std::uint32_t reg_mem_offset = 0x1cU;

        enum class TIM2EN_RW_Timer2ClockEnable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,          // Clock signal is NOT feed to this peripheral
            clock_enable   = field_bit_mask // Clock signal is feed to this peripheral
        };

        enum class WWDGEN_RW_WindowWatchdogClockEnable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,          // Clock signal is NOT feed to this peripheral
            clock_enable   = field_bit_mask // Clock signal is feed to this peripheral
        };

        enum class I2C1EN_RW_InterIntegratedCircuitClockEnable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 21U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,          // Clock signal is NOT feed to this peripheral
            clock_enable   = field_bit_mask // Clock signal is feed to this peripheral
        };

        enum class PWREN_RW_PowerInterfaceModuleClockEnable: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 28U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_clock       = 0U,          // Clock signal is NOT feed to this peripheral
            clock_enable   = field_bit_mask // Clock signal is feed to this peripheral
        };

        constexpr static std::tuple<
            TIM2EN_RW_Timer2ClockEnable,
            WWDGEN_RW_WindowWatchdogClockEnable,
            I2C1EN_RW_InterIntegratedCircuitClockEnable,
            PWREN_RW_PowerInterfaceModuleClockEnable
        > reg_fields = {};

    };

}
