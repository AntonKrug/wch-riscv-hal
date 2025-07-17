//
// Created by anton on 12/01/2025.
//

#pragma once


#include "system_defines.h"

// Setup some defines before including SoC
// NOTE: keep this unused include as the linker script will use it later
//       also other system SoC code can refence these to pick user's
//       choices, like what speed the CPU will be running
#include "system_soc.h"
#include "system/linker_utils.h"
