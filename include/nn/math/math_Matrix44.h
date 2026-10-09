#pragma once

#include <nn/math/math_Matrix34.h>

namespace nn{
namespace math{

enum PivotDirection
{
    PIVOT_NONE,
    PIVOT_UPSIDE_TO_TOP,
    PIVOT_UPSIDE_TO_RIGHT,
    PIVOT_UPSIDE_TO_BOTTOM,
    PIVOT_UPSIDE_TO_LEFT,
    PIVOT_NUM
};


class MTX44;

inline MTX44* MTX44Copy(MTX44* pOut, const MTX44* m);
inline MTX44* MTX44Copy(MTX44* pOut, const MTX44& m) { return MTX44Copy(pOut, &m); }

inline MTX44* MTX44Mult(MTX44* pOut, const MTX44* __restrict p1, const MTX44* __restrict p2);
inline MTX44* MTX44Mult(MTX44* pOut, const MTX44* __restrict p1, f32 f);

inline MTX44* MTX44Mult(MTX44* pOut, const MTX44& m1, f32 f){ return MTX44Mult(pOut, &m1, f); }
inline MTX44* MTX44Mult(MTX44* pOut, const MTX44& m1, const MTX44& m2) { return MTX44Mult(pOut, &m1, &m2); }

class MTX44_
{
public:
    struct BaseData
    {
        f32 _00;
        f32 _01;
        f32 _02;
        f32 _03;
        f32 _10;
        f32 _11;
        f32 _12;
        f32 _13;
        f32 _20;
        f32 _21;
        f32 _22;
        f32 _23;
        f32 _30;
        f32 _31;
        f32 _32;
        f32 _33;
    };
    union
    {
        BaseData f;
        #if defined(NN_MATH_USE_ANONYMOUS)
            struct
            {
                f32 _00, _01, _02, _03;
                f32 _10, _11, _12, _13;
                f32 _20, _21, _22, _23;
                f32 _30, _31, _32, _33;
            };
        #endif
        f32 m[4][4];
        f32 a[16];
        VEC4_ v[4];
    };
};

class MTX44 : public MTX44_
{
public:
    typedef MTX44 self_type;

    MTX44() 
    {
    }

    explicit MTX44(const f32* p) { (void)MTX44Copy(this, (MTX44*)p); }
    explicit MTX44(const MTX34& rhs)
    {
        (void)MTX34Copy((MTX34*)this, (MTX34*)&rhs);
        f._30 = f._31 = f._32 = 0.f; f._33 = 1.f;
    }
    MTX44(const MTX44& rhs) { (void)MTX44Copy(this, &rhs); }
    MTX44(f32 x00, f32 x01, f32 x02, f32 x03,f32 x10, f32 x11, f32 x12, f32 x13,f32 x20, f32 x21, f32 x22, f32 x23,f32 x30, f32 x31, f32 x32, f32 x33)
    {
        f._00 = x00; f._01 = x01; f._02 = x02; f._03 = x03;
        f._10 = x10; f._11 = x11; f._12 = x12; f._13 = x13;
        f._20 = x20; f._21 = x21; f._22 = x22; f._23 = x23;
        f._30 = x30; f._31 = x31; f._32 = x32; f._33 = x33;
    }

    operator f32*() { return this->a; }
    operator const f32*() const { return this->a; }
    self_type& operator *= (const self_type& rhs) { return *MTX44Mult(this, this, &rhs); }
    self_type& operator *= (f32 f) { return *MTX44Mult(this, this, f); }

    static const int ROW_COUNT = 4; //
    static const int COLUMN_COUNT = 4; //
    static const MTX44& Identity()
    {
        static const MTX44 identity(1.0f, 0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f);
        return identity;
    }
};

inline MTX44* MTX44Identity(MTX44* pOut)
{
    MTX44Copy(pOut, MTX44::Identity());

    return pOut;
}

}
}

