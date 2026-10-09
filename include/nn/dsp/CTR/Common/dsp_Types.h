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

#define NN_DSP_ADDR_TO_ARM(address)  (u32)((address) << 1)
#define NN_DSP_ADDR_TO_DSP(address)  (u16)((u32)(address) >> 1)
#define NN_DSP_WORD_TO_ARM(word)     (u16)((word) << 1)
#define NN_DSP_WORD_TO_DSP(word)     (u16)((word) >> 1)
#define NN_DSP_WORD_TO_ARM32(word)   (u32)((word) << 1)
#define NN_DSP_WORD_TO_DSP32(word)   (u32)((word) >> 1)
#define NN_DSP_32BIT_TO_ARM(value)   (u32)(((u32)(value) >> 16) | ((u32)(value) << 16))
#define NN_DSP_32BIT_TO_DSP(value)   (u32)(((u32)(value) >> 16) | ((u32)(value) << 16))

#ifdef NN_SYSTEM_PROCESS
#define NN_DSP_BYTE_TO_UNIT(byte)    (u16)(byte)
#define NN_DSP_UNIT_TO_BYTE(unit)    (u16)(unit)
#else
#define NN_DSP_BYTE_TO_UNIT(byte)    (u16)((byte) >> 1)
#define NN_DSP_UNIT_TO_BYTE(unit)    (u16)((unit) << 1)
#endif

#define NN_DSP_WORD_UNIT             (3 - sizeof(DSPWord))

} // namespace CTR
} // namespace dsp
} // namespace nn

