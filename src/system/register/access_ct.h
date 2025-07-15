//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"
#include "combine.h"
#include "access_primitives.h"
#include "peripheral/obtain_reg_metadata.h"
#include "peripheral/base_addr.h"

constexpr std::uint32_t full32_bit_reg = 0xffffffffU;

namespace soc::reg {

    #pragma region isReg

    template<std::uint32_t BaseAddr, auto TestedRegFieldHead, auto... TestedRegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<TestedRegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<TestedRegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    isRegFieldsSetMipCt() -> bool {
        constexpr auto combined_value = soc::reg::combine::enums_to_uint32<TestedRegFieldHead, TestedRegFieldTails...>();
        constexpr auto combined_mask  = soc::reg::combine::field_masks_to_uint32<TestedRegFieldHead, TestedRegFieldTails...>();
        // auto [regOffset, _] = RegMetadata::fromRegField<TestedRegFieldHead>();
        constexpr auto reg_offset     = peripheral::reg_mem_offset::fromRegField<TestedRegFieldHead>();
        const     auto actual_value   = soc::reg::access::readCtAddr<BaseAddr + reg_offset>();

        return (actual_value & combined_mask) == combined_value;
    }

    template<auto TestedRegFieldHead, auto... TestedRegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<TestedRegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<TestedRegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    isRegFieldsSetSipCt() -> bool {
        constexpr auto base_addr = peripheral::base_addr::fromRegField<TestedRegFieldHead>();
        return isRegFieldsSetMipCt<base_addr, TestedRegFieldHead, TestedRegFieldTails...>();
    }

    #pragma endregion

    #pragma region writeReg

