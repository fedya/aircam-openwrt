
        .text
        .global test_rjmp
test_rjmp:
        nop
        rjmp    0f
        nop
0:      rjmp    extsym10

        .global test_rcall
test_rcall:
        nop
test_rcall2:
        rcall   0f
        nop
0:      rcall   extsym21

        .global test_branch
test_branch:
        brne    0f
	/* This will generate a reloc since test_branch is global */
        bral    test_branch
0:	breq    extsym21

        .global test_lddpc
test_lddpc:
        lddpc   r0,sym1
        lddpc   r0,sym1
        lddpc   r0,extsym16

        .align	2
sym1:   nop
        nop

	.global	test_local
test_local:
	lddpc	r0, .LC1
	lddpc	r0, .LC2
	lddpc	r0, .LC1 + 0x4

	.align	2
.LC1:
	nop
	nop
.LC2:
	nop
	nop

	.section .text.init,"ax"
	.global test_inter_section
test_inter_section:
	rcall	test_rcall
	nop
	rcall	test_rcall
	rcall	test_rcall2
	nop
	rcall	test_rcall2
