#pragma once

#include <nn/math/math_Constant.h>
#include <nn/math/math_Arithmetic.h>

#define NN_MATH_RAD_TO_FIDX(rad)  ((rad)  * (256.f / (2.0f * ::nn::math::PI)))

namespace nn{
namespace math{
namespace internal{
struct SinCosSample
{
    f32 sin_val;
    f32 cos_val;
    f32 sin_delta;
    f32 cos_delta;
};

}

void SinCosFIdx(f32* pSin, f32* pCos, f32 fidx);
f32 SinFIdx(f32 fidx);
f32 CosFIdx(f32 fidx);
f32 AtanFIdx(f32 x);
ushort Atan2Idx(f32 y, f32 x);
f32 Atan2FIdx(f32 y, f32 x);

inline f32 TanFIdx(f32 fidx)
{
    f32 sin, cos;
    SinCosFIdx(&sin, &cos, fidx);
    return sin / cos;
}

inline f32 TanRad(f32 rad)  { return TanFIdx(NN_MATH_RAD_TO_FIDX(rad)); }

inline f32 CosRad(f32 rad)  { return CosFIdx(NN_MATH_RAD_TO_FIDX(rad)); }

inline void SinCosRad(f32* s, f32* c, f32 rad) { SinCosFIdx(s, c, NN_MATH_RAD_TO_FIDX(rad)); }

inline f32 TanDeg(f32 deg){ return TanFIdx(deg * 0.7111111); }

}
}