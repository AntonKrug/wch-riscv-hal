REM go to this directory, for example: `cd C:\Users\Fredy\CLionProjects\wch-riscv-hal\launch\real-hw` and run the openocd and then gdb scripts
REM expecting correct OpenOCD (WCH variant with the wlinke interface support) bin folder being present in path already

openocd -f openocd/wch.cfg