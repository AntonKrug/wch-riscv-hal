//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>


namespace Riscv::Csr {
    // https://five-embeddev.com/riscv-priv-isa-manual/Priv-v1.12/priv-csrs.html
    // 11:10 bits => 00/01/10=read&write 11=read-only
    // 9:8   bits => 00=unpriviledged 01=supervisor 10=hypervisor 11=machine


    constexpr std::uint16_t maskReadWrite  = 0b11'00'0000'0000;
    constexpr std::uint16_t maskPriviledge = 0b00'11'0000'0000;


    enum class ReadWrite: std::uint16_t {
        readWrite1 = 0b00'00'0000'0000,
        readWrite2 = 0b01'00'0000'0000,
        readWrite3 = 0b10'00'0000'0000,
        readOnly   = 0b11'00'0000'0000
    };


    enum class Priviledge: std::uint16_t {
        unpriviledged = 0b00'00'0000'0000,
        supervisor    = 0b00'01'0000'0000,
        hypervisor    = 0b00'10'0000'0000,
        machine       = 0b00'11'0000'0000
    };


    // WCH QingKe V2 - https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
    enum class QingKeV2: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010, // 0x342 MRW machine trap cause

        // Debug (supervisor) mode
        dcsr         = 0b01'11'1011'0000, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0010, // 0x7B2 DRW debug scratch register 1

