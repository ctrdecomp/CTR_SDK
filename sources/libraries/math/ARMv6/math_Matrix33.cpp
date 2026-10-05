// Filename: math_Matrix33.cpp
//
// Project: Horizon

#include <nn/math/math_Matrix34.h>

namespace nn {
namespace math {
namespace ARMv6 {

asm MTX33* MTX33CopyAsm(MTX33*, const MTX33*)
{
    VLDMIA      r1!,{s0-s4}
    MOV         r2, r0
    VLDMIA      r1,{s5-s8}          
    VSTMIA      r2!,{s0-s4}
    VSTMIA      r2,{s5-s8}
    BX          lr
}

asm VEC3* VEC3TransformAsm(VEC3*, const MTX33*, const VEC3*)
{
    VLDMIA      r1,{s0-s8}
    VLDMIA      r2,{s9-s11}

    VMUL.F32    s12,s0,s9
    VMUL.F32    s13,s3,s9
    VMUL.F32    s14,s6,s9

    VMLA.F32    s12,s1,s10
    VMLA.F32    s13,s4,s10
    VMLA.F32    s14,s7,s10

    VMLA.F32    s12,s2,s11
    VMLA.F32    s13,s5,s11
    VMLA.F32    s14,s8,s11

    VSTMIA      r0,{s12-s14}

    BX          lr
}

} // namespace ARMv6
} // namespace math
} // namespace nn