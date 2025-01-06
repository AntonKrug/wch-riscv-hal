//
// Created by anton on 06/01/2025.
//

#pragma once

#include "helper_templates.h"

namespace Literals::Timer {

    #pragma region toHz
    consteval auto operator""_mhz_to_hz(const unsigned long long hours) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(hours);
    }

    consteval auto operator""_mhz_to_hz(const long double hours) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(hours);
    }

    consteval auto operator""_khz_to_hz(const unsigned long long hours) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(hours);
    }

    consteval auto operator""_khz_to_hz(const long double hours) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(hours);
    }
    #pragma endregion

}
