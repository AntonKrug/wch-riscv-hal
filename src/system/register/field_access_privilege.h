//
// Created by anton on 15/02/2025.
//

#pragma once

#include <cstdint>

namespace Soc::Reg::FieldAccessRight {

    enum class AccessRightEnum: std::uint32_t {
        ReadOnly,
        WriteOnly,
        ReadWrite
    };

    constexpr std::uint32_t ReadOnly  = static_cast<std::uint32_t>(AccessRightEnum::ReadOnly);
    constexpr std::uint32_t WriteOnly = static_cast<std::uint32_t>(AccessRightEnum::WriteOnly);
    constexpr std::uint32_t ReadWrite = static_cast<std::uint32_t>(AccessRightEnum::ReadWrite);

    template<AccessRightEnum access>
    constexpr auto isWritable() -> bool {
        if (access == AccessRightEnum::ReadOnly) {
            return false;
        }
        return true;
    }

    template<std::uint32_t access>
    constexpr auto isWritable() -> bool {
        if (access == static_cast<std::uint32_t>(AccessRightEnum::ReadOnly)) {
            return false;
        }
        return true;
    }

}