namespace nn{
namespace math{
namespace ARMv6{
namespace 
{
    inline void SwapF(f32 &a, f32 &b)
    {
        f32 tmp;
        tmp = a;
        a = b;
        b = tmp;
    }
}

inline MTX44* MTX44MultC(MTX44* pOut, const MTX44* __restrict p1, const MTX44* __restrict p2)
{
    NN_NULL_ASSERT_(pOut);
    NN_NULL_ASSERT_(p1);
    NN_NULL_ASSERT_(p2);
    
    MTX44 mTmp;
    
    MTX44* __restrict pDst = ( pOut == p1 || pOut == p2 ) ? &mTmp : pOut;
    
    pDst->f._00 = p1->f._00 * p2->f._00 + p1->f._01 * p2->f._10 + p1->f._02 * p2->f._20 + p1->f._03 * p2->f._30;
    pDst->f._01 = p1->f._00 * p2->f._01 + p1->f._01 * p2->f._11 + p1->f._02 * p2->f._21 + p1->f._03 * p2->f._31;
    pDst->f._02 = p1->f._00 * p2->f._02 + p1->f._01 * p2->f._12 + p1->f._02 * p2->f._22 + p1->f._03 * p2->f._32;
    pDst->f._03 = p1->f._00 * p2->f._03 + p1->f._01 * p2->f._13 + p1->f._02 * p2->f._23 + p1->f._03 * p2->f._33;
    
    pDst->f._10 = p1->f._10 * p2->f._00 + p1->f._11 * p2->f._10 + p1->f._12 * p2->f._20 + p1->f._13 * p2->f._30;
    pDst->f._11 = p1->f._10 * p2->f._01 + p1->f._11 * p2->f._11 + p1->f._12 * p2->f._21 + p1->f._13 * p2->f._31;
    pDst->f._12 = p1->f._10 * p2->f._02 + p1->f._11 * p2->f._12 + p1->f._12 * p2->f._22 + p1->f._13 * p2->f._32;
    pDst->f._13 = p1->f._10 * p2->f._03 + p1->f._11 * p2->f._13 + p1->f._12 * p2->f._23 + p1->f._13 * p2->f._33;
    
    pDst->f._20 = p1->f._20 * p2->f._00 + p1->f._21 * p2->f._10 + p1->f._22 * p2->f._20 + p1->f._23 * p2->f._30;
    pDst->f._21 = p1->f._20 * p2->f._01 + p1->f._21 * p2->f._11 + p1->f._22 * p2->f._21 + p1->f._23 * p2->f._31;
    pDst->f._22 = p1->f._20 * p2->f._02 + p1->f._21 * p2->f._12 + p1->f._22 * p2->f._22 + p1->f._23 * p2->f._32;
    pDst->f._23 = p1->f._20 * p2->f._03 + p1->f._21 * p2->f._13 + p1->f._22 * p2->f._23 + p1->f._23 * p2->f._33;

    pDst->f._30 = p1->f._30 * p2->f._00 + p1->f._31 * p2->f._10 + p1->f._32 * p2->f._20 + p1->f._33 * p2->f._30;
    pDst->f._31 = p1->f._30 * p2->f._01 + p1->f._31 * p2->f._11 + p1->f._32 * p2->f._21 + p1->f._33 * p2->f._31;
    pDst->f._32 = p1->f._30 * p2->f._02 + p1->f._31 * p2->f._12 + p1->f._32 * p2->f._22 + p1->f._33 * p2->f._32;
    pDst->f._33 = p1->f._30 * p2->f._03 + p1->f._31 * p2->f._13 + p1->f._32 * p2->f._23 + p1->f._33 * p2->f._33;
    
    if (pDst != pOut)
    {
        MTX44Copy(pOut, pDst);
    }
    
    return pOut;
}
MTX44* MTX44MultAsm(MTX44* pOut, const MTX44* __restrict p1, const MTX44* __restrict p2);

inline MTX44* MTX44MultC(MTX44* pOut, const MTX44* p, f32 f)
{
    pOut->f._00 = p->f._00 * f;
    pOut->f._01 = p->f._01 * f;
    pOut->f._02 = p->f._02 * f;
    pOut->f._03 = p->f._03 * f;

    pOut->f._10 = p->f._10 * f;
    pOut->f._11 = p->f._11 * f;
    pOut->f._12 = p->f._12 * f;
    pOut->f._13 = p->f._13 * f;

    pOut->f._20 = p->f._20 * f;
    pOut->f._21 = p->f._21 * f;
    pOut->f._22 = p->f._22 * f;
    pOut->f._23 = p->f._23 * f;

    pOut->f._30 = p->f._30 * f;
    pOut->f._31 = p->f._31 * f;
    pOut->f._32 = p->f._32 * f;
    pOut->f._33 = p->f._33 * f;

    return pOut;
}
MTX44* MTX44MultAsm(MTX44* pOut, const MTX44* __restrict p1, f32 f);

inline MTX44* MTX44MultScaleC(MTX44* pOut, const MTX44* pM, const VEC3* pS)
{
    // Version where the scale matrix is applied from the right.
    pOut->f._00 = pM->f._00 * pS->x;
    pOut->f._10 = pM->f._10 * pS->x;
    pOut->f._20 = pM->f._20 * pS->x;

    pOut->f._01 = pM->f._01 * pS->y;
    pOut->f._11 = pM->f._11 * pS->y;
    pOut->f._21 = pM->f._21 * pS->y;

    pOut->f._02 = pM->f._02 * pS->z;
    pOut->f._12 = pM->f._12 * pS->z;
    pOut->f._22 = pM->f._22 * pS->z;

    if (pOut != pM)
    {
        pOut->f._03 = pM->f._03;
        pOut->f._13 = pM->f._13;
        pOut->f._23 = pM->f._23;
    }

    return pOut;
}
void MTX44MultScaleAsm(MTX44* pOut, MTX44 const* p1, VEC3 const* p2);

inline MTX44* MTX44MultTranslateC(MTX44* pOut, const VEC3* pT, const MTX44* pM)
{
    NN_NULL_ASSERT_(pOut);
    NN_NULL_ASSERT_(pT);
    NN_NULL_ASSERT_(pM);

    const f32 (*const src)[4] = pM->m;
    f32 (*const dst)[4] = pOut->m;
    
    if (src != dst)
    {
        dst[0][0] = src[0][0];  dst[0][1] = src[0][1];  dst[0][2] = src[0][2];
        dst[1][0] = src[1][0];  dst[1][1] = src[1][1];  dst[1][2] = src[1][2];
        dst[2][0] = src[2][0];  dst[2][1] = src[2][1];  dst[2][2] = src[2][2];
    }
    
    dst[0][3] = src[0][3] + pT->x;
    dst[1][3] = src[1][3] + pT->y;
    dst[2][3] = src[2][3] + pT->z;
    
    return pOut;
}
MTX44* MTX44MultTranslateAsm(MTX44* pOut, const VEC3* v, const MTX44* p1);

inline u32 MTX44InverseC(MTX44* pOut, const MTX44* p)
{
    MTX44 mTmp;
    f32 (*src)[4];
    f32 (*inv)[4];
    f32   w;

    MTX44Copy(&mTmp, p);
    MTX44Identity(pOut);
    
    src = mTmp.m;
    inv = pOut->m;
    
    for (int i = 0; i < 4; ++i)
    {
        f32 max = 0.0f;
        s32 swp = i;

        for(int k = i ; k < 4 ; k++)
        {
            f32 ftmp;
            ftmp = ::std::fabs(src[k][i]);
            if (ftmp > max){
                max = ftmp;
                swp = k;
            }
        }
        
        if (max == 0.0f)
        {
            return 0;
        }

        if (swp != i)
        {
            for (int k = 0; k < 4; k++)
            {
                SwapF(src[i][k], src[swp][k]);
                SwapF(inv[i][k], inv[swp][k]);
            }
        }

        
        w = 1.0f / src[i][i];
        for (int j = 0; j < 4; ++j)
        {
            src[i][j] *= w;
            inv[i][j] *= w;
        }
        
        for (int k = 0; k < 4; ++k )
        {
            if ( k == i )
                continue;
            
            w = src[k][i];
            for (int j = 0; j < 4; ++j)
            {
                src[k][j] -= src[i][j] * w;
                inv[k][j] -= inv[i][j] * w;
            }
        }
    }
    
    return 1;
}

inline u32 MTX44InverseC_FAST(MTX44* pOut, const MTX44* p)
{
    const f32 (*src)[4];
    f32 (*inv)[4];

    src = p->m;
    inv = pOut->m;

    f32 a11, a12, a13, a14, a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44;
    f32 b11, b12, b13, b14, b21, b22, b23, b24, b31, b32, b33, b34, b41, b42, b43, b44;
    f32 det;
    
    a11 = src[0][0];
    a12 = src[0][1];
    a13 = src[0][2];
    a14 = src[0][3];

    a21 = src[1][0];
    a22 = src[1][1];
    a23 = src[1][2];
    a24 = src[1][3];

    a31 = src[2][0];
    a32 = src[2][1];
    a33 = src[2][2];
    a34 = src[2][3];

    a41 = src[3][0];
    a42 = src[3][1];
    a43 = src[3][2];
    a44 = src[3][3];
    
    det = a11*(a22*a33*a44 + a23*a34*a42 + a24*a32*a43)
        + a12*(a21*a34*a43 + a23*a31*a44 + a24*a33*a41)
        + a13*(a21*a32*a44 + a22*a34*a41 + a24*a31*a42)
        + a14*(a21*a33*a42 + a22*a31*a43 + a23*a32*a41)
        - a11*(a22*a34*a43 + a23*a32*a44 + a24*a33*a42)
        - a12*(a21*a33*a44 + a23*a34*a41 + a24*a31*a43)
        - a13*(a21*a34*a42 + a22*a31*a44 + a24*a32*a41)
        - a14*(a21*a32*a43 + a22*a33*a41 + a23*a31*a42);
        
    if(det==0.0f)
        return 0;

    det = 1.0f / det;

    f32 a33xa44_a34xa43, a32xa44_a34xa42, a33xa42_a32xa43,
        a33xa41_a31xa43, a31xa44_a34xa41, a32xa41_a31xa42;
    
    a33xa44_a34xa43 = a33*a44 - a34*a43;
    a32xa44_a34xa42 = a32*a44 - a34*a42;
    a33xa42_a32xa43 = a33*a42 - a32*a43;
    a33xa41_a31xa43 = a33*a41 - a31*a43;
    a31xa44_a34xa41 = a31*a44 - a34*a41;
    a32xa41_a31xa42 = a32*a41 - a31*a42;
    
    f32 a23xa44_a24xa43, a24xa33_a23xa34, a24xa42_a22xa44, a22xa43_a23xa42,
        a22xa34_a24xa32, a23xa32_a22xa33, a21xa44_a24xa41, a23xa41_a21xa43,
        a24xa31_a21xa34, a21xa33_a23xa31, a21xa42_a22xa41, a22xa31_a21xa32;
    
    a23xa44_a24xa43 = a23*a44 - a24*a43;
    a24xa33_a23xa34 = a24*a33 - a23*a34;
    a24xa42_a22xa44 = a24*a42 - a22*a44;
    a22xa43_a23xa42 = a22*a43 - a23*a42;
    a22xa34_a24xa32 = a22*a34 - a24*a32;
    a23xa32_a22xa33 = a23*a32 - a22*a33;
    a21xa44_a24xa41 = a21*a44 - a24*a41;
    a23xa41_a21xa43 = a23*a41 - a21*a43;
    a24xa31_a21xa34 = a24*a31 - a21*a34;
    a21xa33_a23xa31 = a21*a33 - a23*a31;
    a21xa42_a22xa41 = a21*a42 - a22*a41;
    a22xa31_a21xa32 = a22*a31 - a21*a32;
    
    b11 =( a22*a33xa44_a34xa43) - (a23*a32xa44_a34xa42) - (a24*a33xa42_a32xa43);
    b12 =( a13*a32xa44_a34xa42) + (a14*a33xa42_a32xa43) - (a12*a33xa44_a34xa43);
    b13 =( a12*a23xa44_a24xa43) + (a13*a24xa42_a22xa44) + (a14*a22xa43_a23xa42);
    b14 =( a12*a24xa33_a23xa34) + (a13*a22xa34_a24xa32) + (a14*a23xa32_a22xa33);
    b21 =( a23*a31xa44_a34xa41) + (a24*a33xa41_a31xa43) - (a21*a33xa44_a34xa43);
    b22 =( a11*a33xa44_a34xa43) - (a13*a31xa44_a34xa41) - (a14*a33xa41_a31xa43);
    b23 =( a13*a21xa44_a24xa41) + (a14*a23xa41_a21xa43) - (a11*a23xa44_a24xa43);
    b24 =( a13*a24xa31_a21xa34) + (a14*a21xa33_a23xa31) - (a11*a24xa33_a23xa34);
    b31 =( a21*a32xa44_a34xa42) - (a22*a31xa44_a34xa41) - (a24*a32xa41_a31xa42);
    b32 =( a12*a31xa44_a34xa41) + (a14*a32xa41_a31xa42) - (a11*a32xa44_a34xa42);
    b33 =( a14*a21xa42_a22xa41) - (a11*a24xa42_a22xa44) - (a12*a21xa44_a24xa41);
    b34 =( a14*a22xa31_a21xa32) - (a11*a22xa34_a24xa32) - (a12*a24xa31_a21xa34);
    b41 =( a21*a33xa42_a32xa43) - (a22*a33xa41_a31xa43) + (a23*a32xa41_a31xa42);
    b42 =( a12*a33xa41_a31xa43) - (a13*a32xa41_a31xa42) - (a11*a33xa42_a32xa43);
    b43 =(-a13*a21xa42_a22xa41) - (a11*a22xa43_a23xa42) - (a12*a23xa41_a21xa43);
    b44 =(-a13*a22xa31_a21xa32) - (a11*a23xa32_a22xa33) - (a12*a21xa33_a23xa31);

    b11 = b11 * det;
    b12 = b12 * det;
    b13 = b13 * det;
    b14 = b14 * det;
    b21 = b21 * det;
    b22 = b22 * det;
    b23 = b23 * det;
    b24 = b24 * det;
    b31 = b31 * det;
    b32 = b32 * det;
    b33 = b33 * det;
    b34 = b34 * det;
    b41 = b41 * det;
    b42 = b42 * det;
    b43 = b43 * det;
    b44 = b44 * det;

    inv[0][0] = b11;
    inv[0][1] = b12;
    inv[0][2] = b13;
    inv[0][3] = b14;

    inv[1][0] = b21;
    inv[1][1] = b22;
    inv[1][2] = b23;
    inv[1][3] = b24;

    inv[2][0] = b31;
    inv[2][1] = b32;
    inv[2][2] = b33;
    inv[2][3] = b34;

    inv[3][0] = b41;
    inv[3][1] = b42;
    inv[3][2] = b43;
    inv[3][3] = b44;

    return 1;
}

inline MTX44* MTX44FrustumC_FAST(MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    NN_NULL_ASSERT_(pOut);

    f32 (*const m)[4] = pOut->m;
    f32 tmp1 =  1.0f / (r - l);
    f32 tmp3 =  1.0f / (f - n);
    f32 tmp2 =  1.0f / (t - b);

    register f32 m00, m02, m11, m12, m22, m23;

    m00 = (2*n) * tmp1;
    m02 = (r + l) * tmp1;

    m11 = (2*n) * tmp2;
    m12 = (t + b) * tmp2;

    m22 = f * tmp3;
    m23 = f * n * tmp3;

    m[0][1] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = -1.0f;
    m[3][3] = 0.0f;

    m[0][0] = m00;
    m[0][2] = m02;

    m[1][1] = m11;
    m[1][2] = m12;

    m[2][2] = m22;
    m[2][3] = m23;

    return pOut;
}
inline MTX44* MTX44FrustumC(MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    NN_NULL_ASSERT_(pOut);

    f32 (*const m)[4] = pOut->m;
    f32 tmp     =  1.0f / (r - l);
    m[0][0] =  (2*n) * tmp;
    m[0][1] =  0.0f;
    m[0][2] =  (r + l) * tmp;
    m[0][3] =  0.0f;

    tmp     =  1.0f / (t - b);
    m[1][0] =  0.0f;
    m[1][1] =  (2*n) * tmp;
    m[1][2] =  (t + b) * tmp;
    m[1][3] =  0.0f;

    m[2][0] =  0.0f;
    m[2][1] =  0.0f;

    tmp = 1.0f / (f - n);

    m[2][2] = f * tmp;
    m[2][3] = f * n * tmp;

    m[3][0] =  0.0f;
    m[3][1] =  0.0f;
    m[3][2] = -1.0f;
    m[3][3] =  0.0f;

    return pOut;
}

inline MTX44* MTX44PivotC_FAST(MTX44* pOut, PivotDirection pivot)
{
    f32 (*const m)[4] = pOut->m;
    if ((pivot == PIVOT_NONE) || (pivot == PIVOT_UPSIDE_TO_LEFT))
    {
        return pOut;
    }

    if (pivot == PIVOT_UPSIDE_TO_RIGHT)
    {
        register f32 m00, m01, m02, m03, m10, m11, m12, m13;

        m00 = -m[0][0];
        m01 = -m[0][1];
        m02 = -m[0][2];
        m03 = -m[0][3];

        m10 = -m[1][0];
        m11 = -m[1][1];
        m12 = -m[1][2];
        m13 = -m[1][3];

        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;

        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
    }
    else if (pivot == PIVOT_UPSIDE_TO_BOTTOM)
    {
        register f32 m00, m01, m02, m03, m10, m11, m12, m13;

        m10 = m[0][0];
        m11 = m[0][1];
        m12 = m[0][2];
        m13 = m[0][3];

        m00 = -m[1][0];
        m01 = -m[1][1];
        m02 = -m[1][2];
        m03 = -m[1][3];

        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;

        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
    }
    else
    {
        register f32 m00, m01, m02, m03, m10, m11, m12, m13;

        m10 = -m[0][0];
        m11 = -m[0][1];
        m12 = -m[0][2];
        m13 = -m[0][3];

        m00 = m[1][0];
        m01 = m[1][1];
        m02 = m[1][2];
        m03 = m[1][3];

        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;

        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
    }
    return pOut;
}
inline MTX44* MTX44PivotC(MTX44* pOut, PivotDirection pivot)
{
    const f32 PIVOT_ROTATION_SIN_COS[PIVOT_NUM][2] =
    {
        { 0.0f,  1.0f }, // NONE
        { -1.0f, 0.0f }, // TO_UP
        { 0.0f, -1.0f }, // TO_RIGHT
        { 1.0f,  0.0f }, // TO_BOTTOM
        { 0.0f,  1.0f }, // TO_LEFT
    };

    if (pivot == PIVOT_NONE)
    {
        return pOut;
    }

    f32 sin = PIVOT_ROTATION_SIN_COS[ pivot ][ 0 ];
    f32 cos = PIVOT_ROTATION_SIN_COS[ pivot ][ 1 ];

    f32 (*const m)[4] = pOut->m;

    if (sin == 0.0f)
    {
        m[0][0] = cos * m[0][0];
        m[0][1] = cos * m[0][1];
        m[0][2] = cos * m[0][2];
        m[0][3] = cos * m[0][3];

        m[1][0] = cos * m[1][0];
        m[1][1] = cos * m[1][1];
        m[1][2] = cos * m[1][2];
        m[1][3] = cos * m[1][3];
    }
    else
    {
        f32 tmp = m[0][0];
        m[0][0] = -sin * m[1][0];
        m[1][0] = sin * tmp;

        tmp = m[0][1];
        m[0][1] = -sin * m[1][1];
        m[1][1] = sin * tmp;

        tmp = m[0][2];
        m[0][2] = -sin * m[1][2];
        m[1][2] = sin * tmp;

        tmp = m[0][3];
        m[0][3] = -sin * m[1][3];
        m[1][3] = sin * tmp;
    }

    return pOut;
}

inline MTX44* MTX44PerspectiveRadC(MTX44* pOut, f32 fovy, f32 aspect, f32 n, f32 f)
{
    NN_NULL_ASSERT_(pOut);

    f32 (*const m)[4] = pOut->m;

    const f32 angle = fovy * 0.5f;

    const f32 cot = 1.0f / ::std::tanf(angle);

    m[0][0] =  cot / aspect;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = cot;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;

    const f32 tmp = 1.0f / (f - n);
    m[2][2] = f * tmp;
    m[2][3] = f * n * tmp;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = -1.0f;
    m[3][3] = 0.0f;

    return pOut;
}
inline MTX44* MTX44PerspectiveRadC_FAST(MTX44* pOut, f32 fovy, f32 aspect, f32 n, f32 f)
{
    NN_NULL_ASSERT_(pOut);

    f32 (*const m)[4] = pOut->m;

    const f32 angle = fovy * 0.5f;

    const f32 cot = 1.0f / ::std::tanf(angle);

    const f32 tmp = 1.0f / (f - n);

    register f32 m00, m11, m22, m23;

    m00 =  cot / aspect;
    m11 =  cot;
    m22 = f * tmp;
    m23 = f * n * tmp;

    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = -1.0f;
    m[3][3] = 0.0f;

    m[0][0] = m00;
    m[1][1] = m11;
    m[2][2] = m22;
    m[2][3] = m23;


    return pOut;
}

MTX44* MTX44CopyAsm(MTX44* pOut, const MTX44* p);
inline MTX44* MTX44CopyC(MTX44* pOut, const MTX44* p)
{
    if (pOut != p)
    {
        *pOut = *p;
    }

    return pOut;
}

}

inline u32 MTX44Inverse(MTX44* pOut, const MTX44* p)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6::MTX44InverseC(pOut,p);
    #else
        return ARMv6::MTX44InverseC_FAST(pOut,p);
    #endif
}
inline u32 MTX44Inverse(MTX44* pOut, const MTX44& m) { return MTX44Inverse(pOut, &m); }

inline MTX44* MTX44Mult(MTX44* pOut, const MTX44* __restrict p1, const MTX44* __restrict p2)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6:MTX44MultC(pOut, p1, p2);
    #else
        return ARMv6::MTX44MultAsm(pOut, p1, p2);
    #endif
}

