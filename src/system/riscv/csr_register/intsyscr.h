//
// Created by anton on 28/01/2025.
//

// https://www.wch-ic.com/downloads/QingKeV2_Processor_Manual_PDF.html
// https://www.reddit.com/r/RISCV/comments/126262j/notes_on_wch_fast_interrupts/
// https://www.eevblog.com/forum/microcontrollers/ch32v003-fast-interrupt-(pfic)-features/
// Note: This is bespoke WCH CSR and is not described in the standard

#pragma once

#include <cstdint>
#include <type_traits>

#include "system/register/util.h"
#include "system/register/field_access_privilege.h"

namespace Riscv::Csr::Intsyscr {

    enum class Hwstken_MRW_HardwarePrologueEpilogue: std::uint32_t {
        // Hardware prologue and epilogue on IRQs, on low-end devices (QingKeV2 like CH32V003)
        // it saves footprint, but doesn't improve performance (or can make it worse) and might
        // waste more stack than necesary. On higher-end device it has shadow registers without
        // wasting stack and improves performance as well. Nesting is possible upto 2 levels
        // deep. One problem is that this is non-standard and requires WCH toolchain and changing
        //  __attribute__((interrupt()))
        //  on all handlers to:
        //  __attribute__((interrupt("WCH-Interrupt-fast")))
        // Another problem on QingKeV2 is that application will get smaller footprint as the
        // prologue and epilogue doesn't have to be done by software at a cost of saving all
        // registers by hardware, the hardware will still have to take time to transfer 10
        // registers one by one into the stack (in this order: x1(ra), x5(t0), x6(t1), x7(t2),
        // x10(a0), x11(a1), x12(a2), x13(a3), x14(a4), x15(a5)) and waste 48bytes on a stack
        // regardless if these needed to be saved or not. Note: NOTE: The stack gets allocated
        // 48bytes to save 10 registers (yet 10x4=40). Compared to using software implemented
        // PE, the prologue and epilogue might be significantly smaller and faster if the
        // handlers are minimalistic (which should be the aim anyway for IRQ handlers) and
        // do not use many registers (therefore they do not need to save/restore) as many as
        // the HPE feature does.
        // There are only limited amount of IRQs and the benefits (or drawbacks) of HPE are
        // only applied to the handlers of these IRQs. On some applications might
        // worth investigating how the footprint and runtime behavior changes between using
        // HPE or not. Because blindly enabling the HPE might negatively impact the application.
        fieldBitMask = 0b1u, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disable      = 0,            // disable HW prologue and epilogue (see intsyscr.h for more details)
        enable       = fieldBitMask, // enable HW prologoue and epiloge (see intsyscr.h for more details)
    };

    enum class Inesten_MRW_InteruptNesting: std::uint32_t {
        // Enable nesting of intreupts together with PFIC settings the IRQs can get different
        // priorities and dictating order of execution.
        fieldBitMask = 0b1u << 1, // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disable      = 0,
        enable       = fieldBitMask,
    };

    enum class Eabien_MRW_EmbeddedAbi: std::uint32_t {
        fieldBitMask = 0b1u << 2,     // not holding any settings or value, it's a bitmask for this specific field
        fieldAccess  = Soc::Reg::FieldAccessRight::ReadWrite,

        disble       = 0,            // Keeping EABI disabled, the way WCH recomends
        enable       = fieldBitMask, // WCH noted that this shouldn't be enabled, and left in the default disabled state
    };

    template<typename CsrField>
    concept IsAnyField =
        std::is_same_v<CsrField, Hwstken_MRW_HardwarePrologueEpilogue> ||
        std::is_same_v<CsrField, Inesten_MRW_InteruptNesting> ||
        std::is_same_v<CsrField, Eabien_MRW_EmbeddedAbi>;


}