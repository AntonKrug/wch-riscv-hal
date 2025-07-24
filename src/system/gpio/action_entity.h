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

        constexpr ActionEntity(std::uint32_t address, std::uint32_t value, std::uint32_t mask, std::uint32_t writable, std::uint32_t port_number)
        : address(address), value(value), mask(mask), writable(writable), port_number(port_number) {};

        // array of pins? not needed such granularity?

        consteval void a() const {
            if constexpr (writable == mask) {
                // call what can be written is already enrolled, we do not need to read the register
                // before and can blindly and more efficiently write to it

                soc::reg::access::writeCtAddrVal<address, value>();
            } else {
                // we need to read original register first, apply clean out the mask content and
                // then apply this new value to it

                const auto old_value = soc::reg::access::readCtAddr<address>();
                constexpr std::uint32_t mask_inverted = 0xffff'ffffU ^ mask;
                soc::reg::access::writeCtAddr<address>(value | (old_value & mask_inverted));
            }
        }
    };


    template<ActionEntity TplActionEntity>
    constexpr void apply_action() {

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

    // constexpr ActionEntity(std::uint32_t address, std::uint32_t value, std::uint32_t mask, std::uint32_t writable, std::uint32_t port_number)
    // : address(address), value(value), mask(mask), writable(writable), port_number(port_number) {};
    //
    // constexpr void apply_action() const {
    //     if constexpr (writable == mask) {
    //         // call what can be written is already enrolled, we do not need to read the register
    //         // before and can blindly write to it
    //     } else {
    //         // we need to read original register first, apply clean out the mask content and
    //         // then apply this new value to it
    //
    //         const auto old_value = soc::reg::access::readCtAddr<address>();
    //         // soc::reg::access::writeCtAddr<register_address_configuration>(mode_raw_value);
    //         // mode_generic_raw((old_value & mask_configuration_inverted) | register_configuration_shift<slew | drive | multiplexing>());
    //     }
    // }

    // consteval void a() const {
    //     if consteval {
    //         // do work
    //         if constexpr (writable == mask) {
    //             // call what can be written is already enrolled, we do not need to read the register
    //             // before and can blindly and more efficiently write to it
    //
    //             soc::reg::access::writeCtAddrVal<address, value>();
    //         } else {
    //             // we need to read original register first, apply clean out the mask content and
    //             // then apply this new value to it
    //
    //             const auto old_value = soc::reg::access::readCtAddr<address>();
    //             constexpr std::uint32_t mask_inverted = 0xffff'ffffU ^ TplActionEntity.mask;
    //             soc::reg::access::writeCtAddr<TplActionEntity.address>(TplActionEntity.value | (old_value & mask_inverted));
    //         }
    //     } else {
    //         // some
    //     }
    // }


}

