/*
 * File: freeverb.cpp
 *
 */

#include "unit_revfx.h"
#include "utils/buffer_ops.h"
#include "revmodel.hpp"
#include "tuning.h"

#define k_user_revfx_param_time 0
#define k_user_revfx_param_depth 1
#define k_user_revfx_param_shift_depth 2

static unit_runtime_desc_t s_desc;
static int32_t p_[11];

static revmodel model; // buffers must be re-initialized

// buffers on SDRAM
static float *bufCL = nullptr;
static float *bufCR = nullptr;
static float *bufAL = nullptr;
static float *bufAR = nullptr;

inline float *sdram_alloc_f32(size_t bufsize) {
    float *m = (float *)s_desc.hooks.sdram_alloc(bufsize * sizeof(float) + 32);
    if (m) {
        buf_clr_f32(m, bufsize);
    }
    return m;
}

inline void sdram_free(void *buf) {
    s_desc.hooks.sdram_free((uint8_t *)buf);
}

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

    if (!desc->hooks.sdram_alloc)
        return k_unit_err_memory;

    // allocate multiple buffers as a single buffer
    // because calling sdram_alloc for more than 16 times will fail
    float *bufcombL[numcombs];
    float *bufcombR[numcombs];
    float *bufallpassL[numallpasses];
    float *bufallpassR[numallpasses];
    size_t pos;
    bufCL = sdram_alloc_f32(combtuningL1 + combtuningL2 +
                            combtuningL3 + combtuningL4 +
                            combtuningL5 + combtuningL6 +
                            combtuningL7 + combtuningL8);
    if (bufCL) {
        pos = 0;
        bufcombL[0] = &bufCL[pos];
        pos += combtuningL1;
        bufcombL[1] = &bufCL[pos];
        pos += combtuningL2;
        bufcombL[2] = &bufCL[pos];
        pos += combtuningL3;
        bufcombL[3] = &bufCL[pos];
        pos += combtuningL4;
        bufcombL[4] = &bufCL[pos];
        pos += combtuningL5;
        bufcombL[5] = &bufCL[pos];
        pos += combtuningL6;
        bufcombL[6] = &bufCL[pos];
        pos += combtuningL7;
        bufcombL[7] = &bufCL[pos];
    } else {
        return k_unit_err_memory;
    }
    bufCR = sdram_alloc_f32(combtuningR1 + combtuningR2 +
                            combtuningR3 + combtuningR4 +
                            combtuningR5 + combtuningR6 +
                            combtuningR7 + combtuningR8);
    if (bufCR) {
        pos = 0;
        bufcombR[0] = &bufCR[pos];
        pos += combtuningR1;
        bufcombR[1] = &bufCR[pos];
        pos += combtuningR2;
        bufcombR[2] = &bufCR[pos];
        pos += combtuningR3;
        bufcombR[3] = &bufCR[pos];
        pos += combtuningR4;
        bufcombR[4] = &bufCR[pos];
        pos += combtuningR5;
        bufcombR[5] = &bufCR[pos];
        pos += combtuningR6;
        bufcombR[6] = &bufCR[pos];
        pos += combtuningR7;
        bufcombR[7] = &bufCR[pos];
    } else {
        sdram_free(bufCL);
        return k_unit_err_memory;
    }
    bufAL = sdram_alloc_f32(allpasstuningL1 + allpasstuningL2 +
                            allpasstuningL3 + allpasstuningL4);
    if (bufAL) {
        pos = 0;
        bufallpassL[0] = &bufAL[pos];
        pos += allpasstuningL1;
        bufallpassL[1] = &bufAL[pos];
        pos += allpasstuningL2;
        bufallpassL[2] = &bufAL[pos];
        pos += allpasstuningL3;
        bufallpassL[3] = &bufAL[pos];
    } else {
        sdram_free(bufCL);
        sdram_free(bufCR);
        return k_unit_err_memory;
    }
    bufAR = sdram_alloc_f32(allpasstuningR1 + allpasstuningR2 +
                            allpasstuningR3 + allpasstuningR4);
    if (bufAR) {
        pos = 0;
        bufallpassR[0] = &bufAR[pos];
        pos += allpasstuningR1;
        bufallpassR[1] = &bufAR[pos];
        pos += allpasstuningR2;
        bufallpassR[2] = &bufAR[pos];
        pos += allpasstuningR3;
        bufallpassR[3] = &bufAR[pos];
    } else {
        sdram_free(bufCL);
        sdram_free(bufCR);
        sdram_free(bufAL);
        return k_unit_err_memory;
    }
    model.setbuffers(bufcombL, bufcombR, bufallpassL, bufallpassR);

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
    const float valf = param_10bit_to_f32(value);
    float drywet;
    switch (id) {
    case k_user_revfx_param_time:
        model.setroomsize(valf);
        break;
    case k_user_revfx_param_depth:
        model.setdamp(1.0 - valf);
        break;
    case k_user_revfx_param_shift_depth:
        drywet = (value + 1000) * 0.0005f;
        model.setwet(drywet / scalewet);
        model.setdry((1.0f - drywet) / scaledry);
        break;
    default:
        break;
  }
  p_[id] = value;
}

__unit_callback void unit_teardown() {
    sdram_free(bufCL);
    sdram_free(bufCR);
    sdram_free(bufAL);
    sdram_free(bufAR);
}

__unit_callback void unit_reset() {
}

__unit_callback void unit_resume() {
}

__unit_callback void unit_suspend() {
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
