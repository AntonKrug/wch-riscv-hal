//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include "op.h"

namespace soc::gpio {

#pragma region Enroll

    // template<typename TplOpFusionInstanceType>
    // concept EnrollableType = requires(TplOpFusionInstanceType op_fusion_chain) {  // NOLINT
    //     { op_fusion_chain.template enroll<Op{}>() };
    // };
    //
    // template<auto TplFusionInstance>
    // concept Enrollable = EnrollableType<decltype(TplFusionInstance)>; // NOLINT
    //
    // template<auto... TplFusionInstances>
    // concept EnrollableAll = (Enrollable<TplFusionInstances> && ...); // NOLINT

#pragma endregion

}