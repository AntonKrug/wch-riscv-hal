//
// Created by Fredy on 7/26/2025.
//

#pragma once

#include <cstdint>

#include "gpio_common.h"
#include "system/memory_map/concepts.h"
#include "system/register/access_primitives.h"
#include "system/gpio/op.h"

#define WCH_OPTIMIZE_GPIO_PIN __attribute__ ((always_inline, optimize("-Os"))) // NOLINT

// TODO // NOLINT(*-dynamic-static-initializers) for whole file
// TODO right now fixated on the 8pin per PORT devices, but some have 16pin per PORT and will have to be more generic
//      in the future

namespace peripheral::gpio{

    #pragma region Declarations

    template<BaseAddress TplRegisterBase>
    struct Registers { // NOLINT
        constexpr static std::uint32_t register_base_uint32 = static_cast<std::uint32_t>(TplRegisterBase);
        constexpr static std::uint32_t configuration        = register_base_uint32;          // CFGLR, sometimes CFGHR
        constexpr static std::uint32_t input_data           = register_base_uint32 + 0x08U;  // INDR
        constexpr static std::uint32_t output_data          = register_base_uint32 + 0x0CU;  // OUTDR
        constexpr static std::uint32_t set_reset            = register_base_uint32 + 0x10U;  // BSHR
        constexpr static std::uint32_t reset                = register_base_uint32 + 0x14U;  // BCR
        constexpr static std::uint32_t configuration_lock   = register_base_uint32 + 0x18U;  // LCKR
    };

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    struct Pin {   // NOLINT
    private:
        // Registers addresses
        constexpr static std::uint32_t register_base_uint32                = static_cast<std::uint32_t>(TplBaseAddress); // NOLINT(*-dynamic-static-initializers)
        constexpr static std::uint32_t register_address_configuration      = register_base_uint32;          // CFGLR, sometimes CFGHR
        constexpr static std::uint32_t register_address_input_data         = register_base_uint32 + 0x08U;  // INDR
        constexpr static std::uint32_t register_address_output_data        = register_base_uint32 + 0x0CU;  // OUTDR
        constexpr static std::uint32_t register_address_set_reset          = register_base_uint32 + 0x10U;  // BSHR
        constexpr static std::uint32_t register_address_reset              = register_base_uint32 + 0x14U;  // BCR
        constexpr static std::uint32_t register_address_configuration_lock = register_base_uint32 + 0x18U;  // LCKR

        constexpr static std::uint32_t port_number = TplPortNumber;

        // Generic mask
        template<std::uint32_t TplNonInvertedValue>
        static constexpr std::uint32_t register_mask_invert();

        // Masks and conversions for configuration register
        template<std::uint8_t TplUnshiftedValue>
        static constexpr std::uint32_t register_configuration_shift();

        constexpr static std::uint32_t mask_configuration_keep     = register_configuration_shift<0b1111U>();  // NOLINT(*-dynamic-static-initializers)
        constexpr static std::uint32_t mask_configuration_inverted = register_mask_invert<mask_configuration_keep>(); // NOLINT(*-dynamic-static-initializers)

        // Masks and conversions for Input, Output, Reset (not Reset/Set), Port Lock registers
        template<std::uint8_t TplUnshiftedValue>
        static constexpr std::uint32_t register_pin_data_shift_ct();

        static constexpr std::uint32_t register_pin_data_shift(std::uint32_t unshifted_value);

        constexpr static std::uint32_t mask_pin_data_keep     = register_pin_data_shift_ct<0b1U>();       // NOLINT(*-dynamic-static-initializers)
        constexpr static std::uint32_t mask_pin_data_inverted = register_mask_invert<mask_pin_data_keep>();   // NOLINT(*-dynamic-static-initializers)

        // Masks and conversions for Reset/Set register
        template<bool TplIsReset, bool TplIsSet>
        static constexpr std::uint32_t register_reset_set_shift_ct();

        constexpr static std::uint32_t mask_reset_set_keep     = register_reset_set_shift_ct<true, true>(); // NOLINT(*-dynamic-static-initializers)
        constexpr static std::uint32_t mask_reset_set_inverted = register_mask_invert<mask_reset_set_keep>();   // NOLINT(*-dynamic-static-initializers)

        // Helper methods
        template<std::uint8_t TplModeRawValue>
        static constexpr void mode_generic_raw_ct();

        static constexpr void mode_generic_raw(std::uint8_t mode_raw_value);

    public:
        constexpr static std::uint32_t port_base_address = static_cast<std::uint32_t>(TplBaseAddress); // NOLINT(*-dynamic-static-initializers)
        constexpr static std::uint8_t  pin_number        = TplPinNumber; // NOLINT(*-dynamic-static-initializers)

        // Pin methods
        const Pin& operator=(std::uint8_t value) const; // NOLINT(*-unconventional-assign-operator) we are returning Pin& https://www.reddit.com/r/cpp_questions/comments/12wamum/clangtidy_operator_should_return_myclass/

        template<PinInputDrive TplDrive>
        static constexpr soc::gpio::Op mode_input_op_ct();

        template<PinInputDrive TplDrive>
        static constexpr void mode_input_ct();

        template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
        static constexpr soc::gpio::Op mode_output_op_ct();

        template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
        static constexpr void mode_output_ct();
    };

