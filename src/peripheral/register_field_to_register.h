//
// Created by anton on 04/02/2025.
//

#pragma once

#include "concept.h"
#include "ch32v00x/rcc.h"
#include "ch32v00x/rcc/ctlr.h"
#include "ch32v00x/rcc/cfgr0.h"
#include "ch32v00x/rcc/intr.h"
#include "system/register/concept.h"

// TODO: own namespace, combine the CSR actions (registers are basic and CSR register on top)
//       able to provide isntance instead of baseAddress
//       constrains on high level functions/public ones, not the lower access level ones


// RCC

//TODO: make sure they are the same type
// match head to each single tail, and with empty Tail pack it will become true by default
// TODO: they need to match same peripheral base type
// template <typename HeadType, typename... TailTypes>
// concept HeadSameAsTail = sizeof...(TailTypes) == 0 || (std::is_same_v<HeadType, TailTypes> && ...);

namespace PeripheralBaseAddr {

    template<Peripheral::Rcc::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return Peripheral::Rcc::baseAddr;
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}

namespace RegMemOffset {

    template<Peripheral::Rcc::Ctlr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Ctrlr);
    }

    template<Peripheral::Rcc::Cfgr0::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Cfgr0);
    }

    template<Peripheral::Rcc::Intr::IsAnyRegField RegFieldType>
    constexpr auto fromRegFieldType() -> std::uint32_t {
        return static_cast<std::uint32_t>(Peripheral::Rcc::RegOffset::Intr);
    }

    template<auto RegField>
    constexpr auto fromRegField() -> std::uint32_t {
        return fromRegFieldType<decltype(RegField)>();
    }

}



// ReSharper disable once CppUnnamedNamespaceInHeaderFile
// namespace {
//     template<auto RegField>
//     requires Soc::MemMappedReg::Concept::RegisterFieldEnumWhichContainsFieldBitMask<decltype(RegField)>
//     constexpr auto regFieldMask() -> std::uint32_t {
//         return static_cast<std::uint32_t>(decltype(RegField)::fieldBitMask);
//     }
// }


#pragma region Primitives

namespace Soc::Reg {

    template<std::uint32_t Addr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    readCt() -> std::uint32_t {
        return *reinterpret_cast<volatile std::uint32_t *>(Addr);
    }

    template<std::uint32_t Addr, std::uint32_t CtValue>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt() -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(Addr))=CtValue;
    }

    template<std::uint32_t Addr>
    inline auto
    __attribute__ ((
        always_inline,
        optimize("-Os"),
    ))
    writeCt(const std::uint32_t rtValue) -> void {
        *(reinterpret_cast<volatile std::uint32_t *>(Addr))=rtValue;
    }

}

#pragma endregion

#pragma region isReg

template<std::uint32_t BaseAddr, auto TestedRegFieldHead, auto... TestedRegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldsSetMipCt() -> bool {
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
    constexpr auto combinedMask  = Soc::Reg::combineFieldMasksToUint32<TestedRegFieldHead, TestedRegFieldTails...>();
    constexpr auto regOffset     = RegMemOffset::fromRegField<TestedRegFieldHead>();
    const     auto actualValue   = Soc::Reg::readCt<BaseAddr + regOffset>();

    return (actualValue & combinedMask) == combinedValue;
}

template<auto TestedRegFieldHead, auto... TestedRegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<TestedRegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
isRegFieldsSetSipCt() -> bool {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<TestedRegFieldHead>();
    return isRegFieldsSetMipCt<baseAddr, TestedRegFieldHead, TestedRegFieldTails...>();
}

#pragma endregion

#pragma region writeReg

template<std::uint32_t BaseAddr, auto RegFieldHead, auto... RegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldsMipCt() -> void {
    constexpr auto regOffset     = RegMemOffset::fromRegField<RegFieldHead>();
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<RegFieldHead, RegFieldTails...>();
    Soc::Reg::writeCt<BaseAddr + regOffset, combinedValue>();
}

