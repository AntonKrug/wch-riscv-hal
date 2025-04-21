//
// Created by anton on 08/02/2025.
//

#pragma once

namespace Soc::Reg::Concept {

    #pragma region FieldType - concepts checking various properties of the Register Field Enums

    template<typename RegFieldType>
    concept FieldTypeWithBitMask = requires
        { { RegFieldType::fieldBitMask }; };

    template<typename RegFieldType>
    concept FieldTypeWithAccess = requires
        { { RegFieldType::fieldAccess }; };

    template<typename RegFieldType>
    concept FieldTypeWithMaskAndAccess =
        FieldTypeWithBitMask<RegFieldType> and FieldTypeWithAccess<RegFieldType>;

    template<typename RegFieldType>
    concept FieldTypeWithEnableValue = requires
    { { RegFieldType::enable }; };

    template<typename RegFieldType>
    concept FieldTypeWithDisableValue = requires
    { { RegFieldType::disable }; };

    template<typename RegFieldType>
    concept FieldTypeWithFieldBitOffset = requires
    { { RegFieldType::fieldBitOffset }; };

    template<typename RegFieldType>
    concept FieldTypeWithEnableAndDisableValues =
        FieldTypeWithEnableValue<RegFieldType> and FieldTypeWithDisableValue<RegFieldType>;

    // Supports bool semantics (has enable/disable) values and has offset which can be used for conversions
    // TODO: check size of the bitmask and allow 1bit enums only
    template<typename RegFieldType>
    concept FieldTypeWithBoolInteroperability =
        FieldTypeWithEnableValue<RegFieldType> and FieldTypeWithDisableValue<RegFieldType> and FieldTypeWithFieldBitOffset<RegFieldType>;

    #pragma endregion

    #pragma region Field - helpers so we can use FieldType concepts without having to get the type with decltype

    template<auto TplRegField>
    concept FieldWithBitMask =
    FieldTypeWithBitMask<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithAccess =
        FieldTypeWithAccess<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithMaskAndAccess =
        FieldTypeWithMaskAndAccess<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithEnableValue =
        FieldTypeWithEnableValue<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithDisableValue =
        FieldTypeWithDisableValue<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithFieldBitOffset =
        FieldTypeWithFieldBitOffset<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithEnableAndDisableValues =
        FieldTypeWithEnableAndDisableValues<decltype(TplRegField)>;

    template<auto TplRegField>
    concept FieldWithBoolInteroperability =
        FieldTypeWithBoolInteroperability<decltype(TplRegField)>;

    #pragma endregion

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}