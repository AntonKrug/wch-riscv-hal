//
// Created by anton on 16/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Dma {

    struct Intfr {
        constexpr static std::uint32_t regFieldAlignment = 0x4;

        enum class GIF_RO_GlobalInterruptFlag: std::uint32_t {
            fieldBitOffset = 0,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0 << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Can be queried to find out which channel experienced IRQ before querying what type of IRQ happened
        };

        enum class TCIF_RO_TransferCompleteInterruptFlag: std::uint32_t {
            fieldBitOffset = 1,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0 << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Transmission was completed
        };

        enum class HTIF_RO_HalfTransferInterruptFlag: std::uint32_t {
            fieldBitOffset = 2,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0 << fieldBitOffset, // No interrupt occurred
            interruptOccurred = fieldBitMask,        // Transmission transferred more than half of the content specified in CNT register
        };

        enum class TEIF_RO_TransferErrorInterruptFlag: std::uint32_t {
            fieldBitOffset = 3,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadOnly,

            noInterrupt       = 0 << fieldBitOffset, // No interrupt occurred
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
