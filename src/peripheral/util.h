//
// Created by anton on 11/02/2025.
//

#pragma once

#include <utility>
#include <cstdint>

// #include "register_field_to_register.h"
// #include "system/register/util.h"
//#include <cstddef>

namespace Peripheral::Util {

    #pragma region Field Bitmask and Field Access

    template<typename Tuple, std::size_t Index>
    constexpr auto getBitmaskFromTupleIndex() -> std::uint32_t {
        return static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldBitMask);
    }

    template<typename Tuple, std::size_t Index>
    constexpr auto getWritableFromTupleIndex() -> bool {
        return Soc::Reg::isFieldAccessWritable<static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldAccess)>();
    }

    template<typename Tuple, std::size_t... Indices>
    constexpr auto getWritableMaskFromTupleIndices(std::index_sequence<Indices...>) -> std::uint32_t {
        return ( (getWritableFromTupleIndex<Tuple, Indices>?getBitmaskFromTupleIndex<Tuple, Indices>():0) | ...);
    }

    template<typename Tuple>
    constexpr auto getWritableMaskFromTuple() -> std::uint32_t {
        return getMasksFromTupleIndices<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

    #pragma endregion

    #pragma region IsSameAsOneFieldFromTuple

    template<typename RegField, typename Tuple, std::size_t Index>
    constexpr auto IsSameAsOneFieldFromTupleIndex() -> bool {
        return std::is_same_v<RegField, std::tuple_element_t<Index, Tuple>>;
    }

    template<typename RegField, typename Tuple, std::size_t... Indices>
    constexpr auto IsSameAsOneFieldFromTupleIndices(std::index_sequence<Indices...>) -> bool {
        return ( IsSameAsOneFieldFromTupleIndex<RegField, Tuple, Indices>() || ...);
    }

    template<typename RegField, typename Tuple>
    constexpr bool IsSameAsOneFieldFromTuple() {
        return IsSameAsOneFieldFromTupleIndices<RegField, Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

    #pragma endregion

}

