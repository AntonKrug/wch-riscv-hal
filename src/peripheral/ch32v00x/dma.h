//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include "field/dma/cfgr.h"
#include "field/dma/intfcr.h"
#include "field/dma/intfr.h"

namespace Peripheral::Dma {

    constexpr std::uint32_t baseAddr = 0x4002'0000U;
    constexpr std::uint16_t instanceOffset = 0x400U;

    // DMA boundaries section and sentences are confusing and in some cases do not make sense
    // "penultimate digit" second last number 0, 1, 2
    // DMA 1 channels must be on 64K bondaries 0-64K or 64k-128k 63k-65k
    // CH32v307VCT6 with 128k ram
    // DMA1 channel 2, 3, 4, 5 128k boundaries
    // DMA1 channel 1, 6, 7 needs 64k boundaries

    enum class Id: std::uint32_t {
        // DMA1 ch1 triggers
        Adc1HwTrigger     = 0x011U,
        Tim2Ch3HwTrigger  = 0x011U,
        Dma1Ch1SwTrigger  = 0x111U,

        // DMA1 ch2 triggers
        Spi1RxHwTrigger   = 0x012U,
        Tim1Ch1HwTrigger  = 0x012U,
        Tim2UpHwTrigger   = 0x012U,
        Dma1Ch2SwTrigger  = 0x112U,

        // DMA1 ch3 triggers
        Spi1TxHwTrigger   = 0x013U,
        Tim1Ch2HwTrigger  = 0x013U,
        Dma1Ch3SwTrigger  = 0x113U,

        // DMA1 ch4 triggers
        Usart1TxHwTrigger = 0x014U,
        Tim1Ch4HwTrigger  = 0x014U,
        Tim1TrigHwTrigger = 0x014U,
        Tim1ComHwTrigger  = 0x014U,
        Dma1Ch4SwTrigger  = 0x114U,

        // DMA1 ch5 triggers
        Usart1RxHwTrigger = 0x015U,
        Tim1UpHwTrigger   = 0x015U,
        Tim2Ch1HwTrigger  = 0x015U,
        Dma1Ch5SwTrigger  = 0x115U,

        // DMA1 ch6 triggers
        I2c1TxHwTrigger   = 0x016U,
        Tim1Ch3HwTrigger  = 0x016U,
        Dma1Ch6SwTrigger  = 0x116U,

        // DMA1 ch7 triggers
        I2cRxHwTrigger    = 0x017U,
        Tim2Ch2HwTrigger  = 0x017U,
        Tim2Ch4HwTrigger  = 0x017U,
        Dma1Ch7SwTrigger  = 0x117U,
    };

    constexpr auto ToInstanceAndChannel = [](Id id) constexpr -> std::uint16_t {
        return static_cast<std::uint16_t>(static_cast<std::uint32_t>(id) & 0x0ffU);
    };

    constexpr auto ToInstance = [](Id id) constexpr -> std::uint8_t  {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(id) & 0x00fU);
    };

    constexpr auto ToChannel = [](Id id) constexpr -> std::uint8_t  {
        return static_cast<std::uint8_t>((static_cast<std::uint32_t>(id) & 0x0f0U) >> 8U);
    };

    constexpr auto IsHwTrigger = [](Id id) constexpr -> std::uint8_t  {
        return static_cast<std::uint8_t>((static_cast<std::uint32_t>(id) & 0xf00U) >> 16U);
    };

    template<Id TplIdHead, Id... TplIdTail>
    constexpr auto noDuplicateId() -> void {
        // Check if current HEAD has duplicate in the TAIL section
        static_assert(
            ( (ToInstanceAndChannel(TplIdHead) != ToInstanceAndChannel(TplIdTail)) && ... ),
            "Duplicate IDs used, there might be overlap between HW and SW triggers, or multiple HW triggers used of the same instance and channel");

        // If there are 2 or more TAIL entries (after truncating HEAD), then
        // make the first TAIL entry as new HEAD and then keep checking for more combinations
        if constexpr (sizeof...(TplIdTail) >= 2U) {
            noDuplicateId<TplIdTail...>();
        }
    }

    enum class Direction {
        PeripheralToMemory, // MEM2MEM=0, DIR=0  *MADDR=*PADDR
        MemoryToPeripheral, // MEM2MEM=0, DIR=1  *PADDR=*MADDR
        MemoryToMemory      // MEM2MEM=1, DIR=x  *MADDR=*PADDR
    };

    enum class Priority {
        low,
        medium,
        high,
        veryHigh
    };

    enum class SizeAlignment {
        byte,
        word,
        doubleWord
    };

    template<
        Id             TplRequester,
        Direction      TplDirection,
        std::uintptr_t TplSourceAddress,
        SizeAlignment  TplSourceAlignment,
        bool           TplSourceIncrement,
        std::uintptr_t TplDestinationAddress,
        SizeAlignment  TplDestinationAlignment,
        bool           TplDestinationIncrement,
        std::uint16_t  TplSizeOfTransmission,
        bool           TplCyclicMode,
        Priority       TplPriority,
        bool           TplTransmissionErrorIrq,
        bool           TplHalfTransmissionIrq,
        bool           TplFullTransmissionIrq,
        bool           TplEnableDma
    >
    constexpr auto initDmaGenericCt() -> void {
        constexpr std::uint8_t instance = ToInstance(TplRequester);
        constexpr std::uint8_t channel  = ToChannel(TplRequester);
        constexpr bool isHwTrigger      = IsHwTrigger(TplRequester);


        // static_assert(
        //     instance == 1U &&
        //     (channel == 2U || channel == 3U || channel == 4U || channel == 5U) &&
        //     TplSourceAddress < 64535, "Breaking boundary");
    }


}
