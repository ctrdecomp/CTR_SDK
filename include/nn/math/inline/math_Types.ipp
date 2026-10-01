#pragma once

#include <nn/math/math_Vector3.h>
#include <nn/math/math_Matrix34.h>

namespace nn{
namespace math{

inline VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pM, const VEC3* pV)
{
    VEC3 tmp;
    tmp.x = pM->m[0][0] * pV->x + pM->m[0][1] * pV->y + pM->m[0][2] * pV->z;
    tmp.y = pM->m[1][0] * pV->x + pM->m[1][1] * pV->y + pM->m[1][2] * pV->z;
    tmp.z = pM->m[2][0] * pV->x + pM->m[2][1] * pV->y + pM->m[2][2] * pV->z;

    pOut->x = tmp.x;
    pOut->y = tmp.y;
    pOut->z = tmp.z;
    
    return pOut;
}
}
}