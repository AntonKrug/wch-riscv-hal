//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Apb1pcenr {
        // Advanced Peripheral Bus 1 (low speed) peripheral clock enable

        constexpr static std::uint32_t regMemOffset = 0x1cU;

        enum class TIM2EN_RW_Timer2ClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 0, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0U,          // Clock signal is NOT feed to this peripheral
            clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
        };

        enum class WWDGEN_RW_WindowWatchdogClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 11, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0U,          // Clock signal is NOT feed to this peripheral
            clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
        };

        enum class I2C1EN_RW_InterIntegratedCircuitClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 21, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0U,          // Clock signal is NOT feed to this peripheral
            clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
        };

        enum class PWREN_RW_PowerInterfaceModuleClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 28, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noClock      = 0U,          // Clock signal is NOT feed to this peripheral
            clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
        };

        constexpr static std::tuple<
            TIM2EN_RW_Timer2ClockEnable,
            WWDGEN_RW_WindowWatchdogClockEnable,
            I2C1EN_RW_InterIntegratedCircuitClockEnable,
            PWREN_RW_PowerInterfaceModuleClockEnable
        > regFields = {};

    };

}
