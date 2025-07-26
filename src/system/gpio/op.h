//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <cstdint>

#include "system/register/access_ct.h"

namespace soc::gpio {

    // alternative names Spec, Directive, Op

    struct Op { // NOLINT
        std::uint32_t address;               // address to apply the value to
        std::uint32_t bit_set_reset_address; // alternative adddress to use to apply set/reset mechanism
        std::uint32_t value;                 // value to be applied
        std::uint32_t mask;                  // bitmask of what part of the value is the desired value
        std::uint32_t writable;              // bitmask of the whole register to know what is writable at that address
        std::uint32_t port_number;           // to tell apart portA from B without need to check address
    };

    // GPIOx_OUTDR register layout converted to GPIOx_BSHR register layout
    template<std::uint32_t TplWriteValue>
    constexpr std::uint32_t convert_write_value_to_bit_set_clear() {
        std::uint32_t set_clear_result = 0U;
        std::uint32_t write_value = TplWriteValue;

        while (write_value > 0U) {
            set_clear_result = set_clear_result << 1U;
            // if it's 1 to be written it can stay in the same bit position,
            // if it's 0 to be written we need to shift it into 16th bit position
            set_clear_result |= ( (write_value % 1U) == 1U) ? 1U : static_cast<std::uint32_t>(1U) << 16U;
            write_value = write_value >> 1U;
        }

        return set_clear_result;
    }

    template<Op TplOp>
    constexpr void execute_op() {
        if constexpr (TplOp.bit_set_reset_address != 0U) {
            // There is set/reset alternative address for this operation. We do not need to read,
            // mask the value and write on this exact op when we can convert it slightly an op
            // which doesn't destroy all original content in the peripheral registers as it can
            // operate in the bit set/reset mode as well.
            constexpr std::uint32_t bit_set_clear_value = convert_write_value_to_bit_set_clear<TplOp.value>();

            soc::reg::access::writeCtAddrVal<TplOp.bit_set_reset_address, bit_set_clear_value>();
        } else if constexpr (TplOp.writable == TplOp.mask) {
            // call what can be written is already enrolled, we do not need to read the register
            // before and can blindly and more efficiently write to it

            soc::reg::access::writeCtAddrVal<TplOp.address, TplOp.value>();
        } else {
            // we need to read original register first, apply clean out the mask content and
            // then apply this new value to it

            const auto old_value = soc::reg::access::readCtAddr<TplOp.address>();
            constexpr std::uint32_t mask_inverted = 0xffff'ffffU ^ TplOp.mask;
            soc::reg::access::writeCtAddr<TplOp.address>(TplOp.value | (old_value & mask_inverted));
        }
    }

}

