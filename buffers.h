#include "userrevfx.h"
#include "tuning.h"

// Buffers for the combs
static __sdram float	bufcombL1[combtuningL1];
static __sdram float	bufcombR1[combtuningR1];
static __sdram float	bufcombL2[combtuningL2];
static __sdram float	bufcombR2[combtuningR2];
static __sdram float	bufcombL3[combtuningL3];
static __sdram float	bufcombR3[combtuningR3];
static __sdram float	bufcombL4[combtuningL4];
static __sdram float	bufcombR4[combtuningR4];
static __sdram float	bufcombL5[combtuningL5];
static __sdram float	bufcombR5[combtuningR5];
static __sdram float	bufcombL6[combtuningL6];
static __sdram float	bufcombR6[combtuningR6];
static __sdram float	bufcombL7[combtuningL7];
static __sdram float	bufcombR7[combtuningR7];
static __sdram float	bufcombL8[combtuningL8];
static __sdram float	bufcombR8[combtuningR8];

// Buffers for the allpasses
static __sdram float	bufallpassL1[allpasstuningL1];
static __sdram float	bufallpassR1[allpasstuningR1];
static __sdram float	bufallpassL2[allpasstuningL2];
static __sdram float	bufallpassR2[allpasstuningR2];
static __sdram float	bufallpassL3[allpasstuningL3];
static __sdram float	bufallpassR3[allpasstuningR3];
static __sdram float	bufallpassL4[allpasstuningL4];
static __sdram float	bufallpassR4[allpasstuningR4];

