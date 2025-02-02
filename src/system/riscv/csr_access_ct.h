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


namespace Riscv::Csr::AccessCt {


    using namespace Riscv;


    #pragma region getCsrFromField


    template<Riscv::Csr::Intsyscr::IsAnyField Field>
    constexpr auto getCsrFromField(Field field) {
        return QingKeV2::intsyscr;
    }


    template<Riscv::Csr::Mtvec::IsAnyField... Field>
    constexpr auto getCsrFromField(Field... field) {
        return QingKeV2::mtvec;
    }


    template<Riscv::Csr::Mstatus::IsAnyField... Field>
    constexpr auto getCsrFromField(Field... field) {
        return QingKeV2::mstatus;
    }


    #pragma endregion


    #pragma region ClearSetWriteAbstracted


    template <auto Csr, auto... ClearFields>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::SameCsrFieldEnumsAndMatchingParentCsr<Csr, ClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    clear() -> void {
    }


    // All the writes for various types
    template <auto Csr, auto... WriteFields>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::CsrFieldEnumMatchingCsrGeneric<false, Csr, WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        constexpr std::uint32_t value = (static_cast<std::uint32_t>(WriteFields) | ...);
        writeUin32<Csr, value>();
    }


    // When only specific fields needs to be updated (it will for various types of updates to the minimum of instructions)
    template <auto Csr, auto... Values>
    requires Concepts::IsCsrEnumValid<Csr> &&
             Concepts::CsrFieldEnumMatchingCsrGeneric<false, Csr, Values...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr std::uint32_t clearValueUin32 = Csr::getMaskFromFieldEnumValues<Values...>();
        constexpr std::uint32_t setValueUin32   = (static_cast<std::uint32_t>(Values) | ...);

        clearAndSetUin32<Csr, clearValueUin32, setValueUin32>();
    }

    #pragma endregion


    #pragma region ClearSetWriteWithParentCsrAutodetection


    // All the Sets for various types
    template <auto... SetFields>
    requires Concepts::SameCsrFieldEnums<SetFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    set() -> void {
        constexpr auto parentCsr = getCsrFromField(SetFields...);
        set<parentCsr, SetFields...>();
    }


    // All the writes for various types
    template <auto... WriteFields>
    requires Concepts::SameCsrFieldEnums<WriteFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    write() -> void {
        constexpr auto parentCsr = getCsrFromField(WriteFields...);
        write<parentCsr, WriteFields...>();
    }


    // When only specific fields needs to be updated, it will detect what parent CSR it belongs, clear and set correct bits
    template <auto... SetWithClearFields>
    requires Concepts::SameCsrFieldEnums<SetWithClearFields...>
    inline
    constexpr auto
    __attribute__ ((always_inline))
    setWithAutoClear() -> void {
        constexpr auto parentCsr = getCsrFromField(SetWithClearFields...);
        setWithAutoClear<parentCsr,  SetWithClearFields...>();
    }


    #pragma endregion


}


