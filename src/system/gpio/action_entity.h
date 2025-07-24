//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <cstdint>

#include "system/register/access_ct.h"

namespace soc::gpio {

    struct ActionEntity { // NOLINT
        std::uint32_t address;     // address to apply the value to
        std::uint32_t value;       // value to be applied
        std::uint32_t mask;        // bitmask of what part of the value is the desired value
        std::uint32_t writable;    // bitmask of the whole register to know what is writable at that address
        std::uint32_t port_number; // to tell apart portA from B without need to check address
        // array of pins? not needed such granularity?
    };


    template<ActionEntity TplActionEntity>
    constexpr void apply_action_to_register() {
        if constexpr (TplActionEntity.writable == TplActionEntity.mask) {
            // call what can be written is already enrolled, we do not need to read the register
            // before and can blindly and more efficiently write to it

            soc::reg::access::writeCtAddrVal<TplActionEntity.address, TplActionEntity.value>();
        } else {
            // we need to read original register first, apply clean out the mask content and
            // then apply this new value to it

            const auto old_value = soc::reg::access::readCtAddr<TplActionEntity.address>();
            constexpr std::uint32_t mask_inverted = 0xffff'ffffU ^ TplActionEntity.mask;
            soc::reg::access::writeCtAddr<TplActionEntity.address>(TplActionEntity.value | (old_value & mask_inverted));
        }
    }

}

