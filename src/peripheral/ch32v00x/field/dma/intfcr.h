//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Dma {

    struct Intfcr {
        constexpr static std::uint32_t regFieldAlignment = 0x4U;

        enum class CGIF_WO_ClearGlobalInterruptFlag: std::uint32_t {
            fieldBitOffset = 0U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::WriteOnly,

            noAction       = 0U << fieldBitOffset, // default
            clearFlag      = fieldBitMask,        // will clean GIF field from INTFR register
        };

        enum class CTCIF_WO_ClearTransferCompleteInterruptFlag: std::uint32_t {
            fieldBitOffset = 1U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::WriteOnly,

            noAction       = 0U << fieldBitOffset, // default
            clearFlag      = fieldBitMask,        // will clean TCIF field from INTFR register
        };

        enum class CHTIF_WO_ClearHalfTransferInterruptFlag: std::uint32_t {
            fieldBitOffset = 2U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::WriteOnly,

            noAction       = 0U << fieldBitOffset, // default
            clearFlag      = fieldBitMask,        // will clean HTIF field from INTFR register
        };

        enum class CTEIF_WO_ClearTransferErrorInterruptFlag: std::uint32_t {
            fieldBitOffset = 3U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::WriteOnly,

            noAction       = 0U << fieldBitOffset, // default
            clearFlag      = fieldBitMask,        // will clean TEIF field from INTFR register
        };

        constexpr static std::tuple<
            CTEIF_WO_ClearTransferErrorInterruptFlag,
            CHTIF_WO_ClearHalfTransferInterruptFlag,
            CTCIF_WO_ClearTransferCompleteInterruptFlag,
            CGIF_WO_ClearGlobalInterruptFlag
        > regFields = {};

    };
}
