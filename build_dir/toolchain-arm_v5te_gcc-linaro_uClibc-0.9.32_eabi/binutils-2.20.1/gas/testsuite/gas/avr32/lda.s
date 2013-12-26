
	.text
	.global lda_test
lda_test:
toofar_negative:
	sub	r8, r9, 0
far_negative:
	sub	r10, r11, 0

	.fill	32760, 1, 0x00

	lda.w	r0, far_negative
	lda.w	r1, toofar_negative
	lda.w	r2, different_section
	lda.w	r3, undefined
	lda.w	r4, toofar_positive
	lda.w	r5, far_positive

	.cpool

	.fill	32744, 1, 0x00
far_positive:
	sub	r12, sp, 0
	.fill	16, 1, 0x00
toofar_positive:
	sub	lr, pc, 0

	.data
different_section:
	.long	0x12345678
