// Filename: math_Matrix44.cpp
//
// Project: Horizon

#include <nn/math/math_Matrix44.h>

namespace nn { 
namespace math { 
namespace ARMv6 {

asm MTX44* MTX44CopyAsm(MTX44* , const MTX44* )
{
    CMP         r1,r0
    BXEQ        lr
    VLDMIA      r1,{s0-s15}
    VSTMIA      r0,{s0-s15}
    BX          lr
}

asm MTX44* MTX44MultAsm(MTX44* , const MTX44* , const MTX44* )
{
    VPUSH       {d8-d12}

    VLDMIA      r2!,{s16-s23}
    VLDR.F32    s24,[r1,#16*0+0*4]  // p1[0][0]
    VLDR.F32    s25,[r1,#16*1+0*4]  // p1[1][0]

    VMUL.F32    s0,s24,s16
    VMUL.F32    s1,s24,s17
    VMUL.F32    s2,s24,s18
    VMUL.F32    s3,s24,s19
    VLDR.F32    s24,[r1,#16*2+0*4]  // p1[2][0]

    VMUL.F32    s4,s25,s16
    VMUL.F32    s5,s25,s17
    VMUL.F32    s6,s25,s18
    VMUL.F32    s7,s25,s19
    VLDR.F32    s25,[r1,#16*3+0*4]  // p1[3][0]

    VMUL.F32    s8,s24,s16
    VMUL.F32    s9,s24,s17
    VMUL.F32    s10,s24,s18
    VMUL.F32    s11,s24,s19
    VLDR.F32    s24,[r1,#16*0+1*4]  // p1[0][1]

    VMUL.F32    s12,s25,s16
    VMUL.F32    s13,s25,s17
    VMUL.F32    s14,s25,s18
    VMUL.F32    s15,s25,s19
    VLDR.F32    s25,[r1,#16*1+1*4]  // p1[1][1]

    VLDMIA      r2!,{s16-s19}

    VMLA.F32    s0,s24,s20
    VMLA.F32    s1,s24,s21
    VMLA.F32    s2,s24,s22
    VMLA.F32    s3,s24,s23
    VLDR.F32    s24,[r1,#16*2+1*4]  // p1[2][1]

    VMLA.F32    s4,s25,s20
    VMLA.F32    s5,s25,s21
    VMLA.F32    s6,s25,s22
    VMLA.F32    s7,s25,s23
    VLDR.F32    s25,[r1,#16*3+1*4]  // p1[3][1]

    VMLA.F32    s8,s24,s20
    VMLA.F32    s9,s24,s21
    VMLA.F32    s10,s24,s22
    VMLA.F32    s11,s24,s23
    VLDR.F32    s24,[r1,#16*0+2*4]  // p1[0][2]

    VMLA.F32    s12,s25,s20
    VMLA.F32    s13,s25,s21
    VMLA.F32    s14,s25,s22
    VMLA.F32    s15,s25,s23
    VLDR.F32    s25,[r1,#16*1+2*4]  // p1[1][2]

    VLDMIA      r2,{s20-s23}

    VMLA.F32    s0,s24,s16
    VMLA.F32    s1,s24,s17
    VMLA.F32    s2,s24,s18
    VMLA.F32    s3,s24,s19
    VLDR.F32    s24,[r1,#16*2+2*4]  // p1[2][2]

    VMLA.F32    s4,s25,s16
    VMLA.F32    s5,s25,s17
    VMLA.F32    s6,s25,s18
    VMLA.F32    s7,s25,s19
    VLDR.F32    s25,[r1,#16*3+2*4]  // p1[3][2]

    VMLA.F32    s8,s24,s16
    VMLA.F32    s9,s24,s17
    VMLA.F32    s10,s24,s18
    VMLA.F32    s11,s24,s19
    VLDR.F32    s24,[r1,#16*0+3*4]  // p1[0][3]

    VMLA.F32    s12,s25,s16
    VMLA.F32    s13,s25,s17
    VMLA.F32    s14,s25,s18
    VMLA.F32    s15,s25,s19
    VLDR.F32    s25,[r1,#16*1+3*4]  // p1[1][3]

    VMLA.F32    s0,s24,s20
    VMLA.F32    s1,s24,s21
    VMLA.F32    s2,s24,s22
    VMLA.F32    s3,s24,s23
    VLDR.F32    s24,[r1,#16*2+3*4]  // p1[2][3]

    VMLA.F32    s4,s25,s20
    VMLA.F32    s5,s25,s21
    VMLA.F32    s6,s25,s22
    VMLA.F32    s7,s25,s23
    VLDR.F32    s25,[r1,#16*3+3*4]  // p1[3][3]

    VMLA.F32    s8,s24,s20
    VMLA.F32    s9,s24,s21
    VMLA.F32    s10,s24,s22
    VMLA.F32    s11,s24,s23

    VMLA.F32    s12,s25,s20
    VMLA.F32    s13,s25,s21
    VMLA.F32    s14,s25,s22
    VMLA.F32    s15,s25,s23

    VPOP        {d8-d12}
    VSTMIA      r0,{s0-s15}
    BX          lr

}

asm MTX44* MTX44MultAsm(MTX44* , const MTX44* , f32 )
{
    VPUSH       {d8}
    VLDMIA      r1!,{s16}
    VLDMIA      r1,{s1-s15}

    VMUL.F32    s1,s1,s0
    VMUL.F32    s2,s2,s0
    VMUL.F32    s3,s3,s0

    VMUL.F32    s4,s4,s0
    VMUL.F32    s5,s5,s0
    VMUL.F32    s6,s6,s0
    VMUL.F32    s7,s7,s0

    VMUL.F32    s8,s8,s0
    VMUL.F32    s9,s9,s0
    VMUL.F32    s10,s10,s0
    VMUL.F32    s11,s11,s0

    VMUL.F32    s12,s12,s0
    VMUL.F32    s13,s13,s0
    VMUL.F32    s14,s14,s0
    VMUL.F32    s15,s15,s0

    VMUL.F32    s0,s16,s0
    VPOP        {d8}
    VSTMIA      r0,{s0-s15}
    BX          lr
}

asm MTX44* MTX44MultTranslateAsm(MTX44* , const VEC3*, const MTX44*)
{
    VLDMIA      r2,{s0-s11}
    VLDMIA      r1,{s12-s14}
    
    VADD.F32    s3,s3,s12
    VADD.F32    s7,s7,s13
    VADD.F32    s11,s11,s14

    VSTMIA      r0,{s0-s11}
    BX          lr
}

asm MTX44* MTX44MultScaleAsm(MTX44*, const VEC3*, const MTX44*)
{
    VLDMIA      r2,{s0-s11}
    VLDMIA      r1,{s12-s14}

    VMUL.F32    s0,s0,s12
    VMUL.F32    s1,s1,s12
    VMUL.F32    s2,s2,s12
    VMUL.F32    s3,s3,s12

    VMUL.F32    s4,s4,s13
    VMUL.F32    s5,s5,s13
    VMUL.F32    s6,s6,s13
    VMUL.F32    s7,s7,s13

    VMUL.F32    s8,s8,s14
    VMUL.F32    s9,s9,s14
    VMUL.F32    s10,s10,s14
    VMUL.F32    s11,s11,s14

    VSTMIA      r0,{s0-s11}
    BX          lr
}

} // namespace ARMv6
} // namespace math
} // namespace nn