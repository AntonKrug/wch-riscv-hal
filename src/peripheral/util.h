//
// Created by anton on 11/02/2025.
//

#pragma once

#include <utility>
#include <cstdint>

#include "register_field_to_register.h"
#include "system/register/util.h"
//#include <cstddef>

namespace Peripheral::Util {

    #pragma region Bitmask

    template<typename f, std::size_t Index>
    constexpr std::uint32_t getMaskFromTupleIndex() {
        return static_cast<std::uint32_t>(std::tuple_element_t<Index, f>::fieldBitMask);
    }

    template<typename f, std::size_t Index>
    constexpr bool isWritableFromTupleIndex() {
        return Soc::Reg::isFieldAccessWritable<static_cast<std::uint32_t>(std::tuple_element_t<Index, f>::fieldAccess)>();
    }

    template<typename f, std::size_t... Indices>
    constexpr std::uint32_t getWritableMaskFromTupleIndices(std::index_sequence<Indices...>) {
        return ( (isWritableFromTupleIndex?getMaskFromTupleIndex<f, Indices>():0) | ...);
    }

    template<typename f>
    constexpr std::uint32_t getWritableMaskFromTuple() {
        return getMasksFromTupleIndices<f>(std::make_index_sequence<std::tuple_size_v<f>>{});
    }

    #pragma endregion

}

