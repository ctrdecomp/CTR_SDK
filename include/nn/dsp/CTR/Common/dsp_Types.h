#pragma once

#include "nn/types.h"

namespace nn {
namespace dsp {
namespace CTR {

typedef u16 DSPAddr;
typedef u16 DSPWord;
typedef u16 DSPByte;
typedef u32 DSPWord32;
typedef u32 DSPByte32;
typedef u32 DSPAddrInARM;

#define NN_DSP_32BIT_TO_DSP(value)   (u32)(((u32)(value) >> 16) | ((u32)(value) << 16))

} // namespace CTR
} // namespace dsp
} // namespace nn

