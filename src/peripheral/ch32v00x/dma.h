//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include <type_traits>

#include "field/dma/cfgr.h"
#include "field/dma/intfcr.h"
#include "field/dma/intfr.h"
#include "system/register/access_primitives.h"
#include "system/register/util.h"

namespace Peripheral::Dma {

    constexpr static std::uint32_t constBaseAddr             = 0x4002'0000U;
    constexpr static std::uint32_t constInstanceOffset       =       0x400U; // DMA2_INTFR - DMA1_INTFR
    constexpr static std::uint32_t constInitfrExtendedOffset =       0x0d0U; // DMA2_EXTEM_INTFR - DMA2_INTFR
    constexpr static std::uint32_t constRegisterSpacing      =         0x4U; // DMA1_INTFCR - DMA1_INTFR
    constexpr static std::uint32_t constCfgrOffset           =         0x8U; // DMA1_CFGR1 - DMA1_INTFR
    constexpr static std::uint32_t constChannelSpacing       =        0x14U; // DMA1_CFGR2 - DMA1_CFGR1

    // DMA boundaries section and sentences are confusing and in some cases do not make sense
    // "penultimate digit" second last number 0, 1, 2
    // DMA 1 channels must be on 64K bondaries 0-64K or 64k-128k 63k-65k
    // CH32v307VCT6 with 128k ram
    // DMA1 channel 2, 3, 4, 5 128k boundaries
    // DMA1 channel 1, 6, 7 needs 64k boundaries

    enum class Id: std::uint32_t {
        // DMA1 ch1 triggers
        Adc1HwTrigger     = 0x011U, // DMA1 ch1
        Tim2Ch3HwTrigger  = 0x011U, // DMA1 ch1
        Dma1Ch1SwTrigger  = 0x111U, // DMA1 ch1

        // DMA1 ch2 triggers
        Spi1RxHwTrigger   = 0x012U, // DMA1 ch2
        Tim1Ch1HwTrigger  = 0x012U, // DMA1 ch2
        Tim2UpHwTrigger   = 0x012U, // DMA1 ch2
        Dma1Ch2SwTrigger  = 0x112U, // DMA1 ch2

        // DMA1 ch3 triggers
        Spi1TxHwTrigger   = 0x013U, // DMA1 ch3
        Tim1Ch2HwTrigger  = 0x013U, // DMA1 ch3
        Dma1Ch3SwTrigger  = 0x113U, // DMA1 ch3

        // DMA1 ch4 triggers
        Usart1TxHwTrigger = 0x014U, // DMA1 ch4
        Tim1Ch4HwTrigger  = 0x014U, // DMA1 ch4
        Tim1TrigHwTrigger = 0x014U, // DMA1 ch4
        Tim1ComHwTrigger  = 0x014U, // DMA1 ch4
        Dma1Ch4SwTrigger  = 0x114U, // DMA1 ch4

        // DMA1 ch5 triggers
        Usart1RxHwTrigger = 0x015U, // DMA1 ch5
        Tim1UpHwTrigger   = 0x015U, // DMA1 ch5
        Tim2Ch1HwTrigger  = 0x015U, // DMA1 ch5
        Dma1Ch5SwTrigger  = 0x115U, // DMA1 ch5

        // DMA1 ch6 triggers
        I2c1TxHwTrigger   = 0x016U, // DMA1 ch6
        Tim1Ch3HwTrigger  = 0x016U, // DMA1 ch6
        Dma1Ch6SwTrigger  = 0x116U, // DMA1 ch6

        // DMA1 ch7 triggers
        I2cRxHwTrigger    = 0x017U, // DMA1 ch7
        Tim2Ch2HwTrigger  = 0x017U, // DMA1 ch7
        Tim2Ch4HwTrigger  = 0x017U, // DMA1 ch7
        Dma1Ch7SwTrigger  = 0x117U, // DMA1 ch7
    };