inline MTX44* MTX44Mult(MTX44* pOut, const MTX44* __restrict p1, f32 f)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6:MTX44MultC(pOut, p1, f);
    #else
        ARMv6::MTX44MultAsm(pOut, p1, f);
    #endif
}

inline MTX44* MTX44Copy(MTX44* pOut, const MTX44* p)
{
    #ifdef NN_MATH_BUILD_FAST
        return ARMv6::MTX44CopyC(pOut,p);
    #else
        return ARMv6::MTX44CopyAsm(pOut,p);
    #endif
}

inline MTX44* MTX44FrustumPivot(MTX44* pOut, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, PivotDirection pivot = PIVOT_NONE)
{
    #ifdef NN_MATH_BUILD_FAST
        ARMv6::MTX44FrustumC(pOut, l, r, b, t, n, f);
        ARMv6::MTX44PivotC(pOut, pivot);
        return pOut;
    #else
        ARMv6::MTX44FrustumC_FAST(pOut, l, r, b, t, n, f);
        ARMv6::MTX44PivotC_FAST(pOut, pivot);
        return pOut;
    #endif
}

inline MTX44* MTX44PerspectivePivotRad(MTX44* pOut, f32 fovy, f32 aspect, f32 n, f32 f, PivotDirection pivot = PIVOT_NONE)
{
    #ifdef NN_MATH_BUILD_FAST
        ARMv6::MTX44PerspectiveRadC(pOut, fovy, aspect, n, f);
        ARMv6::MTX44PivotC(pOut, pivot);
        return pOut;
    #else
        ARMv6::MTX44PerspectiveRadC_FAST(pOut, fovy, aspect, n, f);
        ARMv6::MTX44PivotC_FAST(pOut, pivot);
        return pOut;
    #endif
}
}
}

