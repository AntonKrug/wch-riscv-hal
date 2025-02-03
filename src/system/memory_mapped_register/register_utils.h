//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"

namespace SoC::MemMappedReg {


    template<std::uint32_t bitMaskValue>
    constexpr std::uint8_t bitMaskOffsetCt() {
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


    template<Concept::RegisterFieldEnumWhichContainsFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawToEnum(std::uint32_t registerValue) {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        return static_cast<RegisterFieldType>(mask & registerValue);
    }


    template<Concept::RegisterFieldEnumWhichContainsFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawToNormalized(std::uint32_t registerValue) -> std::uint32_t {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        constexpr auto offset = bitMaskOffsetCt<static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask)>();
        return (registerValue & mask) >> offset;
    }




}