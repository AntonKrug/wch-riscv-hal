/* Chip: CH32Vxxx */
/* References:
    https://mcyoung.xyz/2021/06/01/linker-script/
    https://mcuoneclipse.com/2013/04/14/text-data-and-bss-code-and-data-size-explained/
    https://www.sifive.com/blog/all-aboard-part-3-linker-relaxation-in-riscv-toolchain
    https://uw.cs.cs350.narkive.com/2CEqIPGX/bss-and-sbss
    https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-elf.adoc
    https://blog.thea.codes/the-most-thoroughly-commented-linker-script/
    https://gcc-help.gcc.gnu.narkive.com/QHfS11NY/gnu-linkonce-and-gcc-except-table#
    https://docs.oracle.com/cd/E19683-01/816-7777/6mdorm6is/index.html
    https://github.com/picolibc/picolibc/blob/main/doc/init.md
    https://docs.oracle.com/cd/E19455-01/816-0559/chapter2-48195/index.html
    https://maskray.me/blog/2021-11-07-init-ctors-init-array
    https://groups.google.com/g/llvm-dev/c/h_G8vRnqRPM/m/xXhzF1_eBQAJ
    https://en.cppreference.com/w/cpp/language/siof
    https://five-embeddev.com/quickref/global_pointer.html
    https://hackmd.io/@hauhsu/rJ89ghF6n?utm_source=preview-mode&utm_medium=rec
    https://stackoverflow.com/questions/19781375/how-to-place-constant-at-specific-address-with-ld-linker-command-file
    https://stackoverflow.com/questions/38239722/how-to-place-a-symbol-at-certain-address-via-the-linker-script
    https://users.informatik.haw-hamburg.de/~krabat/FH-Labor/gnupro/5_GNUPro_Utilities/c_Using_LD/ldLinker_scripts.html#MEMORY_command
    https://man7.org/linux/man-pages/man5/elf.5.html
    https://stackoverflow.com/questions/6387614/how-to-determine-maximum-stack-usage-in-embedded-system-with-gcc
    https://www.eevblog.com/forum/programming/gccs-risc-v-msmall-data-limit-option/
    http://freertoshal.github.io/doxygen/group__LINKER.html
    https://stackoverflow.com/questions/28837199/can-i-use-preprocessor-directives-in-ld-file
    ${cross_prefix}${cross_c}${cross_suffix} -E -P -x c ../Ld/WCH32Vxxx-universal.ld.tpl -o ../Ld/Link.ld
    https://stackoverflow.com/questions/70863659/what-does-hidden-and-provide-hidden-in-linker-scripts-exactly-do
    https://www.reddit.com/r/RISCV/comments/1bcdq5h/software_threads_in_riscv/
    https://www.eevblog.com/forum/microcontrollers/ch32v307-risc-v-minicore-with-ethernet/125/
    https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Function-Attributes.html
*/

/*TODO: allow GP to be placed to periheral range as well */

/* DO NOT DIRECTLY EDIT THIS FILE, but edit the included header file,
   it contains comments how to tweak this linker script too */

/* This for the c pre-processor, open/edit user_src/platform_configuration/system.h file to see details */
#include "user_src/platform_configuration/system.h"


OUTPUT_ARCH("riscv")


ENTRY(entry_jump_to_init)


MEMORY
{
  ROM      (rx)   : ORIGIN = 0x00000000,               LENGTH = SYSTEM_WCH_CHIP_ROM_SIZE
#ifdef SYSTEM_WCH_CHIP_EEPROM_SIZE
  ROM_SLOW (rx)   : ORIGIN = SYSTEM_WCH_CHIP_ROM_SIZE, LENGTH = SYSTEM_WCH_CHIP_EEPROM_SIZE - SYSTEM_WCH_CHIP_ROM_SIZE
#endif
  RAM      (rwx)  : ORIGIN = 0x20000000,               LENGTH = SYSTEM_WCH_CHIP_RAM_SIZE
}


