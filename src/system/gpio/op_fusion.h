//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


namespace soc::gpio {

    template<Op... TplOps>
    struct OpFusion { // NOLINT

        // add/commit/enroll alternative name
        template<Op TplOp>
        [[nodiscard]] static constexpr auto enroll() {
            return OpFusion<TplOps..., TplOp>{};
        }
    };

}