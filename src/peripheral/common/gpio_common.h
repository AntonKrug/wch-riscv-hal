//
// Created by Fredy on 7/26/2025.
//

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


}
