#include <stddef.h>

#pragma arm

#define NN_ASM __asm

#ifndef NN_IS_UNUSED_VAR
#define NN_IS_UNUSED_VAR
#endif

#define ASM_STRUCTURE_MEMBER_OFFSET_OF(s,m) __cpp(offsetof(s,m))

#define ASM_OP_LOG_OR :OR:

       void JpegMpEncoderAsm_ConvertWorkToAsm(register JpegMpEncoderContext* pCtx);
NN_ASM void JpegMpEncoderAsm_ConvertWorkToAsm(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx r0
#define t1       r1
#define t2       r2
#define t3       r3
#define t0       r12

	ldr	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pDst)]
	ldr	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, dstOffset)]
	ldr	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, writeLimit)]
	ldr	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, bitCode)]

	add	t1, t0, t1
	add	t2, t0, t2

	ldrb	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, bitLen)]
	sub	t2, t2, #1
	str	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.pDst2)]
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.pDst2Limit)]

	mov	t3, t3, lsl #24
	str	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitCode2)]
	add	t0, t0, #24
	str	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)]

	bx	lr

#undef arg_pCtx
#undef t1
#undef t2
#undef t3
#undef t0
}

       void JpegMpEncoderAsm_ConvertWorkToC(register JpegMpEncoderContext* pCtx);
NN_ASM void JpegMpEncoderAsm_ConvertWorkToC(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx r0
#define t1       r1
#define t2       r2
#define t3       r3
#define t0       r12

	ldr	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pDst)]
	ldr	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.pDst2)]
	ldr	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitCode2)]
	ldr	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)]

	sub	t0, t1, t0
	str	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, dstOffset)]

	mov	t2, t2, lsr #24
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, bitCode)]
	sub	t3, t3, #24

	strb	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, bitLen)]

	bx	lr

#undef arg_pCtx
#undef t1
#undef t2
#undef t3
#undef t0
}

       bool JpegMpEncoderAsm_ComponentSequential(register JpegMpEncoderContext* pCtx, register JpegMpEncoderComponentStructure* component);
