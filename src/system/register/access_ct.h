//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "concept.h"
#include "combine.h"
#include "access_primitives.h"
#include "peripheral/obtain_reg_metadata.h"
#include "peripheral/base_addr.h"

constexpr std::uint32_t full32bitReg = 0xffffffffU;

namespace Soc::Reg {

    #pragma region isReg

    template<std::uint32_t BaseAddr, auto TestedRegFieldHead, auto... TestedRegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    isRegFieldsSetMipCt() -> bool {
        constexpr auto combinedValue  = Soc::Reg::Combine::enumsToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
        constexpr auto combinedMask   = Soc::Reg::Combine::fieldMasksToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
        // auto [regOffset, _] = RegMetadata::fromRegField<TestedRegFieldHead>();
        constexpr auto regOffset      = Peripheral::RegMemOffset::fromRegField<TestedRegFieldHead>();
        const     auto actualValue    = Soc::Reg::Access::readCtAddr<BaseAddr + regOffset>();

        return (actualValue & combinedMask) == combinedValue;
    }

    template<auto TestedRegFieldHead, auto... TestedRegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    isRegFieldsSetSipCt() -> bool {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegField<TestedRegFieldHead>();
        return isRegFieldsSetMipCt<baseAddr, TestedRegFieldHead, TestedRegFieldTails...>();
    }

    #pragma endregion

    #pragma region writeReg

    template<std::uint32_t BaseAddr, auto RegFieldHead, auto... RegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
    inline constexpr
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    auto writeRegFieldsMipCt() -> void {
        constexpr auto regOffset     = Peripheral::RegMemOffset::fromRegField<RegFieldHead>();
        constexpr auto combinedValue = Soc::Reg::Combine::enumsToUint32<RegFieldHead, RegFieldTails...>();
        Soc::Reg::Access::writeCtAddrVal<BaseAddr + regOffset, combinedValue>();
    }

    template<auto RegFieldHead, auto... RegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
    inline constexpr
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    auto writeRegFieldsSipCt() -> void {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegField<RegFieldHead>();
        return writeRegFieldsMipCt<baseAddr, RegFieldHead, RegFieldTails...>();
    }

    #pragma endregion

    #pragma region setRegWithRawValue

    template<std::uint32_t BaseAddress, std::uint32_t ValueToBeWritten, typename  RegFieldTypeHead, typename ... RegFieldTypeTails>
    requires
        Soc::Reg::Concept::FieldTypeWithBitMask<RegFieldTypeHead> &&
        (Soc::Reg::Concept::FieldTypeWithBitMask<RegFieldTypeTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsWithRawValueMipCt() -> void {
        constexpr auto regOffset              = Peripheral::RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto regFieldsTuple         = Peripheral::RegFieldTuple::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto maskGoingToWritten     = Soc::Reg::Combine::fieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
        constexpr auto maskShouldBeKept       = 0xffffffffu ^ maskGoingToWritten;
        constexpr auto maskAllowedToBeWritten = Soc::Reg::Combine::writableMaskFromTupleType<decltype(regFieldsTuple)>();
        constexpr auto maskForbiddenToWrite   = 0xffffffffu ^ maskAllowedToBeWritten;

        static_assert(
            ValueToBeWritten != 0U || maskGoingToWritten != 0U,
            "Both value-to-be-set and their mask are empty, this indicates badly described register field");

        static_assert(
            (maskForbiddenToWrite & maskGoingToWritten) == 0,
            "Some bit[s]/field[s] were going to be written into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are setting a RO field.");

        if constexpr (maskGoingToWritten == 0xffffffffU || maskGoingToWritten == maskAllowedToBeWritten) {
            // Either whole register is going to be set, or whole writable part of the register,
            // therefore no need to be clearing the register before setting it,
            // we can just write the whole register directly
            Soc::Reg::Access::writeCtAddr<BaseAddress + regOffset>(ValueToBeWritten);
        } else {
            auto actualValue = Soc::Reg::Access::readCtAddr<BaseAddress + regOffset>();

            // if clearing and setting is the same value, then clearing can be omited
            if constexpr (maskGoingToWritten != ValueToBeWritten) {
                actualValue &= maskShouldBeKept;   // to really clear we need to invert the mask
            }

            // if nothing to set then ommit setting
            if constexpr (ValueToBeWritten > 0U) {
                actualValue |= ValueToBeWritten;
            }
            Soc::Reg::Access::writeCtAddr<BaseAddress + regOffset>(actualValue);
        }
    }

    template<std::uint32_t ValueToBeWritten, typename  RegFieldTypeHead, typename... RegFieldTypeTails>
    // requires
    //     Soc::Reg::Concept::FieldTypeWithBitMask<RegFieldTypeHead> &&
    //     (Soc::Reg::Concept::FieldTypeWithBitMask<RegFieldTypeTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsWithRawValueSipCt() -> void {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegFieldType<RegFieldTypeHead>();
        return setRegFieldsWithRawValueMipCt<baseAddr, ValueToBeWritten, RegFieldTypeHead, RegFieldTypeTails...>();
    }

