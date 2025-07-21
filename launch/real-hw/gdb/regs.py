import gdb

class Eregs(gdb.Command):
    def __init__(self):
        super(RV32ERegs, self).__init__("regs_grouped", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        for i in range(16):
            reg = gdb.parse_and_eval(f"${i}")
            print(f"x{i:2} = {reg}")

Eregs()