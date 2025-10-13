/*
 *  File: header.c
 *
 *  NTS-1 mkII reverb effect unit header definition
 *
 */

#include "unit.h"  // Note: Include common definitions for all units
#include "runtime.h"

// ---- Unit header definition  --------------------------------------------------------------------

const __unit_header unit_header_t unit_header = {
    .header_size = sizeof(unit_header_t),
    .target = UNIT_TARGET_PLATFORM | k_unit_module_revfx,
    .api = UNIT_API_VERSION,
    .dev_id = 0x42636877U,  // "Bchw"
    .unit_id = 0x05040000,  // Product number(05),Unit type(04=RevFX),reserved
    .version = 0x00010000U,
    .name = "Freeverb",
    .num_presets = 0,
    .num_params = 6,
    
    .params = {
        // Format: min, max, center, default, type, frac. bits, frac. mode, <reserved>, name

        // See common/runtime.h for type enum and unit_param_t structure

        // Page 1
        {0, 200, 0, 40, k_unit_param_type_none, 1, 0, 0, {"Time"}},
        {0, 200, 0, 60, k_unit_param_type_none, 1, 0, 0, {"Wet"}},
        {0, 200, 0, 60, k_unit_param_type_none, 1, 0, 0, {"Width"}},

        // Page 2
        {0, 200, 0, 100, k_unit_param_type_none, 1, 0, 0, {"Damp"}},
        {0, 1, 0, 0, k_unit_param_type_onoff, 0, 0, 0, {"Freeze"}},
        {0, 200, 0, 100, k_unit_param_type_none, 1, 0, 0, {"Dry"}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}}},
};