        // Unprivileged - WCH vendor exclusive
        intsyscr     = 0b10'00'0000'0100, // 0x804 URW interrupt system control
    };


    // WCH QingKe V3 - https://www.wch-ic.com/downloads/QingKeV3_Processor_Manual_PDF.html
    enum class QingKeV3: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010, // 0x342 MRW machine trap cause
        mtval        = 0b00'11'0100'0011, // 0x343 MRW machine bad address or instruction

        // Machine memory protection - 0x3A0-0x3AF MRW Physical memory protection configuration
        pmpcfg0      = 0b00'11'1010'0000 +  0,
        pmpcfg1      = 0b00'11'1010'0000 +  1,
        pmpcfg2      = 0b00'11'1010'0000 +  2,
        pmpcfg3      = 0b00'11'1010'0000 +  3,
        pmpcfg4      = 0b00'11'1010'0000 +  4,
        pmpcfg5      = 0b00'11'1010'0000 +  5,
        pmpcfg6      = 0b00'11'1010'0000 +  6,
        pmpcfg7      = 0b00'11'1010'0000 +  7,
        pmpcfg8      = 0b00'11'1010'0000 +  8,
        pmpcfg9      = 0b00'11'1010'0000 +  9,
        pmpcfg10     = 0b00'11'1010'0000 + 10,
        pmpcfg11     = 0b00'11'1010'0000 + 11,
        pmpcfg12     = 0b00'11'1010'0000 + 12,
        pmpcfg13     = 0b00'11'1010'0000 + 13,
        pmpcfg14     = 0b00'11'1010'0000 + 14,
        pmpcfg15     = 0b00'11'1010'0000 + 15,

        // Machine memory protection - 0x3B0-0x3BF MRW Physical memory protection address
        pmpaddr0     = 0b00'11'1011'0000 +  0,
        pmpaddr1     = 0b00'11'1011'0000 +  1,
        pmpaddr2     = 0b00'11'1011'0000 +  2,
        pmpaddr3     = 0b00'11'1011'0000 +  3,
        pmpaddr4     = 0b00'11'1011'0000 +  4,
        pmpaddr5     = 0b00'11'1011'0000 +  5,
        pmpaddr6     = 0b00'11'1011'0000 +  6,
        pmpaddr7     = 0b00'11'1011'0000 +  7,
        pmpaddr8     = 0b00'11'1011'0000 +  8,
        pmpaddr9     = 0b00'11'1011'0000 +  9,
        pmpaddr10    = 0b00'11'1011'0000 + 10,
        pmpaddr11    = 0b00'11'1011'0000 + 11,
        pmpaddr12    = 0b00'11'1011'0000 + 12,
        pmpaddr13    = 0b00'11'1011'0000 + 13,
        pmpaddr14    = 0b00'11'1011'0000 + 14,
        pmpaddr15    = 0b00'11'1011'0000 + 15,

        // Machine debug/trace
        tselect      = 0b01'11'1010'0000, // 0x7A0 MRW debug/trace trigger
        tdata1       = 0b01'11'1010'0001, // 0x7A1 MRW first debug/trace data
        tdata2       = 0b01'11'1010'0010, // 0x7A2 MRW second debug/trace data

        // Supervisor debug
        dcsr         = 0b01'11'1011'0000, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0011, // 0x7B3 DRW debug scratch register 1

        // Unpriviledged - WCH vendor exclusive
        gintenr      = 0b10'00'0000'0000, // 0x800 URW global interupt enable
        intsyscr     = 0b10'00'0000'0100, // 0x804 URW interupt system control

        // Machine - WCH vendor exclusive
        corecfgr     = 0b10'11'0000'0000, // 0xBC0 MRW microprocessor configuration
        inestcr      = 0b10'11'0000'0001, // 0xBC1 MRW interupt nested control
    };


    // WCH QingKe V4 - https://www.wch-ic.com/downloads/QingKeV4_Processor_Manual_PDF.html
    enum class QingKeV4: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010, // 0x342 MRW machine trap cause
        mtval        = 0b00'11'0100'0011, // 0x343 MRW machine bad address or instruction

        // Machine memory protection - 0x3A0-0x3AF MRW Physical memory protection configuration
        pmpcfg0      = 0b00'11'1010'0000 +  0,
        pmpcfg1      = 0b00'11'1010'0000 +  1,
        pmpcfg2      = 0b00'11'1010'0000 +  2,
        pmpcfg3      = 0b00'11'1010'0000 +  3,
        pmpcfg4      = 0b00'11'1010'0000 +  4,
        pmpcfg5      = 0b00'11'1010'0000 +  5,
        pmpcfg6      = 0b00'11'1010'0000 +  6,
        pmpcfg7      = 0b00'11'1010'0000 +  7,
        pmpcfg8      = 0b00'11'1010'0000 +  8,
        pmpcfg9      = 0b00'11'1010'0000 +  9,
        pmpcfg10     = 0b00'11'1010'0000 + 10,
        pmpcfg11     = 0b00'11'1010'0000 + 11,
        pmpcfg12     = 0b00'11'1010'0000 + 12,
        pmpcfg13     = 0b00'11'1010'0000 + 13,
        pmpcfg14     = 0b00'11'1010'0000 + 14,
        pmpcfg15     = 0b00'11'1010'0000 + 15,

        // Machine memory protection - 0x3B0-0x3BF MRW Physical memory protection address
        pmpaddr0     = 0b00'11'1011'0000 +  0,
        pmpaddr1     = 0b00'11'1011'0000 +  1,
        pmpaddr2     = 0b00'11'1011'0000 +  2,
        pmpaddr3     = 0b00'11'1011'0000 +  3,
        pmpaddr4     = 0b00'11'1011'0000 +  4,
        pmpaddr5     = 0b00'11'1011'0000 +  5,
        pmpaddr6     = 0b00'11'1011'0000 +  6,
        pmpaddr7     = 0b00'11'1011'0000 +  7,
        pmpaddr8     = 0b00'11'1011'0000 +  8,
        pmpaddr9     = 0b00'11'1011'0000 +  9,
        pmpaddr10    = 0b00'11'1011'0000 + 10,
        pmpaddr11    = 0b00'11'1011'0000 + 11,
        pmpaddr12    = 0b00'11'1011'0000 + 12,
        pmpaddr13    = 0b00'11'1011'0000 + 13,
        pmpaddr14    = 0b00'11'1011'0000 + 14,
        pmpaddr15    = 0b00'11'1011'0000 + 15,

        // Unpriviledged floating point
        fflags       = 0b00'00'0000'0001, // URW floating point accrued exception
        frm          = 0b00'00'0000'0010, // URW floating point dynamic rounding mode
        fcsr         = 0b00'00'0000'0011, // URW floating point control + status (frm + fflags)

        // Supervisor debug
        dcsr         = 0b01'11'1011'0000, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0011, // 0x7B3 DRW debug scratch register 1

        // Unpriviledged - WCH vendor exclusive
        gintenr      = 0b10'00'0000'0000, // 0x800 URW global interupt enable
        intsyscr     = 0b10'00'0000'0100, // 0x804 URW interupt system control

        // Machine - WCH vendor exclusive
        corecfgr     = 0b10'11'0000'0000, // 0xBC0 MRW microprocessor configuration
        cstrcr       = 0b10'11'0000'0010, // 0xBC2 MRW cache policy configuration
        cpmpocr      = 0b10'11'0000'0011, // 0xBC3 MRW cache policy overrrides PMP control
        cmcr         = 0b10'11'1101'0000, // 0xBD0 MWO cache operation control
        cinfor       = 0b11'11'1100'0000, // 0xFC0 MRO cache information
    };

}