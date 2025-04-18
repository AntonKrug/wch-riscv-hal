//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include "field/dma/cfgr.h"
#include "field/dma/intfcr.h"
#include "field/dma/intfr.h"

namespace Peripheral::Dma {

    constexpr std::uint32_t baseAddr = 0x4002'0000U;

    enum class Id : std::uint32_t {
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

    constexpr auto ToInstance  = [](Id id) constexpr -> std::uint8_t { return  static_cast<std::uint32_t>(id) & 0x00fU; };
    constexpr auto ToChannel   = [](Id id) constexpr -> std::uint8_t { return (static_cast<std::uint32_t>(id) & 0x0f0U) >> 8U; };
    constexpr auto IsHwTrigger = [](Id id) constexpr -> std::uint8_t { return (static_cast<std::uint32_t>(id) & 0xf00U) >> 16U; };

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
