//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"


namespace Peripheral::Rcc::Apb2pcenr {
    // Advanced Peripheral Bus 2 (low speed) peripheral clock enable

    using namespace Soc::Reg::FieldAccessPrivilege; // so we can use Field::AccessRights::ReadOnly...

    enum class AFIOEN_RW_AlternateFuctionInputOutputClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class IOPAEN_RW_InputOutputPortAClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class IOPAEN_RW_InputOutputPortCClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 4, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class IOPDEN_RW_InputOutputPortDClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 5, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class ACD1EN_RW_AnalogDigitalConverter1ClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 9, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class TIM1EN_RW_Timer1ClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 11, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class SPI1EN_RW_SerialPeripheralInterface1ClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 12, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    enum class USART1EN_RW_UniversalSynchronousAsynchronousReceiverTransmitter1ClockEnable: std::uint32_t {
        fieldBitMask = 0b1u << 14, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        noClock      = 0u,
        clockEnable  = fieldBitMask
    };

    constexpr std::uint32_t regMemOffset = 0x18u;

    constexpr std::tuple<
        AFIOEN_RW_AlternateFuctionInputOutputClockEnable,
        IOPAEN_RW_InputOutputPortAClockEnable,
        IOPAEN_RW_InputOutputPortCClockEnable,
        IOPDEN_RW_InputOutputPortDClockEnable,
        ACD1EN_RW_AnalogDigitalConverter1ClockEnable,
        TIM1EN_RW_Timer1ClockEnable,
        SPI1EN_RW_SerialPeripheralInterface1ClockEnable,
        USART1EN_RW_UniversalSynchronousAsynchronousReceiverTransmitter1ClockEnable
    > regFields;

    template<typename RegField>
    concept IsAnyRegField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(regFields)>();

}
