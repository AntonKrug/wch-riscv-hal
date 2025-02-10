//
// Created by anton on 08/02/2025.
//

#pragma once

#include "system/ch32v003/clocks.h"
#include "utils/literals/timer.h"

namespace UserConfig {
    using namespace Literals::Timer;

    // you can specify clock frequency explicitly, but it might fail to meet clock constrains,
    // or describe it as fraction of the input clock which should more likely pass the constrains
    // see Rcc::Cfgr0 to see the constrains
    constexpr std::uint32_t systemClock = 3_mhz_to_hz;
    // constexpr std::uint32_t systemClock = Soc::Clocks::Hsi / 3;
}