    constexpr auto idToTruncateTrigger = [](const Id id) constexpr -> std::uint32_t { return   static_cast<std::uint32_t>(id) & 0x0ffU; };
    constexpr auto idToDmaInstance     = [](const Id id) constexpr -> std::uint32_t { return   static_cast<std::uint32_t>(id) & 0x00fU; };
    constexpr auto idToChannel         = [](const Id id) constexpr -> std::uint32_t { return  (static_cast<std::uint32_t>(id) & 0x0f0U) >> 8U; };
    constexpr auto idIsHwTrigger       = [](const Id id) constexpr -> bool          { return ((static_cast<std::uint32_t>(id) & 0xf00U) == 0U); };
    constexpr auto idIsExtended        = [](const Id id) constexpr -> bool          { return idToDmaInstance(id)==2U && idToChannel(id)>8U; };

    constexpr auto addressInstanceBase = [](const Id id) constexpr -> std::uint32_t {
        return constBaseAddr + (idToDmaInstance(id) * constInstanceOffset);
    };

    constexpr auto addressChannelRegister = [](const Id id, const std::uint32_t registerCount) constexpr -> std::uint32_t {
        return addressInstanceBase(id) + constCfgrOffset + (idToChannel(id) * constChannelSpacing) + (registerCount * constRegisterSpacing);
    };

    constexpr auto addressIntfr  = [](const Id id) constexpr -> std::uintptr_t { return addressInstanceBase(id) + (idIsExtended(id) ? constInitfrExtendedOffset : 0U); };
    constexpr auto addressIntfcr = [](const Id id) constexpr -> std::uintptr_t { return addressIntfr(id) + constRegisterSpacing; };
    constexpr auto addressCfgr   = [](const Id id) constexpr -> std::uintptr_t { return addressChannelRegister(id, 0U); };
    constexpr auto addressCntr   = [](const Id id) constexpr -> std::uintptr_t { return addressChannelRegister(id, 1U); };
    constexpr auto addressPaddr  = [](const Id id) constexpr -> std::uintptr_t { return addressChannelRegister(id, 2U); };
    constexpr auto addressMaddr  = [](const Id id) constexpr -> std::uintptr_t { return addressChannelRegister(id, 3U); };

    template<Id TplIdHead, Id... TplIdTail>
    inline constexpr auto
    __attribute__ ((always_inline))
    noDuplicateId() -> void {
        // Check if current HEAD has duplicate in the TAIL section
        static_assert(
            ( (idToTruncateTrigger(TplIdHead) != idToTruncateTrigger(TplIdTail)) && ... ),
            "Duplicate IDs used, there might be overlap between HW and SW triggers, or multiple HW triggers used of the same instance and channel");

        // If there are 2 or more TAIL entries (after truncating HEAD), then
        // make the first TAIL entry as new HEAD and then keep checking for more combinations
        if constexpr (sizeof...(TplIdTail) >= 2U) {
            noDuplicateId<TplIdTail...>();
        }
    }

    enum class Direction {
        PeripheralToMemory, // MEM2MEM=0, DIR=0, PADDR (source) => MADDR (destination)
        MemoryToPeripheral, // MEM2MEM=0, DIR=1, MADDR (source) => PADDR (destination)
        MemoryToMemory      // MEM2MEM=1, DIR=x, PADDR (source) => MADDR (destination)
    };

    using Priority            = Cfgr::PL_RW_ChannelPriority;
    using PeripheralAlignment = Cfgr::PSIZE_RW_PeripheralAlignment;
    using MemoryAlignment     = Cfgr::MSIZE_RW_MemoryAlignment;


