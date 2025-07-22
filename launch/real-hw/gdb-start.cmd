REM can't use the `set disassembler-options xw` with upstream xpack toolchain, but can have TUI mode
REM and can't access the CSR intsyscr as it's just 0x804

REM xpack toolchain
REM riscv-none-elf-gdb.exe -tui -x gdb/gdbinit ../../cmake-build-debug/src/wch-riscv-hal-pass-1.elf
riscv-none-elf-gdb-py3.exe -tui -x gdb/gdbinit ../../cmake-build-debug/src/wch-riscv-hal-pass-1.elf

REM WCH toolchain
REM riscv-wch-elf-gdb-py3.exe -x gdb/gdbinit ../../cmake-build-debug/src/wch-riscv-hal-pass-1.elf

REM riscv-none-elf-gdb.exe -x gdb/hw_script ../../cmake-build-debug/src/wch-riscv-hal-pass-1.hex