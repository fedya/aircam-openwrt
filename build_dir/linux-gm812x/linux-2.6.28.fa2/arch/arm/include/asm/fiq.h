/*
 *  arch/arm/include/asm/fiq.h
 *
 * Support for FIQ on ARM architectures.
 * Written by Philip Blundell <philb@gnu.org>, 1998
 * Re-written by Russell King
 */

#ifndef __ASM_FIQ_H
#define __ASM_FIQ_H

#include <asm/ptrace.h>

struct fiq_handler {
	struct fiq_handler *next;
	/* Name
	 */
	const char *name;
	/* Called to ask driver to relinquish/
	 * reacquire FIQ
	 * return zero to accept, or -<errno>
	 */
	int (*fiq_op)(void *, int relinquish);
	/* data for the relinquish/reacquire functions
	 */
	void *dev_id;
};

extern int claim_fiq(struct fiq_handler *f);
extern void release_fiq(struct fiq_handler *f);
extern void set_fiq_handler(void *start, unsigned int length);
extern void set_fiq_regs(struct pt_regs *regs);
extern void get_fiq_regs(struct pt_regs *regs);
extern void enable_fiq(int fiq);
extern void disable_fiq(int fiq);
 
/*
 * The following definitions are for multiple FIQ
 */
#ifdef CONFIG_MFIQ
extern int claim_mfiq(int irq, struct fiq_handler *f);
extern int release_mfiq(int fiq_idx, struct fiq_handler *f);
extern int set_mfiq_handler(int fiq_idx, void *start, unsigned int length);
extern int set_mfiq_regs(int fiq_idx, struct pt_regs *regs);
extern int get_mfiq_regs(int fiq_idx, struct pt_regs *regs);

/* for register c-handler */
extern int request_mfiq(unsigned int fiq, irqreturn_t (*handler)(int, void *, struct pt_regs *),
		 unsigned long irq_flags, const char *devname, void *dev_id, int handler_len);
extern void free_mfiq(unsigned int fiq, void *dev_id);
#endif /* CONFIG_MFIQ */

#endif
