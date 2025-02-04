//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

namespace Peripheral::Rcc::Ctlr {

    // TODO: check other cpus
    enum class HSION_InternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1, // not holding any settings or value, it's a bitmask for this specific field
        enable       = 0b1, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE). v003=24Mhz, x033/x035=48Mhz others=8Mhz
        disable      = 0    // after disabling HSI, it takes 6 cycles to propagate the change
    };


    enum class HSIRDY_InternalHighSpeedClockStable: std::uint32_t {
        fieldBitMask     = 0b1'0, // not holding any settings or value, it's a bitmask for this specific field
        isStableAndReady = 0b1'0, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
        notReady         = 0      // HSI not stable enough yet, wait for longer
    };


    enum class HSITRIM_InternalHighSpeedTrim: std::uint32_t {
        // Able to superimpose on top of HSICAL[7:0] to adjust HSI frequency. Range is 0-31 60kHz steps, 16 is the default midpoint
        fieldBitMask     = 0b11111'0'0'0, // not holding any settings or value, it's a bitmask for this specific field
    };


    template<typename RegField>
    concept IsAnyRegField =
        std::is_same_v<RegField, HSION_InternalHighSpeedClockEnable> ||
        std::is_same_v<RegField, HSIRDY_InternalHighSpeedClockStable> ||
        std::is_same_v<RegField, HSITRIM_InternalHighSpeedTrim>;

}