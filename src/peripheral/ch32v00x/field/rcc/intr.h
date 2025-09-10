//
// Created by anton on 06/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Intr { // NOLINT
        // Clock Interrupt

        constexpr static std::uint32_t reg_mem_offset = 0x08U;

        enum class LSIRDYF_RO_InternalLowSpeedReadyFlag: std::uint32_t { // NOLINT
            field_bit_mask  = 1U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt    = 0U, // default
            ready_interrupt = field_bit_mask, // set by hw, to clear write LSIRDYC
        };

        enum class HSIRDYF_RO_InternalHighSpeedReadyFlag: std::uint32_t { // NOLINT
            field_bit_mask  = 1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt    = 0U, // default
            ready_interrupt = field_bit_mask, // set by hw, to clear write HSIRDYC
        };

        enum class HSERDYF_RO_ExternalHighSpeedReadyFlag: std::uint32_t { // NOLINT
            field_bit_mask  = 1U << 3U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt    = 0U, // default
            ready_interrupt = field_bit_mask, // set by hw, to clear write HSERDYC
        };

        enum class PLLRDYF_RO_PhaseLockedLoopReadyFlag: std::uint32_t { // NOLINT
            field_bit_mask  = 1U << 4U, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt    = 0U, // default
            ready_interrupt = field_bit_mask, // set by hw, to clear write PLLRDYC
        };

        // 2 bits reserved

        enum class CSSF_RO_ExternalHighSpeedSecurityFlag: std::uint32_t { // NOLINT
            field_bit_mask    = 1U << 7U, // not holding any settings or value, it's a bitmask for this specific field
            field_access      = soc::reg::field_access_right::ReadOnly,

            no_interrupt      = 0U, // default
            failure_interrupt = field_bit_mask, // set by hw, to clear write CSSC
        };

        enum class LSIRDYIE_RW_InternalLowSpeedReadyInterruptEnable: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 8U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            disable        = 0U, // default
            enable         = field_bit_mask,
        };

        // 1 bit reserved

        enum class HSIRDYIE_RW_InternalHighSpeedReadyInterruptEnable: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 10U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            disable        = 0U, // default
            enable         = field_bit_mask,
        };

        enum class HSERDYIE_RW_ExternalHighSpeedReadyInterruptEnable: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            disable        = 0U, // default
            enable         = field_bit_mask,
        };

        enum class PLLRDYIE_RW_PhaseLockedLoopReadyInterruptEnable: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 12U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            disable        = 0U, // default
            enable         = field_bit_mask,
        };

        // 3bits reserved

        enum class LSIRDYC_WO_InternalLowSpeedReadyClear: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 16U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::WriteOnly,

            no_action      = 0U, // default
            clear_flag     = field_bit_mask,
        };

        // 1bit reserved

        enum class HSIRDYC_WO_InternalHighSpeedReadyClear: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 18U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::WriteOnly,

            no_action      = 0U, // default
            clear_flag     = field_bit_mask,
        };

        enum class HSERDYC_WO_ExternalHighSpeedReadyClear: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 19U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::WriteOnly,

            no_action      = 0U, // default
            clear_flag     = field_bit_mask,
        };

        enum class PLLRDYC_WO_PhaseLockedLoopReadyClear: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 20U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::WriteOnly,

            no_action      = 0U, // default
            clear_flag     = field_bit_mask,
        };

        // 2bits reserved

        enum class CSSC_WO_ExternalHighSpeedSecurityClear: std::uint32_t { // NOLINT
            field_bit_mask = 1U << 23U,  // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::WriteOnly,

            no_action      = 0U, // default
            clear_flag     = field_bit_mask,
        };

        // 31-24 reserved

        constexpr static std::tuple<
            LSIRDYF_RO_InternalLowSpeedReadyFlag,
            HSIRDYF_RO_InternalHighSpeedReadyFlag,
            HSERDYF_RO_ExternalHighSpeedReadyFlag,
            PLLRDYF_RO_PhaseLockedLoopReadyFlag,
            CSSF_RO_ExternalHighSpeedSecurityFlag,
            LSIRDYIE_RW_InternalLowSpeedReadyInterruptEnable,
            HSIRDYIE_RW_InternalHighSpeedReadyInterruptEnable,
            HSERDYIE_RW_ExternalHighSpeedReadyInterruptEnable,
            PLLRDYIE_RW_PhaseLockedLoopReadyInterruptEnable,
            LSIRDYC_WO_InternalLowSpeedReadyClear,
            HSIRDYC_WO_InternalHighSpeedReadyClear,
            HSERDYC_WO_ExternalHighSpeedReadyClear,
            PLLRDYC_WO_PhaseLockedLoopReadyClear,
            CSSC_WO_ExternalHighSpeedSecurityClear
        > reg_fields = {};

    };

}
