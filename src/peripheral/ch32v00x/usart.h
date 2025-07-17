//
// Created by anton on 03/01/2025.
//

#pragma once

#include <cstdint>
#include <utility>

#include "system/memory_map/concepts.h"

// TODO usart baud rates and enum

namespace peripheral::usart{


    #pragma region Enums


    // Use as safety trick to keep addresses types of different peripherals
    // from mixing and enforce type sctrictness with this address type.
    // When created new type alias with `using`, it allowed implicit casting
    // and various fixes were a lot of syntax sugar, while this is fairly
    // small approach.
    // Alternative could be to use a library dedicated for this purpose:
    // https://github.com/dbj-systems/nothingbut
    enum class base_address : std::uint32_t;


    enum class BaudRate: std::uint32_t {
        baud4_8k   =   4'800U,
        baud9_6k   =   9'600U, // often used - slow baudrate
        baud19_2k  =  19'200U,
        baud38_4k  =  38'400U,
        baud57_6k  =  57'600U,
        baud115_2k = 115'200U, // often used - faster baudrate
        baud230_4k = 230'400U,
    };


    #pragma endregion

    namespace _internal {

        template<typename TYPE>
        constexpr auto absolute_value_ct(TYPE const value) -> TYPE {
            return (value < 0U) ? -value : value;
        }

        template<typename TYPE>
        constexpr auto round_ct(TYPE const value) -> TYPE {
            return static_cast<TYPE>(static_cast<int>(value + 0.5F)); // NOLINT(*-incorrect-roundings)
        }

        template<std::uint32_t ExpectedBaudRate, std::uint32_t ActualBaudRate, double ErrorThreshold>
        constexpr void checkBaudRateError() {
            // https://ccsinfo.com/forum/viewtopic.php?t=51587
            constexpr double baudError =
                absolute_value_ct(100.0F *
                     ((static_cast<double>(ExpectedBaudRate) - ActualBaudRate) / ExpectedBaudRate));
            static_assert(baudError < ErrorThreshold, "Bauderror of USARTDIV is over the threashold");
        }

    }


    template<std::uint32_t hclk, std::uint32_t expectedBaudRate, double errorThreshold = 1.0>
    constexpr static std::uint32_t calculateUsartDivCT() {
        constexpr double expected_divider = hclk / (16.0F * expectedBaudRate);
        static_assert((1U << 12U) > expected_divider, "USARTDIV's 12bit mantisa would overflow with these hclk and baudRate values");
        constexpr std::uint16_t mantisa = static_cast<std::uint16_t>(expected_divider);

        constexpr double leftOver = _internal::round_ct(expected_divider - mantisa);
        constexpr std::uint8_t fraction = static_cast<std::uint8_t>(leftOver * 16U);

        static_assert(fraction > 0 || mantisa >0U, "Expected baud rate needs to be 16x slower than HCLK or it would result a zero USARTDIV");

        constexpr std::uint32_t actualBaudRate = hclk / (16.0F * mantisa + fraction);
        _internal::checkBaudRateError<expectedBaudRate, actualBaudRate, errorThreshold>();

        // TODO static print
        constexpr std::uint32_t usart_div = mantisa << 4U | fraction;
        return usart_div;
    }


    template<std::uint32_t Hclk, BaudRate BaudRate, double ErrorThreshold = 1.0F>
    constexpr static std::uint32_t calculate_usart_div_ct() {
        return calculateUsartDivCT<Hclk, std::to_underlying(BaudRate), ErrorThreshold>();
    }


    #pragma region Declarations


    template<base_address TplBaseAddress>
    struct Device {
    private:
        template<base_address TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t RegisterBaseUint32     = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t status                 = RegisterBaseUint32;         // R32_USART_STATR
            constexpr static std::uint32_t data                   = RegisterBaseUint32 + 0x04U;  // R32_USART_DATAR
            constexpr static std::uint32_t baudrate               = RegisterBaseUint32 + 0x08U;  // R32_USART_BRR
            constexpr static std::uint32_t control1               = RegisterBaseUint32 + 0x0CU;  // R32_USART_CTLR1
            constexpr static std::uint32_t control2               = RegisterBaseUint32 + 0x10U;  // R32_USART_CTLR2
            constexpr static std::uint32_t control3               = RegisterBaseUint32 + 0x14U;  // R32_USART_CTLR3
            constexpr static std::uint32_t protectionAndPrescaler = RegisterBaseUint32 + 0x18U;  // R32_USART_GPR
        };

    public:
        constexpr static base_address                  baseAddress       = TplBaseAddress;
        constexpr static std::uint32_t                 baseAddressUint32 = static_cast<std::uint32_t>(baseAddress);
        struct           RegistersType<TplBaseAddress> registers         = {};
    };


    #pragma endregion


    #pragma region Definition


    template<long long int Address>
    requires soc::mem_concept::IsValidPeripheralBaseAddress<Address>
    constexpr static auto MakeBaseAddress() -> base_address {
        return static_cast<base_address>(Address);
    };


    #pragma endregion


}

