//
// Created by anton on 26/12/2024.
//

#ifndef SOC_H
#define SOC_H

#define SYSTEM_WCH
#define SYSTEM_WCH_CH32v00x
#define SYSTEM_WCH_CH32v003

#include "../../peripheral/ch32v00x/gpio.h"

struct Soc {
    struct Gpio {
        constexpr static BundledPeripheral::Gpio::Base<0x40010800> A = {};
        constexpr static BundledPeripheral::Gpio::Base<0x40011000> B = {};
        constexpr static BundledPeripheral::Gpio::Base<0x40011400> C = {};
    };
};

#endif //SOC_H
