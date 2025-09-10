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

        template<typename TplType>
        constexpr auto absolute_value_ct(TplType const value) -> TplType {
            return (static_cast<std::uint32_t>(value) < 0U) ? -value : value;
        }

        template<typename TplType>
        constexpr auto round_ct(TplType const value) -> TplType {
            return static_cast<TplType>(static_cast<int>(value + 0.5F)); // NOLINT(*-incorrect-roundings)
        }

        template<std::uint32_t TplExpectedBaudRate, std::uint32_t TplActualBaudRate, double TplErrorThreshold>
        constexpr void check_baud_rate_error() {
            // https://ccsinfo.com/forum/viewtopic.php?t=51587
            constexpr double baud_error =
                absolute_value_ct(100.0F *
                     ((static_cast<double>(TplExpectedBaudRate) - static_cast<double>(TplActualBaudRate))
                         / static_cast<double>(TplExpectedBaudRate)));

            static_assert(baud_error < TplErrorThreshold, "Baud error of USARTDIV is over the threshold");
        }

    }


    template<std::uint32_t TplHclk, std::uint32_t TplExpectedBaudRate, double TplErrorThreshold = 1.0>
    constexpr static std::uint32_t calculate_usart_div_ct() {
        constexpr double expected_divider = static_cast<double>(TplHclk) / (16.0F * static_cast<double>(TplExpectedBaudRate));
        static_assert(static_cast<double>(static_cast<std::uint32_t>(1U) << 12U) > expected_divider, "USARTDIV's 12bit mantissa would overflow with these TplHclk and baudRate values");
        constexpr auto mantissa = static_cast<std::uint16_t>(expected_divider);

        constexpr double left_over = _internal::round_ct(expected_divider - static_cast<double>(mantissa));
        constexpr auto fraction = static_cast<std::uint8_t>(left_over * 16.0F);

        static_assert(fraction>0U || mantissa >0U, "Expected baud rate needs to be 16x slower than HCLK or it would result a zero USARTDIV");

        constexpr std::uint32_t actual_baud_rate = TplHclk / (static_cast<std::uint32_t>(16U) * mantissa + fraction);
        _internal::check_baud_rate_error<TplExpectedBaudRate, actual_baud_rate, TplErrorThreshold>();

        // TODO static print
        constexpr std::uint32_t usart_div = static_cast<std::uint32_t>(mantissa) << 4U | fraction;
        return usart_div;
    }


    template<std::uint32_t Hclk, BaudRate BaudRate, double ErrorThreshold = 1.0F>
    constexpr static std::uint32_t calculate_usart_div_ct() {
        return calculate_usart_div_ct<Hclk, std::to_underlying(BaudRate), ErrorThreshold>();
    }


    #pragma region Declarations


    template<base_address TplBaseAddress>
    struct Device { // NOLINT
    private:
        template<base_address TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t RegisterBaseUint32     = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t status                 = RegisterBaseUint32;          // R32_USART_STATR
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

