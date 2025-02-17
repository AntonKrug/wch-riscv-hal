//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Rstsckr {
        // Reset status control

        enum class LSION_RW_InternalLowSpeedClockEnable: std::uint32_t {
            fieldBitMask   = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable        = 0u,  // will take 3 cycles for LSIRDY to take effect
            enabled        = fieldBitMask // check LSIRDY if it's ready
        };

        enum class LSIRDY_RO_InternalLowSpeedClockSteadyReadyFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 1, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            notReady       = 0u,  // 128kHz LSI clock not steady/ready yet, takes 3 cycles for LSION changes to apply
            readyAndStable = fieldBitMask // 128kHz LSI clock steady/ready, takes 3 cycles for LSION changes to apply
        };

        enum class RMVF_RW_ClearResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 24, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            doNothing      = 0u,  // keep the flag as it is
            clearFlag      = fieldBitMask // clear the reset flag[s]
        };

        enum class PINRSTF_RO_ExternalManualResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 26, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no NRST pin reset occured, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // NRST pin reset occured happened, set by HW, cleared by user with RMVF
        };

        enum class PORRSTF_RO_PowerUpDownResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 27, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no power-up/down reset, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class SFTRSTF_RO_SoftwareResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 28, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no software reset, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class IWDGRSTF_RO_IndependentWatchdogResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 29, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no independent watchdog reset, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // independent watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class WWDGRSTF_RO_WindowWatchdogResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 30, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no window watchdog reset, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // window watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class LPWRRSTF_RO_LowPowerResetFlag: std::uint32_t {
            fieldBitMask   = 0b1u << 31, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noReset        = 0u,  // no low power reset, set by HW, cleared by user with RMVF
            resetOccured   = fieldBitMask // low power reset happened, set by HW, cleared by user with RMVFs
        };

        constexpr static std::uint32_t regMemOffset = 0x24u;

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
