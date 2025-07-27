//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


// name ideas config chain, crafter compositor, batch, fusion, fuser, flow, optimizer, pipeline, process, reducer, meld, forge,

namespace soc::gpio {

    template<Op... TplOps>
    struct OpFusion { // NOLINT
    private:

        static constexpr std::size_t ops_count = sizeof...(TplOps);

    public:
        static constexpr std::array<Op, ops_count> ops = {TplOps...};

        // add/commit/enroll alternative name
        template<Op TplOp>
        [[nodiscard]] static constexpr auto enroll() {
            return OpFusion<TplOps..., TplOp>{};
        }
    };

}