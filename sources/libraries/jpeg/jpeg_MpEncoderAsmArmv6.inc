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
    LDR     R12, [R0,#0x310]
    LDR     R1, [R0,#0x314]
    LDR     R2, [R0,#0x318]
    LDR     R3, [R0,#0x31C]
    ADD     R1, R12, R1
    ADD     R2, R12, R2
    LDRB    R12, [R0,#0x320]
    SUB     R2, R2, #1
    STR     R1, [R0,#0x354]
	STR     R2, [R0,#0x358]
    MOV     R3, R3,LSL#24
    STR     R3, [R0,#0x350]
    ADD     R12, R12, #0x18
    STR     R12, [R0,#0x34C]
    BX      LR
}

void JpegMpEncoderAsm_ConvertWorkToC(register JpegMpEncoderContext* pCtx);

NN_ASM void JpegMpEncoderAsm_ConvertWorkToC(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8
    LDR     R12, [R0,#0x310]
    LDR     R1, [R0,#0x354]
    LDR     R2, [R0,#0x350]
    LDR     R3, [R0,#0x34C]
    SUB     R12, R1, R12
    STR     R12, [R0,#0x314]
    MOV     R2, R2,LSR#24
    STR     R2, [R0,#0x31C]
    SUB     R3, R3, #0x18
    STRB    R3, [R0,#0x320]
    BX      LR
}

bool JpegMpEncoderAsm_ComponentSequential(register JpegMpEncoderContext* pCtx, register JpegMpEncoderComponentStructure* component);

NN_ASM bool JpegMpEncoderAsm_ComponentSequential(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register JpegMpEncoderComponentStructure* component NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mDcTable)]

	push	{r1, r4-r11, lr}

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)

	add	r3, r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mCoefficients)

	add	r4, r12, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufsi)
	add	r5, r12, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufco)

	ldr	r8, [r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mLastDcValue)]
	ldrsh	r7, [r3]

	subs	r8, r7, r8

	str	r7, [r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mLastDcValue)]

	rsbmi	r8, r8, #0

	clz	r9, r8
	rsb	r9, r9, #32

	mvnmi	r8, r8

	ldr	r1, [r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderComponentStructure, mAcTable)]

	ldrb	r7, [r4, r9]
	ldr	r6, [r5, r9, lsl #2]

	add	r4, r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufsi)
	add	r5, r1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderHuffmanStructure, mEhufco)
	stmdb	r2, {r3, r4, r5}

	mvn	r1, #0

	bic	r6, r6, r1, lsl r7
	ldmia	r2, {r3, r4, r5}
	bic	r8, r8, r1, lsl r9
	bl	bitWrite

	ldrsh	r10, [r3, #1 * 2]

	adr	r11, zigTable

	mov	r12, #0

	cmp	r10, #0
	bne	zerorunDone
	b	asm_sequential_zero

	cmp	r12, #16
	blt	zerorunDone

zerorunLoop
	ldrb	r7, [r4, #0xF0]
	ldr	r6, [r5, #0xF0 * 4]
	ldmia	r2, {r3, r4, r5}
	mvn	r1, #0
	mov	r9, #0
	bic	r6, r6, r1, lsl r7
	bl	bitWrite

	cmp	r12, #32
	sub	r12, r12, #16
	bge	zerorunLoop

zerorunDone

	movs	r8, r10
	rsbmi	r8, r8, #0

	clz	r9, r8
	rsb	r9, r9, #32

	orr	lr, r9, r12, lsl #4

	mvnmi	r8, r8

	ldrb	r7, [r4, lr]
	ldr	r6, [r5, lr, lsl #2]

	mvn	r1, #0
	ldmia	r2, {r3, r4, r5}
	bic	r6, r6, r1, lsl r7
	bic	r8, r8, r1, lsl r9

	ldrb	r1, [r11, #1]!

	bl	bitWrite

	ldrsh	r10, [r3, r1]
	cmp	r1, #0
	beq	asm_sequential_end

	cmp	r10, #0
	mov	r12, #0
	bne	zerorunDone

asm_sequential_zero

	ldrb	r1, [r11, #1]!
	add	r12, r12, #1

	cmp	r1, #0
	beq	asm_sequential_end_zerorun

	ldrsh	r10, [r3, r1]

	cmp	r10, #0

	beq	asm_sequential_zero

	cmp	r12, #16
	blt	zerorunDone
	b	zerorunLoop

asm_sequential_end_zerorun
	ldrb	r7, [r4]
	ldr	r6, [r5]
	mvn	r1, #0
	ldmia	r2, {r3, r4, r5}
	bic	r6, r6, r1, lsl r7
	mov	r9, #0
	bl	bitWrite

asm_sequential_end
	mov	r0, #1
	pop	{r1, r4-r11, pc}

bitWriteError
	mov	r0, #0
	pop	{r1, r4-r11, pc}

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
	mov	r6, r8
	mov	r9, #0
bitWrite
	sub	r3, r3, r7
	cmp	r3, #24
	orr	r4, r4, r6, lsl r3
	bgt	done
	ldr	r7, [r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.pDst2Limit)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.bitLen2)]
	mov	r6, r4, lsr #24
	mov	r4, r4, lsl #8
	strb	r6, [r5], #1
	cmp	r5, r7
	add	r3, r3, #8
	bcc	next

	b	bitWriteError
next
	cmp	r6, #0xff
	streqb	r4, [r5], #1
	cmp	r3, #24
	bgt	done2

	mov	r6, r4, lsr #24
	mov	r4, r4, lsl #8
	strb	r6, [r5], #1
	cmp	r5, r7
	add	r3, r3, #8
	bcc	next2

	b	bitWriteError
next2
	cmp	r6, #0xff
	streqb	r4, [r5], #1
done2
done
	movs	r7, r9
	stmia	r2, {r3, r4, r5}
	bne	bitWriteCont
	ldmdb	r2, {r3, r4, r5}
	bx	lr
}

void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	r3,  [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push	{r4-r11, r12, lr}

	mov	r2, r2, lsl #3 + 1
	add	r3, r3, r1, lsl #3 + 1

	mla	r3, r2, r12, r3

	mov	r12, r12, lsl #1

	add	r4, r0, #8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r4, [sp, #0x20]

	add	r1, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

10

	ldr	r8,  [r3], r12
	ldr	r9,  [r3], r12
	ldr	r11, [r3], r12
	ldr	r10, [r3], r12
	ldr	r6,  [r3], r12
	ldr	r4,  [r3], r12

	pkhbt	r7,  r10, r11, lsl #16
	pkhtb	r11, r11, r10, asr #16
	pkhtb	r10, r4,  r6,  asr #16
	pkhbt	r6,  r6,  r4,  lsl #16

	uxtb16	r4, r10, ror #8
	uxtb16	r5, r11, ror #8

	strd	r4, r5, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	strd	r4, r5, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + (8 + 4) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r4, r6, ror #8
	uxtb16	r5, r7, ror #8

	strd	r4, r5, [r1, #4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	strd	r4, r5, [r1, #(8 + 4) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhtb	r4, r9, r8, asr #16
	pkhbt	r5, r8, r9, lsl #16

	ldr	r8, [r3], r12
	ldr	r9, [r3], r12

	uxtb16	r6, r6
	uxtb16	r7, r7

	pkhbt	lr, r9, r8, lsl #16
	pkhtb	r2, r8, r9, asr #16

	uxtb16	r8, r4, ror #8
	uxtb16	r9, r2, ror #8

	strd	r8, r9, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strd	r8, r9, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r8, r5, ror #8
	uxtb16	r9, lr, ror #8

	strd	r8, r9, [r1], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	strd	r8, r9, [r1], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	uxtb16	r8, r4
	uxtb16	r4, r5
	uxtb16	r5, lr

	ldr	lr, [sp, #0x20]

	uxtb16	r9,  r2
	uxtb16	r10, r10
	uxtb16	r11, r11

	stmia	r0!, {r4, r5, r6, r7, r8, r9, r10, r11}

	add	r3, r3, #4
	sub	r3, r3, r12, lsl #3
	cmp	r0, lr
	pld	[r3, #0x20]

	bne	%b10

	pop	{r4-r11, r12, pc}
}

void JpegMpEncoderAsm_Yuyv8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8






    LDR     R12, [R0,#0x328]
    LDR     R3, [R0,#0x308]
    ADD     R0, R0, #0
    PUSH    {R4-R11,R12, LR}
    MOV     R2, R2,LSL#5
    ADD     R3, R3, R1,LSL#5
    MLA     R3, R2, R12, R3
    MOV     R12, R12,LSL#1
    ADD     R1, R0, #0x200

10

	ldr	r6, [r3], r12
	ldr	r7, [r3], r12
	ldr	r8, [r3], r12
	ldr	r9, [r3], r12
	ldr	r10, [r3], r12
	ldr	r11, [r3], r12
	ldr	lr, [r3], r12
	ldr	r2, [r3], r12

	pkhbt	r4, r6, r7, lsl #16
	pkhtb	r5, r7, r6, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5

	str	r4, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	str	r5, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pkhbt	r4, r2, lr, lsl #16
	pkhtb	r5, lr, r2, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  2 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xa - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r10, r11, lsl #16
	pkhtb	r5, r11, r10, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  4 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xc - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r9, r8, lsl #16
	pkhtb	r5, r8, r9, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  6 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xe - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r6, r8, lsl #16
	pkhtb	r5, r8, r6, asr #16
	pkhbt	r6, r7, r9, lsl #16
	pkhtb	r7, r9, r7, asr #16
	pkhbt	r8, lr, r10, lsl #16
	pkhtb	r9, r10, lr, asr #16
	pkhbt	r10, r2, r11, lsl #16
	pkhtb	r11, r11, r2, asr #16

	uhadd8	lr, r8, r10
	uhadd8	r2, r9, r11
	uhadd8	r10, r4, r6
	uhadd8	r11, r5, r7

	uxtb16	lr, lr, ror #8

	str	lr, [r1, #6 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	uxtb16	r2, r2, ror #8

	str	r2, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 6 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r11, r11, ror #8

	str	r11, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	uxtb16	r5, r10, ror #8

	ldr	r6, [r3], r12
	ldr	r7, [r3], r12
	ldr	r8, [r3], r12
	ldr	r9, [r3], r12
	ldr	r10, [r3], r12
	ldr	r11, [r3], r12
	ldr	lr, [r3], r12
	ldr	r2, [r3], r12

	str	r5, [r1], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pkhbt	r4, r6, r7, lsl #16
	pkhtb	r5, r7, r6, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5

	str	r4, [r0, #(      0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(  8 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r2, lr, lsl #16
	pkhtb	r5, lr, r2, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  2 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xa + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r10, r11, lsl #16
	pkhtb	r5, r11, r10, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  4 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xc + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r9, r8, lsl #16
	pkhtb	r5, r8, r9, asr #16
	uxtb16	r4, r4
	uxtb16	r5, r5
	str	r4, [r0, #(  6 + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	str	r5, [r0, #(0xe + 0x80 - 0x10) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhbt	r4, r6, r8, lsl #16
	pkhtb	r5, r8, r6, asr #16
	pkhbt	r6, r7, r9, lsl #16
	pkhtb	r7, r9, r7, asr #16
	pkhbt	r8, lr, r10, lsl #16
	pkhtb	r9, r10, lr, asr #16
	pkhbt	r10, r2, r11, lsl #16
	pkhtb	r11, r11, r2, asr #16

	uhadd8	lr, r8, r10
	uhadd8	r2, r9, r11
	uhadd8	r10, r4, r6
	uhadd8	r11, r5, r7

	uxtb16	lr, lr, ror #8

	str	lr, [r1, #(2 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	uxtb16	r2, r2, ror #8

	str	r2, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + (2 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r11, r11, ror #8

	str	r11, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + (4 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r10, r10, ror #8

	str	r10, [r1, #(4 - 8) * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	sub	r4, r1, r0
	add	r3, r3, #4
	sub	r3, r3, r12, lsl #4

	cmp	r4, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	pld	[r3, #0x20]
	bne	%b10

	pop  {r4-r11, r12, pc}





}

       void JpegMpEncoderAsm_Yuyv8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Yuyv8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8






	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push {r4-r11, r12, lr}

	mov	r2, r2, lsl #3 + 1
	add	r3, r3, r1, lsl #4 + 1

	mla	r3, r2, r12, r3

	mov	r12, r12, lsl #1

	add	r1, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

10

	ldr	r8, [r3], r12
	ldr	r9, [r3], r12
	ldr	r11, [r3], r12
	ldr	r10, [r3], r12
	ldr	r6, [r3], r12
	ldr	r4, [r3], r12

	pkhbt	r7, r10, r11, lsl #16
	pkhtb	r11, r11, r10, asr #16
	pkhtb	r10, r4, r6, asr #16
	pkhbt	r6, r6, r4, lsl #16

	uxtb16	r4, r10, ror #8
	uxtb16	r5, r11, ror #8

	strd	r4, r5, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	uxtb16	r4, r6, ror #8
	uxtb16	r5, r7, ror #8

	strd	r4, r5, [r1, #4 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pkhtb	r4, r9, r8, asr #16
	pkhbt	r5, r8, r9, lsl #16

	ldr	r8, [r3], r12
	ldr	r9, [r3], r12

	uxtb16	r6, r6
	uxtb16	r7, r7

	pkhbt	lr, r9, r8, lsl #16
	pkhtb	r2, r8, r9, asr #16

	uxtb16	r8, r4, ror #8
	uxtb16	r9, r2, ror #8

	strd	r8, r9, [r1, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	uxtb16	r8, r5, ror #8
	uxtb16	r9, lr, ror #8

	strd	r8, r9, [r1], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	uxtb16	r8, r4
	uxtb16	r4, r5
	uxtb16	r5, lr
	uxtb16	r9, r2
	uxtb16	r10, r10
	uxtb16	r11, r11

	stmia	r0!, {r4, r5, r6, r7, r8, r9, r10, r11}

	sub	r4, r1, r0
	add	r3, r3, #4
	sub	r3, r3, r12, lsl #3

	cmp	r4, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (8 * 8 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	pld	[r3, #0x20]
	bne	%b10

	pop  {r4-r11, r12, pc}





}

       void JpegMpEncoderAsm_CtrRgb565ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x2)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #3 + 1

	add	r1, r12, #8 * 2

	mov	r4, #0x00000010

	mla	r3, r2, r1, r3

	adr	r5, constTblCenterCtrRgb565To444

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	r1, [r3], #4

	ldmib	r5, {r10, r11, r12, lr}

10

	and	r8, r10, r1, lsr #3
	and	r10, r10, r10, lsl #1
	and	r9, r10, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r10, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6,   r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1,   r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6,   r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	ldmia	r5, {r2, r10, r11, r12}

	usat	r6,   #8, r6,   asr #12
	usat	r7, #8, r7, asr #12

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	ldrcc	r1, [r3], #4

	strh	r6,   [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]
	sub	r2, r4, #0x1e

	subs	r4, r4, #0x00000002
	ldrnesh	r6, [r5, r2]

	ldrne	r1, [r3, r1]!

	addne	r3, r3, #4
	addne	r0, r0, r6

	bne	%b10

	pop  {r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	0x00fc00fc
	dcd	__cpp((s_Coef565G2yMul << 16)  | s_Coef565R2yMul)
	dcd	__cpp(s_Coef565Rgb2yAdd)
	dcd	__cpp((s_Coef565B2crMul << 16) | s_Coef565B2yMul)









}

       void JpegMpEncoderAsm_CtrRgb565ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8












	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x2)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #4

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1

	add	r1, r12, #8 * 2

	mov	r4, #0x00000020

	mla	r3, r2, r1, r3

	adr	r5, constTblCenterCtrRgb565To420

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	r7, [r3, r1]

	ldmia	r5, {r10, r11, r12, lr}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	push	{r0, r1, r2, r6}

	mov	r1, r7

10
	str	r0, [sp, #0x00]
	pld	[r3]

	and	r8, r10, r1, lsr #3

	and	r2,   r10, r10, lsl #1
	and	r9, r2, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r2, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldr	r6, [r3], #4

	add	r7, r7, r8

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	and	r8, r10, r6, lsr #3
	and	r10, r10, r10, lsl #1
	and	r2,   r10, r6, lsr #8
	and	r10, r10, r6, lsl #3

	pkhbt	r6,   r2,   r8, lsl #16
	pkhtb	r8, r8, r2,   asr #16

	smlad	r1, r6,   r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r10, r11, r12}

	adds	r4, r4, #0x20000000

	usat	r2, #8, r2, asr #14
	ldrcc	r1, [r3, r1]
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	r6, [sp, #0x30]

	subs	r4, r4, #0x00000004

	popeq  {r0-r3, r4-r11, r12, pc}

	sub	r2, r4, #0x2c

	add	r3, r3, r6, lsl #1

	ldr	r2, [r5, r2]
	ldr	r6, [sp, #0x08]

	ldr	r1, [r3, r1]

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

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










}

       void JpegMpEncoderAsm_CtrRgb565ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x2)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1

	add	r1, r12, #16 * 2

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenterCtrRgb565To422

	mov	r4, #0x00000010

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldr	r1, [r3], #4

	ldmib	r5, {r10, r11, r12, lr}

10

	and	r8, r10, r1, lsr #3
	and	r10, r10, r10, lsl #1
	and	r9, r10, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r10, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7
	ldmia	r5, {r2, r10, r11, r12}

	adds	r4, r4, #0x20000000

	ldrcc	r1, [r3], #4

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]

	ldrsh	r6, [r2, r4]

	subs	r4, r4, #0x00000002

	ldrne	r1, [r3, r1]!

	addne	r3, r3, #4
	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop  {r4-r11, r12, pc}

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








}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #3 + 1

	add	r3, r3, r1, lsl #3 + 1 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push {r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgb565Block8To444

	mov	r4, #0x00000008

	ldr	r1, [r3]

	ldmib	r5, {r10, r11, r12, lr}

10

	and	r8, r10, r1, lsr #3
	and	r10, r10, r10, lsl #1
	and	r9, r10, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r10, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6,   r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1,   r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6,   r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	adr	r1, srcOffsetTblCtrRgb565Block8To444 - 4

	ldmia	r5, {r2, r10, r11, r12}

	usat	r6,   #8, r6,   asr #12
	usat	r7, #8, r7, asr #12

	ldrb	r1, [r1, r4, ror #30]

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	teq	r1, #0x00
	ldrne	r1, [r3, r1]

	strh	r6,   [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001
	ldrnesh	r6, [r2, #-0x1e]

	addne	r0, r0, r6

	bne	%b10

	pop  {r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
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








}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8











	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #4 + 1

	add	r3, r3, r1, lsl #4 + 1 + 3

	push {r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	mov	r4, #0x00000008

	adr	r5, constTblCenterCtrRgb565Block8To420

	add	r12, r3, r12, lsl #3 + 1
	str	r12, [sp, #0x30 - 0x10]

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	r7, [r3, #0x02*2]!

	ldmia	r5, {r10, r11, r12, lr}

	sub	r1, r3, #0x02*2

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{r0, r1, r2, r6}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	mov	r1, r7

10
	str	r0, [sp, #0x00]
	pld	[r3]

	and	r8, r10, r1, lsr #3

	and	r2,   r10, r10, lsl #1
	and	r9, r2, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r2, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldr	r6, [r3, #-0x02*2]

	adr	r3, srcOffsetTblCtrRgb565Block8To420 - 8

	add	r7, r7, r8

	ldrb	r3, [r3, r4, ror #29]

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	and	r8, r10, r6, lsr #3
	and	r10, r10, r10, lsl #1
	and	r2,   r10, r6, lsr #8
	and	r10, r10, r6, lsl #3

	pkhbt	r6,   r2,   r8, lsl #16
	pkhtb	r8, r8, r2,   asr #16

	smlad	r1, r6,   r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r10, r11, r12}

	adds	r4, r4, #0x20000000

	teq	r3, #0x00
	ldrne	r1, [r3, r1]!

	usat	r2, #8, r2, asr #14
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	r4, r4, #0x00000001

	addne	r2, r5, r4, lsl #2
	ldrne	r6, [sp, #0x08]
	ldrne	r2, [r2, #-0x2c]

	popeq  {r0-r3, r4-r11, r12, pc}

	cmp	r4, #0x00000004

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

	bne	%b10

	ldr	r3, [sp, #0x30]

	str	r3, [sp, #0x04]
	ldr	r1, [r3, #0x02*2]!

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









}

       void JpegMpEncoderAsm_CtrRgb565Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb565Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #3 + 1

	add	r3, r3, r1, lsl #4 + 1 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push {r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgb565Block8To422

	mov	r4, #0x00000008

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldr	r1, [r3]

	ldmia	r5, {r10, r11, r12, lr}

10

	and	r8, r10, r1, lsr #3
	and	r10, r10, r10, lsl #1
	and	r9, r10, r1, lsr #8

	pkhbt	r7, r9, r8, lsl #16
	pkhtb	r8, r8, r9, asr #16
	and	r9, r10, r1, lsl #3

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	r1, srcOffsetTblCtrRgb565Block8To422 - 8

	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	r1, [r1, r4, ror #29]

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7

	ldmia	r5, {r10, r11, r12}

	adds	r4, r4, #0x20000000

	teq	r1, #0x00
	ldrne	r1, [r3, r1]

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001

	ldrsh	r6, [r2, #-0x1e]

	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop  {r4-r11, r12, pc}

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








}

       void JpegMpEncoderAsm_Rgb8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #3 + 1
	add	r3, r3, r1, lsl #3

	add	r1, r12, #8 * 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Rgb8To444

	mov	r4, #0x00000010

	ldr	r1, [r3], #2
	ldr	r10, [r3], #4

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhbt	r8, r10, r10, lsl #8
	pkhtb	r9, r1, r10, asr #24
	uxtb16	r7, r7
	uxtb16	r8, r8, ror #8
	uxtb16	r9, r9, ror #16

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6,   r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1,   r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6,   r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	ldmia	r5, {r2, r11, r12}

	usat	r6,   #8, r6,   asr #12
	usat	r7, #8, r7, asr #12

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	ldrcc	r1, [r3], #2
	ldrcc	r10, [r3], #4

	strh	r6,   [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]
	sub	r2, r4, #0x1e

	subs	r4, r4, #0x00000002
	ldrnesh	r6, [r5, r2]

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #2]

	addne	r3, r3, #2 * 3
	addne	r0, r0, r6

	bne	%b10

	pop  {r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)









}

       void JpegMpEncoderAsm_Rgb8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8












	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #4

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1
	add	r3, r3, r1, lsl #4

	add	r1, r12, #8 * 3

	adr	r5, constTblCenter_Rgb8To420

	mla	r3, r2, r1, r3

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	r4, #0x00000020

	add	r8, r3, r1

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	r10, [r8]
	ldr	r8, [r8, #2]

	ldmia	r5, {r11, r12, lr}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	push	{r0, r1, r2, r6}

10
	str	r0, [sp, #0x00]
	pld	[r3]

	pkhbt	r7, r10, r10, lsl #8
	pkhtb	r9, r10, r8, asr #24
	pkhbt	r8, r8, r8, lsl #8
	uxtb16	r7, r7
	uxtb16	r8, r8, ror #8
	uxtb16	r9, r9, ror #16

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldr	r10, [r3], #2

	add	r7, r7, r8

	ldr	r8, [r3], #4

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhbt	r6, r10, r10, lsl #8
	pkhtb	r10, r10, r8, asr #24
	pkhbt	r8, r8, r8, lsl #8
	uxtb16	r6, r6
	uxtb16	r8, r8, ror #8
	uxtb16	r10, r10, ror #16

	smlad	r1, r6,   r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	usat	r2, #8, r2, asr #14
	ldrcc	r10, [r1, r3]!
	usat	r6, #8, r6, asr #14
	ldrcc	r8, [r1, #2]

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	r6, [sp, #0x30]

	subs	r4, r4, #0x00000004

	popeq  {r0-r3, r4-r11, r12, pc}

	sub	r2, r4, #0x2c

	add	r3, r3, r6, lsl #1

	ldr	r2, [r5, r2]
	ldr	r6, [sp, #0x08]

	ldr	r10, [r1, r3]!

	and	r7, r2, #0xff
	ldr	r8, [r1, #2]
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

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










}

       void JpegMpEncoderAsm_Rgb8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgb8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push {r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1
	add	r3, r3, r1, lsl #4

	add	r1, r12, #16 * 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Rgb8To422

	mov	r4, #0x00000010

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldr	r1, [r3], #2
	ldr	r10, [r3], #4

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhbt	r8, r10, r10, lsl #8
	pkhtb	r9, r1, r10, asr #24
	uxtb16	r7, r7
	uxtb16	r8, r8, ror #8
	uxtb16	r9, r9, ror #16

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7
	ldmia	r5, {r2, r11, r12}

	adds	r4, r4, #0x20000000

	ldrcc	r1, [r3], #2
	ldrcc	r10, [r3], #4

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]

	ldrsh	r6, [r2, r4]

	subs	r4, r4, #0x00000002

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #2]

	addne	r3, r3, #2 * 3
	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop  {r4-r11, r12, pc}

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








}

       void JpegMpEncoderAsm_CtrRgb8Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	r2, r2, lsl #3

	add	r3, r3, r1, lsl #3 + 1 + 3
	add	r3, r3, r1, lsl #3 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push {r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgb8Block8To444

	mov	r4, #0x00000008

	ldr	r10, [r3]
	ldr	r1, [r3, #2]

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r10, r10, lsl #8
	pkhtb	r8, r1, r1, asr #8
	pkhbt	r9, r10, r1, lsl #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6,   r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1,   r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6,   r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	adr	r1, srcOffsetTblCtrRgb8Block8To444 - 4

	ldmia	r5, {r2, r11, r12}

	usat	r6,   #8, r6,   asr #12
	usat	r7, #8, r7, asr #12

	ldrb	r1, [r1, r4, ror #30]

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	teq	r1, #0x00
	ldrne	r10, [r1, r3]!
	ldrne	r1, [r1, #2]

	strh	r6,   [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001
	ldrnesh	r6, [r2, #-0x1e]

	addne	r0, r0, r6

	bne	%b10

	pop  {r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
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








}

void JpegMpEncoderAsm_CtrRgb8Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	r2, r2, lsl #4

	add	r3, r3, r1, lsl #4 + 1 + 3
	add	r3, r3, r1, lsl #4 + 3

	push	{r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	mov	r4, #0x00000008

	adr	r5, constTblCenterCtrRgb8Block8To420

	add	r12, r3, r12, lsl #3
	str	r12, [sp, #0x30 - 0x10]

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	r7, [r3, #(0x02*3) + 2]!
	ldr	r10, [r3, #-2]!

	ldmia	r5, {r11, r12, lr}

	sub	r1, r3, #(0x02*3)

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{r0, r1, r2, r6}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	mov	r1, r7

10
	str	r0, [sp, #0x00]

	pkhbt	r7, r10, r10, lsl #8
	pkhtb	r8, r1, r1, asr #8
	pkhbt	r9, r10, r1, lsl #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldr	r10, [r3, #-(0x02*3)]
	ldr	r6, [r3, #-(0x02*3) + 2]

	adr	r3, srcOffsetTblCtrRgb8Block8To420 - 8

	add	r7, r7, r8

	ldrb	r3, [r3, r4, ror #29]

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhbt	r2, r10, r10, lsl #8
	pkhtb	r8, r6, r6, asr #8
	pkhbt	r10, r10, r6, lsl #8
	uxtb16	r6, r2, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r10, r10

	smlad	r1, r6, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	add	r1, r1, r3, lsl #1
	teq	r3, #0x00
	ldrne	r10, [r3, r1]!
	ldrne	r1, [r3, #2]

	usat	r2, #8, r2, asr #14
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	r4, r4, #0x00000001

	addne	r2, r5, r4, lsl #2
	ldrne	r6, [sp, #0x08]
	ldrne	r2, [r2, #-0x2c]

	popeq	{r0-r3, r4-r11, r12, pc}

	cmp	r4, #0x00000004

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

	bne	%b10

	ldr	r3, [sp, #0x30]

	str	r3, [sp, #0x04]
	ldr	r10, [r3, #0x03*2]!
	ldr	r1, [r3, #2]

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
}

       void JpegMpEncoderAsm_CtrRgb8Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgb8Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then Y output pointer    r1, r2 = col, row, then temps
	; r3 = source pointer                 r4 = loop counter    r5 = constant table
	; r6 = CbCr output pointer            r7 = g0r0   r8 = g1r1   r9 = b1b0
	; r10-r12, lr = mask/constants

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthX3)]

	mov	r2, r2, lsl #3

	add	r3, r3, r1, lsl #4 + 1 + 3
	add	r3, r3, r1, lsl #4 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push	{r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgb8Block8To422

	mov	r4, #0x00000008

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldr	r10, [r3]
	ldr	r1, [r3, #2]

	ldmia	r5, {r11, r12, lr}

10
	pkhbt	r7, r10, r10, lsl #8
	pkhtb	r8, r1, r1, asr #8
	pkhbt	r9, r10, r1, lsl #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	r1, srcOffsetTblCtrRgb8Block8To422 - 8

	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	r1, [r1, r4, ror #29]

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7

	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	add	r1, r1, r1, lsl #1
	teq	r1, #0x00
	ldrne	r10, [r1, r3]!
	ldrne	r1, [r1, #2]

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001

	ldrsh	r6, [r2, #-0x1e]

	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop	{r4-r11, r12, pc}

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
}

       void JpegMpEncoderAsm_Rgba8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then Y output pointer    r1, r2 = col, row, then temps
	; r3 = source pointer                 r4 = loop counter    r5 = constant table
	; r6 = temp   r7 = g0r0   r8 = g1r1   r9 = b1b0   r10-r12, lr = mask/constants
	; [sp, #0x20] = source stride

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #3 + 2

	add	r1, r12, #8 * 4

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Rgba8To444

	mov	r4, #0x00000010

	ldmia	r3!, {r1, r10}

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhbt	r8, r10, r10, lsl #8
	pkhtb	r9, r10, r1, asr #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6, r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1, r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6, r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	ldmia	r5, {r2, r11, r12}

	usat	r6, #8, r6, asr #12
	usat	r7, #8, r7, asr #12

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	ldmiacc	r3!, {r1, r10}

	strh	r6, [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]
	sub	r2, r4, #0x1e

	subs	r4, r4, #0x00000002
	ldrnesh	r6, [r5, r2]

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #4]

	addne	r3, r3, #2 * 4
	addne	r0, r0, r6

	bne	%b10

	pop	{r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)
}

       void JpegMpEncoderAsm_Rgba8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then pDst      r1, r2 = col, row, then temps    r3 = source pointer
	; r4 = loop counter         r5 = constant table              r6 = temp
	; r7 = g0r0   r8 = g1r1     r9 = b1b0    r10-r12, lr = mask/constants
	; Stack: [sp+0x00] pDstCbCr  [sp+0x08] pDstY0  [sp+0x0c] pDstY1  [sp+0x30] stride

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #4

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 2

	add	r1, r12, #8 * 4

	adr	r5, constTblCenter_Rgba8To420

	mla	r3, r2, r1, r3

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	r4, #0x00000020

	add	r8, r3, r1

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldmia	r8, {r8, r10}

	ldmia	r5, {r11, r12, lr}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	push	{r0, r1, r2, r6}

10
	str	r0, [sp, #0x00]
	pld	[r3]

	pkhbt	r7, r8, r8, lsl #8
	pkhtb	r9, r10, r8, asr #16
	pkhbt	r8, r10, r10, lsl #8
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	add	r7, r7, r8

	ldmia	r3!, {r8, r10}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhbt	r6, r8, r8, lsl #8
	pkhtb	r0, r10, r8, asr #16
	pkhbt	r8, r10, r10, lsl #8
	uxtb16	r6, r6
	uxtb16	r8, r8
	uxtb16	r10, r0

	smlad	r1, r6, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	addcc	r8, r3, r1

	usat	r2, #8, r2, asr #14
	ldmiacc	r8, {r8, r10}
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	r6, [sp, #0x30]

	subs	r4, r4, #0x00000004

	popeq	{r0-r3, r4-r11, r12, pc}

	sub	r2, r4, #0x2c

	add	r3, r3, r6, lsl #1

	ldr	r2, [r5, r2]
	add	r8, r3, r1
	ldr	r6, [sp, #0x08]

	ldmia	r8, {r8, r10}

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

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
}

       void JpegMpEncoderAsm_Rgba8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_Rgba8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then Y output pointer    r1, r2 = col, row, then temps
	; r3 = source pointer                 r4 = loop counter    r5 = constant table
	; r6 = CbCr output pointer            r7 = g0r0   r8 = g1r1   r9 = b1b0
	; r10-r12, lr = mask/constants        [sp, #0x20] = source stride

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 2

	add	r1, r12, #16 * 4

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Rgba8To422

	mov	r4, #0x00000010

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldmia	r3!, {r1, r10}

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhbt	r8, r10, r10, lsl #8
	pkhtb	r9, r10, r1, asr #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7
	ldmia	r5, {r2, r11, r12}

	adds	r4, r4, #0x20000000

	ldmiacc	r3!, {r1, r10}

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]

	ldrsh	r6, [r2, r4]

	subs	r4, r4, #0x00000002

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #4]

	addne	r3, r3, #2 * 4
	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop	{r4-r11, r12, pc}

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
}

       void JpegMpEncoderAsm_CtrRgba8Block8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);
NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then Y output pointer    r1, r2 = col, row, then temps
	; r3 = source pointer                 r4 = loop counter    r5 = constant table
	; r6 = temp   r7 = g0r0   r8 = g1r1   r9 = b1b0   r10-r12, lr = mask/constants

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #3 + 2

	add	r3, r3, r1, lsl #3 + 2 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push	{r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgba8Block8To444

	mov	r4, #0x00000008

	ldmia	r3, {r1, r10}

	ldmib	r5, {r11, r12, lr}

10
	pkhtb	r7, r1, r1, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r9, r1, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6, r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1, r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6, r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	adr	r1, srcOffsetTblCtrRgba8Block8To444 - 4

	ldmia	r5, {r2, r11, r12}

	usat	r6, #8, r6, asr #12
	usat	r7, #8, r7, asr #12

	ldrb	r1, [r1, r4, ror #30]

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	add	r10, r3, r1, lsl #2
	teq	r1, #0x00
	ldmiane	r10, {r1, r10}

	strh	r6, [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001
	ldrnesh	r6, [r2, #-0x1e]

	addne	r0, r0, r6

	bne	%b10

	pop	{r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
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
}

void JpegMpEncoderAsm_CtrRgba8Block8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #4 + 2

	add	r3, r3, r1, lsl #4 + 2 + 3

	push	{r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	mov	r4, #0x00000008

	adr	r5, constTblCenterCtrRgba8Block8To420

	add	r12, r3, r12, lsl #3 + 2
	str	r12, [sp, #0x30 - 0x10]

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	ldr	r7, [r3, #(0x02*4)]!
	ldr	r10, [r3, #4]

	ldmia	r5, {r11, r12, lr}

	sub	r1, r3, #(0x02*4)

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	push	{r0, r1, r2, r6}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	mov	r1, r7

10
	str	r0, [sp, #0x00]

	pkhtb	r7, r1, r1, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r9, r1, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldmdb	r3, {r6, r10}

	adr	r3, srcOffsetTblCtrRgba8Block8To420 - 8

	add	r7, r7, r8

	ldrb	r3, [r3, r4, ror #29]

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhtb	r2, r6, r6, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r10, r6, r10, lsl #16
	uxtb16	r6, r2
	uxtb16	r8, r8
	uxtb16	r10, r10, ror #8

	smlad	r1, r6, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	add	r3, r1, r3, lsl #2
	teq	r3, r1
	ldmiane	r3, {r1, r10}

	usat	r2, #8, r2, asr #14
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	subs	r4, r4, #0x00000001

	addne	r2, r5, r4, lsl #2
	ldrne	r6, [sp, #0x08]
	ldrne	r2, [r2, #-0x2c]

	popeq	{r0-r3, r4-r11, r12, pc}

	cmp	r4, #0x00000004

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

	bne	%b10

	ldr	r1, [sp, #0x30]

	add	r3, r1, #(0x02*4)
	str	r1, [sp, #0x04]
	ldmia	r3, {r1, r10}

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
}

void JpegMpEncoderAsm_CtrRgba8Block8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_CtrRgba8Block8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, inputBufferWidth)]

	mov	r2, r2, lsl #3 + 2

	add	r3, r3, r1, lsl #4 + 2 + 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	push	{r4-r11, r12, lr}

	mla	r3, r2, r12, r3

	adr	r5, constTblCenterCtrRgba8Block8To422

	mov	r4, #0x00000008

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldmia	r3, {r1, r10}

	ldmia	r5, {r11, r12, lr}

10
	pkhtb	r7, r1, r1, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r9, r1, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	adr	r1, srcOffsetTblCtrRgba8Block8To422 - 8

	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldrb	r1, [r1, r4, ror #29]

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7

	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	add	r1, r3, r1, lsl #2
	teq	r1, r3
	ldmiane	r1, {r1, r10}

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	add	r2, r5, r4, lsl #1

	subs	r4, r4, #0x00000001

	ldrsh	r6, [r2, #-0x1e]

	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop	{r4-r11, r12, pc}

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
}

void JpegMpEncoderAsm_Bgr8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #3 + 1
	add	r3, r3, r1, lsl #3

	add	r1, r12, #8 * 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Bgr8To444

	mov	r4, #0x00000010

	ldr	r1, [r3], #2
	ldr	r10, [r3], #4

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhtb	r8, r10, r10, asr #8
	pkhbt	r9, r1, r10, lsl #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6, r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1, r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6, r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	ldmia	r5, {r2, r11, r12}

	usat	r6, #8, r6, asr #12
	usat	r7, #8, r7, asr #12

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	ldrcc	r1, [r3], #2
	ldrcc	r10, [r3], #4

	strh	r6, [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]
	sub	r2, r4, #0x1e

	subs	r4, r4, #0x00000002
	ldrnesh	r6, [r5, r2]

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #2]

	addne	r3, r3, #2 * 3
	addne	r0, r0, r6

	bne	%b10

	pop	{r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)
}

void JpegMpEncoderAsm_Bgr8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #4

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1
	add	r3, r3, r1, lsl #4

	add	r1, r12, #8 * 3

	adr	r5, constTblCenter_Bgr8To420

	mla	r3, r2, r1, r3

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	r4, #0x00000020

	add	r8, r3, r1

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldr	r10, [r8]
	ldr	r8, [r8, #2]

	ldmia	r5, {r11, r12, lr}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	push	{r0, r1, r2, r6}

10
	str	r0, [sp, #0x00]
	pld	[r3]

	pkhbt	r7, r10, r10, lsl #8
	pkhbt	r9, r10, r8, lsl #8
	pkhtb	r8, r8, r8, asr #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	ldr	r10, [r3], #2

	add	r7, r7, r8

	ldr	r8, [r3], #4

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhbt	r6, r10, r10, lsl #8
	pkhbt	r10, r10, r8, lsl #8
	pkhtb	r8, r8, r8, asr #8
	uxtb16	r6, r6, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r10, r10

	smlad	r1, r6, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	usat	r2, #8, r2, asr #14
	ldrcc	r10, [r1, r3]!
	usat	r6, #8, r6, asr #14
	ldrcc	r8, [r1, #2]

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	r6, [sp, #0x30]

	subs	r4, r4, #0x00000004

	popeq	{r0-r3, r4-r11, r12, pc}

	sub	r2, r4, #0x2c

	add	r3, r3, r6, lsl #1

	ldr	r2, [r5, r2]
	ldr	r6, [sp, #0x08]

	ldr	r10, [r1, r3]!

	and	r7, r2, #0xff
	ldr	r8, [r1, #2]
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

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
}

void JpegMpEncoderAsm_Bgr8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Bgr8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x3)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 1
	add	r3, r3, r1, lsl #4

	add	r1, r12, #16 * 3

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Bgr8To422

	mov	r4, #0x00000010

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldr	r1, [r3], #2
	ldr	r10, [r3], #4

	ldmib	r5, {r11, r12, lr}

10
	pkhbt	r7, r1, r1, lsl #8
	pkhtb	r8, r10, r10, asr #8
	pkhbt	r9, r1, r10, lsl #8
	uxtb16	r7, r7, ror #24
	uxtb16	r8, r8, ror #24
	uxtb16	r9, r9

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7
	ldmia	r5, {r2, r11, r12}

	adds	r4, r4, #0x20000000

	ldrcc	r1, [r3], #2
	ldrcc	r10, [r3], #4

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]

	ldrsh	r6, [r2, r4]

	subs	r4, r4, #0x00000002

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #2]

	addne	r3, r3, #2 * 3
	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop	{r4-r11, r12, pc}

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
}

void JpegMpEncoderAsm_Abgr8ToYuv444(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv444(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	; r0 = pCtx, then Y output pointer     r1, r2 = col, row, then temps
	; r3 = source pointer                  r4 = loop counter     r5 = constant table
	; r6 = temp   r7 = g0r0   r8 = g1r1   r9 = b1b0   r10-r12, lr = constants/mask
	; [sp, #0x20] = source stride

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #3 + 2

	add	r1, r12, #8 * 4

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Abgr8To444

	mov	r4, #0x00000010

	ldmia	r3!, {r1, r10}

	ldmib	r5, {r11, r12, lr}

10
	pkhtb	r7, r1, r1, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r9, r1, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	smlad	r6, r7, r10, r11
	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlad	r1, r8, r12, r11
	smlad	r8, r8, r10, r11

	smlabb	r6, r9, r12, r6
	smlabt	r7, r9, lr, r7

	smlatb	r8, r9, r12, r8
	smlatt	r9, r9, lr, r1

	ldmia	r5, {r2, r11, r12}

	usat	r6, #8, r6, asr #12
	usat	r7, #8, r7, asr #12

	usat	r8, #8, r8, asr #12
	usat	r9, #8, r9, asr #12

	adds	r4, r4, #0x40000000

	ldmiacc	r3!, {r1, r10}

	strh	r6, [r2, r0]!
	strh	r7, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r8, [r2, #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]
	strh	r9, [r2, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)) + 8 * __cpp(sizeof(JpegMpEncoderYCbCrType))]

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]
	sub	r2, r4, #0x1e

	subs	r4, r4, #0x00000002
	ldrnesh	r6, [r5, r2]

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #4]

	addne	r3, r3, #2 * 4
	addne	r0, r0, r6

	bne	%b10

	pop	{r4-r11, r12, pc}

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
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf) - (0x10 * __cpp(sizeof(JpegMpEncoderYCbCrType)))
	dcd	__cpp((s_Coef8G2yMul << 16)  | s_Coef8R2yMul)
	dcd	__cpp(s_Coef8Rgb2yAdd)
	dcd	__cpp((s_Coef8B2crMul << 16) | s_Coef8B2yMul)
}

void JpegMpEncoderAsm_Abgr8ToYuv420(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv420(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM8x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #4

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 2

	add	r1, r12, #8 * 4

	adr	r5, constTblCenter_Abgr8To420

	mla	r3, r2, r1, r3

	add	r2, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mov	r4, #0x00000020

	add	r8, r3, r1

	add	r6, r2, #1 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	ldmia	r8, {r8, r10}

	ldmia	r5, {r11, r12, lr}

	add	r0, r2, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	push	{r0, r1, r2, r6}

10
	str	r0, [sp, #0x00]
	pld	[r3]

	pkhtb	r7, r8, r8, asr #24
	pkhtb	r1, r10, r10, asr #24
	pkhbt	r9, r8, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r1
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	ldr	r0, [sp, #0x0c]

	add	r7, r7, r8

	ldmia	r3!, {r8, r10}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x0c]

	pkhtb	r6, r8, r8, asr #24
	pkhtb	r2, r10, r10, asr #24
	pkhbt	r10, r8, r10, lsl #16
	uxtb16	r6, r6
	uxtb16	r8, r2
	uxtb16	r10, r10, ror #8

	smlad	r1, r6, r11, r12
	smlad	r2, r8, r11, r12
	smlabb	r1, r10, lr, r1
	smlatb	r2, r10, lr, r2

	add	r7, r7, r6

	ldr	r0, [sp, #0x08]

	ldmdb	r5, {r6, r11, r12}

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	add	r7, r7, r8

	add	r9, r9, r10

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	str	r0, [sp, #0x08]

	add	r9, r9, r9, lsr #16

	smlad	r2, r7, r12, r11
	smlad	r1, r7, r6, r11
	smlabt	r2, r9, lr, r2

	smlabb	r6, r9, r12, r1

	ldrd	r0, r1, [sp, #0x00]
	ldmia	r5, {r11, r12}

	adds	r4, r4, #0x20000000

	addcc	r8, r3, r1

	usat	r2, #8, r2, asr #14
	ldmiacc	r8, {r8, r10}
	usat	r6, #8, r6, asr #14

	strh	r2, [r0, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]
	strh	r6, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	bcc	%b10

	ldr	r6, [sp, #0x30]

	subs	r4, r4, #0x00000004

	popeq	{r0-r3, r4-r11, r12, pc}

	sub	r2, r4, #0x2c

	add	r3, r3, r6, lsl #1

	ldr	r2, [r5, r2]
	add	r8, r3, r1
	ldr	r6, [sp, #0x08]

	ldmia	r8, {r8, r10}

	and	r7, r2, #0xff
	add	r6, r6, r2, asr #16
	sub	r0, r0, r7
	sxtab	r7, r6, r2, ror #8

	strd	r6, r7, [sp, #0x08]

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
}

void JpegMpEncoderAsm_Abgr8ToYuv422(register JpegMpEncoderContext* pCtx, register u32 col, register u32 row);

NN_ASM void JpegMpEncoderAsm_Abgr8ToYuv422(register JpegMpEncoderContext* pCtx NN_IS_UNUSED_VAR, register u32 col NN_IS_UNUSED_VAR, register u32 row NN_IS_UNUSED_VAR)
{
	PRESERVE8

	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, asmWork.inputBufferWidthM16x4)]

	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, pSrc)]

	mov	r2, r2, lsl #3

	push	{r4-r11, r12, lr}

	add	r3, r3, r1, lsl #4 + 2

	add	r1, r12, #16 * 4

	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderContext, convWork)

	mla	r3, r2, r1, r3

	adr	r5, constTblCenter_Abgr8To422

	mov	r4, #0x00000010

	add	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	ldmia	r3!, {r1, r10}

	ldmib	r5, {r11, r12, lr}

10
	pkhtb	r7, r1, r1, asr #24
	pkhtb	r8, r10, r10, asr #24
	pkhbt	r9, r1, r10, lsl #16
	uxtb16	r7, r7
	uxtb16	r8, r8
	uxtb16	r9, r9, ror #8

	smlad	r1, r7, r11, r12
	smlad	r2, r8, r11, r12

	ldmdb	r5, {r10, r11, r12}

	add	r7, r7, r8

	smlabb	r1, r9, lr, r1
	smlatb	r2, r9, lr, r2

	add	r9, r9, r9, lsr #16

	smlad	r8, r7, r10, r11

	smlad	r7, r7, r12, r11

	usat	r1, #8, r1, asr #12
	usat	r2, #8, r2, asr #12

	strh	r1, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	strh	r2, [r0], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	smlabb	r8, r9, r12, r8
	smlabt	r7, r9, lr, r7
	ldmia	r5, {r2, r11, r12}

	adds	r4, r4, #0x20000000

	ldmiacc	r3!, {r1, r10}

	usat	r7, #8, r7, asr #13
	usat	r8, #8, r8, asr #13

	strh	r7, [r6, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,crBuf)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf))]

	strh	r8, [r6], #8 * __cpp(sizeof(JpegMpEncoderYCbCrType))

	pld	[r3, #0x20]
	bcc	%b10

	ldr	r1, [sp, #0x20]

	ldrsh	r6, [r2, r4]

	subs	r4, r4, #0x00000002

	ldrne	r1, [r3, r1]!
	ldrne	r10, [r3, #4]

	addne	r3, r3, #2 * 4
	addne	r0, r0, r6
	addne	r6, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpEncoderConvertToYuvFuncWork,cbBuf)

	bne	%b10

	pop	{r4-r11, r12, pc}

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
}

void JpegMpEncoderAsm_ForwardDct(register JpegMpEncoderYCbCrType *data, register const JpegMpEncoderQTableType *qtable, register s16 *output);

NN_ASM void JpegMpEncoderAsm_ForwardDct(register JpegMpEncoderYCbCrType *data NN_IS_UNUSED_VAR, register const JpegMpEncoderQTableType *qtable NN_IS_UNUSED_VAR, register s16 *output NN_IS_UNUSED_VAR)
{
	PRESERVE8

	push	{r4-r11, r12, lr}

	sub	sp, sp, #64 * 4

	add	r3, r0, #64 * __cpp(sizeof(JpegMpEncoderYCbCrType))
	adr	r4, loopTable

	push	{r1, r2}

	adr	r2, constTblCenter1
	add	r1, sp, #2 * 4

	ldmdb	r2, {r5, r11}

asm_forwarddct_1st_loop

	ldmia	r0!, {r6, r7, r8, r9}
	mov	r12,      #0x01000000
	orr	r12, r12, #0x00000100

	sadd16	r10, r6, r7
	ssub16	r7, r6, r7
	sadd16	lr, r9, r8
	ssub16	r8, r9, r8

	ssub16	r6, r10, lr

	sadd16	r10, r10, lr

	add	lr, r6, r6, lsl #16

	smlad	r9, r10, r12, r5

	smusd	r10, r10, r12

	smuad	r12, lr, r11

	smusd	lr, lr, r11

	sadd16	r6, r8, r7

	add	r8, r7, r7, lsl #16

	stmia	r1!, {r9, r10, r12, lr}

	pkhbt	r7, r7, r6

	add	r6, r6, r6, asr #16

	ldmia	r2, {r9, r10, r12, lr}

	pkhbt	r8, r6, r8

	smuad	r6, r7, r11
	smusd	r7, r7, r11

	smlad	r9,  r8, r9,  r6
	smlad	r10, r8, r10, r7
	smlad	r12, r8, r12, r7
	smlad	lr,  r8, lr,  r6

	ldrb	r8, [r4], #1
	cmp	r0, r3
	stmia	r1, {r9, r10, r12, lr}
	add	r1, sp, r8

	bcc	asm_forwarddct_1st_loop

asm_forwarddct_2nd_loop

	ldr	r6,  [r1]
	ldr	r7,  [r1, #1*8*4]
	ldr	r8,  [r1, #2*8*4]
	ldr	r9,  [r1, #3*8*4]
	ldr	r10, [r1, #4*8*4]
	ldr	r11, [r1, #5*8*4]
	ldr	r12, [r1, #6*8*4]
	ldr	lr,  [r1, #7*8*4]

	adr	r2, constTblCenter2
	mov	r3, #32768

	add	r5, r6, r7
	sub	r6, r6, r7
	add	r7, r8, r9
	sub	r8, r8, r9
	add	r9, r10, r11
	sub	r10, r10, r11
	add	r11, r12, lr
	sub	r12, r12, lr

	add	lr, r5, r11
	sub	r11, r5, r11
	sub	r5, r7, r9
	add	r7, r7, r9
	add	r9, r5, r11

	add	r5, lr, #128
	add	lr, r5, r7
	sub	r7, r5, r7
	mov	lr, lr, asr #8
	pkhbt	lr, lr, r7, lsl #16 - 8

	ldmdb	r2, {r0, r1}

	add	r12, r12, r6
	add	r10, r10, r8
	add	r8, r8, r6
	add	r12, r12, r10

	mla	r7, r0, r9, r3
	mla	r9, r1, r9, r3
	mla	r5, r0, r10, r3
	mla	r10, r1, r10, r3

	ldmia	r2, {r0, r1, r2, r3}

	add	r7, r7, r11, lsl #8
	add	r9, r9, r11, lsl #8
	add	r11, r5, r6, lsl #8
	add	r10, r10, r6, lsl #8

	mul	r6, r0, r12
	mul	r12, r1, r12

	ldrd	r0, r1, [sp, #0]

	mla	r6, r2, r8, r6
	mla	r8, r3, r8, r12

	ldmia	r0!, {r2, r3}
	mov	r5, #2048

	add	r12, r11, r6
	sub	r6, r11, r6
	add	r11, r10, r8
	sub	r8, r10, r8

	smlatb	r7, r7, r3, r5

	smlatt	r11, r11, r3, r5

	smlabb	r10, lr, r2, r5

	smlatt	r12, r12, r2, r5

	ldmia	r0!, {r2, r3}
	mov	r7, r7, asr #12
	mov	r10, r10, asr #12
	pkhbt	r7, r7, r11, lsl #16 - 12

	smlatb	lr, lr, r2, r5

	smlatt	r8, r8, r2, r5

	smlatb	r9, r9, r3, r5

	smlatt	r5, r6, r3, r5

	pkhbt	r6, r10, r12, lsl #16 - 12

	ldrb	r10, [r4], #1

	mov	lr, lr, asr #12
	pkhbt	r8, lr, r8, lsl #16 - 12
	mov	r9, r9, asr #12
	pkhbt	r9, r9, r5, lsl #16 - 12
	stmia	r1!, {r6, r7, r8, r9}

	cmp	r10, #0xff

	strd	r0, r1, [sp, #0]
	add	r1, sp, r10

	bne	asm_forwarddct_2nd_loop

	add	sp, sp, #(2*4) + (64*4)
	pop	{r4-r11, r12, pc}

loopTable
	dcb	                  2*8*4+2*4, 4*8*4+2*4, 6*8*4+2*4, 7*8*4+2*4, 5*8*4+2*4, 3*8*4+2*4, 1*8*4+2*4
	dcb	0*4+2*4, 4*4+2*4, 2*4+2*4, 6*4+2*4, 1*4+2*4, 5*4+2*4, 3*4+2*4, 7*4+2*4
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
}

#undef ASM_STRUCTURE_MEMBER_OFFSET_OF
#undef ASM_OP_LOG_OR

#pragma arm