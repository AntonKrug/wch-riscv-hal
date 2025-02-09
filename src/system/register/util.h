//
// Created by anton on 07/02/2025.
//

#pragma once

#include <cstdint>

#include "concept.h"

namespace Soc::Reg {

    template<std::uint32_t BitMaskValue>
    constexpr auto bitMaskOffsetCt() -> std::uint8_t {
        std::uint8_t count = 0;
        std::uint32_t tmp = BitMaskValue;
        while ( ((tmp&0b1) == 0) && (count<31) ) {
            tmp >>= 1;
            count++;
        }
        return count;
    }

    template<Concept::FieldEnumWithFieldBitMask EnumValue>
    constexpr auto enumBitMaskOffsetCt() -> std::uint8_t {
        return bitMaskOffsetCt<static_cast<std::uint32_t>(EnumValue::fieldBitMask)>();
    }

    template<std::uint32_t RawValue, Concept::FieldEnumWithFieldBitMask EnumValue>
    constexpr auto rawValueOffsetToEnumsOffsetCt() -> std::uint8_t {
        return RawValue << enumBitMaskOffsetCt<EnumValue>();
    }

    template<std::uint32_t BitMaskValue>
    constexpr auto bitMasksSizeCt() -> std::uint8_t {
        std::uint8_t count = 0;
        std::uint32_t tmp = BitMaskValue;
        while ( ((tmp&0b1) == 0) && (count<31) ) {
            tmp >>= 1;
            count++;
        }
        count = 0;
        while (tmp&0b1) {
            tmp >>= 1;
            count++;
        }
        return count;
    }

    enum class FieldAccessRightsEnum: std::uint32_t {
        ReadOnly,
        WriteOnly,
        ReadWrite
    };

    namespace FieldAccessRights {
        constexpr std::uint32_t ReadOnly  = static_cast<std::uint32_t>(FieldAccessRightsEnum::ReadOnly);
        constexpr std::uint32_t WriteOnly = static_cast<std::uint32_t>(FieldAccessRightsEnum::WriteOnly);
        constexpr std::uint32_t ReadWrite = static_cast<std::uint32_t>(FieldAccessRightsEnum::ReadWrite);
    }

}