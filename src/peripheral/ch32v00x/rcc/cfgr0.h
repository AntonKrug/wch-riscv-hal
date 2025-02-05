//
// Created by anton on 05/02/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

// TODO: different SoC might have more registers

namespace Peripheral::Rcc::Cfgr0 {


    enum class SW_RW_SystemClockSource: std::uint32_t {
        fieldBitMask = 0b11, // not holding any settings or value, it's a bitmask for this specific field
        hsi          = 0b00, // default. internal high speed clock, is selected by HW when returning from standby and stop modes, or when HSE fails when CSSON is enabled
        hse          = 0b01, // external high speed clock, HW might automatically switch to HSI in some scenarios, read HSI comment
        pll          = 0b10, // phase locked loop generator
        reserved     = 0b11, // N/A do not use this value
    };


    enum class SWS_RO_SystemClockSourceStatus: std::uint32_t {
        fieldBitMask = 0b11'00, // not holding any settings or value, it's a bitmask for this specific field
        usingHsi     = 0b00,    // internal high speed clock used
        usingHse     = 0b01,    // external high speed clock used
        usingPll     = 0b10,    // phase locked loop generator used
        reserved     = 0b11,    // N/A do not use this value
    };


    //TODO: this register might change between SoCs
    enum class HPRE_RW_HbClockPrescaler: std::uint32_t {
        fieldBitMask            = 0b1111'00'00, // not holding any settings or value, it's a bitmask for this specific field
        sysclkDivideLinearly2   = 0b0001'00'00, // linear division of 2, prefect buffer must be on
        sysclkDivideLinearly3   = 0b0010'00'00, // default linear division of 3, prefect buffer must be on
        sysclkDivideLinearly4   = 0b0011'00'00, // linear division of 4, prefect buffer must be on
        sysclkDivideLinearly5   = 0b0100'00'00, // linear division of 5, prefect buffer must be on
        sysclkDivideLinearly6   = 0b0101'00'00, // linear division of 6, prefect buffer must be on
        sysclkDivideLinearly7   = 0b0110'00'00, // linear division of 7, prefect buffer must be on
        sysclkDivideLinearly8   = 0b0111'00'00, // linear division of 8, prefect buffer must be on
        sysclkDivideByPowers2   = 0b1000'00'00, // power by 2 division of 2, prefect buffer must be on
        sysclkDivideByPowers4   = 0b1001'00'00, // power by 2 division of 4, prefect buffer must be on
        sysclkDivideByPowers8   = 0b1010'00'00, // power by 2 division of 8, prefect buffer must be on
        sysclkDivideByPowers16  = 0b1011'00'00, // power by 2 division of 16, prefect buffer must be on
        sysclkDivideByPowers32  = 0b1100'00'00, // power by 2 division of 32, prefect buffer must be on
        sysclkDivideByPowers64  = 0b1101'00'00, // power by 2 division of 64, prefect buffer must be on
        sysclkDivideByPowers128 = 0b1110'00'00, // power by 2 division of 128, prefect buffer must be on
        sysclkDivideByPowers256 = 0b1111'00'00, // power by 2 division of 256, prefect buffer must be on
        noPrescaler             = 0,            // prescaler is off
    };


    //TODO: v2x/v3x has simpler ACDPRE, but also lower limit for maximal clock 14MHz
    template<std::uint32_t value>
    constexpr auto getAcdPrescaler() -> std::uint8_t {
        // single equation to cover both case:
        // divisor=(2*(1-MOD(lower,2))+(lower+MOD(lower,2))*POWER(2,(higher+1)*MOD(lower,2)))
        // to reverse it, first try the even case:
        // lower=divisor−2 and check if the result is even and in the lower 3bit range
        // if yes the higher can be any value, but preferably 0 and you are done
        // if not then iterate over possible higher 2bit values while calculating:
        // lower=(divisor/(2(upper+1)))-1
        // if that is in lowers range and odd, then confirm our interated higher value matches
        // calculated higher = log2(divisor/(lower+1))−1
        constexpr std::uint8_t lower3  = value & 0b00'111;
        constexpr std::uint8_t higher2 = value & 0b11'000;
        if (lower3 % 2) {
            // Lower 3bits are even(0,2,4,6), ignore the higher 2bits and just produce 2+lower3 value (2,4,6,8)
            return lower3 + 2;
        } else {
            // Lower 3bits are odd(1,3,5,7), use higher 2bits to get powers (2,4,8,16) multipled with lower3+1 (2,4,6,8)
            return (1u<<(higher2+1))*(lower3+1);
        }
    }


    enum class ADCPRE_RW_AnalogDigitalConverterClockPrescaler: std::uint32_t {
        // see getAcdPrescaler() for equation, lower 3-bits are threated differently than higher 2-bits
        fieldBitMask       = 0b11111'000'0000'00'00, // not holding any settings or value, it's a bitmask for this specific field

        hbDividedEven2var0 = 0b00000'000'0000'00'00, // default ADC = HB / 2 (must be less than 24MHz)
        hbDividedOdd4      = 0b00001'000'0000'00'00, // ADC = HB / 4 (must be less than 24MHz)
        hbDividedEven4var0 = 0b00010'000'0000'00'00, // ADC = HB / 4 (must be less than 24MHz)
        hbDividedOdd8var0  = 0b00011'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedEven6var0 = 0b00100'000'0000'00'00, // ADC = HB / 6 (must be less than 24MHz)
        hbDividedOdd12     = 0b00101'000'0000'00'00, // ADC = HB / 12 (must be less than 24MHz)
        hbDividedEven8var0 = 0b00110'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedOdd16var0 = 0b00111'000'0000'00'00, // ADC = HB / 16 (must be less than 24MHz)

