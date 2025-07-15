//
// Created by anton on 27/01/2025.
//

// TODO: unify singular and plural accesses into one, hide the parent convertors, cleanAndSet call subsets when needed,
//       setWithAutoClear should be using clear and set instead
//       getBits from the enums, offsets, sizes... , when clear is 0xff you can do write instead


#pragma once

#include <cstdint>

#include "concepts.h"
#include "csr_util.h"
#include "csr_access_primitives_ct.h"
#include "csr_field_to_csr.h"
#include "system/register/util.h"


namespace riscv::csr::access_ct {

    #pragma region ClearSetWriteAbstracted


    template <auto Csr, auto... ClearFields>
    requires riscv::concepts::is_csr_enum_valid<Csr> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, ClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
        clearUint32<Csr, soc::reg::combine::enums_to_uint32<ClearFields...>()>();
    }


    template <auto Csr, auto... SetFields>
    requires riscv::concepts::is_csr_enum_valid<Csr> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        setUint32<Csr, soc::reg::combine::enums_to_uint32<SetFields...>()>();
    }


    template <auto Csr, auto... WriteFields>
    requires riscv::concepts::is_csr_enum_valid<Csr> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        writeUint32<Csr, soc::reg::combine::enums_to_uint32<WriteFields...>()>();
    }


    template <auto Csr, auto... ClearFields, auto... SetFields>
    requires riscv::concepts::is_csr_enum_valid<Csr> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, ClearFields...> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clearSet() -> void {
        clearAndSetUint32<
            Csr,
            soc::reg::combine::enums_to_uint32<ClearFields...>(),
            soc::reg::combine::enums_to_uint32<SetFields...>()>();
    }


    // When only specific fields needs to be updated (it will for various types of updates to the minimum of instructions)
    template <auto Csr, auto... SetFields>
    requires riscv::concepts::is_csr_enum_valid<Csr> &&
             riscv::concepts::same_csr_field_enums_and_matching_parent_csr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr std::uint32_t clear_value_uint32 = soc::reg::combine::field_masks_to_uint32<SetFields...>();
        constexpr std::uint32_t set_value_uint32   = (static_cast<std::uint32_t>(SetFields) | ...);

        clearAndSetUint32<Csr, clear_value_uint32, set_value_uint32>();
    }


    #pragma endregion


    #pragma region ClearSetWriteWithParentCsrAutodetection


    template <auto... SetFields>
    requires riscv::concepts::same_csr_field_enums<SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
        constexpr auto parent_csr = riscv::csr::access_ct::getCsrFromField(SetFields...);
        clear<parent_csr, SetFields...>();
    }


    template <auto... SetFields>
    requires riscv::concepts::same_csr_field_enums<SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        constexpr auto parent_csr = riscv::csr::access_ct::getCsrFromField(SetFields...);
        set<parent_csr, SetFields...>();
    }


    template <auto... WriteFields>
    requires riscv::concepts::same_csr_field_enums<WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        constexpr auto parent_csr = riscv::csr::access_ct::getCsrFromField(WriteFields...);
        write<parent_csr, WriteFields...>();
    }


    // When only specific fields needs to be updated, it will detect
    // what parent CSR it belongs and what bit need to be cleared prior
    // setting correct bits. If unecesary work is detected, it will be
    // ommited to not cause overhead.
    template <auto... SetWithClearFields>
    requires riscv::concepts::same_csr_field_enums<SetWithClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr auto parent_csr = riscv::csr::access_ct::getCsrFromField(SetWithClearFields...);
        setWithAutoClear<parent_csr,  SetWithClearFields...>();
    }


    #pragma endregion


}


