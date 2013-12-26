	.text
	.global _start
_start:
	nop
	nop

	.section .text.pcrel10,"ax"
	.global extsym10
extsym10:
	nop

	.section .text.pcrel16,"ax"
	.global extsym16
extsym16:
	nop

	.section .text.pcrel21,"ax"
	.global extsym21
extsym21:
	nop
