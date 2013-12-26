#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

#if __LINUX_ARM_ARCH__ < 6 && !defined(CONFIG_CPU_FMP626)
#error SMP not supported on pre-ARMv6 CPUs
#endif

/*
 * ARMv6 Spin-locking.
 *
 * We exclusively read the old value.  If it is zero, we may have
 * won the lock, so we try exclusively storing it.  A memory barrier
 * is required after we get a lock, and before we release it, because
 * V6 CPUs are assumed to have weakly ordered memory.
 *
 * Unlocked value: 0
 * Locked value: 1
 */

#define __raw_spin_is_locked(x)		((x)->lock != 0)
#define __raw_spin_unlock_wait(lock) \
	do { while (__raw_spin_is_locked(lock)) cpu_relax(); } while (0)

#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)

#ifdef CONFIG_CPU_FMP626
static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%1], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	teq	%0, #0\n"
"	mcrne	p13, 0, %0, c0, c0, 0	@ wait for event\n"
"	mcreq	p13, 0, %2, c1, c0, 0	@ data for strex\n"
"	stceq	p13, c2, [%1], {4}	@ strex to address\n"
"	mrceq	p13, 0, pc, c3, c0, 0	@ strex status\n"
"	bne	1b"
	: "=&r" (tmp)
	: "r" (&lock->lock), "r" (1)
	: "cc");

	smp_mb();
}
#else
static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldrex	%0, [%1]\n"
"	teq	%0, #0\n"
#ifdef CONFIG_CPU_32v6K
"	wfene\n"
#endif
"	strexeq	%0, %2, [%1]\n"
"	teqeq	%0, #0\n"
"	bne	1b"
	: "=&r" (tmp)
	: "r" (&lock->lock), "r" (1)
	: "cc");

	smp_mb();
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	unsigned long tmp;

	__asm__ __volatile__(
"	ldc	p13, c1, [%1], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	teq	%0, #0\n"
"	mcreq	p13, 0, %2, c1, c0, 0	@ data for strex\n"
"	stceq	p13, c2, [%1], {4}	@ strex to address\n"
"	mrceq	p13, 0, %0, c3, c0, 0	@ strex status\n"
	: "=&r" (tmp)
	: "r" (&lock->lock), "r" (1)
	: "cc");

	if (tmp == 0) {
		smp_mb();
		return 1;
	} else {
		return 0;
	}
}
#else
static inline int __raw_spin_trylock(raw_spinlock_t *lock)
{
	unsigned long tmp;

	__asm__ __volatile__(
"	ldrex	%0, [%1]\n"
"	teq	%0, #0\n"
"	strexeq	%0, %2, [%1]"
	: "=&r" (tmp)
	: "r" (&lock->lock), "r" (1)
	: "cc");

	if (tmp == 0) {
		smp_mb();
		return 1;
	} else {
		return 0;
	}
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	smp_mb();

	__asm__ __volatile__(
"	str	%1, [%0]\n"
"	mcr	p15, 0, %1, c7, c10, 4\n" /* DSB */
"	mcr	p13, 0, %1, c0, c0, 1	@ set event\n"
	:
	: "r" (&lock->lock), "r" (0)
	: "cc");
}
#else
static inline void __raw_spin_unlock(raw_spinlock_t *lock)
{
	smp_mb();

	__asm__ __volatile__(
"	str	%1, [%0]\n"
#ifdef CONFIG_CPU_32v6K
"	mcr	p15, 0, %1, c7, c10, 4\n" /* DSB */
"	sev"
#endif
	:
	: "r" (&lock->lock), "r" (0)
	: "cc");
}
#endif

/*
 * RWLOCKS
 *
 *
 * Write locks are easy - we just set bit 31.  When unlocking, we can
 * just write zero since the lock is exclusively held.
 */

#ifdef CONFIG_CPU_FMP626
static inline void __raw_write_lock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%1], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	teq	%0, #0\n"
"	mcrne	p13, 0, %0, c0, c0, 0	@ wait for event\n"
"	mcreq	p13, 0, %2, c1, c0, 0	@ data for strex\n"
"	stceq	p13, c2, [%1], {4}	@ strex to address\n"
"	mrceq	p13, 0, pc, c3, c0, 0	@ strex status\n"
"	bne	1b"
	: "=&r" (tmp)
	: "r" (&rw->lock), "r" (0x80000000)
	: "cc");

	smp_mb();
}
#else
static inline void __raw_write_lock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldrex	%0, [%1]\n"
"	teq	%0, #0\n"
#ifdef CONFIG_CPU_32v6K
"	wfene\n"
#endif
"	strexeq	%0, %2, [%1]\n"
"	teq	%0, #0\n"
"	bne	1b"
	: "=&r" (tmp)
	: "r" (&rw->lock), "r" (0x80000000)
	: "cc");

	smp_mb();
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%1], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	teq	%0, #0\n"
"	mcreq	p13, 0, %2, c1, c0, 0	@ data for strex\n"
"	stceq	p13, c2, [%1], {4}	@ strex to address\n"
"	mrceq	p13, 0, %0, c3, c0, 0	@ strex status\n"
	: "=&r" (tmp)
	: "r" (&rw->lock), "r" (0x80000000)
	: "cc");

	if (tmp == 0) {
		smp_mb();
		return 1;
	} else {
		return 0;
	}
}
#else
static inline int __raw_write_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	__asm__ __volatile__(
"1:	ldrex	%0, [%1]\n"
"	teq	%0, #0\n"
"	strexeq	%0, %2, [%1]"
	: "=&r" (tmp)
	: "r" (&rw->lock), "r" (0x80000000)
	: "cc");

	if (tmp == 0) {
		smp_mb();
		return 1;
	} else {
		return 0;
	}
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline void __raw_write_unlock(raw_rwlock_t *rw)
{
	smp_mb();

	__asm__ __volatile__(
	"str	%1, [%0]\n"
"	mcr	p15, 0, %1, c7, c10, 4\n" /* DSB */
"	mcr	p13, 0, %1, c0, c0, 1	@ set event\n"
	:
	: "r" (&rw->lock), "r" (0)
	: "cc");
}
#else
static inline void __raw_write_unlock(raw_rwlock_t *rw)
{
	smp_mb();

	__asm__ __volatile__(
	"str	%1, [%0]\n"
#ifdef CONFIG_CPU_32v6K
"	mcr	p15, 0, %1, c7, c10, 4\n" /* DSB */
"	sev\n"
#endif
	:
	: "r" (&rw->lock), "r" (0)
	: "cc");
}
#endif

