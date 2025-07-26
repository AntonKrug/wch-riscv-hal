//
// Created by Fredy on 4/17/2025.
//

#pragma once

#include <type_traits>

#include "peripheral/ch32v00x/dma_channels.h"

#include "dma/field/cfgr.h"
#include "dma/field/intfcr.h"
#include "dma/field/intfr.h"
#include "system/register/access_primitives.h"
#include "system/register/util.h"

namespace peripheral::dma {

    // DMA boundaries section and sentences are confusing and in some cases do not make sense
    // "penultimate digit" second last number 0, 1, 2
    // DMA 1 channels must be on 64K bondaries 0-64K or 64k-128k 63k-65k
    // CH32v307VCT6 with 128k ram
    // DMA1 channel 2, 3, 4, 5 128k boundaries
    // DMA1 channel 1, 6, 7 needs 64k boundaries

    #pragma region Constants

    constexpr static std::uint32_t const_base_addr              = 0x4002'0000U;
    constexpr static std::uint32_t const_instance_offset        =       0x400U; // DMA2_INTFR - DMA1_INTFR
    constexpr static std::uint32_t const_initfr_extended_offset =       0x0d0U; // DMA2_EXTEM_INTFR - DMA2_INTFR
    constexpr static std::uint32_t const_register_spacing       =         0x4U; // DMA1_INTFCR - DMA1_INTFR
    constexpr static std::uint32_t const_cfgr_offset            =         0x8U; // DMA1_CFGR1 - DMA1_INTFR
    constexpr static std::uint32_t const_channel_spacing        =        0x14U; // DMA1_CFGR2 - DMA1_CFGR1
    constexpr static std::uint32_t const_fields_per_channel     =           4U; // INTFR/INTFCR have 4 fields per channel

    #pragma endregion

    #pragma region Types - move to separate header eventually - split common logic away from SoC specific

    // No need to invent all new types if existing register field is more suiting and we can use 'alias'
    using Priority            = Cfgr::PL_RW_ChannelPriority;
    using PeripheralAlignment = Cfgr::PSIZE_RW_PeripheralAlignment;
    using MemoryAlignment     = Cfgr::MSIZE_RW_MemoryAlignment;

    enum class Direction {
        PeripheralToMemory, // MEM2MEM=0, DIR=0, PADDR (source) => MADDR (destination)
        MemoryToPeripheral, // MEM2MEM=0, DIR=1, MADDR (source) => PADDR (destination)
        MemoryToMemory      // MEM2MEM=1, DIR=x, PADDR (source) => MADDR (destination)
    };

    #pragma endregion

    #pragma region Conversions and address calcuations

    template<typename TplPointerType>
    requires std::is_pointer_v<TplPointerType>
    constexpr auto pointerToPeripheralSizeAlignment() -> Cfgr::PSIZE_RW_PeripheralAlignment {
        return soc::reg::value_to_register_field_enum<alignof(TplPointerType), Cfgr::PSIZE_RW_PeripheralAlignment>();
    }

    template<typename TplPointerType>
    requires std::is_pointer_v<TplPointerType>
    constexpr auto pointerToMemorySizeAlignment() -> Cfgr::MSIZE_RW_MemoryAlignment {
        return soc::reg::value_to_register_field_enum<alignof(TplPointerType), Cfgr::MSIZE_RW_MemoryAlignment>();
    }

    template<Id TplIdEnum>
    class IdHelperCt {
    private:
        static constexpr std::uint32_t raw_value = static_cast<std::uint32_t>(TplIdEnum);

    public:
        static constexpr std::uint32_t truncatedTrigger = raw_value & 0x0ffU;
        static constexpr std::uint32_t dmaInstance      = raw_value & 0x00fU;
        static constexpr std::uint32_t channel          = (raw_value & 0x0f0U) >> 8U;
        static constexpr bool          isHwTrigger      = (raw_value & 0xf00U) == 0U;
        static constexpr bool          isExtended       = (dmaInstance == 2U && channel > 8U);
    };

