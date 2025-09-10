//
// Created by anton on 26/04/2025.
//

#pragma once

#include <cstdint>
#include <vector>

namespace peripheral::dma {

    enum class Id: std::uint32_t { // NOLINT
        // DMA1 ch1 triggers
        adc1_hw_trigger     = 0x011U, // DMA1 ch1
        tim2_ch3_hw_trigger  = 0x011U, // DMA1 ch1
        dma1_ch1_sw_trigger  = 0x111U, // DMA1 ch1

        // DMA1 ch2 triggers
        spi1_rx_hw_trigger   = 0x012U, // DMA1 ch2
        tim1_ch1_hw_trigger  = 0x012U, // DMA1 ch2
        tim2_up_hw_trigger   = 0x012U, // DMA1 ch2
        dma1_ch2_sw_trigger  = 0x112U, // DMA1 ch2

        // DMA1 ch3 triggers
        spi1_tx_hw_trigger   = 0x013U, // DMA1 ch3
        tim1_ch2_hw_trigger  = 0x013U, // DMA1 ch3
        dma1_ch3_sw_trigger  = 0x113U, // DMA1 ch3

        // DMA1 ch4 triggers
        usart1_tx_hw_trigger = 0x014U, // DMA1 ch4
        tim1_ch4_hw_trigger  = 0x014U, // DMA1 ch4
        tim1_trig_hw_trigger = 0x014U, // DMA1 ch4
        tim1_com_hw_trigger  = 0x014U, // DMA1 ch4
        dma1_ch4_sw_trigger  = 0x114U, // DMA1 ch4

        // DMA1 ch5 triggers
        usart1_rx_hw_trigger = 0x015U, // DMA1 ch5
        tim1_up_hw_trigger   = 0x015U, // DMA1 ch5
        tim2_ch1_hw_trigger  = 0x015U, // DMA1 ch5
        dma1_ch5_sw_trigger  = 0x115U, // DMA1 ch5

        // DMA1 ch6 triggers
        i2c1_tx_hw_trigger   = 0x016U, // DMA1 ch6
        tim1_ch3_hw_trigger  = 0x016U, // DMA1 ch6
        dma1_ch6_sw_trigger  = 0x116U, // DMA1 ch6

        // DMA1 ch7 triggers
        i2c_rx_hw_trigger    = 0x017U, // DMA1 ch7
        tim2_ch2_hw_trigger  = 0x017U, // DMA1 ch7
        tim2_ch4_hw_trigger  = 0x017U, // DMA1 ch7
        dma1_ch7_sw_trigger  = 0x117U, // DMA1 ch7
    };

    // https://stackoverflow.com/questions/33241909/cannot-create-constexpr-stdvector
    constexpr Id sw_triggers[] {
        Id::dma1_ch1_sw_trigger,
        Id::dma1_ch2_sw_trigger,
        Id::dma1_ch3_sw_trigger,
        Id::dma1_ch4_sw_trigger,
        Id::dma1_ch5_sw_trigger,
        Id::dma1_ch6_sw_trigger,
        Id::dma1_ch7_sw_trigger
    };

}