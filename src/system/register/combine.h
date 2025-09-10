//
// Created by anton on 15/02/2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include <tuple>

#include "concepts.h"
#include "field_access_privilege.h"

namespace soc::reg::combine {

    // TODO: add sameFieldEnums to registers like i could do on CSRs, but only on higher level functions not down here
    template<auto... Enums>
    constexpr auto enums_to_uint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(Enums) | ...);
    }

    template <auto... FieldValues>
    requires (concepts::field_with_bit_mask<FieldValues> && ...)
    constexpr auto field_masks_to_uint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(decltype(FieldValues)::field_bit_mask) | ...);
    }

    template <concepts::field_type_with_bit_mask... FieldTypes>
    constexpr auto field_type_masks_to_uint32() -> std::uint32_t {
        return (static_cast<std::uint32_t>(FieldTypes::field_bit_mask) | ...);
    }

    #pragma region Field Bitmask and Field Access

    template<typename Tuple, std::size_t Index>
    constexpr auto bitmask_from_tuple_type_index() -> std::uint32_t {
        return static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::field_bit_mask);
    }

    template<typename Tuple, std::size_t Index>
    constexpr auto writable_from_tuple_type_index() -> bool {
        return field_access_right::isWritable<static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::field_bit_mask)>();
    }

    template<typename Tuple, std::size_t... Indices>
    constexpr auto writable_mask_from_tuple_type_indices(std::index_sequence<Indices...>) -> std::uint32_t {
        return ( (writable_from_tuple_type_index<Tuple, Indices>() ? bitmask_from_tuple_type_index<Tuple, Indices>() : 0) | ...);
    }

    template<typename Tuple>
    constexpr auto writable_mask_from_tuple_type() -> std::uint32_t {
        return writable_mask_from_tuple_type_indices<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

    #pragma endregion

}