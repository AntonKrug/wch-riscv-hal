//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "concept.h"
#include "combine.h"
#include "access_primitives_ct.h"
#include "peripheral/obtain_reg_metadata.h"
#include "peripheral/base_addr.h"

constexpr std::uint32_t full32bitReg = 0xffffffffu;

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
        const     auto actualValue    = Soc::Reg::Access::readCt<BaseAddr + regOffset>();

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
        Soc::Reg::Access::writeCt<BaseAddr + regOffset, combinedValue>();
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

    #pragma region setReg

    template<std::uint32_t baseAddress, auto RegFieldHead, auto... RegFieldTails>
    requires
        Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
        (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsMipCt() -> void {
        // TODO: unify getting fieldtuple and reg offset
        // auto [ regOffset, regFieldTuple] = RegMetadata::fromRegField<RegFieldHead>();
        constexpr auto regOffset              = Peripheral::RegMemOffset::fromRegField<RegFieldHead>();
        constexpr auto regFieldsTuple         = Peripheral::RegFieldTuple::fromRegField<RegFieldHead>();
        constexpr auto valueToBeWritten       = Soc::Reg::Combine::enumsToUint32<RegFieldHead, RegFieldTails...>();
        constexpr auto maskGoingToWritten     = Soc::Reg::Combine::fieldMasksToUint32<RegFieldHead, RegFieldTails...>();
        constexpr auto maskShouldBeKept       = 0xffffffffu ^ maskGoingToWritten;
        constexpr auto maskAllowedToBeWritten = Soc::Reg::Combine::writableMaskFromTupleType<decltype(regFieldsTuple)>();
        constexpr auto maskForbiddenToWrite   = 0xffffffffu ^ maskAllowedToBeWritten;

        static_assert(
            valueToBeWritten != 0u || maskGoingToWritten != 0u,
            "Both value-to-be-set and their mask are empty, this indicates badly described register field");

        static_assert(
            (maskForbiddenToWrite & maskGoingToWritten) == 0,
            "Some bit[s]/field[s] were going to be written into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are setting a RO field.");

        if constexpr (maskGoingToWritten == 0xffffffffu || maskGoingToWritten == maskAllowedToBeWritten) {
            // Either whole register is going to be set, or whole writable part of the register,
            // therefore no need to be clearing the register before setting it,
            // we can just write the whole register directly
            Soc::Reg::Access::writeCt<baseAddress + regOffset>(valueToBeWritten);
        } else {
            auto actualValue = Soc::Reg::Access::readCt<baseAddress + regOffset>();

            // if clearing and setting is the same value, then clearing can be omited
            if constexpr (maskGoingToWritten != valueToBeWritten) {
                actualValue &= maskShouldBeKept;   // to really clear we need to invert the mask
            }

            // if nothing to set then ommit setting
            if constexpr (valueToBeWritten > 0) {
                actualValue |= valueToBeWritten;
            }
            Soc::Reg::Access::writeCt<baseAddress + regOffset>(actualValue);
        }
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

    template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
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
        constexpr auto maskForbiddenToWrite   = 0xffffffffu ^ maskAllowedToBeWritten;

        static_assert(
            maskToClear != 0u,
            "Going not clear anything, this indicates badly described register field");

        static_assert(
            (maskForbiddenToWrite & maskToClear) == 0,
            "Some bit[s]/field[s] were going to be cleared into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are clearing a RO field.");

        if constexpr (maskToClear == maskAllowedToBeWritten) {
            // everything what can be written is to be cleared, no point reading
            Soc::Reg::Access::writeCt<baseAddress + regOffset>(0);
        }
        else {
            const auto actualValue = Soc::Reg::Access::readCt<baseAddress + regOffset>();
            Soc::Reg::Access::writeCt<baseAddress + regOffset>(actualValue & maskToClear);
        }

    }


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

    template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    keepRegFieldTypesMipCt() -> void {
        constexpr auto regOffset    = Peripheral::RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto combinedMask = Soc::Reg::Combine::fieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
        const     auto actualValue  = Soc::Reg::Access::readCt<baseAddress + regOffset>();

        Soc::Reg::Access::writeCt<baseAddress + regOffset>( actualValue & (0xffffffff ^ combinedMask) );
    }


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
