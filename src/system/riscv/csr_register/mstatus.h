//
// Created by anton on 30/01/2025.
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// https://five-embeddev.com/riscv-priv-isa-manual/Priv-v1.12/machine.html#machine-status-registers-mstatus-and-mstatush
// https://docs.openhwgroup.org/projects/cv32e40s-user-manual/en/latest/control_status_registers.html#machine-status-mstatus
// https://danielmangum.com/posts/risc-v-bytes-privilege-levels/
//
// NOTE: WCH implementation differs from standard, the MPRV is not implemented but added non-standard MPOP and MPPOP
//       also doesn't support supervisor and hypervisor privilege modes

#pragma once

#include <cstdint>
#include <type_traits>

#include "system/register/util.h"

//TODO: unpriviledged vs user

namespace riscv::csr::mstatus {

    enum class Mie_MRW_MachineInteruptEnable: std::uint32_t {
        fieldBitMask = 0b1U << 3U,    // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = soc::reg::field_access_right::ReadWrite,

        disable      = 0U,            // Globally disable machine level interupts (bit 3)
        enable       = fieldBitMask, // Globally enable machine level interupts (bit 3)
    };

    enum class Mpie_MRW_MachinePreviousInteruptWasEnabled: std::uint32_t {
        fieldBitMask = 0b1U << 7U,    // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = soc::reg::field_access_right::ReadWrite,

        disabled     = 0U,            // Captured value of MIE was "disabled" prior interupt, mret to restore it (bit 7)
        enabled      = fieldBitMask, // Captured value of MIE was "enabled" prior interupt, mret to restore it (bit 7)
    };

    enum class Mpp_MRW_MachinePreviousPriviledge: std::uint32_t {
        fieldBitMask = 0b11U << 11U,  // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = soc::reg::field_access_right::ReadWrite,

        user         = 0b00U << 11U,  // 00 user prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2
    //  supervisor   = 0b01u << 11U,  // 01 supervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
    //  hypervisor   = 0b10u << 11U,  // 10 hypervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
        machine      = 0b11U << 11U,  // 11 machine prior interupt, mret to restore it (bit 12-11)
    };

    enum class Mpop_MRW: std::uint32_t {
        fieldBitMask = 0b1U << 23U, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = soc::reg::field_access_right::ReadWrite,

        disable      = 0U,
        enable       = fieldBitMask,
    };

    enum class Mppop_MRW: std::uint32_t {
        fieldBitMask = 0b1U << 23U, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = soc::reg::field_access_right::ReadWrite,

        disable      = 0U,
        enable       = fieldBitMask,
    };

    template<typename Field>
    concept is_any_field =
        std::is_same_v<Field, Mie_MRW_MachineInteruptEnable>  ||
        std::is_same_v<Field, Mpie_MRW_MachinePreviousInteruptWasEnabled> ||
        std::is_same_v<Field, Mpp_MRW_MachinePreviousPriviledge>  ||
        std::is_same_v<Field, Mpop_MRW> ||
        std::is_same_v<Field, Mppop_MRW>;


}