template<auto RegFieldHead, auto... RegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
inline constexpr
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
auto writeRegFieldsSipCt() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<RegFieldHead>();
    return writeRegFieldsMipCt<baseAddr, RegFieldHead, RegFieldTails...>();
}

#pragma endregion

#pragma region setReg

template<std::uint32_t baseAddress, auto RegFieldHead, auto... RegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldsMipCt() -> void {
    constexpr auto regOffset     = RegMemOffset::fromRegField<RegFieldHead>();
    constexpr auto combinedValue = Soc::Reg::combineEnumsToUint32<RegFieldHead, RegFieldTails...>();
    constexpr auto combinedMask  = Soc::Reg::combineFieldMasksToUint32<RegFieldHead, RegFieldTails...>();

    // TODO: detect when full 0xffffffff mask and replace the clear with write,
    //       also detect the cases where all writable fields are already masked and
    //       act as the full clear and do write instead

    static_assert(
        combinedValue != 0u || combinedMask != 0u,
        "Both value-to-be-set and their mask are empty, this indicates badly described register field");

    if constexpr (combinedMask == 0xffffffffu) {
        Soc::Reg::writeCt<baseAddress + regOffset>(combinedValue);
    } else {
        auto actualValue = Soc::Reg::readCt<baseAddress + regOffset>();

        // if clearing and setting is the same, then clearing can be omited
        if constexpr (combinedMask != combinedValue) {
            actualValue &= 0xffffffffu ^ combinedMask;   // to really clear we need to invert the mask
        }

        // if nothing to set then ommit setting
        if constexpr (combinedValue > 0) {
            actualValue |= combinedValue;
        }
        Soc::Reg::writeCt<baseAddress + regOffset>(actualValue);
    }
}

template<auto RegFieldHead, auto... RegFieldTails>
requires
    Soc::Reg::Concept::FieldWithBitMask<RegFieldHead> &&
    (Soc::Reg::Concept::FieldWithBitMask<RegFieldTails> && ...)
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
setRegFieldsSipCt() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegField<RegFieldHead>();
    return setRegFieldsMipCt<baseAddr, RegFieldHead, RegFieldTails...>();
}

#pragma endregion

#pragma region ClearReg

template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldTypesMipCt() -> void {
    constexpr auto regOffset    = RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
    constexpr auto combinedMask = Soc::Reg::combineFieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
    const     auto actualValue  = Soc::Reg::readCt<baseAddress + regOffset>();

    Soc::Reg::writeCt<baseAddress + regOffset>(actualValue & combinedMask);
}


template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
clearRegFieldTypesSipCt() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegFieldType<RegFieldTypeHead>();
    return clearRegFieldTypesMipCt<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
}

#pragma endregion

#pragma region KeepReg

template<std::uint32_t baseAddress, typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldTypesMipCt() -> void {
    constexpr auto regOffset    = RegMemOffset::fromRegFieldType<RegFieldTypeHead>();
    constexpr auto combinedMask = Soc::Reg::combineFieldTypeMasksToUint32<RegFieldTypeHead, RegFieldTypeTails...>();
    const     auto actualValue  = Soc::Reg::readCt<baseAddress + regOffset>();

    Soc::Reg::writeCt<baseAddress + regOffset>( actualValue & (0xffffffff ^ combinedMask) );
}


template<typename RegFieldTypeHead, typename... RegFieldTypeTails>
inline auto
__attribute__ ((
    always_inline,
    optimize("-Os"),
))
keepRegFieldTypesSipCt() -> void {
    constexpr auto baseAddr = PeripheralBaseAddr::fromRegFieldType<RegFieldTypeHead>();
    return keepRegFieldTypesMipCt<baseAddr, RegFieldTypeHead, RegFieldTypeTails...>();
}

#pragma endregion