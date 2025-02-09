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

    template<Concept::FieldEnumWithFieldBitMask EnumType>
    // requires Concept::FieldEnumWithFieldBitMask<decltype(EnumValue)>
    constexpr auto enumBitMaskOffsetCt() -> std::uint8_t {
        return bitMaskOffsetCt<static_cast<std::uint32_t>(EnumType::fieldBitMask)>();
    }

    template<std::uint32_t RawValue, Concept::FieldEnumWithFieldBitMask EnumValue>
    //requires Concept::FieldEnumWithFieldBitMask<decltype(EnumValue)>
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

    // TODO: add sameFieldEnums to registers like i could do on CSRs, but only on higher level functions not down here
    template<auto... Enums>
    constexpr auto combineEnumsToUint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(Enums) | ...);
    }

    template <auto... FieldValues>
    requires (Concept::FieldEnumWithFieldBitMask<decltype(FieldValues)> && ...)
    constexpr auto combineFieldMasksToUint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(decltype(FieldValues)::fieldBitMask) | ...);
    }

    template<Concept::FieldEnumWithFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawValueToEnumValue(const std::uint32_t registerValue) {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        return static_cast<RegisterFieldType>(mask & registerValue);
    }

    template<Concept::FieldEnumWithFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawValueToNormalizedValue(const std::uint32_t registerValue) -> std::uint32_t {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        constexpr auto offset = Soc::Reg::bitMaskOffsetCt<static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask)>();
        return (registerValue & mask) >> offset;
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