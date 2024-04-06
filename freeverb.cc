/*
 * File: freeverb.cpp
 *
 */

//#include "unit_revfx.h"
#include "unit_delfx.h"
#include "utils/buffer_ops.h"
#include "revmodel.hpp"
#include "buffers.h"

#define k_user_revfx_param_time 0
#define k_user_revfx_param_depth 1
#define k_user_revfx_param_shift_depth 2

static unit_runtime_desc_t s_desc;
static int32_t p_[11];
static int32_t err = 0;
static int32_t err2 = 0;
static int32_t err3 = 0;
static int32_t err4 = 0;

static revmodel model; // buffers must be re-initialized

inline float *sdram_alloc_f32(size_t bufsize) {
    float *m = (float *)s_desc.hooks.sdram_alloc(bufsize * sizeof(float) + 32);
    if (m) {
        buf_clr_f32(m, bufsize);
    }
    return m;
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
    float *buf;
    size_t pos;
    buf = sdram_alloc_f32(combtuningL1 + combtuningL2 +
                          combtuningL3 + combtuningL4 +
                          combtuningL5 + combtuningL6 +
                          combtuningL7 + combtuningL8);
    if (buf) {
        pos = 0;
        bufcombL1 = &buf[pos];
        pos += combtuningL1;
        bufcombL2 = &buf[pos];
        pos += combtuningL2;
        bufcombL3 = &buf[pos];
        pos += combtuningL3;
        bufcombL4 = &buf[pos];
        pos += combtuningL4;
        bufcombL5 = &buf[pos];
        pos += combtuningL5;
        bufcombL6 = &buf[pos];
        pos += combtuningL6;
        bufcombL7 = &buf[pos];
        pos += combtuningL7;
        bufcombL8 = &buf[pos];
        err++;
    }
    buf = sdram_alloc_f32(combtuningR1 + combtuningR2 +
                          combtuningR3 + combtuningR4 +
                          combtuningR5 + combtuningR6 +
                          combtuningR7 + combtuningR8);
    if (buf) {
        pos = 0;
        bufcombR1 = &buf[pos];
        pos += combtuningR1;
        bufcombR2 = &buf[pos];
        pos += combtuningR2;
        bufcombR3 = &buf[pos];
        pos += combtuningR3;
        bufcombR4 = &buf[pos];
        pos += combtuningR4;
        bufcombR5 = &buf[pos];
        pos += combtuningR5;
        bufcombR6 = &buf[pos];
        pos += combtuningR6;
        bufcombR7 = &buf[pos];
        pos += combtuningR7;
        bufcombR8 = &buf[pos];
        err++;
    }
    err2 = (uint32_t) bufcombR8;
    buf = sdram_alloc_f32(allpasstuningL1 + allpasstuningL2 +
                          allpasstuningL3 + allpasstuningL4);
    if (buf) {
        pos = 0;
        bufallpassL1 = &buf[pos];
        pos += allpasstuningL1;
        bufallpassL2 = &buf[pos];
        pos += allpasstuningL2;
        bufallpassL3 = &buf[pos];
        pos += allpasstuningL3;
        bufallpassL4 = &buf[pos];
        err++;
    }
    buf = sdram_alloc_f32(allpasstuningR1 + allpasstuningR2 +
                          allpasstuningR3 + allpasstuningR4);
    if (buf) {
        pos = 0;
        bufallpassR1 = &buf[pos];
        pos += allpasstuningR1;
        bufallpassR2 = &buf[pos];
        pos += allpasstuningR2;
        bufallpassR3 = &buf[pos];
        pos += allpasstuningR3;
        bufallpassR4 = &buf[pos];
        err++;
    }
//    model.setup();
	// Tie the components to their buffers
	model.combL[0].setbuffer(bufcombL1,combtuningL1);
	model.combR[0].setbuffer(bufcombR1,combtuningR1);
	model.combL[1].setbuffer(bufcombL2,combtuningL2);
	model.combR[1].setbuffer(bufcombR2,combtuningR2);
	model.combL[2].setbuffer(bufcombL3,combtuningL3);
	model.combR[2].setbuffer(bufcombR3,combtuningR3);
	model.combL[3].setbuffer(bufcombL4,combtuningL4);
	model.combR[3].setbuffer(bufcombR4,combtuningR4);
	model.combL[4].setbuffer(bufcombL5,combtuningL5);
	model.combR[4].setbuffer(bufcombR5,combtuningR5);
	model.combL[5].setbuffer(bufcombL6,combtuningL6);
	model.combR[5].setbuffer(bufcombR6,combtuningR6);
	model.combL[6].setbuffer(bufcombL7,combtuningL7);
	model.combR[6].setbuffer(bufcombR7,combtuningR7);
	model.combL[7].setbuffer(bufcombL8,combtuningL8);
	model.combR[7].setbuffer(bufcombR8,combtuningR8);
	model.allpassL[0].setbuffer(bufallpassL1,allpasstuningL1);
	model.allpassR[0].setbuffer(bufallpassR1,allpasstuningR1);
	model.allpassL[1].setbuffer(bufallpassL2,allpasstuningL2);
	model.allpassR[1].setbuffer(bufallpassR2,allpasstuningR2);
	model.allpassL[2].setbuffer(bufallpassL3,allpasstuningL3);
	model.allpassR[2].setbuffer(bufallpassR3,allpasstuningR3);
	model.allpassL[3].setbuffer(bufallpassL4,allpasstuningL4);
	model.allpassR[3].setbuffer(bufallpassR4,allpasstuningR4);

	// Set default values
	model.allpassL[0].setfeedback(0.5f);
	model.allpassR[0].setfeedback(0.5f);
	model.allpassL[1].setfeedback(0.5f);
	model.allpassR[1].setfeedback(0.5f);
	model.allpassL[2].setfeedback(0.5f);
	model.allpassR[2].setfeedback(0.5f);
	model.allpassL[3].setfeedback(0.5f);
	model.allpassR[3].setfeedback(0.5f);
	model.setwet(initialwet);
	model.setroomsize(initialroom);
	model.setdry(initialdry);
	model.setdamp(initialdamp);
	model.setwidth(initialwidth);
	model.setmode(initialmode);

	// Buffer will be full of rubbish - so we MUST mute them
	model.mute();

    err4 = (uint32_t) model.combR[7].buffer;
    err++;
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
    switch (id) {
    case k_user_revfx_param_time:
        model.setroomsize(valf);
        break;
    case k_user_revfx_param_depth:
        model.setdamp(1.0 - valf);
        break;
    case k_user_revfx_param_shift_depth:
        model.setwet(valf);
        model.setdry(1.0 - valf);
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
}

__unit_callback void unit_suspend() {
}

__unit_callback int32_t unit_get_param_value(uint8_t id) {
    return p_[id];
}

static char errnum[8];
static char hexstr[17] = "0123456789ABCDEF";
__unit_callback const char * unit_get_param_str_value(uint8_t id, int32_t value) {
    if (id == 3) {
        switch(value) {
        case 0:
            errnum[0] = 0x30 + (err / 100) % 10;
            errnum[1] = 0x30 + (err / 10) % 10;
            errnum[2] = 0x30 + err % 10;
            errnum[3] = 0;
            return errnum;
            break;
        case 1:
            errnum[0] = hexstr[(err2 & 0xf000) >> 12];
            errnum[1] = hexstr[(err2 & 0xf00) >> 8];
            errnum[2] = hexstr[(err2 & 0xf0) >> 4];
            errnum[3] = hexstr[err2 & 0xf];
            return errnum;
            break;
        case 2:
            errnum[0] = hexstr[(err3 & 0xf000) >> 12];
            errnum[1] = hexstr[(err3 & 0xf00) >> 8];
            errnum[2] = hexstr[(err3 & 0xf0) >> 4];
            errnum[3] = hexstr[err3 & 0xf];
            return errnum;
            break;
        case 3:
            errnum[0] = hexstr[(err4 & 0xf000) >> 12];
            errnum[1] = hexstr[(err4 & 0xf00) >> 8];
            errnum[2] = hexstr[(err4 & 0xf0) >> 4];
            errnum[3] = hexstr[err4 & 0xf];
            return errnum;
            break;
        default:
            return nullptr;
        }
    }
    return nullptr;
}

__unit_callback void unit_set_tempo(uint32_t tempo) {
}

__unit_callback void unit_tempo_4ppqn_tick(uint32_t counter) {
}