SECTIONS
{


  /* first power-up/reset code including vector table */
  .init : ALIGN(2) /* align from now on to 2-byte blocks */
  {
    *.init
  } > ROM


  /* vector table base address needs to be 1KB aligned in the QingKe V2 microprocessor.
      but we offset it by 4 bytes, as the first entry in table is not implemented, and
      allows us overlap it entry location 0x0 and have embedded jump inside first entry
      to later stage of initialization */
  .init.irq_vector_table_plus_one : ALIGN(4)
  {
    KEEP (*(.init.irq_vector_table_plus_one))
  } > ROM = (SYSTEM_WCH_VECTOR_TABLE_ADDRESS + 0x4) /* if table is 0x0 but forcing the offset PLUS ONE vector by 4 bytes */


  /* most (if not all) of the application code, and other constants*/
  .text : ALIGN(4)
  {
    PROVIDE(__main_user = .);
    *(.text.main_user) /* making user's code main function as close to the 0 as possible
                        * when placed just after vector table we can calculate its position
                        * at compile time more easily and allow easier jump to this from
                        * hhandle_reset, as it's doing preparation for `mret` "jump" */

    *(.text.handle_reset) /* part of the HAL's init, setting up clock, gp, intializing
                           * (with zeros and values) variables. Make sure this section
                           * is at begining of the section so it the first entry instruction
                           * jump can be done with a short relative jump and will not
                           * require many instructions (which could affect vector table
                           * as they might be overlapping and we have only 4 bytes spare
                           * in the table to fit the the jump instruction) */
    *(.text*)             /* "gcc -ffunction-sections" */
#ifdef SYSTEM_WCH_LINKER_GP_INSIDE_ROM
    . = ALIGN(SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT);          /* see system.h for details */
    PROVIDE( __global_pointer_without_offset = .);
    /*https://five-embeddev.com/quickref/global_pointer.html*/
    /* if i do not use this exact symbol name for GP, it looks like the linker will not pick it up
       for the relaxation optimizations */
    PROVIDE( __global_pointer$ = . + SYSTEM_WCH_LINKER_GP_OFFSET); /* see system.h for details */
#endif /* SYSTEM_WCH_LINKER_GP_INSIDE_ROM */
#ifndef SYSTEM_WCH_LINKER_SMALL_RO_DATA_IN_RAM
    /* small read-only data placed in ROM together with other ROM content */
    PROVIDE( __small_ro_data_in_ROM_begin = .);
    . = ALIGN(4)
    *(.srodata.cst4)  /*  4-byte constants 32-bit */
    . = ALIGN(2)
    *(.srodata.cst2)  /*  2-byte constants (similar to cst16) */
    . = ALIGN(2)
    *(.srodata.cst16) /* 16-bit  constants 2 byte */
    . = ALIGN(1)
    *(.srodata.cst8)  /*  8-bit  constants 1 byte */
    . = ALIGN(4)
    *(.srodata*)      /* eveyrthing else */
    PROVIDE( __small_ro_data_in_ROM_end = .);
#endif /* end SYSTEM_WCH_LINKER_SMALL_RO_DATA_IN_RAM */
    . = ALIGN(SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT); /* see system.h for details in case rodata contains 64-bit variables */
    *(.rodata*)            /* read only data (constant global/static variables) */
    *(.gnu.linkonce.t.*)   /* to merge duplicate C++ template instances and vague linking of weakly defined symbols */
    *(.gnu.linkonce.r.*)   /* to merge duplicate readonly C++ static constructors and destructors, or read only static data/variabled ***** maybe allow it to be placed to text */
#ifndef SYSTEM_WCH_CHIP_EEPROM_SIZE
    /* This is fallback for all the SoCs who do not expose the non-zero-wait-flash
       and make the code more portable between SoC. The code targeting ROM_SLOW can
       be just fine executed from regular "faster ROM" */
    *(.rom_slow*)
#endif
  } > ROM

#ifdef SYSTEM_WCH_CHIP_EEPROM_SIZE
/* When SoC contains non-zero-wait-flash aka 480k-R0WAIT or 224k-R0WAIT eeprom,
   and user explicitly adds it as attribute before function, then place that code
   into the slow non-zero-wait section:
   __attribute__ ((section (".rom_slow")))
   void doSomething()
   {
   }
 */
  .rom_slow : ALIGN(4)
  {
    *(.rom_slow*)
  } > ROM_SLOW
#endif

  /* function pointers to be run before init, before C runtime startup - vtable validation, global initialization */
  /* https://maskray.me/blog/2021-11-07-init-ctors-init-array ********************************************************************** */
  .preinit_array :
  {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > ROM


  /* function pointers to be run after C runtime startup, but before main - constructors  */
  /* init for dynamic initialization of non-local (static) of variables in classes */
  /* and function which have __attribute__((constructor)) or __attribute__((constructor(PRIORITY_NUMBER_101_to_65535))) while 0-100 will throw -Wprio-ctor-dtor, gcov uses 100 */
  /*
  Unlike .ctors, the execution order of .init_array is forward (follows .init). a.o:(.init_array) b.o:(.init_array) has a different order from a.o:(.ctors) b.o:(.ctors). In a future section we will discuss that this difference can expose a type of very subtle bugs called "static initialization order fiasco".
  In GCC, newer ABI implementations like AArch64 and RISC-V only use .init_array and don't provide .ctors.
  */
  .init_array :
  {
    PROVIDE_HIDDEN(__init_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))   /* C++ static constructors */
    KEEP(*(.init_array))
    PROVIDE_HIDDEN(__init_array_end = .);
  } > ROM


  /* cleanup, finalization and destructor function pointers */
  /* or functions which have __attribute__((destructor)) or __attribute__((destructor(101_to_65535))) while 0-100 will throw -Wprio-ctor-dtor, gcov uses 100  */
  .fini_array :
  {
    PROVIDE_HIDDEN(__fini_array_start = .);
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*))) /* C++ static destructors */
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN(__fini_array_end = .);
  } > ROM


  .data_runtime_location : ALIGN(4)
  {
    PROVIDE(__data_ram_start = .); /* for startup code - where to write into when ROM -> RAM values initialization */
  } >RAM AT>ROM   /* Place the this section in RAM but load it from ROM */
                  /* syntax is as following:                         */
                  /* > PLACE_IT         AT> LOAD_IT_FROM             */
                  /* aka:                                            */
                  /* > RUNTIME_LOCATION AT> DEVICE_POWER_UP_LOCATION */


  .data_boot_location : ALIGN(4)
  {
    PROVIDE(__data_rom_start = .); /* for startup code - where to copy from when doing ROM -> RAM values initialization */
  } >ROM


  /* These global/static variables will require in startup to be initialized with values loaded from flash */
  .data : ALIGN(4)
  {
    *(.data*)            /* main section contains many if not all variables */
    *(.gnu.linkonce.d.*) /* shared link-only-once data to merge references to writable data, static variables which are defined in multiple objects and need to be merged into one  */
#ifndef SYSTEM_WCH_LINKER_GP_INSIDE_ROM
    . = ALIGN(SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT);          /* see system.h for details */
    PROVIDE( __global_pointer_without_offset = .);
    /*https://five-embeddev.com/quickref/global_pointer.html*/
    /* if i do not use this exact symbol name for GP, it looks like the linker will not pick it up
       for the relaxation optimizations */
    PROVIDE( __global_pointer$ = . + SYSTEM_WCH_LINKER_GP_OFFSET); /* see system.h for details */
#endif /* SYSTEM_WCH_LINKER_GP_INSIDE_ROM */

    PROVIDE( __small_rw_data_begin = .);
    *(.sdata*)
    PROVIDE( __small_rw_data_end = .);
/*    *(.gnu.linkonce.s.*) */ /* to merge special section - platform specific data, metadata, debuging section, runtime configuration data */
#ifdef SYSTEM_WCH_LINKER_SMALL_RO_DATA_IN_RAM
    /* small read-only data wasting RAM just to be close to GP*/
    PROVIDE( __small_ro_data_in_RAM_begin = .);
    *(.srodata.cst4)  /*  4-byte constants 32-bit */
    *(.srodata.cst2)  /*  2-byte constants (similar to cst16) */
    *(.srodata.cst16) /* 16-bit  constants 2 byte */
    *(.srodata.cst8)  /*  8-bit  constants 1 byte */
    *(.srodata*)      /* eveyrthing else */
    PROVIDE( __small_ro_data_in_RAM_end = .);
#endif /* end SYSTEM_WCH_LINKER_SMALL_RO_DATA_IN_RAM */

    . = ALIGN(4);
    PROVIDE( __data_ram_end = .); /* For startup code - where to stop when copying values when doing ROM -> RAM values initialization */
  } >RAM AT>ROM                   /* Loaded from ROM and placed to RAM */

  PROVIDE(__data_size    = __data_ram_end - __data_ram_start);
  PROVIDE(__data_rom_end = __data_rom_start + __data_size);


  /* uninitialized data - global/static variables which just need to be zeroized, do not need to be loaded */
  .bss : ALIGN(4)
  {
    PROVIDE(__block_started_by_symbol = .);     /* For startup code - where to start when zeroing values */
    *(.sbss*)                                   /* Small unitizlized (to be zeroized) variables */
    *(.gnu.linkonce.sb.*)                       /* To merge duplicate static unintialized (to be zeroized) variables, */
                                                /* special case of *.gnu.linkonce.b. needing aditional handling */
    *(.bss*)                                    /* the main block of unitialized (to be zeroized) static/global variables */
    *(.gnu.linkonce.b.*)                        /* to merge duplicate static or global unintialized (to be zeroized) variables */
    *(COMMON*)                                  /* common extern declared, but not defined symbols */
    . = ALIGN(4);                               /* align the stop below to whole multiples of 4 so the for loop can work on word size all the time */
    PROVIDE(__block_started_by_symbol_end = .); /* for startup code - where to stop zeroing */
  } >RAM AT>ROM


  /* for `malloc()`, `calloc()`, or `realloc()`, `new` etc... */
  .heap : ALIGN(SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT) /* see system.h for details */
  {
    PROVIDE(__heap_start = .);
    . += SYSTEM_WCH_LINKER_HEAP_SIZE;
    __heap_end = .;
    . = ALIGN(SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT); /* see system.h for details */
    PROVIDE(__heap_end = __heap_end);
  } >RAM


  /* Callstack, local variables, saving register when calling IRQ, return addresses when returning from methods, */
  /* temporary values etc... */
  .stack ORIGIN(RAM) + LENGTH(RAM) - SYSTEM_WCH_LINKER_STACK_SIZE : ALIGN(4)
  {
    PROVIDE(__stack_start = . );
    . = . + SYSTEM_WCH_LINKER_STACK_SIZE;
    PROVIDE(__stack_end = .);
  } >RAM


}