NN_ASM bool JpegMpEncoderAsm_ComponentSequential(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register JpegMpEncoderComponentStructure* component NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_pComponent r1

#define pWork          r2

#define du             r3
#define pHufsi         r4
#define pHufco         r5

#define bitLen2_       du
#define bitCode2_      pHufsi
#define pDst2_         pHufco

#define t0             r6
#define t1             r7
#define t2             r8
#define t3             r9
#define t4             arg_pComponent

#define value          r10
#define pZig           r11
#define zerorun        r12

#define rrrrssss       lr

#define PROLOGUE_INST         push {r1, r4-r11, lr}
#define EPILOGUE_INST         pop  {r1, r4-r11, pc}

	ldr	r12, [arg_pComponent, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mDcTable)]

	PROLOGUE_INST

	add	pWork, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)

	add	du, arg_pComponent, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mCoefficients)

	add	pHufsi, r12, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufsi)
	add	pHufco, r12, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufco)

	ldr	t2, [arg_pComponent, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mLastDcValue)]
	ldrsh	t1, [du]

	subs	t2, t1, t2

	str	t1, [arg_pComponent, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mLastDcValue)]

	rsbmi	t2, t2, #0

	clz	t3, t2
	rsb	t3, t3, #32

	mvnmi	t2, t2

	ldr	t4, [arg_pComponent, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mAcTable)]

	ldrb	t1, [pHufsi, t3]
	ldr	t0, [pHufco, t3, lsl #2]

	add	pHufsi, t4, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufsi)
	add	pHufco, t4, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufco)
	stmdb	pWork, {du, pHufsi, pHufco}

	mvn	t4, #0

	bic	t0, t0, t4, lsl t1
	ldmia	pWork, {bitLen2_, bitCode2_, pDst2_}
	bic	t2, t2, t4, lsl t3
	bl	bitWrite

	ldrsh	value, [du, #1 * 2]

	adr	pZig, zigTable

	mov	zerorun, #0

	cmp	value, #0
	bne	zerorunDone
	b	asm_sequential_zero

	cmp	zerorun, #16
	blt	zerorunDone

zerorunLoop
	ldrb	t1, [pHufsi, #0xF0]
	ldr	t0, [pHufco, #0xF0 * 4]
	ldmia	pWork, {bitLen2_, bitCode2_, pDst2_}
	mvn	t4, #0
	mov	t3, #0
	bic	t0, t0, t4, lsl t1
	bl	bitWrite

	cmp	zerorun, #32
	sub	zerorun, zerorun, #16
	bge	zerorunLoop

zerorunDone

	movs	t2, value
	rsbmi	t2, t2, #0

	clz	t3, t2
	rsb	t3, t3, #32

	orr	rrrrssss, t3, zerorun, lsl #4

	mvnmi	t2, t2

	ldrb	t1, [pHufsi, rrrrssss]
	ldr	t0, [pHufco, rrrrssss, lsl #2]

	mvn	t4, #0
	ldmia	pWork, {bitLen2_, bitCode2_, pDst2_}
	bic	t0, t0, t4, lsl t1
	bic	t2, t2, t4, lsl t3

	ldrb	t4, [pZig, #1]!

	bl	bitWrite

	ldrsh	value, [du, t4]
	cmp	t4, #0
	beq	asm_sequential_end

	cmp	value, #0
	mov	zerorun, #0
	bne	zerorunDone

asm_sequential_zero

	ldrb	t4, [pZig, #1]!
	add	zerorun, zerorun, #1

	cmp	t4, #0
	beq	asm_sequential_end_zerorun

	ldrsh	value, [du, t4]

	cmp	value, #0

	beq	asm_sequential_zero

	cmp	zerorun, #16
	blt	zerorunDone
	b	zerorunLoop

asm_sequential_end_zerorun
	ldrb	t1, [pHufsi]
	ldr	t0, [pHufco]
	mvn	t4, #0
	ldmia	pWork, {bitLen2_, bitCode2_, pDst2_}
	bic	t0, t0, t4, lsl t1
	mov	t3, #0
	bl	bitWrite

asm_sequential_end
	mov	r0, #1
	EPILOGUE_INST

bitWriteError
	mov	r0, #0
	EPILOGUE_INST

zigTable
	dcb	       1*2,  8*2, 16*2,  9*2,  2*2,  3*2, 10*2
	dcb	17*2, 24*2, 32*2, 25*2, 18*2, 11*2,  4*2,  5*2
	dcb	12*2, 19*2, 26*2, 33*2, 40*2, 48*2, 41*2, 34*2
	dcb	27*2, 20*2, 13*2,  6*2,  7*2, 14*2, 21*2, 28*2
	dcb	35*2, 42*2, 49*2, 56*2, 57*2, 50*2, 43*2, 36*2
	dcb	29*2, 22*2, 15*2, 23*2, 30*2, 37*2, 44*2, 51*2
	dcb	58*2, 59*2, 52*2, 45*2, 38*2, 31*2, 39*2, 46*2
	dcb	53*2, 60*2, 61*2, 54*2, 47*2, 55*2, 62*2, 63*2
	dcb	 0

bitWriteCont
	mov	t0, t2
	mov	t3, #0
bitWrite
	sub	bitLen2_, bitLen2_, t1
	cmp	bitLen2_, #24
	orr	bitCode2_, bitCode2_, t0, lsl bitLen2_
	bgt	done
	ldr	t1, [pWork, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.pDst2Limit)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)]
	mov	t0, bitCode2_, lsr #24
	mov	bitCode2_, bitCode2_, lsl #8
	strb	t0, [pDst2_], #1
	cmp	pDst2_, t1
	add	bitLen2_, bitLen2_, #8
	bcc	next

	b	bitWriteError
next
	cmp	t0, #0xff
	streqb	bitCode2_, [pDst2_], #1
	cmp	bitLen2_, #24
	bgt	done2

	mov	t0, bitCode2_, lsr #24
	mov	bitCode2_, bitCode2_, lsl #8
	strb	t0, [pDst2_], #1
	cmp	pDst2_, t1
	add	bitLen2_, bitLen2_, #8
	bcc	next2

	b	bitWriteError
next2
	cmp	t0, #0xff
	streqb	bitCode2_, [pDst2_], #1
done2
done
	movs	t1, t3
	stmia	pWork, {bitLen2_, bitCode2_, pDst2_}
	bne	bitWriteCont
	ldmdb	pWork, {du, pHufsi, pHufco}
	bx	lr

#undef arg_pCtx
#undef arg_pComponent

#undef pWork
#undef du
#undef pHufsi
#undef pHufco

#undef bitLen2_
#undef bitCode2_
#undef pDst2_

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4

#undef value
#undef pZig
#undef zerorun

#undef rrrrssss

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

#if 1

       void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx          r0
#define arg_col           r1
#define arg_row           r2

#define pDstY             arg_pCtx
#define pDstCbCr          arg_col
#define t9                arg_row
#define pYuyv             r3

#define t0                r4
#define t1                r5
#define t2                r6
#define t3                r7
#define t4                r8
#define t5                r9
#define t6                r10
#define t7                r11
#define width_            r12
#define t8                lr

#define PROLOGUE_INST     push {r4-r11, r12, lr}
#define EPILOGUE_INST     pop  {r4-r11, r12, pc}
#define STACK_OFFSET_PDSTYEND       0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	width_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	pYuyv, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mov	arg_row, arg_row, lsl #3 + 1
	add	pYuyv, pYuyv, arg_col, lsl #3 + 1

	mla	pYuyv, arg_row, width_, pYuyv

	mov	width_, width_, lsl #1

	add	t0, pDstY, #8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	t0, [sp, #STACK_OFFSET_PDSTYEND]

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

10

	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_
	ldr	t7, [pYuyv], width_
	ldr	t6, [pYuyv], width_
	ldr	t2, [pYuyv], width_
	ldr	t0, [pYuyv], width_

	pkhbt	t3, t6, t7, lsl #16
	pkhtb	t7, t7, t6, asr #16
	pkhtb	t6, t0, t2, asr #16
	pkhbt	t2, t2, t0, lsl #16

	uxtb16	t0, t6, ror #8
	uxtb16	t1, t7, ror #8

	strd	t0, t1, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	strd	t0, t1, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + (8 + 4) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t0, t2, ror #8
	uxtb16	t1, t3, ror #8

	strd	t0, t1, [pDstCbCr, #4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	strd	t0, t1, [pDstCbCr, #(8 + 4) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhtb	t0, t5, t4, asr #16
	pkhbt	t1, t4, t5, lsl #16

	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_

	uxtb16	t2, t2
	uxtb16	t3, t3

	pkhbt	t8, t5, t4, lsl #16
	pkhtb	t9, t4, t5, asr #16

	uxtb16	t4, t0, ror #8
	uxtb16	t5, t9, ror #8

	strd	t4, t5, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strd	t4, t5, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t4, t1, ror #8
	uxtb16	t5, t8, ror #8

	strd	t4, t5, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	strd	t4, t5, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	uxtb16	t4, t0
	uxtb16	t0, t1
	uxtb16	t1, t8

	ldr	t8, [sp, #STACK_OFFSET_PDSTYEND]

	uxtb16	t5, t9
	uxtb16	t6, t6
	uxtb16	t7, t7

	stmia	pDstY!, {t0, t1, t2, t3, t4, t5, t6, t7}

	add	pYuyv, pYuyv, #4
	sub	pYuyv, pYuyv, width_, lsl #3
	cmp	pDstY, t8
	pld	[pYuyv, #0x20]

	bne	%b10

	EPILOGUE_INST

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pDstY
#undef pDstCbCr
#undef t9
#undef pYuyv

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef width_
#undef t8

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDSTYEND

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}
#else

       void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx          r0
#define arg_col           r1
#define arg_row           r2

#define pDstY             arg_pCtx
#define pDstCbCr          arg_col
#define y2                arg_row
#define pYuyv             r3

#define t0                r4
#define t1                r5
#define t2                r6
#define t3                r7
#define t4                r8
#define t5                r9
#define t6                r10
#define t7                r11
#define width_            r12
#define t8                lr

#define PROLOGUE_INST     stmfd sp!, {r4-r11, r12, lr}
#define EPILOGUE_INST     ldmfd sp!, {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	width_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	pYuyv, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mov	arg_row, arg_row, lsl #3 + 1
	add	pYuyv, pYuyv, arg_col, lsl #3 + 1

	mla	pYuyv, arg_row, width_, pYuyv

	mov	width_, width_, lsl #1

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	y2, =0x16fe1ef0

10

	ldrb	t0, [pYuyv, #0x1]
	ldrb	t1, [pYuyv, #0x5]
	ldrb	t2, [pYuyv, #0x9]
	ldrb	t3, [pYuyv, #0xd]

	ldrb	t4, [pYuyv, #0x3]
	ldrb	t5, [pYuyv, #0x7]
	ldrb	t6, [pYuyv, #0xb]
	ldrb	t7, [pYuyv, #0xf]

	ands	t8, y2, #0xf0000000

	strb	t0, [pDstCbCr, #0x08 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t1, [pDstCbCr, #0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t1, [pDstCbCr, #0x18 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t2, [pDstCbCr, #0x20 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t2, [pDstCbCr, #0x28 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t3, [pDstCbCr, #0x30 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t3, [pDstCbCr, #0x38 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	strb	t4, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x00 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t4, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x08 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t5, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t5, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x18 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t6, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x20 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t6, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x28 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t7, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x30 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t7, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 0x38 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t0, [pDstCbCr], t8, asr #28 - 1

	mov	y2, y2, lsl #4

	ldrb	t1, [pYuyv, #0x2]
	ldrb	t2, [pYuyv, #0x4]
	ldrb	t3, [pYuyv, #0x6]
	ldrb	t4, [pYuyv, #0x8]
	ldrb	t5, [pYuyv, #0xa]
	ldrb	t6, [pYuyv, #0xc]
	ldrb	t7, [pYuyv, #0xe]
	ldrb	t0, [pYuyv], width_

	strb	t1, [pDstY, #0x08 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t2, [pDstY, #0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t3, [pDstY, #0x18 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t4, [pDstY, #0x20 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t5, [pDstY, #0x28 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t6, [pDstY, #0x30 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t7, [pDstY, #0x38 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strb	t0, [pDstY], t8, asr #28 - 1

	bne	%b10

	EPILOGUE_INST

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pDstY
#undef pDstCbCr
#undef y2
#undef pYuyv

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef width_
#undef t8

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}
#endif

       void JpegMpEncoderAsm_Yuyv8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx          r0
#define arg_col           r1
#define arg_row           r2

#define pDstY             arg_pCtx
#define pDstCbCr          arg_col
#define t9                arg_row
#define pYuyv             r3

#define t0                r4
#define t1                r5
#define t2                r6
#define t3                r7
#define t4                r8
#define t5                r9
#define t6                r10
#define t7                r11
#define width_            r12
#define t8                lr

#define PROLOGUE_INST     push {r4-r11, r12, lr}
#define EPILOGUE_INST     pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	width_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	pYuyv, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mov	arg_row, arg_row, lsl #4 + 1
	add	pYuyv, pYuyv, arg_col, lsl #4 + 1

	mla	pYuyv, arg_row, width_, pYuyv

	mov	width_, width_, lsl #1

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

10

	ldr	t2, [pYuyv], width_
	ldr	t3, [pYuyv], width_
	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_
	ldr	t6, [pYuyv], width_
	ldr	t7, [pYuyv], width_
	ldr	t8, [pYuyv], width_
	ldr	t9, [pYuyv], width_

	pkhbt	t0, t2, t3, lsl #16
	pkhtb	t1, t3, t2, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1

	str	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	str	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pkhbt	t0, t9, t8, lsl #16
	pkhtb	t1, t8, t9, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  2 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xa - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t6, t7, lsl #16
	pkhtb	t1, t7, t6, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  4 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xc - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t5, t4, lsl #16
	pkhtb	t1, t4, t5, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  6 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xe - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t2, t4, lsl #16
	pkhtb	t1, t4, t2, asr #16
	pkhbt	t2, t3, t5, lsl #16
	pkhtb	t3, t5, t3, asr #16
	pkhbt	t4, t8, t6, lsl #16
	pkhtb	t5, t6, t8, asr #16
	pkhbt	t6, t9, t7, lsl #16
	pkhtb	t7, t7, t9, asr #16

	uhadd8	t8, t4, t6
	uhadd8	t9, t5, t7
	uhadd8	t6, t0, t2
	uhadd8	t7, t1, t3

	uxtb16	t8, t8, ror #8

	str	t8, [pDstCbCr, #6 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	uxtb16	t9, t9, ror #8

	str	t9, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 6 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t7, t7, ror #8

	str	t7, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	uxtb16	t1, t6, ror #8

	ldr	t2, [pYuyv], width_
	ldr	t3, [pYuyv], width_
	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_
	ldr	t6, [pYuyv], width_
	ldr	t7, [pYuyv], width_
	ldr	t8, [pYuyv], width_
	ldr	t9, [pYuyv], width_

	str	t1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pkhbt	t0, t2, t3, lsl #16
	pkhtb	t1, t3, t2, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1

	str	t0, [pDstY, #(      0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(  8 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t9, t8, lsl #16
	pkhtb	t1, t8, t9, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  2 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xa + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t6, t7, lsl #16
	pkhtb	t1, t7, t6, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  4 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xc + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t5, t4, lsl #16
	pkhtb	t1, t4, t5, asr #16
	uxtb16	t0, t0
	uxtb16	t1, t1
	str	t0, [pDstY, #(  6 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	t1, [pDstY, #(0xe + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	t0, t2, t4, lsl #16
	pkhtb	t1, t4, t2, asr #16
	pkhbt	t2, t3, t5, lsl #16
	pkhtb	t3, t5, t3, asr #16
	pkhbt	t4, t8, t6, lsl #16
	pkhtb	t5, t6, t8, asr #16
	pkhbt	t6, t9, t7, lsl #16
	pkhtb	t7, t7, t9, asr #16

	uhadd8	t8, t4, t6
	uhadd8	t9, t5, t7
	uhadd8	t6, t0, t2
	uhadd8	t7, t1, t3

	uxtb16	t8, t8, ror #8

	str	t8, [pDstCbCr, #(2 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	uxtb16	t9, t9, ror #8

	str	t9, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + (2 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t7, t7, ror #8

	str	t7, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + (4 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t6, t6, ror #8

	str	t6, [pDstCbCr, #(4 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	sub	t0, pDstCbCr, pDstY
	add	pYuyv, pYuyv, #4
	sub	pYuyv, pYuyv, width_, lsl #4

	cmp	t0, #STRUCT_OFFSET_CBBUF - (8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	pld	[pYuyv, #0x20]
	bne	%b10

	EPILOGUE_INST

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pDstY
#undef pDstCbCr
#undef t9
#undef pYuyv

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef width_
#undef t8

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Yuyv8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx          r0
#define arg_col           r1
#define arg_row           r2

#define pDstY             arg_pCtx
#define pDstCbCr          arg_col
#define t9                arg_row
#define pYuyv             r3

#define t0                r4
#define t1                r5
#define t2                r6
#define t3                r7
#define t4                r8
#define t5                r9
#define t6                r10
#define t7                r11
#define width_            r12
#define t8                lr

#define PROLOGUE_INST     push {r4-r11, r12, lr}
#define EPILOGUE_INST     pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	width_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	pYuyv, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mov	arg_row, arg_row, lsl #3 + 1
	add	pYuyv, pYuyv, arg_col, lsl #4 + 1

	mla	pYuyv, arg_row, width_, pYuyv

	mov	width_, width_, lsl #1

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

10

	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_
	ldr	t7, [pYuyv], width_
	ldr	t6, [pYuyv], width_
	ldr	t2, [pYuyv], width_
	ldr	t0, [pYuyv], width_

	pkhbt	t3, t6, t7, lsl #16
	pkhtb	t7, t7, t6, asr #16
	pkhtb	t6, t0, t2, asr #16
	pkhbt	t2, t2, t0, lsl #16

	uxtb16	t0, t6, ror #8
	uxtb16	t1, t7, ror #8

	strd	t0, t1, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t0, t2, ror #8
	uxtb16	t1, t3, ror #8

	strd	t0, t1, [pDstCbCr, #4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhtb	t0, t5, t4, asr #16
	pkhbt	t1, t4, t5, lsl #16

	ldr	t4, [pYuyv], width_
	ldr	t5, [pYuyv], width_

	uxtb16	t2, t2
	uxtb16	t3, t3

	pkhbt	t8, t5, t4, lsl #16
	pkhtb	t9, t4, t5, asr #16

	uxtb16	t4, t0, ror #8
	uxtb16	t5, t9, ror #8

	strd	t4, t5, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	uxtb16	t4, t1, ror #8
	uxtb16	t5, t8, ror #8

	strd	t4, t5, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	uxtb16	t4, t0
	uxtb16	t0, t1
	uxtb16	t1, t8
	uxtb16	t5, t9
	uxtb16	t6, t6
	uxtb16	t7, t7

	stmia	pDstY!, {t0, t1, t2, t3, t4, t5, t6, t7}

	sub	t0, pDstCbCr, pDstY
	add	pYuyv, pYuyv, #4
	sub	pYuyv, pYuyv, width_, lsl #3

	cmp	t0, #STRUCT_OFFSET_CBBUF - (8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	pld	[pYuyv, #0x20]
	bne	%b10

	EPILOGUE_INST

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pDstY
#undef pDstCbCr
#undef t9
#undef pYuyv

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef width_
#undef t8

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row

#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x2)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #3 + 1

	add	t0, r12, #8 * 2

	mov	loopXY, #0x00000010

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenterCtrRgb565To444

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	t0, [pRgb], #4

	ldmib	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10

	and	g1r1, rgbMask, t0, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	b1b0, rgbMask, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, rgbMask, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	ldmia	pConst, {t1, rgbMask, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	ldrcc	t0, [pRgb], #4

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]
	sub	t1, loopXY, #0x1e

	subs	loopXY, loopXY, #0x00000002
	ldrnesh	t2, [pConst, t1]

	ldrne	t0, [pRgb, t0]!

	addne	pRgb, pRgb, #4
	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1

#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x30
#define STACK_OFFSET_PDSTCBCR       0x00

#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x2)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1

	add	t0, r12, #8 * 2

	mov	loopXY, #0x00000020

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenterCtrRgb565To420

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	g0r0, [pRgb, t0]

	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	push	{pDst, t0, t1, t2}

	mov	t0, g0r0

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	and	g1r1, rgbMask, t0, lsr #3

	and	t1,   rgbMask, rgbMask, lsl #1
	and	b1b0, t1, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, t1, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldr	t2, [pRgb], #4

	add	g0r0, g0r0, g1r1

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	and	g1r1, rgbMask, t2, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	t1,   rgbMask, t2, lsr #8
	and	b3b2, rgbMask, t2, lsl #3

	pkhbt	t2,   t1,   g1r1, lsl #16
	pkhtb	g1r1, g1r1, t1,   asr #16

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	usat	t1, #8, t1, asr #14
	ldrcc	t0, [pRgb, t0]
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	t2, [sp, #STACK_OFFSET_WIDTH]

	subs	loopXY, loopXY, #0x00000004

	EPILOGUE_EQ_INST

	sub	t1, loopXY, #0x2c

	add	pRgb, pRgb, t2, lsl #1

	ldr	t1, [pConst, t1]
	ldr	t2, [sp, #STACK_OFFSET_PDSTY0]

	ldr	t0, [pRgb, t0]

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565To420
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTH
#undef STACK_OFFSET_PDSTCBCR

#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x2)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1

	add	t0, r12, #16 * 2

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenterCtrRgb565To422

	mov	loopXY, #0x00000010

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	t0, [pRgb], #4

	ldmib	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10

	and	g1r1, rgbMask, t0, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	b1b0, rgbMask, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, rgbMask, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0
	ldmia	pConst, {t1, rgbMask, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	ldrcc	t0, [pRgb], #4

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]

	ldrsh	pDstCbCr, [t1, loopXY]

	subs	loopXY, loopXY, #0x00000002

	ldrne	t0, [pRgb, t0]!

	addne	pRgb, pRgb, #4
	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTblCtrRgb565To422
	dcw	0
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	0

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565To422
	dcd	offsetTblCtrRgb565To422
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #3 + 1

	add	pRgb, pRgb, arg_col, lsl #3 + 1 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgb565Block8To444

	mov	loopXY, #0x00000008

	ldr	t0, [pRgb]

	ldmib	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10

	and	g1r1, rgbMask, t0, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	b1b0, rgbMask, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, rgbMask, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	adr	t0, srcOffsetTblCtrRgb565Block8To444 - 4

	ldmia	pConst, {t1, rgbMask, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	ldrb	t0, [t0, loopXY, ror #30]

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	teq	t0, #0x00
	ldrne	t0, [pRgb, t0]

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001
	ldrnesh	t2, [t1, #-0x1e]

	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565Block8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

srcOffsetTblCtrRgb565Block8To444
	dcd	0x003e3a2e*2
	dcd	0x2a3c382c*2
	dcd	0x28363226*2
	dcd	0x22343024*2
	dcd	0x201e1a0e*2
	dcd	0x0a1c180c*2
	dcd	0x08161206*2
	dcd	0x02141004*2

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTHx8        0x30
#define STACK_OFFSET_PDSTCBCR       0x00
#define STACK_OFFSET_PRGB           0x04
#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #4 + 1

	add	pRgb, pRgb, arg_col, lsl #4 + 1 + 3

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	mov	loopXY, #0x00000008

	adr	pConst, constTblCenterCtrRgb565Block8To420

	add	r12, pRgb, r12, lsl #3 + 1
	str	r12, [sp, #STACK_OFFSET_WIDTHx8 - 0x10]

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	g0r0, [pRgb, #0x02*2]!

	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	sub	t0, pRgb, #0x02*2

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{pDst, t0, t1, t2}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	mov	t0, g0r0

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	and	g1r1, rgbMask, t0, lsr #3

	and	t1,   rgbMask, rgbMask, lsl #1
	and	b1b0, t1, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, t1, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldr	t2, [pRgb, #-0x02*2]

	adr	pRgb, srcOffsetTblCtrRgb565Block8To420 - 8

	add	g0r0, g0r0, g1r1

	ldrb	pRgb, [pRgb, loopXY, ror #29]

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	and	g1r1, rgbMask, t2, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	t1,   rgbMask, t2, lsr #8
	and	b3b2, rgbMask, t2, lsl #3

	pkhbt	t2,   t1,   g1r1, lsl #16
	pkhtb	g1r1, g1r1, t1,   asr #16

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	teq	pRgb, #0x00
	ldrne	t0, [pRgb, t0]!

	usat	t1, #8, t1, asr #14
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	loopXY, loopXY, #0x00000001

	addne	t1, pConst, loopXY, lsl #2
	ldrne	t2, [sp, #STACK_OFFSET_PDSTY0]
	ldrne	t1, [t1, #-0x2c]

	EPILOGUE_EQ_INST

	cmp	loopXY, #0x00000004

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	bne	%b10

	ldr	pRgb, [sp, #STACK_OFFSET_WIDTHx8]

	str	pRgb, [sp, #STACK_OFFSET_PRGB]
	ldr	t0, [pRgb, #0x02*2]!

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565Block8To420
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

srcOffsetTblCtrRgb565Block8To420
	dcd	0x6a3e3a2e*2,0x007e7a6e*2
	dcd	0x62363226*2,0x2a767266*2
	dcd	0x4a1e1a0e*2,0x225e5a4e*2
	dcd	0x42161206*2,0x0a565246*2
	dcd	0x6a3e3a2e*2,0x007e7a6e*2
	dcd	0x62363226*2,0x2a767266*2
	dcd	0x4a1e1a0e*2,0x225e5a4e*2
	dcd	0x42161206*2,0x0a565246*2

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTHx8
#undef STACK_OFFSET_PDSTCBCR
#undef STACK_OFFSET_PRGB
#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #3 + 1

	add	pRgb, pRgb, arg_col, lsl #4 + 1 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgb565Block8To422

	mov	loopXY, #0x00000008

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	t0, [pRgb]

	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10

	and	g1r1, rgbMask, t0, lsr #3
	and	rgbMask, rgbMask, rgbMask, lsl #1
	and	b1b0, rgbMask, t0, lsr #8

	pkhbt	g0r0, b1b0, g1r1, lsl #16
	pkhtb	g1r1, g1r1, b1b0, asr #16
	and	b1b0, rgbMask, t0, lsl #3

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	t0, srcOffsetTblCtrRgb565Block8To422 - 8

	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	t0, [t0, loopXY, ror #29]

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	ldmia	pConst, {rgbMask, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	teq	t0, #0x00
	ldrne	t0, [pRgb, t0]

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001

	ldrsh	pDstCbCr, [t1, #-0x1e]

	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTblCtrRgb565Block8To422
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef565G2cbMul << 16) | (s_Coef565R2cbMul & 0xffff))
	dcd	__cpp(s_Coef565Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef565G2crMul << 16) | s_Coef565R2crMul_B2cbMul)
constTblCenterCtrRgb565Block8To422
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)

srcOffsetTblCtrRgb565Block8To422
	dcd	0x6a3e3a2e*2,0x007e7a6e*2
	dcd	0x683c382c*2,0x2a7c786c*2
	dcd	0x62363226*2,0x28767266*2
	dcd	0x60343024*2,0x22747064*2
	dcd	0x4a1e1a0e*2,0x205e5a4e*2
	dcd	0x481c180c*2,0x0a5c584c*2
	dcd	0x42161206*2,0x08565246*2
	dcd	0x40141004*2,0x02545044*2

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgb8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row

#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #3 + 1
	add	pRgb, pRgb, arg_col, lsl #3

	add	t0, r12, #8 * 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Rgb8To444

	mov	loopXY, #0x00000010

	ldr	t0, [pRgb], #2
	ldr	rgbMask, [pRgb], #4

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	pkhtb	b1b0, t0, rgbMask, asr #24
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1, ror #8
	uxtb16	b1b0, b1b0, ror #16

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	ldrcc	t0, [pRgb], #2
	ldrcc	rgbMask, [pRgb], #4

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]
	sub	t1, loopXY, #0x1e

	subs	loopXY, loopXY, #0x00000002
	ldrnesh	t2, [pConst, t1]

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #2]

	addne	pRgb, pRgb, #2 * 3
	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgb8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1

#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgb8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x30
#define STACK_OFFSET_PDSTCBCR       0x00

#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1
	add	pRgb, pRgb, arg_col, lsl #4

	add	t0, r12, #8 * 3

	adr	pConst, constTblCenter_Rgb8To420

	mla	pRgb, arg_row, t0, pRgb

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	loopXY, #0x00000020

	add	g1r1, pRgb, t0

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	rgbMask, [g1r1]
	ldr	g1r1, [g1r1, #2]

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	push	{pDst, t0, t1, t2}

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	pkhbt	g0r0, rgbMask, rgbMask, lsl #8
	pkhtb	b1b0, rgbMask, g1r1, asr #24
	pkhbt	g1r1, g1r1, g1r1, lsl #8
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1, ror #8
	uxtb16	b1b0, b1b0, ror #16

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldr	rgbMask, [pRgb], #2

	add	g0r0, g0r0, g1r1

	ldr	g1r1, [pRgb], #4

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhbt	t2, rgbMask, rgbMask, lsl #8
	pkhtb	b3b2, rgbMask, g1r1, asr #24
	pkhbt	g1r1, g1r1, g1r1, lsl #8
	uxtb16	t2, t2
	uxtb16	g1r1, g1r1, ror #8
	uxtb16	b3b2, b3b2, ror #16

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	usat	t1, #8, t1, asr #14
	ldrcc	rgbMask, [t0, pRgb]!
	usat	t2, #8, t2, asr #14
	ldrcc	g1r1, [t0, #2]

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	t2, [sp, #STACK_OFFSET_WIDTH]

	subs	loopXY, loopXY, #0x00000004

	EPILOGUE_EQ_INST

	sub	t1, loopXY, #0x2c

	add	pRgb, pRgb, t2, lsl #1

	ldr	t1, [pConst, t1]
	ldr	t2, [sp, #STACK_OFFSET_PDSTY0]

	ldr	rgbMask, [t0, pRgb]!

	and	t3, t1, #0xff
	ldr	g1r1, [t0, #2]
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgb8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTH
#undef STACK_OFFSET_PDSTCBCR

#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgb8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1
	add	pRgb, pRgb, arg_col, lsl #4

	add	t0, r12, #16 * 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Rgb8To422

	mov	loopXY, #0x00000010

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	t0, [pRgb], #2
	ldr	rgbMask, [pRgb], #4

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	pkhtb	b1b0, t0, rgbMask, asr #24
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1, ror #8
	uxtb16	b1b0, b1b0, ror #16

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0
	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	ldrcc	t0, [pRgb], #2
	ldrcc	rgbMask, [pRgb], #4

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]

	ldrsh	pDstCbCr, [t1, loopXY]

	subs	loopXY, loopXY, #0x00000002

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #2]

	addne	pRgb, pRgb, #2 * 3
	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTbl_Rgb8To422
	dcw	0
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	0

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgb8To422
	dcd	offsetTbl_Rgb8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb8Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	arg_row, arg_row, lsl #3

	add	pRgb, pRgb, arg_col, lsl #3 + 1 + 3
	add	pRgb, pRgb, arg_col, lsl #3 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgb8Block8To444

	mov	loopXY, #0x00000008

	ldr	rgbMask, [pRgb]
	ldr	t0, [pRgb, #2]

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, rgbMask, rgbMask, lsl #8
	pkhtb	g1r1, t0, t0, asr #8
	pkhbt	b1b0, rgbMask, t0, lsl #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	adr	t0, srcOffsetTblCtrRgb8Block8To444 - 4

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	ldrb	t0, [t0, loopXY, ror #30]

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	teq	t0, #0x00
	ldrne	rgbMask, [t0, pRgb]!
	ldrne	t0, [t0, #2]

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001
	ldrnesh	t2, [t1, #-0x1e]

	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgb8Block8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgb8Block8To444
	dcd	0x003e3a2e*3
	dcd	0x2a3c382c*3
	dcd	0x28363226*3
	dcd	0x22343024*3
	dcd	0x201e1a0e*3
	dcd	0x0a1c180c*3
	dcd	0x08161206*3
	dcd	0x02141004*3

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb8Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTHx8        0x30
#define STACK_OFFSET_PDSTCBCR       0x00
#define STACK_OFFSET_PRGB           0x04
#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	arg_row, arg_row, lsl #4

	add	pRgb, pRgb, arg_col, lsl #4 + 1 + 3
	add	pRgb, pRgb, arg_col, lsl #4 + 3

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	mov	loopXY, #0x00000008

	adr	pConst, constTblCenterCtrRgb8Block8To420

	add	r12, pRgb, r12, lsl #3
	str	r12, [sp, #STACK_OFFSET_WIDTHx8 - 0x10]

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	g0r0, [pRgb, #(0x02*3) + 2]!
	ldr	rgbMask, [pRgb, #-2]!

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	sub	t0, pRgb, #(0x02*3)

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{pDst, t0, t1, t2}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	mov	t0, g0r0

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]

	pkhbt	g0r0, rgbMask, rgbMask, lsl #8
	pkhtb	g1r1, t0, t0, asr #8
	pkhbt	b1b0, rgbMask, t0, lsl #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldr	rgbMask, [pRgb, #-(0x02*3)]
	ldr	t2, [pRgb, #-(0x02*3) + 2]

	adr	pRgb, srcOffsetTblCtrRgb8Block8To420 - 8

	add	g0r0, g0r0, g1r1

	ldrb	pRgb, [pRgb, loopXY, ror #29]

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhbt	t1, rgbMask, rgbMask, lsl #8
	pkhtb	g1r1, t2, t2, asr #8
	pkhbt	b3b2, rgbMask, t2, lsl #8
	uxtb16	t2, t1, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b3b2, b3b2

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	add	t0, t0, pRgb, lsl #1
	teq	pRgb, #0x00
	ldrne	rgbMask, [pRgb, t0]!
	ldrne	t0, [pRgb, #2]

	usat	t1, #8, t1, asr #14
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	loopXY, loopXY, #0x00000001

	addne	t1, pConst, loopXY, lsl #2
	ldrne	t2, [sp, #STACK_OFFSET_PDSTY0]
	ldrne	t1, [t1, #-0x2c]

	EPILOGUE_EQ_INST

	cmp	loopXY, #0x00000004

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	bne	%b10

	ldr	pRgb, [sp, #STACK_OFFSET_WIDTHx8]

	str	pRgb, [sp, #STACK_OFFSET_PRGB]
	ldr	rgbMask, [pRgb, #0x03*2]!
	ldr	t0, [pRgb, #2]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgb8Block8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgb8Block8To420
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x62363226,0x2a767266
	dcd	0x4a1e1a0e,0x225e5a4e
	dcd	0x42161206,0x0a565246
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x62363226,0x2a767266
	dcd	0x4a1e1a0e,0x225e5a4e
	dcd	0x42161206,0x0a565246

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTHx8
#undef STACK_OFFSET_PDSTCBCR
#undef STACK_OFFSET_PRGB
#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgb8Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	arg_row, arg_row, lsl #3

	add	pRgb, pRgb, arg_col, lsl #4 + 1 + 3
	add	pRgb, pRgb, arg_col, lsl #4 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgb8Block8To422

	mov	loopXY, #0x00000008

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	rgbMask, [pRgb]
	ldr	t0, [pRgb, #2]

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, rgbMask, rgbMask, lsl #8
	pkhtb	g1r1, t0, t0, asr #8
	pkhbt	b1b0, rgbMask, t0, lsl #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	t0, srcOffsetTblCtrRgb8Block8To422 - 8

	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	t0, [t0, loopXY, ror #29]

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	add	t0, t0, t0, lsl #1
	teq	t0, #0x00
	ldrne	rgbMask, [t0, pRgb]!
	ldrne	t0, [t0, #2]

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001

	ldrsh	pDstCbCr, [t1, #-0x1e]

	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTblCtrRgb8Block8To422
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgb8Block8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgb8Block8To422
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x683c382c,0x2a7c786c
	dcd	0x62363226,0x28767266
	dcd	0x60343024,0x22747064
	dcd	0x4a1e1a0e,0x205e5a4e
	dcd	0x481c180c,0x0a5c584c
	dcd	0x42161206,0x08565246
	dcd	0x40141004,0x02545044

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgba8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row

#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #3 + 2

	add	t0, r12, #8 * 4

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Rgba8To444

	mov	loopXY, #0x00000010

	ldmia	pRgb!, {t0, rgbMask}

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	pkhtb	b1b0, rgbMask, t0, asr #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	ldmiacc	pRgb!, {t0, rgbMask}

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]
	sub	t1, loopXY, #0x1e

	subs	loopXY, loopXY, #0x00000002
	ldrnesh	t2, [pConst, t1]

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #4]

	addne	pRgb, pRgb, #2 * 4
	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgba8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1

#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgba8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x30
#define STACK_OFFSET_PDSTCBCR       0x00

#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 2

	add	t0, r12, #8 * 4

	adr	pConst, constTblCenter_Rgba8To420

	mla	pRgb, arg_row, t0, pRgb

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	loopXY, #0x00000020

	add	g1r1, pRgb, t0

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldmia	g1r1, {g1r1, rgbMask}

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	push	{pDst, t0, t1, t2}

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	pkhbt	g0r0, g1r1, g1r1, lsl #8
	pkhtb	b1b0, rgbMask, g1r1, asr #16
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	add	g0r0, g0r0, g1r1

	ldmia	pRgb!, {g1r1, rgbMask}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhbt	t2, g1r1, g1r1, lsl #8
	pkhtb	pDst, rgbMask, g1r1, asr #16
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	uxtb16	t2, t2
	uxtb16	g1r1, g1r1
	uxtb16	b3b2, pDst

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	addcc	g1r1, pRgb, t0

	usat	t1, #8, t1, asr #14
	ldmiacc	g1r1, {g1r1, rgbMask}
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	t2, [sp, #STACK_OFFSET_WIDTH]

	subs	loopXY, loopXY, #0x00000004

	EPILOGUE_EQ_INST

	sub	t1, loopXY, #0x2c

	add	pRgb, pRgb, t2, lsl #1

	ldr	t1, [pConst, t1]
	add	g1r1, pRgb, t0
	ldr	t2, [sp, #STACK_OFFSET_PDSTY0]

	ldmia	g1r1, {g1r1, rgbMask}

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgba8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTH
#undef STACK_OFFSET_PDSTCBCR

#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Rgba8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 2

	add	t0, r12, #16 * 4

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Rgba8To422

	mov	loopXY, #0x00000010

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldmia	pRgb!, {t0, rgbMask}

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhbt	g1r1, rgbMask, rgbMask, lsl #8
	pkhtb	b1b0, rgbMask, t0, asr #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0
	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	ldmiacc	pRgb!, {t0, rgbMask}

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]

	ldrsh	pDstCbCr, [t1, loopXY]

	subs	loopXY, loopXY, #0x00000002

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #4]

	addne	pRgb, pRgb, #2 * 4
	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTbl_Rgba8To422
	dcw	0
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	0

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Rgba8To422
	dcd	offsetTbl_Rgba8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgba8Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #3 + 2

	add	pRgb, pRgb, arg_col, lsl #3 + 2 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgba8Block8To444

	mov	loopXY, #0x00000008

	ldmia	pRgb, {t0, rgbMask}

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhtb	g0r0, t0, t0, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, t0, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	adr	t0, srcOffsetTblCtrRgba8Block8To444 - 4

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	ldrb	t0, [t0, loopXY, ror #30]

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	add	rgbMask, pRgb, t0, lsl #2
	teq	t0, #0x00
	ldmiane	rgbMask, {t0, rgbMask}

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001
	ldrnesh	t2, [t1, #-0x1e]

	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgba8Block8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgba8Block8To444
	dcd	0x003e3a2e
	dcd	0x2a3c382c
	dcd	0x28363226
	dcd	0x22343024
	dcd	0x201e1a0e
	dcd	0x0a1c180c
	dcd	0x08161206
	dcd	0x02141004

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgba8Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTHx8        0x30
#define STACK_OFFSET_PDSTCBCR       0x00
#define STACK_OFFSET_PRGB           0x04
#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #4 + 2

	add	pRgb, pRgb, arg_col, lsl #4 + 2 + 3

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	mov	loopXY, #0x00000008

	adr	pConst, constTblCenterCtrRgba8Block8To420

	add	r12, pRgb, r12, lsl #3 + 2
	str	r12, [sp, #STACK_OFFSET_WIDTHx8 - 0x10]

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	g0r0, [pRgb, #(0x02*4)]!
	ldr	rgbMask, [pRgb, #4]

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	sub	t0, pRgb, #(0x02*4)

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{pDst, t0, t1, t2}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	mov	t0, g0r0

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]

	pkhtb	g0r0, t0, t0, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, t0, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldmdb	pRgb, {t2, rgbMask}

	adr	pRgb, srcOffsetTblCtrRgba8Block8To420 - 8

	add	g0r0, g0r0, g1r1

	ldrb	pRgb, [pRgb, loopXY, ror #29]

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhtb	t1, t2, t2, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b3b2, t2, rgbMask, lsl #16
	uxtb16	t2, t1
	uxtb16	g1r1, g1r1
	uxtb16	b3b2, b3b2, ror #8

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	add	pRgb, t0, pRgb, lsl #2
	teq	pRgb, t0
	ldmiane	pRgb, {t0, rgbMask}

	usat	t1, #8, t1, asr #14
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	loopXY, loopXY, #0x00000001

	addne	t1, pConst, loopXY, lsl #2
	ldrne	t2, [sp, #STACK_OFFSET_PDSTY0]
	ldrne	t1, [t1, #-0x2c]

	EPILOGUE_EQ_INST

	cmp	loopXY, #0x00000004

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	bne	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTHx8]

	add	pRgb, t0, #(0x02*4)
	str	t0, [sp, #STACK_OFFSET_PRGB]
	ldmia	pRgb, {t0, rgbMask}

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgba8Block8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgba8Block8To420
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x62363226,0x2a767266
	dcd	0x4a1e1a0e,0x225e5a4e
	dcd	0x42161206,0x0a565246
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x62363226,0x2a767266
	dcd	0x4a1e1a0e,0x225e5a4e
	dcd	0x42161206,0x0a565246

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTHx8
#undef STACK_OFFSET_PDSTCBCR
#undef STACK_OFFSET_PRGB
#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_CtrRgba8Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	arg_row, arg_row, lsl #3 + 2

	add	pRgb, pRgb, arg_col, lsl #4 + 2 + 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	PROLOGUE_INST

	mla	pRgb, arg_row, r12, pRgb

	adr	pConst, constTblCenterCtrRgba8Block8To422

	mov	loopXY, #0x00000008

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldmia	pRgb, {t0, rgbMask}

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhtb	g0r0, t0, t0, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, t0, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	t0, srcOffsetTblCtrRgba8Block8To422 - 8

	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	t0, [t0, loopXY, ror #29]

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	add	t0, pRgb, t0, lsl #2
	teq	t0, pRgb
	ldmiane	t0, {t0, rgbMask}

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	t1, pConst, loopXY, lsl #1

	subs	loopXY, loopXY, #0x00000001

	ldrsh	pDstCbCr, [t1, #-0x1e]

	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTblCtrRgba8Block8To422
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenterCtrRgba8Block8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

srcOffsetTblCtrRgba8Block8To422
	dcd	0x6a3e3a2e,0x007e7a6e
	dcd	0x683c382c,0x2a7c786c
	dcd	0x62363226,0x28767266
	dcd	0x60343024,0x22747064
	dcd	0x4a1e1a0e,0x205e5a4e
	dcd	0x481c180c,0x0a5c584c
	dcd	0x42161206,0x08565246
	dcd	0x40141004,0x02545044

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Bgr8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row

#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #3 + 1
	add	pRgb, pRgb, arg_col, lsl #3

	add	t0, r12, #8 * 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Bgr8To444

	mov	loopXY, #0x00000010

	ldr	t0, [pRgb], #2
	ldr	rgbMask, [pRgb], #4

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhtb	g1r1, rgbMask, rgbMask, asr #8
	pkhbt	b1b0, t0, rgbMask, lsl #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	ldrcc	t0, [pRgb], #2
	ldrcc	rgbMask, [pRgb], #4

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]
	sub	t1, loopXY, #0x1e

	subs	loopXY, loopXY, #0x00000002
	ldrnesh	t2, [pConst, t1]

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #2]

	addne	pRgb, pRgb, #2 * 3
	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Bgr8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1

#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Bgr8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x30
#define STACK_OFFSET_PDSTCBCR       0x00

#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1
	add	pRgb, pRgb, arg_col, lsl #4

	add	t0, r12, #8 * 3

	adr	pConst, constTblCenter_Bgr8To420

	mla	pRgb, arg_row, t0, pRgb

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	loopXY, #0x00000020

	add	g1r1, pRgb, t0

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	rgbMask, [g1r1]
	ldr	g1r1, [g1r1, #2]

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	push	{pDst, t0, t1, t2}

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	pkhbt	g0r0, rgbMask, rgbMask, lsl #8
	pkhbt	b1b0, rgbMask, g1r1, lsl #8
	pkhtb	g1r1, g1r1, g1r1, asr #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	ldr	rgbMask, [pRgb], #2

	add	g0r0, g0r0, g1r1

	ldr	g1r1, [pRgb], #4

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhbt	t2, rgbMask, rgbMask, lsl #8
	pkhbt	b3b2, rgbMask, g1r1, lsl #8
	pkhtb	g1r1, g1r1, g1r1, asr #8
	uxtb16	t2, t2, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b3b2, b3b2

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	usat	t1, #8, t1, asr #14
	ldrcc	rgbMask, [t0, pRgb]!
	usat	t2, #8, t2, asr #14
	ldrcc	g1r1, [t0, #2]

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	t2, [sp, #STACK_OFFSET_WIDTH]

	subs	loopXY, loopXY, #0x00000004

	EPILOGUE_EQ_INST

	sub	t1, loopXY, #0x2c

	add	pRgb, pRgb, t2, lsl #1

	ldr	t1, [pConst, t1]
	ldr	t2, [sp, #STACK_OFFSET_PDSTY0]

	ldr	rgbMask, [t0, pRgb]!

	and	t3, t1, #0xff
	ldr	g1r1, [t0, #2]
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Bgr8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTH
#undef STACK_OFFSET_PDSTCBCR

#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Bgr8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x3)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 1
	add	pRgb, pRgb, arg_col, lsl #4

	add	t0, r12, #16 * 3

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Bgr8To422

	mov	loopXY, #0x00000010

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldr	t0, [pRgb], #2
	ldr	rgbMask, [pRgb], #4

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhbt	g0r0, t0, t0, lsl #8
	pkhtb	g1r1, rgbMask, rgbMask, asr #8
	pkhbt	b1b0, t0, rgbMask, lsl #8
	uxtb16	g0r0, g0r0, ror #24
	uxtb16	g1r1, g1r1, ror #24
	uxtb16	b1b0, b1b0

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0
	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	ldrcc	t0, [pRgb], #2
	ldrcc	rgbMask, [pRgb], #4

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]

	ldrsh	pDstCbCr, [t1, loopXY]

	subs	loopXY, loopXY, #0x00000002

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #2]

	addne	pRgb, pRgb, #2 * 3
	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTbl_Bgr8To422
	dcw	0
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	0

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Bgr8To422
	dcd	offsetTbl_Bgr8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Abgr8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row

#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #3 + 2

	add	t0, r12, #8 * 4

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Abgr8To444

	mov	loopXY, #0x00000010

	ldmia	pRgb!, {t0, rgbMask}

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhtb	g0r0, t0, t0, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, t0, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	smlad	t2,   g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	t0,   g1r1, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	g1r1, g1r1, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlabb	t2,   b1b0, g2crMul_r2crMul_b2cbMul, t2
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0

	smlatb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlatt	b1b0, b1b0, b2crMul_b2yMul, t0

	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	usat	t2,   #8, t2,   asr #12
	usat	g0r0, #8, g0r0, asr #12

	usat	g1r1, #8, g1r1, asr #12
	usat	b1b0, #8, b1b0, asr #12

	adds	loopXY, loopXY, #0x40000000

	ldmiacc	pRgb!, {t0, rgbMask}

	strh	t2,   [t1, pDstY]!
	strh	g0r0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	g1r1, [t1, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	b1b0, [t1, #STRUCT_OFFSET_CBBUF_TO_CRBUF + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]
	sub	t1, loopXY, #0x1e

	subs	loopXY, loopXY, #0x00000002
	ldrnesh	t2, [pConst, t1]

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #4]

	addne	pRgb, pRgb, #2 * 4
	addne	pDstY, pDstY, t2

	bne	%b10

	EPILOGUE_INST

	dcw	(-64-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-64  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Abgr8To444
	dcd	STRUCT_OFFSET_CBBUF - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1

#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Abgr8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDst       arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define t2         r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12
#define b3b2             rgbMask

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          t2
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define t3   g0r0

#define PROLOGUE_INST         push {r4-r11, r12, lr}

#define EPILOGUE_EQ_INST         popeq  {r0-r3, r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x30
#define STACK_OFFSET_PDSTCBCR       0x00

#define STACK_OFFSET_PDSTY0         0x08
#define STACK_OFFSET_PDSTY1         0x0c

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 2

	add	t0, r12, #8 * 4

	adr	pConst, constTblCenter_Abgr8To420

	mla	pRgb, arg_row, t0, pRgb

	add	t1, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	loopXY, #0x00000020

	add	g1r1, pRgb, t0

	add	t2, t1, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldmia	g1r1, {g1r1, rgbMask}

	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

	add	pDst, t1, #STRUCT_OFFSET_CBBUF

	push	{pDst, t0, t1, t2}

10
	str	pDst, [sp, #STACK_OFFSET_PDSTCBCR]
	pld	[pRgb]

	pkhtb	g0r0, g1r1, g1r1, asr #24
	pkhtb	t0, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, g1r1, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, t0
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY1]

	add	g0r0, g0r0, g1r1

	ldmia	pRgb!, {g1r1, rgbMask}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY1]

	pkhtb	t2, g1r1, g1r1, asr #24
	pkhtb	t1, rgbMask, rgbMask, asr #24
	pkhbt	b3b2, g1r1, rgbMask, lsl #16
	uxtb16	t2, t2
	uxtb16	g1r1, t1
	uxtb16	b3b2, b3b2, ror #8

	smlad	t0, t2,   g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd
	smlabb	t0, b3b2, b2crMul_b2yMul, t0
	smlatb	t1, b3b2, b2crMul_b2yMul, t1

	add	g0r0, g0r0, t2

	ldr	pDst, [sp, #STACK_OFFSET_PDSTY0]

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	add	g0r0, g0r0, g1r1

	add	b1b0, b1b0, b3b2

	strh	t0, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	pDst, [sp, #STACK_OFFSET_PDSTY0]

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	t1, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd
	smlad	t0, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd
	smlabt	t1, b1b0, b2crMul_b2yMul, t1

	smlabb	t2, b1b0, g2crMul_r2crMul_b2cbMul, t0

	ldrd	pDst, t0, [sp, #STACK_OFFSET_PDSTCBCR]
	ldmia	pConst, {g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	addcc	g1r1, pRgb, t0

	usat	t1, #8, t1, asr #14
	ldmiacc	g1r1, {g1r1, rgbMask}
	usat	t2, #8, t2, asr #14

	strh	t1, [pDst, #STRUCT_OFFSET_CBBUF_TO_CRBUF]
	strh	t2, [pDst], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	t2, [sp, #STACK_OFFSET_WIDTH]

	subs	loopXY, loopXY, #0x00000004

	EPILOGUE_EQ_INST

	sub	t1, loopXY, #0x2c

	add	pRgb, pRgb, t2, lsl #1

	ldr	t1, [pConst, t1]
	add	g1r1, pRgb, t0
	ldr	t2, [sp, #STACK_OFFSET_PDSTY0]

	ldmia	g1r1, {g1r1, rgbMask}

	and	t3, t1, #0xff
	add	t2, t2, t1, asr #16
	sub	pDst, pDst, t3
	sxtab	t3, t2, t1, ror #8

	strd	t2, t3, [sp, #STACK_OFFSET_PDSTY0]

	b	%b10

	dcd	(((-256-3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+3-2) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+5-3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-0+7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+4-5) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((    -3  ) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+6-4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-4+3) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+7-6) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128-7+4) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((+1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64+1-7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	(((-128  +7) * __cpp(sizeof(JpegMpEncoderYCbCrType)))<<16) ASM_OP_LOG_OR ((((-1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))&0xff)<<8) ASM_OP_LOG_OR ((64  -1) * __cpp(sizeof(JpegMpEncoderYCbCrType)))

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 2)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Abgr8To420
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDst
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef t2
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd
#undef b3b2

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef t3

#undef PROLOGUE_INST
#undef EPILOGUE_EQ_INST
#undef STACK_OFFSET_WIDTH
#undef STACK_OFFSET_PDSTCBCR

#undef STACK_OFFSET_PDSTY0
#undef STACK_OFFSET_PDSTY1

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

       void JpegMpEncoderAsm_Abgr8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx   r0
#define arg_col    r1
#define arg_row    r2

#define pRgb       r3
#define pDstY      arg_pCtx
#define loopXY     r4
#define pConst     r5

#define t0         arg_col
#define t1         arg_row
#define pDstCbCr   r6
#define g0r0       r7
#define g1r1       r8
#define b1b0       r9

#define rgbMask          r10
#define g2yMul_r2yMul    r11
#define rgb2yAdd         r12

#define b2crMul_b2yMul   lr

#define g2cbMul_r2cbMul          rgbMask
#define rgb2cbcrAdd              g2yMul_r2yMul
#define g2crMul_r2crMul_b2cbMul  rgb2yAdd

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_WIDTH          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	r12, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x4)]

	ldr	pRgb, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #3

	PROLOGUE_INST

	add	pRgb, pRgb, arg_col, lsl #4 + 2

	add	t0, r12, #16 * 4

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	pRgb, arg_row, t0, pRgb

	adr	pConst, constTblCenter_Abgr8To422

	mov	loopXY, #0x00000010

	add	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	ldmia	pRgb!, {t0, rgbMask}

	ldmib	pConst, {g2yMul_r2yMul, rgb2yAdd, b2crMul_b2yMul}

10
	pkhtb	g0r0, t0, t0, asr #24
	pkhtb	g1r1, rgbMask, rgbMask, asr #24
	pkhbt	b1b0, t0, rgbMask, lsl #16
	uxtb16	g0r0, g0r0
	uxtb16	g1r1, g1r1
	uxtb16	b1b0, b1b0, ror #8

	smlad	t0, g0r0, g2yMul_r2yMul, rgb2yAdd
	smlad	t1, g1r1, g2yMul_r2yMul, rgb2yAdd

	ldmdb	pConst, {g2cbMul_r2cbMul, rgb2cbcrAdd, g2crMul_r2crMul_b2cbMul}

	add	g0r0, g0r0, g1r1

	smlabb	t0, b1b0, b2crMul_b2yMul, t0
	smlatb	t1, b1b0, b2crMul_b2yMul, t1

	add	b1b0, b1b0, b1b0, lsr #16

	smlad	g1r1, g0r0, g2cbMul_r2cbMul, rgb2cbcrAdd

	smlad	g0r0, g0r0, g2crMul_r2crMul_b2cbMul, rgb2cbcrAdd

	usat	t0, #8, t0, asr #12
	usat	t1, #8, t1, asr #12

	strh	t0, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	t1, [pDstY], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	g1r1, b1b0, g2crMul_r2crMul_b2cbMul, g1r1
	smlabt	g0r0, b1b0, b2crMul_b2yMul, g0r0
	ldmia	pConst, {t1, g2yMul_r2yMul, rgb2yAdd}

	adds	loopXY, loopXY, #0x20000000

	ldmiacc	pRgb!, {t0, rgbMask}

	usat	g0r0, #8, g0r0, asr #13
	usat	g1r1, #8, g1r1, asr #13

	strh	g0r0, [pDstCbCr, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	strh	g1r1, [pDstCbCr], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[pRgb, #0x20]
	bcc	%b10

	ldr	t0, [sp, #STACK_OFFSET_WIDTH]

	ldrsh	pDstCbCr, [t1, loopXY]

	subs	loopXY, loopXY, #0x00000002

	ldrne	t0, [pRgb, t0]!
	ldrne	rgbMask, [pRgb, #4]

	addne	pRgb, pRgb, #2 * 4
	addne	pDstY, pDstY, pDstCbCr
	addne	pDstCbCr, pDstY, #STRUCT_OFFSET_CBBUF

	bne	%b10

	EPILOGUE_INST

offsetTbl_Abgr8To422
	dcw	0
	dcw	(-128-2  ) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-3+2) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-5+3) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-4+5) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-6+4) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-7+6) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128-1+7) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	(-128  +1) * __cpp(sizeof(JpegMpEncoderYCbCrType))
	dcw	0

	dcd	__cpp((s_Coef8G2cbMul << 16) | (s_Coef8R2cbMul & 0xffff))
	dcd	__cpp(s_Coef8Rgb2cbcrAdd << 1)
	dcd	__cpp((s_Coef8G2crMul << 16) | s_Coef8R2crMul_B2cbMul)
constTblCenter_Abgr8To422
	dcd	offsetTbl_Abgr8To422
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pRgb
#undef pDstY
#undef loopXY
#undef pConst

#undef t0
#undef t1
#undef pDstCbCr
#undef g0r0
#undef g1r1
#undef b1b0

#undef rgbMask
#undef g2yMul_r2yMul
#undef rgb2yAdd

#undef b2crMul_b2yMul

#undef g2cbMul_r2cbMul
#undef rgb2cbcrAdd
#undef g2crMul_r2crMul_b2cbMul

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_WIDTH

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}

#if 0

       void JpegMpEncoderAsm_Xuyv8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Xuyv8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx          r0
#define arg_col           r1
#define arg_row           r2

#define pDstY             arg_pCtx
#define pDstOffsetTbl     arg_col
#define t9                arg_row
#define pXuyv             r3

#define t0                r4
#define t1                r5
#define t2                r6
#define t3                r7
#define t4                r8
#define t5                r9
#define t6                r10
#define t7                r11
#define width_            r12
#define t8                lr

#define PROLOGUE_INST     push {r4-r11, r12, lr}
#define EPILOGUE_INST     pop  {r4-r11, r12, pc}
#define STACK_OFFSET_PXUYV          0x20

#define STRUCT_OFFSET_CBBUF           ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)
#define STRUCT_OFFSET_CBBUF_TO_CRBUF (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-STRUCT_OFFSET_CBBUF)

	ldr	width_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	pXuyv, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	arg_row, arg_row, lsl #4 + 2

	PROLOGUE_INST

	add	pXuyv, pXuyv, arg_col, lsl #4 + 2

	mla	pXuyv, arg_row, width_, pXuyv

	add	pDstY, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)
	adr	pDstOffsetTbl, offsetTblXuyv8To420

	mov	width_, width_, lsl #2

	mov	t9, #0x00000008

10
	add	t0, pXuyv, #2 * 4
	str	t0, [sp, #STACK_OFFSET_PXUYV]
20

	ldrd	t0, t1, [pXuyv], width_
	ldrd	t2, t3, [pXuyv], width_
	ldrd	t4, t5, [pXuyv], width_
	ldrd	t6, t7, [pXuyv], width_

	uhadd8	t8, t0, t1
	pkhtb	t0, t2, t0, asr #16
	uhadd8	t2, t2, t3
	pkhtb	t1, t3, t1, asr #16

	uhadd8	t3, t4, t5
	pkhtb	t4, t4, t6, asr #16
	uhadd8	t6, t6, t7
	pkhtb	t5, t5, t7, asr #16

	ldrh	t7, [pDstOffsetTbl], #2

	uhadd8	t2, t2, t8
	uhadd8	t3, t3, t6

	eor	t8, t7, #__cpp(0x06 * sizeof(JpegMpEncoderYCbCrType))

	uxtb16	t0, t0
	str	t0, [t7, pDstY]!
	uxtb16	t1, t1
	str	t1, [t7, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	t4, t4
	str	t4, [t8, pDstY]!
	uxtb16	t5, t5
	str	t5, [t8, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	ldrh	t7, [pDstOffsetTbl], #2

	pkhbt	t0, t2, t3, lsl #16
	pkhtb	t1, t3, t2, asr #16

	tst	t7, #__cpp(0x02 * sizeof(JpegMpEncoderYCbCrType))
	uxtb16eq t0, t0, ror #8
	uxtb16ne t0, t0, ror #24
	str	t0, [t7, pDstY]!

	uxtb16eq t1, t1, ror #8
	uxtb16ne t1, t1, ror #24
	str	t1, [t7, #STRUCT_OFFSET_CBBUF_TO_CRBUF]

	adds	t9, t9, #0x40000000
	bcc	%b20

	ldr	pXuyv, [sp, #STACK_OFFSET_PXUYV]
	subs	t9, t9, #1
	bne	%b10

	EPILOGUE_INST

offsetTblXuyv8To420
	dcw	__cpp(0x00 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x00 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x04 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x06 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x80 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x04 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x84 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x02 * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x10 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x08 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x14 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x0e * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x90 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x0c * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x94 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x0a * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x20 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x10 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x24 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x16 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xa0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x14 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xa4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x12 * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x30 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x18 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x34 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x1e * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xb0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x1c * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xb4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x1a * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x40 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x20 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x44 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x26 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xc0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x24 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xc4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x22 * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x50 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x28 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x54 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x2e * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xd0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x2c * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xd4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x2a * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x60 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x30 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x64 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x36 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xe0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x34 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xe4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x32 * sizeof(JpegMpEncoderYCbCrType)))

	dcw	__cpp(0x70 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x38 * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0x74 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x3e * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xf0 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x3c * sizeof(JpegMpEncoderYCbCrType)))
	dcw	__cpp(0xf4 * sizeof(JpegMpEncoderYCbCrType)), STRUCT_OFFSET_CBBUF + __cpp((0x3a * sizeof(JpegMpEncoderYCbCrType)))

#undef arg_pCtx
#undef arg_col
#undef arg_row

#undef pDstY
#undef pDstOffsetTbl
#undef t9
#undef pXuyv

#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef width_
#undef t8

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PXUYV

#undef STRUCT_OFFSET_CBBUF
#undef STRUCT_OFFSET_CBBUF_TO_CRBUF
}
#endif

       void JpegMpEncoderAsm_ForwardDct(register JpegMpEncoderYCbCrType *data, register const JpegMpEncoderQTableType *qtable, register s16 *output);
NN_ASM void JpegMpEncoderAsm_ForwardDct(register JpegMpEncoderYCbCrType *data NN_IS_UNUSED_VAR, register const JpegMpEncoderQTableType *qtable NN_IS_UNUSED_VAR, register s16 *output NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define pSrc        r0
#define arg_pQtable r1
#define arg_pDst    r2

#define pTmp        arg_pQtable
#define pConst      arg_pDst

#define pSrcEnd     r3
#define pLoopTable  r4

#define const_0xfffc_0000  r5
#define t0          r6
#define t1          r7
#define t2          r8
#define t3          r9
#define a1a0        r10
#define a6a7        t1
#define a2a3        lr
#define a5a4        t2
#define c2c3        a2a3
#define c1c0        a1a0

#define const_FC4_0x0100   r11
#define const_0x0100_0100  r12

#define result0     t3
#define result1     c1c0
#define result2     const_0x0100_0100
#define result3     c2c3

#define c6c4        a5a4
#define c5c7        a6a7

#define c7pc5       t0
#define c7mc5       c5c7

#define const_FSEC2_FSC24    result0
#define const_mFSEC6_FSC64   result1
#define const_FSEC6_mFSC64   result2
#define const_mFSEC2_mFSC24  result3

#define result4     const_FSEC2_FSC24
#define result5     const_mFSEC6_FSC64
#define result6     const_FSEC6_mFSC64
#define result7     const_mFSEC2_mFSC24

#define t4              r10
#define t5              r11
#define t6              r12
#define t7              lr
#define t8              r5

#define const_FC4       r0
#define const_mFC4      pTmp
#define const_32768     r3

#define const_FSC24     const_FC4
#define const_mFSC64    const_mFC4
#define const_FSEC2     pConst
#define const_FSEC6     const_32768

#define pQtable         const_FSC24
#define pDst            const_mFSC64
#define q1q0            const_FSEC2
#define q3q2            const_FSEC6

#define q5q4            q1q0
#define q7q6            q3q2

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_FRAME_TMP_ALLOC_SIZE  (64*4)

#define STACK_OFFSET_TMP            (2*4)

#define STACK_FRAME_TMP_FREE_SIZE   (STACK_OFFSET_TMP+STACK_FRAME_TMP_ALLOC_SIZE)

#define STACK_OFFSET_ARG_PQTABLE    0
#define STACK_OFFSET_ARG_PDST       4

	PROLOGUE_INST

	sub	sp, sp, #STACK_FRAME_TMP_ALLOC_SIZE

	add	pSrcEnd, pSrc, #64 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	adr	pLoopTable, loopTable

	push	{arg_pQtable, arg_pDst}

	adr	pConst, constTblCenter1
	add	pTmp, sp, #STACK_OFFSET_TMP

	ldmdb	pConst, {const_0xfffc_0000, const_FC4_0x0100}

asm_forwarddct_1st_loop

	ldmia	pSrc!, {t0, t1, t2, t3}
	mov	const_0x0100_0100,                    #0x01000000
	orr	const_0x0100_0100, const_0x0100_0100, #0x00000100

	sadd16	a1a0, t0, t1
	ssub16	a6a7, t0, t1
	sadd16	a2a3, t3, t2
	ssub16	a5a4, t3, t2

	ssub16	t0, a1a0, a2a3

	sadd16	c1c0, a1a0, a2a3

	add	c2c3, t0, t0, lsl #16

	smlad	result0, c1c0, const_0x0100_0100, const_0xfffc_0000

	smusd	result1, c1c0, const_0x0100_0100

	smuad	result2, c2c3, const_FC4_0x0100

	smusd	result3, c2c3, const_FC4_0x0100

	sadd16	t0, a5a4, a6a7

	add	c6c4, a6a7, a6a7, lsl #16

	stmia	pTmp!, {result0, result1, result2, result3}

	pkhbt	c5c7, a6a7, t0

	add	t0, t0, t0, asr #16

	ldmia	pConst, {const_FSEC2_FSC24, const_mFSEC6_FSC64, const_FSEC6_mFSC64, const_mFSEC2_mFSC24}

	pkhbt	c6c4, t0, c6c4

	smuad	c7pc5, c5c7, const_FC4_0x0100
	smusd	c7mc5, c5c7, const_FC4_0x0100

	smlad	result4, c6c4, const_FSEC2_FSC24,   c7pc5
	smlad	result5, c6c4, const_mFSEC6_FSC64,  c7mc5
	smlad	result6, c6c4, const_FSEC6_mFSC64,  c7mc5
	smlad	result7, c6c4, const_mFSEC2_mFSC24, c7pc5

	ldrb	c6c4, [pLoopTable], #1
	cmp	pSrc, pSrcEnd
	stmia	pTmp, {result4, result5, result6, result7}
	add	pTmp, sp, c6c4

	bcc	asm_forwarddct_1st_loop

asm_forwarddct_2nd_loop

	ldr	t0, [pTmp]
	ldr	t1, [pTmp, #1*8*4]
	ldr	t2, [pTmp, #2*8*4]
	ldr	t3, [pTmp, #3*8*4]
	ldr	t4, [pTmp, #4*8*4]
	ldr	t5, [pTmp, #5*8*4]
	ldr	t6, [pTmp, #6*8*4]
	ldr	t7, [pTmp, #7*8*4]

	adr	pConst, constTblCenter2
	mov	const_32768, #32768

	add	t8, t0, t1
	sub	t0, t0, t1
	add	t1, t2, t3
	sub	t2, t2, t3
	add	t3, t4, t5
	sub	t4, t4, t5
	add	t5, t6, t7
	sub	t6, t6, t7

	add	t7, t8, t5
	sub	t5, t8, t5
	sub	t8, t1, t3
	add	t1, t1, t3
	add	t3, t8, t5

	add	t8, t7, #128
	add	t7, t8, t1
	sub	t1, t8, t1
	mov	t7, t7, asr #8
	pkhbt	t7, t7, t1, lsl #16 - 8

	ldmdb	pConst, {const_FC4, const_mFC4}

	add	t6, t6, t0
	add	t4, t4, t2
	add	t2, t2, t0
	add	t6, t6, t4

	mla	t1, const_FC4,  t3, const_32768
	mla	t3, const_mFC4, t3, const_32768
	mla	t8, const_FC4,  t4, const_32768
	mla	t4, const_mFC4, t4, const_32768

	ldmia	pConst, {const_FSC24, const_mFSC64, const_FSEC2, const_FSEC6}

	add	t1, t1, t5, lsl #8
	add	t3, t3, t5, lsl #8
	add	t5, t8, t0, lsl #8
	add	t4, t4, t0, lsl #8

	mul	t0, const_FSC24,  t6
	mul	t6, const_mFSC64, t6

	ldrd	pQtable, pDst, [sp, #STACK_OFFSET_ARG_PQTABLE]

	mla	t0, const_FSEC2,  t2, t0
	mla	t2, const_FSEC6,  t2, t6

	ldmia	pQtable!, {q1q0, q3q2}
	mov	t8, #2048

	add	t6, t5, t0
	sub	t0, t5, t0
	add	t5, t4, t2
	sub	t2, t4, t2

	smlatb	t1, t1, q3q2, t8

	smlatt	t5, t5, q3q2, t8

	smlabb	t4, t7, q1q0, t8

	smlatt	t6, t6, q1q0, t8

	ldmia	pQtable!, {q5q4, q7q6}
	mov	t1, t1, asr #12
	mov	t4, t4, asr #12
	pkhbt	t1, t1, t5, lsl #16 - 12

	smlatb	t7, t7, q5q4, t8

	smlatt	t2, t2, q5q4, t8

	smlatb	t3, t3, q7q6, t8

	smlatt	t8, t0, q7q6, t8

	pkhbt	t0, t4, t6, lsl #16 - 12

	ldrb	t4, [pLoopTable], #1

	mov	t7, t7, asr #12
	pkhbt	t2, t7, t2, lsl #16 - 12
	mov	t3, t3, asr #12
	pkhbt	t3, t3, t8, lsl #16 - 12
	stmia	pDst!, {t0, t1, t2, t3}

	cmp	t4, #0xff

	strd	pQtable, pDst, [sp, #STACK_OFFSET_ARG_PQTABLE]
	add	pTmp, sp, t4

	bne	asm_forwarddct_2nd_loop

	add	sp, sp, #STACK_FRAME_TMP_FREE_SIZE
	EPILOGUE_INST

loopTable
	dcb	                              2*8*4+STACK_OFFSET_TMP, 4*8*4+STACK_OFFSET_TMP, 6*8*4+STACK_OFFSET_TMP, 7*8*4+STACK_OFFSET_TMP, 5*8*4+STACK_OFFSET_TMP, 3*8*4+STACK_OFFSET_TMP, 1*8*4+STACK_OFFSET_TMP
	dcb	0*4+STACK_OFFSET_TMP, 4*4+STACK_OFFSET_TMP, 2*4+STACK_OFFSET_TMP, 6*4+STACK_OFFSET_TMP, 1*4+STACK_OFFSET_TMP, 5*4+STACK_OFFSET_TMP, 3*4+STACK_OFFSET_TMP, 7*4+STACK_OFFSET_TMP
	dcb	0xff

	dcd	0xfffc0000
	dcd	(181 << 16) ASM_OP_LOG_OR 0x0100
constTblCenter1
	dcd	( 139 << 16) ASM_OP_LOG_OR     98
	dcd	(-334 << 16) ASM_OP_LOG_OR    237
	dcd	( 334 << 16) ASM_OP_LOG_OR ((-237) & 0xffff)
	dcd	(-139 << 16) ASM_OP_LOG_OR ( (-98) & 0xffff)

	dcd	181
	dcd	-181
constTblCenter2
	dcd	98
	dcd	-237
	dcd	139
	dcd	334

#undef pSrc
#undef arg_pQtable
#undef arg_pDst

#undef pTmp
#undef pConst

#undef pSrcEnd
#undef pLoopTable

#undef const_0xfffc_0000
#undef t0
#undef t1
#undef t2
#undef t3
#undef a1a0
#undef a6a7
#undef a2a3
#undef a5a4
#undef c2c3
#undef c1c0

#undef const_FC4_0x0100
#undef const_0x0100_0100

#undef result0
#undef result1
#undef result2
#undef result3

#undef c6c4
#undef c5c7

#undef c7pc5
#undef c7mc5

#undef const_FSEC2_FSC24
#undef const_mFSEC6_FSC64
#undef const_FSEC6_mFSC64
#undef const_mFSEC2_mFSC24

#undef result4
#undef result5
#undef result6
#undef result7

#undef t4
#undef t5
#undef t6
#undef t7
#undef t8

#undef const_FC4
#undef const_mFC4
#undef const_32768

#undef const_FSC24
#undef const_mFSC64
#undef const_FSEC2
#undef const_FSEC6

#undef pQtable
#undef pDst
#undef q1q0
#undef q3q2

#undef q5q4
#undef q7q6

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_FRAME_TMP_ALLOC_SIZE
#undef STACK_OFFSET_TMP
#undef STACK_FRAME_TMP_FREE_SIZE
#undef STACK_OFFSET_ARG_PQTABLE
#undef STACK_OFFSET_ARG_PDST
}

#undef ASM_STRUCTURE_MEMBER_OFFSET_OF
#undef ASM_OP_LOG_OR

#pragma arm
