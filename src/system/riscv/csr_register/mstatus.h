//
// Created by anton on 30/01/2025.
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// https://five-embeddev.com/riscv-priv-isa-manual/Priv-v1.12/machine.html#machine-status-registers-mstatus-and-mstatush
// https://docs.openhwgroup.org/projects/cv32e40s-user-manual/en/latest/control_status_registers.html#machine-status-mstatus
// NOTE: WCH implementation differs from standard, the MPRV is not implemented but added non-standard MPOP and MPPOP

#pragma once

#include <cstdint>
#include <type_traits>


//TODO: unpriviledged vs user

namespace Riscv::Csr::Mstatus {


    enum class MieMachineInteruptEnable: std::uint32_t {
        fieldBitMask = 0b1'000, // THIS IS INTERNAL, don't use it
        disable      = 0,       // Globally disable machine level interupts (bit 3)
        enable       = 0b1'000, // Globally enable machine level interupts (bit 3)
    };


    enum class MpieMachinePreviousInteruptEnabled: std::uint32_t {
        fieldBitMask = 0b1'000'0'000, // THIS IS INTERNAL, don't use it
        disabled     = 0,             // Captured value of MIE was "disabled" prior interupt, mret to restore it (bit 7)
        enabled      = 0b1'000'0'000, // Captured value of MIE was "enabled" prior interupt, mret to restore it (bit 7)
    };


    enum class MppMachinePreviousPriviledge: std::uint32_t {
        fieldBitMask = 0b11'000'0'000'0'000, // THIS IS INTERNAL, don't use it
        user         = 0b00'000'0'000'0'000, // 00 user prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2
    //  supervisor   = 0b01'000'0'000'0'000, // 01 supervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
    //  hypervisor   = 0b10'000'0'000'0'000, // 10 hypervisor prior interupt, mret to restore it (bit 12-11). Unsupported by QingKeV2/V3/V4
        machine      = 0b11'000'0'000'0'000, // 11 machine prior interupt, mret to restore it (bit 12-11)
    };


    enum class Mpop: std::uint32_t {
        fieldBitMask = 0b1'0000000000'00'000'000'0'000, // THIS IS INTERNAL, don't use it
        disable      = 0,
    };


    enum class Mppop: std::uint32_t {
        fieldBitMask = 0b1'0'0000000000'00'000'000'0'000, // THIS IS INTERNAL, don't use it
        disable      = 0,
    };


    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, MieMachineInteruptEnable>  ||
        std::is_same_v<Field, MpieMachinePreviousInteruptEnabled> ||
        std::is_same_v<Field, MppMachinePreviousPriviledge>  ||
        std::is_same_v<Field, Mpop> ||
        std::is_same_v<Field, Mppop>;


}

