//
// Created by anton
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// NOTE: the WCH implemetion differs tiny bit from the standard spec as extra field

#pragma once

#include <cstdint>
#include <type_traits>

#include "system/register/util.h"


namespace Riscv::Csr::Mtvec {

    using namespace Soc::Reg::FieldAccessPriviledges;

    enum class Mode0_RW_VectorizationEnable: std::uint32_t {
        fieldBitMask             = 0b1u, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess              = FieldAccessRights::ReadWrite,

        singleUnifiedTrapHandler = 0,   // all interupts handled by only single handler
        vectorizedInterupts      = 0b1, // each interupt has dedicated memory location (BASEADDR + IRQ * 4) in the table, see Mode1
    };

    enum class Mode1_RW_VectorizedBehaviour: std::uint32_t {
        fieldBitMask          = 0b1u << 1,     // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess           = FieldAccessRights::ReadWrite,

        executeInstructions   = 0,            // use table for instructions, small relative jump instructions fit into, other instructions can be used too
        absoluteJumpAddresses = fieldBitMask, // use table for addresses, always jumping, but able to able to address any location
    };

    enum class BaseAddr_RW: std::uint32_t {
        // the address value needs to be 1k aligned (0x400)
        fieldBitMask = 0b111111111111111111111111111111u << 2, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = FieldAccessRights::ReadWrite,
    };

    template<typename Field>
    concept IsAnyField =
        std::is_same_v<Field, Mode0_RW_VectorizationEnable> ||
        std::is_same_v<Field, Mode1_RW_VectorizedBehaviour> ||
        std::is_same_v<Field, BaseAddr_RW>;

    template<auto Address>
    constexpr auto CheckVectorBaseAddressAlignment() -> std::uint32_t {
        static_assert(Address >= 0,         "Address must be positive number.");
        static_assert(Address % 0x400 == 0, "Address must be aligned to 1K boundary (0x400).");
        static_assert(
            ((Address >= 0)           && (Address < 0x78000))     || // in flash ROM 480kibi range
            ((Address >= 0x2000'0000) && (Address < 0x2004'0000))    // in sram  RAM 256kibi range
            , "Address must be inside ROM or RAM address range");
        return static_cast<std::uint32_t>(Address);
    }

    template<std::uint32_t IrqVector, Mode0_RW_VectorizationEnable Mode0, Mode1_RW_VectorizedBehaviour Mode1>
    constexpr auto CalculateMtvecRawValue() -> std::uint32_t {
        constexpr std::uint32_t irqVectorTableAddressSanitized =
            CheckVectorBaseAddressAlignment<IrqVector>();

        constexpr auto mtvecValue =
            Soc::Reg::combineEnumsToUint32<Mode0, Mode1>() + irqVectorTableAddressSanitized;

        return mtvecValue;
    }

}