//
// Created by anton on 05/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

// TODO: different SoC might have more registers

namespace Peripheral::Rcc {

    struct Cfgr0 {
        // Clock configuration

        constexpr static std::uint32_t regMemOffset = 0x04u;

        enum class SW_RW_SystemClockSource: std::uint32_t {
            fieldBitMask = 0b11u, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            hsi          = 0b00u, // default. internal high speed clock, is selected by HW when returning from standby and stop modes, or when HSE fails when CSSON is enabled
            hse          = 0b01u, // external high speed clock, HW might automatically switch to HSI in some scenarios, read HSI comment
            pll          = 0b10u, // phase locked loop generator

            reserved     = 0b11u, // N/A do not use this value
        };

        enum class SWS_RO_SystemClockSourceStatus: std::uint32_t {
            fieldBitMask = 0b11u << 2, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadOnly,

            usingHsi     = 0b00u << 2, // internal high speed clock used
            usingHse     = 0b01u << 2, // external high speed clock used
            usingPll     = 0b10u << 2, // phase locked loop generator used

            reserved     = 0b11u << 2, // N/A do not use this value
        };

        //TODO: this register might change between SoCs
        enum class HPRE_RW_HbClockPrescaler: std::uint32_t { // To drive HCLK
            fieldBitMask    = 0b1'111u << 4, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess     = Soc::Reg::FieldAccessRight::ReadWrite,

            noPrescaler     = 0,             // HCLK = SYSCLK (prescaler is off)

            divide2linearly = 0b0'001u << 4, // HCLK = SYSCLK / 2 (linear division of 2, prefetch buffer must be on)
            divide3linearly = 0b0'010u << 4, // HCLK = SYSCLK / 3 (default linear division of 3, prefetch buffer must be on)
            divide4linearly = 0b0'011u << 4, // HCLK = SYSCLK / 4 (linear division of 4, prefetch buffer must be on)
            divide5linearly = 0b0'100u << 4, // HCLK = SYSCLK / 5 (linear division of 5, prefetch buffer must be on)
            divide6linearly = 0b0'101u << 4, // HCLK = SYSCLK / 6 (linear division of 6, prefetch buffer must be on)
            divide7linearly = 0b0'110u << 4, // HCLK = SYSCLK / 7 (linear division of 7, prefetch buffer must be on)
            divide8linearly = 0b0'111u << 4, // HCLK = SYSCLK / 8 (linear division of 8, prefetch buffer must be on)

            divide2powers   = 0b1'000u << 4, // HCLK = SYSCLK / 2 (power by 2 division of 2, prefetch buffer must be on)
            divide4powers   = 0b1'001u << 4, // HCLK = SYSCLK / 4 (power by 2 division of 4, prefetch buffer must be on)
            divide8powers   = 0b1'010u << 4, // HCLK = SYSCLK / 8 (power by 2 division of 8, prefetch buffer must be on)
            divide16powers  = 0b1'011u << 4, // HCLK = SYSCLK / 16 (power by 2 division of 16, prefetch buffer must be on)
            divide32powers  = 0b1'100u << 4, // HCLK = SYSCLK / 32 (power by 2 division of 32, prefetch buffer must be on)
            divide64powers  = 0b1'101u << 4, // HCLK = SYSCLK / 64 (power by 2 division of 64, prefetch buffer must be on)
            divide128powers = 0b1'110u << 4, // HCLK = SYSCLK / 128 (power by 2 division of 128, prefetch buffer must be on)
            divide256powers = 0b1'111u << 4, // HCLK = SYSCLK / 256 (power by 2 division of 256, prefetch buffer must be on)
        };

        constexpr static std::uint8_t log2_int(std::uint32_t x) {
            std::uint8_t result = 0;
            while (x > 1) {
                x >>= 1;
                ++result;
            }
            return result;
        }

