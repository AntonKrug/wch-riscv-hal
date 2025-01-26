//
// Created by anton on 13/01/2025.
//

#pragma once

/* keep them as defines, even when they could be changed to consexpr, because
 * this file will be used from linker script when doing linker-pre-processing
 * hack to support many platforms with one script*/

// will allow to reach -2k before this point and +2k above this point
#define GP_LINKER_OFFSET_SYMETRIC 0

// half of the 12-bit GP's signed range, able to reach only positive values
#define GP_LINKER_OFFSET_UNIDIRECTIONAL_UP (1<<11)

// When GP is RAM and we moved small RO data to RAM too, but might not be
// reachable due lots of small RW data taking all space, offset GP in such
// way that small RO data will get priority
#define GP_LINKER_OFFSET_INCLUDE_SMALL_RO_DATA_IN_RAM ((__small_ro_data_in_RAM_end - __global_pointer_without_offset) - 1<<12)