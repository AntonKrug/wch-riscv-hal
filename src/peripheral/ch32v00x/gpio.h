//
// Created by anton on 26/12/2024.
//

#pragma once

#include <array>
#include <cstdint>

#include "usart.h"
#include "system/memory_map/concepts.h"

#define WCH_OPTIMIZE_GPIO __attribute__ ((optimize("-Os"))) // NOLINT

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

    #pragma endregion


    #pragma region Declarations


    struct Pins {
        const BaseAddress port_base_address;
        const std::array<std::uint8_t, 8U> pin_numbers;

        const Pins &operator=(std::uint8_t value) const;

        template<std::uint8_t ModeRawValue>
        constexpr void mode_generic_raw_ct() const;

        template<PinInputDrive Drive>
        constexpr void mode_input_ct() const;

        template<PinOutputSlewRateCt SlewRate, bool IsMultiplexingAlternateFunction, PinOutputDrive Drive>
        constexpr void mode_output_ct() const;
    };


    template<BaseAddress TplBaseAddress>
    struct Port {
    private:
        template<BaseAddress TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t register_base_uint32 = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t configuration        = register_base_uint32;          // CFGLR, sometimes CFGHR
            constexpr static std::uint32_t input_data           = register_base_uint32 + 0x08U;  // INDR
            constexpr static std::uint32_t output_data          = register_base_uint32 + 0x0CU;  // OUTDR
            constexpr static std::uint32_t set_reset            = register_base_uint32 + 0x10U;  // BSHR
            constexpr static std::uint32_t reset                = register_base_uint32 + 0x14U;  // BCR
            constexpr static std::uint32_t configuration_lock   = register_base_uint32 + 0x18U;  // LCKR
        };

    public:
        constexpr static BaseAddress   base_address        = TplBaseAddress;
        constexpr static std::uint32_t base_address_uint32 = static_cast<std::uint32_t>(TplBaseAddress);

        struct RegistersType<TplBaseAddress> registers = {};

        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator std::uint32_t() const; // NOLINT(*-explicit-constructor)

        // constexpr static Pins get_pin(std::uint8_t pin);

        template<std::uint8_t Pin>
        constexpr static Pins get_pin();

        constexpr static Pins get_pin(const std::array<std::uint8_t, 8U> &pins);
    };


    #pragma endregion


    #pragma region Definition - Port


    template<BaseAddress TplBaseAddress>
    WCH_OPTIMIZE_GPIO constexpr Port<TplBaseAddress>::operator std::uint32_t() const { // NOLINT(*-explicit-constructor)
        return static_cast<uint32_t>(base_address);
    }


    // template<BaseAddress TplBaseAddress>
    // WCH_OPTIMIZE_GPIO constexpr Pins Port<TplBaseAddress>::get_pin(const std::uint8_t pin) {
    //     return Pins{base_address, {pin}};
    // }

    template<BaseAddress TplBaseAddress>
    template<std::uint8_t Pin>
    WCH_OPTIMIZE_GPIO constexpr Pins Port<TplBaseAddress>::get_pin() {
        return Pins{base_address, {Pin}};
    }


    /// TODO: fold and variadic template get multiple pins
    // template<BaseAddress TplBaseAddress>
    // WCH_OPTIMIZE_GPIO constexpr Pins Port<TplBaseAddress>::get_pin(const std::array<std::uint8_t, 8U> &pins) {
    //     return Pins{base_address, pins};
    // }


    #pragma endregion


    #pragma region Definition - Pins


    WCH_OPTIMIZE_GPIO inline const Pins &Pins::operator=(const std::uint8_t value) const {
        auto *ptr = reinterpret_cast<std::uint8_t *>(port_base_address); // NOLINT
        *ptr = value;
        return *this;
    }

    template<std::uint8_t ModeRawValue>
    constexpr void Pins::mode_generic_raw_ct() const {
        auto *ptr = reinterpret_cast<std::uint8_t *>(port_base_address); // NOLINT

    }

    template<PinInputDrive InputDrive>
    constexpr void Pins::mode_input_ct() const {
        constexpr auto raw_value = static_cast<std::uint8_t>(InputDrive) << pin_drive_bit_offset; // NOLINT
        mode_generic_raw_ct<raw_value>();
    }

    template<PinOutputSlewRateCt SlewRate, bool IsMultiplexingAlternateFunction, PinOutputDrive Drive>
    constexpr void Pins::mode_output_ct() const {
        constexpr std::uint8_t raw_value =
            static_cast<std::uint8_t>(SlewRate) |  // NOLINT
            static_cast<std::uint8_t>(Drive) << pin_drive_bit_offset | // NOLINT
            (IsMultiplexingAlternateFunction == true) ? 1U << pin_output_multiplexing_bit_offset : 0U; // NOLINT

        mode_generic_raw_ct<raw_value>();
    }


    // WCH_OPTIMIZE_GPIO inline auto Pins::SetOutputValue(const int value) const -> void {
    //     auto *ptr = reinterpret_cast<std::uint8_t *>(port_base_address);
    //     *ptr = value;
    // }


    #pragma endregion


    #pragma region Defintion - other


    template<long long int Address>
    requires soc::mem_concept::IsValidPeripheralBaseAddress<Address>
    WCH_OPTIMIZE_GPIO constexpr static BaseAddress make_base_address() {
        return static_cast<BaseAddress>(Address);
    };


    #pragma endregion

}
