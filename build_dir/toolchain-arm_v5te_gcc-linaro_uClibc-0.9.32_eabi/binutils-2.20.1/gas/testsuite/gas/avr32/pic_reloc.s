
	.text
	.global	mcall_got
mcall_got:
.L1:
	mcall	r6[extfunc@got]
	mcall	r6[.L1@got]
	mcall	r6[.L2@got]
	mcall	r6[mcall_got@got]
.L2:

	.global	ldw_got
ldw_got:
.L3:	ld.w	r0,r6[extvar@got]
	ld.w	r0,r6[.L3@got]
	ld.w	r0,r6[.L4@got]
	ld.w	r0,r6[ldw_got@got]
.L4:
