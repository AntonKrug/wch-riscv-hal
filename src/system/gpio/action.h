//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>
#include <utility>

#include "action.h"
#include "concepts.h"

namespace soc::gpio {

    template<ActionEntity... TplActions>
    struct Action { // NOLINT
    private:

        static constexpr std::size_t actions_count = sizeof...(TplActions);

    public:
        static constexpr std::array<int, actions_count> entities = {TplActions...};

        template<ActionEntity TplAction>
        [[nodiscard]] static constexpr auto enroll() {
            return Action<TplActions..., TplAction>{};
        }
    };

}