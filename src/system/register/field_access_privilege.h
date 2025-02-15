//
// Created by anton on 15/02/2025.
//

#pragma once

#include <cstdint>

namespace Soc::Reg::FieldAccessPrivilege::Field {

    enum class AccessRightsEnum: std::uint32_t {
        ReadOnly,
        WriteOnly,
        ReadWrite
    };

    namespace AccessRights {
        constexpr std::uint32_t ReadOnly  = static_cast<std::uint32_t>(AccessRightsEnum::ReadOnly);
        constexpr std::uint32_t WriteOnly = static_cast<std::uint32_t>(AccessRightsEnum::WriteOnly);
        constexpr std::uint32_t ReadWrite = static_cast<std::uint32_t>(AccessRightsEnum::ReadWrite);
    }

    template<AccessRightsEnum access>
    constexpr auto isWritable() -> bool {
        if (access == AccessRightsEnum::ReadOnly) {
            return false;
        }
        return true;
    }

    template<std::uint32_t access>
    constexpr auto isWritable() -> bool {
        if (access == static_cast<std::uint32_t>(AccessRightsEnum::ReadOnly)) {
            return false;
        }
        return true;
    }

}
