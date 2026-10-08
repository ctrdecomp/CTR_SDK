#include <nn/math.h>

#include <nn/math/math_Arithmetic.h>
#include <cstdlib>

namespace nn {
namespace math {
namespace ARMv6 {

    asm f32 HermiteAsm(f32, f32, f32 ,f32 ,f32 , f32)
    {
        VDIV.F32    s6,s4,s5
        VLDR.F32    s7,%F1
        VLDR.F32    s8,%F2
        VLDR.F32    s9,%F3
        VSUB.F32    s10,s0,s2
        VSUB.F32    s11,s6,s7
        VMUL.F32    s12,s6,s6
        VNMLS.F32   s9,s6,s8
        VMUL.F32    s13,s6,s3
        VMUL.F32    s14,s4,s11
        VMUL.F32    s15,s10,s9
        VMLA.F32    s13,s11,s1
        VMLA.F32    s0,s15,s12
        VMLA.F32    s0,s13,s14
        BX          lr
1
        DCD         0x3F800000
2
        DCD         0x40000000
3
        DCD         0x40400000
    }

}  // namespace ARMv6
}  // namespace math
}  // namespace nn
