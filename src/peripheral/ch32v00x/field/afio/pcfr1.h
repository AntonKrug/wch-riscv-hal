//
// Created by anton on 18/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Peripheral::Afio {

    struct Pcfr1 {
        // Remap 1

        constexpr static std::uint32_t regMemOffset = 0x04u;

        enum class SPI1_RM_RW_SerialPeripheralInterface1Remapping: std::uint32_t {
            fieldBitMask   = 0b1u, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            defaultMap     = 0,            // NSS/PC1 CK/PC5 MISO/PC7 MOSI/PC6
            alternativeMap = fieldBitMask, // NSS/PC0 CK/PC5 MISO/PC7 MOSI/PC6
        };

        enum class I2C1_RM_RW_InterIntegratedCircuit1Remapping: std::uint32_t {
            fieldBitMask   = 0b1u << 1, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            defaultMap     = 0,            // SCL/PB6 SD/PB7
            alternativeMap = fieldBitMask, // SCL/PB8 SD/PB9
        };

        enum class USART1_RM_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Remapping: std::uint32_t {
            fieldBitMask   = 0b1u << 2, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            defaultMap     = 0,            // TX/PA9 RX/PA10
            alternativeMap = fieldBitMask, // TX/PB6 RX/PB7
        };

        enum class TIM1_RM_RW_Timmer1Remapping: std::uint32_t {
            fieldBitMask    = 0b11u << 6, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess     = Soc::Reg::FieldAccessRight::ReadWrite,

            defaultMap      = 0b00, // ETR/PC5 CH1/PD2 CH2/PA1 CH3/PC3 CH4/PC4 BKIN/PC2 CH1N/PD0 CH2N/PA2 CH3N/PD1
            alternativeMap1 = 0b01, // ETR/PC5 CH1/PC6 CH2/PC7 CH3/PC0 CH4/PD3 BKIN/PC1 CH1N/PC3 CH2N/PC4 CH3N/PD1
            alternativeMap2 = 0b10, // ETR/PD4 CH1/PD2 CH2/PA1 CH3/PC3 CH4/PC4 BKIN/PC2 CH1N/PD0 CH2N/PA2 CH3N/PD1
            alternativeMap3 = 0b11, // ETR/PC2 CH1/PC4 CH2/PC7 CH3/PC5 CH4/PD4 BKIN/PC1 CH1N/PC3 CH2N/PD2 CH3N/PC6
        };

        constexpr static std::tuple<
            SPI1_RM_RW_SerialPeripheralInterface1Remapping,
            I2C1_RM_RW_InterIntegratedCircuit1Remapping
        > regFields = {};

    };

}