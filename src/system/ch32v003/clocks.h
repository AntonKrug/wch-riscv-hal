//
// Created by anton on 08/02/2025.
//

#pragma once

#include <cstdint>

#include "utils/literals/timer.h"

//TODO: add define guards so including it with wrong cpu package will not be possible

namespace soc::clocks {
    using namespace literals::timer;
    constexpr std::uint32_t hsi = 24_mhz_to_hz;  // High speed internal RC oscillator v003=24Mhz, x033/x035=48Mhz others=8Mhz
    constexpr std::uint32_t lsi = 128_khz_to_hz; // for time, sleep modes, watchdog
}