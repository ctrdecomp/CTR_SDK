#pragma once

#include <nn/math/math_Vector3.h>

#pragma push
#pragma Otime

namespace nn{
namespace math{
class VEC3;
class MTX33;
class MTX34;

inline MTX33* MTX33Copy(MTX33* pOut, const MTX33* p);
//inline MTX34* MTX33ToMTX34(MTX34* pOut, const MTX33* pM);
//inline MTX33* MTX34ToMTX33(MTX33* pOut, const MTX34* pM);

class MTX33_
{
public:
    struct BaseData
    {
        f32 _00;
        f32 _01;
        f32 _02;
        f32 _10;
        f32 _11;
        f32 _12;
        f32 _20;
        f32 _21;
        f32 _22;
    };

    union
    {
        #if defined(NN_MATH_USE_ANONYMOUS)
            struct
            {
                f32 _00, _01, _02;
                f32 _10, _11, _12;
                f32 _20, _21, _22;
            };
        #endif
        BaseData f;
        f32 m[3][3];
        f32 a[9];
        VEC3_ v[3];
    };
};

class MTX33 : public MTX33_
{
public:
    MTX33() 
    {
    }

    explicit MTX33(const f32* p) { MTX33Copy(this, reinterpret_cast<const MTX33*>(p)); }
    //explicit MTX33(const MTX34& rhs) { MTX34ToMTX33(this, &rhs); }
    MTX33(f32 x00, f32 x01, f32 x02,f32 x10, f32 x11, f32 x12,f32 x20, f32 x21, f32 x22)
    {
        f._00 = x00; f._01 = x01; f._02 = x02;
        f._10 = x10; f._11 = x11; f._12 = x12;
        f._20 = x20; f._21 = x21; f._22 = x22;
    }
    static const int ROW_COUNT = 3;
    static const int COLUMN_COUNT = 3;
    
    static const MTX33& Identity()
    {
        static const MTX33 identity(1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,0.0f, 0.0f, 1.0f);
        return identity;
    }
};

}
}

namespace nn{
namespace math{
namespace ARMv6{

inline MTX33* MTX33CopyC(MTX33* pOut, const MTX33* p)
{
    if (pOut != p)
    {
        *pOut = *p;
    }

    return pOut;
}
MTX33* MTX33CopyAsm(MTX33* pOut, const MTX33* p);

template<typename TMatrix>
inline TMatrix* MTX33MultC(TMatrix* pOut, const TMatrix* __restrict p1, const TMatrix* __restrict p2)
{
    NN_NULL_ASSERT_(p1);
    NN_NULL_ASSERT_(p2);
    NN_NULL_ASSERT_(pOut);

    TMatrix mTmp;
    TMatrix* __restrict pDst = (pOut == p1 || pOut == p2) ? &mTmp : pOut;

    pDst->f._00 = p1->f._00 * p2->f._00 + p1->f._01 * p2->f._10 + p1->f._02 * p2->f._20;
    pDst->f._01 = p1->f._00 * p2->f._01 + p1->f._01 * p2->f._11 + p1->f._02 * p2->f._21;
    pDst->f._02 = p1->f._00 * p2->f._02 + p1->f._01 * p2->f._12 + p1->f._02 * p2->f._22;

    pDst->f._10 = p1->f._10 * p2->f._00 + p1->f._11 * p2->f._10 + p1->f._12 * p2->f._20;
    pDst->f._11 = p1->f._10 * p2->f._01 + p1->f._11 * p2->f._11 + p1->f._12 * p2->f._21;
    pDst->f._12 = p1->f._10 * p2->f._02 + p1->f._11 * p2->f._12 + p1->f._12 * p2->f._22;

    pDst->f._20 = p1->f._20 * p2->f._00 + p1->f._21 * p2->f._10 + p1->f._22 * p2->f._20;
    pDst->f._21 = p1->f._20 * p2->f._01 + p1->f._21 * p2->f._11 + p1->f._22 * p2->f._21;
    pDst->f._22 = p1->f._20 * p2->f._02 + p1->f._21 * p2->f._12 + p1->f._22 * p2->f._22;

    if (pDst == &mTmp)
    {
        pOut->f._00 = pDst->f._00; pOut->f._01 = pDst->f._01; pOut->f._02 = pDst->f._02;
        pOut->f._10 = pDst->f._10; pOut->f._11 = pDst->f._11; pOut->f._12 = pDst->f._12;
        pOut->f._20 = pDst->f._20; pOut->f._21 = pDst->f._21; pOut->f._22 = pDst->f._22;
    }

    return pOut;
}

template<typename TMatrix>
TMatrix* MTX33MultAsm(TMatrix* pOut, const TMatrix* p1, const TMatrix* p2);

VEC3* VEC3TransformAsm(VEC3* pOut, const MTX33* pM, const VEC3* pV);
inline VEC3* VEC3TransformC(VEC3* pOut, const MTX33* pM, const VEC3* pV)
{
    NN_NULL_ASSERT_(pOut);
    NN_NULL_ASSERT_(pM);
    NN_NULL_ASSERT_(pV);

    VEC3 vTmp;
    VEC3* pDst = (pOut == pV) ? &vTmp : pOut;
    pDst->x = pM->f._00 * pV->x + pM->f._01 * pV->y + pM->f._02 * pV->z;
    pDst->y = pM->f._10 * pV->x + pM->f._11 * pV->y + pM->f._12 * pV->z;
    pDst->z = pM->f._20 * pV->x + pM->f._21 * pV->y + pM->f._22 * pV->z;

    if (pDst == &vTmp)
    {
        pOut->x = pDst->x;
        pOut->y = pDst->y;
        pOut->z = pDst->z;
    }

    return pOut;
}

}

inline VEC3* VEC3Transform(VEC3* pOut, const MTX33* __restrict pM, const VEC3* __restrict pV)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6::VEC3TransformC(pOut,pM,pV);
    #else
        return ARMv6::VEC3TransformAsm(pOut, pM, pV);
    #endif
}

inline MTX33* MTX33Copy(MTX33* pOut, const MTX33* p)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6::MTX33CopyC(pOut,p);
    #else
        return ARMv6::MTX33CopyAsm(pOut,p);
    #endif
}

