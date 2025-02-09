//
// Created by anton on 08/02/2025.
//

#pragma once

namespace Soc::Reg::Concept {

    template<typename RegField>
    concept FieldEnumWithFieldBitMask = requires
        { { RegField::fieldBitMask }; };

    template<typename RegField>
    concept FieldEnumWhithFieldAccess = requires
        { { RegField::fieldAccess }; };

    template<typename RegField>
    concept FieldEnumWhithMaskAndAccess =
        FieldEnumWithFieldBitMask<RegField> and FieldEnumWhithFieldAccess<RegField>;

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}