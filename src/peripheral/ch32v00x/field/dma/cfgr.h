//
// Created by anton on 17/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Dma {
    struct Cfgr {
        constexpr static std::uint32_t regMemAlignment = 0x20;

        // Write to this whole register only when the channel is off, after setting EN=enabled, do not configure
        // (aka write to CFGR, CNTR, PADDR, MADDR) any further until it's off

        enum class MEM2MEM_RW_MemoryToMemory: std::uint32_t {
            fieldBitOffset = 14,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0 << fieldBitOffset, // When disabled, then DIR dictates the behavior
            enable  = fieldBitMask,        // When enabled, the DIR is ignored, Memory-Memory *MADDR=*PADDR
        };

        enum class PL_RW_ChannelPriority: std::uint32_t {
            fieldBitOffset = 12,
            fieldBitMask   = 0b11u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            low      = 0b00u << fieldBitOffset,
            medium   = 0b01u << fieldBitOffset,
            high     = 0b10u << fieldBitOffset,
            veryHigh = 0b11u << fieldBitOffset
        };

        enum class MSIZE_RW_MemoryAlignment: std::uint32_t {
            fieldBitOffset = 10,
            fieldBitMask   = 0b11u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            bit8     = 0b00u << fieldBitOffset,
            bit16    = 0b01u << fieldBitOffset,
            bit32    = 0b10u << fieldBitOffset,
            reserved = 0b11u << fieldBitOffset
        };

        enum class PSIZE_RW_PeripheralAlignment: std::uint32_t {
            fieldBitOffset = 8,
            fieldBitMask   = 0b11u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            bit8     = 0b00u << fieldBitOffset,
            bit16    = 0b01u << fieldBitOffset,
            bit32    = 0b10u << fieldBitOffset,
            reserved = 0b11u << fieldBitOffset
        };

        enum class MINC_RW_MemoryAddressIncrementMode: std::uint32_t {
            fieldBitOffset = 7,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            noIncrement = 0b0u << fieldBitOffset, // Do not incremet the MADDR
            increment   = fieldBitMask            // Increment MADDR register depending how MSIZE field is set (1,2 or 4 byte increments)
        };

        enum class PINC_RW_MemoryAddressIncrementMode: std::uint32_t {
            fieldBitOffset = 6,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            noIncrement = 0b0u << fieldBitOffset, // Do not incremet the PADDR
            increment   = fieldBitMask            // Increment PADDR register depending how PSIZE field is set (1,2 or 4 byte increments)
        };

        enum class CIRC_RW_CyclicMode: std::uint32_t {
            fieldBitOffset = 5,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            singleOperation = 0b0u << fieldBitOffset, // After CNT register is 0 the DMA is finished
            cyclicOperation = fieldBitMask            // After CNT register is 0 it will get reloaded to its initial written value and continues transfering until EN is disabled
        };

        enum class DIR_RW_DataTransferDirection: std::uint32_t {
            fieldBitOffset = 4,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            readFromPeripheral = 0b0u << fieldBitOffset, // if (MEM2MEM==disabled) Peripheral2Memory *MADDR=*PADDR else Memory-Memory *MADDR=*PADDR
            readFromMemory     = fieldBitMask            // if (MEM2MEM==enabled)  Memory2Peripheral *PADDR=*MADDR else Memory-Memory *MADDR=*PADDR
        };

        enum class TEIE_RW_TransmissionErrorInteruptEnable: std::uint32_t {
            fieldBitOffset = 3,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0u << fieldBitOffset, // No IRQ is raised when errors occurs
            enable  = fieldBitMask            // In a case of reading/writting at reserved area a IRQ will happen and also EN field gets disabled
        };

        enum class HTIE_RW_HalfTransmissionInteruptEnable: std::uint32_t {
            fieldBitOffset = 2,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0u << fieldBitOffset, // No IRQ is raised durning transmission
            enable  = fieldBitMask            // IRQ will be raised when transmitting more than half of the transfer size (set in CNTR register)
        };

        enum class TCIE_RW_TransmissionCompletionInteruptEnable: std::uint32_t {
            fieldBitOffset = 1,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0u << fieldBitOffset, // No IRQ is raised after transmission is complete
            enable  = fieldBitMask            // IRQ will be raised when transmitting completed (transfering all data set in CNTR register)
        };

        enum class EN_RW_ChannelEnable: std::uint32_t {
            fieldBitOffset = 0,
            fieldBitMask   = 0b1u << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0u << fieldBitOffset, // Channel is off, no DMA is used
            enable  = fieldBitMask            // Channel is on, DMA is used, no more writting/configuration to DMA until it's off again. HW can still turn off the channel when error occurs.
        };

        constexpr static std::tuple<
            MEM2MEM_RW_MemoryToMemory,
            PL_RW_ChannelPriority,
            MSIZE_RW_MemoryAlignment,
            PSIZE_RW_PeripheralAlignment,
            MINC_RW_MemoryAddressIncrementMode,
            PINC_RW_MemoryAddressIncrementMode,
            CIRC_RW_CyclicMode,
            DIR_RW_DataTransferDirection,
            TEIE_RW_TransmissionErrorInteruptEnable,
            HTIE_RW_HalfTransmissionInteruptEnable,
            TCIE_RW_TransmissionCompletionInteruptEnable,
            EN_RW_ChannelEnable
        > regFields = {};

    };
}