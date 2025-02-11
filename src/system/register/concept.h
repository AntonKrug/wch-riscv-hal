//
// Created by anton on 08/02/2025.
//

#pragma once

namespace Soc::Reg::Concept {

    #pragma region FieldType

    template<typename RegFieldType>
    concept FieldTypeWithBitMask = requires
        { { RegFieldType::fieldBitMask }; };


    template<typename RegFieldType>
    concept FieldTypeWhithAccess = requires
        { { RegFieldType::fieldAccess }; };

    template<typename RegFieldType>
    concept FieldTypeWhithMaskAndAccess =
        FieldTypeWithBitMask<RegFieldType> and FieldTypeWhithAccess<RegFieldType>;

    #pragma endregion

    #pragma region Field

    template<auto RegField>
    concept FieldWithBitMask =
    FieldTypeWithBitMask<decltype(RegField)>;

    template<auto RegField>
    concept FieldWhithAccess =
        FieldTypeWhithAccess<decltype(RegField)>;

    template<auto RegField>
    concept FieldWhithMaskAndAccess =
        FieldWithBitMask<RegField> and FieldWhithAccess<RegField>;

    #pragma endregion

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}