#pragma once

#include <nn/math/math_Matrix23.h>
#include <nn/math/math_Matrix33.h>
#include <nn/math/math_Matrix34.h>
#include <nn/math/math_Matrix44.h>
#include <nn/math/math_Transform.h>
#include <nn/math/math_Quaternion.h>
#include <nn/math/math_Vec2.h>
#include <nn/math/math_Vec3.h>
#include <nn/math/math_Vec4.h>

#ifdef __cplusplus

using namespace nn::math::ARMv6;

#endif

namespace nn{
namespace math{

inline MTX22* MTX23ToMTX22(MTX22* pOut, const MTX23* pM)
{
    NN_NULL_ASSERT_(pOut);
    NN_NULL_ASSERT_(pM);
    
    pOut->f._00 = pM->f._00; pOut->f._01 = pM->f._01;
    pOut->f._10 = pM->f._10; pOut->f._11 = pM->f._11;
    
    return pOut;
}

inline MTX23* MTX22ToMTX23(MTX23* pOut, const MTX22* pM)
{
    NN_NULL_ASSERT_(pOut);
    NN_NULL_ASSERT_(pM);
    
    pOut->f._00 = pM->f._00;
    pOut->f._01 = pM->f._01;
    pOut->f._10 = pM->f._10;
    pOut->f._11 = pM->f._11;

    pOut->f._02 = pOut->f._12 = 0.f;
    return pOut;
}
}
}