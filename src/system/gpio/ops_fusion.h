//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


namespace soc::gpio {

    template<Op... TplOps>
    struct OpsFusion { // NOLINT

    private:
        static constexpr std::size_t              op_count = sizeof...(TplOps); // NOLINT(*-dynamic-static-initializers)
        static constexpr std::array<Op, op_count> data     = { TplOps... };     // NOLINT(*-dynamic-static-initializers)

        template<std::uint32_t TplAddress>
        static constexpr int find_op_index_with_address() {
            int ret = -1; // -1 will be returned if nothing is found

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
            return OpsFusion<(TplIndexes == TplIndex ? TplNewOp : data[TplIndexes])...>{};
        }

    public:

        template<Op TplOp>
        [[nodiscard]] static constexpr auto enroll() { // NOLINT
            static_assert(
                (TplOp.value & ~TplOp.mask) == 0U,
                "Op value is attempting to write outside its own mask"); // might consider removing this check for more lazier setting/clearing of register

            static_assert(
                (~TplOp.writable & TplOp.mask) == 0U,
                "Op mask is proposing a write outside the writable region of the register"); // if removed it could allow lazier usage

            if constexpr (constexpr int index = find_op_index_with_address<TplOp.address>(); index >= 0) {

                // Found existing Op on the same address, combine them into one and replace the old Op
                constexpr auto old_op = data[index];

                // Confirm that we will not change state of values we expect to not change
                static_assert(
                    old_op.bit_set_reset_address == TplOp.bit_set_reset_address,
                    "Old and new OP doesn't mach in fields where they should (bit_set_reset_address)");

                static_assert(
                    old_op.writable == TplOp.writable,
                    "Old and new OP doesn't mach in fields where they should (writable)");

                static_assert(
                    old_op.port_number == TplOp.port_number,
                    "Old and new OP doesn't mach in fields where they should (port_number)");

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
                return OpsFusion<TplOps..., TplOp>{};
            }
        }

        template <typename TplOpsTuple, std::size_t... Is>
        [[nodiscard]] static constexpr auto enroll_ops_tuple_impl(TplOpsTuple&& tuple, std::index_sequence<Is...>) {
            enroll(std::get<Is>(std::forward<TplOpsTuple>(tuple))...);
        }

        template <typename TplOpsTuple>
        [[nodiscard]] static constexpr auto enroll_ops_tuple(TplOpsTuple&& tuple) {
            constexpr auto size = std::tuple_size_v<std::decay_t<TplOpsTuple>>;
            return enroll_ops_tuple_impl(std::forward<TplOpsTuple>(tuple), std::make_index_sequence<size>{});
        }

        // empty fusion merge
        static constexpr auto merge(const OpsFusion<> other_ops_fuson) {
            return OpsFusion<(TplOps, ...)>{};
        }

        // // single Op fusion merge
        // template <Op TplOtherOp>
        // static constexpr auto merge(const OpsFusion<TplOtherOp> other_ops_fuson) {
        //     return enroll<TplOtherOp>();
        // }
        //
        // // multiple Ops fusion to be merged
        // template <Op TplOpsHead, Op... TplOpsTail>
        // static constexpr auto merge(const OpsFusion<TplOpsHead, TplOpsTail...> other_ops_fuson) {
        //     return merge<TplOpsTail...>();
        // }

        // multiple Ops fusion to be merged
        template <Op... TplOtherOps>
        static constexpr auto merge(const OpsFusion<TplOtherOps...> other_ops_fuson) {
            return OpsFusion<TplOps..., TplOtherOps...>{};
        }


        // TODO: replace constexpr to consteval
        constexpr static void execute() {
            (execute_op<TplOps>(), ...);
        }

    };


}