
	.text
	.global	jmptable_test
jmptable_test:
	sub	r8, pc, -(.L1 - .)
	add	pc, r8, r0 << 2
	nop
	.align	2
.L1:	rjmp	1f
	rjmp	2f
	rjmp	3f
1:	nop
2:	nop
3:	nop
