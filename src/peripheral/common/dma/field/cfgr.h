//
// Created by anton on 17/04/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/field_access_privilege.h"

namespace peripheral::dma {
    struct Cfgr { // NOLINT
        // Write to this whole register only when the channel is off, after setting EN=enabled, do not configure
        // (aka write to CFGR, CNTR, PADDR, MADDR) any further until it's off.
        // It can set itselff off on error, on finished transmission while cyclic mode is off

        enum class MEM2MEM_RW_MemoryToMemory: std::uint32_t { // NOLINT
            field_bit_offset = 14U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable          = 0U, // When disabled, then DIR dictates the behavior and channel needs to be started with peripheral request
            enable           = field_bit_mask,         // When enabled, the DIR is ignored, Memory-Memory *MADDR=*PADDR, started instantly after EN is set to enable. Can probably be used to read peripherals, but will not get peripheral request trigger.
        };

        enum class PL_RW_ChannelPriority: std::uint32_t { // NOLINT
            field_bit_offset = 12U,
            field_bit_mask   = 0b11U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access    = soc::reg::field_access_right::ReadWrite,

            low       = 0b00U << field_bit_offset,
            medium    = 0b01U << field_bit_offset,
            high      = 0b10U << field_bit_offset,
            very_high = 0b11U << field_bit_offset
        };

        enum class MSIZE_RW_MemoryAlignment: std::uint32_t { // NOLINT
            field_bit_offset = 10U,
            field_bit_mask   = 0b11U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            bit8     = 0b00U << field_bit_offset,
            bit16    = 0b01U << field_bit_offset,
            bit32    = 0b10U << field_bit_offset,
            reserved = 0b11U << field_bit_offset
        };

        enum class PSIZE_RW_PeripheralAlignment: std::uint32_t { // NOLINT
            field_bit_offset = 8U,
            field_bit_mask   = 0b11U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            bit8     = 0b00U << field_bit_offset,
            bit16    = 0b01U << field_bit_offset,
            bit32    = 0b10U << field_bit_offset,
            reserved = 0b11U << field_bit_offset
        };

        enum class MINC_RW_MemoryAddressIncrementMode: std::uint32_t { // NOLINT
            field_bit_offset = 7U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // Do not increment the MADDR
            enable  = field_bit_mask // Increment MADDR register depending how MSIZE field is set (1,2 or 4 byte increments)
        };

        enum class PINC_RW_PeripheralAddressIncrementMode: std::uint32_t { // NOLINT
            field_bit_offset = 6U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // Do not increment the PADDR
            enable  = field_bit_mask // Increment PADDR register depending how PSIZE field is set (1,2 or 4 byte increments)
        };

        enum class CIRC_RW_CyclicMode: std::uint32_t { // NOLINT
            field_bit_offset = 5U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // After CNT register is 0 the DMA is finished, and EN=disabled
            enable  = field_bit_mask // After CNT register is 0 it will get reloaded to its initial written value and continues transfering until EN is disabled
        };

        enum class DIR_RW_DataTransferDirection: std::uint32_t { // NOLINT
            field_bit_offset = 4U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            read_from_peripheral = 0b0U,          // if (MEM2MEM==disabled) Peripheral2Memory *MADDR=*PADDR else Memory-Memory *MADDR=*PADDR
            read_from_memory     = field_bit_mask // if (MEM2MEM==disabled) Memory2Peripheral *PADDR=*MADDR else Memory-Memory *MADDR=*PADDR
        };

        enum class TEIE_RW_TransmissionErrorInterruptEnable: std::uint32_t { // NOLINT
            field_bit_offset = 3U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // No IRQ is raised when errors occurs
            enable  = field_bit_mask // In a case of reading/writting at reserved area a IRQ will happen and also EN field gets disabled
        };

        enum class HTIE_RW_HalfTransmissionInterruptEnable: std::uint32_t { // NOLINT
            field_bit_offset = 2U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // No IRQ is raised during transmission
            enable  = field_bit_mask // IRQ will be raised when transmitting more than half of the transfer size (set in CNTR register)
        };

        enum class TCIE_RW_TransmissionCompletionInterruptEnable: std::uint32_t {  // NOLINT
            field_bit_offset = 1U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // No IRQ is raised after transmission is complete
            enable  = field_bit_mask // IRQ will be raised when transmitting completed (transferring all data set in CNTR register)
        };

        enum class EN_RW_ChannelEnable: std::uint32_t { // NOLINT
            field_bit_offset = 0U,
            field_bit_mask   = 0b1U << field_bit_offset, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            disable = 0b0U,          // Channel is off, no DMA is used
            enable  = field_bit_mask // Channel is on, DMA is used, no more writing/configuration to DMA until it's off again. HW can still turn off the channel when error occurs, or when in singleOperation the CNT counts to 0.
        };

        constexpr static std::tuple<
            MEM2MEM_RW_MemoryToMemory,
            PL_RW_ChannelPriority,
            MSIZE_RW_MemoryAlignment,
            PSIZE_RW_PeripheralAlignment,
            MINC_RW_MemoryAddressIncrementMode,
            PINC_RW_PeripheralAddressIncrementMode,
            CIRC_RW_CyclicMode,
            DIR_RW_DataTransferDirection,
            TEIE_RW_TransmissionErrorInterruptEnable,
            HTIE_RW_HalfTransmissionInterruptEnable,
            TCIE_RW_TransmissionCompletionInterruptEnable,
            EN_RW_ChannelEnable
        > reg_fields = {};

    };
}