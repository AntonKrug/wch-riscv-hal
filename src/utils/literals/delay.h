//
// Created by anton on 06/01/2025.
//

#pragma once

#include "helper_templates.h"

namespace Literals::Delay {


     #pragma region ToMicroSeconds


     consteval auto operator""_hour_to_us(const unsigned long long hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<static_cast<unsigned int>(1'000'000) * 60 * 60>(hours);
     }


     consteval auto operator""_min_to_us(const unsigned long long minutes) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000'000 * 60>(minutes);
     }


     consteval auto operator""_s_to_us(const unsigned long long seconds) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000'000>(seconds);
     }


     consteval auto operator""_ms_to_us(const unsigned long long milisecond) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000>(milisecond);
     }


     consteval auto operator""_hour_to_us(const long double hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<static_cast<unsigned int>(1'000'000) * 60 * 60>(hours);
     }


     consteval auto operator""_min_to_us(const long double minutes) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000'000 * 60>(minutes);
     }


     consteval auto operator""_s_to_us(const long double seconds) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000'000>(seconds);
     }


     consteval auto operator""_ms_to_us(const long double milisecond) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000>(milisecond);
     }


     #pragma endregion


     #pragma region ToMiliSeconds


     consteval auto operator""_day_to_ms(const unsigned long long hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60 * 60 * 24>(hours);
     }


     consteval auto operator""_hour_to_ms(const unsigned long long hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60 * 60>(hours);
     }


     consteval auto operator""_min_to_ms(const unsigned long long minutes) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60>(minutes);
     }


     consteval auto operator""_s_to_ms(const unsigned long long seconds) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000>(seconds);
     }


     consteval auto operator""_day_to_ms(const long double hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60 * 60 * 24>(hours);
     }


     consteval auto operator""_hour_to_ms(const long double hours) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60 * 60>(hours);
     }


     consteval auto operator""_min_to_ms(const long double minutes) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000 * 60>(minutes);
     }


     consteval auto operator""_s_to_ms(const long double seconds) -> std::uint32_t {
          return Literals::HelperTemplates::multiply<1'000>(seconds);
     }


     #pragma endregion

}

