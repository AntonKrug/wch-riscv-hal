//
// Created by anton on 06/02/2025.
//

#pragma once

#include <cstdint>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Peripheral::Rcc::Intr {
    // Clock Interupt

    using namespace Soc::Reg::FieldAccessPrivilege; // so we can use Field::AccessRights::ReadOnly...

    enum class LSIRDYF_RO_InternalLowSpeedReadyFlag: std::uint32_t {
        fieldBitMask  = 1u, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess   = Field::AccessRights::ReadOnly,

        noInterupt    = 0, // default
        readyInterupt = fieldBitMask, // set by hw, to clear write LSIRDYC
    };

    enum class HSIRDYF_RO_InternalHighSpeedReadyFlag: std::uint32_t {
        fieldBitMask  = 1u << 2, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess   = Field::AccessRights::ReadOnly,

        noInterupt    = 0, // default
        readyInterupt = fieldBitMask, // set by hw, to clear write HSIRDYC
    };

    enum class HSERDYF_RO_ExternalHighSpeedReadyFlag: std::uint32_t {
        fieldBitMask  = 1u << 3, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess   = Field::AccessRights::ReadOnly,

        noInterupt    = 0, // default
        readyInterupt = fieldBitMask, // set by hw, to clear write HSERDYC
    };

    enum class PLLRDYF_RO_PhaseLockedLoopReadyFlag: std::uint32_t {
        fieldBitMask  = 1u << 4, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess   = Field::AccessRights::ReadOnly,

        noInterupt    = 0, // default
        readyInterupt = fieldBitMask, // set by hw, to clear write PLLRDYC
    };

    // 2 bits reseved

    enum class CSSF_RO_ExternalHighSpeedSecurityFlag: std::uint32_t {
        fieldBitMask    = 1u << 7, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess     = Field::AccessRights::ReadOnly,

        noInterupt      = 0, // default
        failureInterupt = fieldBitMask, // set by hw, to clear write CSSC
    };

    enum class LSIRDYIE_RW_InternalLowSpeedReadyInteruptEnable: std::uint32_t {
        fieldBitMask = 1u << 8, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0, // default
        enable       = fieldBitMask,
    };

    // 1 bit reserved

    enum class HSIRDYIE_RW_InternalHighSpeedReadyInteruptEnable: std::uint32_t {
        fieldBitMask = 1u << 10, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0, // default
        enable       = fieldBitMask,
    };

    enum class HSERDYIE_RW_ExternalHighSpeedReadyInteruptEnable: std::uint32_t {
        fieldBitMask = 1u << 11, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0, // default
        enable       = fieldBitMask,
    };

    enum class PLLRDYIE_RW_PhaseLockedLoopReadyInteruptEnable: std::uint32_t {
        fieldBitMask = 1u << 12, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::ReadWrite,

        disable      = 0, // default
        enable       = fieldBitMask,
    };

    // 3bits reserved

    enum class LSIRDYC_WO_InternalLowSpeedReadyClear: std::uint32_t {
        fieldBitMask = 1u << 16, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::WriteOnly,

        noAction     = 0, // default
        clearFlag    = fieldBitMask,
    };

    // 1bit reserved

    enum class HSIRDYC_WO_InternalHighSpeedReadyClear: std::uint32_t {
        fieldBitMask = 1u << 18, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::WriteOnly,

        noAction     = 0, // default
        clearFlag    = fieldBitMask,
    };

    enum class HSERDYC_WO_ExternalHighSpeedReadyClear: std::uint32_t {
        fieldBitMask = 1u << 19, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::WriteOnly,

        noAction     = 0, // default
        clearFlag    = fieldBitMask,
    };

    enum class PLLRDYC_WO_PhaseLockedLoopReadyClear: std::uint32_t {
        fieldBitMask = 1u << 20, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::WriteOnly,

        noAction     = 0, // default
        clearFlag    = fieldBitMask,
    };

    // 2bits reserved

    enum class CSSC_WO_ExternalHighSpeedSecurityClear: std::uint32_t {
        fieldBitMask = 1u << 23,  // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Field::AccessRights::WriteOnly,

        noAction     = 0, // default
        clearFlag    = fieldBitMask,
    };

    // 31-24 reserved
    struct metadata {
        static constexpr std::uint32_t offset = 0x08u;

        static constexpr std::tuple<
            LSIRDYF_RO_InternalLowSpeedReadyFlag,
            HSIRDYF_RO_InternalHighSpeedReadyFlag,
            HSERDYF_RO_ExternalHighSpeedReadyFlag,
            PLLRDYF_RO_PhaseLockedLoopReadyFlag,
            CSSF_RO_ExternalHighSpeedSecurityFlag,
            LSIRDYIE_RW_InternalLowSpeedReadyInteruptEnable,
            HSIRDYIE_RW_InternalHighSpeedReadyInteruptEnable,
            HSERDYIE_RW_ExternalHighSpeedReadyInteruptEnable,
            PLLRDYIE_RW_PhaseLockedLoopReadyInteruptEnable,
            LSIRDYC_WO_InternalLowSpeedReadyClear,
            HSIRDYC_WO_InternalHighSpeedReadyClear,
            HSERDYC_WO_ExternalHighSpeedReadyClear,
            PLLRDYC_WO_PhaseLockedLoopReadyClear,
            CSSC_WO_ExternalHighSpeedSecurityClear> fields = {};

        static constexpr auto getMetadata() {
            return std::tuple{offset, fields};
        }

    };

    template<typename RegField>
    concept IsAnyRegField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(metadata::fields)>();

}