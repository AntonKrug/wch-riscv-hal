//
// Created by anton on 26/12/2024.
//

#pragma once

#include <array>
#include <cstdint>
#include "system/memory_map/concepts.h"

namespace Peripheral::Gpio{


    #pragma region Enums


    // Use as safety trick to keep addresses types of different peripherals
    // from mixing and enforce type sctrictness with this address type,
    // alternative could be:
    // https://github.com/dbj-systems/nothingbut
    enum class GpioBaseAddress : std::uint32_t;


    enum class ActuationType: std::uint8_t {
        execusionPrecission,
        set,
        modeChange
    };


    enum class PinMode: std::uint8_t {
        inputAnalog                           = 0b00'00,
        inputFloating                         = 0b01'00,
        inputPullUpOrDown                     = 0b10'00, // up/down is set with OUTDR
        outputPushPullNormalSpeed             = 0b00'01, // 10Mhz
        outputOpenDrainNormalSpeed            = 0b01'01, // 10Mhz
        alternateFunctionPushPullNormalSpeed  = 0b10'01, // 10Mhz
        alternateFunctionOpenDrainNormalSpeed = 0b11'01, // 10Mhz
        outputPushPullSlowSpeed               = 0b00'10, // 5Mhz
        outputOpenDrainSlowSpeed              = 0b01'10, // 5Mhz
        alternateFunctionPushPullSlowSpeed    = 0b10'10, // 5Mhz
        alternateFunctionOpenDrainSlowSpeed   = 0b11'10, // 5Mhz
        outputPushPullFastSpeed               = 0b00'11, // 50Mhz for all, expect 30Mhz for CH32V003
        outputOpenDrainFastSpeed              = 0b01'11, // 50Mhz for all, expect 30Mhz for CH32V003
        alternateFunctionPushPullFastSpeed    = 0b10'11, // 50Mhz for all, expect 30Mhz for CH32V003
        alternateFunctionOpenDrainFastSpeed   = 0b11'11, // 50Mhz for all, expect 30Mhz for CH32V003
    };


    #pragma endregion


    #pragma region Declarations


    struct SequenceEntity {
        const GpioBaseAddress portBaseAddress;
        const std::array<std::uint8_t, 8> pinNumbers;
        const ActuationType action;
        const int value;
    };


    struct Pins {
        const GpioBaseAddress portBaseAddress;
        const std::array<std::uint8_t, 8> pinNumbers;

        auto operator=(int value) const -> const Pins &; // NOLINT(*-unconventional-assign-operator)

        auto SetOutputValue(int value) const -> void;

        [[nodiscard]] constexpr auto sequence() const;

        template<PinMode TplPinMode>
        [[nodiscard]] constexpr auto mode() const;
    };


    template <Pins... TplPinsEntities>
    struct PinsSet {
        static constexpr std::array<SequenceEntity, sizeof...(TplPinsEntities)> entities = { TplPinsEntities ... };

        auto operator=(int value) const -> const PinsSet &; // NOLINT(*-unconventional-assign-operator)
    };


    template<GpioBaseAddress TplBaseAddress>
    struct Port {
    private:
        template<GpioBaseAddress TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t RegisterBaseUint32 = static_cast<std::uint32_t>(TplRegisterBase);
            constexpr static std::uint32_t configuration      = RegisterBaseUint32;         // CFGLR, sometimes CFGHR
            constexpr static std::uint32_t inputData          = RegisterBaseUint32 + 0x08;  // INDR
            constexpr static std::uint32_t outputData         = RegisterBaseUint32 + 0x0C;  // OUTDR
            constexpr static std::uint32_t setReset           = RegisterBaseUint32 + 0x10;  // BSHR
            constexpr static std::uint32_t reset              = RegisterBaseUint32 + 0x14;  // BCR
            constexpr static std::uint32_t configurationLock  = RegisterBaseUint32 + 0x18;  // LCKR
        };

    public:
        constexpr static GpioBaseAddress baseAddress       = TplBaseAddress;
        constexpr static std::uint32_t   baseAddressUint32 = static_cast<std::uint32_t>(TplBaseAddress);

        struct RegistersType<TplBaseAddress> Registers = {};

        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator std::uint32_t() const; // NOLINT(*-explicit-constructor)

        constexpr static auto GetPin(std::uint8_t pin) -> Pins;

