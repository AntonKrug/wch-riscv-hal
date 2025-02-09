//
// Created by anton on 03/02/2025.
//

#pragma once

#include <cstdint>

#include "concept.h"
#include "system/register/util.h"

namespace Soc::MemMappedReg {


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
        constexpr auto offset = Soc::Reg::bitMaskOffsetCt<static_cast<std::uint32_t>(RegisterFieldType::fieldBitMask)>();
        return (registerValue & mask) >> offset;
    }




}