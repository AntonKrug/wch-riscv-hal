//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

namespace Soc::Reg::Access {

    template<std::uint32_t Addr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    readCt() -> std::uint32_t {
        return *reinterpret_cast<volatile std::uint32_t *>(Addr);
    }

    template<std::uint32_t Addr, std::uint32_t CtValue>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt() -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(Addr))=CtValue;
    }

    template<std::uint32_t Addr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt(const std::uint32_t rtValue) -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(Addr))=rtValue;
    }

}

