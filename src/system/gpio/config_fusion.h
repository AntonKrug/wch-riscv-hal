//
// Created by Fredy on 7/24/2025.
//

#pragma once

#include <array>

#include "concepts.h"


// name ideas config chain, crafter compositor, batch, fusion, fuser, flow, optimizer, pipeline, process, reducer, meld, forge,

namespace soc::gpio {

    template<ConfigEntity... TplConfigEntities>
    struct ConfigFusion { // NOLINT
    private:

        static constexpr std::size_t actions_count = sizeof...(TplConfigEntities);

    public:
        static constexpr std::array<int, actions_count> entities = {TplConfigEntities...};

        template<ConfigEntity TplConfigEntity>
        [[nodiscard]] static constexpr auto enroll() {
            return ConfigFusion<TplConfigEntities..., TplConfigEntity>{};
        }
    };

}