    #pragma endregion

    #pragma region setReg

    template<std::uint32_t BaseAddress, auto RegFieldHead, auto... RegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsMipCt() -> void {
        constexpr auto valueToBeWritten = Soc::Reg::Combine::enumsToUint32<RegFieldHead, RegFieldTails...>();
        setRegFieldsWithRawValueMipCt<BaseAddress, valueToBeWritten, decltype(RegFieldHead),decltype(RegFieldTails)...>();
    }

    template<auto RegFieldHead, auto... RegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsSipCt() -> void {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegField<RegFieldHead>();
        return setRegFieldsMipCt<baseAddr, RegFieldHead, RegFieldTails...>();
    }

    #pragma endregion

    #pragma region ClearReg

    // TODO: requirements/concepts for arguments
    template<std::uint32_t BaseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    clearRegFieldTypesMipCt() -> void {
        constexpr auto regOffset              = Peripheral::RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto regFieldsTuple         = Peripheral::RegFieldTuple::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto maskToClear            = Soc::Reg::Combine::fieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
        constexpr auto maskAllowedToBeWritten = Soc::Reg::Combine::writableMaskFromTupleType<decltype(regFieldsTuple)>();
        constexpr auto maskForbiddenToWrite   = 0xffffffffU ^ maskAllowedToBeWritten;

        static_assert(
            maskToClear != 0U,
            "Going not clear anything, this indicates badly described register field");

        static_assert(
            (maskForbiddenToWrite & maskToClear) == 0,
            "Some bit[s]/field[s] were going to be cleared into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are clearing a RO field.");

        if constexpr (maskToClear == maskAllowedToBeWritten) {
            // everything what can be written is to be cleared, no point reading
            Soc::Reg::Access::writeCtAddrVal<BaseAddress + regOffset, 0>();
        }
        else {
            const auto actualValue = Soc::Reg::Access::readCtAddr<BaseAddress + regOffset>();
            Soc::Reg::Access::writeCtAddr<BaseAddress + regOffset>(actualValue & maskToClear);
        }

    }

    // TODO: requirements/concepts for arguments
    template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    clearRegFieldTypesSipCt() -> void {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegFieldType<RegFieldTypeHead>();
        return clearRegFieldTypesMipCt<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
    }

    #pragma endregion

    #pragma region KeepReg

    // TODO: requirements/concepts for arguments
    template<std::uint32_t BaseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    keepRegFieldTypesMipCt() -> void {
        constexpr auto regOffset    = Peripheral::RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto combinedMask = Soc::Reg::Combine::fieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
        const     auto actualValue  = Soc::Reg::Access::readCtAddr<BaseAddress + regOffset>();

        Soc::Reg::Access::writeCtAddr<BaseAddress + regOffset>( actualValue & (0xffffffff ^ combinedMask) );
    }

    // TODO: requirements/concepts for arguments
    template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    keepRegFieldTypesSipCt() -> void {
        constexpr auto baseAddr = Peripheral::BaseAddr::fromRegFieldType<RegFieldTypeHead>();
        return keepRegFieldTypesMipCt<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
    }

    #pragma endregion

}