        template<std::uint32_t SysClock, std::uint32_t DesiredHbClock>
        constexpr static auto getHbPrescaler() -> std::uint8_t {
            static_assert(
                DesiredHbClock <= SysClock,
                "DesiredHbClock must be smaller or the same as SysClock");

            constexpr double ratioFraction = static_cast<double>(SysClock) / DesiredHbClock;
            constexpr auto ratioWhole = static_cast<std::uint32_t>(ratioFraction);

            static_assert(
                ratioFraction == static_cast<double>(ratioWhole),
                "The SysClock and DesiredHbClock frequencies can't be divided by a whole number division");

            // static_assert( ratioWhole == 3, "aaaa");

            if constexpr (ratioWhole == 1) {
                return 0u;
            } else if constexpr (ratioWhole < 9) {
                return ratioWhole - 1;
            } else {
                static_assert(ratioWhole <= 256, "The DesiredHbClock is too small for such SysClock");

                // poor's man log2
                constexpr std::uint8_t count = log2_int(ratioWhole);
                static_assert( (1u << count) == ratioWhole, "The SysClock/DesiredHbClock needs to be either smaller ratio, or a power of 2");
                return count + 7u;
            }
        }

        template<std::uint32_t SysClock, std::uint32_t DesiredHbClock>
        constexpr static auto getHbPrescalerWithOffset() -> std::uint32_t {
            return Soc::Reg::rawValueOffsetToEnumsOffsetCt<
                getHbPrescaler<SysClock, DesiredHbClock>(),
                HPRE_RW_HbClockPrescaler>();
        }

        template<std::uint32_t SysClock, std::uint32_t DesiredHbClock>
        constexpr static auto getHbPrescalerEnum() -> HPRE_RW_HbClockPrescaler {
            return static_cast<HPRE_RW_HbClockPrescaler>(getHbPrescalerWithOffset<SysClock, DesiredHbClock>());
        }

        enum class ADCPRE_RW_AnalogDigitalConverterClockPrescaler: std::uint32_t {
            // see getAcdDividerFromPrescalerFieldValue() for equation, lower 3-bits are threated differently than higher 2-bits
            fieldBitMask    = 0b11'111u << 11, // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess     = Soc::Reg::FieldAccessRight::ReadWrite,

            divide2even     = 0b00'000u << 11, // default ADC = HB / 2 (must be less than 24MHz)
            divide4odd      = 0b00'001u << 11, // ADC = HB / 4 (must be less than 24MHz)
            divide4even     = 0b00'010u << 11, // ADC = HB / 4 (must be less than 24MHz)
            divide8odd      = 0b00'011u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divideeven      = 0b00'100u << 11, // ADC = HB / 6 (must be less than 24MHz)
            divide12odd     = 0b00'101u << 11, // ADC = HB / 12 (must be less than 24MHz)
            divide8even     = 0b00'110u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divide16odd     = 0b00'111u << 11, // ADC = HB / 16 (must be less than 24MHz)

            divide2evenAlt1 = 0b01'000u << 11, // ADC = HB / 2 (must be less than 24MHz)
            divide8oddAlt1  = 0b01'001u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divide4evenAlt1 = 0b01'010u << 11, // ADC = HB / 4 (must be less than 24MHz)
            divide16oddAlt1 = 0b01'011u << 11, // ADC = HB / 16 (must be less than 24MHz)
            divide5evenAlt1 = 0b01'100u << 11, // ADC = HB / 6 (must be less than 24MHz)
            divide24odd     = 0b01'101u << 11, // ADC = HB / 24 (must be less than 24MHz)
            divide8evenAlt1 = 0b01'110u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divide32odd     = 0b01'111u << 11, // ADC = HB / 32 (must be less than 24MHz)

            divide2evenAlt2 = 0b10'000u << 11, // ADC = HB / 2 (must be less than 24MHz)
            divide16oddAlt2 = 0b10'001u << 11, // ADC = HB / 16 (must be less than 24MHz)
            divide4evenAlt2 = 0b10'010u << 11, // ADC = HB / 4 (must be less than 24MHz)
            dvided32oddAlt1 = 0b10'011u << 11, // ADC = HB / 32 (must be less than 24MHz)
            dvided6evenAlt2 = 0b10'100u << 11, // ADC = HB / 6 (must be less than 24MHz)
            divide48odd     = 0b10'101u << 11, // ADC = HB / 48 (must be less than 24MHz)
            divide8evenAlt2 = 0b10'110u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divide64odd     = 0b10'111u << 11, // ADC = HB / 64 (must be less than 24MHz)