/* write_can_lock - would write_trylock() succeed? */
#define __raw_write_can_lock(x)		((x)->lock == 0)

/*
 * Read locks are a bit more hairy:
 *  - Exclusively load the lock value.
 *  - Increment it.
 *  - Store new lock value if positive, and we still own this location.
 *    If the value is negative, we've already failed.
 *  - If we failed to store the value, we want a negative result.
 *  - If we failed, try again.
 * Unlocking is similarly hairy.  We may have multiple read locks
 * currently active.  However, we know we won't have any write
 * locks.
 */
#ifdef CONFIG_CPU_FMP626
static inline void __raw_read_lock(raw_rwlock_t *rw)
{
	unsigned long tmp, tmp2;

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%2], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	adds	%0, %0, #1\n"
"	mcrpl	p13, 0, %0, c1, c0, 0	@ data for strex\n"
"	stcpl	p13, c2, [%2], {4}	@ strex to address\n"
"	mrcpl	p13, 0, %1, c3, c0, 0	@ strex status\n"
"	mcrmi	p13, 0, %0, c0, c0, 0	@ wait for event\n"
"	rsbpls	%0, %1, #0\n"
"	bmi	1b"
	: "=&r" (tmp), "=&r" (tmp2)
	: "r" (&rw->lock)
	: "cc");

	smp_mb();
}
#else
static inline void __raw_read_lock(raw_rwlock_t *rw)
{
	unsigned long tmp, tmp2;

	__asm__ __volatile__(
"1:	ldrex	%0, [%2]\n"
"	adds	%0, %0, #1\n"
"	strexpl	%1, %0, [%2]\n"
#ifdef CONFIG_CPU_32v6K
"	wfemi\n"
#endif
"	rsbpls	%0, %1, #0\n"
"	bmi	1b"
	: "=&r" (tmp), "=&r" (tmp2)
	: "r" (&rw->lock)
	: "cc");

	smp_mb();
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	unsigned long tmp;

	smp_mb();

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%1], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	sub	%0, %0, #1\n"
"	mcr	p13, 0, %0, c1, c0, 0	@ data for strex\n"
"	stc	p13, c2, [%1], {4}	@ strex to address\n"
"	mrc	p13, 0, pc, c3, c0, 0	@ strex status\n"
"	bne	1b\n"
"	cmp	%0, #0\n"
"	mcreq   p15, 0, %0, c7, c10, 4\n"
"	mcreq	p13, 0, %0, c0, c0, 1	@ set event\n"
	: "=&r" (tmp)
	: "r" (&rw->lock)
	: "cc");
}
#else
static inline void __raw_read_unlock(raw_rwlock_t *rw)
{
	unsigned long tmp, tmp2;

	smp_mb();

	__asm__ __volatile__(
"1:	ldrex	%0, [%2]\n"
"	sub	%0, %0, #1\n"
"	strex	%1, %0, [%2]\n"
"	teq	%1, #0\n"
"	bne	1b"
#ifdef CONFIG_CPU_32v6K
"\n	cmp	%0, #0\n"
"	mcreq   p15, 0, %0, c7, c10, 4\n"
"	seveq"
#endif
	: "=&r" (tmp), "=&r" (tmp2)
	: "r" (&rw->lock)
	: "cc");
}
#endif

#ifdef CONFIG_CPU_FMP626
static inline int __raw_read_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp, tmp2 = 1;

	__asm__ __volatile__(
"1:	ldc	p13, c1, [%2], {4}	@ set address for ldrex\n"
"	mrc	p13, 0, %0, c1, c0, 0	@ ldrex\n"
"	adds	%0, %0, #1\n"
"	mcrpl	p13, 0, %0, c1, c0, 0	@ data for strex\n"
"	stcpl	p13, c2, [%2], {4}	@ strex to address\n"
"	mrcpl	p13, 0, %1, c3, c0, 0	@ strex status\n"
	: "=&r" (tmp), "+r" (tmp2)
	: "r" (&rw->lock)
	: "cc");

	smp_mb();
	return tmp2 == 0;
}
#else
static inline int __raw_read_trylock(raw_rwlock_t *rw)
{
	unsigned long tmp, tmp2 = 1;

	__asm__ __volatile__(
"1:	ldrex	%0, [%2]\n"
"	adds	%0, %0, #1\n"
"	strexpl	%1, %0, [%2]\n"
	: "=&r" (tmp), "+r" (tmp2)
	: "r" (&rw->lock)
	: "cc");

	smp_mb();
	return tmp2 == 0;
}
#endif

/* read_can_lock - would read_trylock() succeed? */
#define __raw_read_can_lock(x)		((x)->lock < 0x80000000)

#define _raw_spin_relax(lock)	cpu_relax()
#define _raw_read_relax(lock)	cpu_relax()
#define _raw_write_relax(lock)	cpu_relax()

#endif /* __ASM_SPINLOCK_H */
