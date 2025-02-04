//
// Created by anton on 27/01/2025.
//

// TODO: unify singular and plural accesses into one, hide the parent convertors, cleanAndSet call subsets when needed,
//       setWithAutoClear should be using clear and set instead
//       getBits from the enums, offsets, sizes... , when clear is 0xff you can do write instead


#pragma once

#include <cstdint>

#include "concepts.h"
#include "csr_utils.h"
#include "csr_access_primitives_ct.h"
#include "csr_field_to_csr.h"


namespace Riscv::Csr::AccessCt {

    #pragma region ClearSetWriteAbstracted


    template <auto Csr, auto... ClearFields>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, ClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
        clearUint32<Csr, combineFieldsToUint32<ClearFields...>()>();
    }


    template <auto Csr, auto... SetFields>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        setUint32<Csr, combineFieldsToUint32<SetFields...>()>();
    }


    template <auto Csr, auto... WriteFields>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        writeUint32<Csr, combineFieldsToUint32<WriteFields...>()>();
    }


    template <auto Csr, auto... ClearFields, auto... SetFields>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, ClearFields...> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clearSet() -> void {
        clearAndSetUint32<
            Csr,
            combineFieldsToUint32<ClearFields...>(),
            combineFieldsToUint32<SetFields...>()>();
    }


    // When only specific fields needs to be updated (it will for various types of updates to the minimum of instructions)
    template <auto Csr, auto... SetFields>
    requires Riscv::Concepts::IsCsrEnumValid<Csr> &&
             Riscv::Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr std::uint32_t clearValueUin32 = Csr::getMaskFromFieldEnumValues<SetFields...>();
        constexpr std::uint32_t setValueUin32   = (static_cast<std::uint32_t>(SetFields) | ...);

        clearAndSetUint32<Csr, clearValueUin32, setValueUin32>();
    }


    #pragma endregion


    #pragma region ClearSetWriteWithParentCsrAutodetection


    template <auto... SetFields>
    requires Riscv::Concepts::SameCsrFieldEnums<SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
        constexpr auto parentCsr = getCsrFromField(SetFields...);
        clear<parentCsr, SetFields...>();
    }


    template <auto... SetFields>
    requires Riscv::Concepts::SameCsrFieldEnums<SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        constexpr auto parentCsr = Riscv::Csr::AccessCt::getCsrFromField(SetFields...);
        set<parentCsr, SetFields...>();
    }


    template <auto... WriteFields>
    requires Riscv::Concepts::SameCsrFieldEnums<WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        constexpr auto parentCsr = Riscv::Csr::AccessCt::getCsrFromField(WriteFields...);
        write<parentCsr, WriteFields...>();
    }


    // When only specific fields needs to be updated, it will detect
    // what parent CSR it belongs and what bit need to be cleared prior
    // setting correct bits. If unecesary work is detected, it will be
    // ommited to not cause overhead.
    template <auto... SetWithClearFields>
    requires Riscv::Concepts::SameCsrFieldEnums<SetWithClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr auto parentCsr = Riscv::Csr::AccessCt::getCsrFromField(SetWithClearFields...);
        setWithAutoClear<parentCsr,  SetWithClearFields...>();
    }


    #pragma endregion


}


