//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"


namespace Peripheral::Rcc::Apb1pcenr {
    // Advanced Peripheral Bus 1 (low speed) peripheral clock enable

    using namespace Soc::Reg::FieldAccessPrivilege; // so we can use Field::AccessRights::ReadOnly...

    enum class TIM2EN_RW_Timer2ClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,          // Clock signal is NOT feed to this peripheral
        clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
    };

    enum class WWDGEN_RW_WindowWatchdogClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 11, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,          // Clock signal is NOT feed to this peripheral
        clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
    };

    enum class I2C1EN_RW_InterIntegratedCircuitClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 21, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,          // Clock signal is NOT feed to this peripheral
        clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
    };

    enum class PWREN_RW_PowerInterfaceModuleClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 28, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,          // Clock signal is NOT feed to this peripheral
        clockEnable  = fieldBitMask // Clock signal is feed to this peripheral
    };

    constexpr std::uint32_t regMemOffset = 0x1Cu;

    constexpr std::tuple<
        TIM2EN_RW_Timer2ClockEnable,
        WWDGEN_RW_WindowWatchdogClockEnable,
        I2C1EN_RW_InterIntegratedCircuitClockEnable,
        PWREN_RW_PowerInterfaceModuleClockEnable
    > regFields;

    template<typename RegField>
    concept IsAnyRegField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(regFields)>();

}