    template<Id TplIdEnum>
    class AddressCt {
    private:
        static constexpr auto id = IdHelperCt<TplIdEnum>();
        static constexpr std::uint32_t instance_base = const_base_addr + ( id.dmaInstance * const_instance_offset);

        template<std::uint32_t TplRegisterCount>
        static constexpr auto channelRegister() -> std::uint32_t {
            return
                instance_base +
                const_cfgr_offset +
                (id.channel * const_channel_spacing) +
                (TplRegisterCount * const_register_spacing);
        }

    public:
        static constexpr std::uint32_t intfr  = instance_base + (id.isExtended ? const_initfr_extended_offset : 0U); // Status flags (global event, completion, half-transmission, error)
        static constexpr std::uint32_t intfcr = intfr + const_register_spacing; // Clearing status flags (global event, completion, half-transmission, error)
        static constexpr std::uint32_t intfrOffset = id.channel * const_fields_per_channel; // Offsset of the fields is the same for intfcr as well

        static constexpr std::uint32_t cfgr  = channelRegister<0U>(); // Configuration for this channel
        static constexpr std::uint32_t cntr  = channelRegister<1U>(); // Count for this channel (when transfering more than 1)
        static constexpr std::uint32_t paddr = channelRegister<2U>(); // Source address in MemoryToMemory and PeripheralToMemory, destination in MemoryToPeripheral
        static constexpr std::uint32_t maddr = channelRegister<3U>(); // Source address in MemoryToPeripheral, destination MemoryToMemory and PeripheralToMemory
    };

    #pragma endregion

    #pragma region Common checks and asserts

    template<Id TplIdHead, Id... TplIdTail>
    inline constexpr auto
    __attribute__ ((always_inline))
    noDuplicateId() -> void {
        // Check if current HEAD has duplicate in the TAIL section
        static_assert(
            ( (IdHelperCt<TplIdHead>::truncatedTrigger != IdHelperCt<TplIdTail>::truncatedTrigger) && ... ),
            "Duplicate IDs used, there might be overlap between HW and SW triggers, or multiple HW triggers used of the same instance and channel");

        // If there are 2 or more TAIL entries (after truncating HEAD), then
        // make the first TAIL entry as new HEAD and then keep checking for more combinations
        if constexpr (sizeof...(TplIdTail) >= 2U) {
            noDuplicateId<TplIdTail...>();
        }
    }

    template<
        Id        TplRequester,
        std::uint32_t TplMemoryAddress,
        std::uint16_t TplSizeOfTransmission
    >
    constexpr auto isInBoundariesCheckCt() -> void {
        constexpr auto instance = IdHelperCt<TplRequester>::dmaInstance;
        constexpr auto channel  = IdHelperCt<TplRequester>::channel;

        static_assert(
            (instance == 1U && (channel == 2U || channel == 3U || channel == 4U || channel == 5U)) &&
            ((TplMemoryAddress + TplSizeOfTransmission - 1U) <= 131072U), "Breaking 128k boundary");

        static_assert(
            (instance == 1U && (channel == 1U || channel == 6U || channel == 7U)) &&
            ((TplMemoryAddress + TplSizeOfTransmission -1U) <= 65535U), "Breaking 128k boundary");
    };

    template<
        Id        TplRequester,
        std::uint16_t TplSizeOfTransmission>
    [[nodiscard]] constexpr auto isInBoundariesCheck(std::uint32_t memoryAddress) -> bool {
        constexpr auto instance = IdHelperCt<TplRequester>::dmaInstance;
        constexpr auto channel  = IdHelperCt<TplRequester>::channel;

        if constexpr (instance == 1U && (channel == 2U || channel == 3U || channel == 4U || channel == 5U)) {
            return ((memoryAddress + TplSizeOfTransmission - 1U) <= 131072U);
        } else if constexpr (instance == 1U && (channel == 1U || channel == 6U || channel == 7U)) {
            return ((memoryAddress + TplSizeOfTransmission - 1U) <= 131072U);
        } else {
            return true;
        }
    }

