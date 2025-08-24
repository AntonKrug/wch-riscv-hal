//
// Created by anton on 18/02/2025.
//

//TODO: not finished - 003 different from 002,004,006,007

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace peripheral::afio {

    struct Pcfr1 {
        // Remap 1

        constexpr static std::uint32_t reg_mem_offset = 0x04U;

        enum class SPI1_RM_RW_SerialPeripheralInterface1Remapping: std::uint32_t {
            fieldBitMask   = 0b1U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = soc::reg::field_access_right::ReadWrite,

            defaultMap     = 0U,           // NSS/PC1 CK/PC5 MISO/PC7 MOSI/PC6
            alternativeMap = fieldBitMask, // NSS/PC0 CK/PC5 MISO/PC7 MOSI/PC6
        };

        enum class I2C1_RM_RW_InterIntegratedCircuit1Remapping: std::uint32_t {
            fieldBitMask   = 0b1U << 1U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = soc::reg::field_access_right::ReadWrite,

            defaultMap     = 0U,           // SCL/PB6 SD/PB7
            alternativeMap = fieldBitMask, // SCL/PB8 SD/PB9
        };

        enum class USART1_RM_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Remapping: std::uint32_t {
            fieldBitMask   = 0b1U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = soc::reg::field_access_right::ReadWrite,

            defaultMap     = 0U,           // TX/PA9 RX/PA10
            alternativeMap = fieldBitMask, // TX/PB6 RX/PB7
        };

        enum class TIM1_RM_RW_Timmer1Remapping: std::uint32_t {
            fieldBitMask    = 0b11U << 6U, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess     = soc::reg::field_access_right::ReadWrite,

            defaultMap      = 0b00U, // ETR/PC5 CH1/PD2 CH2/PA1 CH3/PC3 CH4/PC4 BKIN/PC2 CH1N/PD0 CH2N/PA2 CH3N/PD1
            alternativeMap1 = 0b01U, // ETR/PC5 CH1/PC6 CH2/PC7 CH3/PC0 CH4/PD3 BKIN/PC1 CH1N/PC3 CH2N/PC4 CH3N/PD1
            alternativeMap2 = 0b10U, // ETR/PD4 CH1/PD2 CH2/PA1 CH3/PC3 CH4/PC4 BKIN/PC2 CH1N/PD0 CH2N/PA2 CH3N/PD1
            alternativeMap3 = 0b11U, // ETR/PC2 CH1/PC4 CH2/PC7 CH3/PC5 CH4/PD4 BKIN/PC1 CH1N/PC3 CH2N/PD2 CH3N/PC6
        };

        constexpr static std::tuple<
            SPI1_RM_RW_SerialPeripheralInterface1Remapping,
            I2C1_RM_RW_InterIntegratedCircuit1Remapping,
            USART1_RM_RW_UniversalSynchronousAsynchronousReceiverTransmitter1Remapping,
            TIM1_RM_RW_Timmer1Remapping
        > reg_fields = {};

    };

}