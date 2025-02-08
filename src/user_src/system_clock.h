//
// Created by anton on 08/02/2025.
//

#pragma once

#include "system/ch32v003/clocks.h"
#include "utils/literals/timer.h"

namespace UserConfig {
    using namespace Literals::Timer;

    constexpr std::uint32_t systemClock = 3_mhz_to_hz;  // you can specify clock explicitly, but it might fail to meet clock constrains
    // constexpr std::uint32_t systemClock = Soc::Clocks::Hsi / 3;
}
