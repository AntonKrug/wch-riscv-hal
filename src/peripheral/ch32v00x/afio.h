//
// Created by anton on 18/02/2025.
//

#pragma once

#include <cstdint>

#include "field/afio/pcfr1.h"
#include "system/register/util.h"

namespace peripheral::afio {

    constexpr std::uint32_t base_addr = 0x4001'0000U;

    namespace concepts {
        template<typename RegField> concept IsPcfr1Field = soc::reg::is_same_as_one_field_from_tuple<RegField, decltype(Pcfr1::reg_fields)>();

        template<typename Reg>
        concept IsAnyRegField =
            IsPcfr1Field<Reg>;

    }

    namespace reg_mem_offset {
        template<concepts::IsPcfr1Field _> constexpr auto fromRegFieldType() { return Pcfr1::reg_mem_offset; }
    }

    namespace reg_field_tuple {
        template<concepts::IsPcfr1Field _> constexpr auto fromRegFieldType() { return Pcfr1::reg_fields; }
    }

}
