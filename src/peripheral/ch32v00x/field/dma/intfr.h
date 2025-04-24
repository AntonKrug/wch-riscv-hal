//
// Created by anton on 16/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Dma {

    struct Intfr {
        // Flags are set when events happen regardless if the IRQ was enabled or not.
        // NOTE: these fields can't be used directly as is, as the real registers have many instances of these fields repeated

        enum class GIF_RO_GlobalInterruptFlag: std::uint32_t {
            fieldBitOffset = 0U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0U << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Can be queried to find out which channel experienced IRQ before querying what type of IRQ happened
        };

        enum class TCIF_RO_TransferCompleteInterruptFlag: std::uint32_t {
            fieldBitOffset = 1U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0U << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Transmission was completed
        };

        enum class HTIF_RO_HalfTransferInterruptFlag: std::uint32_t {
            fieldBitOffset = 2U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0U << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Transmission transferred more than half of the content specified in CNT register
        };

        enum class TEIF_RO_TransferErrorInterruptFlag: std::uint32_t {
            fieldBitOffset = 3U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0U << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Transmission experienced error, disabled channel (EN=disabled in CFGR register)
        };

        constexpr static std::tuple<
            TEIF_RO_TransferErrorInterruptFlag,
            HTIF_RO_HalfTransferInterruptFlag,
            TCIF_RO_TransferCompleteInterruptFlag,
            GIF_RO_GlobalInterruptFlag
        > regFields = {};

    };
}
