//
// Created by anton on 17/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace Peripheral::Dma {
    struct Cfgr {
        constexpr static std::uint32_t regMemAlignment = 0x20U;

        // Write to this whole register only when the channel is off, after setting EN=enabled, do not configure
        // (aka write to CFGR, CNTR, PADDR, MADDR) any further until it's off.
        // It can set itselff off on error, on finished transmission while cyclic mode is off

        enum class MEM2MEM_RW_MemoryToMemory: std::uint32_t {
            fieldBitOffset = 14U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0U << fieldBitOffset, // When disabled, then DIR dictates the behavior and channel needs to be started with peripheral request
            enable  = fieldBitMask,         // When enabled, the DIR is ignored, Memory-Memory *MADDR=*PADDR, started instantly after EN is set to enable. Can probably be used to read peripherals, but will not get peripheral request trigger.
        };

        enum class PL_RW_ChannelPriority: std::uint32_t {
            fieldBitOffset = 12U,
            fieldBitMask   = 0b11U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            low      = 0b00U << fieldBitOffset,
            medium   = 0b01U << fieldBitOffset,
            high     = 0b10U << fieldBitOffset,
            veryHigh = 0b11U << fieldBitOffset
        };

        enum class MSIZE_RW_MemoryAlignment: std::uint32_t {
            fieldBitOffset = 10U,
            fieldBitMask   = 0b11U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            bit8     = 0b00U << fieldBitOffset,
            bit16    = 0b01U << fieldBitOffset,
            bit32    = 0b10U << fieldBitOffset,
            reserved = 0b11U << fieldBitOffset
        };

        enum class PSIZE_RW_PeripheralAlignment: std::uint32_t {
            fieldBitOffset = 8U,
            fieldBitMask   = 0b11U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            bit8     = 0b00U << fieldBitOffset,
            bit16    = 0b01U << fieldBitOffset,
            bit32    = 0b10U << fieldBitOffset,
            reserved = 0b11U << fieldBitOffset
        };

        enum class MINC_RW_MemoryAddressIncrementMode: std::uint32_t {
            fieldBitOffset = 7U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            noIncrement = 0b0U << fieldBitOffset, // Do not incremet the MADDR
            increment   = fieldBitMask            // Increment MADDR register depending how MSIZE field is set (1,2 or 4 byte increments)
        };

        enum class PINC_RW_MemoryAddressIncrementMode: std::uint32_t {
            fieldBitOffset = 6U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            noIncrement = 0b0U << fieldBitOffset, // Do not incremet the PADDR
            increment   = fieldBitMask            // Increment PADDR register depending how PSIZE field is set (1,2 or 4 byte increments)
        };

        enum class CIRC_RW_CyclicMode: std::uint32_t {
            fieldBitOffset = 5U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            singleOperation = 0b0U << fieldBitOffset, // After CNT register is 0 the DMA is finished, and EN=disabled
            cyclicOperation = fieldBitMask            // After CNT register is 0 it will get reloaded to its initial written value and continues transfering until EN is disabled
        };

        enum class DIR_RW_DataTransferDirection: std::uint32_t {
            fieldBitOffset = 4U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            readFromPeripheral = 0b0U << fieldBitOffset, // if (MEM2MEM==disabled) Peripheral2Memory *MADDR=*PADDR else Memory-Memory *MADDR=*PADDR
            readFromMemory     = fieldBitMask            // if (MEM2MEM==disabled) Memory2Peripheral *PADDR=*MADDR else Memory-Memory *MADDR=*PADDR
        };

        enum class TEIE_RW_TransmissionErrorInteruptEnable: std::uint32_t {
            fieldBitOffset = 3U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0U << fieldBitOffset, // No IRQ is raised when errors occurs
            enable  = fieldBitMask            // In a case of reading/writting at reserved area a IRQ will happen and also EN field gets disabled
        };

        enum class HTIE_RW_HalfTransmissionInteruptEnable: std::uint32_t {
            fieldBitOffset = 2U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0U << fieldBitOffset, // No IRQ is raised durning transmission
            enable  = fieldBitMask            // IRQ will be raised when transmitting more than half of the transfer size (set in CNTR register)
        };

        enum class TCIE_RW_TransmissionCompletionInteruptEnable: std::uint32_t {
            fieldBitOffset = 1U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0U << fieldBitOffset, // No IRQ is raised after transmission is complete
            enable  = fieldBitMask            // IRQ will be raised when transmitting completed (transfering all data set in CNTR register)
        };

        enum class EN_RW_ChannelEnable: std::uint32_t {
            fieldBitOffset = 0U,
            fieldBitMask   = 0b1U << fieldBitOffset, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess    = Soc::Reg::FieldAccessRight::ReadWrite,

            disable = 0b0U << fieldBitOffset, // Channel is off, no DMA is used
            enable  = fieldBitMask            // Channel is on, DMA is used, no more writting/configuration to DMA until it's off again. HW can still turn off the channel when error occurs, or when in singleOperation the CNT counts to 0.
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