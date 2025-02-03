//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

namespace Peripheral::Rcc {

    // TODO: check other cpus
    enum class HsionInternalHighSpeedClockEnable: std::uint32_t {
        fieldBitMask = 0b1, // not holding any settings or value, it's a bitmask for this specific field
        enable24MHz  = 0b1, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
        disable      = 0    // after disabling HSI, it takes 6 cycles to propagate the change
    };


    enum class HsirdyInternalHighSpeedClockStable: std::uint32_t {
        fieldBitMask     = 0b1'0, // not holding any settings or value, it's a bitmask for this specific field
        isReadyAndStable = 0b1'0, // HSI can set by HW as well (when waking up from standby or when expierencing failure while using HSE)
        notReady         = 0      // HSI not stable enough yet, wait for longer
    };


    enum class HsitrimInternalHighSpeedTrim: std::uint32_t {
        // Able to superimpose on top of HSICAL[7:0] to adjust HSI frequency. Range is 0-31 60kHz steps, 16 is the default midpoint
        fieldBitMask     = 0b11111'0'0'0, // not holding any settings or value, it's a bitmask for this specific field
    };


    template<typename RegisterField>
    concept IsAnyRegisterField =
        std::is_same_v<RegisterField, HsionInternalHighSpeedClockEnable> ||
        std::is_same_v<RegisterField, HsirdyInternalHighSpeedClockStable>
    ;

}