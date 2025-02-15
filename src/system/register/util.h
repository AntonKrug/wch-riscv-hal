//
// Created by anton on 07/02/2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include <tuple>

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

    template<Concept::FieldTypeWithBitMask EnumType>
    // requires Concept::FieldEnumWithFieldBitMask<decltype(EnumValue)>
    constexpr auto enumBitMaskOffsetCt() -> std::uint8_t {
        return bitMaskOffsetCt<static_cast<std::uint32_t>(EnumType::fieldBitMask)>();
    }

    template<std::uint32_t RawValue, Concept::FieldTypeWithBitMask EnumValue>
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

    template<Concept::FieldTypeWithBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawValueToEnumValue(const std::uint32_t registerValue) {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        return static_cast<RegisterFieldType>(mask & registerValue);
    }

    template<Concept::FieldTypeWithBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawValueToNormalizedValue(const std::uint32_t registerValue) -> std::uint32_t {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        constexpr auto offset = Soc::Reg::bitMaskOffsetCt<static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask)>();
        return (registerValue & mask) >> offset;
    }


    #pragma region IsSameAsOneFieldFromTuple

    template<typename RegField, typename Tuple, std::size_t Index>
    constexpr auto IsSameAsOneFieldFromTupleIndex() -> bool {
        return std::is_same_v<const RegField, std::tuple_element_t<Index, Tuple>>;
    }

    template<typename RegField, typename Tuple, std::size_t... Indices>
    constexpr auto IsSameAsOneFieldFromTupleIndices(std::index_sequence<Indices...>) -> bool {
        return ( IsSameAsOneFieldFromTupleIndex<RegField, Tuple, Indices>() || ...);
    }

    template<typename RegField, typename Tuple>
    constexpr auto IsSameAsOneFieldFromTuple() -> bool {
        return IsSameAsOneFieldFromTupleIndices<RegField, Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

    #pragma endregion

}