template<typename TMatrix>
inline TMatrix* MTX33Mult(TMatrix* pOut, const TMatrix* p1, const TMatrix* p2)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6::MTX33MultC(pOut,p1,p2);
    #else
        return ARMv6::MTX33MultAsm(pOut,p1,p2);
    #endif
}

template<typename TMatrix>
inline TMatrix* MTX33Mult(TMatrix* pOut, const TMatrix& m1, const TMatrix& m2) { return MTX33Mult(pOut, &m1, &m2); }

template<typename TMatrix>
inline asm TMatrix* MTX33MultAsm(TMatrix* pOut, const TMatrix* p1, const TMatrix* p2)
{
    MOV         r3,#__cpp(offsetof(TMatrix,f))
    ADD         r1,r1,r3
    ADD         r2,r2,r3
    MOV         r3,#__cpp(TMatrix::COLUMN_COUNT)*4

    CMP         r3,#3*4
    BNE         LABELX

    VPUSH       {d8}                  // Save registers
    VLDMIA      r2!,{s10-s15}         // First and second line of matrix p2 to registers [S10-S15]

    VLDR.F32    s16,[r1,#3*4*0+4*0]
    VLDR.F32    s17,[r1,#3*4*1+4*0]

    VMUL.F32    s0,s10,s16
    VMUL.F32    s1,s11,s16
    VMUL.F32    s2,s12,s16
    VLDR.F32    s16,[r1,#3*4*2+4*0]

    VMUL.F32    s3,s10,s17
    VMUL.F32    s4,s11,s17
    VMUL.F32    s5,s12,s17
    VLDR.F32    s17,[r1,#3*4*0+4*1]

    VMUL.F32    s6,s10,s16
    VMUL.F32    s7,s11,s16
    VMUL.F32    s8,s12,s16
    VLDR.F32    s16,[r1,#3*4*1+4*1]

    VLDMIA      r2,{s10-s12}         // Third line of matrix p2 to registers [S10-S12]
    VMLA.F32    s0,s13,s17
    VMLA.F32    s1,s14,s17
    VMLA.F32    s2,s15,s17
    VLDR.F32    s17,[r1,#3*4*2+4*1]
                
    VMLA.F32    s3,s13,s16
    VMLA.F32    s4,s14,s16
    VMLA.F32    s5,s15,s16
    VLDR.F32    s16,[r1,#3*4*0+4*2]
                
    VMLA.F32    s6,s13,s17
    VMLA.F32    s7,s14,s17
    VMLA.F32    s8,s15,s17
    VLDR.F32    s17,[r1,#3*4*1+4*2]

    VMLA.F32    s0,s10,s16
    VMLA.F32    s1,s11,s16
    VMLA.F32    s2,s12,s16
    VLDR.F32    s16,[r1,#3*4*2+4*2]
                
    VMLA.F32    s3,s10,s17
    VMLA.F32    s4,s11,s17
    VMLA.F32    s5,s12,s17
                
    VMLA.F32    s6,s10,s16
    VMLA.F32    s7,s11,s16
    VMLA.F32    s8,s12,s16

    VPOP        {d8}

    VSTMIA      r0,{s0-s8}
    BX          lr

LABELX
    VPUSH       {d8-d13}
    VLDMIA      r2,{s9-s11}
    VLDMIA      r1,{s18-s20}
    ADD         r1,r1,r3
    ADD         r2,r2,r3
    VLDMIA      r2,{s12-s14}
    VLDMIA      r1,{s21-s23}
    ADD         r1,r1,r3
    ADD         r2,r2,r3
    VLDMIA      r2,{s15-s17}
    VLDMIA      r1,{s24-s26}

    VMUL.F32    s0,s9,s18
    VMUL.F32    s1,s10,s18
    VMUL.F32    s2,s11,s18

    VMUL.F32    s3,s9,s21
    VMUL.F32    s4,s10,s21
    VMUL.F32    s5,s11,s21

    VMUL.F32    s6,s9,s24
    VMUL.F32    s7,s10,s24
    VMUL.F32    s8,s11,s24

    VMLA.F32    s0,s12,s19
    VMLA.F32    s1,s13,s19
    VMLA.F32    s2,s14,s19
                
    VMLA.F32    s3,s12,s22
    VMLA.F32    s4,s13,s22
    VMLA.F32    s5,s14,s22
                
    VMLA.F32    s6,s12,s25
    VMLA.F32    s7,s13,s25
    VMLA.F32    s8,s14,s25

    VMLA.F32    s0,s15,s20
    VMLA.F32    s1,s16,s20
    VMLA.F32    s2,s17,s20
                
    VMLA.F32    s3,s15,s23
    VMLA.F32    s4,s16,s23
    VMLA.F32    s5,s17,s23
                
    VMLA.F32    s6,s15,s26
    VMLA.F32    s7,s16,s26
    VMLA.F32    s8,s17,s26

    VPOP        {d8-d13}

    ADD         r1,r0,r3
    ADD         r2,r1,r3
    VSTMIA      r0,{s0-s2}
    VSTMIA      r1,{s3-s5}
    VSTMIA      r2,{s6-s8}
    BX          lr
}

}
}

