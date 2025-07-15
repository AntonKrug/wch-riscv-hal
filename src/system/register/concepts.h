//
// Created by anton on 08/02/2025.
//

#pragma once

namespace soc::reg::concepts {

    #pragma region FieldType - concepts checking various properties of the Register Field Enums

    template<typename RegFieldType>
    concept field_type_with_bit_mask = requires
        { { RegFieldType::fieldBitMask }; };

    template<typename RegFieldType>
    concept field_type_with_access = requires
        { { RegFieldType::fieldAccess }; };

    template<typename RegFieldType>
    concept field_type_with_mask_and_access =
        field_type_with_bit_mask<RegFieldType> and field_type_with_access<RegFieldType>;

    template<typename RegFieldType>
    concept field_type_with_enable_value = requires
    { { RegFieldType::enable }; };

    template<typename RegFieldType>
    concept field_type_with_disable_value = requires
    { { RegFieldType::disable }; };

    template<typename RegFieldType>
    concept field_type_with_field_bit_offset = requires
    { { RegFieldType::fieldBitOffset }; };

    template<typename RegFieldType>
    concept field_type_with_enable_and_disable_values =
        field_type_with_enable_value<RegFieldType> and field_type_with_disable_value<RegFieldType>;

    // Supports bool semantics (has enable/disable) values and has offset which can be used for conversions
    // TODO: check size of the bitmask and allow 1bit enums only
    template<typename RegFieldType>
    concept field_type_with_bool_interoperability =
        field_type_with_enable_value<RegFieldType> and field_type_with_disable_value<RegFieldType> and field_type_with_field_bit_offset<RegFieldType>;

    #pragma endregion

    #pragma region Field - helpers so we can use FieldType concepts without having to get the type with decltype

    template<auto TplRegField>
    concept field_with_bit_mask =
    field_type_with_bit_mask<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_access =
        field_type_with_access<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_mask_and_access =
        field_type_with_mask_and_access<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_enable_value =
        field_type_with_enable_value<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_disable_value =
        field_type_with_disable_value<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_field_bit_offset =
        field_type_with_field_bit_offset<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_enable_and_disable_values =
        field_type_with_enable_and_disable_values<decltype(TplRegField)>;

    template<auto TplRegField>
    concept field_with_bool_interoperability =
        field_type_with_bool_interoperability<decltype(TplRegField)>;

    #pragma endregion

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}