//
// Created by anton on 03/01/2025.
//

#pragma once


#include "peripheral/ch32v00x/gpio.h"
#include "peripheral/ch32v00x/usart.h"
#include "peripheral/ch32v00x/rcc.h"


namespace soc::types {
    // TODO private, maybe local/static


    template<
        peripheral::gpio::BaseAddress TplBaseAddressA,
        peripheral::gpio::BaseAddress TplBaseAddressC,
        peripheral::gpio::BaseAddress TplBaseAddressD>
    struct GpioACD {
        constexpr static peripheral::gpio::Port<TplBaseAddressA> A = {};
        constexpr static peripheral::gpio::Port<TplBaseAddressC> C = {};
        constexpr static peripheral::gpio::Port<TplBaseAddressD> D = {};
    };


    template<
        peripheral::usart::base_address TplBaseAddress,
        std::uint8_t TplMapping,
        peripheral::gpio::Pins TplCk,
        peripheral::gpio::Pins TplTx,
        peripheral::gpio::Pins TplRx,
        peripheral::gpio::Pins TplCts,
        peripheral::gpio::Pins TplRts>
    struct UartInstance {
        constexpr static std::uint8_t           mapping = TplMapping;
        constexpr static peripheral::gpio::Pins ck      = TplCk;
        constexpr static peripheral::gpio::Pins tx      = TplTx;
        constexpr static peripheral::gpio::Pins rx      = TplRx;
        constexpr static peripheral::gpio::Pins cts     = TplCts;
        constexpr static peripheral::gpio::Pins rts     = TplRts;

        constexpr static peripheral::usart::Device<TplBaseAddress> device = {};
    };


}

