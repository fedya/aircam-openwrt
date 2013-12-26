
	.text
	.global	diff32
diff32:
	.long	.L2 - .L1
	.global	diff16
diff16:
	.short	.L2 - .L1
	.global	diff8
diff8:
	.byte	.L2 - .L1

	.global	symdiff_test
	.align	1
symdiff_test:
	nop
.L1:	nop
	nop
.L2:	nop
