//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::rcc {

    struct Apb2prstr { //NOLINT
        // Advanced Peripheral Bus 2 (low speed) peripheral reset register

        constexpr static std::uint32_t reg_mem_offset = 0x0cU;

        enum class AFIORST_RW_AlternateFuctionInputOutputReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 0U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class IOPARST_RW_InputOutputPortAReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class IOPCRST_RW_InputOutputPortCReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 4U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class IOPDRST_RW_InputOutputPortDReset: std::uint32_t { //NOLINT
            field_bit_mask = 0b1U << 5U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class ADC1RST_RW_AnalogDigitalConverter1Reset: std::uint32_t { //NOLINT
            field_bit_mask = static_cast<std::uint32_t>(0b1U) << 9U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class TIM1RST_RW_Timer1Reset: std::uint32_t { //NOLINT
            field_bit_mask = static_cast<std::uint32_t>(0b1U) << 11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class SPI1RST_RW_SerialPeripheralInterface1Reset: std::uint32_t { //NOLINT
            field_bit_mask = static_cast<std::uint32_t>(0b1U) << 12U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        enum class USART1RST_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Reset: std::uint32_t { //NOLINT
            field_bit_mask = static_cast<std::uint32_t>(0b1U) << 14U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_impact      = 0U,          // Peripheral is NOT held in RESET
            keep_in_reset  = field_bit_mask // Peripheral held in RESET
        };

        constexpr static std::tuple<
            AFIORST_RW_AlternateFuctionInputOutputReset,
            IOPARST_RW_InputOutputPortAReset,
            IOPCRST_RW_InputOutputPortCReset,
            IOPDRST_RW_InputOutputPortDReset,
            ADC1RST_RW_AnalogDigitalConverter1Reset,
            TIM1RST_RW_Timer1Reset,
            SPI1RST_RW_SerialPeripheralInterface1Reset,
            USART1RST_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Reset
        > reg_fields = {};

    };

}