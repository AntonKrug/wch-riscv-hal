//
// Created by Fredy on 4/19/2025.
//

#pragma once

#include <cstdint>

struct Buffer {
    int value[10];
};


inline constexpr Buffer globalBuffer{42};
inline constexpr const void* globalBufferPtr = &globalBuffer;
// inline constexpr std::uintptr_t globalBufferAddr = (std::uintptr_t)(globalBufferPtr);
