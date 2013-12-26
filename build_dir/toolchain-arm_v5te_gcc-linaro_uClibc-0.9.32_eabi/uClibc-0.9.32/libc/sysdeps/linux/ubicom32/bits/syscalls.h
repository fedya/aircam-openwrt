#ifndef _BITS_SYSCALLS_H
#define _BITS_SYSCALLS_H
#ifndef _SYSCALL_H
# error "Never use <bits/syscalls.h> directly; include <sys/syscall.h> instead."
#endif

/* m68k headers does stupid stuff with __NR_iopl / __NR_vm86:
 * #define __NR_iopl   not supported
 * #define __NR_vm86   not supported
 */
#undef __NR_iopl
#undef __NR_vm86

#ifndef __ASSEMBLER__

#include <errno.h>

/* Linux takes system call arguments in registers:

	syscall number	%d8
	arg 1		%d0
	arg 2		%d1
	arg 3		%d2
	arg 4		%d3
	arg 5		%d4
	arg 6		%d5
*/

#define __loadargs_0(name, dummy)					      \
	d8 = (long int)name

#define __loadargs_1(name, __arg1)					\
	__loadargs_0(name, 0);						\
	d0_retval = (long int) __arg1

#define __loadargs_2(name, __arg1, __arg2)    \
	__loadargs_1(name, __arg1);			   \
	d1 = (long int) __arg2

#define __loadargs_3(name, __arg1, __arg2, __arg3) \
	__loadargs_2(name, __arg1, __arg2); \
	d2 = (long int) __arg3

#define __loadargs_4(name, __arg1, __arg2, __arg3, __arg4)	\
	__loadargs_3(name, __arg1, __arg2, __arg3); \
	d3 = (long int)__arg4

#define __loadargs_5(name, __arg1, __arg2, __arg3, __arg4, __arg5) \
	__loadargs_4(name, __arg1, __arg2, __arg3, __arg4); \
	d4 = (long int)__arg5

#define __loadargs_6(name, __arg1, __arg2, __arg3, __arg4, __arg5, __arg6) \
	__loadargs_5(name, __arg1, __arg2, __arg3, __arg4, __arg5); \
	d5 = (long int)__arg6

#define INTERNAL_SYSCALL_NCS(name, err, nr, args...)			\
({									\
	register long int d0_retval  __asm__ ("d0");			\
	register long int d1  __asm__ ("d1");				\
	register long int d2  __asm__ ("d2");				\
	register long int d3  __asm__ ("d3");				\
	register long int d4  __asm__ ("d4");				\
	register long int d5  __asm__ ("d5");				\
	register long int d8  __asm__ ("d8");				\
	__loadargs_##nr (name, args);					\
	__asm__ __volatile__ (						\
	"	moveai a5, #%%hi(0x40400000)\n\t"			\
	"	calli a5, 16(a5)\n\t"					\
		: "+r" (d0_retval), "+r" (d1), "+r" (d2), "+r" (d3),	\
		  "+r" (d4), "+r" (d5),  "+r" (d8) :			\
		: "cc", "memory",					\
		  "acc0_lo", "acc0_hi", "acc1_lo", "acc1_hi",		\
		  "source3",						\
		  "a0", "a3", "a4", "a5",				\
		  "d6", "d7", "d9", "d14", "d15"			\
	);								\
	d0_retval;							\
})

#endif /* __ASSEMBLER__ */
#endif /* _BITS_SYSCALLS_H */
