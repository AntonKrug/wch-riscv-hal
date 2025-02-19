//
// Created by anton on 18/02/2025.
//

#pragma once

#include <cstdint>

#include "field/afio/pcfr1.h"

namespace Peripheral::Afio {

    constexpr std::uint32_t baseAddr = 0x4001'0000;

    namespace Concept {
        template<typename RegField> concept IsPcfr1Field = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Pcfr1::regFields)>();

        template<typename Reg>
        concept IsAnyRegField =
            IsPcfr1Field<Reg>;

    }

    namespace RegMemOffset {
        template<Concept::IsPcfr1Field _> constexpr auto fromRegFieldType() { return Pcfr1::regMemOffset; }
    }

    namespace RegFieldTuple {
        template<Concept::IsPcfr1Field _> constexpr auto fromRegFieldType() { return Pcfr1::regFields; }
    }

}
