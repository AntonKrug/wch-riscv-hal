//
// Created by Fredy on 4/19/2025.
//

#pragma once

#include <cstdint>

// struct Buffer {
//     int value[10];
// };
//
//
// inline constexpr Buffer globalBuffer{42};
// inline constexpr const void* globalBufferPtr = &globalBuffer;
// inline constexpr std::uintptr_t globalBufferAddr = (std::uintptr_t)(globalBufferPtr);
//

constinit int myVar = 10;


constexpr int* getPointer() {
    return &myVar;  // Return a constexpr pointer to myVar
}

constexpr uintptr_t getPointerAsUintPtr() {
    return reinterpret_cast<uintptr_t>(&myVar);  // Convert pointer to uintptr_t
}

// constexpr std::uintptr_t ptr = (uintptr_t)(static_cast<void *>(getPointer()));

constexpr std::uintptr_t ptr2 = getPointerAsUintPtr();