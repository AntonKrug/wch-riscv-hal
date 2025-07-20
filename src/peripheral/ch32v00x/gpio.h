//
// Created by anton on 26/12/2024.
//

#pragma once

#include <array>
#include <cstdint>
#include <tuple>

#include "usart.h"
#include "system/memory_map/concepts.h"
#include "system/register/access_primitives.h"

#define WCH_OPTIMIZE_GPIO __attribute__ ((always_inline, optimize("-Os"))) // NOLINT

namespace peripheral::gpio{


    #pragma region Enums and constants


    // Use as safety trick to keep addresses types of different peripherals
    // from mixing and enforce type strictness with this address type.
    // When created new type alias with `using`, it allowed implicit casting
    // and various fixes were a lot of syntax sugar, while this is fairly
    // small approach.
    // Alternative could be to use a library dedicated for this purpose:
    // https://github.com/dbj-systems/nothingbut
    enum class BaseAddress : std::uint32_t;


    enum class PinConfiguration: std::uint8_t { // NOLINT
        inputAnalog                           = 0b00'00U, // input mode for analog peripherals such as ADC // NOLINT
        inputFloating                         = 0b01'00U, // input high-Z mode NOLINT
        inputPullUpOrDown                     = 0b10'00U, // input up/down is set with OUTDR // NOLINT
        outputPushPullNormalSpeed             = 0b00'01U, // 10Mhz // NOLINT
        outputOpenDrainNormalSpeed            = 0b01'01U, // 10Mhz // NOLINT
        alternateFunctionPushPullNormalSpeed  = 0b10'01U, // 10Mhz // NOLINT
        alternateFunctionOpenDrainNormalSpeed = 0b11'01U, // 10Mhz // NOLINT
        outputPushPullSlowSpeed               = 0b00'10U, // 5Mhz // NOLINT
        outputOpenDrainSlowSpeed              = 0b01'10U, // 5Mhz // NOLINT
        alternateFunctionPushPullSlowSpeed    = 0b10'10U, // 5Mhz // NOLINT
        alternateFunctionOpenDrainSlowSpeed   = 0b11'10U, // 5Mhz // NOLINT
        outputPushPullFastSpeed               = 0b00'11U, // 50Mhz for all, expect 30Mhz for CH32V003 // NOLINT
        outputOpenDrainFastSpeed              = 0b01'11U, // 50Mhz for all, expect 30Mhz for CH32V003 // NOLINT
        alternateFunctionPushPullFastSpeed    = 0b10'11U, // 50Mhz for all, expect 30Mhz for CH32V003 // NOLINT
        alternateFunctionOpenDrainFastSpeed   = 0b11'11U, // 50Mhz for all, expect 30Mhz for CH32V003 // NOLINT
    };

    // the drive offsets for input and output are the same
    constexpr std::uint16_t pin_drive_bit_offset = 2U;

    enum class PinOutputSlewRateCt: std::uint8_t { // NOLINT
        slow   = 0b10U, // 5MHz for all, except 2MHz for CH32V003 // NOLINT
        normal = 0b01U, // 10MHz // NOLINT
        fast   = 0b11U  // 50MHz for all, except 30MHz for CH32V003 // NOLINT
    };

    constexpr std::uint8_t pin_output_multiplexing_bit_offset = 3U; // NOLINT

    enum class PinOutputDrive: std::uint8_t { // NOLINT
        push_pull  = 0b0U, // Driven by OUTDR value NOLINT
        open_drain = 0b1U, // NOLINT
    };

    enum class PinInputDrive: std::uint8_t { // NOLINT
        analog            = 0b00U, // For analog peripherals such as ADC // NOLINT
        floating          = 0b01U, // High-Z mode // NOLINT
        pull_up_pull_down = 0b10U  // Driven by OUTDR value // NOLINT
    };

    constexpr std::uint8_t pin_configuration_bit_offset = 4U; // NOLINT

    #pragma endregion