    template<
        PeripheralAlignment TplPeripheralAlignment,
        MemoryAlignment     TplMemoryAlignment>
    constexpr auto comparePAddressWithMAddressAlignment() -> std::int32_t {
        constexpr auto p_align = static_cast<std::uint32_t>(TplPeripheralAlignment) >> static_cast<std::uint32_t>(PeripheralAlignment::fieldBitOffset);
        constexpr auto m_align = static_cast<std::uint32_t>(TplMemoryAlignment) >> static_cast<std::uint32_t>(MemoryAlignment::fieldBitOffset);
        if (p_align > m_align) {
            return 1;
        } else if (p_align < m_align) {
            return -1;
        } else {
            // pAlign == mAlign
            return 0;
        }
    }

    #pragma endregion

    #pragma region Init DMA channel

    template<
        Id              TplRequesterId,
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
        constexpr auto isHwTrigger          = IdHelperCt<TplRequesterId>::isHwTrigger;
        constexpr auto sourceIncrement      = soc::reg::bool_to_register_field_enum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = soc::reg::bool_to_register_field_enum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto isCyclic             = soc::reg::bool_to_register_field_enum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = soc::reg::bool_to_register_field_enum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = soc::reg::bool_to_register_field_enum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = soc::reg::bool_to_register_field_enum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = soc::reg::bool_to_register_field_enum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();
        constexpr AddressCt<TplRequesterId> addresses;

        constexpr auto destinationAlignment = pointerToMemorySizeAlignment<TplDestinationPointerType>();

        static_assert(isHwTrigger == true, "PeripheralToMemory Requester ID can't be triggered by SW requester ID, enabling EN field will not start transmission, but wait for HW event requester");

        if constexpr (TplTransferCount == 0U) {
            static_assert(!TplSourceIncrement && !TplDestinationIncrement,
                "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
            );
        }

        soc::reg::access::writeCtAddrVal<addresses.cntr, static_cast<std::uint32_t>(TplTransferCount)>();
        soc::reg::access::writeCtAddrVal<addresses.paddr, TplSourceAddress>();
        soc::reg::access::writeCtAddr<addresses.maddr>(reinterpret_cast<std::uint32_t>(destinationPointer));

        soc::reg::access::writeCtAddrVal<
            addresses.cfgr,
            soc::reg::combine::enums_to_uint32<
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
        constexpr auto isHwTrigger          = IdHelperCt<TplRequesterId>::isHwTrigger;
        constexpr auto sourceIncrement      = soc::reg::bool_to_register_field_enum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = soc::reg::bool_to_register_field_enum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto isCyclic             = soc::reg::bool_to_register_field_enum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = soc::reg::bool_to_register_field_enum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = soc::reg::bool_to_register_field_enum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = soc::reg::bool_to_register_field_enum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = soc::reg::bool_to_register_field_enum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();
        constexpr AddressCt<TplRequesterId> addresses;

        constexpr auto sourceAlignment = pointerToMemorySizeAlignment<TplSourcePointerType>();

        static_assert(isHwTrigger == true, "MemoryToPeripheral Requester ID can't be triggered by SW requester ID, enabing EN field will not start transmission, but wait for HW event requester");

        if constexpr (TplTransferCount == 0U) {
            static_assert(!TplSourceIncrement && !TplDestinationIncrement,
                "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
            );
        }

        soc::reg::access::writeCtAddrVal<addresses.cntr, static_cast<std::uint32_t>(TplTransferCount)>();
        soc::reg::access::writeCtAddrVal<addresses.paddr, TplDestinationAddress>();
        soc::reg::access::writeCtAddr<addresses.maddr>(reinterpret_cast<std::uint32_t>(sourcePointer));

        soc::reg::access::writeCtAddrVal<
            addresses.cfgr,
            soc::reg::combine::enums_to_uint32<
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
        constexpr auto isHwTrigger          = IdHelperCt<TplRequesterId>::isHwTrigger;
        constexpr auto sourceIncrement      = soc::reg::bool_to_register_field_enum<TplSourceIncrement,      Cfgr::PINC_RW_PeripheralAddressIncrementMode>();
        constexpr auto destinationIncrement = soc::reg::bool_to_register_field_enum<TplDestinationIncrement, Cfgr::MINC_RW_MemoryAddressIncrementMode>();
        constexpr auto irqTransmissionError = soc::reg::bool_to_register_field_enum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = soc::reg::bool_to_register_field_enum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = soc::reg::bool_to_register_field_enum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        // When enabling channel in MemoryToMemory mode, it will start transition instantly
        constexpr auto isEnabled            = soc::reg::bool_to_register_field_enum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();
        constexpr AddressCt<TplRequesterId> addresses;

