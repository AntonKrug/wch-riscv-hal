//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc {

    struct Apb2prstr {
        // Advanced Peripheral Bus 2 (low speed) peripheral reset register

        constexpr static std::uint32_t regMemOffset = 0x0Cu;

        enum class AFIORST_RW_AlternateFuctionInputOutputReset: std::uint32_t {
            fieldBitMask = 0b1u << 0, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class IOPARST_RW_InputOutputPortAReset: std::uint32_t {
            fieldBitMask = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class IOPCRST_RW_InputOutputPortCReset: std::uint32_t {
            fieldBitMask = 0b1u << 4, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class IOPDRST_RW_InputOutputPortDReset: std::uint32_t {
            fieldBitMask = 0b1u << 5, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class ADC1RST_RW_AnalogDigitalConverter1Reset: std::uint32_t {
            fieldBitMask = 0b1u << 9, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class TIM1RST_RW_Timer1Reset: std::uint32_t {
            fieldBitMask = 0b1u << 11, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class SPI1RST_RW_SerialPeripheralInterface1Reset: std::uint32_t {
            fieldBitMask = 0b1u << 12, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
        };

        enum class USART1RST_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Reset: std::uint32_t {
            fieldBitMask = 0b1u << 14, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noImpact     = 0u,          // Peripheral is NOT held in RESET
            keepInReset  = fieldBitMask // Peripheral held in RESET
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
        > regFields = {};

    };

}