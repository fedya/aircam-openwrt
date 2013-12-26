/* Setjmp for Ubicom32 */
	.text
	.global _setjmp
	.type   _setjmp,@function
_setjmp:
	movea	a3, d0		; A3 now holds the jmp buf that was passed in.
	move.4	(a3)4++, d10
	move.4	(a3)4++, d11
	move.4	(a3)4++, d12
	move.4	(a3)4++, d13
	move.4  (a3)4++, a1
	move.4  (a3)4++, a2
	move.4  (a3)4++, a5
	move.4  (a3)4++, a6
	move.4  (a3)4++, a7
	move.4	d0, #0

	calli	a5, 0(a5)
	.size _setjmp, . - _setjmp

/*
 * __sigsetjmp for Ubicom32
 *     d0 holds sigjmp_buf and d1 holds the savemask. We will save the frame and then just call __sigjmp_save to do the mask save.
 */
	.global __sigsetjmp
	.type   __sigsetjmp,@function
__sigsetjmp:
	movea	a3, d0		; A3 now holds the jmp buf that was passed in.
	move.4	(a3)4++, d10
	move.4	(a3)4++, d11
	move.4	(a3)4++, d12
	move.4	(a3)4++, d13
	move.4  (a3)4++, a1
	move.4  (a3)4++, a2
	move.4  (a3)4++, a5
	move.4  (a3)4++, a6
	move.4  (a3)4++, a7

	;; The frame has been saved. Call _sigjmp_save to get sigmask saved. a5 still has the return address and it will go back to that.
#if defined(__UBICOM32_FDPIC__)
	call	a3, __sigjmp_save
#else
	moveai	a3, #%hi(__sigjmp_save)
	calli	a3, %lo(__sigjmp_save)(a3)
#endif
	.size __sigjmp_save, . - __sigjmp_save

	.global	__longjmp
	.type   __longjmp,@function
__longjmp:
	movea	a3, d0		; A3 now holds the jmp buf that was passed in.
	move.4  d10, (a3)4++
	move.4  d11, (a3)4++
	move.4  d12, (a3)4++
	move.4  d13, (a3)4++
	move.4  a1 , (a3)4++
	move.4  a2 , (a3)4++
	move.4  a5 , (a3)4++
	move.4  a6 , (a3)4++
	move.4  a7 , (a3)4++

	sub.4	#0, #0, d1	; Test if d1 is zero. If it is we have to return 1 other wise return content of d1
	jmpeq.t	2f
	move.4	d0, d1		; d1 is non zero load it into d0
	jmpt.t	3f

2:	move.4	d0, #1
3:	calli	a5, 0(a5)
	.size __longjmp, . - __longjmp
	.global __GI___longjmp
	.hidden __GI___longjmp
	.set __GI___longjmp,__longjmp