            divide2evenAlt3 = 0b11'000u << 11, // ADC = HB / 2 (must be less than 24MHz)
            divide32oddAlt2 = 0b11'001u << 11, // ADC = HB / 32 (must be less than 24MHz)
            divide4evenAlt3 = 0b11'010u << 11, // ADC = HB / 4 (must be less than 24MHz)
            divide64oddAlt1 = 0b11'011u << 11, // ADC = HB / 64 (must be less than 24MHz)
            divide6evenAlt3 = 0b11'100u << 11, // ADC = HB / 6 (must be less than 24MHz)
            divide96odd     = 0b11'101u << 11, // ADC = HB / 96 (must be less than 24MHz)
            divide8evenAlt3 = 0b11'110u << 11, // ADC = HB / 8 (must be less than 24MHz)
            divide128odd    = 0b11'111u << 11, // ADC = HB / 128 (must be less than 24MHz)
        };

        //TODO: v2x/v3x has simpler ACDPRE, but also lower limit for maximal clock 14MHz
        template<std::uint32_t Value>
        constexpr static auto getAcdDividerFromPrescalerFieldValue() -> std::uint8_t {
            // single equation to cover both case:
            // divisor=(2*(1-MOD(lower,2))+(lower+MOD(lower,2))*POWER(2,(higher+1)*MOD(lower,2)))
            // to reverse it, first try the even case:
            // lower=divisor−2 and check if the result is even and in the lower 3bit range
            // if yes the higher can be any value, but preferably 0 and you are done
            // if not then iterate over possible higher 2bit values while calculating:
            // lower=(divisor/(2(upper+1)))-1
            // if that is in lowers range and odd, then confirm our interated higher value matches
            // calculated higher = log2(divisor/(lower+1))−1
            constexpr std::uint8_t lower3  = Value & 0b00'111u;
            constexpr std::uint8_t higher2 = Value & 0b11'000u;
            if (lower3 % 2) {
                // Lower 3bits are even(0,2,4,6), ignore the higher 2bits and just produce 2+lower3 value (2,4,6,8)
                return lower3 + 2;
            } else {
                // Lower 3bits are odd(1,3,5,7), use higher 2bits to get powers (2,4,8,16) multipled with lower3+1 (2,4,6,8)
                return (1u<<(higher2+1))*(lower3+1);
            }
        }

        // TODO on other SoCs this is bigger register, might be worth making it bigger here
        enum class PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator: std::uint32_t {
            // see getAcdPrescaler() for equation
            fieldBitMask = 0b1u << 16,   // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            hsi2x        = 0,            // PLL = 2 * internal high speed clock, default, configure before enabling PLL (PLLCLK = 2*PLLSCR)
            hse2x        = fieldBitMask, // PLL = 2 * external high speed clock, configure before enabling PLL (PLLCLK = 2*PLLSCR)
        };

        //TODO: this register might change between SoCs
        enum class MCO_RW_MicrocontrolerClockPinOutput: std::uint32_t {
            fieldBitMask = 0b111u << 24,  // not holding any settings or value, it's a bitmask for this specific field
            fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

            noOutput     = 0b0'00u << 24, // default, no clock is output
            noOutputAlt1 = 0b0'01u << 24, // no clock is output
            noOutputAlt2 = 0b0'10u << 24, // no clock is output
            noOutputAlt3 = 0b0'11u << 24, // no clock is output

            sysclk       = 0b1'00u << 24, // sysclk is exposed on the pin output
            hsi          = 0b1'01u << 24, // hsi is exposed on the pin output
            hse          = 0b1'10u << 24, // hse is exposed on the pin output
            pll          = 0b1'11u << 24, // pll is exposed on the pin output
        };

        constexpr static std::tuple<
            SW_RW_SystemClockSource,
            SWS_RO_SystemClockSourceStatus,
            HPRE_RW_HbClockPrescaler,
            ADCPRE_RW_AnalogDigitalConverterClockPrescaler,
            PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator,
            MCO_RW_MicrocontrolerClockPinOutput
        > regFields = {};

    };

}
