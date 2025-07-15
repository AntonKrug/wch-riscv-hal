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

namespace peripheral::rcc {

    constexpr std::uint32_t base_addr = 0x4002'1000U;

    namespace concepts {
        template<typename RegField> concept IsCtlrField      = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Ctlr::reg_fields)>();
        template<typename RegField> concept IsCfgr0Field     = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Cfgr0::reg_fields)>();
        template<typename RegField> concept IsIntrField      = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Intr::reg_fields)>();
        template<typename RegField> concept IsApb2prstrField = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Apb2prstr::reg_fields)>();
        template<typename RegField> concept IsApb1prstrField = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Apb1prstr::reg_fields)>();
        template<typename RegField> concept IsAhbpcenrField  = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Ahbpcenr::reg_fields)>();
        template<typename RegField> concept IsApb2pcenrField = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Apb2pcenr::reg_fields)>();
        template<typename RegField> concept IsApb1pcenrField = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Apb1pcenr::reg_fields)>();
        template<typename RegField> concept IsRstsckrField   = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Rstsckr::reg_fields)>();

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

    namespace reg_mem_offset {
        template<concepts::IsCtlrField _>      constexpr auto fromRegFieldType() { return Ctlr::reg_mem_offset; }
        template<concepts::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Cfgr0::reg_mem_offset; }
        template<concepts::IsIntrField _>      constexpr auto fromRegFieldType() { return Intr::reg_mem_offset; }
        template<concepts::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Apb2prstr::reg_mem_offset; }
        template<concepts::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Apb1prstr::reg_mem_offset; }
        template<concepts::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Ahbpcenr::reg_mem_offset; }
        template<concepts::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Apb1pcenr::reg_mem_offset; }
        template<concepts::IsRstsckrField _>   constexpr auto fromRegFieldType() { return Rstsckr::reg_mem_offset; }
    }

    namespace reg_field_tuple {
        template<concepts::IsCtlrField _>      constexpr auto fromRegFieldType() { return Ctlr::reg_fields; }
        template<concepts::IsCfgr0Field _>     constexpr auto fromRegFieldType() { return Cfgr0::reg_fields; }
        template<concepts::IsIntrField _>      constexpr auto fromRegFieldType() { return Intr::reg_fields; }
        template<concepts::IsApb2prstrField _> constexpr auto fromRegFieldType() { return Apb2prstr::reg_fields; }
        template<concepts::IsApb1prstrField _> constexpr auto fromRegFieldType() { return Apb1prstr::reg_fields; }
        template<concepts::IsAhbpcenrField _>  constexpr auto fromRegFieldType() { return Ahbpcenr::reg_fields; }
        template<concepts::IsApb1pcenrField _> constexpr auto fromRegFieldType() { return Apb1pcenr::reg_fields; }
        template<concepts::IsRstsckrField _>   constexpr auto fromRegFieldType() { return rcc::Rstsckr::reg_fields; }
    }

}
