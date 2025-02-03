//
// Created by anton on 03/02/2025.
//

#pragma once

namespace SoC::MemMappedReg::Concept {

    template<typename RegisterField>
    concept RegisterFieldEnumWhichContainsFieldBitMask = requires
    { { RegisterField::fieldBitMask }; };

}