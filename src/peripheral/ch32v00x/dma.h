//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include "field/dma/cfgr.h"
#include "field/dma/intfcr.h"
#include "field/dma/intfr.h"

namespace Peripheral::Dma {

    constexpr std::uint32_t baseAddr = 0x4002'0000;

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
        std::uint8_t TplInstance,
        std::uint8_t TplChannel,
        Direction TplDirection,
        std::uint32_t TplSourceAddress,
        std::uint32_t TplDestinationAddress,
        Priority TplPriority,
        SizeAlignment TplSourceAlignment,
        SizeAlignment TplDestinationAlignment,
        bool TplSourceIncrement,
        bool TplDestinationIncrement,
        bool TplCyclicMode,
        bool TplTransmissionErrorIrq,
        bool TplHalfTransmissionIrq,
        bool TplFullTransmissionIrq,
        bool TplEnableIrq
    >
    constexpr static auto configureDma() -> void {
    }

}
