//
// Created by anton on 06/01/2025.
//

#pragma once

#include "helper_templates.h"

namespace Literals::Timer {


    #pragma region frequencyToHz


    consteval auto operator""_mhz_to_hz(const unsigned long long megaherz) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(megaherz);
    }


    consteval auto operator""_mhz_to_hz(const long double megaherz) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(megaherz);
    }


    consteval auto operator""_khz_to_hz(const unsigned long long kiloherz) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(kiloherz);
    }


    consteval auto operator""_khz_to_hz(const long double kiloherz) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(kiloherz);
    }


    #pragma endregion


    #pragma region periodToHz


    consteval auto operator""_ms_to_hz(const unsigned long long milisecond) -> std::uint32_t {
        return Literals::HelperTemplates::invert<1'000>(milisecond);
    }


    consteval auto operator""_us_to_hz(const unsigned long long nanosecond) -> std::uint32_t {
        return Literals::HelperTemplates::invert<1'000'000>(nanosecond);
    }


    consteval auto operator""_ns_to_hz(const unsigned long long microsecond) -> std::uint32_t {
        return Literals::HelperTemplates::invert<1'000'000'000>(microsecond);
    }


    #pragma endregion


}
