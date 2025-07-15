//
// Created by anton on 18/01/2025.
//

#pragma once

// https://en.cppreference.com/w/cpp/language/constraints
#include <concepts>

namespace soc::mem_concept {

    template<long long int Address>
    concept IsValidPeripheralBaseAddress = (
        Address >= 0x4000'0000 &&
        Address <= 0x5005'0400 &&
        (Address % 0x400) == 0);  // Each periperal is aligned to 1k (0x400)


    template<long long int Address>
    concept IsWordAlignedBaseAddress = ((Address % 0x4) == 0);

}