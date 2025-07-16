//
// Created by anton on 08/02/2025.
//

#pragma once

// This file (and its path) is required by linker script and cmake, pick up your SOC
// here and you can include this file in your main.cpp file and others etc..
// This way your files will depend on the same system as linker scrip and will be consistently
// up to date. This allows single project serve multiple different SoCs without having
// need for many different (but very similar) linker scripts.

// The base where the all the vector pointers are offset by 4bytes in linkerscript as it allows at
// address 0 to overlap with the entry jump instructions (and the first vector table entry
// is not implemented for this purpose). Therefore in this HAL the base address is shifted by
// 4 bytes with the linker script for the table, but the table has the first entry unimplemented.
// This way we can have this "overlap" at the begining of the table while not freaking out
// the linkerscript and still allow all the options to position the table at different locations
// while at the same time having the vector table implemented nicely in C++ instead of
// handcrafted and hardcoded assembly.
// TLDR; We omit the first unused 4 bytes from the vector table away, so then linker script
//       base address can add back these 4bytes into offset. This in effect gives us biggest
//       freedom while still being safe. But best results might be by leaving it at 0 as any
//       other values might induce extra processing and latency to calculate the address
#define SYSTEM_WCH_VECTOR_TABLE_ADDRESS 0x0

/* Larger devices with 256k ROM can have their ROM/RAM sizes tweaked, see SRAM_CODE_MODE in
 * their datasheet. For other devices this setting is ignored. Note: this needs to be defined
 * before including the system's soc header, as it's ROM/RAM size settings depend on this define */
#define SYSTEM_WCH_SRAM_CODE_MODE "10x"

/* RAM_SIZE > STACK_SIZE + HEAP_SIZE:
 * https://stackoverflow.com/questions/6387614/how-to-determine-maximum-stack-usage-in-embedded-system-with-gcc */
#define SYSTEM_WCH_LINKER_STACK_SIZE 256

/* preferably avoid heap is you can */
#define SYSTEM_WCH_LINKER_HEAP_SIZE 0

/* when using any 64-byte types, double, long long, uint64_t then switch to 8,
 * otherwise leave value at 4, can be used for ALIGNMENT_FOR_GP and other
 * alignement, the GP itself doesn't need to be aligned 8 bytes, but variables
 * which GP helps to point to might require 8byte alignemnt*/
#define SYSTEM_WCH_LINKER_LARGEST_TYPE_ALIGNMENT 4

/* When set to `GP_LINKER_OFFSET_SYMETRIC` then whatever point used for this calculation is the midpoint
 * and GP optimized code will be able to reach 2k of data before this location.
 * When set to `GP_LINKER_OFFSET_UNIDIRECTIONAL` will offset the 12-bit GP that it will mark just the
 * begining of GP's range. And GP optimized code will be able to reach 4k of data
 * after this location. Also feel free to use any other useful values outside these defines */
#define SYSTEM_WCH_LINKER_GP_OFFSET GP_LINKER_OFFSET_SYMETRIC

/* Depending on GP's location, the small read-only (RO) data might lose GP's
 * relaxation optimization benefits if put into ROM and GP placed in RAM. One
 * option is to move GP into the ROM section, but *all* RAM accesses (not just
 * small) will lose benefits of GP. Another option is to leave GP in RAM and waste
 * RAM on small RO content which could be in ROM, but by moving this small RO
 * section into RAM it can get GP's relaxation optimizations. Also if the small R/W
 * data is bigger than GP's 4k range then there is no point adding them to the
 * RAM as the GP will not reach it anyway. Unless manually user will set the
 * liker GP offset to hit the end of of small R/W data, but cover small R/O data. */
#define SYSTEM_WCH_LINKER_SMALL_RO_DATA_IN_RAM

/* If defined it will place the GP register into the ROM, while when not defined
 * the GP will be placed into RAM. Depending on the nature of the application,
 * one can benefit more from ROM GP relaxation compared to RAM, also consider
 * tweaking the GP_OFFSET and the SMALL_RO_DATA_IN_RAM at the same time.
 * However, predicting which will be the the best option for your application
 * might be non trivial, therefore some experimentation and trial&error is
 * expected. However if in doubt, leaving these settings in defaults should
 * be good enough for most applications. */
//#define SYSTEM_WCH_LINKER_GP_INSIDE_ROM

//TODO: ***** do proper description and choose name
#define SYSTEM_WCH_IRQ_VECTORIZED
