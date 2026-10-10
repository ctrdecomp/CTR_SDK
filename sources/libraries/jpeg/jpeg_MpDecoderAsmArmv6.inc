#include <stddef.h>

#pragma arm

#define NN_ASM __asm

#ifndef NN_IS_UNUSED_VAR
#define NN_IS_UNUSED_VAR
#endif

#define ASM_STRUCTURE_MEMBER_OFFSET_OF(s,m) __cpp(offsetof(s,m))

#define ASM_OP_LOG_OR :OR:


       void JpegMpDecoderAsmConvertWorkToAsm(register JpegMpDecoderContext* pCtx);
NN_ASM void JpegMpDecoderAsmConvertWorkToAsm(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8


	ldrsb	r2, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bitlen)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bcode2)]
	ldr	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrc)]
	ldr	r5, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, srcOffset)]
	orr	r3, r3, #1 << 31
	mov	r3, r3, ror r2
	rsb	r2, r2, #31
	mov	r3, r3, lsr r2
	mov	r3, r3, lsl r2
	ldr	r2, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, outputBufferWidth)]
	str	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]

	add	r4, r4, r5
	str	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]

	mov	r3, r2, lsl #1
	str	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	add	r4, r3, r2
	str	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	mov	r4, r2, lsl #2
	str	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	bx	lr

}

       void JpegMpDecoderAsmConvertWorkToC(register JpegMpDecoderContext* pCtx);
NN_ASM void JpegMpDecoderAsmConvertWorkToC(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8


	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	ldr	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	ldr	r5, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrc)]

	rsb	r2, r3, #0
	and	r2, r3, r2
	clz	r2, r2
	strb	r2, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bitlen)]

	rsb	r2, r2, #32
	mov	r2, r3, lsr r2
	str	r2, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bcode2)]

	sub	r4, r4, r5
	str	r4, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, srcOffset)]

	bx	lr

}

       void JpegMpDecoderAsmGetMatrix(register JpegMpDecoderContext* pCtx, register s32 n, register s16 *pFirstElement);
