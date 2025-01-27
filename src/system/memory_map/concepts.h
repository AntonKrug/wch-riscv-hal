//
// Created by anton on 18/01/2025.
//

#pragma once

// https://en.cppreference.com/w/cpp/language/constraints
#include <concepts>

namespace SoC::MemoryConcepts {

    template<long long int address>
    concept IsValidPeripheralBaseAddress = (
        address >= 0x4000'0000 &&
        address <= 0x5005'0400 &&
        (address % 0x400) == 0);  // Each periperal is aligned to 1k (0x400)


    template<long long int address>
    concept IsWordAlignedBaseAddress = ((address % 0x4) == 0);

}