        constexpr static auto GetPin(const std::array<std::uint8_t, 8> &pins) -> Pins;
    };


    template<SequenceEntity ... TplEntities>
    struct Sequence {
        static constexpr std::array<SequenceEntity, sizeof...(TplEntities)> actuations = { TplEntities ... };

        template<Pins TplPinsToSetOn>
        [[nodiscard]] constexpr auto on() const;

        template<Pins TplPinsToSetOff>
        [[nodiscard]] constexpr auto off() const;

        template<Pins TplPins, PinMode TplMode>
        [[nodiscard]] constexpr auto mode() const;

        [[nodiscard]] constexpr int executeExact() const;  // TODO no discard is tempory while returning

//        void executeFast() const;
    };


    #pragma endregion


    #pragma region Definition - Port


    template<GpioBaseAddress TplBaseAddress>
    constexpr Port<TplBaseAddress>::operator std::uint32_t() const { // NOLINT(*-explicit-constructor)
        return static_cast<uint32_t>(baseAddress);
    }


    template<GpioBaseAddress TplBaseAddress>
    constexpr auto Port<TplBaseAddress>::GetPin(const std::uint8_t pin) -> Pins {
        return Pins{baseAddress, {pin}};
    }


    template<GpioBaseAddress TplBaseAddress>
    constexpr auto Port<TplBaseAddress>::GetPin(const std::array<std::uint8_t, 8> &pins) -> Pins {
        return Pins{baseAddress, pins};
    }


    #pragma endregion


    #pragma region Definition - Sequence


    template<SequenceEntity... TplEntities>
    template<Pins TplPins>
    constexpr auto Sequence<TplEntities...>::on() const {
        constexpr SequenceEntity newEntity{TplPins.portBaseAddress, TplPins.pinNumbers, ActuationType::set, 1};
        constexpr Sequence<newEntity, TplEntities ...> ans;
        return ans;
    }


    template<SequenceEntity... TplEntities>
    template<Pins TplPins> // TODO support variadic
    constexpr auto Sequence<TplEntities...>::off() const {
        constexpr SequenceEntity newEntity{TplPins.portBaseAddress, TplPins.pinNumbers, ActuationType::set, 0};
        constexpr Sequence<newEntity, TplEntities ...> ans;
        return ans;
    }


    template<SequenceEntity... TplEntities>
    template<Pins TplPins, PinMode TplPinMode>
    constexpr auto Sequence<TplEntities...>::mode() const {
        constexpr SequenceEntity newEntity{
            TplPins.portBaseAddress,
            TplPins.pinNumbers,
            ActuationType::modeChange,
            static_cast<uint8_t>(TplPinMode)};

        constexpr Sequence<newEntity, TplEntities ...> ans;
        return ans;
    }


    template<SequenceEntity... TplEntities>
    constexpr int Sequence<TplEntities...>::executeExact() const {
        int sum = 0;
        for (auto action : actuations) {
            sum += action.pinNumbers[0];
        }

        return sum;
    }


    #pragma endregion


    #pragma region Definition - Pins


    inline auto Pins::operator=(const int value) const -> const Pins & { // NOLINT(*-unconventional-assign-operator)
        auto *ptr = reinterpret_cast<std::uint8_t *>(portBaseAddress);
        *ptr = value;
        return *this;
    }


    inline auto Pins::SetOutputValue(const int value) const -> void {
        auto *ptr = reinterpret_cast<std::uint8_t *>(portBaseAddress);
        *ptr = value;
    }


    // ReSharper disable once CppMemberFunctionMayBeStatic
    constexpr auto Pins::sequence() const { // NOLINT(*-convert-member-functions-to-static)
        constexpr Sequence<> ans;
        return ans;
    }


    template<PinMode TplPinMode>
    constexpr auto Pins::mode() const {
        constexpr SequenceEntity newEntity{
            portBaseAddress,
            pinNumbers,
            ActuationType::modeChange,
            static_cast<std::uint8_t>(TplPinMode)};

        constexpr Sequence<newEntity> ans;
        return ans;
    }


    #pragma endregion


    #pragma region Definition - PinsPack


    template<Pins... TplEntities>
    auto PinsSet<TplEntities...>::operator=(int) const -> const PinsSet &  {
        return *this;
    }


    #pragma endregion


    #pragma region Defintion - other


    template<long long int address>
    requires ValidPeripheralBaseAddress<address>
    constexpr static auto MakeBaseAddress() -> GpioBaseAddress {
        return static_cast<GpioBaseAddress>(address);
    };


    #pragma endregion


}
