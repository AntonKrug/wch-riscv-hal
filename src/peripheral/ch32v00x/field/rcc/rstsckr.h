//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Rstsckr { // NOLINT
        // Reset status control

        constexpr static std::uint32_t reg_mem_offset = 0x24U;

        enum class LSION_RW_InternalLowSpeedClockEnable: std::uint32_t { // NOLINT
            field_bit_mask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            disable        = 0U,            // will take 3 cycles for LSIRDY to take effect
            enable         = field_bit_mask // check LSIRDY if it's ready
        };

        enum class LSIRDY_RO_InternalLowSpeedClockSteadyReadyFlag: std::uint32_t { // NOLINT
            field_bit_mask   = 0b1U << 1U, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadOnly,

            not_ready        = 0U,            // 128kHz LSI clock not steady/ready yet, takes 3 cycles for LSION changes to apply
            ready_and_stable = field_bit_mask // 128kHz LSI clock steady/ready, takes 3 cycles for LSION changes to apply
        };

        enum class RMVF_RW_ClearResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 24U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadWrite,

            do_nothing      = 0U,            // keep the flag as it is
            clear_flag      = field_bit_mask // clear the reset flag[s]
        };

        enum class PINRSTF_RO_ExternalManualResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 26U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no NRST pin reset occurred, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // NRST pin reset occurred happened, set by HW, cleared by user with RMVF
        };

        enum class PORRSTF_RO_PowerUpDownResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 27U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no power-up/down reset, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class SFTRSTF_RO_SoftwareResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 28U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no software reset, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // power-up/down reset happened, set by HW, cleared by user with RMVFs
        };

        enum class IWDGRSTF_RO_IndependentWatchdogResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 29U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no independent watchdog reset, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // independent watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class WWDGRSTF_RO_WindowWatchdogResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 30U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no window watchdog reset, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // window watchdog reset happened, set by HW, cleared by user with RMVFs
        };

        enum class LPWRRSTF_RO_LowPowerResetFlag: std::uint32_t { // NOLINT
            field_bit_mask  = static_cast<std::uint32_t>(0b1U) << 31U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_reset        = 0U,            // no low power reset, set by HW, cleared by user with RMVF
            reset_occurred  = field_bit_mask // low power reset happened, set by HW, cleared by user with RMVFs
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
        > reg_fields = {};

    };

}
