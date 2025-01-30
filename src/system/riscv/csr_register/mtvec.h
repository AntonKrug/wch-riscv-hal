//
// Created by anton
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// NOTE: the WCH implemetion differs tiny bit from the standard spec

#pragma once

#include <cstdint>
#include <type_traits>


namespace Riscv::Csr::Mtvec {


    enum class Mode0: std::uint32_t {
        // Interrupt or exception entry address mode selection
        fieldBitMask             = 0b1,
        singleUnifiedTrapHandler = 0b0, // all interupts handled by the same code
        vectorizedInterupts      = 0b1, // jump to addr => BASEADDR + IRQ * 4
    };


    enum class Mode1: std::uint32_t {
        // Vectorized table behaviour
        fieldBitMask          = 0b1'0,
        executeInstructions   = 0b0'0, // small relative jump instructions fit into the table, other instructions will be executed too
        absoluteJumpAddresses = 0b1'0, // always jumping, but able to able to address any location
    };


    enum class BaseAddr: std::uint32_t {
        fieldBitMask = 0b111111111111111111111111111111'0'0,
    };


    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Mode0> ||
        std::is_same_v<Field, Mode1> ||
        std::is_same_v<Field, BaseAddr>;


}