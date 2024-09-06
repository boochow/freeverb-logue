/*
 * File: freeverb.cpp
 *
 */

#include "unit.h"
#include "revmodel.hpp"
#include "tuning.h"
/*
#define k_user_revfx_param_time 0
#define k_user_revfx_param_depth 1
#define k_user_revfx_param_shift_depth 2
#define k_user_revfx_param_width 3
#define k_user_revfx_param_freeze 4
*/

enum {
    k_user_revfx_param_roomsize,
    k_user_revfx_param_damp,
    k_user_revfx_param_width,
    k_user_revfx_param_freeze,
    k_user_revfx_param_wet,
    k_user_revfx_param_dry,
};

static unit_runtime_desc_t s_desc;
static int32_t p_[11];

static revmodel model; // buffers must be re-initialized

__unit_callback int8_t unit_init(const unit_runtime_desc_t * desc) {
    if (!desc)
      return k_unit_err_undef;
    
    // Note: make sure the unit is being loaded to the correct platform/module target
    if (desc->target != unit_header.target)
      return k_unit_err_target;
    
    // Note: check API compatibility with the one this unit was built against
    if (!UNIT_API_IS_COMPAT(desc->api))
      return k_unit_err_api_version;
    
    // Check compatibility of samplerate with unit, for NTS-1 MKII should be 48000
    if (desc->samplerate != 48000)
      return k_unit_err_samplerate;

    // Check compatibility of frame geometry
    if (desc->input_channels != 2 || desc->output_channels != 2)  // should be stereo input/output
      return k_unit_err_geometry;

    // Cache the runtime descriptor for later use
    s_desc = *desc;

    return k_unit_err_none;
}

__unit_callback void unit_render(const float * in, float * out, uint32_t frames)
{
    float * __restrict in_l = (float *) in;
    float * __restrict in_r = (float *) (in + 1);
    float * __restrict my_l = (float *) out;
    float * __restrict my_r = (float *) (out + 1);

    model.processmix(in_l, in_r, my_l, my_r, frames, 2);
}


__unit_callback void unit_set_param_value(uint8_t id, int32_t value)
{
    float valf;
    float drywet;
    switch (id) {
    case k_user_revfx_param_roomsize:
        valf = value * 0.005f;
        model.setroomsize(valf);
        break;
    case k_user_revfx_param_damp:
        valf = value * 0.005f;
        model.setdamp(1.0 - valf);
        break;
    case k_user_revfx_param_width:
        valf = value * 0.005f;
        model.setwidth(valf);
        break;
    case k_user_revfx_param_freeze:
        valf = value * 1.f;
        model.setmode(valf);
        break;
    case k_user_revfx_param_wet:
        valf = value * 0.005f;
        model.setwet(valf);
        break;
    case k_user_revfx_param_dry:
        valf = value * 0.005f;
        model.setdry(valf);
        break;
    default:
        break;
  }
  p_[id] = value;
}

__unit_callback void unit_teardown() {
}

__unit_callback void unit_reset() {
}

__unit_callback void unit_resume() {
    model.mute();
}

__unit_callback void unit_suspend() {
    model.mute();
}

__unit_callback int32_t unit_get_param_value(uint8_t id) {
    return p_[id];
}

__unit_callback const char * unit_get_param_str_value(uint8_t id, int32_t value) {
    return nullptr;
}

__unit_callback void unit_set_tempo(uint32_t tempo) {
}

__unit_callback void unit_tempo_4ppqn_tick(uint32_t counter) {
}
