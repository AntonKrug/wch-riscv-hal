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
        static constexpr std::array<int, actions_count> entities = {TplActions...};

        template<ActionEntity TplAction, std::size_t... Index>
        [[nodiscard]] static constexpr auto enroll_helper(std::index_sequence<Index...>) {
            return Action<entities[Index], TplAction>{};
        }

    public:
        template<ActionEntity TplAction>
        [[nodiscard]] static constexpr auto enroll() {
            return enroll_helper<TplAction>(std::make_index_sequence<actions_count>{});
        }
    };

}