    #pragma region Declarations

    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    struct Pin {   // NOLINT
    private:
        template<BaseAddress TplRegisterBase>
        struct Registers {
            constexpr static std::uint32_t register_base_uint32 = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t configuration        = register_base_uint32;          // CFGLR, sometimes CFGHR
            constexpr static std::uint32_t input_data           = register_base_uint32 + 0x08U;  // INDR
            constexpr static std::uint32_t output_data          = register_base_uint32 + 0x0CU;  // OUTDR
            constexpr static std::uint32_t set_reset            = register_base_uint32 + 0x10U;  // BSHR
            constexpr static std::uint32_t reset                = register_base_uint32 + 0x14U;  // BCR
            constexpr static std::uint32_t configuration_lock   = register_base_uint32 + 0x18U;  // LCKR
        };

    public:
        constexpr static std::uint32_t port_base_address = static_cast<std::uint32_t>(TplBaseAddress);
        constexpr static std::uint8_t  pin_number        = TplPinNumber;

        const Pin &operator=(std::uint8_t value) const;

        template<std::uint8_t TplModeRawValue>
        static constexpr void mode_generic_raw_ct();

        template<PinInputDrive TplDrive>
        static constexpr void mode_input_ct();

        template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
        static constexpr void mode_output_ct();

        static constexpr std::uint32_t pin_mask();
    };


    template<BaseAddress TplBaseAddress>
    struct Port {
        constexpr static BaseAddress   base_address        = TplBaseAddress;
        constexpr static std::uint32_t base_address_uint32 = static_cast<std::uint32_t>(TplBaseAddress);

        template<std::uint8_t TplPin>
        constexpr static Pin<TplBaseAddress, TplPin> get_pin();
    };


    #pragma endregion


    #pragma region Definition - Port


    template<BaseAddress TplBaseAddress>
    template<std::uint8_t TplPin>
    WCH_OPTIMIZE_GPIO constexpr Pin<TplBaseAddress, TplPin> Port<TplBaseAddress>::get_pin() {
        return Pin<TplBaseAddress, TplPin>{};
    }

    #pragma endregion


    #pragma region Definition - Pins


    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO inline const Pin<TplBaseAddress, TplPinNumber> & Pin<TplBaseAddress, TplPinNumber>::operator=(const std::uint8_t value) const {
        soc::reg::access::writeCtAddr<Registers<TplBaseAddress>::output_data>(value);
        return *this;
    }


    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    template<std::uint8_t TplModeRawValue>
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPinNumber>::mode_generic_raw_ct() {
        soc::reg::access::writeCtAddrVal<static_cast<std::uint32_t>(port_base_address), TplModeRawValue>();
    }


    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    template<PinInputDrive TplDrive>
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPinNumber>::mode_input_ct() {
        constexpr auto raw_value = static_cast<std::uint8_t>(TplDrive) << pin_drive_bit_offset; // NOLINT
        mode_generic_raw_ct<raw_value << (TplPinNumber * pin_configuration_bit_offset)>();
    }


    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPinNumber>::mode_output_ct() {
        constexpr std::uint32_t raw_value =
            static_cast<std::uint8_t>(TplSlewRate) |  // NOLINT
            static_cast<std::uint8_t>(TplDrive) << pin_drive_bit_offset | // NOLINT
            (TplIsMultiplexingAlternateFunction == true) ? 1U << pin_output_multiplexing_bit_offset : 0U; // NOLINT

        mode_generic_raw_ct<raw_value << (TplPinNumber * pin_configuration_bit_offset)>();
    }


    template<BaseAddress TplBaseAddress, std::uint8_t TplPinNumber>
    constexpr std::uint32_t Pin<TplBaseAddress, TplPinNumber>::pin_mask() {
        return 1U << pin_number; // NOLINT
    }


    #pragma endregion


    #pragma region Defintion - other


    template<long long int Address>
    requires soc::mem_concept::IsValidPeripheralBaseAddress<Address>
    WCH_OPTIMIZE_GPIO constexpr static BaseAddress make_base_address() {
        return static_cast<BaseAddress>(Address);
    };


    #pragma endregion

}
