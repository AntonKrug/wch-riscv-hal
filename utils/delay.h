//
// Created by anton on 04/01/2025.
//

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>

namespace DelayLiterals {
     namespace HelperTemplates {
          template<typename T, std::uint32_t multiplier>
          consteval auto toAny(const unsigned long long time) -> std::uint32_t {
               // Compile time check before multiplying if the T would overflow when multiplying
               assert(time < (std::numeric_limits<T>::max() / multiplier));
               return time * multiplier;
          }

          template<std::uint32_t multiplier>
          consteval auto toAnyUint32(const unsigned long long time) -> std::uint32_t {
               return toAny<std::uint32_t, multiplier>(time);
          }
     }

     #pragma region ToMicroSeconds
     consteval auto operator""_hour_to_us(const unsigned long long hours) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<static_cast<unsigned int>(1'000'000) * 60 * 60>(hours);
     }

     consteval auto operator""_min_to_us(const unsigned long long minutes) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000'000 * 60>(minutes);
     }

     consteval auto operator""_s_to_us(const unsigned long long seconds) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000'000>(seconds);
     }

     // constexpr auto operator""_ms_to_us(const unsigned long long milisecond) -> std::uint32_t {
     //      return HelperTemplates::toAnyUint32<1'000>(milisecond);
     // }
     #pragma endregion

     #pragma region ToMiliSeconds
     consteval auto operator""_day_to_ms(const unsigned long long hours) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000 * 60 * 60 * 24>(hours);
     }

     consteval auto operator""_hour_to_ms(const unsigned long long hours) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000 * 60 * 60>(hours);
     }

     consteval auto operator""_min_to_ms(const unsigned long long minutes) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000 * 60>(minutes);
     }

     consteval auto operator""_s_to_ms(const unsigned long long seconds) -> std::uint32_t {
          return HelperTemplates::toAnyUint32<1'000>(seconds);
     }

     #pragma endregion

}

namespace Utils {

     inline auto delayUs(const std::uint32_t us) -> void {
          // TODO finish
     }

     inline auto delayMs(const std::uint32_t us) -> void {
          // TODO finish
     }
}
