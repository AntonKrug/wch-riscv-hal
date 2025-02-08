//
// Created by anton on 08/02/2025.
//

#pragma once

#include <cstdint>

#include "utils/literals/timer.h"

//TODO: add define guards so including it with wrong cpu package will not be possible

namespace Soc::Clocks {
    using namespace Literals::Timer;
    constexpr std::uint32_t Hsi = 24_mhz_to_hz;  // High speed internal RC oscilator v003=24Mhz, x033/x035=48Mhz others=8Mhz
    constexpr std::uint32_t Lsi = 128_khz_to_hz; // for time, sleep modes, watchdog
}