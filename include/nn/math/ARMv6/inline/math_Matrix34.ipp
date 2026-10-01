#include <nn/math/math_Vector3.h>
#include <nn/math/math_Matrix34.h>
#include <nn/dbg/dbg_Break.h>

namespace nn{
namespace math{
class MTX34;

namespace ARMv6{

inline MTX34* MTX34ScaleC(MTX34* pOut, const VEC3* pS)
{
    f32 (*const m)[4] = pOut->m;

    m[0][0] = pS->x; m[0][1] = 0.0f;  m[0][2] = 0.0f;  m[0][3] = 0.0f;
    m[1][0] = 0.0f;  m[1][1] = pS->y; m[1][2] = 0.0f;  m[1][3] = 0.0f;
    m[2][0] = 0.0f;  m[2][1] = 0.0f;  m[2][2] = pS->z; m[2][3] = 0.0f;

    return pOut;
}

inline MTX34* MTX34ScaleC_FAST(MTX34* pOut, const VEC3* pS)
{
    f32 f0 = 0.0f;
    const unsigned int f32_0 = *(reinterpret_cast<unsigned int*>(&f0));
    unsigned int *m = reinterpret_cast<unsigned int *>(pOut->m);
    const unsigned int *p = reinterpret_cast<const unsigned int*>(pS);

    m[0] =  p[0];  m[1] = f32_0;  m[2] = f32_0;   m[3] = f32_0;
    m[4] = f32_0;  m[5] =  p[1];  m[6] = f32_0;   m[7] = f32_0;
    m[8] = f32_0;  m[9] = f32_0;  m[10] =  p[2];  m[11] = f32_0;

    return pOut;
}

inline VEC3* VEC3TransformC(VEC3* pOut, const MTX34* __restrict pM, const VEC3* __restrict pV)
{
    VEC3 vTmp;
    VEC3* pDst = (pOut == pV) ? &vTmp : pOut;
    
    pDst->x = pM->m[0][0] * pV->x + pM->m[0][1] * pV->y + pM->m[0][2] * pV->z + pM->m[0][3];
    pDst->y = pM->m[1][0] * pV->x + pM->m[1][1] * pV->y + pM->m[1][2] * pV->z + pM->m[1][3];
    pDst->z = pM->m[2][0] * pV->x + pM->m[2][1] * pV->y + pM->m[2][2] * pV->z + pM->m[2][3];
    
    if (pDst == &vTmp)
    {
        pOut->x = pDst->x;
        pOut->y = pDst->y;
        pOut->z = pDst->z;
    }
    return pOut;
}

inline VEC3* VEC3NormalizeC_FAST(VEC3* pOut, const VEC3* p)
{
    register f32 x, y, z, mag;
        
    x = p->x;
    y = p->y;
    z = p->z;
        
    mag = (x * x) + (y * y) + (z * z);

    mag = 1.0f / ::std::sqrtf(mag);
        
    x *= mag;
    y *= mag;
    z *= mag;

    pOut->x = x;
    pOut->y = y;
    pOut->z = z;
    
    return pOut;
}

}
}
}