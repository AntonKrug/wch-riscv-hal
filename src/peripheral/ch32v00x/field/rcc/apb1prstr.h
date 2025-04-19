//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Apb1prstr {
        // Advanced Peripheral Bus 1 (low speed) peripheral reset register

        constexpr static std::uint32_t regMemOffset = 0x10U;

        enum class TIM2RST_RW_Timer2Reset: std::uint32_t {
            fieldBitMask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0U,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class WWDGRST_RW_WindowWatchdogReset: std::uint32_t {
            fieldBitMask = 0b1U << 11U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0U,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class I2C1RST_RW_WindowWatchdogReset: std::uint32_t {
            fieldBitMask = 0b1U << 21U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0U,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class PWRRST_RW_PowerInterfaceModuleReset: std::uint32_t {
            fieldBitMask = 0b1U << 28U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0U,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        constexpr static std::tuple<
            TIM2RST_RW_Timer2Reset,
            WWDGRST_RW_WindowWatchdogReset,
            I2C1RST_RW_WindowWatchdogReset,
            PWRRST_RW_PowerInterfaceModuleReset
        > regFields = {};

    };

}
