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

namespace Riscv::Csr::Mstatus {

    enum class Mie_MRW_MachineInteruptEnable: std::uint32_t {
        fieldBitMask = 0b1u << 3,    // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disable      = 0,            // Globally disable machine level interupts (bit 3)
        enable       = fieldBitMask, // Globally enable machine level interupts (bit 3)
    };

    enum class Mpie_MRW_MachinePreviousInteruptWasEnabled: std::uint32_t {
        fieldBitMask = 0b1u << 7,    // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disabled     = 0,            // Captured value of MIE was "disabled" prior interupt, mret to restore it (bit 7)
        enabled      = fieldBitMask, // Captured value of MIE was "enabled" prior interupt, mret to restore it (bit 7)
    };

    enum class Mpp_MRW_MachinePreviousPriviledge: std::uint32_t {
        fieldBitMask = 0b11u << 11,  // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        user         = 0b00u << 11,  // 00 user prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2
    //  supervisor   = 0b01u << 11,  // 01 supervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
    //  hypervisor   = 0b10u << 11,  // 10 hypervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
        machine      = 0b11u << 11,  // 11 machine prior interupt, mret to restore it (bit 12-11)
    };

    enum class Mpop_MRW: std::uint32_t {
        fieldBitMask = 0b1u << 23, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disable      = 0,
        enable       = fieldBitMask,
    };

    enum class Mppop_MRW: std::uint32_t {
        fieldBitMask = 0b1 << 23, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disable      = 0,
        enable       = fieldBitMask,
    };

    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Mie_MRW_MachineInteruptEnable>  ||
        std::is_same_v<Field, Mpie_MRW_MachinePreviousInteruptWasEnabled> ||
        std::is_same_v<Field, Mpp_MRW_MachinePreviousPriviledge>  ||
        std::is_same_v<Field, Mpop_MRW> ||
        std::is_same_v<Field, Mppop_MRW>;


}

