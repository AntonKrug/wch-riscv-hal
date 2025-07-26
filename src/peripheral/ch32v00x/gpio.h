//
// Created by anton on 26/12/2024.
//

#pragma once

#include <cstdint>

#include "system/memory_map/concepts.h"
#include "system/register/access_primitives.h"
#include "system/gpio/op.h"

#define WCH_OPTIMIZE_GPIO __attribute__ ((always_inline, optimize("-Os"))) // NOLINT

// TODO // NOLINT(*-dynamic-static-initializers) for whole file

namespace peripheral::gpio{

    constexpr std::uint32_t full_of_ones = 0xffff'ffffU; // TODO: move these to some common 'lib' with inversion and static casting and shifting and masking and setting, oneOffHot

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

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumer, std::uint8_t TplPinNumber>
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

        constexpr static std::uint32_t port_number = TplPortNumer;

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
        static constexpr std::uint32_t register_pindata_shift_ct();

        static constexpr std::uint32_t register_pindata_shift(std::uint32_t unshifted_value);

        constexpr static std::uint32_t mask_pin_data_keep     = register_pindata_shift_ct<0b1U>();       // NOLINT(*-dynamic-static-initializers)
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
        const Pin &operator=(std::uint8_t value) const;

        template<PinInputDrive TplDrive>
        static constexpr soc::gpio::Op mode_input_op_ct();

        template<PinInputDrive TplDrive>
        static constexpr void mode_input_ct();

        template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
        static constexpr soc::gpio::Op mode_output_op_ct();

        template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
        static constexpr void mode_output_ct();
    };


    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber>
    struct Port { // NOLINT
        constexpr static BaseAddress   base_address        = TplBaseAddress;
        constexpr static std::uint32_t base_address_uint32 = static_cast<std::uint32_t>(TplBaseAddress);

        template<std::uint8_t TplPin>
        constexpr static Pin<TplBaseAddress, TplPortNumber, TplPin> get_pin();
    };


    #pragma endregion


    #pragma region Definition - Port


    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber>
    template<std::uint8_t TplPin>
    WCH_OPTIMIZE_GPIO constexpr Pin<TplBaseAddress, TplPortNumber, TplPin> Port<TplBaseAddress, TplPortNumber>::get_pin() {
        return Pin<TplBaseAddress, TplPortNumber, TplPin>{};
    }

    #pragma endregion

    #pragma region Defintion - Pins - mask generators - shiters - etc

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplUnshiftedValue>
    WCH_OPTIMIZE_GPIO inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_configuration_shift() {
        // 2-bit for
        // 2-bit for
        return static_cast<std::uint32_t>(TplUnshiftedValue) << static_cast<std::uint32_t>(4U * TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplUnshiftedValue>
    constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_pindata_shift_ct() {
        return static_cast<std::uint32_t>(TplUnshiftedValue) << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_pindata_shift(const std::uint32_t unshifted_value) {
        return unshifted_value << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<bool TplIsReset, bool TplIsSet>
    WCH_OPTIMIZE_GPIO inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_reset_set_shift_ct() {
        constexpr auto reset = (TplIsReset == true) ? (static_cast<std::uint32_t>(0b1U) << 16U) : 0U;
        constexpr auto set = (TplIsSet == true) ? 0b1U : 0U;
        return ( reset | set )  << static_cast<std::uint32_t>(TplPinNumber);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint32_t TplNonInvertedValue>
    WCH_OPTIMIZE_GPIO inline constexpr std::uint32_t Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::register_mask_invert() {
        return full_of_ones ^ TplNonInvertedValue;
    }

    #pragma endregion

    #pragma region Definition - Pins


    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO inline const Pin<TplBaseAddress, TplPortNumber, TplPinNumber> & Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::operator=(const std::uint8_t value) const {
        const auto old_value = soc::reg::access::readCtAddr<register_address_output_data>();
        const auto new_value = (old_value & mask_pin_data_inverted) | register_pindata_shift(value);
        soc::reg::access::writeCtAddr<register_address_output_data>(new_value);
        return *this;
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<std::uint8_t TplModeRawValue>
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_generic_raw_ct() {
        soc::reg::access::writeCtAddrVal<register_address_configuration, TplModeRawValue>();
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_generic_raw(const std::uint8_t mode_raw_value) {
        soc::reg::access::writeCtAddr<register_address_configuration>(mode_raw_value);
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinInputDrive TplDrive>
    WCH_OPTIMIZE_GPIO inline constexpr soc::gpio::Op Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_input_op_ct() {
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
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_input_ct() {
        constexpr auto op = mode_input_op_ct<TplDrive>();
        soc::gpio::execute_op<op>();
    }

    template<BaseAddress TplBaseAddress, std::uint32_t TplPortNumber, std::uint8_t TplPinNumber>
    template<PinOutputSlewRateCt TplSlewRate, bool TplIsMultiplexingAlternateFunction, PinOutputDrive TplDrive>
    WCH_OPTIMIZE_GPIO inline constexpr soc::gpio::Op Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_output_op_ct() {
        constexpr auto slew = static_cast<std::uint32_t>(TplSlewRate);
        constexpr auto drive = static_cast<std::uint32_t>(TplDrive) << static_cast<std::uint32_t>(pin_drive_bit_offset);
        constexpr auto multiplexing =
            (TplIsMultiplexingAlternateFunction == true) ?
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
    WCH_OPTIMIZE_GPIO inline constexpr void Pin<TplBaseAddress, TplPortNumber, TplPinNumber>::mode_output_ct() {
        constexpr auto op = mode_output_op_ct<TplSlewRate, TplIsMultiplexingAlternateFunction, TplDrive>();
        soc::gpio::execute_op<op>();
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
