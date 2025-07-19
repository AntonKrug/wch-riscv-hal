//
// Created by anton on 08/02/2025.
//

#pragma once

#include "system/ch32v003/clocks.h"
#include "utils/literals/timer.h"

namespace user_config {
    using namespace literals::timer;

    // you can specify clock frequency explicitly, but it might fail to meet clock constrains,
    // or describe it as fraction of the input clock which should more likely pass the constraints
    // see Rcc::Cfgr0 to see the constraints
    // constexpr std::uint32_t system_clock = 3_mhz_to_hz;
    constexpr std::uint32_t system_clock = soc::clocks::hsi / 3; // for v003 the HSI=24Mhz -> 24/3 = 8Mhz

    // RCC peripheral has Ctrl register, which has HSITRIM (InternalHighSpeedClockTrim) field
    // It's able to be superimposed on top of HSICAL[7:0] to adjust HSI frequency.
    // Range is 0-31 60kHz steps, value 16 is the default midpoint, possible values:
    // hsiTrim=0  coresponds to -960 kHz
    // hsiTrim=1  coresponds to -900 kHz
    // hsiTrim=2  coresponds to -840 kHz
    // hsiTrim=3  coresponds to -780 kHz
    // hsiTrim=4  coresponds to -720 kHz
    // hsiTrim=5  coresponds to -660 kHz
    // hsiTrim=6  coresponds to -600 kHz
    // hsiTrim=7  coresponds to -540 kHz
    // hsiTrim=8  coresponds to -480 kHz
    // hsiTrim=9  coresponds to -420 kHz
    // hsiTrim=10 coresponds to -360 kHz
    // hsiTrim=11 coresponds to -300 kHz
    // hsiTrim=12 coresponds to -240 kHz
    // hsiTrim=13 coresponds to -180 kHz
    // hsiTrim=14 coresponds to -120 kHz
    // hsiTrim=15 coresponds to  -60 kHz
    // hsiTrim=16 coresponds to    0 kHz
    // hsiTrim=17 coresponds to   60 kHz
    // hsiTrim=18 coresponds to  120 kHz
    // hsiTrim=19 coresponds to  180 kHz
    // hsiTrim=20 coresponds to  240 kHz
    // hsiTrim=21 coresponds to  300 kHz
    // hsiTrim=22 coresponds to  360 kHz
    // hsiTrim=23 coresponds to  420 kHz
    // hsiTrim=24 coresponds to  480 kHz
    // hsiTrim=25 coresponds to  540 kHz
    // hsiTrim=26 coresponds to  600 kHz
    // hsiTrim=27 coresponds to  660 kHz
    // hsiTrim=28 coresponds to  720 kHz
    // hsiTrim=29 coresponds to  780 kHz
    // hsiTrim=30 coresponds to  840 kHz
    // hsiTrim=31 coresponds to  900 kHz
    constexpr std::uint8_t hsi_trim = 10;
}
