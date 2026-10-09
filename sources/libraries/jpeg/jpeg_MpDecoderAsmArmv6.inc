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

#define arg_pCtx r0
#define t1       r1
#define t2       r2
#define t3       r3
#define t0       r12

	ldrsb	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bitlen)]
	ldr	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bcode2)]
	ldr	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrc)]
	ldr	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, srcOffset)]
	orr	t1, t1, #1 << 31
	mov	t1, t1, ror t0
	rsb	t0, t0, #31
	mov	t1, t1, lsr t0
	mov	t1, t1, lsl t0
	ldr	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, outputBufferWidth)]
	str	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]

	add	t2, t2, t3
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]

	mov	t1, t0, lsl #1
	str	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	add	t2, t1, t0
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	mov	t2, t0, lsl #2
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	bx	lr

#undef arg_pCtx
#undef t1
#undef t2
#undef t3
#undef t0
}

       void JpegMpDecoderAsmConvertWorkToC(register JpegMpDecoderContext* pCtx);
NN_ASM void JpegMpDecoderAsmConvertWorkToC(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx r0
#define t1       r1
#define t2       r2
#define t3       r3
#define t0       r12

	ldr	t1, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	ldr	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	ldr	t3, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrc)]

	rsb	t0, t1, #0
	and	t0, t1, t0
	clz	t0, t0
	strb	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bitlen)]

	rsb	t0, t0, #32
	mov	t0, t1, lsr t0
	str	t0, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, bcode2)]

	sub	t2, t2, t3
	str	t2, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, srcOffset)]

	bx	lr

#undef arg_pCtx
#undef t1
#undef t2
#undef t3
#undef t0
}

       void JpegMpDecoderAsmGetMatrix(register JpegMpDecoderContext* pCtx, register s32 n, register s16 *pFirstElement);
NN_ASM void JpegMpDecoderAsmGetMatrix(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 n NN_IS_UNUSED_VAR, register s16 *pFirstElement NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx r0
#define arg_n    r1
#define arg_pFirstElement r2

#define ax         r0
#define dx         r1
#define t2         r2
#define t3         r3
#define pSrc_      r4
#define pSrcLimit_ r5
#define bcode_     r6
#define chitFF_    r7
#define d          r8
#define cx         r9
#define t0         r10
#define t1         r11
#define pJw        r12
#define v          lr

#define NTBL_ELEM0  624
#define NTBL_ELEM1  (688+624)
#define NTBL_ELEM2  (688*2+624)
#define NTBL_ELEM3  (688*3+624)

#define PROLOGUE_INST         push {r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {r4-r11, r12, pc}
#define STACK_OFFSET_PFIRSTELEMENT 0x20

	macro
	CHECK_PSRC_LIMIT $dest
	cmp	pSrc_, pSrcLimit_
	bcs	$dest
	mend

	mov	r12, arg_pFirstElement

	PROLOGUE_INST

	cmp	arg_n, #0
	moveq	v, #NTBL_ELEM0
	movne	v, #NTBL_ELEM2

	add	d, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[0])

	ldr	t0, offsetJw

	ldr	pSrcLimit_, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pSrcLimit)]

	ldr	pSrc_,      [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	ldr	bcode_,     [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	ldrb	chitFF_,    [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, chitFF)]

	add	pJw, arg_pCtx, t0

	movs	t0, bcode_, lsl #8
	bne	asm_getmatrix_hloadbit01

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_2

	ldrb	chitFF_, [pSrc_], #1
	rsb	t0, bcode_, #0
	and	t0, bcode_, t0
	bic	bcode_, bcode_, t0
	clz	t0, t0
	add	t0, t0, #8
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orr	t1, chitFF_, #1 << 31
	orr	bcode_, bcode_, t1, ror t0

asm_getmatrix_hloadbit01

	add	t0, pJw, v
	add	t0, t0, bcode_, lsr #24
	ldrb	t1, [t0, #174]
	ldrb	cx, [t0, #430]

	cmp	t1, #0
	beq	asm_getmatrix_hloadbit02

	movs	bcode_, bcode_, lsl t1
	beq	asm_getmatrix_set_error_hlb_0

asm_getmatrix_hloadbit07

	movs	ax, cx
	ldr	t3, [sp, #STACK_OFFSET_PFIRSTELEMENT]
	beq	asm_getmatrix_getmatrix1

	mov	ax, #1

	movs	bcode_, bcode_, lsl #1
	bne	asm_getmatrix_loadbit01

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_5

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

asm_getmatrix_loadbit01

	rsbcc	dx, ax, ax, lsl cx
	movcs	dx, #0
	sbc	ax, ax, #0

asm_getmatrix_getmtxsb01

	subs	cx, cx, #1

	ble	asm_getmatrix_getmtxsb03

	movs	bcode_, bcode_, lsl #1
	adcne	ax, ax, ax
	bne	asm_getmatrix_getmtxsb01

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_5

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

	adc	ax, ax, ax
	b	asm_getmatrix_getmtxsb01

asm_getmatrix_hloadbit02a

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_3

	ldrb	chitFF_, [pSrc_], #1
	rsb	t0, bcode_, #0
	and	t0, bcode_, t0
	bic	bcode_, bcode_, t0
	clz	t0, t0
	add	t0, t0, #8
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orr	t1, chitFF_, #1 << 31
	orr	bcode_, bcode_, t1, ror t0

asm_getmatrix_hloadbit02

	movs	t0, bcode_, lsl #9

	beq	asm_getmatrix_hloadbit02a

asm_getmatrix_hloadbit03

	add	t2, pJw, v
	mov	t1,    bcode_, lsr #23
	ldr	t0, [t2, #(17 + 9) * 4]!
	mov	bcode_, bcode_, lsl #9

asm_getmatrix_hloadbit04
	cmp	t1, t0

	ble	asm_getmatrix_hloadbit06

asm_getmatrix_hloadbit04a

	movs	bcode_, bcode_, lsl #1

	bne	asm_getmatrix_hloadbit05

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_4

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

asm_getmatrix_hloadbit05

	ldr	t0, [t2, #4]!
	adc	t1, t1, t1
	cmp	t1, t0
	ble	asm_getmatrix_hloadbit06

	add	t0, pJw, v
	sub	t0, t2, t0
	cmp	t0, #(17 + 17) * 4
	blt	asm_getmatrix_hloadbit04a

	b	asm_getmatrix_set_error_range_3

asm_getmatrix_hloadbit06

	ldr	t0, [t2, #-(17 * 4)]!

	add	cx, pJw, v
	sub	t2, t2, cx
	cmp	t2, #16 * 4

	add	cx, cx, t2, lsr #1

	bgt	asm_getmatrix_set_error_range_4

	ldrlesh	cx, [cx, #70 * 2]

	cmp	v, #NTBL_ELEM0
	addeq	t1, t1, #20
	addne	t1, t1, #20+312

	sub	t1, t1, t0
	add	t1, t1, cx
	cmp	t1, #__cpp(JPEG_DECODER_JW_SIZE)

	ldrccb	cx, [pJw, t1]
	bcc	asm_getmatrix_hloadbit07

	b	asm_getmatrix_set_error_range_4

asm_getmatrix_getmtxsb03

	sub	ax, ax, dx

asm_getmatrix_getmatrix1

	ldrsh	t2, [t3]

	add	v, v, #NTBL_ELEM1 - NTBL_ELEM0

	add	t2, t2, ax
	strh	t2, [t3]
	strh	t2, [d], #2

	add	t3, d, #63 * 2

asm_getmatrix_getmatrix2

	movs	t0, bcode_, lsl #8

	bne	asm_getmatrix_hloadbit11

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_2

	ldrb	chitFF_, [pSrc_], #1
	rsb	t0, bcode_, #0
	and	t0, bcode_, t0
	bic	bcode_, bcode_, t0
	clz	t0, t0
	add	t0, t0, #8
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orr	t1, chitFF_, #1 << 31
	orr	bcode_, bcode_, t1, ror t0

asm_getmatrix_hloadbit11

	add	t0, pJw, v
	add	t0, t0, bcode_, lsr #24
	ldrb	t1, [t0, #174]
	ldrb	cx, [t0, #430]

	cmp	t1, #0
	beq	asm_getmatrix_hloadbit12

	movs	bcode_, bcode_, lsl t1

	beq	asm_getmatrix_set_error_hlb_0

asm_getmatrix_hloadbit17

	cmp	cx, #0

	beq	asm_getmatrix_getmatrix3

	add	t2, d,  cx, lsr #4
	add	t2, t2, cx, lsr #4
	cmp	t2, t3

	bcs	asm_getmatrix_set_error_range_5

	mov	t0, #0
asm_getmatrix_getmatrix6
	cmp	d, t2
	strcch	t0, [d], #2
	bcc	asm_getmatrix_getmatrix6

asm_getmatrix_getmatrix7

	ands	cx, cx, #15
	mov	ax, cx
	beq	asm_getmatrix_getmatrix8

	mov	ax, #1

	movs	bcode_, bcode_, lsl #1
	bne	asm_getmatrix_loadbit03

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_5

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

asm_getmatrix_loadbit03

	rsbcc	dx, ax, ax, lsl cx
	movcs	dx, #0
	sbc	ax, ax, #0

asm_getmatrix_getmtxsb04

	subs	cx, cx, #1

	ble	asm_getmatrix_getmtxsb06

	movs	bcode_, bcode_, lsl #1
	adcne	ax, ax, ax
	bne	asm_getmatrix_getmtxsb04

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_5

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

	adc	ax, ax, ax
	b	asm_getmatrix_getmtxsb04

asm_getmatrix_getmatrix3

	tst	d, #0x02
	strhne	cx, [d], #2

asm_getmatrix_getmatrix3a

	cmp	d, t3
	strcc	cx, [d], #4
	bcc	asm_getmatrix_getmatrix3a

	b	asm_getmatrix_getmatrix4

asm_getmatrix_hloadbit12a

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_3

	ldrb	chitFF_, [pSrc_], #1
	rsb	t0, bcode_, #0
	and	t0, bcode_, t0
	bic	bcode_, bcode_, t0
	clz	t0, t0
	add	t0, t0, #8
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orr	t1, chitFF_, #1 << 31
	orr	bcode_, bcode_, t1, ror t0

asm_getmatrix_hloadbit12

	movs	t0, bcode_, lsl #9

	beq	asm_getmatrix_hloadbit12a

asm_getmatrix_hloadbit13

	add	t2, pJw, v
	mov	t1,    bcode_, lsr #23
	ldr	t0, [t2, #(17 + 9) * 4]!
	mov	bcode_, bcode_, lsl #9

asm_getmatrix_hloadbit14
	cmp	t1, t0

	ble	asm_getmatrix_hloadbit16

asm_getmatrix_hloadbit14a

	movs	bcode_, bcode_, lsl #1

	bne	asm_getmatrix_hloadbit15

	CHECK_PSRC_LIMIT	asm_getmatrix_set_error_underrun_4

	ldrb	chitFF_, [pSrc_], #1
	mov	t0, #1 << 24
	cmp	chitFF_, #0xff
	addeq	pSrc_, pSrc_, #1
	orrs	bcode_, t0, chitFF_, lsl #24 + 1

asm_getmatrix_hloadbit15

	ldr	t0, [t2, #4]!
	adc	t1, t1, t1
	cmp	t1, t0
	ble	asm_getmatrix_hloadbit16

	add	t0, pJw, v
	sub	t0, t2, t0
	cmp	t0, #(17 + 17) * 4
	blt	asm_getmatrix_hloadbit14a

	b	asm_getmatrix_set_error_range_3

asm_getmatrix_hloadbit16

	ldr	t0, [t2, #-(17 * 4)]!

	add	cx, pJw, v
	sub	t2, t2, cx
	cmp	t2, #16 * 4

	add	cx, cx, t2, lsr #1

	bgt	asm_getmatrix_set_error_range_4

	ldrlesh	cx, [cx, #70 * 2]

	cmp	v, #NTBL_ELEM1
	addeq	t1, t1, #20+36
	addne	t1, t1, #20+348

	sub	t1, t1, t0
	add	t1, t1, cx
	cmp	t1, #__cpp(JPEG_DECODER_JW_SIZE)

	ldrccb	cx, [pJw, t1]
	bcc	asm_getmatrix_hloadbit17

	b	asm_getmatrix_set_error_range_4

asm_getmatrix_getmtxsb06

	sub	ax, ax, dx

asm_getmatrix_getmatrix8

	strh	ax, [d], #2
	cmp	d, t3
	bcc	asm_getmatrix_getmatrix2

asm_getmatrix_getmatrix4

	ldr	t0, offsetJw
	sub	arg_pCtx, pJw, t0

	str	pSrc_,      [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.pSrc2)]
	str	bcode_,     [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.bcode3)]
	strb	chitFF_,    [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, chitFF)]

	EPILOGUE_INST

asm_getmatrix_set_error_underrun_2
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_2)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_3
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_3)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_4
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_4)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_underrun_5
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_UNDERRUN_5)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_3
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_3)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_4
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_4)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_range_5
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_RANGE_5)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error_hlb_0
	mvn	t0, #__cpp(-1-JPEG_DECODER_ERROR_HLB_0)
	b	asm_getmatrix_set_error

asm_getmatrix_set_error
	ldr	t1, offsetJw
	sub	t1, pJw, t1
	ldrsb	r0, [t1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, u.s.errorCode)]
	cmp	r0, #0
	streqb	t0, [t1, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, u.s.errorCode)]

	EPILOGUE_INST

offsetJw
	dcd	ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, jwUnion.jw)

#undef arg_pCtx
#undef arg_n
#undef arg_pFirstElement

#undef ax
#undef dx
#undef t2
#undef t3
#undef pSrc_
#undef pSrcLimit_
#undef bcode_
#undef chitFF_
#undef d
#undef cx
#undef t0
#undef t1
#undef pJw
#undef v

#undef NTBL_ELEM0
#undef NTBL_ELEM1
#undef NTBL_ELEM2
#undef NTBL_ELEM3

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PFIRSTELEMENT

}

       void JpegMpDecoderAsmDecodeBlock(register JpegMpDecoderContext* pCtx, register u8 *d, register s32 n);
