#include <sys/syscall.h>

	.global __syscall_error

/* int _clone(int (*fn)(void *arg), void *child_stack, int flags, void *arg); */
	.text
	.type   clone,@function
	.global clone
clone:
	/* Sanity check arguments.  */
	sub.4	#0, #0, d0	/* Test if fn is zero. */
	jmpeq.f 1f		/* arg bad */
	sub.4	#0, #0, d1	/* Test if child_stack is zero. */
	jmpeq.f	1f		/* arg bad */

	/* Set up child_stack frame.. arg and fn */
	move.4	a3, d1
	move.4	-4(a3)++, d3	/* push 'arg' to child stack */
	move.4	-4(a3)++, d0	/* push fn to child stack, this will become a5
				 * on new child */
	move.4	d1, a3
	move.4	-4(sp)++, a5	/* push a5 to our stack */

	/*
	 * The syscall clone is clone(int flags, void *child_stack) so we set
	 * that up now
	 */
	move.4	d0, d2		/* Move flags to d0 */

	/* Do Clone syscall */
	movei	d8, #__NR_clone
	moveai	a5, #%hi(0x40400000)
	calli	a5, 0x10(a5)
	move.4	a5, (sp)4++	/* pop a5 from stack */

	/* Clone Complete */
	cmpi	d0, #0		/* Test if d0 is less than zero. If it is we
				 * return the error */
	jmplt.f	2f		/* If return is less than we had an error */
	jmpeq.f	3f		/* If return is 0 we are in the clone, jump to
				 * thread start */
	ret	a5		/* d0 is pid */

	/* Invalid Value */
1:	movei   d0, #-22	/* EINVAL */

	/* Call syscall Error */
2:	sub.4	d0, #0, d0	/* d0 = -d0 */
#if defined(__UBICOM32_FDPIC__)
	call	a3, __syscall_error
#else
	moveai	a3, #%hi(__syscall_error)
	calli	a3, %lo(__syscall_error)(a3)
#endif
	/* Child Thread Start */
3:
#if defined(__UBICOM32_FDPIC__)
	/* a5 actually contains a function descriptor for fdpic */
	move.4	a0, 4(a5)	/* set GOT for this function */
	move.4	a5, 0(a5)	/* get address of entry point */
#endif
	move.4	d0, (sp)4++	/* pop 'arg' to stack */
	calli	a5, 0(a5)
	movei	d8, #__NR_exit
	moveai	a3, #%hi(0x40400000)
	calli	a3, 0x10(a3)

	.size clone, . - clone
	.global __GI_clone
	.hidden __GI_clone
	.set __GI_clone,clone