    // template<SizeAlignment alignment>
    // constexpr auto sizeAlignmentToMsize() -> Cfgr::MSIZE_RW_MemoryAlignment {
    //     constexpr auto value = static_cast<std::uint32_t>(alignment);
    //     constexpr auto offsetValue = value << static_cast<std::uint32_t>(Cfgr::MSIZE_RW_MemoryAlignment::fieldBitOffset);
    //     return static_cast<Cfgr::MSIZE_RW_MemoryAlignment>(offsetValue);
    // }
    //
    // template<SizeAlignment alignment>
    // constexpr auto sizeAlignmentToPsize() -> Cfgr::PSIZE_RW_PeripheralAlignment {
    //     constexpr auto value = static_cast<std::uint32_t>(alignment);
    //     constexpr auto offsetValue = value << static_cast<std::uint32_t>(Cfgr::PSIZE_RW_PeripheralAlignment::fieldBitOffset);
    //     return static_cast<Cfgr::PSIZE_RW_PeripheralAlignment>(offsetValue);
    // }

    template<typename TplPointerType>
    requires std::is_pointer_v<TplPointerType>
    constexpr auto pointerToPeripheralSizeAlignment() -> Cfgr::PSIZE_RW_PeripheralAlignment {
        return Soc::Reg::valueToRegisterFieldEnum<alignof(TplPointerType), Cfgr::PSIZE_RW_PeripheralAlignment>();
    }

    template<typename TplPointerType>
    requires std::is_pointer_v<TplPointerType>
    constexpr auto pointerToMemorySizeAlignment() -> Cfgr::MSIZE_RW_MemoryAlignment {
        return Soc::Reg::valueToRegisterFieldEnum<alignof(TplPointerType), Cfgr::MSIZE_RW_MemoryAlignment>();
    }

    template<
        Id            TplRequester,
        std::uint32_t TplMemoryAddress,
        std::uint16_t TplSizeOfTransmission
    >
    constexpr auto isInBoundariesCheckCt() -> void {
        constexpr auto instance = idToDmaInstance(TplRequester);
        constexpr auto channel  = idToChannel(TplRequester);

        static_assert(
            (instance == 1U && (channel == 2U || channel == 3U || channel == 4U || channel == 5U)) &&
            ((TplMemoryAddress + TplSizeOfTransmission - 1U) <= 131072U), "Breaking 128k boundary");

        static_assert(
            (instance == 1U && (channel == 1U || channel == 6U || channel == 7U)) &&
            ((TplMemoryAddress + TplSizeOfTransmission -1U) <= 65535U), "Breaking 128k boundary");
    }

    template<
        PeripheralAlignment TplPeripheralAlignment,
        MemoryAlignment     TplMemoryAlignment>
    constexpr auto comparePAddressWithMAddressAlignment() -> std::int32_t {
        constexpr auto pAlign = static_cast<std::uint32_t>(TplPeripheralAlignment) >> static_cast<std::uint32_t>(PeripheralAlignment::fieldBitOffset);
        constexpr auto mAlign = static_cast<std::uint32_t>(TplMemoryAlignment) >> static_cast<std::uint32_t>(MemoryAlignment::fieldBitOffset);
        if (pAlign > mAlign) {
            return 1;
        } else if (pAlign < mAlign) {
            return -1;
        } else {
            // pAlign == mAlign
            return 0;
        }
    }

    template<
        Id            TplRequester,
        std::uint16_t TplSizeOfTransmission
    >
    [[nodiscard]] constexpr auto isInBoundariesCheck(std::uint32_t memoryAddress) -> bool {
        constexpr auto instance = idToDmaInstance(TplRequester);
        constexpr auto channel  = idToChannel(TplRequester);

        if constexpr (instance == 1U && (channel == 2U || channel == 3U || channel == 4U || channel == 5U)) {
            return ((memoryAddress + TplSizeOfTransmission - 1U) <= 131072U);
        } else if constexpr (instance == 1U && (channel == 1U || channel == 6U || channel == 7U)) {
            return ((memoryAddress + TplSizeOfTransmission - 1U) <= 131072U);
        } else {
            return true;
        }
    }

