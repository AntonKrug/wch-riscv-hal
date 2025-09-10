//
// Created by anton on 16/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "../../../../system/register/field_access_privilege.h"

namespace peripheral::dma {

    struct Intfr { // NOLINT
        // Flags are set when events happen regardless if the IRQ was enabled or not.
        // NOTE: these fields can't be used directly as is, as the real registers have many instances of these fields repeated

        enum class GIF_RO_GlobalInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 0U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadOnly,

            no_interrupt       = 0U,             // No interrupt occurred
            interrupt_occurred = field_bit_mask, // Can be queried to find out which channel experienced IRQ before querying what type of IRQ happened
        };

        enum class TCIF_RO_TransferCompleteInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 1U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadOnly,

            no_interrupt       = 0U,             // No interrupt occurred
            interrupt_occurred = field_bit_mask, // Transmission was completed
        };

        enum class HTIF_RO_HalfTransferInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 2U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt       = 0U,             // No interrupt occurred
            interrupt_occurred = field_bit_mask, // Transmission transferred more than half of the content specified in CNT register
        };

        enum class TEIF_RO_TransferErrorInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 3U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadOnly,

            no_interrupt       = 0U,             // No interrupt occurred
            interrupt_occurred = field_bit_mask, // Transmission experienced error, disabled channel (EN=disabled in CFGR register)
        };

        constexpr static std::tuple<
            TEIF_RO_TransferErrorInterruptFlag,
            HTIF_RO_HalfTransferInterruptFlag,
            TCIF_RO_TransferCompleteInterruptFlag,
            GIF_RO_GlobalInterruptFlag
        > reg_fields = {};

    };
}
