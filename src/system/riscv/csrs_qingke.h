//
// Created by anton on 27/01/2025.
//

#pragma once

#include <cstdint>


namespace riscv::csr {
    // https://five-embeddev.com/riscv-priv-isa-manual/Priv-v1.12/priv-csrs.html
    // 11:10 bits => 00/01/10=read&write 11=read-only
    // 9:8   bits => 00=unprivileged 01=supervisor 10=hypervisor 11=machine


    constexpr std::uint16_t mask_read_write  = 0b11'00'0000'0000U;
    constexpr std::uint16_t mask_privileged  = 0b00'11'0000'0000U;


    enum class ReadWrite: std::uint16_t {
        read_write1 = 0b00'00'0000'0000U,
        read_write2 = 0b01'00'0000'0000U,
        read_write3 = 0b10'00'0000'0000U,
        read_only   = 0b11'00'0000'0000U
    };


    enum class Privilege: std::uint16_t {
        unprivileged = 0b00'00'0000'0000U,
        supervisor   = 0b00'01'0000'0000U,
        hypervisor   = 0b00'10'0000'0000U,
        machine      = 0b00'11'0000'0000U
    };


    // WCH QingKe V2 - https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
    enum class QingKeV2: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010U, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011U, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000U, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001U, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101U, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000U, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001U, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010U, // 0x342 MRW machine trap cause

        // Debug (supervisor) mode
        dcsr         = 0b01'11'1011'0000U, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001U, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010U, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0010U, // 0x7B2 DRW debug scratch register 1

