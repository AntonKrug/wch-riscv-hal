//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"


namespace Peripheral::Rcc::Ahbpcenr {
    // Advanced High-performance Bus peripheral clock enable

    using namespace Soc::Reg::FieldAccessPrivilege; // so we can use Field::AccessRights::ReadOnly...

    enum class DMA1EN_RW_DirectMemoryAccess1Enable: std::uint32_t {
        fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0u,
        enable       = fieldBitMask
    };

    enum class SRAMEN_RW_SRAM_Clock_Durning_Sleep_Enable: std::uint32_t {
        fieldBitMask = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0u,          // The SRAM clock disabled durning sleep
        enable       = fieldBitMask // The SRAM clock enabled durning sleep
    };

    constexpr std::uint32_t regMemOffset = 0x14u;

    constexpr std::tuple<
        DMA1EN_RW_DirectMemoryAccess1Enable,
        SRAMEN_RW_SRAM_Clock_Durning_Sleep_Enable
    > regFields;

    template<typename RegField>
    concept IsAnyRegField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(regFields)>();


}
