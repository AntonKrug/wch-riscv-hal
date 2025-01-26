//
// Created by anton on 03/01/2025.
//

#pragma once

// #include <array>
#include <cstdint>
//#include <cmath>
#include <utility>

#include "system/memory_map/concepts.h"

// TODO usart baud rates and enum

namespace Peripheral::Usart{


    #pragma region Enums


    // Use as safety trick to keep addresses types of different peripherals
    // from mixing and enforce type sctrictness with this address type.
    // When created new type alias with `using`, it allowed implicit casting
    // and various fixes were a lot of syntax sugar, while this is fairly
    // small approach.
    // Alternative could be to use a library dedicated for this purpose:
    // https://github.com/dbj-systems/nothingbut
    enum class BaseAddress : std::uint32_t;


    enum class BaudRate: std::uint32_t {
        baud4_8k   =   4'800,
        baud9_6k   =   9'600, // often used - slow baudrate
        baud19_2k  =  19'200,
        baud38_4k  =  38'400,
        baud57_6k  =  57'600,
        baud115_2k = 115'200, // often used - faster baudrate
        baud230_4k = 230'400,
    };


    #pragma endregion


    namespace {
        // https://medium.com/@mane.tako/understanding-unnamed-namespaces-in-c-7d41d367fd47

        template<typename TYPE>
        constexpr auto absoluteValueCt(TYPE const value) -> TYPE {
            return (value < 0) ? -value : value;
        }

        template<typename TYPE>
        constexpr auto roundCt(TYPE const value) -> TYPE {
            return static_cast<TYPE>(static_cast<int>(value + 0.5f));
        }

        template<std::uint32_t expectedBaudRate, std::uint32_t actualBaudRate, double errorThreshold>
        constexpr void checkBaudRateError() {
            // https://ccsinfo.com/forum/viewtopic.php?t=51587
            constexpr double baudError =
                absoluteValueCt(100.0 *
                     ((static_cast<double>(expectedBaudRate) - actualBaudRate) / expectedBaudRate));
            static_assert(baudError < errorThreshold, "Bauderror of USARTDIV is over the threashold");
        }

    }


    template<std::uint32_t hclk, std::uint32_t expectedBaudRate, double errorThreshold = 1.0>
    constexpr static std::uint32_t calculateUsartDivCT() {
        constexpr double expectedDivider = hclk / (16.0 * expectedBaudRate);
        static_assert((1<<12) > expectedDivider, "USARTDIV's 12bit mantisa would overflow with these hclk and baudRate values");
        constexpr std::uint16_t mantisa = static_cast<std::uint16_t>(expectedDivider);

        constexpr double leftOver = roundCt(expectedDivider - mantisa);
        constexpr std::uint8_t fraction = static_cast<std::uint8_t>(leftOver * 16);

        static_assert(fraction > 0 || mantisa >0, "Expected baud rate needs to be 16x slower than HCLK or it would result a zero USARTDIV");

        constexpr std::uint32_t actualBaudRate = hclk / (16.0 * mantisa + fraction);
        checkBaudRateError<expectedBaudRate, actualBaudRate, errorThreshold>();

        // TODO static print
        constexpr std::uint32_t usartDiv = mantisa << 4 | fraction;
        return usartDiv;
    }


    template<std::uint32_t hclk, BaudRate baudRate, double errorThreshold = 1.0>
    constexpr static std::uint32_t calculateUsartDivCT() {
        return calculateUsartDivCT<hclk, std::to_underlying(baudRate), errorThreshold>();
    }


    #pragma region Declarations


    template<BaseAddress TplBaseAddress>
    struct Device {
    private:
        template<BaseAddress TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t RegisterBaseUint32     = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t status                 = RegisterBaseUint32;         // R32_USART_STATR
            constexpr static std::uint32_t data                   = RegisterBaseUint32 + 0x04;  // R32_USART_DATAR
            constexpr static std::uint32_t baudrate               = RegisterBaseUint32 + 0x08;  // R32_USART_BRR
            constexpr static std::uint32_t control1               = RegisterBaseUint32 + 0x0C;  // R32_USART_CTLR1
            constexpr static std::uint32_t control2               = RegisterBaseUint32 + 0x10;  // R32_USART_CTLR2
            constexpr static std::uint32_t control3               = RegisterBaseUint32 + 0x14;  // R32_USART_CTLR3
            constexpr static std::uint32_t protectionAndPrescaler = RegisterBaseUint32 + 0x18;  // R32_USART_GPR
        };

    public:
        constexpr static BaseAddress                   baseAddress       = TplBaseAddress;
        constexpr static std::uint32_t                 baseAddressUint32 = static_cast<std::uint32_t>(baseAddress);
        struct           RegistersType<TplBaseAddress> registers         = {};
    };


    #pragma endregion


    #pragma region Definition


    template<long long int address>
    requires ValidPeripheralBaseAddress<address>
    constexpr static auto MakeBaseAddress() -> BaseAddress {
        return static_cast<BaseAddress>(address);
    };


    #pragma endregion


}

