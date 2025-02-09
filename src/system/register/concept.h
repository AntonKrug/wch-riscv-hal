//
// Created by anton on 08/02/2025.
//

#pragma once

namespace Soc::Reg::Concept {

    template<typename CsrField>
    concept FieldEnumWhichContainsFieldsBitMask = requires
        { { CsrField::fieldBitMask }; };

    // template<typename... CsrFields>
    // concept FieldEnumWhichContainsFieldsBitMask =
    //     (... && requires { { CsrFields::fieldBitMask }; });
}