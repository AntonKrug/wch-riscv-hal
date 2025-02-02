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


namespace Riscv::Csr::Mstatus {


    enum class Mie: std::uint32_t {
        fieldBitMask      = 0b1'000, // THIS IS INTERNAL, don't use it
        machineIrqDisable = 0,      // Globally disable machine level IRQs (bit 3)
        machineIrqEnable  = 0b1'000, // Globally enable machine level IRQs (bit 3)
    };


    enum class Mpie: std::uint32_t { // AAAAAAAAAAAAAAAAAAAAAAAAAaa
        fieldBitMask              = 0b1'000'0'000, // THIS IS INTERNAL, don't use it
        machinePreviousIrqDisable = 0,            // Captured value of MIE was "disabled" when an interupt happend, mret will restore it back (bit 7)
        machinePreviousIrqEnable  = 0b1'000'0'000, // Captured value of MIE was "enabled" when an interupt happend, mret will restore it back (bit 7)
    };


    enum class Mpp: std::uint32_t {
        fieldBitMask                             = 0b11'000'0'000'0'000, // THIS IS INTERNAL, don't use it
        machinePreviousPriviledgeIsUnpriviledged = 0b00'000'0'000'0'000, // 00 Unpriviledged mode was set prior handling interupt as machine mode, mret will restore it (bit 12-11)
        machinePreviousPriviledgeIsSupervisor    = 0b01'000'0'000'0'000, // 01 Supervisor mode was set prior handling interupt as machine mode, mret will restore it (bit 12-11)
        machinePreviousPriviledgeIsHypervisor    = 0b10'000'0'000'0'000, // 10 Hypervisor mode was set prior handling interupt as machine mode, mret will restore it (bit 12-11)
        machinePreviousPriviledgeIsMachine       = 0b11'000'0'000'0'000, // 11 Machine mode was set prior handling interupt as machine mode, mret will restore it (bit 12-11)
    };


    enum class Mpop: std::uint32_t {
        fieldBitMask          = 0b1'0000000000'00'000'000'0'000, // THIS IS INTERNAL, don't use it
        machineModeIrqDisable = 0,
    };


    enum class Mppop: std::uint32_t {
        fieldBitMask          = 0b1'0'0000000000'00'000'000'0'000, // THIS IS INTERNAL, don't use it
        machineModeIrqDisable = 0,
    };


    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Mie>  ||
        std::is_same_v<Field, Mpie> ||
        std::is_same_v<Field, Mpp>  ||
        std::is_same_v<Field, Mpop> ||
        std::is_same_v<Field, Mppop>;


}

