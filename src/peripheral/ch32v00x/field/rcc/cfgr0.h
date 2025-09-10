//
// Created by anton on 05/02/2025.
//

#pragma once

#include <cstdint>
#include <tuple>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

// TODO: different SoC might have more registers

namespace peripheral::rcc {

    struct Cfgr0 { // NOLINT
        // Clock configuration

        constexpr static std::uint32_t reg_mem_offset = 0x04U;

        enum class SW_RW_SystemClockSource: std::uint32_t { // NOLINT
            field_bit_mask = 0b11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            hsi            = 0b00U, // default. internal high speed clock, is selected by HW when returning from standby and stop modes, or when HSE fails when CSSON is enabled
            hse            = 0b01U, // external high speed clock, HW might automatically switch to HSI in some scenarios, read HSI comment
            pll            = 0b10U, // phase locked loop generator

            reserved       = 0b11U, // N/A do not use this value
        };

        enum class SWS_RO_SystemClockSourceStatus: std::uint32_t { // NOLINT
            field_bit_mask = 0b11U << 2U, // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadOnly,

            using_hsi      = 0b00U << 2U, // internal high speed clock used
            using_hse      = 0b01U << 2U, // external high speed clock used
            using_pll      = 0b10U << 2U, // phase locked loop generator used

            reserved       = 0b11U << 2U, // N/A do not use this value
        };

        //TODO: this register might change between SoCs
        enum class HPRE_RW_HbClockPrescaler: std::uint32_t { // To drive HCLK // NOLINT
            field_bit_mask   = 0b1'111U << 4U, // not holding any settings or value, it's a bitmask for this specific field
            field_access     = soc::reg::field_access_right::ReadWrite,

            no_prescaler     = 0U,             // HCLK = SYSCLK (prescaler is off)

            divide2_linearly = 0b0'001U << 4U, // HCLK = SYSCLK / 2 (linear division of 2, prefetch buffer must be on)
            divide3_linearly = 0b0'010U << 4U, // HCLK = SYSCLK / 3 (default linear division of 3, prefetch buffer must be on)
            divide4_linearly = 0b0'011U << 4U, // HCLK = SYSCLK / 4 (linear division of 4, prefetch buffer must be on)
            divide5_linearly = 0b0'100U << 4U, // HCLK = SYSCLK / 5 (linear division of 5, prefetch buffer must be on)
            divide6_linearly = 0b0'101U << 4U, // HCLK = SYSCLK / 6 (linear division of 6, prefetch buffer must be on)
            divide7_linearly = 0b0'110U << 4U, // HCLK = SYSCLK / 7 (linear division of 7, prefetch buffer must be on)
            divide8_linearly = 0b0'111U << 4U, // HCLK = SYSCLK / 8 (linear division of 8, prefetch buffer must be on)

            divide2_powers   = 0b1'000U << 4U, // HCLK = SYSCLK / 2 (power by 2 division of 2, prefetch buffer must be on)
            divide4_powers   = 0b1'001U << 4U, // HCLK = SYSCLK / 4 (power by 2 division of 4, prefetch buffer must be on)
            divide8_powers   = 0b1'010U << 4U, // HCLK = SYSCLK / 8 (power by 2 division of 8, prefetch buffer must be on)
            divide16_powers  = 0b1'011U << 4U, // HCLK = SYSCLK / 16 (power by 2 division of 16, prefetch buffer must be on)
            divide32_powers  = 0b1'100U << 4U, // HCLK = SYSCLK / 32 (power by 2 division of 32, prefetch buffer must be on)
            divide64_powers  = 0b1'101U << 4U, // HCLK = SYSCLK / 64 (power by 2 division of 64, prefetch buffer must be on)
            divide128_powers = 0b1'110U << 4U, // HCLK = SYSCLK / 128 (power by 2 division of 128, prefetch buffer must be on)
            divide256_powers = 0b1'111U << 4U, // HCLK = SYSCLK / 256 (power by 2 division of 256, prefetch buffer must be on)
        };

        constexpr static std::uint8_t log2_int(std::uint32_t x) {
            std::uint8_t result = 0U;
            while (x > 1U) {
                x >>= 1U;
                ++result;
            }
            return result;
        }

        template<std::uint32_t TplSysClock, std::uint32_t TplDesiredHbClock>
        constexpr static auto get_hb_prescaler() -> std::uint8_t {
            static_assert(
                TplDesiredHbClock <= TplSysClock,
                "TplDesiredHbClock must be smaller or the same as TplSysClock");

            constexpr double ratio_fraction = static_cast<double>(TplSysClock) / static_cast<double>(TplDesiredHbClock);
            constexpr auto ratio_whole = static_cast<std::uint32_t>(ratio_fraction);

            static_assert(
                ratio_fraction == static_cast<double>(ratio_whole),
                "The TplSysClock and TplDesiredHbClock frequencies can't be divided by a whole number division");

            if constexpr (ratio_whole == 1U) {
                return 0U;
            } else if constexpr (ratio_whole < 9U) {
                return static_cast<std::uint8_t>(ratio_whole - 1U);
            } else {
                static_assert(ratio_whole <= 256U, "The TplDesiredHbClock is too small for such TplSysClock");

                // poor's man log2
                constexpr std::uint8_t count = log2_int(ratio_whole);
                static_assert( (static_cast<std::uint32_t>(1U) << static_cast<std::uint32_t>(count)) == ratio_whole, "The TplSysClock/TplDesiredHbClock needs to be either smaller ratio, or a power of 2");
                return static_cast<std::uint8_t>(count + 7U);
            }
        }