NN_ASM void JpegMpDecoderAsmDecodeBlock(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register u8 *d NN_IS_UNUSED_VAR, register s32 n NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx r0
#define arg_pDst r1
#define arg_n    r2

#define tmp_pQyn r3

#define pZigDst  arg_pCtx
#define pQyn     arg_pDst

#define t0       r2
#define t1       r3
#define t2       r4
#define t3       r5
#define t4       r6
#define t5       r7
#define t6       r8
#define t7       r9

#define tz0      r10
#define tz1      r11
#define tz2      r12
#define tz3      lr
#define pMtxN    pQyn

#define pMtx2    pZigDst
#define const_FIX_1082392200_FIX_1414213562 tz0
#define const_FIX_1847759065_FIX_2613125930 tz1
#define t8       tz2

#define pMtx1    tz3

#define pDst     pQyn
#define pMtx2End tz3
#define t9       pMtx2End
#define t10      const_FIX_1847759065_FIX_2613125930

#define PROLOGUE_INST         push {arg_pDst, tmp_pQyn, r4-r11, r12, lr}
#define EPILOGUE_INST         pop  {arg_pDst, tmp_pQyn, r4-r11, r12, pc}
#define STACK_OFFSET_PDST     0x00
#define STACK_OFFSET_PQYN     0x04
#define STACK_OFFSET_PMTX2END 0x28

#define MTX_STRIDE            (1<<MTX_STRIDE_SHIFT)
#define MTX_STRIDE_SHIFT      7

	add	tmp_pQyn, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, qyUnion.qy)
	add	tmp_pQyn, tmp_pQyn, arg_n, lsl #(6 + 1)

	add	r12, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[3][0])

	PROLOGUE_INST

	add	pMtxN, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[0])
	add	pZigDst, arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, mtxUnion.mtx[1])

	ldr	tz3, [pMtxN, #0x14*2]
	ldr	t5,  [pMtxN, #0x08*2]
	ldr	t7,  [pMtxN, #0x02*2]
	ldr	tz2, [pMtxN, #0x22*2]
	ldr	t4,  [pMtxN, #0x0A*2]
	ldr	t6,  [pMtxN, #0x00*2]
	ldr	tz1, [pMtxN, #0x16*2]
	ldr	tz0, [pMtxN, #0x04*2]

	pkhbt	t0, tz3, t5
	pkhbt	t1, t7, tz2
	pkhbt	t2, t6, t4, lsl #16
	pkhtb	t3, tz3, t7, asr #16

	ldr	tz3, [pMtxN, #0x24*2]
	ldr	t7,  [pMtxN, #0x12*2]

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, tz1, t4
	pkhbt	t1, tz0, tz3, lsl #16
	pkhtb	t2, t7, t6, asr #16
	pkhbt	t3, t5, tz2, lsl #16

	ldr	t4,  [pMtxN, #0x20*2]
	ldr	t6,  [pMtxN, #0x06*2]
	ldr	tz2, [pMtxN, #0x30*2]
	ldr	t5,  [pMtxN, #0x0C*2]

	mov	t4, t4, ror #16
	mov	t6, t6, ror #16

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, t4, t7, lsl #16
	pkhbt	t1, t6, tz2, lsl #16
	pkhtb	t2, tz1, tz0, asr #16
	pkhbt	t3, t5, tz3

	ldr	tz3, [pMtxN, #0x18*2]
	ldr	t7,  [pMtxN, #0x26*2]
	ldr	tz0, [pMtxN, #0x10*2]
	ldr	tz1, [pMtxN, #0x2E*2]

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, t7, tz3, lsl #16
	pkhtb	t1, tz2, t5, asr #16
	pkhtb	t2, t4, t6, asr #16
	pkhtb	t3, tz1, tz0, asr #16

	ldr	t4,  [pMtxN, #0x32*2]
	ldr	t6,  [pMtxN, #0x1E*2]
	ldr	tz2, [pMtxN, #0x38*2]
	ldr	t5,  [pMtxN, #0x0E*2]

	mov	t4, t4, ror #16

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, tz1, t6
	pkhbt	t1, tz0, tz2
	pkhbt	t2, t5, t7
	pkhtb	t3, t4, tz3, asr #16

	ldr	t7,  [pMtxN, #0x28*2]
	ldr	tz3, [pMtxN, #0x1A*2]
	ldr	tz0, [pMtxN, #0x3A*2]
	ldr	tz1, [pMtxN, #0x2C*2]

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, t4, t7, lsl #16
	pkhbt	t1, tz3, tz0, lsl #16
	pkhtb	t2, tz1, t5, asr #16
	pkhbt	t3, t6, tz2, lsl #16

	ldr	t6,  [pMtxN, #0x36*2]
	ldr	t5,  [pMtxN, #0x3E*2]
	ldr	t4,  [pMtxN, #0x34*2]
	ldr	tz2, [pMtxN, #0x1C*2]

	mov	t6, t6, ror #16
	mov	t5, t5, ror #16
	mov	t4, t4, ror #16

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, t6, tz1, lsl #16
	pkhtb	t1, t5, tz2, asr #16
	pkhtb	t2, t4, tz3, asr #16
	pkhtb	t3, tz0, t7, asr #16

	ldr	t7,  [pMtxN, #0x3C*2]
	ldr	tz3, [pMtxN, #0x2A*2]

	stmia	pZigDst!, {t0, t1, t2, t3}

	pkhbt	t0, t7, t4, lsl #16
	pkhbt	t1, tz3, t5, lsl #16
	pkhbt	t2, tz2, t6
	pkhtb	t3, t7, tz3, asr #16

	stmia	pZigDst!, {t0, t1, t2, t3}

	sub	pMtx1, pMtx2, #MTX_STRIDE

	ldr	pQyn, [sp, #STACK_OFFSET_PQYN]

	ldr	const_FIX_1082392200_FIX_1414213562, constData0
	ldr	const_FIX_1847759065_FIX_2613125930, constData1

asm_decode_block_decblock02

	ldmia	pMtx1!, {t0, t1, t2, t3}

	ldmia	pQyn!, {t4, t5, t6, t7}

	orr	t8, t0, t1
	orr	t8, t8, t3
	orrs    t8, t8, t2, lsr #16

	beq	asm_decode_block_decblock02a

asm_decode_block_decblock03

	smuad	t8, t0, t4
	smusd	t4, t0, t4

	smusd	t0, t3, t7
	smuad	t7, t3, t7
	smusd	t3, t2, t6
	smuad	t2, t2, t6

	smulbb	t0, t0, const_FIX_1082392200_FIX_1414213562

	smusd	t6, t1, t5
	smuad	t1, t1, t5

	sub	t5, t2, t7
	add	t2, t2, t7

	rsb	t0, t7, t0, asr #FIX_SHIFT

	add	t7, t3, t0
	sub	t3, t3, t0

	sub	t0, t1, t8
	add	t1, t1, t8
	add	t8, t4, t6
	smulbb	t4, t4, const_FIX_1847759065_FIX_2613125930
	smulbt	t8, t8, const_FIX_1847759065_FIX_2613125930
	smulbb	t0, t0, const_FIX_1082392200_FIX_1414213562
	smulbt	t6, t6, const_FIX_1082392200_FIX_1414213562

	rsb	t4, t1, t4, asr #FIX_SHIFT
	add	t4, t4, t8, asr #FIX_SHIFT
	rsb	t0, t4, t0, asr #FIX_SHIFT
	add	t6, t0, t6, asr #FIX_SHIFT
	sub	t6, t6, t8, asr #FIX_SHIFT

	pkhbt	t8, t2, t7, lsl #16
	pkhbt	t2, t1, t4, lsl #16
	ssub16	t7, t8, t2
	sadd16	t1, t8, t2
	mov	t7, t7, ror #16

	ldr	t8, [sp, #STACK_OFFSET_PMTX2END]

	pkhbt	t4, t6, t3, lsl #16
	pkhbt	t3, t0, t5, lsl #16
	ssax	t5, t3, t4
	ssax	t6, t4, t3

	stmia	pMtx2!, {t1, t5, t6, t7}
	cmp	pMtx2, t8
	bcc	asm_decode_block_decblock02

asm_decode_block_decblock04a

	sub	pMtx2End, pMtx2, #MTX_STRIDE - (8 * 2)
	str	pMtx2End, [sp, #STACK_OFFSET_PMTX2END]

	sub	pMtx2, pMtx2, #MTX_STRIDE
	ldr	pDst, [sp, #STACK_OFFSET_PDST]

asm_decode_block_decblock05

	ldr	t1, [pMtx2, #8  * 2]
	ldr	t2, [pMtx2, #16 * 2]
	ldr	t3, [pMtx2, #24 * 2]
	ldr	t4, [pMtx2, #32 * 2]

	ldr	t5, [pMtx2, #40 * 2]
	ldr	t6, [pMtx2, #48 * 2]
	ldr	t7, [pMtx2, #56 * 2]

	orr	t0, t1, t2
	orr	t0, t0, t3
	orr	t0, t0, t4
	orr	t0, t0, t5
	orr	t0, t0, t6
	orrs	t0, t0, t7
	beq	asm_decode_block_decblock05a

	movs	t8, t0, lsl #16
	movnes	t0, t0, lsr #16

	ldr	t0, [pMtx2], #2 * 2
	bne	asm_decode_block_decblock06

	cmp	t8, #0

	pkhtbeq	t2, t2, t6, asr #16
	pkhbtne	t2, t6, t2, lsl #16

	pkhtbeq	t6, t0, t4, asr #16
	pkhbtne	t6, t4, t0, lsl #16

	moveq	t0, t0, lsl #16
	add	t0, t0, #0x80 << __cpp(16 + 5 + s_DqtExtraBits1)

	qsax	t2, t2, t2
	qsax	t6, t6, t6

	usat	t0, #8, t0, asr #__cpp(16 + 5 + s_DqtExtraBits1)

	pkhtbeq	t5, t5, t3, asr #16
	pkhbtne	t5, t3, t5, lsl #16

	smultb	t4, t2, const_FIX_1082392200_FIX_1414213562

	pkhtbeq	t1, t1, t7, asr #16
	pkhbtne	t1, t7, t1, lsl #16

	orr	t0, t0, t0, lsl #8

	ssat	t4, #16, t4, asr #FIX_SHIFT

	orr	t0, t0, t0, lsl #16

	qsub16	t4, t4, t2

	mov	t8, t0
	stmeqia	pDst!, {t0, t8}

	pkhbt	t4, t4, t2, lsl #16

	qsax	t5, t5, t5
	qsax	t1, t1, t1

	qasx	t3, t6, t4
	qsax	t4, t6, t4

	qadd16	t7, t1, t5
	qsub16	t6, t1, t5

	smultb	t5, t5, const_FIX_1847759065_FIX_2613125930
	smultt	t2, t7, const_FIX_1847759065_FIX_2613125930
	smulbb	t6, t6, const_FIX_1082392200_FIX_1414213562
	smultt	t1, t1, const_FIX_1082392200_FIX_1414213562

	mov	t5, t5, asr #FIX_SHIFT
	add	t5, t5, t2, asr #FIX_SHIFT
	qsub16	t5, t5, t7

	mov	t6, t6, asr #FIX_SHIFT
	qsub16	t6, t6, t5

	mov	t1, t1, asr #FIX_SHIFT
	sub	t1, t1, t2, asr #FIX_SHIFT
	qadd16	t1, t1, t6

	pkhbt	t7, t7, t6, lsl #16
	qadd16	t6, t4, t7
	pkhbt	t2, t5, t1, lsl #16
	qsub16	t4, t4, t7
	qasx	t7, t3, t2
	qsax	t5, t3, t2

	ldr	t1, constData_0x00ff00ff

	ssat16	t6, #__cpp(13 + s_DqtExtraBits1), t6
	ssat16	t4, #__cpp(13 + s_DqtExtraBits1), t4
	ssat16	t7, #__cpp(13 + s_DqtExtraBits1), t7
	ssat16	t5, #__cpp(13 + s_DqtExtraBits1), t5

	and	t6, t1, t6, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t4, t1, t4, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t7, t1, t7, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t5, t1, t5, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	t1, t1, t1, ror #1

	orr	t5, t5, t4, ror #8
	orr	t2, t6, t7, ror #8

	eor	t5, t5, t1
	eor	t2, t2, t1

	stmia	pDst!, {t2, t5}
	stmneia	pDst!, {t0, t8}

	cmp	pMtx2, pMtx2End

	bcc	asm_decode_block_decblock05

	EPILOGUE_INST

asm_decode_block_decblock05a
	ldr	t0, [pMtx2], #2 * 2

#if 0

	mov	t4,     #0x00ff0000
	orr	t4, t4, #0x000000ff
#else

	ldr	t4, constData_0x00ff00ff
#endif

	ssat16	t0, #__cpp(13 + s_DqtExtraBits1), t0

	eor	t3, t4, t4, ror #1

	and	t0, t4, t0, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	t1, t0, t3
	eor	t0, t1, t0, lsl #8

	cmp	pMtx2, pMtx2End

	pkhtb	t2, t0, t0, asr #16
	pkhbt	t0, t0, t0, lsl #16
	mov	t3, t2
	mov	t1, t0

	stmia	pDst!, {t0, t1, t2, t3}
	bcc	asm_decode_block_decblock05

	EPILOGUE_INST

asm_decode_block_decblock06

	qadd16	t9, t5, t3
	qadd16	t8, t1, t7
	qsub16	t3, t5, t3
	qsub16	t1, t1, t7
	qadd16	t7, t8, t9
	qsub16	t5, t8, t9

	qsub16	t8, t2, t6
	qadd16	t2, t2, t6

	qadd16	t6, t0, t4

	smulbb	t9, t8, const_FIX_1082392200_FIX_1414213562
	smultb	t8, t8, const_FIX_1082392200_FIX_1414213562

	qsub16	t4, t0, t4

	qadd16	t0, t3, t1

	mov	t9, t9, asr #FIX_SHIFT
	pkhbt	t9, t9, t8, lsl #(16 - FIX_SHIFT)

	smulbt	t8, t0, const_FIX_1847759065_FIX_2613125930
	smultt	t0, t0, const_FIX_1847759065_FIX_2613125930

	qsub16	t9, t9, t2

	mov	t8, t8, asr #FIX_SHIFT
	pkhbt	t8, t8, t0, lsl #(16 - FIX_SHIFT)

	smulbb	t0, t3, const_FIX_1847759065_FIX_2613125930
	smultb	t3, t3, const_FIX_1847759065_FIX_2613125930

	smulbt	t10, t1, const_FIX_1082392200_FIX_1414213562
	smultt	t1,  t1, const_FIX_1082392200_FIX_1414213562

	mov	t0, t0, asr #FIX_SHIFT
	pkhbt	t0, t0, t3, lsl #(16 - FIX_SHIFT)

	smulbb	t3, t5, const_FIX_1082392200_FIX_1414213562
	smultb	t5, t5, const_FIX_1082392200_FIX_1414213562

	mov	t10, t10, asr #FIX_SHIFT
	pkhbt	t1, t10, t1, lsl #(16 - FIX_SHIFT)

	ldr	const_FIX_1847759065_FIX_2613125930, constData1

	mov	t3, t3, asr #FIX_SHIFT
	pkhbt	t3, t3, t5, lsl #(16 - FIX_SHIFT)

	qadd16	t0, t0, t8
	qsub16	t1, t1, t8

	qsub16	t0, t0, t7

	qadd16	t8, t6, t2
	qsub16	t6, t6, t2

	qsub16	t3, t3, t0

	qadd16	t2, t4, t9
	qsub16	t4, t4, t9

	qadd16	t1, t1, t3

	qadd16	t9, t8, t7
	qsub16	t8, t8, t7
	qadd16	t7, t2, t0
	qsub16	t2, t2, t0
	qadd16	t0, t4, t3
	qsub16	t4, t4, t3
	qadd16	t3, t6, t1
	qsub16	t6, t6, t1

	ldr	t1, constData_0x00ff00ff

	ssat16	t9, #__cpp(13 + s_DqtExtraBits1), t9
	ssat16	t8, #__cpp(13 + s_DqtExtraBits1), t8
	ssat16	t7, #__cpp(13 + s_DqtExtraBits1), t7
	ssat16	t2, #__cpp(13 + s_DqtExtraBits1), t2
	ssat16	t0, #__cpp(13 + s_DqtExtraBits1), t0
	ssat16	t4, #__cpp(13 + s_DqtExtraBits1), t4
	ssat16	t3, #__cpp(13 + s_DqtExtraBits1), t3
	ssat16	t6, #__cpp(13 + s_DqtExtraBits1), t6

	and	t9, t1, t9, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t8, t1, t8, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t7, t1, t7, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t2, t1, t2, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t0, t1, t0, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t4, t1, t4, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t3, t1, t3, lsr #__cpp(5 + s_DqtExtraBits1)
	and	t6, t1, t6, lsr #__cpp(5 + s_DqtExtraBits1)

	eor	t1, t1, t1, ror #1

	orr	t9, t9, t7, lsl #8
	orr	t6, t0, t6, lsl #8
	orr	t4, t3, t4, lsl #8
	orr	t8, t2, t8, lsl #8

	pkhbt	t0, t9, t6, lsl #16
	pkhbt	t2, t4, t8, lsl #16
	pkhtb	t3, t6, t9, asr #16
	pkhtb	t5, t8, t4, asr #16

	ldr	pMtx2End, [sp, #STACK_OFFSET_PMTX2END]

	eor	t0, t0, t1
	eor	t2, t2, t1
	eor	t3, t3, t1
	eor	t5, t5, t1

	cmp	pMtx2, pMtx2End

	stmia	pDst!, {t0, t2, t3, t5}
	bcc	asm_decode_block_decblock05

	EPILOGUE_INST

asm_decode_block_decblock02a

	smulbb	t0, t2, t6
	ldr	t8, [sp, #STACK_OFFSET_PMTX2END]

	pkhbt	t0, t0, t0, lsl #16
	mov	t1, t0
	stmia	pMtx2!, {t0, t1}
	stmia	pMtx2!, {t0, t1}

	cmp	pMtx2, t8
	bcc	asm_decode_block_decblock02

	b	asm_decode_block_decblock04a

constData_0x00ff00ff
	dcd	0x00ff00ff

constData0
	dcd	((FIX_1082392200 << 16) ASM_OP_LOG_OR (FIX_1414213562 & 0xffff))
constData1
	dcd	((FIX_1847759065 << 16) ASM_OP_LOG_OR ((-FIX_2613125930) & 0xffff))

#undef arg_pCtx
#undef arg_pDst
#undef arg_n
#undef tmp_pQyn
#undef pZigDst
#undef pQyn
#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef tz0
#undef tz1
#undef tz2
#undef tz3
#undef pMtxN
#undef pMtx2
#undef const_FIX_1082392200_FIX_1414213562
#undef const_FIX_1847759065_FIX_2613125930
#undef t8
#undef pMtx1
#undef pDst
#undef pMtx2End
#undef t9
#undef t10
#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
#undef STACK_OFFSET_PQYN
#undef STACK_OFFSET_PMTX2END
#undef MTX_STRIDE
#undef MTX_STRIDE_SHIFT
}

       void JpegMpDecoderAsmWrite11Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define y0             r0
#define y1             tmp_pDst
#define v              r5
#define u              r6

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {r4-r7}
#define EPILOGUE_INST         pop  {r4-r7}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #8 << DST_PIXEL_SHIFT

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	mov	loopFlags, #0x80

asm_Write11Yuyv8_50

	ldrb	v, [pDcrDcb, #DCR_STRIDE]
	ldrb	u, [pDcrDcb], #2

	ldrb	y1, [pDcrDcb, #DCRDCB_STRIDE - 2 + 1]
	ldrb	y0, [pDcrDcb, #DCRDCB_STRIDE - 2]

	pkhbt	u, u, v, lsl #16

	adds	loopFlags, loopFlags, #0x40000000

	orr	u, y0, u, lsl #8
	orr	u, u, y1, lsl #16
	str	u, [pDst_], #4

	bcc	asm_Write11Yuyv8_50

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, stride

	bne	asm_Write11Yuyv8_50

	EPILOGUE_INST
	bx	lr

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef y0
#undef y1
#undef v
#undef u

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite21Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define y0             r0
#define y1             tmp_pDst
#define v              r6
#define u              r7

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {r4-r7}
#define EPILOGUE_INST         pop  {r4-r7}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #16 << DST_PIXEL_SHIFT

 	add r4, r0, #0x380
    add r5, r0, #0x400

	ldr	loopFlags, =0x00810080

asm_Write21Yuyv8_50

	ldrb	v, [pDcrDcb, #DCR_STRIDE]

	ldrb	y0, [pDrawBlock], #2

	ldrb	u, [pDcrDcb], #1

	ldrb	y1, [pDrawBlock, #-0x01]

	movs	loopFlags, loopFlags, lsl #2

	addmi	pDrawBlock, pDrawBlock, #0x38

	orr	y0, y0, v,  lsl #24
	orr	y0, y0, u,  lsl #8
	orr	y0, y0, y1, lsl #16
	str	y0, [pDst_], #4

	bcc	asm_Write21Yuyv8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00810000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21Yuyv8_50

	EPILOGUE_INST
	bx	lr

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef y0
#undef y1
#undef v
#undef u

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define y0             r0
#define y1             tmp_pDst
#define v              r6
#define u              r7

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {r4-r7}
#define EPILOGUE_INST         pop  {r4-r7}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	mov	loopFlags, #0x80

asm_Write12Yuyv8_50

	ldrb	v, [pDcrDcb, #DCR_STRIDE]
	ldrb	u, [pDcrDcb], #2

	ldrh	y1, [pDrawBlock, #0x08]
	ldrh	y0, [pDrawBlock], #2

	adds	loopFlags, loopFlags, #0x40000000

	mov	v, v, lsl #24
	orr	v, v, u, lsl #8

	orr	y1, y1, y1, lsl #8
	orr	y0, y0, y0, lsl #8

	uxtab16	y1, v, y1
	str	y1, [pDst_, stride]

	uxtab16	y0, v, y0
	str	y0, [pDst_], #4

	bcc	asm_Write12Yuyv8_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #8 << DST_PIXEL_SHIFT

	bne	asm_Write12Yuyv8_50

	EPILOGUE_INST
	bx	lr

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef y0
#undef y1
#undef v
#undef u

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22Yuyv8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Yuyv8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5
#define stride2        tmp_pDst

#define y00            r0
#define y01            r6
#define y10            r7
#define y11            r8

#define vu             r9

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {r4-r9}
#define EPILOGUE_INST         pop  {r4-r9}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	ldr	loopFlags, =0x00210088

	sub	stride2, stride, #0x10 << (DST_PIXEL_SHIFT - 1)

asm_Write22Yuyv8_50

	ldrb	y10, [pDcrDcb, #DCR_STRIDE]
	ldrb	y00, [pDcrDcb], #1

	movs	loopFlags, loopFlags, lsl #2

	addmi	pDrawBlock, pDrawBlock, #0x38

	pkhbt	vu,  y00, y10, lsl #16

	ldrb	y11, [pDrawBlock, #0x09]
	ldrb	y01, [pDrawBlock, #0x01]
	ldrb	y10, [pDrawBlock, #0x08]
	ldrb	y00, [pDrawBlock], #2

	orr	y11, vu,  y11, lsl #8
	orr	y01, vu,  y01, lsl #8
	orr	y11, y10, y11, lsl #8
	str	y11, [pDst_, stride]

	orr	y01, y00, y01, lsl #8
	str	y01, [pDst_], #4

	bcc	asm_Write22Yuyv8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride2, lsl #1

	bne	asm_Write22Yuyv8_50

	EPILOGUE_INST
	bx	lr

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock
#undef stride2

#undef y00
#undef y01
#undef y10
#undef y11

#undef vu

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite11CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define gg2            stride
#define gg3            vu2gAdd
#define rr2            vu2gMul
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            rr1

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_STRIDE   0x00

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

    add r5, r0, #0x400

	adr	pConst, constTbl11CtrRgb565

	sub	stride, stride, #8 << DST_PIXEL_SHIFT
	str	stride, [sp, #STACK_OFFSET_STRIDE]

	mov	loopFlags, #0x80

asm_Write11CtrRgb565_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x40000000

	orr	rr1, gg0, gg1, lsl #16

	ldrh	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2]

	uxtb16	gg0, rr1
	uxtb16	rr1, rr1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, rr1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, rr1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, rr1, v2rMul_u2bMul, u2bAdd

	orr	gg1, gg1, gg1, lsl #8

	mov	gg2, gg2, asr #15
	mov	rr2, rr2, asr #14
	mov	bb2, bb2, asr #14

	uxtb16	gg1, gg1

	pkhbt	gg2, gg2, gg3, lsl #16 - 15
	pkhbt	rr2, rr2, rr3, lsl #16 - 14
	pkhbt	bb2, bb2, bb3, lsl #16 - 14

	qadd16	gg2, gg2, gg1
	qadd16	rr2, rr2, gg1
	qadd16	bb2, bb2, gg1

	usat16	gg2, #8, gg2
	usat16	rr2, #8, rr2
	usat16	bb2, #8, bb2

	and	gg2, rgbMask, gg2, lsr #2
	and	rr2, rgbMask, rr2, lsr #3
	and	bb2, rgbMask, bb2, lsr #3

	orr	bb2, bb2, gg2, lsl #5
	orr	bb2, bb2, rr2, lsl #5 + 6
	str	bb2, [pDst_], #4

	bcc	asm_Write11CtrRgb565_50

	ldr	gg0, [sp, #STACK_OFFSET_STRIDE]

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, gg0

	bne	asm_Write11CtrRgb565_50

	EPILOGUE_INST

constTbl11CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_STRIDE
}

       void JpegMpDecoderAsmWrite21CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #16 << DST_PIXEL_SHIFT

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21CtrRgb565

	ldr	loopFlags, =0x00210080

asm_Write21CtrRgb565_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	gg0, [pDrawBlock, #0x01]
	ldrb	rr0, [pDrawBlock], #2

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	rr0, rr0, gg0, lsl #16

	pkhbt	gg1, gg1, gg1, lsl #16
	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16

	qadd16	gg1, gg1, rr0
	qadd16	rr1, rr1, rr0
	qadd16	bb1, bb1, rr0

	usat16	gg1, #8, gg1
	usat16	rr1, #8, rr1
	usat16	bb1, #8, bb1

	and	gg1, rgbMask, gg1, lsr #2
	and	rr1, rgbMask, rr1, lsr #3
	and	bb1, rgbMask, bb1, lsr #3

	orr	rr0, bb1, gg1, lsl #5
	orr	rr0, rr0, rr1, lsl #5 + 6
	str	rr0, [pDst_], #4

	bcc	asm_Write21CtrRgb565_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21CtrRgb565_50

	EPILOGUE_INST

constTbl21CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12CtrRgb565

	mov	loopFlags, #0x80

asm_Write12CtrRgb565_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock, #0x08 - 1]

	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14
	mov	gg1, gg1, asr #15

	pkhbt	rr0, rr0, gg0, lsl #16

	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16
	pkhbt	gg1, gg1, gg1, lsl #16

	qadd16	rr1, rr1, rr0
	qadd16	bb1, bb1, rr0
	qadd16	gg1, gg1, rr0

	usat16	rr1, #8, rr1
	usat16	bb1, #8, bb1
	usat16	gg1, #8, gg1

	and	rr1, rgbMask, rr1, lsr #3
	and	bb1, rgbMask, bb1, lsr #3
	and	gg1, rgbMask, gg1, lsr #2

	orr	bb1, bb1, rr1, lsl #5 + 6
	orr	bb1, bb1, gg1, lsl #5
	strh	bb1, [pDst_, stride]

	mov	bb1, bb1, lsr #16
	strh	bb1, [pDst_], #2

	bcc	asm_Write12CtrRgb565_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #8 << DST_PIXEL_SHIFT

	bne	asm_Write12CtrRgb565_50

	EPILOGUE_INST

constTbl12CtrRgb565
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	0x003f003f

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef vu2gMul
#undef vu2gAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22CtrRgb565(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb565(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT 1

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22CtrRgb565

	ldr	loopFlags, =0x00210088

asm_Write22CtrRgb565_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	bb0, [pDrawBlock, #0x09]
	ldrb	w,   [pDrawBlock, #0x08]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	pkhbt	w, w, bb0, lsl #16

	ldrb	gg0, [pDrawBlock, #0x01]
	ldrb	bb0, [pDrawBlock], #2

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	gg1, gg1, gg1, lsl #16
	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16

	pkhbt	bb0, bb0, gg0, lsl #16

	qadd16	rr0, rr1, w
	qadd16	gg0, gg1, w

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0

	qadd16	rr1, rr1, bb0
	qadd16	gg1, gg1, bb0

	and	rr0, rgbMask, rr0, lsr #3
	and	gg0, rgbMask, gg0, lsr #2
	orr	rr0, gg0, rr0, lsl #6

	qadd16	gg0, bb1, w
	qadd16	bb1, bb1, bb0

	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	usat16	bb0, #8, gg0
	usat16	bb1, #8, bb1

	and	rr1, rgbMask, rr1, lsr #3
	and	gg1, rgbMask, gg1, lsr #2
	orr	rr1, gg1, rr1, lsl #6

	and	bb0, rgbMask, bb0, lsr #3
	and	bb1, rgbMask, bb1, lsr #3

	orr	rr0, bb0, rr0, lsl #5
	str	rr0, [pDst_, stride]

	orr	rr1, bb1, rr1, lsl #5
	str	rr1, [pDst_], #4

	bcc	asm_Write22CtrRgb565_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #0x10 << DST_PIXEL_SHIFT

	bne	asm_Write22CtrRgb565_50

	EPILOGUE_INST

constTbl22CtrRgb565
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	0x003f003f

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite11CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define gg2            rr1
#define gg3            vu2gMul
#define rr2            vu2gAdd
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            gg1
#define w              v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT (1+3)

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11CtrRgb565Block8
	adr	pDstOffsetTbl, dstOffsetTbl11CtrRgb565Block8

	mov	loopFlags, #0x80

asm_Write11CtrRgb565Block8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x40000000

	orr	gg1, gg0, gg1, lsl #16

	uxtb16	gg0, gg1
	uxtb16	gg1, gg1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, gg1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, gg1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, gg1, v2rMul_u2bMul, u2bAdd

	ldrh	w, [pDcrDcb, #DCRDCB_STRIDE - 2]

	mov	gg2, gg2, asr #15
	mov	rr2, rr2, asr #14
	mov	bb2, bb2, asr #14

	orr	w, w, w, lsl #8

	pkhbt	gg2, gg2, gg3, lsl #16 - 15

	uxtb16	w, w

	pkhbt	rr2, rr2, rr3, lsl #16 - 14
	pkhbt	bb2, bb2, bb3, lsl #16 - 14

	qadd16	gg2, gg2, w
	qadd16	rr2, rr2, w
	qadd16	bb2, bb2, w

	ldrb	w, [pDstOffsetTbl], #1

	usat16	gg2, #8, gg2
	usat16	rr2, #8, rr2
	usat16	bb2, #8, bb2

	and	gg2, rgbMask, gg2, lsr #2
	and	rr2, rgbMask, rr2, lsr #3
	and	bb2, rgbMask, bb2, lsr #3

	orr	bb2, bb2, gg2, lsl #5
	orr	bb2, bb2, rr2, lsl #5 + 6
	str	bb2, [pDst_, w]

	bcc	asm_Write11CtrRgb565Block8_50

	movs	loopFlags, loopFlags, lsr #1

	bne	asm_Write11CtrRgb565Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3
#undef w

#undef pDstOffsetTbl

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite21CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT (1+3)

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21CtrRgb565Block8
	adr	pDstOffsetTbl, dstOffsetTbl21CtrRgb565Block8

	ldr	loopFlags, =0x00210080

asm_Write21CtrRgb565Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	gg0, [pDrawBlock, #0x01]
	ldrb	rr0, [pDrawBlock], #2

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	rr0, rr0, gg0, lsl #16

	ldrb	gg0, [pDstOffsetTbl], #1

	pkhbt	gg1, gg1, gg1, lsl #16
	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16

	qadd16	gg1, gg1, rr0
	qadd16	rr1, rr1, rr0
	qadd16	bb1, bb1, rr0

	usat16	rr0, #8, gg1
	usat16	rr1, #8, rr1
	usat16	bb1, #8, bb1

	and	rr0, rgbMask, rr0, lsr #2
	and	rr1, rgbMask, rr1, lsr #3
	and	bb1, rgbMask, bb1, lsr #3

	orr	rr0, bb1, rr0, lsl #5
	orr	rr0, rr0, rr1, lsl #5 + 6
	str	rr0, [pDst_, gg0]

	bcc	asm_Write21CtrRgb565Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	bne	asm_Write21CtrRgb565Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT (1+3)

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00
	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12CtrRgb565Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl12CtrRgb565Block8

	mov	loopFlags, #0x88

asm_Write12CtrRgb565Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock, #0x08]

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock], #1

	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14
	mov	gg1, gg1, asr #15

	pkhbt	rr0, rr0, gg0, lsl #16

	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16
	pkhbt	gg1, gg1, gg1, lsl #16

	qadd16	rr1, rr1, rr0
	qadd16	bb1, bb1, rr0
	qadd16	gg1, gg1, rr0

	ldrb	rr0, [pDstOffsetTbl], #1

	usat16	rr1, #8, rr1
	usat16	bb1, #8, bb1
	usat16	gg1, #8, gg1

	and	rr1, rgbMask, rr1, lsr #3
	and	bb1, rgbMask, bb1, lsr #3
	and	gg1, rgbMask, gg1, lsr #2

	orr	bb1, bb1, rr1, lsl #5 + 6
	orr	bb1, bb1, gg1, lsl #5
	strh	bb1, [rr0, pDst_]!

	mov	bb1, bb1, lsr #16
	strh	bb1, [rr0, #0x02*2]

	bcc	asm_Write12CtrRgb565Block8_50

	movs	loopFlags, loopFlags, lsr #1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]

	add	pDrawBlock, pDrawBlock, #0x08

	adrcs	pDstOffsetTbl, dstOffsetTbl12CtrRgb565Block8

	bne	asm_Write12CtrRgb565Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef vu2gMul
#undef vu2gAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite22CtrRgb565Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb565Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define DST_PIXEL_SHIFT (1+3)

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX2)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #DST_PIXEL_SHIFT

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22CtrRgb565Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl22CtrRgb565Block8

	ldr	loopFlags, =0x00210088

asm_Write22CtrRgb565Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	bb0, [pDrawBlock, #0x09]
	ldrb	w,   [pDrawBlock, #0x08]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	pkhbt	w, w, bb0, lsl #16

	ldrb	gg0, [pDrawBlock, #0x01]
	ldrb	bb0, [pDrawBlock], #2

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	gg1, gg1, gg1, lsl #16
	pkhbt	rr1, rr1, rr1, lsl #16
	pkhbt	bb1, bb1, bb1, lsl #16

	pkhbt	bb0, bb0, gg0, lsl #16

	qadd16	gg0, gg1, w
	qadd16	rr0, rr1, w

	usat16	gg0, #8, gg0
	usat16	rr0, #8, rr0

	qadd16	gg1, gg1, bb0
	qadd16	rr1, rr1, bb0

	and	rr0, rgbMask, rr0, lsr #3
	and	gg0, rgbMask, gg0, lsr #2
	orr	rr0, gg0, rr0, lsl #6

	qadd16	gg0, bb1, w
	qadd16	bb1, bb1, bb0

	usat16	gg1, #8, gg1
	usat16	rr1, #8, rr1

	usat16	bb0, #8, gg0
	usat16	bb1, #8, bb1

	ldrb	gg0, [pDstOffsetTbl], #1

	and	rr1, rgbMask, rr1, lsr #3
	and	gg1, rgbMask, gg1, lsr #2
	orr	rr1, gg1, rr1, lsl #6

	and	bb0, rgbMask, bb0, lsr #3
	and	bb1, rgbMask, bb1, lsr #3

	orr	w,   bb0, rr0, lsl #5
	orr	bb0, bb1, rr1, lsl #5

	strd	bb0, w, [pDst_, gg0]

	bcc	asm_Write22CtrRgb565Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	adrcs	pDstOffsetTbl, dstOffsetTbl22CtrRgb565Block8

	bne	asm_Write22CtrRgb565Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef DST_PIXEL_SHIFT

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite11Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12

#define gg2            stride
#define gg3            vu2gAdd
#define rr2            vu2gMul
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            rr1

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_STRIDE   0x00

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11Rgb8

	sub	stride, stride, #(8 * 3)
	str	stride, [sp, #STACK_OFFSET_STRIDE]

	mov	loopFlags, #0x80

asm_Write11Rgb8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	adds	loopFlags, loopFlags, #0x40000000

	orr	rr1, gg0, gg1, lsl #16

	ldrh	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2]

	uxtb16	gg0, rr1
	uxtb16	rr1, rr1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, rr1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, rr1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, rr1, v2rMul_u2bMul, u2bAdd

	orr	gg1, gg1, gg1, lsl #8

	mov	gg2, gg2, lsl #16 - 15
	mov	rr2, rr2, lsl #16 - 14
	mov	bb2, bb2, lsl #16 - 14

	uxtb16	gg1, gg1, ror #16

	pkhtb	gg2, gg2, bb3, asr #14
	pkhtb	rr2, rr2, gg3, asr #15
	pkhtb	bb2, bb2, rr3, asr #14

	qadd16	gg2, gg2, gg1
	qadd16	rr2, rr2, gg1
	qadd16	bb2, bb2, gg1

	usat16	gg2, #8, gg2
	usat16	rr2, #8, rr2
	usat16	bb2, #8, bb2

	orr	gg2, rr2, gg2, lsl #8
	strh	gg2, [pDst_, #4]
	orr	bb2, bb2, bb2, lsl #24
	pkhtb	bb2, bb2, gg2, asr #16
	str	bb2, [pDst_], #(3 * 2)

	bcc	asm_Write11Rgb8_50

	ldr	gg0, [sp, #STACK_OFFSET_STRIDE]

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, gg0

	bne	asm_Write11Rgb8_50

	EPILOGUE_INST

constTbl11Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_STRIDE
}

       void JpegMpDecoderAsmWrite21Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #(16 * 3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21Rgb8

	ldr	loopFlags, =0x00210080

asm_Write21Rgb8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	rr0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	rr0, gg0, rr0, lsl #16

	pkhbt	gg0, bb1, gg1, lsl #16
	pkhbt	gg1, gg1, rr1, lsl #16
	pkhbt	rr1, rr1, bb1, lsl #16

	qadd16	gg0, gg0, rr0
	qadd16	gg1, gg1, rr0
	qadd16	rr1, rr1, rr0

	usat16	gg0, #8, gg0
	usat16	gg1, #8, gg1
	usat16	rr1, #8, rr1

	orr	rgbMask, gg1, gg0, lsl #8
	strh	rgbMask, [pDst_, #4]
	orr	rr1, rr1, rr1, lsl #24
	pkhtb	rr1, rr1, rgbMask, asr #16
	str	rr1, [pDst_], #(3 * 2)

	bcc	asm_Write21Rgb8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21Rgb8_50

	EPILOGUE_INST

constTbl21Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12Rgb8

	mov	loopFlags, #0x80

asm_Write12Rgb8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock, #0x08 - 1]

	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14
	mov	gg1, gg1, asr #15

	pkhbt	gg0, gg0, gg0, lsl #16
	pkhbt	rr0, rr0, rr0, lsl #16

	pkhbt	rr1, rr1, bb1, lsl #16
	pkhbt	bb1, gg0, rr0
	pkhbt	gg1, gg1, gg1, lsl #16

	qadd16	gg1, gg1, bb1
	qadd16	rr0, rr1, rr0
	qadd16	gg0, rr1, gg0

	usat16	gg1, #8, gg1
	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0

	orr	rr0, rr0, gg1, lsr #8
	strh	rr0, [pDst_, stride]
	orr	gg0, gg0, gg1, lsl #8
	strh	gg0, [pDst_], #2
	mov	rr0, rr0, lsr #16
	strb	rr0, [pDst_, stride]
	mov	gg0, gg0, lsr #16
	strb	gg0, [pDst_], #1

	bcc	asm_Write12Rgb8_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(8 * 3)

	bne	asm_Write12Rgb8_50

	EPILOGUE_INST

constTbl12Rgb8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef vu2gMul
#undef vu2gAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22Rgb8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Rgb8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22Rgb8

	ldr	loopFlags, =0x00210088

asm_Write22Rgb8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	w,   [pDrawBlock, #0x08]
	ldrb	bb0, [pDrawBlock, #0x09]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	pkhbt	w, bb0, w, lsl #16

	ldrb	bb0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	bb0, gg0, bb0, lsl #16

	pkhbt	rgbMask, bb1, gg1, lsl #16
	pkhbt	gg1,     gg1, rr1, lsl #16
	pkhbt	rr1,     rr1, bb1, lsl #16

	qadd16	rr0, rgbMask, w
	qadd16	gg0, gg1,     w
	qadd16	bb1, rr1,     w

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	bb1, #8, bb1

	qadd16	rgbMask, rgbMask, bb0
	qadd16	gg1,     gg1,     bb0
	qadd16	rr1,     rr1,     bb0

	usat16	rgbMask, #8, rgbMask
	usat16	gg1,     #8, gg1
	usat16	rr1,     #8, rr1

	orr	bb0, gg0, rr0, lsl #8
	add	gg0, pDst_, stride
	orr	bb1, bb1, bb1, lsl #24
	strh	bb0, [gg0, #4]
	pkhtb	bb1, bb1, bb0, asr #16
	str	bb1, [gg0]

	orr	bb0, gg1, rgbMask, lsl #8
	strh	bb0, [pDst_, #4]
	orr	rr1, rr1, rr1, lsl #24
	pkhtb	rr1, rr1, bb0, asr #16
	str	rr1, [pDst_], #(3 * 2)

	bcc	asm_Write22Rgb8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(16 * 3)

	bne	asm_Write22Rgb8_50

	EPILOGUE_INST

constTbl22Rgb8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite11CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12

#define gg2            rr1
#define gg3            vu2gMul
#define rr2            vu2gAdd
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            gg1
#define w              v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #4
	add	tmp_pDst, tmp_pDst, arg_x, lsl #3

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11CtrRgb8Block8
	adr	pDstOffsetTbl, dstOffsetTbl11CtrRgb8Block8

	mov	loopFlags, #0x80

asm_Write11CtrRgb8Block8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	adds	loopFlags, loopFlags, #0x40000000

	orr	gg1, gg0, gg1, lsl #16

	uxtb16	gg0, gg1
	uxtb16	gg1, gg1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, gg1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, gg1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, gg1, v2rMul_u2bMul, u2bAdd

	ldrh	w, [pDcrDcb, #DCRDCB_STRIDE - 2]

	mov	gg2, gg2, lsl #16 - 15
	mov	rr2, rr2, lsl #16 - 14
	mov	bb2, bb2, lsl #16 - 14

	orr	w, w, w, lsl #8

	pkhtb	gg2, gg2, rr3, asr #14
	uxtb16	w, w, ror #16
	pkhtb	bb2, bb2, gg3, asr #15
	pkhtb	rr2, rr2, bb3, asr #14

	qadd16	gg2, gg2, w
	qadd16	bb2, bb2, w
	qadd16	rr2, rr2, w

	ldrb	w, [pDstOffsetTbl], #1

	usat16	gg2, #8, gg2
	usat16	bb2, #8, bb2
	usat16	rr2, #8, rr2

	orr	gg2, bb2, gg2, lsl #8
	strh	gg2, [w, pDst_]!
	orr	rr2, rr2, rr2, lsl #24
	pkhtb	rr2, rr2, gg2, asr #16
	str	rr2, [w, #-4]

	bcc	asm_Write11CtrRgb8Block8_50

	movs	loopFlags, loopFlags, lsr #1

	bne	asm_Write11CtrRgb8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3
#undef w

#undef pDstOffsetTbl

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite21CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #4
	add	tmp_pDst, tmp_pDst, arg_x, lsl #3

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21CtrRgb8Block8
	adr	pDstOffsetTbl, dstOffsetTbl21CtrRgb8Block8

	ldr	loopFlags, =0x00210080

asm_Write21CtrRgb8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	rr0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	rr0, gg0, rr0, lsl #16

	ldrh	gg0, [pDstOffsetTbl], #2

	pkhbt	rgbMask, rr1, gg1, lsl #16
	pkhbt	gg1,     gg1, bb1, lsl #16
	pkhbt	bb1,     bb1, rr1, lsl #16

	qadd16	rgbMask, rgbMask, rr0
	qadd16	gg1,     gg1,     rr0
	qadd16	bb1,     bb1,     rr0

	usat16	rgbMask, #8, rgbMask
	usat16	gg1,     #8, gg1
	usat16	bb1,     #8, bb1

	orr	gg1, gg1, rgbMask, lsl #8
	strh	gg1, [gg0, pDst_]!
	orr	bb1, bb1, bb1, lsl #24
	pkhtb	bb1, bb1, gg1, asr #16
	str	bb1, [gg0, #-4]

	bcc	asm_Write21CtrRgb8Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	bne	asm_Write21CtrRgb8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00
	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #4
	add	tmp_pDst, tmp_pDst, arg_x, lsl #3

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12CtrRgb8Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl12CtrRgb8Block8

	mov	loopFlags, #0x88

asm_Write12CtrRgb8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock, #0x08 - 1]

	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14
	mov	gg1, gg1, asr #15

	pkhbt	gg0, gg0, gg0, lsl #16
	pkhbt	rr0, rr0, rr0, lsl #16

	pkhbt	rr1, bb1, rr1, lsl #16
	pkhbt	bb1, gg0, rr0
	pkhbt	gg1, gg1, gg1, lsl #16

	ldrb	u2bAdd, [pDstOffsetTbl], #1

	qadd16	gg1, gg1, bb1
	qadd16	rr0, rr1, rr0
	qadd16	gg0, rr1, gg0

	usat16	gg1, #8, gg1
	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0

	orr	rr0, rr0, gg1, lsr #8
	strh	rr0, [u2bAdd, pDst_]!
	mov	rr0, rr0, lsr #16
	strb	rr0, [u2bAdd, #2]
	orr	gg0, gg0, gg1, lsl #8
	strh	gg0, [u2bAdd, #-(3*2)]
	mov	gg0, gg0, lsr #16
	strb	gg0, [u2bAdd, #-4]

	bcc	asm_Write12CtrRgb8Block8_50

	movs	loopFlags, loopFlags, lsr #1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]

	add	pDrawBlock, pDrawBlock, #0x08

	adrcs	pDstOffsetTbl, dstOffsetTbl12CtrRgb8Block8

	bne	asm_Write12CtrRgb8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef vu2gMul
#undef vu2gAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite22CtrRgb8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgb8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #4
	add	tmp_pDst, tmp_pDst, arg_x, lsl #3

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22CtrRgb8Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl22CtrRgb8Block8

	ldr	loopFlags, =0x00210088

asm_Write22CtrRgb8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	w,   [pDrawBlock, #0x08]
	ldrb	bb0, [pDrawBlock, #0x09]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	pkhbt	w, bb0, w, lsl #16

	ldrb	bb0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	bb0, gg0, bb0, lsl #16

	pkhbt	rgbMask, rr1, gg1, lsl #16
	pkhbt	gg1,     gg1, bb1, lsl #16
	pkhbt	bb1,     bb1, rr1, lsl #16

	qadd16	rr0, rgbMask, bb0
	qadd16	gg0, gg1,     bb0
	qadd16	rr1, bb1,     bb0

	ldrh	bb0, [pDstOffsetTbl], #2

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	rr1, #8, rr1

	qadd16	rgbMask, rgbMask, w
	qadd16	gg1,     gg1,     w
	qadd16	bb1,     bb1,     w

	usat16	rgbMask, #8, rgbMask
	usat16	gg1,     #8, gg1
	usat16	bb1,     #8, bb1

	orr	gg0, gg0, rr0, lsl #8
	orr	rr1, rr1, rr1, lsl #24
	pkhtb	w, rr1, gg0, asr #16
	str	w, [bb0, pDst_]!

	orr	bb1, bb1, bb1, lsl #24
	orr	gg1, gg1, rgbMask, lsl #8
	mov	bb1, bb1, lsr #16
	pkhbt	w, gg0, gg1
	pkhbt	rgbMask, bb1, gg1, lsl #16
	stmib	bb0, {w, rgbMask}

	bcc	asm_Write22CtrRgb8Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	adrcs	pDstOffsetTbl, dstOffsetTbl22CtrRgb8Block8

	bne	asm_Write22CtrRgb8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite11Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define gg2            stride
#define gg3            vu2gAdd
#define rr2            vu2gMul
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            rr1

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_STRIDE   0x00

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11Rgba8

	sub	stride, stride, #(8 * 4)
	str	stride, [sp, #STACK_OFFSET_STRIDE]

	mov	loopFlags, #0x80

asm_Write11Rgba8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x40000000

	orr	rr1, gg0, gg1, lsl #16

	ldrb	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2]

	uxtb16	gg0, rr1
	uxtb16	rr1, rr1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, rr1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, rr1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, rr1, v2rMul_u2bMul, u2bAdd

	add	gg2, gg1, gg2, asr #15
	add	rr2, gg1, rr2, asr #14
	add	bb2, gg1, bb2, asr #14

	ldrb	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2 + 1]

	pkhbt	gg2, gg2, rgbMask
	pkhbt	rr2, rr2, bb2, lsl #16

	add	gg3, gg1, gg3, asr #15
	add	rr3, gg1, rr3, asr #14
	add	bb3, gg1, bb3, asr #14

	pkhbt	gg3, gg3, rgbMask
	pkhbt	rr3, rr3, bb3, lsl #16

	usat16	gg2, #8, gg2
	usat16	rr2, #8, rr2
	usat16	gg3, #8, gg3
	usat16	rr3, #8, rr3

	orr	rr1, rr2, gg2, lsl #8
	orr	gg1, rr3, gg3, lsl #8
	stmia	pDst_!, {rr1, gg1}

	bcc	asm_Write11Rgba8_50

	ldr	gg0, [sp, #STACK_OFFSET_STRIDE]

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, gg0

	bne	asm_Write11Rgba8_50

	EPILOGUE_INST

constTbl11Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_STRIDE
}

       void JpegMpDecoderAsmWrite21Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #(16 * 4)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21Rgba8

	ldr	loopFlags, =0x00210080

asm_Write21Rgba8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock], #2

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	pkhtb	rgbMask, rgbMask, gg1, asr #15

	mov	rr1, rr1, asr #14
	pkhbt	bb1, rr1, bb1, lsl #16 - 14

	qadd16	u2bAdd,  rgbMask, rr0
	qadd16	rgbMask, rgbMask, gg0
	orr	rr0, rr0, rr0, lsl #16
	orr	gg0, gg0, gg0, lsl #16
	qadd16	rr0, bb1, rr0
	qadd16	rr1, bb1, gg0

	usat16	u2bAdd, #8, u2bAdd
	usat16	rgbMask, #8, rgbMask
	usat16	rr0, #8, rr0
	usat16	rr1, #8, rr1

	orr	rr0, rr0, u2bAdd, lsl #8
	orr	rr1, rr1, rgbMask, lsl #8
	stmia	pDst_!, {rr0, rr1}

	bcc	asm_Write21Rgba8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21Rgba8_50

	EPILOGUE_INST

constTbl21Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12Rgba8

	mov	loopFlags, #0x80

asm_Write12Rgba8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	u2bAdd, [pDrawBlock, #0x08 - 1]

	mov	rr1, rr1, asr #14
	pkhbt	bb1, rr1, bb1, lsl #16 - 14

	pkhtb	rgbMask, rgbMask, gg1, asr #15

	qadd16	rr0,     rgbMask, gg0
	qadd16	rgbMask, rgbMask, u2bAdd
	orr	gg0, gg0, gg0, lsl #16
	orr	u2bAdd, u2bAdd, u2bAdd, lsl #16
	qadd16	gg0, bb1, gg0
	qadd16	bb1, bb1, u2bAdd

	usat16	rr0, #8, rr0
	usat16	rgbMask, #8, rgbMask
	usat16	gg0, #8, gg0
	usat16	bb1, #8, bb1

	orr	gg0, gg0, rr0, lsl #8
	orr	gg1, bb1, rgbMask, lsl #8
	str	gg1, [pDst_, stride]
	str	gg0, [pDst_], #4

	bcc	asm_Write12Rgba8_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(8 * 4)

	bne	asm_Write12Rgba8_50

	EPILOGUE_INST

constTbl12Rgba8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp(s_DefaultAlpha << 16)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef vu2gMul
#undef vu2gAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22Rgba8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Rgba8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22Rgba8

	ldr	loopFlags, =0x00210088

asm_Write22Rgba8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	w,   [pDrawBlock, #0x08]
	ldrb	bb0, [pDrawBlock, #0x09]

	pkhtb	rgbMask, rgbMask, gg1, asr #15

	mov	rr1, rr1, asr #14
	pkhbt	bb1, rr1, bb1, lsl #16 - 14

	qadd16	gg0, rgbMask, w
	qadd16	gg1, rgbMask, bb0
	orr	w,   w,   w,   lsl #16
	orr	bb0, bb0, bb0, lsl #16
	qadd16	rr0, bb1, w
	qadd16	rr1, bb1, bb0

	usat16	gg0, #8, gg0
	usat16	gg1, #8, gg1
	usat16	rr0, #8, rr0
	usat16	rr1, #8, rr1

	ldrb	w,   [pDrawBlock], #2
	ldrb	bb0, [pDrawBlock, #0x01 - 2]

	orr	rr0, rr0, gg0, lsl #8
	add	gg0, pDst_, stride
	orr	rr1, rr1, gg1, lsl #8
	stmia	gg0, {rr0, rr1}

	qadd16	gg0, rgbMask, w
	qadd16	gg1, rgbMask, bb0
	orr	w,   w,   w,   lsl #16
	orr	bb0, bb0, bb0, lsl #16
	qadd16	rr0, bb1, w
	qadd16	rr1, bb1, bb0

	usat16	gg0, #8, gg0
	usat16	gg1, #8, gg1
	usat16	rr0, #8, rr0
	usat16	rr1, #8, rr1

	orr	rr0, rr0, gg0, lsl #8
	orr	rr1, rr1, gg1, lsl #8
	stmia	pDst_!, {rr0, rr1}

	bcc	asm_Write22Rgba8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(16 * 4)

	bne	asm_Write22Rgba8_50

	EPILOGUE_INST

constTbl22Rgba8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha << 16)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite11CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define gg2            rr1
#define gg3            vu2gMul
#define rr2            vu2gAdd
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            gg1
#define w              v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #5

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11CtrRgba8Block8
	adr	pDstOffsetTbl, dstOffsetTbl11CtrRgba8Block8

	mov	loopFlags, #0x80

asm_Write11CtrRgba8Block8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x40000000

	orr	gg1, gg0, gg1, lsl #16

	uxtb16	gg0, gg1
	uxtb16	gg1, gg1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, gg1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, gg1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, gg1, v2rMul_u2bMul, u2bAdd

	ldrb	w,      [pDcrDcb, #DCRDCB_STRIDE - 2]
	ldrb	u2bAdd, [pDcrDcb, #DCRDCB_STRIDE - 2 + 1]

	pkhbt	gg2, rgbMask, gg2, lsl #16 - 15
	pkhbt	gg3, rgbMask, gg3, lsl #16 - 15
	mov	bb2, bb2, asr #14
	mov	bb3, bb3, asr #14
	pkhbt	rr2, bb2, rr2, lsl #16 - 14
	pkhbt	rr3, bb3, rr3, lsl #16 - 14

	orr	w, w, w, lsl #16
	orr	u2bAdd, u2bAdd, u2bAdd, lsl #16
	qadd16	rr2, rr2, w
	qadd16	gg2, gg2, w
	qadd16	rr3, rr3, u2bAdd
	qadd16	gg3, gg3, u2bAdd

	ldrb	w, [pDstOffsetTbl], #1

	usat16	rr2, #8, rr2
	usat16	gg2, #8, gg2
	usat16	rr3, #8, rr3
	usat16	gg3, #8, gg3

	orr	rr2, gg2, rr2, lsl #8
	str	rr2, [w, pDst_]!
	orr	rr3, gg3, rr3, lsl #8
	str	rr3, [w, #4]

	bcc	asm_Write11CtrRgba8Block8_50

	movs	loopFlags, loopFlags, lsr #1

	bne	asm_Write11CtrRgba8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3
#undef w

#undef pDstOffsetTbl

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite21CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #5

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21CtrRgba8Block8
	adr	pDstOffsetTbl, dstOffsetTbl21CtrRgba8Block8

	ldr	loopFlags, =0x00210080

asm_Write21CtrRgba8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock], #2
	ldrb	vu2gMul, [pDrawBlock, #0x01 - 2]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	orr	rr0, rr0, rr0, lsl #16
	orr	gg0, vu2gMul, vu2gMul, lsl #16

	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15
	mov	bb1, bb1, asr #14
	pkhbt	rr1, bb1, rr1, lsl #16 - 14

	qadd16	vu2gMul, rr1, rr0
	qadd16	u2bAdd,  gg1, rr0
	qadd16	rr1,     rr1, gg0
	qadd16	gg1,     gg1, gg0

	ldrb	bb1, [pDstOffsetTbl], #1

	usat16	rr0, #8, vu2gMul
	usat16	gg0, #8, u2bAdd
	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	add	bb1, pDst_, bb1, lsl #2
	orr	rr0, gg0, rr0, lsl #8
	orr	rr1, gg1, rr1, lsl #8
	stmia	bb1, {rr0, rr1}

	bcc	asm_Write21CtrRgba8Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	bne	asm_Write21CtrRgba8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00
	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #5

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12CtrRgba8Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl12CtrRgba8Block8

	mov	loopFlags, #0x88

asm_Write12CtrRgba8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock, #0x08 - 1]

	orr	gg0, gg0, gg0, lsl #16

	mov	bb1, bb1, asr #14
	pkhbt	rr1, bb1, rr1, lsl #16 - 14
	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15

	orr	rr0, rr0, rr0, lsl #16

	qadd16	vu2gMul, rr1, gg0
	qadd16	u2bAdd,  gg1, gg0
	qadd16	rr1,     rr1, rr0
	qadd16	gg1,     gg1, rr0

	ldrb	bb1, [pDstOffsetTbl], #1

	usat16	rr0, #8, vu2gMul
	usat16	gg0, #8, u2bAdd
	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	orr	rr0, gg0, rr0, lsl #8
	str	rr0, [bb1, pDst_]!
	orr	rr1, gg1, rr1, lsl #8
	str	rr1, [bb1, #2*4]

	bcc	asm_Write12CtrRgba8Block8_50

	movs	loopFlags, loopFlags, lsr #1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]

	add	pDrawBlock, pDrawBlock, #0x08

	adrcs	pDstOffsetTbl, dstOffsetTbl12CtrRgba8Block8

	bne	asm_Write12CtrRgba8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef vu2gMul
#undef vu2gAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite22CtrRgba8Block8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22CtrRgba8Block8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define tmp_stride     r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define pDstOffsetTbl  tmp_stride

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_PDST     0x00

	ldr	tmp_pDst,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	tmp_stride, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #5

	mla	pDst_, tmp_stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22CtrRgba8Block8

	add	tmp_stride, pDst_, tmp_stride, lsl #3
	str	tmp_stride, [sp, #STACK_OFFSET_PDST]

	adr	pDstOffsetTbl, dstOffsetTbl22CtrRgba8Block8

	ldr	loopFlags, =0x00210088

asm_Write22CtrRgba8Block8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	w,   [pDrawBlock], #2
	ldrb	bb0, [pDrawBlock, #0x01 - 2]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	orr	w,   w,   w,   lsl #16
	orr	bb0, bb0, bb0, lsl #16

	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15
	mov	bb1, bb1, asr #14
	pkhbt	rr1, bb1, rr1, lsl #16 - 14

	qadd16	rr0, rr1, w
	qadd16	gg0, gg1, w
	qadd16	w,   rr1, bb0
	qadd16	bb0, gg1, bb0

	ldrb	rgbMask, [pDstOffsetTbl], #1

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	w,   #8, w
	usat16	bb0, #8, bb0

	orr	rr0, gg0, rr0, lsl #8
	ldrb	gg0, [pDrawBlock, #0x08 - 2]
	orr	bb1, bb0, w,   lsl #8
	add	rgbMask, pDst_, rgbMask, lsl #2
	ldrb	bb0, [pDrawBlock, #0x09 - 2]
	stmia	rgbMask!, {rr0, bb1}

	orr	gg0, gg0, gg0, lsl #16
	orr	bb0, bb0, bb0, lsl #16

	qadd16	rr0, rr1, gg0
	qadd16	gg0, gg1, gg0
	qadd16	rr1, rr1, bb0
	qadd16	gg1, gg1, bb0

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	orr	rr0, gg0, rr0, lsl #8
	orr	rr1, gg1, rr1, lsl #8
	stmia	rgbMask, {rr0, rr1}

	bcc	asm_Write22CtrRgba8Block8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	ldrcs	pDst_, [sp, #STACK_OFFSET_PDST]
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	adrcs	pDstOffsetTbl, dstOffsetTbl22CtrRgba8Block8

	bne	asm_Write22CtrRgba8Block8_50

	EPILOGUE_INST

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

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef tmp_stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef pDstOffsetTbl

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_PDST
}

       void JpegMpDecoderAsmWrite11Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12

#define gg2            stride
#define gg3            vu2gAdd
#define rr2            vu2gMul
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            rr1

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_STRIDE   0x00

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11Bgr8

	sub	stride, stride, #(8 * 3)
	str	stride, [sp, #STACK_OFFSET_STRIDE]

	mov	loopFlags, #0x80

asm_Write11Bgr8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	adds	loopFlags, loopFlags, #0x40000000

	orr	rr1, gg0, gg1, lsl #16

	ldrh	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2]

	uxtb16	gg0, rr1
	uxtb16	rr1, rr1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, rr1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, rr1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, rr1, v2rMul_u2bMul, u2bAdd

	orr	gg1, gg1, gg1, lsl #8

	mov	gg2, gg2, lsl #16 - 15
	mov	rr2, rr2, lsl #16 - 14
	mov	bb2, bb2, lsl #16 - 14

	uxtb16	gg1, gg1, ror #16

	pkhtb	gg2, gg2, rr3, asr #14
	pkhtb	bb2, bb2, gg3, asr #15
	pkhtb	rr2, rr2, bb3, asr #14

	qadd16	gg2, gg2, gg1
	qadd16	bb2, bb2, gg1
	qadd16	rr2, rr2, gg1

	usat16	gg2, #8, gg2
	usat16	bb2, #8, bb2
	usat16	rr2, #8, rr2

	orr	gg2, bb2, gg2, lsl #8
	strh	gg2, [pDst_, #4]
	orr	rr2, rr2, rr2, lsl #24
	pkhtb	rr2, rr2, gg2, asr #16
	str	rr2, [pDst_], #(3 * 2)

	bcc	asm_Write11Bgr8_50

	ldr	gg0, [sp, #STACK_OFFSET_STRIDE]

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, gg0

	bne	asm_Write11Bgr8_50

	EPILOGUE_INST

constTbl11Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_STRIDE
}

       void JpegMpDecoderAsmWrite21Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #(16 * 3)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21Bgr8

	ldr	loopFlags, =0x00210080

asm_Write21Bgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	rr0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	rr0, gg0, rr0, lsl #16

	pkhbt	rgbMask, rr1, gg1, lsl #16
	pkhbt	gg1,     gg1, bb1, lsl #16
	pkhbt	bb1,     bb1, rr1, lsl #16

	qadd16	rgbMask, rgbMask, rr0
	qadd16	gg1,     gg1,     rr0
	qadd16	bb1,     bb1,     rr0

	usat16	rgbMask, #8, rgbMask
	usat16	gg1,     #8, gg1
	usat16	bb1,     #8, bb1

	orr	gg1, gg1, rgbMask, lsl #8
	strh	gg1, [pDst_, #4]
	orr	bb1, bb1, bb1, lsl #24
	pkhtb	bb1, bb1, gg1, asr #16
	str	bb1, [pDst_], #(3 * 2)

	bcc	asm_Write21Bgr8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21Bgr8_50

	EPILOGUE_INST

constTbl21Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12Bgr8

	mov	loopFlags, #0x80

asm_Write12Bgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock, #0x08 - 1]

	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14
	mov	gg1, gg1, asr #15

	pkhbt	gg0, gg0, gg0, lsl #16
	pkhbt	rr0, rr0, rr0, lsl #16

	pkhbt	rr1, bb1, rr1, lsl #16
	pkhbt	bb1, gg0, rr0
	pkhbt	gg1, gg1, gg1, lsl #16

	qadd16	gg1, gg1, bb1
	qadd16	rr0, rr1, rr0
	qadd16	gg0, rr1, gg0

	usat16	gg1, #8, gg1
	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0

	orr	rr0, rr0, gg1, lsr #8
	strh	rr0, [pDst_, stride]
	orr	gg0, gg0, gg1, lsl #8
	strh	gg0, [pDst_], #2
	mov	rr0, rr0, lsr #16
	strb	rr0, [pDst_, stride]
	mov	gg0, gg0, lsr #16
	strb	gg0, [pDst_], #1

	bcc	asm_Write12Bgr8_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(8 * 3)

	bne	asm_Write12Bgr8_50

	EPILOGUE_INST

constTbl12Bgr8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef vu2gMul
#undef vu2gAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22Bgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Bgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX3)]

	PROLOGUE_INST

	add	arg_x, arg_x, arg_x, lsl #1
	add	tmp_pDst, tmp_pDst, arg_x

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22Bgr8

	ldr	loopFlags, =0x00210088

asm_Write22Bgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	w,   [pDrawBlock, #0x08]
	ldrb	bb0, [pDrawBlock, #0x09]

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	pkhbt	w, bb0, w, lsl #16

	ldrb	bb0, [pDrawBlock], #2
	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	mov	gg1, gg1, asr #15
	mov	rr1, rr1, asr #14
	mov	bb1, bb1, asr #14

	pkhbt	bb0, gg0, bb0, lsl #16

	pkhbt	rgbMask, rr1, gg1, lsl #16
	pkhbt	gg1,     gg1, bb1, lsl #16
	pkhbt	bb1,     bb1, rr1, lsl #16

	qadd16	rr0, rgbMask, w
	qadd16	gg0, gg1,     w
	qadd16	rr1, bb1,     w

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	rr1, #8, rr1

	qadd16	rgbMask, rgbMask, bb0
	qadd16	gg1,     gg1,     bb0
	qadd16	bb1,     bb1,     bb0

	usat16	rgbMask, #8, rgbMask
	usat16	gg1,     #8, gg1
	usat16	bb1,     #8, bb1

	orr	gg0, gg0, rr0, lsl #8
	add	rr0, pDst_, stride
	orr	rr1, rr1, rr1, lsl #24
	strh	gg0, [rr0, #4]
	pkhtb	w, rr1, gg0, asr #16
	str	w, [rr0]

	orr	bb0, gg1, rgbMask, lsl #8
	strh	bb0, [pDst_, #4]
	orr	bb1, bb1, bb1, lsl #24
	pkhtb	bb1, bb1, bb0, asr #16
	str	bb1, [pDst_], #(3 * 2)

	bcc	asm_Write22Bgr8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(16 * 3)

	bne	asm_Write22Bgr8_50

	EPILOGUE_INST

constTbl22Bgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite11Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite11Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4

#define rr1            r5
#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define gg2            stride
#define gg3            vu2gAdd
#define rr2            vu2gMul
#define rr3            v2rAdd
#define bb2            gg0
#define bb3            rr1

#define DCR_STRIDE    (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))
#define DCRDCB_STRIDE (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,drawBlockUnion.draw_block)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}
#define STACK_OFFSET_STRIDE   0x00

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

	add	pDcrDcb,    arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, dcr)

	adr	pConst, constTbl11Abgr8

	sub	stride, stride, #(8 * 4)
	str	stride, [sp, #STACK_OFFSET_STRIDE]

	mov	loopFlags, #0x80

asm_Write11Abgr8_50

	ldrh	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrh	gg0, [pDcrDcb], #2
	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x40000000

	orr	rr1, gg0, gg1, lsl #16

	ldrb	gg1, [pDcrDcb, #DCRDCB_STRIDE - 2]

	uxtb16	gg0, rr1
	uxtb16	rr1, rr1, ror #8

	smlad	gg2, gg0, vu2gMul, vu2gAdd
	smlad	gg3, rr1, vu2gMul, vu2gAdd

	smlatt	rr2, gg0, v2rMul_u2bMul, v2rAdd
	smlatt	rr3, rr1, v2rMul_u2bMul, v2rAdd

	smlabb	bb2, gg0, v2rMul_u2bMul, u2bAdd
	smlabb	bb3, rr1, v2rMul_u2bMul, u2bAdd

	pkhbt	gg2, rgbMask, gg2, lsl #16 - 15
	pkhbt	gg3, rgbMask, gg3, lsl #16 - 15

	ldrb	rgbMask, [pDcrDcb, #DCRDCB_STRIDE - 2 + 1]

	mov	bb2, bb2, asr #14
	mov	bb3, bb3, asr #14
	pkhbt	rr2, bb2, rr2, lsl #16 - 14
	pkhbt	rr3, bb3, rr3, lsl #16 - 14

	orr	gg1, gg1, gg1, lsl #16
	orr	rgbMask, rgbMask, rgbMask, lsl #16
	qadd16	rr2, rr2, gg1
	qadd16	gg2, gg2, gg1
	qadd16	rr3, rr3, rgbMask
	qadd16	gg3, gg3, rgbMask

	usat16	rr2, #8, rr2
	usat16	gg2, #8, gg2
	usat16	rr3, #8, rr3
	usat16	gg3, #8, gg3

	orr	rr1, gg2, rr2, lsl #8
	orr	gg1, gg3, rr3, lsl #8
	stmia	pDst_!, {rr1, gg1}

	bcc	asm_Write11Abgr8_50

	ldr	gg0, [sp, #STACK_OFFSET_STRIDE]

	movs	loopFlags, loopFlags, lsr #1

	addne	pDst_, pDst_, gg0

	bne	asm_Write11Abgr8_50

	EPILOGUE_INST

constTbl11Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb

#undef rr1
#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef gg2
#undef gg3
#undef rr2
#undef rr3
#undef bb2
#undef bb3

#undef DCR_STRIDE
#undef DCRDCB_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
#undef STACK_OFFSET_STRIDE
}

       void JpegMpDecoderAsmWrite21Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite21Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

	sub	stride, stride, #(16 * 4)

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl21Abgr8

	ldr	loopFlags, =0x00210080

asm_Write21Abgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd

	ldrb	rr0, [pDrawBlock], #2

	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	gg0, [pDrawBlock, #0x01 - 2]

	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15
	mov	bb1, bb1, asr #14
	pkhbt	rr1, bb1, rr1, lsl #16 - 14

	orr	rr0, rr0, rr0, lsl #16
	orr	gg0, gg0, gg0, lsl #16

	qadd16	u2bAdd,  rr1, rr0
	qadd16	rgbMask, gg1, rr0
	qadd16	rr1,     rr1, gg0
	qadd16	gg1,     gg1, gg0

	usat16	u2bAdd,  #8, u2bAdd
	usat16	rgbMask, #8, rgbMask
	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	orr	rr0, rgbMask, u2bAdd, lsl #8
	orr	rr1, gg1, rr1, lsl #8
	stmia	pDst_!, {rr0, rr1}

	bcc	asm_Write21Abgr8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride

	bne	asm_Write21Abgr8_50

	EPILOGUE_INST

constTbl21Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite12Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite12Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define v2rMul_u2bMul  r8
#define v2rAdd         r9
#define u2bAdd         r10
#define vu2gMul        r11
#define vu2gAdd        r12
#define rgbMask        lr

#define rr0 vu2gAdd
#define rr1 v2rAdd
#define bb1 v2rMul_u2bMul

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl12Abgr8

	mov	loopFlags, #0x80

asm_Write12Abgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {v2rMul_u2bMul, v2rAdd, u2bAdd, vu2gMul, vu2gAdd, rgbMask}

	adds	loopFlags, loopFlags, #0x20000000

	pkhbt	gg1, gg0, gg1, lsl #16

	ldrb	gg0, [pDrawBlock], #1

	smlatt	rr1, gg1, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg1, v2rMul_u2bMul, u2bAdd
	smlad	gg1, gg1, vu2gMul, vu2gAdd

	ldrb	u2bAdd, [pDrawBlock, #0x08 - 1]

	orr	gg0, gg0, gg0, lsl #16

	mov	bb1, bb1, asr #14
	pkhbt	rr1, bb1, rr1, lsl #16 - 14
	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15

	orr	u2bAdd, u2bAdd, u2bAdd, lsl #16

	qadd16	rr0, rr1, gg0
	qadd16	gg0, gg1, gg0
	qadd16	rr1, rr1, u2bAdd
	qadd16	gg1, gg1, u2bAdd

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	rr1, #8, rr1
	usat16	gg1, #8, gg1

	orr	gg0, gg0, rr0, lsl #8
	orr	gg1, gg1, rr1, lsl #8
	str	gg1, [pDst_, stride]
	str	gg0, [pDst_], #4

	bcc	asm_Write12Abgr8_50

	movs	loopFlags, loopFlags, lsr #1

	add	pDrawBlock, pDrawBlock, #0x08

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(8 * 4)

	bne	asm_Write12Abgr8_50

	EPILOGUE_INST

constTbl12Abgr8
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp(s_DefaultAlpha)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef v2rMul_u2bMul
#undef v2rAdd
#undef vu2gMul
#undef vu2gAdd
#undef u2bAdd
#undef rgbMask

#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

       void JpegMpDecoderAsmWrite22Abgr8(register JpegMpDecoderContext* pCtx, register s32 x, register s32 y);
NN_ASM void JpegMpDecoderAsmWrite22Abgr8(register JpegMpDecoderContext* pCtx NN_IS_UNUSED_VAR, register s32 x NN_IS_UNUSED_VAR, register s32 y NN_IS_UNUSED_VAR)
{
	PRESERVE8

#define arg_pCtx       r0
#define arg_x          r1
#define arg_y          r2

#define tmp_pDst       r12

#define pConst         r0
#define pDst_          r1
#define loopFlags      r2
#define stride         r3
#define pDcrDcb        r4
#define pDrawBlock     r5

#define gg1            r6
#define gg0            r7

#define vu2gMul        r8
#define vu2gAdd        r9
#define v2rMul_u2bMul  r10
#define v2rAdd         r11
#define u2bAdd         r12
#define rgbMask        lr

#define bb0 vu2gMul
#define w   vu2gAdd
#define rr0 v2rMul_u2bMul
#define rr1 v2rAdd
#define bb1 u2bAdd

#define DCR_STRIDE  (ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcb)-ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext,dcr))

#define PROLOGUE_INST         push {arg_pCtx, r4-r11, lr}
#define EPILOGUE_INST         pop  {arg_pCtx, r4-r11, pc}

	ldr	tmp_pDst, [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, pDst)]
	ldr	stride,   [arg_pCtx, #ASM_STRUCTURE_MEMBER_OFFSET_OF(JpegMpDecoderContext, asmWork.outputBufferWidthX4)]

	PROLOGUE_INST

	add	tmp_pDst, tmp_pDst, arg_x, lsl #2

	mla	pDst_, stride, arg_y, tmp_pDst

 	add r4, r0, #0x380
    add r5, r0, #0x400

	adr	pConst, constTbl22Abgr8

	ldr	loopFlags, =0x00210088

asm_Write22Abgr8_50

	ldrb	gg1, [pDcrDcb, #DCR_STRIDE]
	ldrb	gg0, [pDcrDcb], #1

	ldmia	pConst, {vu2gMul, vu2gAdd, v2rMul_u2bMul, v2rAdd, u2bAdd, rgbMask}

	movs	loopFlags, loopFlags, lsl #2

	pkhbt	gg0, gg0, gg1, lsl #16

	addmi	pDrawBlock, pDrawBlock, #0x38

	smlad	gg1, gg0, vu2gMul, vu2gAdd
	smlatt	rr1, gg0, v2rMul_u2bMul, v2rAdd
	smlabb	bb1, gg0, v2rMul_u2bMul, u2bAdd

	ldrb	w,   [pDrawBlock, #0x08]
	ldrb	bb0, [pDrawBlock, #0x09]

	pkhbt	gg1, rgbMask, gg1, lsl #16 - 15
	mov	bb1, bb1, asr #14
	pkhbt	rgbMask, bb1, rr1, lsl #16 - 14

	orr	w,   w,   w,   lsl #16
	orr	bb0, bb0, bb0, lsl #16

	qadd16	rr0, rgbMask, w
	qadd16	gg0, gg1,     w
	qadd16	w,   rgbMask, bb0
	qadd16	bb0, gg1,     bb0

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	w,   #8, w
	usat16	bb0, #8, bb0

	orr	rr0, gg0, rr0, lsl #8
	orr	rr1, bb0, w,   lsl #8
	ldrb	w,   [pDrawBlock], #2
	add	gg0, pDst_, stride
	ldrb	bb0, [pDrawBlock, #0x01 - 2]
	stmia	gg0, {rr0, rr1}

	orr	w,   w,   w,   lsl #16
	orr	bb0, bb0, bb0, lsl #16

	qadd16	rr0, rgbMask, w
	qadd16	gg0, gg1,     w
	qadd16	w,   rgbMask, bb0
	qadd16	bb0, gg1,     bb0

	usat16	rr0, #8, rr0
	usat16	gg0, #8, gg0
	usat16	w,   #8, w
	usat16	bb0, #8, bb0

	orr	rr0, gg0, rr0, lsl #8
	orr	rr1, bb0, w,   lsl #8
	stmia	pDst_!, {rr0, rr1}

	bcc	asm_Write22Abgr8_50

	movs	loopFlags, loopFlags, lsr #16 + 1
	orr	loopFlags, loopFlags, #0x00210000

	sub	pDrawBlock, pDrawBlock, #0x38
	addcs	pDrawBlock, pDrawBlock, #0x40

	add	pDst_, pDst_, stride, lsl #1
	sub	pDst_, pDst_, #(16 * 4)

	bne	asm_Write22Abgr8_50

	EPILOGUE_INST

constTbl22Abgr8
	dcd	__cpp((s_CoefV2gMul << 16) | (s_CoefU2gMul & 0xffff))
	dcd	__cpp(s_CoefVu2gAdd)
	dcd	__cpp((s_CoefV2rMul << 16) | s_CoefU2bMul)
	dcd	__cpp(s_CoefV2rAdd)
	dcd	__cpp(s_CoefU2bAdd)
	dcd	__cpp(s_DefaultAlpha)

#undef arg_pCtx
#undef arg_x
#undef arg_y

#undef tmp_pDst

#undef pConst
#undef pDst_
#undef loopFlags
#undef stride
#undef pDcrDcb
#undef pDrawBlock

#undef gg1
#undef gg0

#undef vu2gMul
#undef vu2gAdd
#undef v2rMul_u2bMul
#undef v2rAdd
#undef u2bAdd
#undef rgbMask

#undef bb0
#undef w
#undef rr0
#undef rr1
#undef bb1

#undef DCR_STRIDE

#undef PROLOGUE_INST
#undef EPILOGUE_INST
}

#undef ASM_STRUCTURE_MEMBER_OFFSET_OF
#undef ASM_OP_LOG_OR

#pragma arm
