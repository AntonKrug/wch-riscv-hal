    .section  .init // https://developer.arm.com/documentation/101754/0623/armclang-Reference/armclang-Integrated-Assembler/Section-directives
    .global  entry_jump_to_init
    .align  1 // 2-byte
    .extern handle_reset  // Declare the C function, so we can jump into it later
    .option norelax
entry_jump_to_init:
// Vector table base address needs to be 1KB aligned in the QingKe V2 microprocessor.
// When the table is set to 0, then it will overlap with the first jump instruction
// however the first entry in the table (which holds true for QingKe V3 and V4 as well)
// is not implemented so they can overlap without harm.
// The 4byte opcode Jump instruction uses `imm20` which can be shifted by 1 bit (2-byte
// aligned) to 21-bits and can jump ~1MiB in each direction. That's enough to cover
// whole FLASH of any (currently) available WCH chips (including the CH32H417 with
// 960KB FLASH), means that handle_reset doesn't need to be right after the IRQ table
// and we can put other code close by instead (if there is benefit for doing so).
// Also do not use compressed 2byte opcodes, many of them depend on various things
// to be present and setup already on the CPU while here at this stage the CPU is
// in unknown state.
    j       handle_reset
    .option relax
