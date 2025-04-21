//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

namespace Soc::Reg::Access {

    template<std::uint32_t TplAddr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    readCt() -> std::uint32_t {
        return *reinterpret_cast<volatile std::uint32_t *>(TplAddr);
    }

    template<std::uint32_t TplAddr, std::uint32_t TplCtValue>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt() -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(TplAddr))=TplCtValue;
    }

    template<std::uint32_t TplAddr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt(const std::uint32_t rtValue) -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(TplAddr))=rtValue;
    }

}

