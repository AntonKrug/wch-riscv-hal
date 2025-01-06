//
// Created by anton on 06/01/2025.
//

#pragma once

#include <limits>

namespace Literals::HelperTemplates {
    template<
        typename FROM,
        typename TO,
        TO multiplier>
    consteval auto multiplyFromAnyToAny(const FROM time) -> TO {
        // Compile time check before multiplying if the T would overflow when multiplying
        assert(time <= (std::numeric_limits<TO>::max() / multiplier));
        return static_cast<TO>((time * static_cast<FROM>(multiplier)));
    }

    template<
        std::uint32_t multiplier,
        typename FROM>
    consteval auto multiply(const FROM time) -> std::uint32_t {
        return multiplyFromAnyToAny<FROM, std::uint32_t, multiplier>(time);
    }
}
