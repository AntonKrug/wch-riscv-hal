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

    // TODO: add sameFieldEnums to registers like i could do on CSRs, but only on higher level functions not down here
    template<auto... Enums>
    constexpr auto combineEnumsToUint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(Enums) | ...);
    }

    template <auto... FieldValues>
    requires (Concept::FieldWithBitMask<FieldValues> && ...)
    constexpr auto combineFieldMasksToUint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(decltype(FieldValues)::fieldBitMask) | ...);
    }

    template <Concept::FieldTypeWithBitMask... FieldTypes>
    constexpr auto combineFieldTypeMasksToUint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(FieldTypes::fieldBitMask) | ...);
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

    enum class FieldAccessRightsEnum: std::uint32_t {
        ReadOnly  = 1,
        WriteOnly = 2,
        ReadWrite = 4
    };

    template<FieldAccessRightsEnum access>
    constexpr auto isFieldAccessWritable() -> bool {
        if (access == FieldAccessRightsEnum::ReadOnly) {
            return false;
        }
        return true;
    }

    template<std::uint32_t access>
    constexpr auto isFieldAccessWritable() -> bool {
        if (access == static_cast<std::uint32_t>(FieldAccessRightsEnum::ReadOnly)) {
            return false;
        }
        return true;
    }

    namespace FieldAccessRights {
        constexpr std::uint32_t ReadOnly  = static_cast<std::uint32_t>(FieldAccessRightsEnum::ReadOnly);
        constexpr std::uint32_t WriteOnly = static_cast<std::uint32_t>(FieldAccessRightsEnum::WriteOnly);
        constexpr std::uint32_t ReadWrite = static_cast<std::uint32_t>(FieldAccessRightsEnum::ReadWrite);
    }

    #pragma region Field Bitmask and Field Access

    template<typename Tuple, std::size_t Index>
    constexpr auto getBitmaskFromTupleTypeIndex() -> std::uint32_t {
        return static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldBitMask);
    }

    template<typename Tuple, std::size_t Index>
    constexpr auto getWritableFromTupleTypeIndex() -> bool {
        return Soc::Reg::isFieldAccessWritable<static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldAccess)>();
    }

    template<typename Tuple, std::size_t... Indices>
    constexpr auto getWritableMaskFromTupleTypeIndices(std::index_sequence<Indices...>) -> std::uint32_t {
        return ( (getWritableFromTupleTypeIndex<Tuple, Indices>()?getBitmaskFromTupleTypeIndex<Tuple, Indices>():0) | ...);
    }

    template<typename Tuple>
    constexpr auto getWritableMaskFromTupleType() -> std::uint32_t {
        return getWritableMaskFromTupleTypeIndices<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

    #pragma endregion

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