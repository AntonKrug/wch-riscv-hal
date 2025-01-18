//
// Created by anton on 26/12/2024.
//

#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

namespace Peripheral::Gpio{


    #pragma region Enums


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

    using BaseAddress = std::uint32_t;


    // Define a concept that restricts the parameter strictly to BaseAddress
    // and accidental replacements with different addresses (like GPIO peripheral)
    // or literal addresses are not possible
    template<typename T>
    concept IsBaseAddress = std::is_same_v<T, BaseAddress>;


    struct SequenceEntity {
        const BaseAddress portBaseAddress;
        const std::array<std::uint8_t, 8> pinNumbers;
        const ActuationType action;
        const int value;
    };


    struct Pins {
        const BaseAddress portBaseAddress;
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


    template<BaseAddress TplBaseAddress>
    struct Port {
    private:
        template<BaseAddress TplRegisterBase>
        struct RegistersType {
            constexpr static std::uint32_t configuration     = TplRegisterBase;         // CFGLR, sometimes CFGHR
            constexpr static std::uint32_t inputData         = TplRegisterBase + 0x08;  // INDR
            constexpr static std::uint32_t outputData        = TplRegisterBase + 0x0C;  // OUTDR
            constexpr static std::uint32_t setReset          = TplRegisterBase + 0x10;  // BSHR
            constexpr static std::uint32_t reset             = TplRegisterBase + 0x14;  // BCR
            constexpr static std::uint32_t configurationLock = TplRegisterBase + 0x18;  // LCKR
        };

    public:
        constexpr static BaseAddress baseAddress = TplBaseAddress;

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


    template<BaseAddress TplBaseAddress>
    constexpr Port<TplBaseAddress>::operator std::uint32_t() const { // NOLINT(*-explicit-constructor)
        return baseAddress;
    }


    template<BaseAddress TplBaseAddress>
    constexpr auto Port<TplBaseAddress>::GetPin(const std::uint8_t pin) -> Pins {
        return Pins{baseAddress, {pin}};
    }


    template<BaseAddress TplBaseAddress>
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


}



// template<>
// struct ChangesCt<> {
//     template<Pins Pins>
//     [[nodiscard]] constexpr auto on() const {
//         constexpr ChangeAction newAction(Pins.portBaseAddress, Pins.pinNumbers, ChangeActionType::set, 1);
//         constexpr ChangesCt<newAction> ans;
//         return ans;
//     }
// };


/*
*    struct ChangeAction {
// Can't contain Pins struct without forward declaraction somewhere at some point,
// that will require to use pointer somewhere which then will end up being a template argument
// and we can't make constexpr value into a pointer one and even if we could we could end up
// with multiple template instances for a same pin?
const int portBaseAddress;
const std::array<int, 8> pinNumbers;
const ChangeActionType action;
const int value;
};


// template<ChangeAction ... Val>
// struct ChangesCt;

// template<ChangeAction ... Val>
// struct ChangesCt;


template<int BaseAddress>
    struct Base {
        //  using value_type = T;
        // using type = Registers<BaseAddress>;
        struct Registers {
            constexpr static int baseAddress       = BaseAddress;         // CFGLR
            constexpr static int inputData         = BaseAddress + 0x08;  // INDR
            constexpr static int outputData        = BaseAddress + 0x0C;  // OUTDR
            constexpr static int setReset          = BaseAddress + 0x10;  // BSHR
            constexpr static int reset             = BaseAddress + 0x14;  // BCR
            constexpr static int configurationLock = BaseAddress + 0x18;  // LCKR
        };

        // ReSharper disable once CppNonExplicitConversionOperator
        constexpr operator int() const; // NOLINT(*-explicit-constructor)

        constexpr static auto GetPin(int pin) -> Pins ;

        constexpr static auto GetPins(const std::array<int, 8> &pins) -> Pins ;
    };

*/