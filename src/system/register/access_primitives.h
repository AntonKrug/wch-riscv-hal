//
// Created by anton on 16/02/2025.
//

#pragma once

#include <cstdint>

namespace Soc::Reg::Access {

    #pragma region Access primitive[s] when both Address & Value are compile-time known

    template<std::uint32_t TplAddress, std::uint32_t TplCtValue>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCtAddrVal() -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(TplAddress))=TplCtValue;
    }

    #pragma endregion

    #pragma region Access primitives when Address is only compile-time known

    template<std::uint32_t TplAddress>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    readCtAddr() -> std::uint32_t {
        return *reinterpret_cast<volatile std::uint32_t *>(TplAddress);
    }

    template<std::uint32_t TplAddress>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCtAddr(const std::uint32_t value) -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(TplAddress))=value;
    }

    #pragma endregion

    #pragma region Access primitives when nothing compile-time known

    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    read(const std::uint32_t address) -> std::uint32_t {
        return *reinterpret_cast<volatile std::uint32_t *>(address);
    }

    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    write(const std::uint32_t address, const std::uint32_t value) -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(address))=value;
    }

    #pragma endregion

}

