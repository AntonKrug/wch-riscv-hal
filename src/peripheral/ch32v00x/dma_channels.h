//
// Created by anton on 26/04/2025.
//

#pragma once

#include <cstdint>
#include <vector>

namespace Peripheral::Dma {

    enum class Id: std::uint32_t {
        // DMA1 ch1 triggers
        Adc1HwTrigger     = 0x011U, // DMA1 ch1
        Tim2Ch3HwTrigger  = 0x011U, // DMA1 ch1
        Dma1Ch1SwTrigger  = 0x111U, // DMA1 ch1

        // DMA1 ch2 triggers
        Spi1RxHwTrigger   = 0x012U, // DMA1 ch2
        Tim1Ch1HwTrigger  = 0x012U, // DMA1 ch2
        Tim2UpHwTrigger   = 0x012U, // DMA1 ch2
        Dma1Ch2SwTrigger  = 0x112U, // DMA1 ch2

        // DMA1 ch3 triggers
        Spi1TxHwTrigger   = 0x013U, // DMA1 ch3
        Tim1Ch2HwTrigger  = 0x013U, // DMA1 ch3
        Dma1Ch3SwTrigger  = 0x113U, // DMA1 ch3

        // DMA1 ch4 triggers
        Usart1TxHwTrigger = 0x014U, // DMA1 ch4
        Tim1Ch4HwTrigger  = 0x014U, // DMA1 ch4
        Tim1TrigHwTrigger = 0x014U, // DMA1 ch4
        Tim1ComHwTrigger  = 0x014U, // DMA1 ch4
        Dma1Ch4SwTrigger  = 0x114U, // DMA1 ch4

        // DMA1 ch5 triggers
        Usart1RxHwTrigger = 0x015U, // DMA1 ch5
        Tim1UpHwTrigger   = 0x015U, // DMA1 ch5
        Tim2Ch1HwTrigger  = 0x015U, // DMA1 ch5
        Dma1Ch5SwTrigger  = 0x115U, // DMA1 ch5

        // DMA1 ch6 triggers
        I2c1TxHwTrigger   = 0x016U, // DMA1 ch6
        Tim1Ch3HwTrigger  = 0x016U, // DMA1 ch6
        Dma1Ch6SwTrigger  = 0x116U, // DMA1 ch6

        // DMA1 ch7 triggers
        I2cRxHwTrigger    = 0x017U, // DMA1 ch7
        Tim2Ch2HwTrigger  = 0x017U, // DMA1 ch7
        Tim2Ch4HwTrigger  = 0x017U, // DMA1 ch7
        Dma1Ch7SwTrigger  = 0x117U, // DMA1 ch7
    };

    // https://stackoverflow.com/questions/33241909/cannot-create-constexpr-stdvector
    constexpr Id swTriggers[] {
        Id::Dma1Ch1SwTrigger,
        Id::Dma1Ch2SwTrigger,
        Id::Dma1Ch3SwTrigger,
        Id::Dma1Ch4SwTrigger,
        Id::Dma1Ch5SwTrigger,
        Id::Dma1Ch6SwTrigger,
        Id::Dma1Ch7SwTrigger
    };

}