//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "../../../../system/register/field_access_privilege.h"

namespace peripheral::dma {

    struct Intfcr {  // NOLINT
        // NOTE: these fields can't be used directly as is, as the real registers have many instances of these fields repeated

        enum class CGIF_WO_ClearGlobalInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 0U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::WriteOnly,

            no_action        = 0U,             // default
            clear_flag       = field_bit_mask, // will clean GIF field from INTFR register
        };

        enum class CTCIF_WO_ClearTransferCompleteInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 1U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::WriteOnly,

            no_action        = 0U,             // default
            clear_flag       = field_bit_mask, // will clean TCIF field from INTFR register
        };

        enum class CHTIF_WO_ClearHalfTransferInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 2U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::WriteOnly,

            no_action        = 0U,             // default
            clear_flag       = field_bit_mask, // will clean HTIF field from INTFR register
        };

        enum class CTEIF_WO_ClearTransferErrorInterruptFlag: std::uint32_t { // NOLINT
            field_bit_offset = 3U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::WriteOnly,

            no_action        = 0U,             // default
            clear_flag       = field_bit_mask, // will clean TEIF field from INTFR register
        };

        constexpr static std::tuple<
            CTEIF_WO_ClearTransferErrorInterruptFlag,
            CHTIF_WO_ClearHalfTransferInterruptFlag,
            CTCIF_WO_ClearTransferCompleteInterruptFlag,
            CGIF_WO_ClearGlobalInterruptFlag
        > reg_fields = {};

    };
}
