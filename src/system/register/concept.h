//
// Created by anton on 08/02/2025.
//

#pragma once

namespace Soc::Reg::Concept {

    #pragma region FieldType

    template<typename RegFieldType>
    concept FieldTypeWithFieldBitMask = requires
        { { RegFieldType::fieldBitMask }; };


    template<typename RegFieldType>
    concept FieldTypeWhithFieldAccess = requires
        { { RegFieldType::fieldAccess }; };

    template<typename RegFieldType>
    concept FieldTypeWhithMaskAndAccess =
        FieldTypeWithFieldBitMask<RegFieldType> and FieldTypeWhithFieldAccess<RegFieldType>;

    #pragma endregion

    #pragma region Field

    template<auto RegField>
    concept FieldWithFieldBitMask =
    FieldTypeWithFieldBitMask<decltype(RegField)>;

    template<auto RegField>
    concept FieldWhithFieldAccess =
        FieldTypeWhithFieldAccess<decltype(RegField)>;

    template<auto RegField>
    concept FieldWhithMaskAndAccess =
        FieldWithFieldBitMask<RegField> and FieldWhithFieldAccess<RegField>;

    #pragma endregion

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}