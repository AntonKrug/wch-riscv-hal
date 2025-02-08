//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>

#include "concepts.h"
#include "system/register/utils.h"

namespace SoC::MemMappedReg {


    template<Concept::RegisterFieldEnumWhichContainsFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawToEnum(const std::uint32_t registerValue) {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        return static_cast<RegisterFieldType>(mask & registerValue);
    }


    template<Concept::RegisterFieldEnumWhichContainsFieldBitMask RegisterFieldType>
    inline constexpr auto
    __attribute__ ((always_inline))
    rawToNormalized(const std::uint32_t registerValue) -> std::uint32_t {
        constexpr auto mask = static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask);
        constexpr auto offset = SoC::Reg::bitMaskOffsetCt<static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask)>();
        return (registerValue & mask) >> offset;
    }




}