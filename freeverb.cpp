/*
 * File: freeverb.cpp
 *
 */

#include "userrevfx.h"
#include "revmodel.hpp"

static revmodel model;

void REVFX_INIT(uint32_t platform, uint32_t api)
{
}

void REVFX_PROCESS(float *main_xn, 
                   uint32_t frames)
{
    float * __restrict my = main_xn;
    const float * my_e = my + 2 * frames;

    model.processmix(my, my + 1, my, my + 1, frames, 2);
}


void REVFX_PARAM(uint8_t index, int32_t value)
{
    const float valf = q31_to_f32(value);
    switch (index) {
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
}