    #pragma endregion


    #pragma region Defintion - Pins - mask generators - shiters - etc

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplUnshiftedValue>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_configuration_shift() {
        // 2-bit for
        // 2-bit for
        return static_cast<std::uint32_t>(TplUnshiftedValue) << static_cast<std::uint32_t>(4U * TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplUnshiftedValue>
    constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_pin_data_shift_ct() {
        return static_cast<std::uint32_t>(TplUnshiftedValue) << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_pin_data_shift(const std::uint32_t unshifted_value) {
        return unshifted_value << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<bool TplIsReset, bool TplIsSet>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_reset_set_shift_ct() {
        constexpr auto reset = (TplIsReset) ? (static_cast<std::uint32_t>(1U) << 16U) : 0U;
        constexpr auto set = (TplIsSet) ? 1U : 0U;
        return ( reset | set )  << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint32_t TplNonInvertedValue>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_mask_invert() {
        return full_of_ones ^ TplNonInvertedValue;
    }

    #pragma endregion

    #pragma region Definition - Pins


    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber> // NOLINT(*-unconventional-assign-operator)
    WCH_OPTIMIZE_GPIO_PIN inline const Pin<TplBaseAddress, TplPortNumber, TplPinNumber>& Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::operator=(const std::uint8_t value) const {
        const auto old_value = soc::reg::access::readCtAddr<register_address_output_data>();
        const auto new_value = (old_value & mask_pin_data_inverted) | register_pin_data_shift(value);
        soc::reg::access::writeCtAddr<register_address_output_data>(new_value);
        return *this;
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplModeRawValue>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_generic_raw_ct() {
        soc::reg::access::writeCtAddrVal<register_address_configuration, TplModeRawValue>();
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_generic_raw(const std::uint8_t mode_raw_value) {
        soc::reg::access::writeCtAddr<register_address_configuration>(mode_raw_value);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinInputDrive TplDrive>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr soc::gpio::Op Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_input_op_ct() {
        return {
            .address               = register_address_configuration,
            .bit_set_reset_address = 0U,
            .value                 = register_configuration_shift<static_cast<std::uint32_t>(TplDrive)>(),
            .mask                  = register_configuration_shift<mask_configuration_keep>(),
            .writable              = full_of_ones,
            .port_number           = TplPortNumber
        };
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinInputDrive TplDrive>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_input_ct() {
        constexpr auto op = mode_input_op_ct<TplDrive>();
        soc::gpio::execute_op<op>();
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr soc::gpio::Op Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_output_op_ct() {
        constexpr auto slew = static_cast<std::uint32_t>(TplSlewRate);
        constexpr auto drive = static_cast<std::uint32_t>(TplDrive) << static_cast<std::uint32_t>(pin_drive_bit_offset);
        constexpr auto multiplexing =
            (TplIsMultiplexingAlternateFunction) ?
            static_cast<std::uint32_t>(1U) << static_cast<std::uint32_t>(pin_output_multiplexing_bit_offset) :
            0U;

        return {
            .address               =  register_address_configuration,
            .bit_set_reset_address = 0U,
            .value                 = register_configuration_shift<slew | drive | multiplexing>(),
            .mask                  = register_configuration_shift<mask_configuration_keep>(),
            .writable              = full_of_ones,
            .port_number           = TplPortNumber
        };
    }


    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
    WCH_OPTIMIZE_GPIO_PIN inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_output_ct() {
        constexpr auto op = mode_output_op_ct<TplSlewRate, TplIsMultiplexingAlternateFunction, TplDrive>();
        soc::gpio::execute_op<op>();
    }

    #pragma endregion


    #pragma region Defintion - other


    template<long long int Address>
    requires soc::mem_concept::IsValidPeripheralBaseAddress<Address>
    WCH_OPTIMIZE_GPIO_PIN constexpr static BaseAddress make_base_address() {
        return static_cast<BaseAddress>(Address);
    };


    #pragma endregion

}
