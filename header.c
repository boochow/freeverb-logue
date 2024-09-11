/*
    BSD 3-Clause License

    Copyright (c) 2023, KORG INC.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of the copyright holder nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//*/

/*
 *  File: header.c
 *
 *  NTS-1 mkII reverb effect unit header definition
 *
 */

#include "unit_revfx.h"   // Note: Include base definitions for revfx units

// ---- Unit header definition  --------------------------------------------------------------------

const __unit_header unit_header_t unit_header = {
    .header_size = sizeof(unit_header_t),
    .target = UNIT_TARGET_PLATFORM | k_unit_module_revfx,
    .api = UNIT_API_VERSION,
    .dev_id = 0x42636877U,  // "Bchw"
    .unit_id = 0x05040000,  // Product number(05),Unit type(04=RevFX),reserved
    .version = 0x00010000U,
    .name = "Freeverb",
    .num_params = 6,
    
    .params = {
        // Format: min, max, center, default, type, frac. bits, frac. mode, <reserved>, name

        // See common/runtime.h for type enum and unit_param_t structure

        // Fixed/direct UI parameters
        // A knob
        {0, 200, 0, 40, k_unit_param_type_none, 1, 0, 0, {"Time"}},

        // B knob
        {0, 200, 0, 60, k_unit_param_type_none, 1, 0, 0, {"Wet"}},

        // REVEB switch + B knob
        {0, 200, 0, 60, k_unit_param_type_none, 1, 0, 0, {"Width"}},

        // 8 Edit menu parameters
        {0, 200, 0, 100, k_unit_param_type_none, 1, 0, 0, {"Damp"}},
        {0, 1, 0, 0, k_unit_param_type_onoff, 0, 0, 0, {"Freeze"}},
        {0, 200, 0, 100, k_unit_param_type_none, 1, 0, 0, {"Dry"}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}},
        {0, 0, 0, 0, k_unit_param_type_none, 0, 0, 0, {""}}},
};