    template<
        Id                  TplRequesterId,
        std::uint32_t       TplSourceAddress,
        PeripheralAlignment TplSourceAlignment,
        bool                TplSourceIncrement,
        bool                TplDestinationIncrement,
        std::uint16_t       TplTransferCount,
        bool                TplCyclicMode,
        Priority            TplPriority,
        bool                TplTransmissionErrorIrq,
        bool                TplHalfTransmissionIrq,
        bool                TplFullTransmissionIrq,
        bool                TplEnableDma,
        typename            TplDestinationPointerType
    >
    requires std::is_pointer_v<TplDestinationPointerType>
    inline constexpr auto
    __attribute__ ((always_inline))
    initPeripheralToMemoryCt(const TplDestinationPointerType destinationPointer) -> void {
        constexpr auto isHwTrigger          = idIsHwTrigger(TplRequesterId);
        constexpr auto sourceIncrement      = Soc::Reg::boolToRegisterFieldEnum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = Soc::Reg::boolToRegisterFieldEnum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto isCyclic             = Soc::Reg::boolToRegisterFieldEnum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = Soc::Reg::boolToRegisterFieldEnum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = Soc::Reg::boolToRegisterFieldEnum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();

        constexpr auto destinationAlignment = pointerToMemorySizeAlignment<TplDestinationPointerType>();

        static_assert(
            TplTransferCount > 0U ||
            ( TplTransferCount == 0U && TplSourceIncrement == false && TplDestinationIncrement == false ),
            "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
        );

        // static_assert(isHwTrigger == true, "For PeripheralToMemory Requester ID indicates that ");

        Soc::Reg::Access::writeCtAddrVal<addressCntr(TplRequesterId), static_cast<std::uint32_t>(TplTransferCount)>();
        Soc::Reg::Access::writeCtAddrVal<addressPaddr(TplRequesterId), TplSourceAddress>();
        Soc::Reg::Access::writeCtAddr<addressMaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(destinationPointer));