        // Unprivileged - WCH vendor exclusive
        intsyscr     = 0b10'00'0000'0100U, // 0x804 URW interrupt system control
    };


    // WCH QingKe V3 - https://www.wch-ic.com/downloads/QingKeV3_Processor_Manual_PDF.html
    enum class QingKeV3: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010U, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011U, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000U, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001U, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101U, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000U, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001U, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010U, // 0x342 MRW machine trap cause
        mtval        = 0b00'11'0100'0011U, // 0x343 MRW machine bad address or instruction

        // Machine memory protection - 0x3A0-0x3AF MRW Physical memory protection configuration
        pmpcfg0      = 0b00'11'1010'0000U +  0U,
        pmpcfg1      = 0b00'11'1010'0000U +  1U,
        pmpcfg2      = 0b00'11'1010'0000U +  2U,
        pmpcfg3      = 0b00'11'1010'0000U +  3U,
        pmpcfg4      = 0b00'11'1010'0000U +  4U,
        pmpcfg5      = 0b00'11'1010'0000U +  5U,
        pmpcfg6      = 0b00'11'1010'0000U +  6U,
        pmpcfg7      = 0b00'11'1010'0000U +  7U,
        pmpcfg8      = 0b00'11'1010'0000U +  8U,
        pmpcfg9      = 0b00'11'1010'0000U +  9U,
        pmpcfg10     = 0b00'11'1010'0000U + 10U,
        pmpcfg11     = 0b00'11'1010'0000U + 11U,
        pmpcfg12     = 0b00'11'1010'0000U + 12U,
        pmpcfg13     = 0b00'11'1010'0000U + 13U,
        pmpcfg14     = 0b00'11'1010'0000U + 14U,
        pmpcfg15     = 0b00'11'1010'0000U + 15U,

        // Machine memory protection - 0x3B0-0x3BF MRW Physical memory protection address
        pmpaddr0     = 0b00'11'1011'0000U +  0U,
        pmpaddr1     = 0b00'11'1011'0000U +  1U,
        pmpaddr2     = 0b00'11'1011'0000U +  2U,
        pmpaddr3     = 0b00'11'1011'0000U +  3U,
        pmpaddr4     = 0b00'11'1011'0000U +  4U,
        pmpaddr5     = 0b00'11'1011'0000U +  5U,
        pmpaddr6     = 0b00'11'1011'0000U +  6U,
        pmpaddr7     = 0b00'11'1011'0000U +  7U,
        pmpaddr8     = 0b00'11'1011'0000U +  8U,
        pmpaddr9     = 0b00'11'1011'0000U +  9U,
        pmpaddr10    = 0b00'11'1011'0000U + 10U,
        pmpaddr11    = 0b00'11'1011'0000U + 11U,
        pmpaddr12    = 0b00'11'1011'0000U + 12U,
        pmpaddr13    = 0b00'11'1011'0000U + 13U,
        pmpaddr14    = 0b00'11'1011'0000U + 14U,
        pmpaddr15    = 0b00'11'1011'0000U + 15U,

        // Machine debug/trace
        tselect      = 0b01'11'1010'0000U, // 0x7A0 MRW debug/trace trigger
        tdata1       = 0b01'11'1010'0001U, // 0x7A1 MRW first debug/trace data
        tdata2       = 0b01'11'1010'0010U, // 0x7A2 MRW second debug/trace data

        // Supervisor debug
        dcsr         = 0b01'11'1011'0000U, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001U, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010U, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0011U, // 0x7B3 DRW debug scratch register 1

        // Unprivileged - WCH vendor exclusive
        gintenr      = 0b10'00'0000'0000U, // 0x800 URW global interrupt enable
        intsyscr     = 0b10'00'0000'0100U, // 0x804 URW interrupt system control

        // Machine - WCH vendor exclusive
        corecfgr     = 0b10'11'0000'0000U, // 0xBC0 MRW microprocessor configuration
        inestcr      = 0b10'11'0000'0001U, // 0xBC1 MRW interrupt nested control
    };


    // WCH QingKe V4 - https://www.wch-ic.com/downloads/QingKeV4_Processor_Manual_PDF.html
    enum class QingKeV4: std::uint16_t {
        // Machine information
        marchid      = 0b11'11'0001'0010U, // 0xF12 MRO architecture ID
        mimpid       = 0b11'11'0001'0011U, // 0xF13 MRO implementation ID

        // Machine trap setup
        mstatus      = 0b00'11'0000'0000U, // 0x300 MRW machine status
        misa         = 0b00'11'0000'0001U, // 0x301 MRO ISA and extension
        mtvec        = 0b00'11'0000'0101U, // 0x305 MRW machine trap-handler base address

        // Machine trap handling
        mscratch     = 0b00'11'0100'0000U, // 0x340 MRW machine scratch register for the handler
        mepc         = 0b00'11'0100'0001U, // 0x341 MRW machine exception PC (where the trap happened)
        mcause       = 0b00'11'0100'0010U, // 0x342 MRW machine trap cause
        mtval        = 0b00'11'0100'0011U, // 0x343 MRW machine bad address or instruction

        // Machine memory protection - 0x3A0-0x3AF MRW Physical memory protection configuration
        pmpcfg0      = 0b00'11'1010'0000U +  0U,
        pmpcfg1      = 0b00'11'1010'0000U +  1U,
        pmpcfg2      = 0b00'11'1010'0000U +  2U,
        pmpcfg3      = 0b00'11'1010'0000U +  3U,
        pmpcfg4      = 0b00'11'1010'0000U +  4U,
        pmpcfg5      = 0b00'11'1010'0000U +  5U,
        pmpcfg6      = 0b00'11'1010'0000U +  6U,
        pmpcfg7      = 0b00'11'1010'0000U +  7U,
        pmpcfg8      = 0b00'11'1010'0000U +  8U,
        pmpcfg9      = 0b00'11'1010'0000U +  9U,
        pmpcfg10     = 0b00'11'1010'0000U + 10U,
        pmpcfg11     = 0b00'11'1010'0000U + 11U,
        pmpcfg12     = 0b00'11'1010'0000U + 12U,
        pmpcfg13     = 0b00'11'1010'0000U + 13U,
        pmpcfg14     = 0b00'11'1010'0000U + 14U,
        pmpcfg15     = 0b00'11'1010'0000U + 15U,

        // Machine memory protection - 0x3B0-0x3BF MRW Physical memory protection address
        pmpaddr0     = 0b00'11'1011'0000U +  0U,
        pmpaddr1     = 0b00'11'1011'0000U +  1U,
        pmpaddr2     = 0b00'11'1011'0000U +  2U,
        pmpaddr3     = 0b00'11'1011'0000U +  3U,
        pmpaddr4     = 0b00'11'1011'0000U +  4U,
        pmpaddr5     = 0b00'11'1011'0000U +  5U,
        pmpaddr6     = 0b00'11'1011'0000U +  6U,
        pmpaddr7     = 0b00'11'1011'0000U +  7U,
        pmpaddr8     = 0b00'11'1011'0000U +  8U,
        pmpaddr9     = 0b00'11'1011'0000U +  9U,
        pmpaddr10    = 0b00'11'1011'0000U + 10U,
        pmpaddr11    = 0b00'11'1011'0000U + 11U,
        pmpaddr12    = 0b00'11'1011'0000U + 12U,
        pmpaddr13    = 0b00'11'1011'0000U + 13U,
        pmpaddr14    = 0b00'11'1011'0000U + 14U,
        pmpaddr15    = 0b00'11'1011'0000U + 15U,

        // Unprivileged floating point
        fflags       = 0b00'00'0000'0001U, // URW floating point accrued exception
        frm          = 0b00'00'0000'0010U, // URW floating point dynamic rounding mode
        fcsr         = 0b00'00'0000'0011U, // URW floating point control + status (frm + fflags)

        // Supervisor debug
        dcsr         = 0b01'11'1011'0000U, // 0x7B0 DRW debug control and status
        dpc          = 0b01'11'1011'0001U, // 0x7B1 DRW debug PC
        dscratch0    = 0b01'11'1011'0010U, // 0x7B2 DRW debug scratch register 0
        dscratch1    = 0b01'11'1011'0011U, // 0x7B3 DRW debug scratch register 1

        // Unprivileged - WCH vendor exclusive
        gintenr      = 0b10'00'0000'0000U, // 0x800 URW global interrupt enable
        intsyscr     = 0b10'00'0000'0100U, // 0x804 URW interrupt system control

        // Machine - WCH vendor exclusive
        corecfgr     = 0b10'11'0000'0000U, // 0xBC0 MRW microprocessor configuration
        cstrcr       = 0b10'11'0000'0010U, // 0xBC2 MRW cache policy configuration
        cpmpocr      = 0b10'11'0000'0011U, // 0xBC3 MRW cache policy overrides PMP control
        cmcr         = 0b10'11'1101'0000U, // 0xBD0 MWO cache operation control
        cinfor       = 0b11'11'1100'0000U, // 0xFC0 MRO cache information
    };

}