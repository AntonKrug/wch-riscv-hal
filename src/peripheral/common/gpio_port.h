//
// Created by Fredy on 7/26/2025.
//

#pragma once

#include "gpio_pin.h"

//TODO do we need to even optimize constexpr?
#define WCH_OPTIMIZE_GPIO_PORT __attribute__ ((always_inline, optimize("-Os"))) // NOLINT

namespace peripheral::gpio {

#pragma region  Declaration

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber>
    struct Port { // NOLINT
        constexpr static BaseAddress   base_address        = TplBaseAddress;
        constexpr static std::uint32_t base_address_uint32 = static_cast<std::uint32_t>(TplBaseAddress);

        template<std::uint8_t TplPin>
        constexpr static Pin<TplBaseAddress, TplPortNumber, TplPin> get_pin();
    };

#pragma endregion


#pragma region Definition

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber>
    template<std::uint8_t TplPin>
    WCH_OPTIMIZE_GPIO_PORT constexpr Pin<TplBaseAddress, TplPortNumber, TplPin> Port<TplBaseAddress, TplPortNumber>::get_pin() {
        return Pin<TplBaseAddress, TplPortNumber, TplPin>{};
    }

#pragma endregion

}