//
// Created by anton on 08/01/2025.
//

#pragma once

#include "helper_templates.h"

namespace Literals::Usart {

    #pragma region Whole numbers

    consteval auto operator""_mbaud(const unsigned long long megabaud) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(megabaud);
    }

    consteval auto operator""_kbaud(const unsigned long long kilobaud) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(kilobaud);
    }

    consteval auto operator""_baud(const unsigned long long baud) -> std::uint32_t {
        return baud;
    }

    #pragma endregion

    #pragma region Fractions

    consteval auto operator""_mbaud(const long double megabaud) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000'000>(megabaud);
    }

    consteval auto operator""_kbaud(const long double kilobaud) -> std::uint32_t {
        return Literals::HelperTemplates::multiply<1'000>(kilobaud);
    }

    #pragma endregion


}