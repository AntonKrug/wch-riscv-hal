//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Apb2pcenr {
        // Advanced Peripheral Bus 2 (low speed) peripheral clock enable

        constexpr static std::uint32_t reg_mem_offset = 0x18U;

        enum class AFIOEN_RW_AlternateFuctionInputOutputClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class IOPAEN_RW_InputOutputPortAClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class IOPAEN_RW_InputOutputPortCClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 4U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class IOPDEN_RW_InputOutputPortDClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 5U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class ACD1EN_RW_AnalogDigitalConverter1ClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 9U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class TIM1EN_RW_Timer1ClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 11U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class SPI1EN_RW_SerialPeripheralInterface1ClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 12U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        enum class USART1EN_RW_UniversalSynchronousAsynchronousReceiverTransmitter1ClockEnable: std::uint32_t {
            fieldBitMask = 0b1U << 14U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = soc::reg::field_access_right::ReadWrite,

            noClock      = 0U,
            clockEnable  = fieldBitMask
        };

        constexpr static std::tuple<
            AFIOEN_RW_AlternateFuctionInputOutputClockEnable,
            IOPAEN_RW_InputOutputPortAClockEnable,
            IOPAEN_RW_InputOutputPortCClockEnable,
            IOPDEN_RW_InputOutputPortDClockEnable,
            ACD1EN_RW_AnalogDigitalConverter1ClockEnable,
            TIM1EN_RW_Timer1ClockEnable,
            SPI1EN_RW_SerialPeripheralInterface1ClockEnable,
            USART1EN_RW_UniversalSynchronousAsynchronousReceiverTransmitter1ClockEnable
        > reg_fields = {};

    };
}