        Soc::Reg::Access::writeCtAddrVal<
            addressCfgr(TplRequesterId),
            Soc::Reg::Combine::enumsToUint32<
                Cfgr::MEM2MEM_RW_MemoryToMemory::disable,
                TplPriority,
                destinationAlignment,
                TplSourceAlignment,
                destinationIncrement,
                sourceIncrement,
                isCyclic,
                Cfgr::DIR_RW_DataTransferDirection::readFromPeripheral,
                irqTransmissionError,
                irqHalfTransmission,
                irqFullTransmission,
                isEnabled
            >()
        >();
    }

    template<
        Id                  TplRequesterId,
        bool                TplSourceIncrement,
        std::uint32_t       TplDestinationAddress,
        PeripheralAlignment TplDestinationAlignment,
        bool                TplDestinationIncrement,
        std::uint16_t       TplTransferCount,
        bool                TplCyclicMode,
        Priority            TplPriority,
        bool                TplTransmissionErrorIrq,
        bool                TplHalfTransmissionIrq,
        bool                TplFullTransmissionIrq,
        bool                TplEnableDma,
        typename            TplSourcePointerType
    >
    requires std::is_pointer_v<TplSourcePointerType>
    inline constexpr auto
    __attribute__ ((always_inline))
    initMemoryToPeripheralCt(const TplSourcePointerType sourcePointer) -> void {
        constexpr auto isHwTrigger          = idIsHwTrigger(TplRequesterId);
        constexpr auto sourceIncrement      = Soc::Reg::boolToRegisterFieldEnum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = Soc::Reg::boolToRegisterFieldEnum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto isCyclic             = Soc::Reg::boolToRegisterFieldEnum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = Soc::Reg::boolToRegisterFieldEnum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = Soc::Reg::boolToRegisterFieldEnum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();

        constexpr auto sourceAlignment = pointerToMemorySizeAlignment<TplSourcePointerType>();

        static_assert(
            TplTransferCount > 0U ||
            ( TplTransferCount == 0U && TplSourceIncrement == false && TplDestinationIncrement == false ),
            "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
        );

        Soc::Reg::Access::writeCtAddrVal<addressCntr(TplRequesterId), static_cast<std::uint32_t>(TplTransferCount)>();
        Soc::Reg::Access::writeCtAddrVal<addressPaddr(TplRequesterId), TplDestinationAddress>();
        Soc::Reg::Access::writeCtAddr<addressMaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(sourcePointer));

        Soc::Reg::Access::writeCtAddrVal<
            addressCfgr(TplRequesterId),
            Soc::Reg::Combine::enumsToUint32<
                Cfgr::MEM2MEM_RW_MemoryToMemory::disable,
                TplPriority,
                sourceAlignment,
                TplDestinationAlignment,
                destinationIncrement,
                sourceIncrement,
                isCyclic,
                Cfgr::DIR_RW_DataTransferDirection::readFromMemory,
                irqTransmissionError,
                irqHalfTransmission,
                irqFullTransmission,
                isEnabled
            >()
        >();
    }

    template<
        Id            TplRequesterId,
        bool          TplSourceIncrement,
        bool          TplDestinationIncrement,
        std::uint16_t TplTransferCount,
        Priority      TplPriority,
        bool          TplTransmissionErrorIrq,
        bool          TplHalfTransmissionIrq,
        bool          TplFullTransmissionIrq,
        bool          TplEnableDma,
        typename      TplSourcePointerType,
        typename      TplDestinationPointerType
    >
    requires std::is_pointer_v<TplDestinationPointerType>
    inline constexpr auto
    __attribute__ ((always_inline))
    initMemoryToMemoryCt(
        const TplSourcePointerType      sourcePointer,
        const TplDestinationPointerType destinationPointer
    ) -> void {
        constexpr auto isHwTrigger          = idIsHwTrigger(TplRequesterId);
        constexpr auto sourceIncrement      = Soc::Reg::boolToRegisterFieldEnum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = Soc::Reg::boolToRegisterFieldEnum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto irqTransmissionError = Soc::Reg::boolToRegisterFieldEnum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        // When enabling channel in MemoryToMemory mode, it will start transition instantly
        constexpr auto isEnabled            = Soc::Reg::boolToRegisterFieldEnum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();

        constexpr auto sourceAlignment      = pointerToPeripheralSizeAlignment<TplSourcePointerType>();
        constexpr auto destinationAlignment = pointerToMemorySizeAlignment<TplDestinationPointerType>();

        static_assert(
            TplTransferCount > 0U ||
            ( TplTransferCount == 0U && TplSourceIncrement == false && TplDestinationIncrement == false ),
            "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
        );

        if constexpr (TplTransferCount > 0U) {
            //TODO check the read and write ranges do not overlap
        }

        Soc::Reg::Access::writeCtAddrVal<addressCntr(TplRequesterId), static_cast<std::uint32_t>(TplTransferCount)>();
        Soc::Reg::Access::writeCtAddr<addressPaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(sourcePointer));
        Soc::Reg::Access::writeCtAddr<addressMaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(destinationPointer));

        Soc::Reg::Access::writeCtAddrVal<
            addressCfgr(TplRequesterId),
            Soc::Reg::Combine::enumsToUint32<
                Cfgr::MEM2MEM_RW_MemoryToMemory::enable,
                TplPriority,
                destinationAlignment,
                sourceAlignment,
                destinationIncrement,
                sourceIncrement,
                Cfgr::CIRC_RW_CyclicMode::disable,
                Cfgr::DIR_RW_DataTransferDirection::readFromPeripheral,
                irqTransmissionError,
                irqHalfTransmission,
                irqFullTransmission,
                isEnabled
            >()
        >();
    }

    template<
        Id                  TplRequesterId,
        Direction           TplDirection,
        PeripheralAlignment TplPeripheralAlignment,
        bool                TplPeripheralIncrement,
        MemoryAlignment     TplMemoryAlignment,
        bool                TplMemoryIncrement,
        std::uint16_t       TplTransferCount,
        bool                TplCyclicMode,
        Priority            TplPriority,
        bool                TplTransmissionErrorIrq,
        bool                TplHalfTransmissionIrq,
        bool                TplFullTransmissionIrq,
        bool                TplEnableDma,
        bool                TplAllowTrucatingTransfer,
        typename            TplPeripheralPointerType,
        typename            TplMemoryPointerType
    >
    requires std::is_pointer_v<TplPeripheralPointerType> and std::is_pointer_v<TplMemoryPointerType>
    inline constexpr auto
    __attribute__ ((always_inline))
    initGenericCt(
        const TplPeripheralPointerType peripheralPointer,
        const TplMemoryPointerType memoryPointer
    ) -> void {
        constexpr auto isHwTrigger          = idIsHwTrigger(TplRequesterId);
        constexpr auto isMemoryToMemory     = TplDirection == Direction::MemoryToMemory;
        constexpr auto isCyclic             = Soc::Reg::boolToRegisterFieldEnum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = Soc::Reg::boolToRegisterFieldEnum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = Soc::Reg::boolToRegisterFieldEnum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = Soc::Reg::boolToRegisterFieldEnum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();

        if constexpr (TplDirection == Direction::MemoryToMemory) {
            static_assert(isHwTrigger == false, "MemomryToMemory mode can't be triggered by HW requester ID, it will be triggered instanly as EN field is enabled");
            static_assert(TplCyclicMode == false, "MemoryToMemory mode can't be used with Cyclic mode");
        }

        if constexpr (TplTransferCount == 0U) {
            static_assert(TplPeripheralIncrement == false && TplMemoryIncrement == false,
                "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
            );
        }

        // Allow manually "override" these alignment asserts with TplAllowTrucatingTransfer if the truncatitation is intentional
        if constexpr (TplAllowTrucatingTransfer == false) {

            // MemoryToMemory      MEM2MEM=1, DIR=x  PADDR (source) => MADDR (destination)
            static_assert(
                TplDirection == Direction::MemoryToMemory &&
                comparePAddressWithMAddressAlignment<TplPeripheralAlignment, TplMemoryAlignment>() <= 0,
                "In MemoryToMemory mode, the PADDR (source) needs to have same or smaller alignment than MADDR (destination), or data truncation will happen");

            // MemoryToPeripheral, MEM2MEM=0, DIR=1  MADDR (source) => PADDR (destination)
            static_assert(
                TplDirection == Direction::MemoryToPeripheral &&
                comparePAddressWithMAddressAlignment<TplPeripheralAlignment, TplMemoryAlignment>() >= 0,
                "In MemoryToPeripheral mode, the MADDR (source) needs to have same or smaller alignment than PADDR (destination), or data truncation will happen");

            // PeripheralToMemory, MEM2MEM=0, DIR=0  PADDR (source) => MADDR (destination)
            static_assert(
                TplDirection == Direction::MemoryToMemory &&
                comparePAddressWithMAddressAlignment<TplPeripheralAlignment, TplMemoryAlignment>() <= 0,
                "In PeripheralToMemory mode, the PADDR (source) needs to have same or smaller alignment than MADDR (destination), or data truncation will happen");
        }

        Soc::Reg::Access::writeCtAddrVal<addressCntr(TplRequesterId), static_cast<std::uint32_t>(TplTransferCount)>();
        Soc::Reg::Access::writeCtAddr<addressPaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(peripheralPointer));
        Soc::Reg::Access::writeCtAddr<addressMaddr(TplRequesterId)>(reinterpret_cast<std::uint32_t>(memoryPointer));

        Soc::Reg::Access::writeCtAddrVal<
            addressCfgr(TplRequesterId),
            Soc::Reg::Combine::enumsToUint32<
                TplDirection,
                isMemoryToMemory,
                TplPriority,
                TplMemoryAlignment,
                TplMemoryIncrement,
                TplPeripheralAlignment,
                TplPeripheralIncrement,
                isCyclic,
                irqTransmissionError,
                irqHalfTransmission,
                irqFullTransmission,
                isEnabled
            >()
        >();
    }

}
