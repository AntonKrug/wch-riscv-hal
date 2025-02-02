//
// Created by anton
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// NOTE: the WCH implemetion differs tiny bit from the standard spec as extra field

#pragma once

#include <cstdint>
#include <type_traits>


namespace Riscv::Csr::Mtvec {


    enum class Mode0VectorizationEnable: std::uint32_t {
        // Interrupt or exception entry address mode selection
        fieldBitMask             = 0b1, // THIS IS INTERNAL, don't use it
        singleUnifiedTrapHandler = 0,   // all interupts handled by only single handler
        vectorizedInterupts      = 0b1, // each interupt has dedicated memory location (BASEADDR + IRQ * 4) in the table, see Mode1
    };


    enum class Mode1VectorizedBehaviour: std::uint32_t {
        // Vectorized table behaviour
        fieldBitMask          = 0b1'0, // THIS IS INTERNAL, don't use it
        executeInstructions   = 0,     // use table for instructions, small relative jump instructions fit into, other instructions can be used too
        absoluteJumpAddresses = 0b1'0, // use table for addresses, always jumping, but able to able to address any location
    };


    enum class BaseAddr: std::uint32_t {
        // the value needs to be 1k aligned (0x400)
        fieldBitMask = 0b111111111111111111111111111111'0'0, // THIS IS INTERNAL, don't use it
    };


    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Mode0VectorizationEnable> ||
        std::is_same_v<Field, Mode1VectorizedBehaviour> ||
        std::is_same_v<Field, BaseAddr>;


    template<auto Address>
    constexpr std::uint32_t CheckVectorBaseAddressAlignment() {
        static_assert(Address >= 0,         "Address must be positive number.");
        static_assert(Address % 0x400 == 0, "Address must be aligned to 1K boundary (0x400).");
        static_assert(
            ((Address >= 0)           && (Address < 0x78000))     || // in flash ROM 480kibi range
            ((Address >= 0x2000'0000) && (Address < 0x2004'0000))    // in sram  RAM 256kibi range
            , "Address must be inside ROM or RAM address range");

        return static_cast<std::uint32_t>(Address);
    }


}