        constexpr auto sourceAlignment      = pointerToPeripheralSizeAlignment<TplSourcePointerType>();
        constexpr auto destinationAlignment = pointerToMemorySizeAlignment<TplDestinationPointerType>();

        static_assert(isHwTrigger == false, "MemomryToMemory mode can't be triggered by HW requester ID, it will be triggered instanly as EN field is enabled");

        if constexpr (TplTransferCount == 0U) {
            static_assert(!TplSourceIncrement && !TplDestinationIncrement,
                "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
            );
        }

        if constexpr (TplTransferCount > 0U) {
            //TODO check the read and write ranges do not overlap
        }

        soc::reg::access::writeCtAddrVal<addresses.cntr, static_cast<std::uint32_t>(TplTransferCount)>();
        soc::reg::access::writeCtAddr<addresses.paddr>(reinterpret_cast<std::uint32_t>(sourcePointer));
        soc::reg::access::writeCtAddr<addresses.maddr>(reinterpret_cast<std::uint32_t>(destinationPointer));

        soc::reg::access::writeCtAddrVal<
            addresses.cfgr,
            soc::reg::combine::enums_to_uint32<
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
        bool                TplAllowTruncatingTransfer,
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
        constexpr auto isHwTrigger          = IdHelperCt<TplRequesterId>::isHwTrigger;
        constexpr auto isMemoryToMemory     = TplDirection == Direction::MemoryToMemory;
        constexpr auto isCyclic             = soc::reg::bool_to_register_field_enum<TplCyclicMode,           Cfgr::CIRC_RW_CyclicMode>();
        constexpr auto irqTransmissionError = soc::reg::bool_to_register_field_enum<TplTransmissionErrorIrq, Cfgr::TEIE_RW_TransmissionErrorInteruptEnable>();
        constexpr auto irqHalfTransmission  = soc::reg::bool_to_register_field_enum<TplHalfTransmissionIrq,  Cfgr::HTIE_RW_HalfTransmissionInteruptEnable>();
        constexpr auto irqFullTransmission  = soc::reg::bool_to_register_field_enum<TplFullTransmissionIrq,  Cfgr::TCIE_RW_TransmissionCompletionInteruptEnable>();
        constexpr auto isEnabled            = soc::reg::bool_to_register_field_enum<TplEnableDma,            Cfgr::EN_RW_ChannelEnable>();
        constexpr AddressCt<TplRequesterId> addresses;

        if constexpr (TplDirection == Direction::MemoryToMemory) {
            static_assert(!isHwTrigger, "MemomryToMemory mode can't be triggered by HW requester ID, it will be triggered instanly as EN field is enabled");
            static_assert(!TplCyclicMode, "MemoryToMemory mode can't be used with Cyclic mode");
        }

        if constexpr (TplTransferCount == 0U) {
            static_assert(!TplPeripheralIncrement  && !TplMemoryIncrement,
                "Can't increment source and/or destination when doing single transfers (TplTransferCount==0)"
            );
        }

        // Allow manually "override" these alignment asserts with TplAllowTruncatingTransfer if the truncatitation is intentional
        if constexpr (!TplAllowTruncatingTransfer) {

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

        soc::reg::access::writeCtAddrVal<addresses.cntr, static_cast<std::uint32_t>(TplTransferCount)>();
        soc::reg::access::writeCtAddr<addresses.paddr>(reinterpret_cast<std::uint32_t>(peripheralPointer));
        soc::reg::access::writeCtAddr<addresses.maddr>(reinterpret_cast<std::uint32_t>(memoryPointer));

        soc::reg::access::writeCtAddrVal<
            addresses.cfgr,
            soc::reg::combine::enums_to_uint32<
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

    #pragma endregion

}