    template<std::uint32_t BaseAddr, auto RegFieldHead, auto... RegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<RegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<RegFieldTails> && ...)
    inline constexpr
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    auto writeRegFieldsMipCt() -> void {
        constexpr auto reg_offset     = peripheral::reg_mem_offset::fromRegField<RegFieldHead>();
        constexpr auto combined_value = soc::reg::combine::enums_to_uint32<RegFieldHead, RegFieldTails...>();
        soc::reg::access::writeCtAddrVal<BaseAddr + reg_offset, combined_value>();
    }

    template<auto RegFieldHead, auto... RegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<RegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<RegFieldTails> && ...)
    inline constexpr
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    auto writeRegFieldsSipCt() -> void {
        constexpr auto base_addr = peripheral::base_addr::fromRegField<RegFieldHead>();
        return writeRegFieldsMipCt<base_addr, RegFieldHead, RegFieldTails...>();
    }

    #pragma endregion

    #pragma region setRegWithRawValue

    template<std::uint32_t BaseAddress, std::uint32_t ValueToBeWritten, typename RegFieldTypeHead, typename ... RegFieldTypeTails>
    requires
        soc::reg::concepts::field_type_with_bit_mask<RegFieldTypeHead> &&
        (soc::reg::concepts::field_type_with_bit_mask<RegFieldTypeTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsWithRawValueMipCt() -> void {
        constexpr auto reg_offset                 = peripheral::reg_mem_offset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto reg_fields_tuple           = peripheral::reg_field_tuple::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto mask_going_to_written      = soc::reg::combine::field_type_masks_to_uint32<RegFieldTypeHead, RegFieldTypeTails...>();
        constexpr auto mask_should_be_kept        = 0xffffffffU ^ mask_going_to_written;
        constexpr auto mask_allowed_to_be_written = soc::reg::combine::writable_mask_from_tuple_type<decltype(reg_fields_tuple)>();
        constexpr auto mask_forbidden_to_write    = 0xffffffffU ^ mask_allowed_to_be_written;

        static_assert(
            ValueToBeWritten != 0U || mask_going_to_written != 0U,
            "Both value-to-be-set and their mask are empty, this indicates badly described register field");

        static_assert(
            (mask_forbidden_to_write & mask_going_to_written) == 0,
            "Some bit[s]/field[s] were going to be written into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are setting a RO field.");

        if constexpr (mask_going_to_written == 0xffffffffU || mask_going_to_written == mask_allowed_to_be_written) {
            // Either whole register is going to be set, or whole writable part of the register,
            // therefore no need to be clearing the register before setting it,
            // we can just write the whole register directly
            soc::reg::access::writeCtAddr<BaseAddress + reg_offset>(ValueToBeWritten);
        } else {
            auto actual_value = soc::reg::access::readCtAddr<BaseAddress + reg_offset>();

            // if clearing and setting is the same value, then clearing can be omited
            if constexpr (mask_going_to_written != ValueToBeWritten) {
                actual_value &= mask_should_be_kept;   // to really clear we need to invert the mask
            }

            // if nothing to set then ommit setting
            if constexpr (ValueToBeWritten > 0U) {
                actual_value |= ValueToBeWritten;
            }
            soc::reg::access::writeCtAddr<BaseAddress + reg_offset>(actual_value);
        }
    }

    template<std::uint32_t ValueToBeWritten, typename  RegFieldTypeHead, typename... RegFieldTypeTails>
    // requires
    //     Soc::Reg::concepts::FieldTypeWithBitMask<RegFieldTypeHead> &&
    //     (Soc::Reg::concepts::FieldTypeWithBitMask<RegFieldTypeTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsWithRawValueSipCt() -> void {
        constexpr auto base_addr = peripheral::base_addr::fromRegFieldType<RegFieldTypeHead>();
        return setRegFieldsWithRawValueMipCt<base_addr, ValueToBeWritten, RegFieldTypeHead, RegFieldTypeTails...>();
    }

    #pragma endregion

    #pragma region setReg

    template<std::uint32_t BaseAddress, auto RegFieldHead, auto... RegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<RegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<RegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsMipCt() -> void {
        constexpr auto value_to_be_written = soc::reg::combine::enums_to_uint32<RegFieldHead, RegFieldTails...>();
        setRegFieldsWithRawValueMipCt<BaseAddress, value_to_be_written, decltype(RegFieldHead),decltype(RegFieldTails)...>();
    }

    template<auto RegFieldHead, auto... RegFieldTails>
    requires
        soc::reg::concepts::field_with_bit_mask<RegFieldHead> &&
        (soc::reg::concepts::field_with_bit_mask<RegFieldTails> && ...)
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    setRegFieldsSipCt() -> void {
        constexpr auto base_addr = peripheral::base_addr::fromRegField<RegFieldHead>();
        return setRegFieldsMipCt<base_addr, RegFieldHead, RegFieldTails...>();
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
        constexpr auto reg_offset                 = peripheral::reg_mem_offset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto reg_fields_tuple           = peripheral::reg_field_tuple::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto mask_to_clear              = soc::reg::combine::field_type_masks_to_uint32<RegFieldTypeHead, RegFieldTypeTails...>();
        constexpr auto mask_allowed_to_be_written = soc::reg::combine::writable_mask_from_tuple_type<decltype(reg_fields_tuple)>();
        constexpr auto mask_forbidden_to_write    = 0xffffffffU ^ mask_allowed_to_be_written;

        static_assert(
            mask_to_clear != 0U,
            "Going not clear anything, this indicates badly described register field");

        static_assert(
            (mask_forbidden_to_write  & mask_to_clear) == 0,
            "Some bit[s]/field[s] were going to be cleared into register which were not writable. It's either bug in SoC's register field description, or in the application. Check if possibly you are clearing a RO field.");

        if constexpr (mask_to_clear == mask_allowed_to_be_written) {
            // everything what can be written is to be cleared, no point reading
            soc::reg::access::writeCtAddrVal<BaseAddress + reg_offset, 0>();
        }
        else {
            const auto actual_value = soc::reg::access::readCtAddr<BaseAddress + reg_offset>();
            soc::reg::access::writeCtAddr<BaseAddress + reg_offset>(actual_value & mask_to_clear);
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
        constexpr auto base_addr = peripheral::base_addr::fromRegFieldType<RegFieldTypeHead>();
        return clearRegFieldTypesMipCt<base_addr, RegFieldTypeHead, RegFieldTypeTails...>();
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
        constexpr auto reg_offset    = peripheral::reg_mem_offset::fromRegFieldType<RegFieldTypeHead>();
        constexpr auto combined_mask = soc::reg::combine::field_type_masks_to_uint32<RegFieldTypeHead, RegFieldTypeTails...>();
        const     auto actual_value  = soc::reg::access::readCtAddr<BaseAddress + reg_offset>();

        soc::reg::access::writeCtAddr<BaseAddress + reg_offset>( actual_value & (0xffffffff ^ combined_mask) );
    }

    // TODO: requirements/concepts for arguments
    template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    keepRegFieldTypesSipCt() -> void {
        constexpr auto base_addr = peripheral::base_addr::fromRegFieldType<RegFieldTypeHead>();
        return keepRegFieldTypesMipCt<base_addr, RegFieldTypeHead, RegFieldTypeTails...>();
    }

    #pragma endregion

}
