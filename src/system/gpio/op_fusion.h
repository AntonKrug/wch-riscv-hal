//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


// name ideas config chain, crafter compositor, batch, fusion, fuser, flow, optimizer, pipeline, process, reducer, meld, forge,

namespace soc::gpio {

    template<auto... TplOps>
    requires ExecutableOpAll<TplOps...>
    struct OpFusion { // NOLINT

        // add/commit/enroll alternative name
        template<auto TplOp>
        requires ExecutableOp<TplOp>
        [[nodiscard]] static constexpr auto enroll() {
            return OpFusion<TplOps..., TplOp>{};
        }
    };

}