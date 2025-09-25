//
// Created by Fredy on 7/26/2025.
//

// TOO: Assert that only PinOutputSlewRateCt fast is used with CH32V00X 2,4,6,7, they can use only 1 speed as output

#pragma once

#include <cstdint>

namespace peripheral::gpio {


    #pragma region Enums and constants

    constexpr std::uint32_t full_of_ones = 0xffff'ffffU; // TODO: move these to some common 'lib' with inversion and static casting and shifting and masking and setting, oneOffHot

    // Use as safety trick to keep addresses types of different peripherals
    // from mixing and enforce type strictness with this address type.
    // When created new type alias with `using`, it allowed implicit casting
    // and various fixes were a lot of syntax sugar, while this is fairly
    // small approach.
    // Alternative could be to use a library dedicated for this purpose:
    // https://github.com/dbj-systems/nothingbut
    enum class BaseAddress : std::uint32_t;  // NOLINT


    enum class PinConfiguration: std::uint8_t { // NOLINT
        input_analog                            = 0b00'00U, // input mode for analog peripherals such as ADC
        input_floating                          = 0b01'00U, // input high-Z mode, the default after reset
        input_pull_up_or_down                   = 0b10'00U, // input up/down is set with OUTDR

        output_push_pull_slow_rate              = 0b00'10U, // 5Mhz
        output_open_drain_slow_rate             = 0b01'10U, // 5Mhz
        alternate_output_push_pull_slow_rate    = 0b10'10U, // 5Mhz
        alternate_output_open_drain_slow_rate   = 0b11'10U, // 5Mhz

        output_push_pull_normal_rate            = 0b00'01U, // 10Mhz
        output_open_drain_normal_rate           = 0b01'01U, // 10Mhz
        alternate_output_push_pull_normal_rate  = 0b10'01U, // 10Mhz
        alternate_output_open_drain_normal_rate = 0b11'01U, // 10Mhz

        // 2,4,6, 7 support only fast rate for output
        output_push_pull_fast_rate              = 0b00'11U, // 50Mhz for all, expect 30Mhz for CH32V00X (3, 2, 4, 6, 7)
        output_open_drain_fast_rate             = 0b01'11U, // 50Mhz for all, expect 30Mhz for CH32V00X (3, 2, 4, 6, 7)
        alternate_output_push_pull_fast_rate    = 0b10'11U, // 50Mhz for all, expect 30Mhz for CH32V00X (3, 2, 4, 6, 7)
        alternate_output_open_drain_fast_rate   = 0b11'11U, // 50Mhz for all, expect 30Mhz for CH32V00X (3, 2, 4, 6, 7)
    };

    // pin mode state after reset (valid value for all current chips)
    constexpr std::uint8_t pin_reset_mode = static_cast<std::uint8_t>(PinConfiguration::input_floating);
    constexpr std::uint8_t pin_reset_out_data_offset = 4U;

    constexpr std::uint8_t pin_reset_out_data = 0U; // output data register is relevant when using floating input
    constexpr std::uint8_t pin_reset_value_offset = 1U;

    // the drive offsets for input and output are the same
    constexpr std::uint16_t pin_drive_bit_offset = 2U;

    enum class PinOutputSlewRateCt: std::uint8_t { // NOLINT
        slow   = 0b10U, // 5MHz for all, except 2MHz for CH32V003
        normal = 0b01U, // 10MHz
        fast   = 0b11U  // 50MHz for all, except 30MHz for CH32V00X  (3, 2, 4, 6, 7, it's only supported mode for 2, 4, 6, 7)
    };

    constexpr std::uint8_t pin_output_multiplexing_bit_offset = 3U; // NOLINT

    enum class PinOutputDrive: std::uint8_t { // NOLINT
        push_pull  = 0b0U, // Driven by OUTDR value
        open_drain = 0b1U,
    };

    enum class PinInputDrive: std::uint8_t { // NOLINT
        analog            = 0b00U, // For analog peripherals such as ADC
        floating          = 0b01U, // High-Z mode
        pull_up_pull_down = 0b10U  // Driven by OUTDR value
    };

    #pragma endregion


}
