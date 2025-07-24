//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <cstdint>

#include "system/register/access_ct.h"

namespace soc::gpio {

    // alternative names Spec, Directive, Op

    struct ConfigEntity { // NOLINT
        std::uint32_t address;     // address to apply the value to
        std::uint32_t value;       // value to be applied
        std::uint32_t mask;        // bitmask of what part of the value is the desired value
        std::uint32_t writable;    // bitmask of the whole register to know what is writable at that address
        std::uint32_t port_number; // to tell apart portA from B without need to check address
        // array of pins? not needed such granularity?
    };


    template<ConfigEntity TplConfigEntity>
    constexpr void apply_config_entity_to_register() {
        if constexpr (TplConfigEntity.writable == TplConfigEntity.mask) {
            // call what can be written is already enrolled, we do not need to read the register
            // before and can blindly and more efficiently write to it

            soc::reg::access::writeCtAddrVal<TplConfigEntity.address, TplConfigEntity.value>();
        } else {
            // we need to read original register first, apply clean out the mask content and
            // then apply this new value to it

            const auto old_value = soc::reg::access::readCtAddr<TplConfigEntity.address>();
            constexpr std::uint32_t mask_inverted = 0xffff'ffffU ^ TplConfigEntity.mask;
            soc::reg::access::writeCtAddr<TplConfigEntity.address>(TplConfigEntity.value | (old_value & mask_inverted));
        }
    }

}

