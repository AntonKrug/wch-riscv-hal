//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <cstdint>

namespace soc::gpio {

    struct ActionEntity { // NOLINT
        std::uint32_t address;     // address to apply the value to
        std::uint32_t value;       // value to be applied
        std::uint32_t mask;        // bitmask of what part of the value is the desired value
        std::uint32_t writable;    // bitmask of the whole register to know what is writable at that address
        std::uint32_t port_number; // to tell apart portA from B without need to check address

        // array of pins? not needed such granularity?
    };

}
