//
// Created by anton on 04/01/2025.
//

#pragma once

#include <cstdint>

#include "field/rcc/ctlr.h"
#include "field/rcc/cfgr0.h"
#include "field/rcc/intr.h"
#include "field/rcc/apb2prstr.h"
#include "field/rcc/apb1prstr.h"
#include "field/rcc/ahbpcenr.h"
#include "field/rcc/apb1pcenr.h"
#include "field/rcc/apb2pcenr.h"
#include "field/rcc/rstsckr.h"

namespace Peripheral::Rcc {

    constexpr std::uint32_t baseAddr = 0x4002'1000U;

    namespace Concept {
        template<typename RegField> concept IsCtlrField      = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Ctlr::regFields)>();
        template<typename RegField> concept IsCfgr0Field     = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Cfgr0::regFields)>();
        template<typename RegField> concept IsIntrField      = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Intr::regFields)>();
        template<typename RegField> concept IsApb2prstrField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Apb2prstr::regFields)>();
        template<typename RegField> concept IsApb1prstrField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Apb1prstr::regFields)>();
        template<typename RegField> concept IsAhbpcenrField  = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Ahbpcenr::regFields)>();
        template<typename RegField> concept IsApb2pcenrField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Apb2pcenr::regFields)>();
        template<typename RegField> concept IsApb1pcenrField = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Apb1pcenr::regFields)>();
        template<typename RegField> concept IsRstsckrField   = Soc::Reg::IsSameAsOneFieldFromTuple<RegField, decltype(Rstsckr::regFields)>();

        template<typename Reg>
        concept IsAnyRegField =
            IsCtlrField<Reg> ||
            IsCfgr0Field<Reg> ||
            IsIntrField<Reg> ||
            IsApb2prstrField<Reg> ||
            IsApb1prstrField<Reg> ||
            IsAhbpcenrField<Reg> ||
            IsApb2pcenrField<Reg> ||
            IsApb1pcenrField<Reg> ||
            IsRstsckrField<Reg>;

    }

    namespace RegMemOffset {
        template<Concept::IsCtlrField _>      constexpr auto fromRegFieldType() { return Ctlr::regMemOffset; }
        template<Concept::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Cfgr0::regMemOffset; }
        template<Concept::IsIntrField _>      constexpr auto fromRegFieldType() { return Intr::regMemOffset; }
        template<Concept::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Apb2prstr::regMemOffset; }
        template<Concept::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Apb1prstr::regMemOffset; }
        template<Concept::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Ahbpcenr::regMemOffset; }
        template<Concept::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Apb1pcenr::regMemOffset; }
        template<Concept::IsRstsckrField _>   constexpr auto fromRegFieldType() { return Rstsckr::regMemOffset; }
    }

    namespace RegFieldTuple {
        template<Concept::IsCtlrField _>      constexpr auto fromRegFieldType() { return Ctlr::regFields; }
        template<Concept::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Cfgr0::regFields; }
        template<Concept::IsIntrField _>      constexpr auto fromRegFieldType() { return Intr::regFields; }
        template<Concept::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Apb2prstr::regFields; }
        template<Concept::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Apb1prstr::regFields; }
        template<Concept::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Ahbpcenr::regFields; }
        template<Concept::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Apb1pcenr::regFields; }
        template<Concept::IsRstsckrField _>   constexpr auto fromRegFieldType() { return Rcc::Rstsckr::regFields; }
    }

}
