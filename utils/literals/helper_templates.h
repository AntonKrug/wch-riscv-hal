//
// Created by anton on 06/01/2025.
//

#pragma once

#include <limits>
#include <cassert>

namespace Literals::HelperTemplates {


    template<
        typename FROM,
        typename TO,
        TO multiplier>
    consteval auto multiplyFromAnyToAny(const FROM value) -> TO {
        // Compile time check before multiplying if the T would overflow when multiplying
        assert(value <= (std::numeric_limits<TO>::max() / multiplier));
        return static_cast<TO>((value * static_cast<FROM>(multiplier)));
    }


    template<
        typename FROM,
        typename TO,
        TO multiplier>
    consteval auto invertFromAnyToAnyWhileMultiply(const FROM value) -> TO {
        // Compile time check before multiplying if the T would overflow when multiplying
        // assert(value <= (std::numeric_limits<TO>::max() / multiplier));
        // 1ms => 0.0001s => 1000hz
        // 1us => 0.0000001s => 10000000Hz
        // TODO: support subfraction of herz
        // TODO: add assertion
        return static_cast<TO>(static_cast<FROM>(multiplier)/value);
    }


    template<
        std::uint32_t multiplier,
        typename FROM>
    consteval auto multiply(const FROM value) -> std::uint32_t {
        return multiplyFromAnyToAny<FROM, std::uint32_t, multiplier>(value);
    }


    template<
        std::uint32_t multiplier,
        typename FROM>
    consteval auto invert(const FROM value) -> std::uint32_t {
        return invertFromAnyToAnyWhileMultiply<FROM, std::uint32_t, multiplier>(value);
    }


}
