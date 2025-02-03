//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"

namespace SoC::MemoryMappedRegister {


    template<std::uint32_t bitMaskValue>
    constexpr std::uint8_t bitMasksffsetCt() {
        std::uint8_t count = 0;
        std::uint32_t tmp = bitMaskValue;
        while ( ((tmp&0b1) == 0) && (count<31) ) {
            tmp >>= 1;
            count++;
        }
        return count;
    }


    template<std::uint32_t bitMaskValue>
    constexpr std::uint8_t bitMasksSizeCt() {
        std::uint8_t count = 0;
        std::uint32_t tmp = bitMaskValue;
        while ( ((tmp&0b1) == 0) && (count<31) ) {
            tmp >>= 1;
            count++;
        }
        count = 0;
        while (tmp&0b1) {
            tmp >>= 1;
            count++;
        }
        return count;
    }


    template<
        SoC::MemoryMappedRegister::Concepts::RegisterFieldEnumWhichContainsFieldBitMask RegisterFieldType,
        std::uint32_t registerValue>
    constexpr auto getEnumFromValue() {
        return static_cast<RegisterFieldType>(registerValue);
    }



}