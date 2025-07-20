//
// Created by Fredy on 7/20/2025.
//

#pragma once

#include <cstdint>

namespace soc::irq {
    constexpr std::uint32_t handlers_vector_table_size = 38U;

    constexpr std::uint32_t memory_after_vector_table =
        handlers_vector_table_size * 4U // each handler has 4bytes pointer
        + 4U; // first unimplemented handler which we used to jump to system init at the CPU power up

}

