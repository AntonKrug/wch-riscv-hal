//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include "action_entity.h"

namespace soc::gpio {

    template <typename  T>
    concept Enrollable = requires(T t) {  // NOLINT
        { t.template enroll<ActionEntity{}>() };
    };

}