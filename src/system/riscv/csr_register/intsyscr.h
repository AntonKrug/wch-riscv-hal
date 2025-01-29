//
// Created by anton on 28/01/2025.
//

#pragma once

#include <cstdint>
#include <type_traits>

namespace Riscv::Csr::A {
    enum class A: std::uint32_t {
        fieldBitMask = 0b1,
        a = 0,
    };


    enum class B: std::uint32_t {
        fieldBitMask = 0b1'0,
        b = 0,
    };

    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, A> ||
        std::is_same_v<Field, B>;

}

namespace Riscv::Csr::Intsyscr {
    // https://www.reddit.com/r/RISCV/comments/126262j/notes_on_wch_fast_interrupts/
    // https://www.eevblog.com/forum/microcontrollers/ch32v003-fast-interrupt-(pfic)-features/


    enum class Hwstken: std::uint32_t {
        // Hardware protolgue and epilogue, on low-end devices (003) it saves footprint,
        // but doesn't improve performance, on higher-end device it has shadow registers
        // and improves performance as well.
        fieldBitMask = 0b1,
        hpeEnable    = 0b1,
        hpeDisable   = 0b0
    };


    enum class Inesten: std::uint32_t {
        fieldBitMask           = 0b1'0,
        interuptNestingEnable  = 0b1'0,
        interuptNestingDisable = 0b0'0,
    };


    enum class Eabien: std::uint32_t {
        fieldBitMask = 0b1'00,
        eabiEnable   = 0b1'00,
        eabiDisable  = 0b0'00
    };


    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Hwstken> ||
        std::is_same_v<Field, Inesten> ||
        std::is_same_v<Field, Eabien>;


    // Accept any above enums, but only them (not mixing enums from different csrs)
    template<IsAnyField... Args>
    constexpr auto combine(const Args... args) -> std::uint32_t {
        return (static_cast<std::uint32_t>(args) | ...);
    }


    template<auto Field>
    auto setOnlyMask() -> std::uint32_t requires IsAnyField<decltype(Field)> {
        return static_cast<std::uint32_t>(decltype(Field)::bitMask);
    }

}