NN_ASM void JpegMpDecoderAsmGetMatrix(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 n NN_IS_UNUSED_VAR, register s16 *pFirstElement NN_IS_UNUSED_VAR)
{
	PRESERVE8





	mov	r12, r2

	push {r0, r4-r11, lr}

	cmp	r2, #0
	moveq	r6, #624
	movne	r6, #(688*2+624)

	add	r8, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[0])

	ldr	r2, offsetJw

	ldr	r5, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrcLimit)]

	ldr	r4,      [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	ldr	r6,     [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	ldrb	r7,    [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, chitFF)]

	add	r12, r0, r2

	movs	r2, r6, lsl #8
	bne	asm_getmatrix_hloadbit01

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_2

	ldrb	r7, [r4], #1
	rsb	r2, r6, #0
	and	r2, r6, r2
	bic	r6, r6, r2
	clz	r2, r2
	add	r2, r2, #8
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orr	r3, r7, #1 << 31
	orr	r6, r6, r3, ror r2

asm_getmatrix_hloadbit01

	add	r2, r12, r6
	add	r2, r2, r6, lsr #24
	ldrb	r3, [r2, #174]
	ldrb	r9, [r2, #430]

	cmp	r3, #0
	beq	asm_getmatrix_hloadbit02

	movs	r6, r6, lsl r3
	beq	asm_getmatrix_set_error_hlb_0

asm_getmatrix_hloadbit07

	movs	r0, r9
	ldr	r5, [sp, #0x20]
	beq	asm_getmatrix_getmatrix1

	mov	r0, #1

	movs	r6, r6, lsl #1
	bne	asm_getmatrix_loadbit01

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_5

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

asm_getmatrix_loadbit01

	rsbcc	r1, r0, r0, lsl r9
	movcs	r1, #0
	sbc	r0, r0, #0

asm_getmatrix_getmtxsb01

	subs	r9, r9, #1

	ble	asm_getmatrix_getmtxsb03

	movs	r6, r6, lsl #1
	adcne	r0, r0, r0
	bne	asm_getmatrix_getmtxsb01

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_5

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

	adc	r0, r0, r0
	b	asm_getmatrix_getmtxsb01

asm_getmatrix_hloadbit02a

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_3

	ldrb	r7, [r4], #1
	rsb	r2, r6, #0
	and	r2, r6, r2
	bic	r6, r6, r2
	clz	r2, r2
	add	r2, r2, #8
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orr	r3, r7, #1 << 31
	orr	r6, r6, r3, ror r2

asm_getmatrix_hloadbit02

	movs	r2, r6, lsl #9

	beq	asm_getmatrix_hloadbit02a

asm_getmatrix_hloadbit03

	add	r4, r12, r6
	mov	r3,    r6, lsr #23
	ldr	r2, [r4, #(17 + 9) * 4]!
	mov	r6, r6, lsl #9

asm_getmatrix_hloadbit04
	cmp	r3, r2

	ble	asm_getmatrix_hloadbit06

asm_getmatrix_hloadbit04a

	movs	r6, r6, lsl #1

	bne	asm_getmatrix_hloadbit05

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_4

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

asm_getmatrix_hloadbit05

	ldr	r2, [r4, #4]!
	adc	r3, r3, r3
	cmp	r3, r2
	ble	asm_getmatrix_hloadbit06

	add	r2, r12, r6
	sub	r2, r4, r2
	cmp	r2, #(17 + 17) * 4
	blt	asm_getmatrix_hloadbit04a

	b	asm_getmatrix_set_error_range_3

asm_getmatrix_hloadbit06

	ldr	r2, [r4, #-(17 * 4)]!

	add	r9, r12, r6
	sub	r4, r4, r9
	cmp	r4, #16 * 4

	add	r9, r9, r4, lsr #1

	bgt	asm_getmatrix_set_error_range_4

	ldrlesh	r9, [r9, #70 * 2]

	cmp	r6, #624
	addeq	r3, r3, #20
	addne	r3, r3, #20+312

	sub	r3, r3, r2
	add	r3, r3, r9
	cmp	r3, #__cpp(JPEG_DECODER_JW_SIZE)

	ldrccb	r9, [r12, r3]
	bcc	asm_getmatrix_hloadbit07

	b	asm_getmatrix_set_error_range_4

asm_getmatrix_getmtxsb03

	sub	r0, r0, r1

asm_getmatrix_getmatrix1

	ldrsh	r4, [r5]

	add	r6, r6, #(688+624) - 624

	add	r4, r4, r0
	strh	r4, [r5]
	strh	r4, [r8], #2

	add	r5, r8, #63 * 2

asm_getmatrix_getmatrix2

	movs	r2, r6, lsl #8

	bne	asm_getmatrix_hloadbit11

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_2

	ldrb	r7, [r4], #1
	rsb	r2, r6, #0
	and	r2, r6, r2
	bic	r6, r6, r2
	clz	r2, r2
	add	r2, r2, #8
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orr	r3, r7, #1 << 31
	orr	r6, r6, r3, ror r2

asm_getmatrix_hloadbit11

	add	r2, r12, r6
	add	r2, r2, r6, lsr #24
	ldrb	r3, [r2, #174]
	ldrb	r9, [r2, #430]

	cmp	r3, #0
	beq	asm_getmatrix_hloadbit12

	movs	r6, r6, lsl r3

	beq	asm_getmatrix_set_error_hlb_0

asm_getmatrix_hloadbit17

	cmp	r9, #0

	beq	asm_getmatrix_getmatrix3

	add	r4, r8,  r9, lsr #4
	add	r4, r4, r9, lsr #4
	cmp	r4, r5

	bcs	asm_getmatrix_set_error_range_5

	mov	r2, #0
asm_getmatrix_getmatrix6
	cmp	r8, r4
	strcch	r2, [r8], #2
	bcc	asm_getmatrix_getmatrix6

asm_getmatrix_getmatrix7

	ands	r9, r9, #15
	mov	r0, r9
	beq	asm_getmatrix_getmatrix8

	mov	r0, #1

	movs	r6, r6, lsl #1
	bne	asm_getmatrix_loadbit03

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_5

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

asm_getmatrix_loadbit03

	rsbcc	r1, r0, r0, lsl r9
	movcs	r1, #0
	sbc	r0, r0, #0

asm_getmatrix_getmtxsb04

	subs	r9, r9, #1

	ble	asm_getmatrix_getmtxsb06

	movs	r6, r6, lsl #1
	adcne	r0, r0, r0
	bne	asm_getmatrix_getmtxsb04

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_5

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

	adc	r0, r0, r0
	b	asm_getmatrix_getmtxsb04

asm_getmatrix_getmatrix3

	tst	r8, #0x02
	strhne	r9, [r8], #2

asm_getmatrix_getmatrix3a

	cmp	r8, r5
	strcc	r9, [r8], #4
	bcc	asm_getmatrix_getmatrix3a

	b	asm_getmatrix_getmatrix4

asm_getmatrix_hloadbit12a

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_3

	ldrb	r7, [r4], #1
	rsb	r2, r6, #0
	and	r2, r6, r2
	bic	r6, r6, r2
	clz	r2, r2
	add	r2, r2, #8
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orr	r3, r7, #1 << 31
	orr	r6, r6, r3, ror r2

asm_getmatrix_hloadbit12

	movs	r2, r6, lsl #9

	beq	asm_getmatrix_hloadbit12a

asm_getmatrix_hloadbit13

	add	r4, r12, r6
	mov	r3,    r6, lsr #23
	ldr	r2, [r4, #(17 + 9) * 4]!
	mov	r6, r6, lsl #9

asm_getmatrix_hloadbit14
	cmp	r3, r2

	ble	asm_getmatrix_hloadbit16

asm_getmatrix_hloadbit14a

	movs	r6, r6, lsl #1

	bne	asm_getmatrix_hloadbit15

 	cmp	r4, r5;
 	bcs	asm_getmatrix_set_error_underrun_4

	ldrb	r7, [r4], #1
	mov	r2, #1 << 24
	cmp	r7, #0xff
	addeq	r4, r4, #1
	orrs	r6, r2, r7, lsl #24 + 1

asm_getmatrix_hloadbit15

	ldr	r2, [r4, #4]!
	adc	r3, r3, r3
	cmp	r3, r2
	ble	asm_getmatrix_hloadbit16

	add	r2, r12, r6
	sub	r2, r4, r2
	cmp	r2, #(17 + 17) * 4
	blt	asm_getmatrix_hloadbit14a

	b	asm_getmatrix_set_error_range_3

asm_getmatrix_hloadbit16

	ldr	r2, [r4, #-(17 * 4)]!

	add	r9, r12, r6
	sub	r4, r4, r9
	cmp	r4, #16 * 4

	add	r9, r9, r4, lsr #1

	bgt	asm_getmatrix_set_error_range_4

	ldrlesh	r9, [r9, #70 * 2]

	cmp	r6, #(688+624)
	addeq	r3, r3, #20+36
	addne	r3, r3, #20+348

	sub	r3, r3, r2
	add	r3, r3, r9
	cmp	r3, #__cpp(JPEG_DECODER_JW_SIZE)

	ldrccb	r9, [r12, r3]
	bcc	asm_getmatrix_hloadbit17

	b	asm_getmatrix_set_error_range_4

asm_getmatrix_getmtxsb06

	sub	r0, r0, r1

asm_getmatrix_getmatrix8

	strh	r0, [r8], #2
	cmp	r8, r5
	bcc	asm_getmatrix_getmatrix2

asm_getmatrix_getmatrix4

	ldr	r2, offsetJw
	sub	r0, r12, r2

	str	r4,      [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	str	r6,     [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	strb	r7,    [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, chitFF)]

	pop  {r0, r4-r11, pc}

asm_getmatrix_set_error_underrun_2
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_2)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_3
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_3)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_4
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_4)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_5
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_5)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_3
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_3)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_4
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_4)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_5
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_5)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_hlb_0
	mvn	r2, #__cpp(-1-JPEG_DECODER_ERROR_HLB_0)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error
	ldr	r3, offsetJw
	sub	r3, r12, r3
	ldrsb	r0, [r3, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, u.s.errorCode)]
	cmp	r0, #0
	streqb	r2, [r3, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, u.s.errorCode)]

	pop  {r0, r4-r11, pc}

offsetJw
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, jwUnion.jw)





}

       void JpegMpDecoderAsmDecodeBlock(register JpegMpDecoderContext* pCtx, register u8 *r8, register s32 n);
NN_ASM void JpegMpDecoderAsmDecodeBlock(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register u8 *r8 NN_IS_UNUSED_VAR, register s32 n NN_IS_UNUSED_VAR)
{
	PRESERVE8











	add	r3, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, qyUnion.qy)
	add	r3, r3, r2, lsl #(6 + 1)

	add	r12, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[3][0])

	push {r1, r3, r4-r11, r12, lr}

	add	r1, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[0])
	add	r0, r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[1])

	ldr	lr, [r1, #0x14*2]
	ldr	r7,  [r1, #0x08*2]
	ldr	r9,  [r1, #0x02*2]
	ldr	r12, [r1, #0x22*2]
	ldr	r6,  [r1, #0x0A*2]
	ldr	r8,  [r1, #0x00*2]
	ldr	r11, [r1, #0x16*2]
	ldr	r10, [r1, #0x04*2]

	pkhbt	r2, lr, r7
	pkhbt	r3, r9, r12
	pkhbt	r4, r8, r6, lsl #16
	pkhtb	r5, lr, r9, asr #16

	ldr	lr, [r1, #0x24*2]
	ldr	r9,  [r1, #0x12*2]

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r11, r6
	pkhbt	r3, r10, lr, lsl #16
	pkhtb	r4, r9, r8, asr #16
	pkhbt	r5, r7, r12, lsl #16

	ldr	r6,  [r1, #0x20*2]
	ldr	r8,  [r1, #0x06*2]
	ldr	r12, [r1, #0x30*2]
	ldr	r7,  [r1, #0x0C*2]

	mov	r6, r6, ror #16
	mov	r8, r8, ror #16

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r6, r9, lsl #16
	pkhbt	r3, r8, r12, lsl #16
	pkhtb	r4, r11, r10, asr #16
	pkhbt	r5, r7, lr

	ldr	lr, [r1, #0x18*2]
	ldr	r9,  [r1, #0x26*2]
	ldr	r10, [r1, #0x10*2]
	ldr	r11, [r1, #0x2E*2]

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r9, lr, lsl #16
	pkhtb	r3, r12, r7, asr #16
	pkhtb	r4, r6, r8, asr #16
	pkhtb	r5, r11, r10, asr #16

	ldr	r6,  [r1, #0x32*2]
	ldr	r8,  [r1, #0x1E*2]
	ldr	r12, [r1, #0x38*2]
	ldr	r7,  [r1, #0x0E*2]

	mov	r6, r6, ror #16

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r11, r8
	pkhbt	r3, r10, r12
	pkhbt	r4, r7, r9
	pkhtb	r5, r6, lr, asr #16

	ldr	r9,  [r1, #0x28*2]
	ldr	lr, [r1, #0x1A*2]
	ldr	r10, [r1, #0x3A*2]
	ldr	r11, [r1, #0x2C*2]

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r6, r9, lsl #16
	pkhbt	r3, lr, r10, lsl #16
	pkhtb	r4, r11, r7, asr #16
	pkhbt	r5, r8, r12, lsl #16

	ldr	r8,  [r1, #0x36*2]
	ldr	r7,  [r1, #0x3E*2]
	ldr	r6,  [r1, #0x34*2]
	ldr	r12, [r1, #0x1C*2]

	mov	r8, r8, ror #16
	mov	r7, r7, ror #16
	mov	r6, r6, ror #16

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r8, r11, lsl #16
	pkhtb	r3, r7, r12, asr #16
	pkhtb	r4, r6, lr, asr #16
	pkhtb	r5, r10, r9, asr #16

	ldr	r9,  [r1, #0x3C*2]
	ldr	lr, [r1, #0x2A*2]

	stmia	r0!, {r2, r3, r4, r5}

	pkhbt	r2, r9, r6, lsl #16
	pkhbt	r3, lr, r7, lsl #16
	pkhbt	r4, r12, r8
	pkhtb	r5, r9, lr, asr #16

	stmia	r0!, {r2, r3, r4, r5}

	sub	lr, r0, #(1<<7)

	ldr	r1, [sp, #0x04]

	ldr	r10, constData0
	ldr	r11, constData1

asm_decode_block_decblock02

	ldmia	lr!, {r2, r3, r4, r5}

	ldmia	r1!, {r6, r7, r8, r9}

	orr	r12, r2, r3
	orr	r12, r12, r5
	orrs    r12, r12, r4, lsr #16

	beq	asm_decode_block_decblock02a

asm_decode_block_decblock03

	smuad	r12, r2, r6
	smusd	r6, r2, r6

	smusd	r2, r5, r9
	smuad	r9, r5, r9
	smusd	r5, r4, r8
	smuad	r4, r4, r8

	smulbb	r2, r2, r10

	smusd	r8, r3, r7
	smuad	r3, r3, r7

	sub	r7, r4, r9
	add	r4, r4, r9

	rsb	r2, r9, r2, asr #FIX_SHIFT

	add	r9, r5, r2
	sub	r5, r5, r2

	sub	r2, r3, r12
	add	r3, r3, r12
	add	r12, r6, r8
	smulbb	r6, r6, r11
	smulbt	r12, r12, r11
	smulbb	r2, r2, r10
	smulbt	r8, r8, r10

	rsb	r6, r3, r6, asr #FIX_SHIFT
	add	r6, r6, r12, asr #FIX_SHIFT
	rsb	r2, r6, r2, asr #FIX_SHIFT
	add	r8, r2, r8, asr #FIX_SHIFT
	sub	r8, r8, r12, asr #FIX_SHIFT

	pkhbt	r12, r4, r9, lsl #16
	pkhbt	r4, r3, r6, lsl #16
	ssub16	r9, r12, r4
	sadd16	r3, r12, r4
	mov	r9, r9, ror #16

	ldr	r12, [sp, #0x28]

	pkhbt	r6, r8, r5, lsl #16
	pkhbt	r5, r2, r7, lsl #16
	ssax	r7, r5, r6
	ssax	r8, r6, r5

	stmia	r0!, {r3, r7, r8, r9}
	cmp	r0, r12
	bcc	asm_decode_block_decblock02

asm_decode_block_decblock04a

	sub	lr, r0, #(1<<7) - (8 * 2)
	str	lr, [sp, #0x28]

	sub	r0, r0, #(1<<7)
	ldr	r1, [sp, #0x00]

asm_decode_block_decblock05

	ldr	r3, [r0, #8  * 2]
	ldr	r4, [r0, #16 * 2]
	ldr	r5, [r0, #24 * 2]
	ldr	r6, [r0, #32 * 2]

	ldr	r7, [r0, #40 * 2]
	ldr	r8, [r0, #48 * 2]
	ldr	r9, [r0, #56 * 2]

	orr	r2, r3, r4
	orr	r2, r2, r5
	orr	r2, r2, r6
	orr	r2, r2, r7
	orr	r2, r2, r8
	orrs	r2, r2, r9
	beq	asm_decode_block_decblock05a

	movs	r12, r2, lsl #16
	movnes	r2, r2, lsr #16

	ldr	r2, [r0], #2 * 2
	bne	asm_decode_block_decblock06

	cmp	r12, #0

	pkhtbeq	r4, r4, r8, asr #16
	pkhbtne	r4, r8, r4, lsl #16

	pkhtbeq	r8, r2, r6, asr #16
	pkhbtne	r8, r6, r2, lsl #16

	moveq	r2, r2, lsl #16
	add	r2, r2, #0x80 << __cpp(16 + 5 + s_DqtExtraBits1)

	qsax	r4, r4, r4
	qsax	r8, r8, r8

	usat	r2, #8, r2, asr #__cpp(16 + 5 + s_DqtExtraBits1)

	pkhtbeq	r7, r7, r5, asr #16
	pkhbtne	r7, r5, r7, lsl #16

	smultb	r6, r4, r10

	pkhtbeq	r3, r3, r9, asr #16
	pkhbtne	r3, r9, r3, lsl #16

	orr	r2, r2, r2, lsl #8

	ssat	r6, #16, r6, asr #FIX_SHIFT

	orr	r2, r2, r2, lsl #16

	qsub16	r6, r6, r4

	mov	r12, r2
	stmeqia	r1!, {r2, r12}

	pkhbt	r6, r6, r4, lsl #16

	qsax	r7, r7, r7
	qsax	r3, r3, r3

	qasx	r5, r8, r6
	qsax	r6, r8, r6

	qadd16	r9, r3, r7
	qsub16	r8, r3, r7

	smultb	r7, r7, r11
	smultt	r4, r9, r11
	smulbb	r8, r8, r10
	smultt	r3, r3, r10

	mov	r7, r7, asr #FIX_SHIFT
	add	r7, r7, r4, asr #FIX_SHIFT
	qsub16	r7, r7, r9

	mov	r8, r8, asr #FIX_SHIFT
	qsub16	r8, r8, r7

	mov	r3, r3, asr #FIX_SHIFT
	sub	r3, r3, r4, asr #FIX_SHIFT
	qadd16	r3, r3, r8

	pkhbt	r9, r9, r8, lsl #16
	qadd16	r8, r6, r9
	pkhbt	r4, r7, r3, lsl #16
	qsub16	r6, r6, r9
	qasx	r9, r5, r4
	qsax	r7, r5, r4

	ldr	r3, constData_0x00ff00ff

	ssat16	r8, #__cpp(13 + s_DqtExtraBits1), r8
	ssat16	r6, #__cpp(13 + s_DqtExtraBits1), r6
	ssat16	r9, #__cpp(13 + s_DqtExtraBits1), r9
	ssat16	r7, #__cpp(13 + s_DqtExtraBits1), r7

	and	r8, r3, r8, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r6, r3, r6, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r9, r3, r9, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r7, r3, r7, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	r3, r3, r3, ror #1

	orr	r7, r7, r6, ror #8
	orr	r4, r8, r9, ror #8

	eor	r7, r7, r3
	eor	r4, r4, r3

	stmia	r1!, {r4, r7}
	stmneia	r1!, {r2, r12}

	cmp	r0, lr

	bcc	asm_decode_block_decblock05

	pop  {r1, r3, r4-r11, r12, pc}

asm_decode_block_decblock05a
	ldr	r2, [r0], #2 * 2

#if 0

	mov	r6,     #0x00ff0000
	orr	r6, r6, #0x000000ff
#else

	ldr	r6, constData_0x00ff00ff
#endif

	ssat16	r2, #__cpp(13 + s_DqtExtraBits1), r2

	eor	r5, r6, r6, ror #1

	and	r2, r6, r2, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	r3, r2, r5
	eor	r2, r3, r2, lsl #8

	cmp	r0, lr

	pkhtb	r4, r2, r2, asr #16
	pkhbt	r2, r2, r2, lsl #16
	mov	r5, r4
	mov	r3, r2

	stmia	r1!, {r2, r3, r4, r5}
	bcc	asm_decode_block_decblock05

	pop  {r1, r3, r4-r11, r12, pc}

asm_decode_block_decblock06

	qadd16	lr, r7, r5
	qadd16	r12, r3, r9
	qsub16	r5, r7, r5
	qsub16	r3, r3, r9
	qadd16	r9, r12, lr
	qsub16	r7, r12, lr

	qsub16	r12, r4, r8
	qadd16	r4, r4, r8

	qadd16	r8, r2, r6

	smulbb	lr, r12, r10
	smultb	r12, r12, r10

	qsub16	r6, r2, r6

	qadd16	r2, r5, r3

	mov	lr, lr, asr #FIX_SHIFT
	pkhbt	lr, lr, r12, lsl #(16 - FIX_SHIFT)

	smulbt	r12, r2, r11
	smultt	r2, r2, r11

	qsub16	lr, lr, r4

	mov	r12, r12, asr #FIX_SHIFT
	pkhbt	r12, r12, r2, lsl #(16 - FIX_SHIFT)

	smulbb	r2, r5, r11
	smultb	r5, r5, r11

	smulbt	r11, r3, r10
	smultt	r3,  r3, r10

	mov	r2, r2, asr #FIX_SHIFT
	pkhbt	r2, r2, r5, lsl #(16 - FIX_SHIFT)

	smulbb	r5, r7, r10
	smultb	r7, r7, r10

	mov	r11, r11, asr #FIX_SHIFT
	pkhbt	r3, r11, r3, lsl #(16 - FIX_SHIFT)

	ldr	r11, constData1

	mov	r5, r5, asr #FIX_SHIFT
	pkhbt	r5, r5, r7, lsl #(16 - FIX_SHIFT)

	qadd16	r2, r2, r12
	qsub16	r3, r3, r12

	qsub16	r2, r2, r9

	qadd16	r12, r8, r4
	qsub16	r8, r8, r4

	qsub16	r5, r5, r2

	qadd16	r4, r6, lr
	qsub16	r6, r6, lr

	qadd16	r3, r3, r5

	qadd16	lr, r12, r9
	qsub16	r12, r12, r9
	qadd16	r9, r4, r2
	qsub16	r4, r4, r2
	qadd16	r2, r6, r5
	qsub16	r6, r6, r5
	qadd16	r5, r8, r3
	qsub16	r8, r8, r3

	ldr	r3, constData_0x00ff00ff

	ssat16	lr, #__cpp(13 + s_DqtExtraBits1), lr
	ssat16	r12, #__cpp(13 + s_DqtExtraBits1), r12
	ssat16	r9, #__cpp(13 + s_DqtExtraBits1), r9
	ssat16	r4, #__cpp(13 + s_DqtExtraBits1), r4
	ssat16	r2, #__cpp(13 + s_DqtExtraBits1), r2
	ssat16	r6, #__cpp(13 + s_DqtExtraBits1), r6
	ssat16	r5, #__cpp(13 + s_DqtExtraBits1), r5
	ssat16	r8, #__cpp(13 + s_DqtExtraBits1), r8

	and	lr, r3, lr, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r12, r3, r12, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r9, r3, r9, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r4, r3, r4, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r2, r3, r2, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r6, r3, r6, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r5, r3, r5, lsr #__cpp(5 + s_DqtExtraBits1)
	and	r8, r3, r8, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	r3, r3, r3, ror #1

	orr	lr, lr, r9, lsl #8
	orr	r8, r2, r8, lsl #8
	orr	r6, r5, r6, lsl #8
	orr	r12, r4, r12, lsl #8

	pkhbt	r2, lr, r8, lsl #16
	pkhbt	r4, r6, r12, lsl #16
	pkhtb	r5, r8, lr, asr #16
	pkhtb	r7, r12, r6, asr #16

	ldr	lr, [sp, #0x28]

	eor	r2, r2, r3
	eor	r4, r4, r3
	eor	r5, r5, r3
	eor	r7, r7, r3

	cmp	r0, lr

	stmia	r1!, {r2, r4, r5, r7}
	bcc	asm_decode_block_decblock05

	pop  {r1, r3, r4-r11, r12, pc}

asm_decode_block_decblock02a

	smulbb	r2, r4, r8
	ldr	r12, [sp, #0x28]

	pkhbt	r2, r2, r2, lsl #16
	mov	r3, r2
	stmia	r0!, {r2, r3}
	stmia	r0!, {r2, r3}

	cmp	r0, r12
	bcc	asm_decode_block_decblock02

	b	asm_decode_block_decblock04a

constData_0x00ff00ff
	dcd	0x00ff00ff

constData0
	dcd	((FIX_1082392200 << 16) ASM_OP_LOG_OR (FIX_1414213562 & 0xffff))
constData1
	dcd	((FIX_1847759065 << 16) ASM_OP_LOG_OR ((-FIX_2613125930) & 0xffff))

}

       void JpegMpDecoderAsmWrite11Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8








	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r4-r7}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

	sub	r3, r3, #8 << (1+3)

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	mov	r2, #0x80

asm_Write11Yuyv8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #2

	ldrb	r12, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2 + 1]
	ldrb	r0, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	pkhbt	r7, r7, r6, lsl #16

	adds	r2, r2, #0x40000000

	orr	r7, r0, r7, lsl #8
	orr	r7, r7, r12, lsl #16
	str	r7, [r1], #4

	bcc	asm_Write11Yuyv8_50

	movs	r2, r2, lsr #1

	addne	r1, r1, r3

	bne	asm_Write11Yuyv8_50

	pop  {r4-r7}
	bx	lr







}

       void JpegMpDecoderAsmWrite21Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8








	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r4-r7}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

	sub	r3, r3, #16 << (1+3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	ldr	r2, =0x00810080

asm_Write21Yuyv8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]

	ldrb	r0, [r5], #2

	ldrb	r7, [r4], #1

	ldrb	r12, [r5, #-0x01]

	movs	r2, r2, lsl #2

	addmi	r5, r5, #0x38

	orr	r0, r0, r6,  lsl #24
	orr	r0, r0, r7,  lsl #8
	orr	r0, r0, r12, lsl #16
	str	r0, [r1], #4

	bcc	asm_Write21Yuyv8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00810000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21Yuyv8_50

	pop  {r4-r7}
	bx	lr







}

       void JpegMpDecoderAsmWrite12Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8








	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r4-r7}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	mov	r2, #0x80

asm_Write12Yuyv8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #2

	ldrh	r12, [r5, #0x08]
	ldrh	r0, [r5], #2

	adds	r2, r2, #0x40000000

	mov	r6, r6, lsl #24
	orr	r6, r6, r7, lsl #8

	orr	r12, r12, r12, lsl #8
	orr	r0, r0, r0, lsl #8

	uxtab16	r12, r6, r12
	str	r12, [r1, r3]

	uxtab16	r0, r6, r0
	str	r0, [r1], #4

	bcc	asm_Write12Yuyv8_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #8 << (1+3)

	bne	asm_Write12Yuyv8_50

	pop  {r4-r7}
	bx	lr







}

       void JpegMpDecoderAsmWrite22Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r4-r7}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	ldr	r2, =0x00210088

	sub	r12, r3, #0x10 << ((1+3) - 1)

asm_Write22Yuyv8_50

	ldrb	r7, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r0, [r4], #1

	movs	r2, r2, lsl #2

	addmi	r5, r5, #0x38

	pkhbt	r9,  r0, r7, lsl #16

	ldrb	r8, [r5, #0x09]
	ldrb	r6, [r5, #0x01]
	ldrb	r7, [r5, #0x08]
	ldrb	r0, [r5], #2

	orr	r8, r9,  r8, lsl #8
	orr	r6, r9,  r6, lsl #8
	orr	r8, r7, r8, lsl #8
	str	r8, [r1, r3]

	orr	r6, r0, r6, lsl #8
	str	r6, [r1], #4

	bcc	asm_Write22Yuyv8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r12, lsl #1

	bne	asm_Write22Yuyv8_50

	pop  {r4-r7}
	bx	lr








}

       void JpegMpDecoderAsmWrite11CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

    add r5, r0, #0x400

	adr	r0, constTbl11CtrRgb565

	sub	r3, r3, #8 << (1+3)
	str	r3, [sp, #0x00]

	mov	r2, #0x80

asm_Write11CtrRgb565_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	adds	r2, r2, #0x40000000

	orr	r11, r7, r6, lsl #16

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	uxtb16	r7, r11
	uxtb16	r11, r11, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r11, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r11, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r11, r10, r12

	orr	r6, r6, r6, lsl #8

	mov	r3, r3, asr #15
	mov	r8, r8, asr #14
	mov	r7, r7, asr #14

	uxtb16	r6, r6

	pkhbt	r3, r3, r10, lsl #16 - 15
	pkhbt	r8, r8, r11, lsl #16 - 14
	pkhbt	r7, r7, r5, lsl #16 - 14

	qadd16	r3, r3, r6
	qadd16	r8, r8, r6
	qadd16	r7, r7, r6

	usat16	r3, #8, r3
	usat16	r8, #8, r8
	usat16	r7, #8, r7

	and	r3, lr, r3, lsr #2
	and	r8, lr, r8, lsr #3
	and	r7, lr, r7, lsr #3

	orr	r7, r7, r3, lsl #5
	orr	r7, r7, r8, lsl #5 + 6
	str	r7, [r1], #4

	bcc	asm_Write11CtrRgb565_50

	ldr	r7, [sp, #0x00]

	movs	r2, r2, lsr #1

	addne	r1, r1, r7

	bne	asm_Write11CtrRgb565_50

	pop  {r0, r4-r11, pc}

constTbl11CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f









}

       void JpegMpDecoderAsmWrite21CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

	sub	r3, r3, #16 << (1+3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21CtrRgb565

	ldr	r2, =0x00210080

asm_Write21CtrRgb565_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r7, [r5, #0x01]
	ldrb	r10, [r5], #2

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r10, r10, r7, lsl #16

	pkhbt	r6, r6, r6, lsl #16
	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16

	qadd16	r6, r6, r10
	qadd16	r11, r11, r10
	qadd16	r12, r12, r10

	usat16	r6, #8, r6
	usat16	r11, #8, r11
	usat16	r12, #8, r12

	and	r6, lr, r6, lsr #2
	and	r11, lr, r11, lsr #3
	and	r12, lr, r12, lsr #3

	orr	r10, r12, r6, lsl #5
	orr	r10, r10, r11, lsl #5 + 6
	str	r10, [r1], #4

	bcc	asm_Write21CtrRgb565_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21CtrRgb565_50

	pop  {r0, r4-r11, pc}

constTbl21CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f









}

       void JpegMpDecoderAsmWrite12CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12CtrRgb565

	mov	r2, #0x80

asm_Write12CtrRgb565_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9, lr}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5, #0x08 - 1]

	mov	r11, r11, asr #14
	mov	r12, r12, asr #14
	mov	r6, r6, asr #15

	pkhbt	r10, r10, r7, lsl #16

	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16
	pkhbt	r6, r6, r6, lsl #16

	qadd16	r11, r11, r10
	qadd16	r12, r12, r10
	qadd16	r6, r6, r10

	usat16	r11, #8, r11
	usat16	r12, #8, r12
	usat16	r6, #8, r6

	and	r11, lr, r11, lsr #3
	and	r12, lr, r12, lsr #3
	and	r6, lr, r6, lsr #2

	orr	r12, r12, r11, lsl #5 + 6
	orr	r12, r12, r6, lsl #5
	strh	r12, [r1, r3]

	mov	r12, r12, lsr #16
	strh	r12, [r1], #2

	bcc	asm_Write12CtrRgb565_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #8 << (1+3)

	bne	asm_Write12CtrRgb565_50

	pop  {r0, r4-r11, pc}

constTbl12CtrRgb565
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	0x003f003f









}

       void JpegMpDecoderAsmWrite22CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22CtrRgb565

	ldr	r2, =0x00210088

asm_Write22CtrRgb565_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r8, [r5, #0x09]
	ldrb	r10,   [r5, #0x08]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	pkhbt	r10, r10, r8, lsl #16

	ldrb	r7, [r5, #0x01]
	ldrb	r8, [r5], #2

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r6, r6, r6, lsl #16
	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16

	pkhbt	r8, r8, r7, lsl #16

	qadd16	r10, r11, r10
	qadd16	r7, r6, r10

	usat16	r10, #8, r10
	usat16	r7, #8, r7

	qadd16	r11, r11, r8
	qadd16	r6, r6, r8

	and	r10, lr, r10, lsr #3
	and	r7, lr, r7, lsr #2
	orr	r10, r7, r10, lsl #6

	qadd16	r7, r12, r10
	qadd16	r12, r12, r8

	usat16	r11, #8, r11
	usat16	r6, #8, r6

	usat16	r8, #8, r7
	usat16	r12, #8, r12

	and	r11, lr, r11, lsr #3
	and	r6, lr, r6, lsr #2
	orr	r11, r6, r11, lsl #6

	and	r8, lr, r8, lsr #3
	and	r12, lr, r12, lsr #3

	orr	r10, r8, r10, lsl #5
	str	r10, [r1, r3]

	orr	r11, r12, r11, lsl #5
	str	r11, [r1], #4

	bcc	asm_Write22CtrRgb565_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #0x10 << (1+3)

	bne	asm_Write22CtrRgb565_50

	pop  {r0, r4-r11, pc}

constTbl22CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f









}

       void JpegMpDecoderAsmWrite11CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8











	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11CtrRgb565Block8
	adr	r3, dstOffsetTbl11CtrRgb565Block8

	mov	r2, #0x80

asm_Write11CtrRgb565Block8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	adds	r2, r2, #0x40000000

	orr	r6, r7, r6, lsl #16

	uxtb16	r7, r6
	uxtb16	r6, r6, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r6, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r6, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r6, r10, r12

	ldrh	r10, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	mov	r3, r3, asr #15
	mov	r8, r8, asr #14
	mov	r7, r7, asr #14

	orr	r10, r10, r10, lsl #8

	pkhbt	r3, r3, r10, lsl #16 - 15

	uxtb16	r10, r10

	pkhbt	r8, r8, r11, lsl #16 - 14
	pkhbt	r7, r7, r5, lsl #16 - 14

	qadd16	r3, r3, r10
	qadd16	r8, r8, r10
	qadd16	r7, r7, r10

	ldrb	r10, [r3], #1

	usat16	r3, #8, r3
	usat16	r8, #8, r8
	usat16	r7, #8, r7

	and	r3, lr, r3, lsr #2
	and	r8, lr, r8, lsr #3
	and	r7, lr, r7, lsr #3

	orr	r7, r7, r3, lsl #5
	orr	r7, r7, r8, lsl #5 + 6
	str	r7, [r1, r10]

	bcc	asm_Write11CtrRgb565Block8_50

	movs	r2, r2, lsr #1

	bne	asm_Write11CtrRgb565Block8_50

	pop  {r0, r4-r11, pc}

constTbl11CtrRgb565Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

dstOffsetTbl11CtrRgb565Block8
	dcd	0x14100400*2
	dcd	0x16120602*2
	dcd	0x1c180c08*2
	dcd	0x1e1a0e0a*2
	dcd	0x34302420*2
	dcd	0x36322622*2
	dcd	0x3c382c28*2
	dcd	0x3e3a2e2a*2










}

       void JpegMpDecoderAsmWrite21CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8











	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21CtrRgb565Block8
	adr	r3, dstOffsetTbl21CtrRgb565Block8

	ldr	r2, =0x00210080

asm_Write21CtrRgb565Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r7, [r5, #0x01]
	ldrb	r10, [r5], #2

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r10, r10, r7, lsl #16

	ldrb	r7, [r3], #1

	pkhbt	r6, r6, r6, lsl #16
	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16

	qadd16	r6, r6, r10
	qadd16	r11, r11, r10
	qadd16	r12, r12, r10

	usat16	r10, #8, r6
	usat16	r11, #8, r11
	usat16	r12, #8, r12

	and	r10, lr, r10, lsr #2
	and	r11, lr, r11, lsr #3
	and	r12, lr, r12, lsr #3

	orr	r10, r12, r10, lsl #5
	orr	r10, r10, r11, lsl #5 + 6
	str	r10, [r1, r7]

	bcc	asm_Write21CtrRgb565Block8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	bne	asm_Write21CtrRgb565Block8_50

	pop  {r0, r4-r11, pc}

constTbl21CtrRgb565Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

dstOffsetTbl21CtrRgb565Block8
	dcd	0x14100400*2,0x54504440*2
	dcd	0x16120602*2,0x56524642*2
	dcd	0x1c180c08*2,0x5c584c48*2
	dcd	0x1e1a0e0a*2,0x5e5a4e4a*2
	dcd	0x34302420*2,0x74706460*2
	dcd	0x36322622*2,0x76726662*2
	dcd	0x3c382c28*2,0x7c786c68*2
	dcd	0x3e3a2e2a*2,0x7e7a6e6a*2










}

       void JpegMpDecoderAsmWrite12CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12CtrRgb565Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl12CtrRgb565Block8

	mov	r2, #0x88

asm_Write12CtrRgb565Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9, lr}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5, #0x08]

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5], #1

	mov	r11, r11, asr #14
	mov	r12, r12, asr #14
	mov	r6, r6, asr #15

	pkhbt	r10, r10, r7, lsl #16

	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16
	pkhbt	r6, r6, r6, lsl #16

	qadd16	r11, r11, r10
	qadd16	r12, r12, r10
	qadd16	r6, r6, r10

	ldrb	r10, [r3], #1

	usat16	r11, #8, r11
	usat16	r12, #8, r12
	usat16	r6, #8, r6

	and	r11, lr, r11, lsr #3
	and	r12, lr, r12, lsr #3
	and	r6, lr, r6, lsr #2

	orr	r12, r12, r11, lsl #5 + 6
	orr	r12, r12, r6, lsl #5
	strh	r12, [r10, r1]!

	mov	r12, r12, lsr #16
	strh	r12, [r10, #0x02*2]

	bcc	asm_Write12CtrRgb565Block8_50

	movs	r2, r2, lsr #1
	ldrcs	r1, [sp, #0x00]

	add	r5, r5, #0x08

	adrcs	r3, dstOffsetTbl12CtrRgb565Block8

	bne	asm_Write12CtrRgb565Block8_50

	pop  {r0, r4-r11, pc}

constTbl12CtrRgb565Block8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	0x003f003f

dstOffsetTbl12CtrRgb565Block8
	dcd	0x05040100*2,0x15141110*2
	dcd	0x0d0c0908*2,0x1d1c1918*2
	dcd	0x25242120*2,0x35343130*2
	dcd	0x2d2c2928*2,0x3d3c3938*2










}

       void JpegMpDecoderAsmWrite22CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8











	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #(1+3)

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22CtrRgb565Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl22CtrRgb565Block8

	ldr	r2, =0x00210088

asm_Write22CtrRgb565Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r8, [r5, #0x09]
	ldrb	r10,   [r5, #0x08]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	pkhbt	r10, r10, r8, lsl #16

	ldrb	r7, [r5, #0x01]
	ldrb	r8, [r5], #2

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r6, r6, r6, lsl #16
	pkhbt	r11, r11, r11, lsl #16
	pkhbt	r12, r12, r12, lsl #16

	pkhbt	r8, r8, r7, lsl #16

	qadd16	r7, r6, r10
	qadd16	r10, r11, r10

	usat16	r7, #8, r7
	usat16	r10, #8, r10

	qadd16	r6, r6, r8
	qadd16	r11, r11, r8

	and	r10, lr, r10, lsr #3
	and	r7, lr, r7, lsr #2
	orr	r10, r7, r10, lsl #6

	qadd16	r7, r12, r10
	qadd16	r12, r12, r8

	usat16	r6, #8, r6
	usat16	r11, #8, r11

	usat16	r8, #8, r7
	usat16	r12, #8, r12

	ldrb	r7, [r3], #1

	and	r11, lr, r11, lsr #3
	and	r6, lr, r6, lsr #2
	orr	r11, r6, r11, lsl #6

	and	r8, lr, r8, lsr #3
	and	r12, lr, r12, lsr #3

	orr	r10,   r8, r10, lsl #5
	orr	r8, r12, r11, lsl #5

	strd	r8, r9, [r1, r7]

	bcc	asm_Write22CtrRgb565Block8_50

	movs	r2, r2, lsr #16 + 1
	ldrcs	r1, [sp, #0x00]
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	adrcs	r3, dstOffsetTbl22CtrRgb565Block8

	bne	asm_Write22CtrRgb565Block8_50

	pop  {r0, r4-r11, pc}

constTbl22CtrRgb565Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

dstOffsetTbl22CtrRgb565Block8
	dcd	0x14100400*2,0x54504440*2
	dcd	0x1c180c08*2,0x5c584c48*2
	dcd	0x34302420*2,0x74706460*2
	dcd	0x3c382c28*2,0x7c786c68*2










}

       void JpegMpDecoderAsmWrite11Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11Rgb8

	sub	r3, r3, #(8 * 3)
	str	r3, [sp, #0x00]

	mov	r2, #0x80

asm_Write11Rgb8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12}

	adds	r2, r2, #0x40000000

	orr	r11, r7, r6, lsl #16

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	uxtb16	r7, r11
	uxtb16	r11, r11, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r11, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r11, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r11, r10, r12

	orr	r6, r6, r6, lsl #8

	mov	r3, r3, lsl #16 - 15
	mov	r8, r8, lsl #16 - 14
	mov	r7, r7, lsl #16 - 14

	uxtb16	r6, r6, ror #16

	pkhtb	r3, r3, r5, asr #14
	pkhtb	r8, r8, r10, asr #15
	pkhtb	r7, r7, r11, asr #14

	qadd16	r3, r3, r6
	qadd16	r8, r8, r6
	qadd16	r7, r7, r6

	usat16	r3, #8, r3
	usat16	r8, #8, r8
	usat16	r7, #8, r7

	orr	r3, r8, r3, lsl #8
	strh	r3, [r1, #4]
	orr	r7, r7, r7, lsl #24
	pkhtb	r7, r7, r3, asr #16
	str	r7, [r1], #(3 * 2)

	bcc	asm_Write11Rgb8_50

	ldr	r7, [sp, #0x00]

	movs	r2, r2, lsr #1

	addne	r1, r1, r7

	bne	asm_Write11Rgb8_50

	pop  {r0, r4-r11, pc}

constTbl11Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite21Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

	sub	r3, r3, #(16 * 3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21Rgb8

	ldr	r2, =0x00210080

asm_Write21Rgb8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r10, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r10, r7, r10, lsl #16

	pkhbt	r7, r12, r6, lsl #16
	pkhbt	r6, r6, r11, lsl #16
	pkhbt	r11, r11, r12, lsl #16

	qadd16	r7, r7, r10
	qadd16	r6, r6, r10
	qadd16	r11, r11, r10

	usat16	r7, #8, r7
	usat16	r6, #8, r6
	usat16	r11, #8, r11

	orr	lr, r6, r7, lsl #8
	strh	lr, [r1, #4]
	orr	r11, r11, r11, lsl #24
	pkhtb	r11, r11, lr, asr #16
	str	r11, [r1], #(3 * 2)

	bcc	asm_Write21Rgb8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21Rgb8_50

	pop  {r0, r4-r11, pc}

constTbl21Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite12Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12Rgb8

	mov	r2, #0x80

asm_Write12Rgb8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5, #0x08 - 1]

	mov	r11, r11, asr #14
	mov	r12, r12, asr #14
	mov	r6, r6, asr #15

	pkhbt	r7, r7, r7, lsl #16
	pkhbt	r10, r10, r10, lsl #16

	pkhbt	r11, r11, r12, lsl #16
	pkhbt	r12, r7, r10
	pkhbt	r6, r6, r6, lsl #16

	qadd16	r6, r6, r12
	qadd16	r10, r11, r10
	qadd16	r7, r11, r7

	usat16	r6, #8, r6
	usat16	r10, #8, r10
	usat16	r7, #8, r7

	orr	r10, r10, r6, lsr #8
	strh	r10, [r1, r3]
	orr	r7, r7, r6, lsl #8
	strh	r7, [r1], #2
	mov	r10, r10, lsr #16
	strb	r10, [r1, r3]
	mov	r7, r7, lsr #16
	strb	r7, [r1], #1

	bcc	asm_Write12Rgb8_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(8 * 3)

	bne	asm_Write12Rgb8_50

	pop  {r0, r4-r11, pc}

constTbl12Rgb8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)








}

       void JpegMpDecoderAsmWrite22Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22Rgb8

	ldr	r2, =0x00210088

asm_Write22Rgb8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10,   [r5, #0x08]
	ldrb	r8, [r5, #0x09]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	pkhbt	r10, r8, r10, lsl #16

	ldrb	r8, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r8, r7, r8, lsl #16

	pkhbt	lr, r12, r6, lsl #16
	pkhbt	r6,     r6, r11, lsl #16
	pkhbt	r11,     r11, r12, lsl #16

	qadd16	r10, lr, r10
	qadd16	r7, r6,     r10
	qadd16	r12, r11,     r10

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r12, #8, r12

	qadd16	lr, lr, r8
	qadd16	r6,     r6,     r8
	qadd16	r11,     r11,     r8

	usat16	lr, #8, lr
	usat16	r6,     #8, r6
	usat16	r11,     #8, r11

	orr	r8, r7, r10, lsl #8
	add	r7, r1, r3
	orr	r12, r12, r12, lsl #24
	strh	r8, [r7, #4]
	pkhtb	r12, r12, r8, asr #16
	str	r12, [r7]

	orr	r8, r6, lr, lsl #8
	strh	r8, [r1, #4]
	orr	r11, r11, r11, lsl #24
	pkhtb	r11, r11, r8, asr #16
	str	r11, [r1], #(3 * 2)

	bcc	asm_Write22Rgb8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(16 * 3)

	bne	asm_Write22Rgb8_50

	pop  {r0, r4-r11, pc}

constTbl22Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite11CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #4
	add	r12, r12, r1, lsl #3

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11CtrRgb8Block8
	adr	r3, dstOffsetTbl11CtrRgb8Block8

	mov	r2, #0x80

asm_Write11CtrRgb8Block8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12}

	adds	r2, r2, #0x40000000

	orr	r6, r7, r6, lsl #16

	uxtb16	r7, r6
	uxtb16	r6, r6, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r6, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r6, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r6, r10, r12

	ldrh	r10, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	mov	r3, r3, lsl #16 - 15
	mov	r8, r8, lsl #16 - 14
	mov	r7, r7, lsl #16 - 14

	orr	r10, r10, r10, lsl #8

	pkhtb	r3, r3, r11, asr #14
	uxtb16	r10, r10, ror #16
	pkhtb	r7, r7, r10, asr #15
	pkhtb	r8, r8, r5, asr #14

	qadd16	r3, r3, r10
	qadd16	r7, r7, r10
	qadd16	r8, r8, r10

	ldrb	r10, [r3], #1

	usat16	r3, #8, r3
	usat16	r7, #8, r7
	usat16	r8, #8, r8

	orr	r3, r7, r3, lsl #8
	strh	r3, [r10, r1]!
	orr	r8, r8, r8, lsl #24
	pkhtb	r8, r8, r3, asr #16
	str	r8, [r10, #-4]

	bcc	asm_Write11CtrRgb8Block8_50

	movs	r2, r2, lsr #1

	bne	asm_Write11CtrRgb8Block8_50

	pop  {r0, r4-r11, pc}

constTbl11CtrRgb8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

dstOffsetTbl11CtrRgb8Block8
	dcd	__cpp((0x14100400UL*3) + 0x04040404)
	dcd	__cpp((0x16120602UL*3) + 0x04040404)
	dcd	__cpp((0x1c180c08UL*3) + 0x04040404)
	dcd	__cpp((0x1e1a0e0aUL*3) + 0x04040404)
	dcd	__cpp((0x34302420UL*3) + 0x04040404)
	dcd	__cpp((0x36322622UL*3) + 0x04040404)
	dcd	__cpp((0x3c382c28UL*3) + 0x04040404)
	dcd	__cpp((0x3e3a2e2aUL*3) + 0x04040404)









}

       void JpegMpDecoderAsmWrite21CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #4
	add	r12, r12, r1, lsl #3

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21CtrRgb8Block8
	adr	r3, dstOffsetTbl21CtrRgb8Block8

	ldr	r2, =0x00210080

asm_Write21CtrRgb8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r10, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r10, r7, r10, lsl #16

	ldrh	r7, [r3], #2

	pkhbt	lr, r11, r6, lsl #16
	pkhbt	r6,     r6, r12, lsl #16
	pkhbt	r12,     r12, r11, lsl #16

	qadd16	lr, lr, r10
	qadd16	r6,     r6,     r10
	qadd16	r12,     r12,     r10

	usat16	lr, #8, lr
	usat16	r6,     #8, r6
	usat16	r12,     #8, r12

	orr	r6, r6, lr, lsl #8
	strh	r6, [r7, r1]!
	orr	r12, r12, r12, lsl #24
	pkhtb	r12, r12, r6, asr #16
	str	r12, [r7, #-4]

	bcc	asm_Write21CtrRgb8Block8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	bne	asm_Write21CtrRgb8Block8_50

	pop  {r0, r4-r11, pc}

constTbl21CtrRgb8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

dstOffsetTbl21CtrRgb8Block8
	dcw	(0x00*3)+4,(0x04*3)+4,(0x10*3)+4,(0x14*3)+4,(0x40*3)+4,(0x44*3)+4,(0x50*3)+4,(0x54*3)+4
	dcw	(0x02*3)+4,(0x06*3)+4,(0x12*3)+4,(0x16*3)+4,(0x42*3)+4,(0x46*3)+4,(0x52*3)+4,(0x56*3)+4
	dcw	(0x08*3)+4,(0x0c*3)+4,(0x18*3)+4,(0x1c*3)+4,(0x48*3)+4,(0x4c*3)+4,(0x58*3)+4,(0x5c*3)+4
	dcw	(0x0a*3)+4,(0x0e*3)+4,(0x1a*3)+4,(0x1e*3)+4,(0x4a*3)+4,(0x4e*3)+4,(0x5a*3)+4,(0x5e*3)+4
	dcw	(0x20*3)+4,(0x24*3)+4,(0x30*3)+4,(0x34*3)+4,(0x60*3)+4,(0x64*3)+4,(0x70*3)+4,(0x74*3)+4
	dcw	(0x22*3)+4,(0x26*3)+4,(0x32*3)+4,(0x36*3)+4,(0x62*3)+4,(0x66*3)+4,(0x72*3)+4,(0x76*3)+4
	dcw	(0x28*3)+4,(0x2c*3)+4,(0x38*3)+4,(0x3c*3)+4,(0x68*3)+4,(0x6c*3)+4,(0x78*3)+4,(0x7c*3)+4
	dcw	(0x2a*3)+4,(0x2e*3)+4,(0x3a*3)+4,(0x3e*3)+4,(0x6a*3)+4,(0x6e*3)+4,(0x7a*3)+4,(0x7e*3)+4









}

       void JpegMpDecoderAsmWrite12CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #4
	add	r12, r12, r1, lsl #3

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12CtrRgb8Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl12CtrRgb8Block8

	mov	r2, #0x88

asm_Write12CtrRgb8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5, #0x08 - 1]

	mov	r11, r11, asr #14
	mov	r12, r12, asr #14
	mov	r6, r6, asr #15

	pkhbt	r7, r7, r7, lsl #16
	pkhbt	r10, r10, r10, lsl #16

	pkhbt	r11, r12, r11, lsl #16
	pkhbt	r12, r7, r10
	pkhbt	r6, r6, r6, lsl #16

	ldrb	r12, [r3], #1

	qadd16	r6, r6, r12
	qadd16	r10, r11, r10
	qadd16	r7, r11, r7

	usat16	r6, #8, r6
	usat16	r10, #8, r10
	usat16	r7, #8, r7

	orr	r10, r10, r6, lsr #8
	strh	r10, [r12, r1]!
	mov	r10, r10, lsr #16
	strb	r10, [r12, #2]
	orr	r7, r7, r6, lsl #8
	strh	r7, [r12, #-(3*2)]
	mov	r7, r7, lsr #16
	strb	r7, [r12, #-4]

	bcc	asm_Write12CtrRgb8Block8_50

	movs	r2, r2, lsr #1
	ldrcs	r1, [sp, #0x00]

	add	r5, r5, #0x08

	adrcs	r3, dstOffsetTbl12CtrRgb8Block8

	bne	asm_Write12CtrRgb8Block8_50

	pop  {r0, r4-r11, pc}

constTbl12CtrRgb8Block8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)

dstOffsetTbl12CtrRgb8Block8
	dcd	__cpp((0x05040100UL*3) + 0x06060606)
	dcd	__cpp((0x15141110UL*3) + 0x06060606)
	dcd	__cpp((0x0d0c0908UL*3) + 0x06060606)
	dcd	__cpp((0x1d1c1918UL*3) + 0x06060606)
	dcd	__cpp((0x25242120UL*3) + 0x06060606)
	dcd	__cpp((0x35343130UL*3) + 0x06060606)
	dcd	__cpp((0x2d2c2928UL*3) + 0x06060606)
	dcd	__cpp((0x3d3c3938UL*3) + 0x06060606)









}

       void JpegMpDecoderAsmWrite22CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #4
	add	r12, r12, r1, lsl #3

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22CtrRgb8Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl22CtrRgb8Block8

	ldr	r2, =0x00210088

asm_Write22CtrRgb8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10,   [r5, #0x08]
	ldrb	r8, [r5, #0x09]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	pkhbt	r10, r8, r10, lsl #16

	ldrb	r8, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r8, r7, r8, lsl #16

	pkhbt	lr, r11, r6, lsl #16
	pkhbt	r6,     r6, r12, lsl #16
	pkhbt	r12,     r12, r11, lsl #16

	qadd16	r10, lr, r8
	qadd16	r7, r6,     r8
	qadd16	r11, r12,     r8

	ldrh	r8, [r3], #2

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r11, #8, r11

	qadd16	lr, lr, r10
	qadd16	r6,     r6,     r10
	qadd16	r12,     r12,     r10

	usat16	lr, #8, lr
	usat16	r6,     #8, r6
	usat16	r12,     #8, r12

	orr	r7, r7, r10, lsl #8
	orr	r11, r11, r11, lsl #24
	pkhtb	r10, r11, r7, asr #16
	str	r10, [r8, r1]!

	orr	r12, r12, r12, lsl #24
	orr	r6, r6, lr, lsl #8
	mov	r12, r12, lsr #16
	pkhbt	r10, r7, r6
	pkhbt	lr, r12, r6, lsl #16
	stmib	r8, {r10, lr}

	bcc	asm_Write22CtrRgb8Block8_50

	movs	r2, r2, lsr #16 + 1
	ldrcs	r1, [sp, #0x00]
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	adrcs	r3, dstOffsetTbl22CtrRgb8Block8

	bne	asm_Write22CtrRgb8Block8_50

	pop  {r0, r4-r11, pc}

constTbl22CtrRgb8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

dstOffsetTbl22CtrRgb8Block8
	dcw	0x00*3,0x04*3,0x10*3,0x14*3,0x40*3,0x44*3,0x50*3,0x54*3
	dcw	0x08*3,0x0c*3,0x18*3,0x1c*3,0x48*3,0x4c*3,0x58*3,0x5c*3
	dcw	0x20*3,0x24*3,0x30*3,0x34*3,0x60*3,0x64*3,0x70*3,0x74*3
	dcw	0x28*3,0x2c*3,0x38*3,0x3c*3,0x68*3,0x6c*3,0x78*3,0x7c*3









}

       void JpegMpDecoderAsmWrite11Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11Rgba8

	sub	r3, r3, #(8 * 4)
	str	r3, [sp, #0x00]

	mov	r2, #0x80

asm_Write11Rgba8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	adds	r2, r2, #0x40000000

	orr	r11, r7, r6, lsl #16

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	uxtb16	r7, r11
	uxtb16	r11, r11, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r11, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r11, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r11, r10, r12

	add	r3, r6, r3, asr #15
	add	r8, r6, r8, asr #14
	add	r7, r6, r7, asr #14

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2 + 1]

	pkhbt	r3, r3, lr
	pkhbt	r8, r8, r7, lsl #16

	add	r10, r6, r10, asr #15
	add	r11, r6, r11, asr #14
	add	r5, r6, r5, asr #14

	pkhbt	r10, r10, lr
	pkhbt	r11, r11, r5, lsl #16

	usat16	r3, #8, r3
	usat16	r8, #8, r8
	usat16	r10, #8, r10
	usat16	r11, #8, r11

	orr	r11, r8, r3, lsl #8
	orr	r6, r11, r10, lsl #8
	stmia	r1!, {r11, r6}

	bcc	asm_Write11Rgba8_50

	ldr	r7, [sp, #0x00]

	movs	r2, r2, lsr #1

	addne	r1, r1, r7

	bne	asm_Write11Rgba8_50

	pop  {r0, r4-r11, pc}

constTbl11Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)








}

       void JpegMpDecoderAsmWrite21Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

	sub	r3, r3, #(16 * 4)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21Rgba8

	ldr	r2, =0x00210080

asm_Write21Rgba8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10, [r5], #2

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r7, [r5, #0x01 - 2]

	pkhtb	lr, lr, r6, asr #15

	mov	r11, r11, asr #14
	pkhbt	r12, r11, r12, lsl #16 - 14

	qadd16	r12,  lr, r10
	qadd16	lr, lr, r7
	orr	r10, r10, r10, lsl #16
	orr	r7, r7, r7, lsl #16
	qadd16	r10, r12, r10
	qadd16	r11, r12, r7

	usat16	r12, #8, r12
	usat16	lr, #8, lr
	usat16	r10, #8, r10
	usat16	r11, #8, r11

	orr	r10, r10, r12, lsl #8
	orr	r11, r11, lr, lsl #8
	stmia	r1!, {r10, r11}

	bcc	asm_Write21Rgba8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21Rgba8_50

	pop  {r0, r4-r11, pc}

constTbl21Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)








}

       void JpegMpDecoderAsmWrite12Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12Rgba8

	mov	r2, #0x80

asm_Write12Rgba8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9, lr}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r12, [r5, #0x08 - 1]

	mov	r11, r11, asr #14
	pkhbt	r12, r11, r12, lsl #16 - 14

	pkhtb	lr, lr, r6, asr #15

	qadd16	r10,     lr, r7
	qadd16	lr, lr, r12
	orr	r7, r7, r7, lsl #16
	orr	r12, r12, r12, lsl #16
	qadd16	r7, r12, r7
	qadd16	r12, r12, r12

	usat16	r10, #8, r10
	usat16	lr, #8, lr
	usat16	r7, #8, r7
	usat16	r12, #8, r12

	orr	r7, r7, r10, lsl #8
	orr	r6, r12, lr, lsl #8
	str	r6, [r1, r3]
	str	r7, [r1], #4

	bcc	asm_Write12Rgba8_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(8 * 4)

	bne	asm_Write12Rgba8_50

	pop  {r0, r4-r11, pc}

constTbl12Rgba8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp(s_DefaultAlpha << 16)








}

       void JpegMpDecoderAsmWrite22Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22Rgba8

	ldr	r2, =0x00210088

asm_Write22Rgba8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r10,   [r5, #0x08]
	ldrb	r8, [r5, #0x09]

	pkhtb	lr, lr, r6, asr #15

	mov	r11, r11, asr #14
	pkhbt	r12, r11, r12, lsl #16 - 14

	qadd16	r7, lr, r10
	qadd16	r6, lr, r8
	orr	r10,   r10,   r10,   lsl #16
	orr	r8, r8, r8, lsl #16
	qadd16	r10, r12, r10
	qadd16	r11, r12, r8

	usat16	r7, #8, r7
	usat16	r6, #8, r6
	usat16	r10, #8, r10
	usat16	r11, #8, r11

	ldrb	r10,   [r5], #2
	ldrb	r8, [r5, #0x01 - 2]

	orr	r10, r10, r7, lsl #8
	add	r7, r1, r3
	orr	r11, r11, r6, lsl #8
	stmia	r7, {r10, r11}

	qadd16	r7, lr, r10
	qadd16	r6, lr, r8
	orr	r10,   r10,   r10,   lsl #16
	orr	r8, r8, r8, lsl #16
	qadd16	r10, r12, r10
	qadd16	r11, r12, r8

	usat16	r7, #8, r7
	usat16	r6, #8, r6
	usat16	r10, #8, r10
	usat16	r11, #8, r11

	orr	r10, r10, r7, lsl #8
	orr	r11, r11, r6, lsl #8
	stmia	r1!, {r10, r11}

	bcc	asm_Write22Rgba8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(16 * 4)

	bne	asm_Write22Rgba8_50

	pop  {r0, r4-r11, pc}

constTbl22Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)








}

       void JpegMpDecoderAsmWrite11CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #5

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11CtrRgba8Block8
	adr	r3, dstOffsetTbl11CtrRgba8Block8

	mov	r2, #0x80

asm_Write11CtrRgba8Block8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	adds	r2, r2, #0x40000000

	orr	r6, r7, r6, lsl #16

	uxtb16	r7, r6
	uxtb16	r6, r6, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r6, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r6, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r6, r10, r12

	ldrb	r10,      [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]
	ldrb	r12, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2 + 1]

	pkhbt	r3, lr, r3, lsl #16 - 15
	pkhbt	r10, lr, r10, lsl #16 - 15
	mov	r7, r7, asr #14
	mov	r5, r5, asr #14
	pkhbt	r8, r7, r8, lsl #16 - 14
	pkhbt	r11, r5, r11, lsl #16 - 14

	orr	r10, r10, r10, lsl #16
	orr	r12, r12, r12, lsl #16
	qadd16	r8, r8, r10
	qadd16	r3, r3, r10
	qadd16	r11, r11, r12
	qadd16	r10, r10, r12

	ldrb	r10, [r3], #1

	usat16	r8, #8, r8
	usat16	r3, #8, r3
	usat16	r11, #8, r11
	usat16	r10, #8, r10

	orr	r8, r3, r8, lsl #8
	str	r8, [r10, r1]!
	orr	r11, r10, r11, lsl #8
	str	r11, [r10, #4]

	bcc	asm_Write11CtrRgba8Block8_50

	movs	r2, r2, lsr #1

	bne	asm_Write11CtrRgba8Block8_50

	pop  {r0, r4-r11, pc}

constTbl11CtrRgba8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

dstOffsetTbl11CtrRgba8Block8
	dcd	__cpp(0x14100400UL*4)
	dcd	__cpp(0x16120602UL*4)
	dcd	__cpp(0x1c180c08UL*4)
	dcd	__cpp(0x1e1a0e0aUL*4)
	dcd	__cpp(0x34302420UL*4)
	dcd	__cpp(0x36322622UL*4)
	dcd	__cpp(0x3c382c28UL*4)
	dcd	__cpp(0x3e3a2e2aUL*4)









}

       void JpegMpDecoderAsmWrite21CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #5

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21CtrRgba8Block8
	adr	r3, dstOffsetTbl21CtrRgba8Block8

	ldr	r2, =0x00210080

asm_Write21CtrRgba8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10, [r5], #2
	ldrb	r8, [r5, #0x01 - 2]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	orr	r10, r10, r10, lsl #16
	orr	r7, r8, r8, lsl #16

	pkhbt	r6, lr, r6, lsl #16 - 15
	mov	r12, r12, asr #14
	pkhbt	r11, r12, r11, lsl #16 - 14

	qadd16	r8, r11, r10
	qadd16	r12,  r6, r10
	qadd16	r11,     r11, r7
	qadd16	r6,     r6, r7

	ldrb	r12, [r3], #1

	usat16	r10, #8, r8
	usat16	r7, #8, r12
	usat16	r11, #8, r11
	usat16	r6, #8, r6

	add	r12, r1, r12, lsl #2
	orr	r10, r7, r10, lsl #8
	orr	r11, r6, r11, lsl #8
	stmia	r12, {r10, r11}

	bcc	asm_Write21CtrRgba8Block8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	bne	asm_Write21CtrRgba8Block8_50

	pop  {r0, r4-r11, pc}

constTbl21CtrRgba8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

dstOffsetTbl21CtrRgba8Block8
	dcd	0x14100400,0x54504440
	dcd	0x16120602,0x56524642
	dcd	0x1c180c08,0x5c584c48
	dcd	0x1e1a0e0a,0x5e5a4e4a
	dcd	0x34302420,0x74706460
	dcd	0x36322622,0x76726662
	dcd	0x3c382c28,0x7c786c68
	dcd	0x3e3a2e2a,0x7e7a6e6a









}

       void JpegMpDecoderAsmWrite12CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #5

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12CtrRgba8Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl12CtrRgba8Block8

	mov	r2, #0x88

asm_Write12CtrRgba8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9, lr}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5, #0x08 - 1]

	orr	r7, r7, r7, lsl #16

	mov	r12, r12, asr #14
	pkhbt	r11, r12, r11, lsl #16 - 14
	pkhbt	r6, lr, r6, lsl #16 - 15

	orr	r10, r10, r10, lsl #16

	qadd16	r8, r11, r7
	qadd16	r12,  r6, r7
	qadd16	r11,     r11, r10
	qadd16	r6,     r6, r10

	ldrb	r12, [r3], #1

	usat16	r10, #8, r8
	usat16	r7, #8, r12
	usat16	r11, #8, r11
	usat16	r6, #8, r6

	orr	r10, r7, r10, lsl #8
	str	r10, [r12, r1]!
	orr	r11, r6, r11, lsl #8
	str	r11, [r12, #2*4]

	bcc	asm_Write12CtrRgba8Block8_50

	movs	r2, r2, lsr #1
	ldrcs	r1, [sp, #0x00]

	add	r5, r5, #0x08

	adrcs	r3, dstOffsetTbl12CtrRgba8Block8

	bne	asm_Write12CtrRgba8Block8_50

	pop  {r0, r4-r11, pc}

constTbl12CtrRgba8Block8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp(s_DefaultAlpha)

dstOffsetTbl12CtrRgba8Block8
	dcd	__cpp(0x05040100UL*4)
	dcd	__cpp(0x15141110UL*4)
	dcd	__cpp(0x0d0c0908UL*4)
	dcd	__cpp(0x1d1c1918UL*4)
	dcd	__cpp(0x25242120UL*4)
	dcd	__cpp(0x35343130UL*4)
	dcd	__cpp(0x2d2c2928UL*4)
	dcd	__cpp(0x3d3c3938UL*4)









}

       void JpegMpDecoderAsmWrite22CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8










	ldr	r12,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #5

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22CtrRgba8Block8

	add	r3, r1, r3, lsl #3
	str	r3, [sp, #0x00]

	adr	r3, dstOffsetTbl22CtrRgba8Block8

	ldr	r2, =0x00210088

asm_Write22CtrRgba8Block8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10,   [r5], #2
	ldrb	r8, [r5, #0x01 - 2]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	orr	r10,   r10,   r10,   lsl #16
	orr	r8, r8, r8, lsl #16

	pkhbt	r6, lr, r6, lsl #16 - 15
	mov	r12, r12, asr #14
	pkhbt	r11, r12, r11, lsl #16 - 14

	qadd16	r10, r11, r10
	qadd16	r7, r6, r10
	qadd16	r10,   r11, r8
	qadd16	r8, r6, r8

	ldrb	lr, [r3], #1

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r10,   #8, r10
	usat16	r8, #8, r8

	orr	r10, r7, r10, lsl #8
	ldrb	r7, [r5, #0x08 - 2]
	orr	r12, r8, r10,   lsl #8
	add	lr, r1, lr, lsl #2
	ldrb	r8, [r5, #0x09 - 2]
	stmia	lr!, {r10, r12}

	orr	r7, r7, r7, lsl #16
	orr	r8, r8, r8, lsl #16

	qadd16	r10, r11, r7
	qadd16	r7, r6, r7
	qadd16	r11, r11, r8
	qadd16	r6, r6, r8

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r11, #8, r11
	usat16	r6, #8, r6

	orr	r10, r7, r10, lsl #8
	orr	r11, r6, r11, lsl #8
	stmia	lr, {r10, r11}

	bcc	asm_Write22CtrRgba8Block8_50

	movs	r2, r2, lsr #16 + 1
	ldrcs	r1, [sp, #0x00]
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	adrcs	r3, dstOffsetTbl22CtrRgba8Block8

	bne	asm_Write22CtrRgba8Block8_50

	pop  {r0, r4-r11, pc}

constTbl22CtrRgba8Block8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

dstOffsetTbl22CtrRgba8Block8
	dcd	0x14100400,0x54504440
	dcd	0x1c180c08,0x5c584c48
	dcd	0x34302420,0x74706460
	dcd	0x3c382c28,0x7c786c68









}

       void JpegMpDecoderAsmWrite11Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11Bgr8

	sub	r3, r3, #(8 * 3)
	str	r3, [sp, #0x00]

	mov	r2, #0x80

asm_Write11Bgr8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12}

	adds	r2, r2, #0x40000000

	orr	r11, r7, r6, lsl #16

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	uxtb16	r7, r11
	uxtb16	r11, r11, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r11, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r11, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r11, r10, r12

	orr	r6, r6, r6, lsl #8

	mov	r3, r3, lsl #16 - 15
	mov	r8, r8, lsl #16 - 14
	mov	r7, r7, lsl #16 - 14

	uxtb16	r6, r6, ror #16

	pkhtb	r3, r3, r11, asr #14
	pkhtb	r7, r7, r10, asr #15
	pkhtb	r8, r8, r5, asr #14

	qadd16	r3, r3, r6
	qadd16	r7, r7, r6
	qadd16	r8, r8, r6

	usat16	r3, #8, r3
	usat16	r7, #8, r7
	usat16	r8, #8, r8

	orr	r3, r7, r3, lsl #8
	strh	r3, [r1, #4]
	orr	r8, r8, r8, lsl #24
	pkhtb	r8, r8, r3, asr #16
	str	r8, [r1], #(3 * 2)

	bcc	asm_Write11Bgr8_50

	ldr	r7, [sp, #0x00]

	movs	r2, r2, lsr #1

	addne	r1, r1, r7

	bne	asm_Write11Bgr8_50

	pop  {r0, r4-r11, pc}

constTbl11Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite21Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

	sub	r3, r3, #(16 * 3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21Bgr8

	ldr	r2, =0x00210080

asm_Write21Bgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r10, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r10, r7, r10, lsl #16

	pkhbt	lr, r11, r6, lsl #16
	pkhbt	r6,     r6, r12, lsl #16
	pkhbt	r12,     r12, r11, lsl #16

	qadd16	lr, lr, r10
	qadd16	r6,     r6,     r10
	qadd16	r12,     r12,     r10

	usat16	lr, #8, lr
	usat16	r6,     #8, r6
	usat16	r12,     #8, r12

	orr	r6, r6, lr, lsl #8
	strh	r6, [r1, #4]
	orr	r12, r12, r12, lsl #24
	pkhtb	r12, r12, r6, asr #16
	str	r12, [r1], #(3 * 2)

	bcc	asm_Write21Bgr8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21Bgr8_50

	pop  {r0, r4-r11, pc}

constTbl21Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite12Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12Bgr8

	mov	r2, #0x80

asm_Write12Bgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r10, [r5, #0x08 - 1]

	mov	r11, r11, asr #14
	mov	r12, r12, asr #14
	mov	r6, r6, asr #15

	pkhbt	r7, r7, r7, lsl #16
	pkhbt	r10, r10, r10, lsl #16

	pkhbt	r11, r12, r11, lsl #16
	pkhbt	r12, r7, r10
	pkhbt	r6, r6, r6, lsl #16

	qadd16	r6, r6, r12
	qadd16	r10, r11, r10
	qadd16	r7, r11, r7

	usat16	r6, #8, r6
	usat16	r10, #8, r10
	usat16	r7, #8, r7

	orr	r10, r10, r6, lsr #8
	strh	r10, [r1, r3]
	orr	r7, r7, r6, lsl #8
	strh	r7, [r1], #2
	mov	r10, r10, lsr #16
	strb	r10, [r1, r3]
	mov	r7, r7, lsr #16
	strb	r7, [r1], #1

	bcc	asm_Write12Bgr8_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(8 * 3)

	bne	asm_Write12Bgr8_50

	pop  {r0, r4-r11, pc}

constTbl12Bgr8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)








}

       void JpegMpDecoderAsmWrite22Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	push {r0, r4-r11, lr}

	add	r1, r1, r1, lsl #1
	add	r12, r12, r1

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22Bgr8

	ldr	r2, =0x00210088

asm_Write22Bgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10,   [r5, #0x08]
	ldrb	r8, [r5, #0x09]

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	pkhbt	r10, r8, r10, lsl #16

	ldrb	r8, [r5], #2
	ldrb	r7, [r5, #0x01 - 2]

	mov	r6, r6, asr #15
	mov	r11, r11, asr #14
	mov	r12, r12, asr #14

	pkhbt	r8, r7, r8, lsl #16

	pkhbt	lr, r11, r6, lsl #16
	pkhbt	r6,     r6, r12, lsl #16
	pkhbt	r12,     r12, r11, lsl #16

	qadd16	r10, lr, r10
	qadd16	r7, r6,     r10
	qadd16	r11, r12,     r10

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r11, #8, r11

	qadd16	lr, lr, r8
	qadd16	r6,     r6,     r8
	qadd16	r12,     r12,     r8

	usat16	lr, #8, lr
	usat16	r6,     #8, r6
	usat16	r12,     #8, r12

	orr	r7, r7, r10, lsl #8
	add	r10, r1, r3
	orr	r11, r11, r11, lsl #24
	strh	r7, [r10, #4]
	pkhtb	r10, r11, r7, asr #16
	str	r10, [r10]

	orr	r8, r6, lr, lsl #8
	strh	r8, [r1, #4]
	orr	r12, r12, r12, lsl #24
	pkhtb	r12, r12, r8, asr #16
	str	r12, [r1], #(3 * 2)

	bcc	asm_Write22Bgr8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(16 * 3)

	bne	asm_Write22Bgr8_50

	pop  {r0, r4-r11, pc}

constTbl22Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)








}

       void JpegMpDecoderAsmWrite11Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

	add	r4,    r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	r0, constTbl11Abgr8

	sub	r3, r3, #(8 * 4)
	str	r3, [sp, #0x00]

	mov	r2, #0x80

asm_Write11Abgr8_50

	ldrh	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrh	r7, [r4], #2
	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	adds	r2, r2, #0x40000000

	orr	r11, r7, r6, lsl #16

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2]

	uxtb16	r7, r11
	uxtb16	r11, r11, ror #8

	smlad	r3, r7, r8, r9
	smlad	r10, r11, r8, r9

	smlatt	r8, r7, r10, r11
	smlatt	r11, r11, r10, r11

	smlabb	r7, r7, r10, r12
	smlabb	r5, r11, r10, r12

	pkhbt	r3, lr, r3, lsl #16 - 15
	pkhbt	r10, lr, r10, lsl #16 - 15

	ldrb	lr, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr)) - 2 + 1]

	mov	r7, r7, asr #14
	mov	r5, r5, asr #14
	pkhbt	r8, r7, r8, lsl #16 - 14
	pkhbt	r11, r5, r11, lsl #16 - 14

	orr	r6, r6, r6, lsl #16
	orr	lr, lr, lr, lsl #16
	qadd16	r8, r8, r6
	qadd16	r3, r3, r6
	qadd16	r11, r11, lr
	qadd16	r10, r10, lr

	usat16	r8, #8, r8
	usat16	r3, #8, r3
	usat16	r11, #8, r11
	usat16	r10, #8, r10

	orr	r11, r3, r8, lsl #8
	orr	r6, r10, r11, lsl #8
	stmia	r1!, {r11, r6}

	bcc	asm_Write11Abgr8_50

	ldr	r7, [sp, #0x00]

	movs	r2, r2, lsr #1

	addne	r1, r1, r7

	bne	asm_Write11Abgr8_50

	pop  {r0, r4-r11, pc}

constTbl11Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)








}

       void JpegMpDecoderAsmWrite21Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

	sub	r3, r3, #(16 * 4)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl21Abgr8

	ldr	r2, =0x00210080

asm_Write21Abgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9

	ldrb	r10, [r5], #2

	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r7, [r5, #0x01 - 2]

	pkhbt	r6, lr, r6, lsl #16 - 15
	mov	r12, r12, asr #14
	pkhbt	r11, r12, r11, lsl #16 - 14

	orr	r10, r10, r10, lsl #16
	orr	r7, r7, r7, lsl #16

	qadd16	r12,  r11, r10
	qadd16	lr, r6, r10
	qadd16	r11,     r11, r7
	qadd16	r6,     r6, r7

	usat16	r12,  #8, r12
	usat16	lr, #8, lr
	usat16	r11, #8, r11
	usat16	r6, #8, r6

	orr	r10, lr, r12, lsl #8
	orr	r11, r6, r11, lsl #8
	stmia	r1!, {r10, r11}

	bcc	asm_Write21Abgr8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x40

	add	r1, r1, r3

	bne	asm_Write21Abgr8_50

	pop  {r0, r4-r11, pc}

constTbl21Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)








}

       void JpegMpDecoderAsmWrite12Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl12Abgr8

	mov	r2, #0x80

asm_Write12Abgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r10, r11, r12, r8, r9, lr}

	adds	r2, r2, #0x20000000

	pkhbt	r6, r7, r6, lsl #16

	ldrb	r7, [r5], #1

	smlatt	r11, r6, r10, r11
	smlabb	r12, r6, r10, r12
	smlad	r6, r6, r8, r9

	ldrb	r12, [r5, #0x08 - 1]

	orr	r7, r7, r7, lsl #16

	mov	r12, r12, asr #14
	pkhbt	r11, r12, r11, lsl #16 - 14
	pkhbt	r6, lr, r6, lsl #16 - 15

	orr	r12, r12, r12, lsl #16

	qadd16	r10, r11, r7
	qadd16	r7, r6, r7
	qadd16	r11, r11, r12
	qadd16	r6, r6, r12

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r11, #8, r11
	usat16	r6, #8, r6

	orr	r7, r7, r10, lsl #8
	orr	r6, r6, r11, lsl #8
	str	r6, [r1, r3]
	str	r7, [r1], #4

	bcc	asm_Write12Abgr8_50

	movs	r2, r2, lsr #1

	add	r5, r5, #0x08

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(8 * 4)

	bne	asm_Write12Abgr8_50

	pop  {r0, r4-r11, pc}

constTbl12Abgr8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp(s_DefaultAlpha)








}

       void JpegMpDecoderAsmWrite22Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8









	ldr	r12, [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	r3,   [r0, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	push {r0, r4-r11, lr}

	add	r12, r12, r1, lsl #2

	mla	r1, r3, r2, r12

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	r0, constTbl22Abgr8

	ldr	r2, =0x00210088

asm_Write22Abgr8_50

	ldrb	r6, [r4, #(ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))]
	ldrb	r7, [r4], #1

	ldmia	r0, {r8, r9, r10, r11, r12, lr}

	movs	r2, r2, lsl #2

	pkhbt	r7, r7, r6, lsl #16

	addmi	r5, r5, #0x38

	smlad	r6, r7, r8, r9
	smlatt	r11, r7, r10, r11
	smlabb	r12, r7, r10, r12

	ldrb	r10,   [r5, #0x08]
	ldrb	r8, [r5, #0x09]

	pkhbt	r6, lr, r6, lsl #16 - 15
	mov	r12, r12, asr #14
	pkhbt	lr, r12, r11, lsl #16 - 14

	orr	r10,   r10,   r10,   lsl #16
	orr	r8, r8, r8, lsl #16

	qadd16	r10, lr, r10
	qadd16	r7, r6,     r10
	qadd16	r10,   lr, r8
	qadd16	r8, r6,     r8

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r10,   #8, r10
	usat16	r8, #8, r8

	orr	r10, r7, r10, lsl #8
	orr	r11, r8, r10,   lsl #8
	ldrb	r10,   [r5], #2
	add	r7, r1, r3
	ldrb	r8, [r5, #0x01 - 2]
	stmia	r7, {r10, r11}

	orr	r10,   r10,   r10,   lsl #16
	orr	r8, r8, r8, lsl #16

	qadd16	r10, lr, r10
	qadd16	r7, r6,     r10
	qadd16	r10,   lr, r8
	qadd16	r8, r6,     r8

	usat16	r10, #8, r10
	usat16	r7, #8, r7
	usat16	r10,   #8, r10
	usat16	r8, #8, r8

	orr	r10, r7, r10, lsl #8
	orr	r11, r8, r10,   lsl #8
	stmia	r1!, {r10, r11}

	bcc	asm_Write22Abgr8_50

	movs	r2, r2, lsr #16 + 1
	orr	r2, r2, #0x00210000

	sub	r5, r5, #0x38
	addcs	r5, r5, #0x40

	add	r1, r1, r3, lsl #1
	sub	r1, r1, #(16 * 4)

	bne	asm_Write22Abgr8_50

	pop  {r0, r4-r11, pc}

constTbl22Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)
}
#undef ASM_STRUCTURE_MEMBER_OFFSET_OF
#undef ASM_OP_LOG_OR

#pragma arm
