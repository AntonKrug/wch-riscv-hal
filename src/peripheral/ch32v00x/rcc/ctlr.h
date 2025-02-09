//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

#include "system/register/util.h"

namespace Peripheral::Rcc::Ctlr {
    // Clock control

    using namespace Soc::Reg; // so we can use FieldAccessRights::ReadOnly...

    // TODO: check other cpus
    // TODO: RW/RO and other access modes as enum
    enum class HSION_RW_InternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,

        enable       = 0b1, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE). v003=24Mhz, x033/x035=48Mhz others=8Mhz
        disable      = 0    // after disabling HSI, it takes 6 cycles to propagate the change
    };

    enum class HSIRDY_RO_InternalHighSpeedClockReady: std::uint32_t {
        fieldBitMask   = 0b1 << 1, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = FieldAccessRights::ReadOnly,

        readyAndStable = 0b1 << 1, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
        notReady       = 0         // HSI not ready/stable yet, wait for a bit longer
    };

    enum class HSITRIM_RW_InternalHighSpeedClockTrim: std::uint32_t {
        // Able to superimpose on top of HSICAL[7:0] to adjust HSI frequency. Range is 0-31 60kHz steps, value 16 is the default midpoint
        fieldBitMask = 0b11111 << 3, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,
    };

    enum class HSICAL_RO_InternalHighSpeedClockCalibration: std::uint32_t {
        fieldBitMask = 0b11111111 << 8, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadOnly,
    };

    enum class HSEON_RW_ExternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1 << 16, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,

        enable       = 0b1 << 16, // changes take 6 cycles to apply. turned off when entering standby mode,
        disable      = 0          // default, changes take 6 cycles to apply
    };

    enum class HSERDY_RO_ExternalHighSpeedClockReady: std::uint32_t {
        fieldBitMask   = 0b1 << 17, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = FieldAccessRights::ReadOnly,

        readyAndStable = 0b1 << 17, // HSE will be turned off when entering standby mode
        notReady       = 0          // default, HSE not ready/stable yet, wait for a bit longer
    };

    enum class HSEBYP_RW_ExternalHighSpeedClockBypass: std::uint32_t {
        fieldBitMask           = 0b1 << 18, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess            = FieldAccessRights::ReadWrite,

        bypassCeramicResonator = 0b1 << 18, // consume digital clock signal on external pin, configure this while HSEON is off
        useCeramicResonator    = 0          // default, resonate oscillator connected to the external pins, configure this while HSEON is off
    };

    enum class CSSON_RW_ClockSafety: std::uint32_t {
        fieldBitMask = 0b1 << 19, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,

        protectHse   = 0b1 << 19, // when HSERDY=1 triggers CSSF and NMI IRQ when HSE abnormal, when HSERDY=0 disable protection
        noProtection = 0          // disable protection HSE issues will not cause IRQ or flags to be set
    };

    enum class PLLON_RW_PhaseLockedLoopEnable: std::uint32_t {
        fieldBitMask = 0b1 << 24, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,

        enable       = 0b1 << 24, // clock generator is on, but will be disabled when entering standby
        disable      = 0          // clock generator is off
    };

    enum class PLLRDY_RO_PhaseLockedLoopReady: std::uint32_t {
        fieldBitMask   = 0b1 << 25, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = FieldAccessRights::ReadOnly,

        readyAndStable = 0b1 << 25, // PLL clock lock achieved
        notReady       = 0          // PLL clock lock NOT achieved yet, wait for a bit longer
    };

    //TODO: v2x/v3x has pll2on,pll2rdy,pll3on,pll3rdy

    template<typename RegField>
    concept IsAnyRegField =
        std::is_same_v<RegField, HSION_RW_InternalHighSpeedClockEnable> ||
        std::is_same_v<RegField, HSIRDY_RO_InternalHighSpeedClockReady> ||
        std::is_same_v<RegField, HSITRIM_RW_InternalHighSpeedClockTrim> ||
        std::is_same_v<RegField, HSICAL_RO_InternalHighSpeedClockCalibration> ||
        std::is_same_v<RegField, HSEON_RW_ExternalHighSpeedClockEnable> ||
        std::is_same_v<RegField, HSERDY_RO_ExternalHighSpeedClockReady> ||
        std::is_same_v<RegField, HSEBYP_RW_ExternalHighSpeedClockBypass> ||
        std::is_same_v<RegField, CSSON_RW_ClockSafety> ||
        std::is_same_v<RegField, PLLON_RW_PhaseLockedLoopEnable> ||
        std::is_same_v<RegField, PLLRDY_RO_PhaseLockedLoopReady>;

}