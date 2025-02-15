//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>
#include <tuple>

#include "system/register/util.h"

namespace Peripheral::Rcc::Ctlr {
    // Clock control

    using namespace Soc::Reg::FieldAccessPrivilege; // so we can use AccessRights::ReadOnly...

    // TODO: check other cpus
    // TODO: RW/RO and other access modes as enum
    enum class HSION_RW_InternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1u,         // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadWrite,

        disable      = 0,            // after disabling HSI, it takes 6 cycles to propagate the change
        enable       = fieldBitMask, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE). v003=24Mhz, x033/x035=48Mhz others=8Mhz
    };

    enum class HSIRDY_RO_InternalHighSpeedClockReady: std::uint32_t {
        fieldBitMask   = 0b1u << 1,    // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = AccessRights::ReadOnly,

        notReady       = 0,            // HSI not ready/stable yet, wait for a bit longer
        readyAndStable = fieldBitMask, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
    };

    enum class HSITRIM_RW_InternalHighSpeedClockTrim: std::uint32_t {
        // Able to superimpose on top of HSICAL[7:0] to adjust HSI frequency. Range is 0-31 60kHz steps, value 16 is the default midpoint
        fieldBitMask = 0b11111u << 3, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadWrite,
    };

    enum class HSICAL_RO_InternalHighSpeedClockCalibration: std::uint32_t {
        fieldBitMask = 0b11111111u << 8, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadOnly,
    };

    enum class HSEON_RW_ExternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 16,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadWrite,

        disable      = 0,            // default, changes take 6 cycles to apply
        enable       = fieldBitMask, // changes take 6 cycles to apply. turned off when entering standby mode,
    };

    enum class HSERDY_RO_ExternalHighSpeedClockReady: std::uint32_t {
        fieldBitMask   = 0b1u << 17,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = AccessRights::ReadOnly,

        notReady       = 0,            // default, HSE not ready/stable yet, wait for a bit longer
        readyAndStable = fieldBitMask, // HSE will be turned off when entering standby mode
    };

    enum class HSEBYP_RW_ExternalHighSpeedClockBypass: std::uint32_t {
        fieldBitMask           = 0b1u << 18,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess            = AccessRights::ReadWrite,

        useCeramicResonator    = 0,            // default, resonate oscillator connected to the external pins, configure this while HSEON is off
        bypassCeramicResonator = fieldBitMask, // consume digital clock signal on external pin, configure this while HSEON is off
    };

    enum class CSSON_RW_ClockSafety: std::uint32_t {
        fieldBitMask = 0b1u << 19,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadWrite,

        noProtection = 0,            // disable protection HSE issues will not cause IRQ or flags to be set
        protectHse   = fieldBitMask, // when HSERDY=1 triggers CSSF and NMI IRQ when HSE abnormal, when HSERDY=0 disable protection
    };

    enum class PLLON_RW_PhaseLockedLoopEnable: std::uint32_t {
        fieldBitMask = 0b1u << 24,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = AccessRights::ReadWrite,

        disable      = 0,            // clock generator is off
        enable       = fieldBitMask, // clock generator is on, but will be disabled when entering standby
    };

    enum class PLLRDY_RO_PhaseLockedLoopReady: std::uint32_t {
        fieldBitMask   = 0b1u << 25,   // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess    = AccessRights::ReadOnly,

        notReady       = 0,            // PLL clock lock NOT achieved yet, wait for a bit longer
        readyAndStable = fieldBitMask, // PLL clock lock achieved
    };

    //TODO: v2x/v3x has pll2on,pll2rdy,pll3on,pll3rdy

    struct metadata {
        static constexpr std::uint32_t offset = 0x00u;

        static constexpr std::tuple<
            HSION_RW_InternalHighSpeedClockEnable,
            HSIRDY_RO_InternalHighSpeedClockReady,
            HSITRIM_RW_InternalHighSpeedClockTrim,
            HSICAL_RO_InternalHighSpeedClockCalibration,
            HSEON_RW_ExternalHighSpeedClockEnable,
            HSERDY_RO_ExternalHighSpeedClockReady,
            HSEBYP_RW_ExternalHighSpeedClockBypass,
            CSSON_RW_ClockSafety,
            PLLON_RW_PhaseLockedLoopEnable,
            PLLRDY_RO_PhaseLockedLoopReady> fields = {};
    };

    template<typename RegField>
    concept IsAnyRegField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(metadata::fields)>();

}