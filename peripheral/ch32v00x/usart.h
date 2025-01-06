//
// Created by anton on 03/01/2025.
//

#pragma once

#include <array>
#include <cstdint>

// TODO usart baud rates and enum

namespace Peripheral::Usart{

    #pragma region Declarations

    struct BaseAddress {
        std::uint32_t value;
    };


    template<BaseAddress TplBaseAddress>
    struct Device {
    private:
        template<BaseAddress TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t status                 = TplRegisterBase.value;         // R32_USART_STATR
            constexpr static std::uint32_t data                   = TplRegisterBase.value + 0x04;  // R32_USART_DATAR
            constexpr static std::uint32_t baudrate               = TplRegisterBase.value + 0x08;  // R32_USART_BRR
            constexpr static std::uint32_t control1               = TplRegisterBase.value + 0x0C;  // R32_USART_CTLR1
            constexpr static std::uint32_t control2               = TplRegisterBase.value + 0x10;  // R32_USART_CTLR2
            constexpr static std::uint32_t control3               = TplRegisterBase.value + 0x14;  // R32_USART_CTLR3
            constexpr static std::uint32_t protectionAndPrescaler = TplRegisterBase.value + 0x18;  // R32_USART_GPR
        };

    public:
        constexpr static BaseAddress                   baseAddress = TplBaseAddress;
        struct           RegistersType<TplBaseAddress> registers   = {};
    };

    #pragma endregion


}