        template<std::uint32_t SysClock, std::uint32_t DesiredHbClock>
        constexpr static auto get_hb_prescaler_with_offset() -> std::uint32_t {
            return soc::reg::rawValueOffsetToEnumsOffsetCt<
                get_hb_prescaler<SysClock, DesiredHbClock>(),
                HPRE_RW_HbClockPrescaler>();
        }

        template<std::uint32_t SysClock, std::uint32_t DesiredHbClock>
        constexpr static auto get_hb_prescaler_enum() -> HPRE_RW_HbClockPrescaler {
            return static_cast<HPRE_RW_HbClockPrescaler>(get_hb_prescaler_with_offset<SysClock, DesiredHbClock>());
        }

        enum class ADCPRE_RW_AnalogDigitalConverterClockPrescaler: std::uint32_t { // NOLINT
            // see getAcdDividerFromPrescalerFieldValue() for equation, lower 3-bits are threaten differently than higher 2-bits
            field_bit_mask    = static_cast<std::uint32_t>(0b11'111U) << 11U, // not holding any settings or value, it's a bitmask for this specific field
            field_access      = soc::reg::field_access_right::ReadWrite,

            divide2_even      = static_cast<std::uint32_t>(0b00'000U) << 11U, // default ADC = HB / 2 (must be less than 24MHz)
            divide4_odd       = static_cast<std::uint32_t>(0b00'001U) << 11U, // ADC = HB / 4 (must be less than 24MHz)
            divide4_even      = static_cast<std::uint32_t>(0b00'010U) << 11U, // ADC = HB / 4 (must be less than 24MHz)
            divide8_odd       = static_cast<std::uint32_t>(0b00'011U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide6_even      = static_cast<std::uint32_t>(0b00'100U) << 11U, // ADC = HB / 6 (must be less than 24MHz)
            divide12_odd      = static_cast<std::uint32_t>(0b00'101U) << 11U, // ADC = HB / 12 (must be less than 24MHz)
            divide8_even      = static_cast<std::uint32_t>(0b00'110U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide16_odd      = static_cast<std::uint32_t>(0b00'111U) << 11U, // ADC = HB / 16 (must be less than 24MHz)

            divide2_even_alt1 = static_cast<std::uint32_t>(0b01'000U) << 11U, // ADC = HB / 2 (must be less than 24MHz)
            divide8_odd_alt1  = static_cast<std::uint32_t>(0b01'001U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide4_even_alt1 = static_cast<std::uint32_t>(0b01'010U) << 11U, // ADC = HB / 4 (must be less than 24MHz)
            divide16_odd_alt1 = static_cast<std::uint32_t>(0b01'011U) << 11U, // ADC = HB / 16 (must be less than 24MHz)
            divide5_even_alt1 = static_cast<std::uint32_t>(0b01'100U) << 11U, // ADC = HB / 6 (must be less than 24MHz)
            divide24_odd      = static_cast<std::uint32_t>(0b01'101U) << 11U, // ADC = HB / 24 (must be less than 24MHz)
            divide8_even_alt1 = static_cast<std::uint32_t>(0b01'110U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide32_odd      = static_cast<std::uint32_t>(0b01'111U) << 11U, // ADC = HB / 32 (must be less than 24MHz)

            divide2_even_alt2 = static_cast<std::uint32_t>(0b10'000U) << 11U, // ADC = HB / 2 (must be less than 24MHz)
            divide16_odd_alt2 = static_cast<std::uint32_t>(0b10'001U) << 11U, // ADC = HB / 16 (must be less than 24MHz)
            divide4_even_alt2 = static_cast<std::uint32_t>(0b10'010U) << 11U, // ADC = HB / 4 (must be less than 24MHz)
            divide32_odd_alt1 = static_cast<std::uint32_t>(0b10'011U) << 11U, // ADC = HB / 32 (must be less than 24MHz)
            divide6_even_alt2 = static_cast<std::uint32_t>(0b10'100U) << 11U, // ADC = HB / 6 (must be less than 24MHz)
            divide48_odd      = static_cast<std::uint32_t>(0b10'101U) << 11U, // ADC = HB / 48 (must be less than 24MHz)
            divide8_even_alt2 = static_cast<std::uint32_t>(0b10'110U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide64_odd      = static_cast<std::uint32_t>(0b10'111U) << 11U, // ADC = HB / 64 (must be less than 24MHz)

            divide2_even_alt3 = static_cast<std::uint32_t>(0b11'000U) << 11U, // ADC = HB / 2 (must be less than 24MHz)
            divide32_odd_alt2 = static_cast<std::uint32_t>(0b11'001U) << 11U, // ADC = HB / 32 (must be less than 24MHz)
            divide4_even_alt3 = static_cast<std::uint32_t>(0b11'010U) << 11U, // ADC = HB / 4 (must be less than 24MHz)
            divide64_odd_alt1 = static_cast<std::uint32_t>(0b11'011U) << 11U, // ADC = HB / 64 (must be less than 24MHz)
            divide6_even_alt3 = static_cast<std::uint32_t>(0b11'100U) << 11U, // ADC = HB / 6 (must be less than 24MHz)
            divide96_odd      = static_cast<std::uint32_t>(0b11'101U) << 11U, // ADC = HB / 96 (must be less than 24MHz)
            divide8_even_alt3 = static_cast<std::uint32_t>(0b11'110U) << 11U, // ADC = HB / 8 (must be less than 24MHz)
            divide128_odd     = static_cast<std::uint32_t>(0b11'111U) << 11U, // ADC = HB / 128 (must be less than 24MHz)
        };

        //TODO: v2x/v3x has simpler ACDPRE, but also lower limit for maximal clock 14MHz
        template<std::uint32_t TplValue>
        constexpr static auto getAcdDividerFromPrescalerFieldValue() -> std::uint8_t {
            // single equation to cover both case:
            // divisor=(2*(1-MOD(lower,2))+(lower+MOD(lower,2))*POWER(2,(higher+1)*MOD(lower,2)))
            // to reverse it, first try the even case:
            // lower=divisor−2 and check if the result is even and in the lower 3bit range
            // if yes the higher can be any value, but preferably 0, and you are done
            // if not then iterate over possible higher 2bit values while calculating:
            // lower=(divisor/(2(upper+1)))-1
            // if that is in lowers range and odd, then confirm our higher value matches
            // calculated higher = log2(divisor/(lower+1))−1
            constexpr std::uint32_t lower3  = TplValue & 0b00'111U;
            constexpr std::uint32_t higher2 = TplValue & 0b11'000U;
            if ((lower3 % 2U) != 0U) {
                // Lower 3bits are even(0,2,4,6), ignore the higher 2bits and just produce 2+lower3 value (2,4,6,8)
                return static_cast<std::uint8_t>(lower3 + 2U);
            } else {
                // Lower 3bits are odd(1,3,5,7), use higher 2bits to get powers (2,4,8,16) multiplied with lower3+1 (2,4,6,8)
                return static_cast<std::uint8_t>((1U<<(higher2+1U)) * (lower3+1U));
            }
        }

        // TODO on other SoCs this is bigger register, might be worth making it bigger here
        enum class PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator: std::uint32_t { // NOLINT
            // see getAcdPrescaler() for equation
            field_bit_mask = static_cast<std::uint32_t>(0b1U) << 16U,  // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            hsi2_x         = 0U,             // PLL = 2 * internal high speed clock, default, configure before enabling PLL (PLLCLK = 2*PLLSCR)
            hse2_x         = field_bit_mask, // PLL = 2 * external high speed clock, configure before enabling PLL (PLLCLK = 2*PLLSCR)
        };

        //TODO: this register might change between SoCs
        enum class MCO_RW_MicrocontrollerClockPinOutput: std::uint32_t { // NOLINT
            field_bit_mask = static_cast<std::uint32_t>(0b111U) << 24U,  // not holding any settings or value, it's a bitmask for this specific field
            field_access   = soc::reg::field_access_right::ReadWrite,

            no_output      = static_cast<std::uint32_t>(0b0'00U) << 24U, // default, no clock is output
            no_output_alt1 = static_cast<std::uint32_t>(0b0'01U) << 24U, // no clock is output
            no_output_alt2 = static_cast<std::uint32_t>(0b0'10U) << 24U, // no clock is output
            no_output_alt3 = static_cast<std::uint32_t>(0b0'11U) << 24U, // no clock is output

            sysclk         = static_cast<std::uint32_t>(0b1'00U) << 24U, // sysclk is exposed on the pin output
            hsi            = static_cast<std::uint32_t>(0b1'01U) << 24U, // hsi is exposed on the pin output
            hse            = static_cast<std::uint32_t>(0b1'10U) << 24U, // hse is exposed on the pin output
            pll            = static_cast<std::uint32_t>(0b1'11U) << 24U, // pll is exposed on the pin output
        };

        constexpr static std::tuple<
            SW_RW_SystemClockSource,
            SWS_RO_SystemClockSourceStatus,
            HPRE_RW_HbClockPrescaler,
            ADCPRE_RW_AnalogDigitalConverterClockPrescaler,
            PLLSRC_RW_InputClockSourceForPhaseLockedLoopGenerator,
            MCO_RW_MicrocontrollerClockPinOutput
        > reg_fields = {};

    };

}
