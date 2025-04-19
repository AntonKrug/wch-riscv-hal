//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Rstsckr {
        // Reset status control

        constexpr static std::uint32_t regMemOffset = 0x24U;

        enum class LSION_RW_InternalLowSpeedClockEnable: std::uint32_t {
            fieldBitMask   = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable        = 0U,  // will take 3 cycles for LSIRDY to take effect
            enable         = fieldBitMask // check LSIRDY if it's ready
        };

        enum class LSIRDY_RO_InternalLowSpeedClockSteadyReadyFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 1U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            notReady       = 0U,  // 128kHz LSI clock not steady/ready yet, takes 3 cycles for LSION changes to apply
            readyAndStable = fieldBitMask // 128kHz LSI clock steady/ready, takes 3 cycles for LSION changes to apply
        };

        enum class RMVF_RW_ClearResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 24U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            doNothing      = 0U,  // keep the flag as it is
            clearFlag      = fieldBitMask // clear the reset flag[s]
        };

        enum class PINRSTF_RO_ExternalManualResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 26U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no NRST pin reset occurred, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // NRST pin reset occurred happened, set by HW, cleared by user with RMVF
        };

        enum class PORRSTF_RO_PowerUpDownResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 27U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no power-up/down reset, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class SFTRSTF_RO_SoftwareResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 28U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no software reset, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class IWDGRSTF_RO_IndependentWatchdogResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 29U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no independent watchdog reset, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // independent watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class WWDGRSTF_RO_WindowWatchdogResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 30U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no window watchdog reset, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // window watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class LPWRRSTF_RO_LowPowerResetFlag: std::uint32_t {
            fieldBitMask   = 0b1U << 31U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0U,  // no low power reset, set by HW, cleared by user with RMVF
            resetOccurred  = fieldBitMask // low power reset happened, set by HW, cleared by user with RMVFs
        };

        constexpr static std::tuple<
            LSION_RW_InternalLowSpeedClockEnable,
            LSIRDY_RO_InternalLowSpeedClockSteadyReadyFlag,
            RMVF_RW_ClearResetFlag,
            PINRSTF_RO_ExternalManualResetFlag,
            PORRSTF_RO_PowerUpDownResetFlag,
            SFTRSTF_RO_SoftwareResetFlag,
            IWDGRSTF_RO_IndependentWatchdogResetFlag,
            WWDGRSTF_RO_WindowWatchdogResetFlag,
            LPWRRSTF_RO_LowPowerResetFlag
        > regFields = {};

    };

}
