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

    constexpr std::uint32_t baseAddr = 0x4002'1000;

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
        template<Rcc::Concept::IsCtlrField _>      constexpr auto fromRegFieldType() { return Rcc::Ctlr::regMemOffset; }
        template<Rcc::Concept::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Rcc::Cfgr0::regMemOffset; }
        template<Rcc::Concept::IsIntrField _>      constexpr auto fromRegFieldType() { return Rcc::Intr::regMemOffset; }
        template<Rcc::Concept::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Rcc::Apb2prstr::regMemOffset; }
        template<Rcc::Concept::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Rcc::Apb1prstr::regMemOffset; }
        template<Rcc::Concept::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Rcc::Ahbpcenr::regMemOffset; }
        template<Rcc::Concept::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Rcc::Apb1pcenr::regMemOffset; }
        template<Rcc::Concept::IsRstsckrField _>   constexpr auto fromRegFieldType() { return Rcc::Rstsckr::regMemOffset; }
    }

    namespace RegFieldTuple {
        template<Rcc::Concept::IsCtlrField _>      constexpr auto fromRegFieldType() { return Rcc::Ctlr::regFields; }
        template<Rcc::Concept::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Rcc::Cfgr0::regFields; }
        template<Rcc::Concept::IsIntrField _>      constexpr auto fromRegFieldType() { return Rcc::Intr::regFields; }
        template<Rcc::Concept::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Rcc::Apb2prstr::regFields; }
        template<Rcc::Concept::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Rcc::Apb1prstr::regFields; }
        template<Rcc::Concept::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Rcc::Ahbpcenr::regFields; }
        template<Rcc::Concept::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Rcc::Apb1pcenr::regFields; }
        template<Rcc::Concept::IsRstsckrField _>   constexpr auto fromRegFieldType() { return Rcc::Rstsckr::regFields; }
    }

}
