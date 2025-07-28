//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


namespace soc::gpio {

    template<Op... TplOps>
    struct OpFusion { // NOLINT
    private:
        static constexpr std::size_t              op_count = sizeof...(TplOps); // NOLINT(*-dynamic-static-initializers)
        static constexpr std::array<Op, op_count> data     = { TplOps... };     // NOLINT(*-dynamic-static-initializers)

        template<std::uint32_t TplAddress>
        static constexpr int find_op_index_with_address() {
            int ret = -1;

            for (std::size_t i=0U; i<op_count; i++) {
                if (data[i].address == TplAddress) {
                    ret = static_cast<int>(i);
                    break;
                }
            }

            return ret;
        }

        template <std::size_t TplIndex, Op TplNewOp, std::size_t... TplIndexes>
        static constexpr auto replace_at(std::index_sequence<TplIndexes...>) {
            return OpFusion<(TplIndexes == TplIndex ? TplNewOp : data[TplIndexes])...>{};
        }

    public:

        template<Op TplOp>
        [[nodiscard]] static constexpr auto enroll() { // NOLINT
            if constexpr (constexpr int index = find_op_index_with_address<TplOp.address>(); index >= 0) {

                // Found existing Op on the same address, combine and replace_at it
                constexpr auto old_op = data[index];
                constexpr Op new_op{
                    .address = TplOp.address,
                    .bit_set_reset_address = TplOp.bit_set_reset_address,
                    .value = TplOp.value | old_op.value,
                    .mask = TplOp.mask | old_op.mask,
                    .writable = TplOp.writable,
                    .port_number = TplOp.port_number,
                };

                return replace_at<static_cast<std::size_t>(index), new_op>(std::make_index_sequence<op_count>{});
            } else {

                // No existing entry found, lets just add it as a new entry
                return OpFusion<TplOps..., TplOp>{};
            }
        }
    };

}