	.section  .init // https://developer.arm.com/documentation/101754/0623/armclang-Reference/armclang-Integrated-Assembler/Section-directives
	.global  entry_jump_to_init
    .align  1 // 2-byte
    .extern handle_reset  // Declare the C function, so we can jump into it later
    .option norelax
entry_jump_to_init:
// vector table base address needs to be 1KB aligned in the QingKe V2 microprocessor.
// when the table is set to 0, then it will overlap with the first jump instruction
// however the first entry in the table is not implemented so they can overlap without
// harm
    j       handle_reset
    .option relax