        hbDividedEven2var1 = 0b01000'000'0000'00'00, // ADC = HB / 2 (must be less than 24MHz)
        hbDividedOdd8var1  = 0b01001'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedEven4var1 = 0b01010'000'0000'00'00, // ADC = HB / 4 (must be less than 24MHz)
        hbDividedOdd16var1 = 0b01011'000'0000'00'00, // ADC = HB / 16 (must be less than 24MHz)
        hbDividedEven6var1 = 0b01100'000'0000'00'00, // ADC = HB / 6 (must be less than 24MHz)
        hbDividedOdd24     = 0b01101'000'0000'00'00, // ADC = HB / 24 (must be less than 24MHz)
        hbDividedEven8var1 = 0b01110'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedOdd32var0 = 0b01111'000'0000'00'00, // ADC = HB / 32 (must be less than 24MHz)

        hbDividedEven2var2 = 0b10000'000'0000'00'00, // ADC = HB / 2 (must be less than 24MHz)
        hbDividedOdd16var2 = 0b10001'000'0000'00'00, // ADC = HB / 16 (must be less than 24MHz)
        hbDividedEven4var2 = 0b10010'000'0000'00'00, // ADC = HB / 4 (must be less than 24MHz)
        hbDividedOdd32var1 = 0b10011'000'0000'00'00, // ADC = HB / 32 (must be less than 24MHz)
        hbDividedEven6var2 = 0b10100'000'0000'00'00, // ADC = HB / 6 (must be less than 24MHz)
        hbDividedOdd48     = 0b10101'000'0000'00'00, // ADC = HB / 48 (must be less than 24MHz)
        hbDividedEven8var2 = 0b10110'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedOdd64var0 = 0b10111'000'0000'00'00, // ADC = HB / 64 (must be less than 24MHz)

        hbDividedEven2var3 = 0b11000'000'0000'00'00, // ADC = HB / 2 (must be less than 24MHz)
        hbDividedOdd32var2 = 0b11001'000'0000'00'00, // ADC = HB / 32 (must be less than 24MHz)
        hbDividedEven4var3 = 0b11010'000'0000'00'00, // ADC = HB / 4 (must be less than 24MHz)
        hbDividedOdd64var1 = 0b11011'000'0000'00'00, // ADC = HB / 64 (must be less than 24MHz)
        hbDividedEven6var3 = 0b11100'000'0000'00'00, // ADC = HB / 6 (must be less than 24MHz)
        hbDividedOdd96     = 0b11101'000'0000'00'00, // ADC = HB / 96 (must be less than 24MHz)
        hbDividedEven8var3 = 0b11110'000'0000'00'00, // ADC = HB / 8 (must be less than 24MHz)
        hbDividedOdd128    = 0b11111'000'0000'00'00, // ADC = HB / 128 (must be less than 24MHz)
    };


    // TODO on other SoCs this is bigger register, might be worth making it bigger here
    enum class PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator: std::uint32_t {
        // see getAcdPrescaler() for equation
        fieldBitMask = 0b1'00000'000'0000'00'00, // not holding any settings or value, it's a bitmask for this specific field
        hse          = 0b1'00000'000'0000'00'00, // external high speed clock is used direcly, configure before enabling PLL
        hsi          = 0b0'00000'000'0000'00'00, // default internal high speed clock is used direcly, configure before enabling PLL
    };


    //TODO: this register might change between SoCs
    enum class MCO_RW_MicrocontrolerClockPinOutput: std::uint32_t {
        fieldBitMask = 0b111'0000000'0'00000'000'0000'00'00, // not holding any settings or value, it's a bitmask for this specific field
        noOutput0    = 0b000'0000000'0'00000'000'0000'00'00, // default no clock is output
        noOutput1    = 0b001'0000000'0'00000'000'0000'00'00, // no clock is output
        noOutput2    = 0b010'0000000'0'00000'000'0000'00'00, // no clock is output
        noOutput3    = 0b011'0000000'0'00000'000'0000'00'00, // no clock is output
        sysclk       = 0b100'0000000'0'00000'000'0000'00'00, // sysclk is exposed on the pin output
        his          = 0b101'0000000'0'00000'000'0000'00'00, // his is exposed on the pin output
        noOutput6    = 0b110'0000000'0'00000'000'0000'00'00, // no clock is output
        noOutput7    = 0b111'0000000'0'00000'000'0000'00'00, // no clock is output
    };



    template<typename RegField>
    concept IsAnyRegField =
        std::is_same_v<RegField, SW_RW_SystemClockSource> ||
        std::is_same_v<RegField, SWS_RO_SystemClockSourceStatus> ||
        std::is_same_v<RegField, HPRE_RW_HbClockPrescaler> ||
        std::is_same_v<RegField, ADCPRE_RW_AnalogDigitalConverterClockPrescaler> ||
        std::is_same_v<RegField, PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator> ||
        std::is_same_v<RegField, MCO_RW_MicrocontrolerClockPinOutput>;

}