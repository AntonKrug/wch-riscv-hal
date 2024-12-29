//
// Created by anton on 26/12/2024.
//

#ifndef GPIO_H
#define GPIO_H

#include <array>
#include <cstdint>

namespace BundledPeripheral::Gpio {


    #pragma region Enums


    enum class ActuationType: std::uint8_t {
        execusionPrecission,
        set,
        modeChange
    };


    enum class PinMode: std::uint8_t {
        directionInput       = 0b00,
        directionOutput10Mhz = 0b01,
        directionOutput2Mhz  = 0b10,
        directionOutput50Mhz = 0b11
    };


    enum class PinConfig: std::uint8_t {
        inputAnalog                = 0b00,
        inputDigitalFloating       = 0b01,
        inputDigitalPullDownOrUp   = 0b10, // up/down is set with OUTDR
        outputPushPull             = 0b00,
        outputOpenDrain            = 0b01,
        outputMultiplexedPushPull  = 0b10,
        outputMultiplexedOpenDrain = 0b11,
    };


    #pragma endregion

    #pragma region Declarations

    struct ActuationEntity {
        const int portBaseAddress;
        const std::array<int, 8> pinNumbers;
        const ActuationType action;
        const int value;
    };


    struct Pins {
        const int portBaseAddress;
        const std::array<int, 8> pinNumbers;

        auto operator=(int value) const -> const Pins &; // NOLINT(*-unconventional-assign-operator)

        auto SetOutputValue(int value) const -> void;

        [[nodiscard]] constexpr auto actuate() const;

        template<PinConfig pinConfig>
        [[nodiscard]] constexpr auto mode() const;
    };


    template <Pins... Entities>
    struct PinsPack {
        static constexpr std::array<ActuationEntity, sizeof...(Entities)> entities = { Entities ... };

        auto operator=(int value) const -> const PinsPack &; // NOLINT(*-unconventional-assign-operator)
    };


    template<int BaseAddress>
    struct Base {
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


    template<ActuationEntity ... Entities>
    struct Actuations {
        static constexpr std::array<ActuationEntity, sizeof...(Entities)> actuations = { Entities ... };

        template<Pins Pins>
        [[nodiscard]] constexpr auto on() const;

        template<Pins Pins>
        [[nodiscard]] constexpr auto off() const;

        template<Pins Pins, PinConfig mode>
        [[nodiscard]] constexpr auto mode() const;

        [[nodiscard]] constexpr int commitExact() const;
    };

    #pragma endregion


    #pragma region Definition - Base


    template<int BaseAddress>
    constexpr Base<BaseAddress>::operator int() const { // NOLINT(*-explicit-constructor)
        return Registers::baseAddress;
    }


    template<int BaseAddress>
    constexpr auto Base<BaseAddress>::GetPin(const int pin) -> Pins {
        return Pins{Registers::baseAddress, {pin}};
    }


    template<int BaseAddress>
    constexpr auto Base<BaseAddress>::GetPins(const std::array<int, 8> &pins) -> Pins {
        return Pins{Registers::baseAddress, pins};
    }


    #pragma endregion


    #pragma region Definition - Actuations


    template<ActuationEntity... Entities>
    template<Pins Pins>
    constexpr auto Actuations<Entities...>::on() const {
        constexpr ActuationEntity newEntity{Pins.portBaseAddress, Pins.pinNumbers, ActuationType::set, 1};
        constexpr Actuations<newEntity, Entities ...> ans;
        return ans;
    }


    template<ActuationEntity... Entities>
    template<Pins Pins>
    constexpr auto Actuations<Entities...>::off() const {
        constexpr ActuationEntity newEntity{Pins.portBaseAddress, Pins.pinNumbers, ActuationType::set, 0};
        constexpr Actuations<newEntity, Entities ...> ans;
        return ans;
    }

    template<ActuationEntity... Entities>
    template<Pins Pins, PinConfig mode>
    constexpr auto Actuations<Entities...>::mode() const {
        constexpr ActuationEntity newEntity{
            Pins.portBaseAddress,
            Pins.pinNumbers,
            ActuationType::modeChange,
            static_cast<uint8_t>(mode)};

        constexpr Actuations<newEntity, Entities ...> ans;
        return ans;
    }


    template<ActuationEntity... Entities>
    constexpr int Actuations<Entities...>::commitExact() const {
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
    constexpr auto Pins::actuate() const { // NOLINT(*-convert-member-functions-to-static)
        constexpr Actuations<> ans;
        return ans;
    }


    template<PinConfig pinConfig>
    constexpr auto Pins::mode() const {
        constexpr ActuationEntity newEntity{
            portBaseAddress,
            pinNumbers,
            ActuationType::modeChange,
            static_cast<std::uint8_t>(pinConfig)};

        constexpr Actuations<newEntity> ans;
        return ans;
    }


    #pragma endregion


    #pragma region Definition - PinsPack


    template<Pins... Entities>
    auto PinsPack<Entities...>::operator=(int) const -> const PinsPack &  {
        return *this;
    }


    #pragma endregion


}


#endif //GPIO_H

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