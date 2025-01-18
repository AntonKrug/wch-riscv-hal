//
// Created by anton on 03/01/2025.
//

#pragma once


#include "../peripheral/ch32v00x/gpio.h"
#include "../peripheral/ch32v00x/usart.h"
#include "../peripheral/ch32v00x/rcc.h"


namespace Soc::Types {
    // TODO private, maybe local/static


    template<
        Peripheral::Gpio::BaseAddress TplBaseAddressA,
        Peripheral::Gpio::BaseAddress TplBaseAddressC,
        Peripheral::Gpio::BaseAddress TplBaseAddressD>
    struct GpioACD {
        constexpr static Peripheral::Gpio::Port<TplBaseAddressA> A = {};
        constexpr static Peripheral::Gpio::Port<TplBaseAddressC> C = {};
        constexpr static Peripheral::Gpio::Port<TplBaseAddressD> D = {};
    };


    template<
        Peripheral::Usart::UsartBaseAddress TplBaseAddress,
        std::uint8_t TplMapping,
        Peripheral::Gpio::Pins TplCk,
        Peripheral::Gpio::Pins TplTx,
        Peripheral::Gpio::Pins TplRx,
        Peripheral::Gpio::Pins TplCts,
        Peripheral::Gpio::Pins TplRts>
    struct UartInstance {
        constexpr static std::uint8_t           mapping     = TplMapping;
        constexpr static Peripheral::Gpio::Pins ck          = TplCk;
        constexpr static Peripheral::Gpio::Pins tx          = TplTx;
        constexpr static Peripheral::Gpio::Pins rx          = TplRx;
        constexpr static Peripheral::Gpio::Pins cts         = TplCts;
        constexpr static Peripheral::Gpio::Pins rts         = TplRts;

        constexpr static Peripheral::Usart::Device<TplBaseAddress> device = {};
    };


}

