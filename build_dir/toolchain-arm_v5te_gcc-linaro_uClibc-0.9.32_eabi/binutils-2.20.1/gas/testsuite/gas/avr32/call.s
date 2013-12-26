
	.text
	.global call_test
call_test:
far_negative:
	nop
toofar_negative:

	.org	0x200000

	call	far_negative
	call	toofar_negative
	call	different_section
	call	undefined
	call	toofar_positive
	.org	0x200030
	call	far_positive

	.cpool

	.org	0x40002c

toofar_positive:
	nop
far_positive:
	nop

	.section .text.init,"ax",@progbits
different_section:
	sub	r0, r1, 0
