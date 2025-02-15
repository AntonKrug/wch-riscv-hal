//
// Created by anton on 15/02/2025.
//

#pragma once

#include <cstdint>
#include <utility>
#include <tuple>

#include "concept.h"
#include "field_access_privilege.h"

namespace Soc::Reg {

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

#pragma region Field Bitmask and Field Access

    template<typename Tuple, std::size_t Index>
    constexpr auto combineBitmaskFromTupleTypeIndex() -> std::uint32_t {
        return static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldBitMask);
    }

    template<typename Tuple, std::size_t Index>
    constexpr auto combineWritableFromTupleTypeIndex() -> bool {
        return Soc::Reg::FieldAccessPrivilege::Field::isWritable<static_cast<std::uint32_t>(std::tuple_element_t<Index, Tuple>::fieldAccess)>();
    }

    template<typename Tuple, std::size_t... Indices>
    constexpr auto combineWritableMaskFromTupleTypeIndices(std::index_sequence<Indices...>) -> std::uint32_t {
        return ( (combineWritableFromTupleTypeIndex<Tuple, Indices>() ? combineBitmaskFromTupleTypeIndex<Tuple, Indices>() : 0) | ...);
    }

    template<typename Tuple>
    constexpr auto combineWritableMaskFromTupleType() -> std::uint32_t {
        return combineWritableMaskFromTupleTypeIndices<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }

#pragma endregion

}