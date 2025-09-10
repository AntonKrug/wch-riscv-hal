//
// Created by anton on 07/02/2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include <tuple>

#include "concepts.h"

namespace soc::reg {
    template<std::uint32_t BitMaskValue>
    constexpr auto bit_mask_offset_ct() -> std::uint32_t {
        std::uint32_t count = 0U;
        std::uint32_t tmp = BitMaskValue;
        while (((tmp & 0b1U) == 0U) && (count < 31U)) {
            tmp >>= 1;
            count++;
        }
        return count;
    }

    template<concepts::field_type_with_bit_mask EnumType>
    // requires concepts::FieldEnumWithFieldBitMask<decltype(EnumValue)>
    constexpr auto enum_bit_mask_offset_ct() -> std::uint8_t {
        return bit_mask_offset_ct<static_cast<std::uint32_t>(EnumType::field_bit_mask)>();
    }

    template<std::uint32_t RawValue, concepts::field_type_with_bit_mask EnumValue>
    //requires concepts::FieldEnumWithFieldBitMask<decltype(EnumValue)>
    constexpr auto rawValueOffsetToEnumsOffsetCt() -> std::uint8_t {
        return RawValue << enum_bit_mask_offset_ct<EnumValue>();
    }

    template<std::uint32_t BitMaskValue>
    constexpr auto bit_masks_size_ct() -> std::uint32_t {
        std::uint32_t count = 0U;
        std::uint32_t tmp = BitMaskValue;
        while (((tmp & 0b1) == 0U) && (count < 31U)) {
            tmp >>= 1;
            count++;
        }
        count = 0U;
        while ((tmp & 0b1) != 0U) {
            tmp >>= 1;
            count++;
        }
        return count;
    }

    template<concepts::field_type_with_bit_mask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    raw_value_to_enum_value(const std::uint32_t register_value) {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::field_bit_mask);
        return static_cast<RegisterFieldType>(mask & register_value);
    }

    template<concepts::field_type_with_bit_mask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    raw_value_to_normalized_value(const std::uint32_t register_value) -> std::uint32_t {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::field_bit_mask);
        constexpr auto offset = soc::reg::bit_mask_offset_ct<static_cast<std::uint32_t>(
            RegisterFieldType::field_bit_mask)>();
        return (register_value & mask) >> offset;
    }


#pragma region Interoperability - converting from other types (and primitives) to Field enums and vice versa

    //TODO: use field offsets everywhere, do not calculate the bitMaskOffsetCt

    template<bool value, typename RegisterFieldType>
        requires concepts::field_type_with_enable_and_disable_values<RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    bool_to_register_field_enum() -> RegisterFieldType {
        return value ? RegisterFieldType::enable : RegisterFieldType::disable;
    }

    template<auto RegisterField>
        requires concepts::field_with_bool_interoperability<RegisterField>
    inline constexpr auto
    __attribute__ ((always_inline))
    register_field_enum_to_bool() -> bool {
        constexpr std::uint32_t rawValue = static_cast<std::uint32_t>(RegisterField) >> decltype(RegisterField
                                           )::fieldBitOffset;
        return rawValue == 1U;
    }

    template<auto Value, concepts::field_type_with_bit_mask RegisterFieldType>
    constexpr auto value_to_register_field_enum() -> RegisterFieldType {
        constexpr auto raw_value =
            static_cast<std::uint32_t>(Value) <<
                static_cast<std::uint32_t>(RegisterFieldType::fieldBitOffset);

        return static_cast<RegisterFieldType>(raw_value);
    }

    template<auto RegisterField>
        requires concepts::field_with_bit_mask<RegisterField>
    constexpr auto register_field_enum_to_uint32() -> std::uint32_t {
        return
            static_cast<std::uint32_t>(RegisterField) >>
                static_cast<std::uint32_t>(decltype(RegisterField)::fieldBitOffset);
    }

#pragma endregion


#pragma region IsSameAsOneFieldFromTuple

    template<typename RegField, typename Tuple, std::size_t Index>
    constexpr auto is_same_as_one_field_from_tuple_index() -> bool {
        return std::is_same_v<const RegField, std::tuple_element_t<Index, Tuple> >;
    }

    template<typename RegField, typename Tuple, std::size_t... Indices>
    constexpr auto is_same_as_one_field_from_tuple_indices(std::index_sequence<Indices...>) -> bool {
        return ( is_same_as_one_field_from_tuple_index<RegField, Tuple, Indices>() || ...);
    }

    template<typename RegField, typename Tuple>
    constexpr auto is_same_as_one_field_from_tuple() -> bool {
        return is_same_as_one_field_from_tuple_indices<RegField, Tuple>(
            std::make_index_sequence<std::tuple_size_v<Tuple> >{});
    }

#pragma endregion
}
