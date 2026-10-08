#pragma once

#include <nn/types.h>
#include <nn/Assert.h>

#include <cmath>

namespace nn { 
namespace math {

u32 CntBit1(u32 x);
u32 CntBit1(const u32* first, const u32* last);

inline f32 Max(f32 a, f32 b ){ if(a < b) b = a; return b; }
/* Inlines */

inline f32 FExp(f32 x)
{
    return ::std::expf(x);
}

inline f32 FLog(f32 x)
{
    NN_WARNING_(x > 0, "FLog: Input is out of the domain.");

    return ::std::logf(x);
}

inline u32 F32AsU32(f32 x)
{
    return *reinterpret_cast<u32*>(&x);
}

inline f32 FMod(f32 x, f32 y)
{
    return ::std::fmodf(x, y);
}

inline f32 FModf(f32 x, f32* y)
{
    return ::std::modff(x, y);
}

inline f32 FSqrt(f32 x)
{
    return ::std::sqrtf(x);
}

inline f32 FAbs(f32 x)
{
    f32 ret;
    ret = ::std::fabsf(x);
    return ret;
}

inline f32 FCeil(f32 x)
{
    return ::std::ceilf(x);
}

inline f32 FFloor(f32 x)
{
    return ::std::floorf(x);
}

/* U16 */

inline u16 F32ToU16(f32 x)
{
    return u16(x);
}

inline f32 U16ToF32(u16 x)
{
    return f32(x);
}

inline f32 HermiteC_FAST(f32 v0, f32 t0, f32 v1, f32 t1, f32 p, f32 d)
{

    f32 s = p / d;
    f32 s_1 = s - 1;
    f32 tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, ret;
    
    tmp1 = (v0 - v1);
    tmp2 = (2 * s - 3);
    tmp3 = s * s;
    tmp4 = p * s_1;
    tmp5 = s_1 * t0;
    tmp6 = s * t1;
    
    ret = v0 + tmp1 * tmp2 * tmp3 + tmp4 * (tmp5 + tmp6);
    return ret;
}

inline f32 HermiteC(f32 v0, f32 t0, f32 v1, f32 t1, f32 p, f32 d)
{
    f32 inv_d = 1 / d;
    f32 s = p * inv_d;
    f32 s_1 = s - 1;
    return v0 + (v0 - v1) * (2 * s - 3) * s * s + p * s_1 * (s_1 * t0 + s * t1);
}

inline u32 Hermite(f32 v0, f32 t0, f32 v1, f32 t1, f32 p, f32 d)
{
    #ifdef NN_MATH_BUILD_FAST
        return HermiteC(v0, t0, v1, t1, p, d);
    #else
        return HermiteC_FAST(v0, t0, v1, t1, p, d);        
    #endif
}

}
}
