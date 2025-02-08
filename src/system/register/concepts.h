//
// Created by anton on 08/02/2025.
//

#pragma once

namespace SoC::Reg::Concept {

    template<typename CsrField>
    concept FieldEnumWhichContainsFieldBitMask = requires
        { { CsrField::fieldBitMask }; };

}