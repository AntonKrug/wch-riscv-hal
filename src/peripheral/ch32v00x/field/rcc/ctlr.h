//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Ctlr {
        // Clock control

        constexpr static std::uint32_t regMemOffset = 0x00U;

        // TODO: check other cpus
        // TODO: RW/RO and other access modes as enum
        enum class HSION_RW_InternalHighSpeedClockEnable: std::uint32_t {
            fieldBitMask = 0b1U,         // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            disable      = 0U,           // after disabling HSI, it takes 6 cycles to propagate the change
            enable       = fieldBitMask, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE). v003=24Mhz, x033/x035=48Mhz others=8Mhz
        };

        enum class HSIRDY_RO_InternalHighSpeedClockReady: std::uint32_t {
            fieldBitMask   = 0b1U << 1,    // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            notReady       = 0U,           // HSI not ready/stable yet, wait for a bit longer
            readyAndStable = fieldBitMask, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
        };

        enum class HSITRIM_RW_InternalHighSpeedClockTrim: std::uint32_t {
            // Able to superimpose on top of HSICAL[7:0] to adjust HSI frequency. Range is 0-31 60kHz steps, value 16 is the default midpoint
            // HSITRIM=0  coresponds to -960 kHz
            // HSITRIM=1  coresponds to -900 kHz
            // HSITRIM=2  coresponds to -840 kHz
            // HSITRIM=3  coresponds to -780 kHz
            // HSITRIM=4  coresponds to -720 kHz
            // HSITRIM=5  coresponds to -660 kHz
            // HSITRIM=6  coresponds to -600 kHz
            // HSITRIM=7  coresponds to -540 kHz
            // HSITRIM=8  coresponds to -480 kHz
            // HSITRIM=9  coresponds to -420 kHz
            // HSITRIM=10 coresponds to -360 kHz
            // HSITRIM=11 coresponds to -300 kHz
            // HSITRIM=12 coresponds to -240 kHz
            // HSITRIM=13 coresponds to -180 kHz
            // HSITRIM=14 coresponds to -120 kHz
            // HSITRIM=15 coresponds to  -60 kHz
            // HSITRIM=16 coresponds to    0 kHz
            // HSITRIM=17 coresponds to   60 kHz
            // HSITRIM=18 coresponds to  120 kHz
            // HSITRIM=19 coresponds to  180 kHz
            // HSITRIM=20 coresponds to  240 kHz
            // HSITRIM=21 coresponds to  300 kHz
            // HSITRIM=22 coresponds to  360 kHz
            // HSITRIM=23 coresponds to  420 kHz
            // HSITRIM=24 coresponds to  480 kHz
            // HSITRIM=25 coresponds to  540 kHz
            // HSITRIM=26 coresponds to  600 kHz
            // HSITRIM=27 coresponds to  660 kHz
            // HSITRIM=28 coresponds to  720 kHz
            // HSITRIM=29 coresponds to  780 kHz
            // HSITRIM=30 coresponds to  840 kHz
            // HSITRIM=31 coresponds to  900 kHz

            fieldBitMask = 0b11111U << 3, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,
        };

        template<std::uint8_t Trim>
        constexpr static auto produceRawTrimValueCt() -> std::uint32_t {
            static_assert(Trim < 32U, "Trim is 5-bits, the value must be in range 0-31");
            return Trim << Soc::Reg::enumBitMaskOffsetCt<HSITRIM_RW_InternalHighSpeedClockTrim>();
        }

        enum class HSICAL_RO_InternalHighSpeedClockCalibration: std::uint32_t {
            fieldBitMask = 0b11111111U << 8U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadOnly,
        };

        enum class HSEON_RW_ExternalHighSpeedClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 16U,   // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            disable      = 0U,           // default, changes take 6 cycles to apply
            enable       = fieldBitMask, // changes take 6 cycles to apply. turned off when entering standby mode,
        };

        enum class HSERDY_RO_ExternalHighSpeedClockReady: std::uint32_t {
            fieldBitMask   = 0b1U << 17U,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            notReady       = 0U,           // default, HSE not ready/stable yet, wait for a bit longer
            readyAndStable = fieldBitMask, // HSE will be turned off when entering standby mode
        };

        enum class HSEBYP_RW_ExternalHighSpeedClockBypass: std::uint32_t {
            fieldBitMask           = 0b1U << 18U,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess            = Soc::Reg::FieldAccessRight::ReadWrite,

            useCeramicResonator    = 0U,           // default, resonate oscillator connected to the external pins, configure this while HSEON is off
            bypassCeramicResonator = fieldBitMask, // consume digital clock signal on external pin, configure this while HSEON is off
        };

        enum class CSSON_RW_ClockSafety: std::uint32_t {
            fieldBitMask = 0b1U << 19U,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noProtection = 0U,           // disable protection HSE issues will not cause IRQ or flags to be set
            protectHse   = fieldBitMask, // when HSERDY=1 triggers CSSF and NMI IRQ when HSE abnormal, when HSERDY=0 disable protection
        };

        enum class PLLON_RW_PhaseLockedLoopEnable: std::uint32_t {
            fieldBitMask = 0b1U << 24U,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            disable      = 0U,           // clock generator is off
            enable       = fieldBitMask, // clock generator is on, but will be disabled when entering standby
        };

        enum class PLLRDY_RO_PhaseLockedLoopReady: std::uint32_t {
            fieldBitMask   = 0b1U << 25U,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            notReady       = 0U,           // PLL clock lock NOT achieved yet, wait for a bit longer
            readyAndStable = fieldBitMask, // PLL clock lock achieved
        };

        //TODO: v2x/v3x has pll2on,pll2rdy,pll3on,pll3rdy

        constexpr static std::tuple<
            HSION_RW_InternalHighSpeedClockEnable,
            HSIRDY_RO_InternalHighSpeedClockReady,
            HSITRIM_RW_InternalHighSpeedClockTrim,
            HSICAL_RO_InternalHighSpeedClockCalibration,
            HSEON_RW_ExternalHighSpeedClockEnable,
            HSERDY_RO_ExternalHighSpeedClockReady,
            HSEBYP_RW_ExternalHighSpeedClockBypass,
            CSSON_RW_ClockSafety,
            PLLON_RW_PhaseLockedLoopEnable,
            PLLRDY_RO_PhaseLockedLoopReady
        > regFields = {};

    };

}