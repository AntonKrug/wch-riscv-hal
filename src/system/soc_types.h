//
// Created by anton on 03/01/2025.
//

#pragma once


#include "peripheral/common/gpio_port.h"
#include "peripheral/ch32v00x/usart.h"
#include "peripheral/ch32v00x/rcc.h"


namespace soc::types {
    // TODO private, maybe local/static


    template<
        peripheral::gpio::BaseAddress TplBaseAddressA,
        peripheral::gpio::BaseAddress TplBaseAddressC,
        peripheral::gpio::BaseAddress TplBaseAddressD>
    struct GpioACD {
        constexpr static peripheral::gpio::Port<TplBaseAddressA, 0U> a = {};
        constexpr static peripheral::gpio::Port<TplBaseAddressC, 2U> c = {};
        constexpr static peripheral::gpio::Port<TplBaseAddressD, 3U> d = {};
    };


    template<
        peripheral::usart::base_address TplBaseAddress,
        std::uint8_t TplMapping,
        peripheral::gpio::Pin TplCk,
        peripheral::gpio::Pin TplTx,
        peripheral::gpio::Pin TplRx,
        peripheral::gpio::Pin TplCts,
        peripheral::gpio::Pin TplRts>
    struct UartInstance {
        constexpr static std::uint8_t          mapping = TplMapping;
        constexpr static peripheral::gpio::Pin ck      = TplCk;
        constexpr static peripheral::gpio::Pin tx      = TplTx;
        constexpr static peripheral::gpio::Pin rx      = TplRx;
        constexpr static peripheral::gpio::Pin cts     = TplCts;
        constexpr static peripheral::gpio::Pin rts     = TplRts;

        constexpr static peripheral::usart::Device<TplBaseAddress> device = {};
    };


}

