/*
 *  linux/arch/arm/kernel/fiq.c
 *
 *  Copyright (C) 1998 Russell King
 *  Copyright (C) 1998, 1999 Phil Blundell
 *
 *  FIQ support written by Philip Blundell <philb@gnu.org>, 1998.
 *
 *  FIQ support re-written by Russell King to be more generic
 *
 * We now properly support a method by which the FIQ handlers can
 * be stacked onto the vector.  We still do not support sharing
 * the FIQ vector itself.
 *
 * Operation is as follows:
 *  1. Owner A claims FIQ:
 *     - default_fiq relinquishes control.
 *  2. Owner A:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  3. Owner B claims FIQ:
 *     - if owner A has a relinquish function.
 *       - disable FIQs.
 *       - saves any registers.
 *       - returns zero.
 *  4. Owner B:
 *     - inserts code.
 *     - sets any registers,
 *     - enables FIQ.
 *  5. Owner B releases FIQ:
 *     - Owner A is asked to reacquire FIQ:
 *	 - inserts code.
 *	 - restores saved registers.
 *	 - enables FIQ.
 *  6. Goto 3
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>

#include <asm/cacheflush.h>
#include <asm/fiq.h>
#include <asm/irq.h>
#include <asm/system.h>

static unsigned long no_fiq_insn;

/* Default reacquire function
 * - we always relinquish FIQ control
 * - we always reacquire FIQ control
 */
static int fiq_def_op(void *ref, int relinquish)
{
	if (!relinquish)
		set_fiq_handler(&no_fiq_insn, sizeof(no_fiq_insn));

	return 0;
}

static struct fiq_handler default_owner = {
	.name	= "default",
	.fiq_op = fiq_def_op,
};

static struct fiq_handler *current_fiq = &default_owner;

int show_fiq_list(struct seq_file *p, void *v)
{
	if (current_fiq != &default_owner)
		seq_printf(p, "FIQ:              %s\n", current_fiq->name);

	return 0;
}

void set_fiq_handler(void *start, unsigned int length)
{
	memcpy((void *)0xffff001c, start, length);
	flush_icache_range(0xffff001c, 0xffff001c + length);
	if (!vectors_high())
		flush_icache_range(0x1c, 0x1c + length);
}

/*
 * Taking an interrupt in FIQ mode is death, so both these functions
 * disable irqs for the duration.  Note - these functions are almost
 * entirely coded in assembly.
 */
void __attribute__((naked)) set_fiq_regs(struct pt_regs *regs)
{
	register unsigned long tmp;
	asm volatile (
	"mov	ip, sp\n\
	stmfd	sp!, {fp, ip, lr, pc}\n\
	sub	fp, ip, #4\n\
	mrs	%0, cpsr\n\
	msr	cpsr_c, %2	@ select FIQ mode\n\
	mov	r0, r0\n\
	ldmia	%1, {r8 - r14}\n\
	msr	cpsr_c, %0	@ return to SVC mode\n\
	mov	r0, r0\n\
	ldmfd	sp, {fp, sp, pc}"
	: "=&r" (tmp)
	: "r" (&regs->ARM_r8), "I" (PSR_I_BIT | PSR_F_BIT | FIQ_MODE));
}

void __attribute__((naked)) get_fiq_regs(struct pt_regs *regs)
{
	register unsigned long tmp;
	asm volatile (
	"mov	ip, sp\n\
	stmfd	sp!, {fp, ip, lr, pc}\n\
	sub	fp, ip, #4\n\
	mrs	%0, cpsr\n\
	msr	cpsr_c, %2	@ select FIQ mode\n\
	mov	r0, r0\n\
	stmia	%1, {r8 - r14}\n\
	msr	cpsr_c, %0	@ return to SVC mode\n\
	mov	r0, r0\n\
	ldmfd	sp, {fp, sp, pc}"
	: "=&r" (tmp)
	: "r" (&regs->ARM_r8), "I" (PSR_I_BIT | PSR_F_BIT | FIQ_MODE));
}

int claim_fiq(struct fiq_handler *f)
{
	int ret = 0;

	if (current_fiq) {
		ret = -EBUSY;

		if (current_fiq->fiq_op != NULL)
			ret = current_fiq->fiq_op(current_fiq->dev_id, 1);
	}

	if (!ret) {
		f->next = current_fiq;
		current_fiq = f;
	}

	return ret;
}

void release_fiq(struct fiq_handler *f)
{
	if (current_fiq != f) {
		printk(KERN_ERR "%s FIQ trying to release %s FIQ\n",
		       f->name, current_fiq->name);
		dump_stack();
		return;
	}

	do
		current_fiq = current_fiq->next;
	while (current_fiq->fiq_op(current_fiq->dev_id, 0));
}

void enable_fiq(int fiq)
{
	enable_irq(fiq + FIQ_START);
}

void disable_fiq(int fiq)
{
	disable_irq(fiq + FIQ_START);
}

#ifdef CONFIG_MFIQ
/* 
 * The following is for multiple FIQ 
 */
#include <mach/ftintc010.h>
#include <asm/io.h> 
#include <mach/mfiq.h>

static DEFINE_SPINLOCK(fiq_database_lock);

typedef irqreturn_t (* asmlinkage C_HANDLER_T)(int, void *, struct pt_regs *);

typedef struct
{
    int         fiq;    
    void        *handler;
    const char  *name;
    void        *dev_id;
    unsigned long flags;
    int         valid;      //1 for valid
}fiq_action_t;

typedef struct 
{   
    /* don't change the following order */ 
    struct pt_regs  fiq_regs[NR_FIQS];
    unsigned int    frame_addr[NR_FIQS];
    int             irq[NR_FIQS];
    int             valid[NR_FIQS]; //0: empty, 1: used. 2: candidate
    char            dev_name[30][NR_FIQS];
    struct pt_regs  dispatch;     
    int             fiq_idx;
    u32             stack_data[20];     //stack to store some registers
    u32             cpsr;
    
    /* following is for c handler */
    struct pt_regs  pt_regs_parm;
    u32             cstack[CSTACK_SZ];    //stack for user c handler
    /* for register fiq c handler */
    fiq_action_t    fiq_action[NR_FIQS];
} fiq_info_t;

static fiq_info_t  *fiq_info;
static unsigned int *fiq_data;
typedef enum
{
    FIQ_DISPATCH_NONE = 0,
    FIQ_DISPATCH_ASSEMBLY,  //Assembly
    FIQ_DISPATCH_C          //C handler
} FIQ_DISPATCH_T;

static FIQ_DISPATCH_T  fiq_init_done = 0;

/* macro for idx range checking
 */ 
#define FIQ_IDX_CHECK(x)    do {if ((x) >= NR_FIQS) return -1;} while(0)

extern unsigned char mfiq_handler_start, mfiq_handler_end;
extern unsigned char mfiq_chandler_start, mfiq_chandler_end;
asmlinkage void asm_do_FIQ(unsigned int irq, struct pt_regs *regs);

/* multiple FIQ init function.
 */
void init_mFIQ(void *function_start, void *function_end)
{    
    u32             mfiqhandler_length;
    unsigned long   flags;
    
    if (fiq_init_done)
        return;
    
    spin_lock_irqsave(&fiq_database_lock, flags);
    
    fiq_init_done = (function_start == (&mfiq_handler_start)) ? FIQ_DISPATCH_ASSEMBLY : FIQ_DISPATCH_C;
    
    fiq_info = (fiq_info_t *)kzalloc(sizeof(fiq_info_t), GFP_ATOMIC);
    if (unlikely(!fiq_info)) 
    {
        printk("%s, error to create fiq info! \n", __FUNCTION__);
        goto exit;
    }
        
    fiq_data = (unsigned int *)kzalloc(FIQ_DATA_LEN * sizeof(unsigned int), GFP_ATOMIC);
    if (unlikely(!fiq_data))
    {
        printk("%s, error to create fiq_data! \n", __FUNCTION__);
        goto exit;
    }

    fiq_data[INTC_VBASE]    = INTC_FTINTC010_VA_BASE;
    fiq_data[FIQ_CNT]       = (u32)&fiq_info->fiq_idx;
    fiq_data[FIQ_HANDLER]   = (u32)&fiq_info->frame_addr[0];    
    fiq_data[FIQ_NUM]       = (u32)&fiq_info->irq[0];
    fiq_data[FIQ_VALID]     = (u32)&fiq_info->valid[0];
    fiq_data[HANDLER_PTREGS]= (u32)&fiq_info->fiq_regs[0];
    fiq_data[PT_REGS]       = (u32)&fiq_info->dispatch;
    fiq_data[STACK_DATA]    = (u32)&fiq_info->stack_data[0];
    fiq_data[CPSR_REG]      = (u32)&fiq_info->cpsr;
    fiq_data[PT_REGS_PARM]  = (u32)&fiq_info->pt_regs_parm;
    fiq_data[CSTACK]        = (u32)&fiq_info->cstack[CSTACK_SZ-1];    //Stack
    fiq_data[ASM_DO_FIQ]    = (u32)asm_do_FIQ;
        
    //get FIQ mode r8-r14 registers
	get_fiq_regs(&fiq_info->dispatch);
	
	/*Note: Only r8, r9, r10, sp, ip are available. Please refer to mfiq.S 
     */
    //store the information in sp register
    fiq_info->dispatch.ARM_sp = (unsigned int)fiq_data;

	set_fiq_regs(&fiq_info->dispatch);
			
	/* copy the body of FIQ dispatch to vector_fiq between __stubs_start and __stubs_end. */
	mfiqhandler_length = (u32)function_end - (u32)function_start;
	if (mfiqhandler_length > 0x1e0) //should not more than 0x1e0 bytes
    {
        printk("The code size of Multiple FIQ dispatcher should not be larger than 0x1e0! (size 0x%x)\n", mfiqhandler_length);
        goto exit;
    }
    
    if (fiq_init_done == FIQ_DISPATCH_ASSEMBLY)    
        printk("Multiple FIQ dispatcher code size = 0x%x bytes \n", mfiqhandler_length);
    else
        printk("Multiple FIQ dispatcher(C-handler) code size = 0x%x bytes \n", mfiqhandler_length);
    
	set_fiq_handler(function_start, mfiqhandler_length);

exit:	
    spin_unlock_irqrestore(&fiq_database_lock, flags);
    
	return;
}

/* return value: fd >= 0 for success */
int claim_mfiq(int irq, struct fiq_handler *f)
{
    int     fiq_idx, candidate_found = 0;
        
    init_mFIQ(&mfiq_handler_start, &mfiq_handler_end);
    
    if (fiq_init_done != FIQ_DISPATCH_ASSEMBLY)
        return -1;
        
    FIQ_IDX_CHECK(fiq_info->fiq_idx);
            
    // find empty slot
    for (fiq_idx = 0; fiq_idx < NR_FIQS; fiq_idx ++)
    {
        if (fiq_info->valid[fiq_idx] == 2) 
        {
            candidate_found = 1;
            break;
        }    
    }
    
    if (!candidate_found) 
    {
        fiq_idx = fiq_info->fiq_idx;
        fiq_info->fiq_idx ++;   //move to new idx 
    }
    
    strcpy(&fiq_info->dev_name[0][fiq_idx], f->name);
    fiq_info->irq[fiq_idx] = irq;
    fiq_info->valid[fiq_idx] = 1;
            	
    return fiq_idx;
}

/* Release a fiq 
 */
int release_mfiq(int fiq_idx, struct fiq_handler *f)
{
    u32     tmp;
        
    if (unlikely(!fiq_init_done))
        return -1;
        
    FIQ_IDX_CHECK(fiq_idx);
    
    if (!fiq_info->valid[fiq_idx])
    {
        printk("This fiq_idx %d is not active! \n", fiq_idx);
        return -1;
    }
    
    if (strcmp(&fiq_info->dev_name[0][fiq_idx], f->name))
    {
        printk("%s, pass a wrong name! \n", f->name);
        return -1;
    }
    
#ifdef CONFIG_FTINTC010EX
    if (fiq_info->irq[fiq_idx] >= 32)
    {
        /* Mask this interrupt, 0 for mask */
        tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);
        tmp &= ~(1 << (fiq_info->irq[fiq_idx] - 32));
        outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);
        
    	/* Clean FIQ */
    	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAREX);
    }
    else
    {
        /* Mask this interrupt, 0 for mask */
        tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
        tmp &= ~(1 << fiq_info->irq[fiq_idx]);
        outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
        
    	/* Clean FIQ */
    	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
    }
#else
    /* Mask this interrupt, 0 for mask */
    tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
    tmp &= ~(1 << fiq_info->irq[fiq_idx]);
    outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
    
	/* Clean FIQ */
	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
#endif    
    fiq_info->valid[fiq_idx] = 2;   //candidate. It can be reused later
    fiq_info->frame_addr[fiq_idx] = 0;
    
    return 0;
}

/* set fiq register */
int set_mfiq_regs(int fiq_idx, struct pt_regs *regs)
{
    if (unlikely(!fiq_init_done))
        return -1;
    
    FIQ_IDX_CHECK(fiq_idx);
    
    if (!fiq_info->valid[fiq_idx])
    {
        printk("This fiq_idx %d is not active! \n", fiq_idx);
        return -1;
    }
    
    memcpy(&fiq_info->fiq_regs[fiq_idx], regs, sizeof(struct pt_regs));            
    return 0;
}

/* get fiq register */
int get_mfiq_regs(int fiq_idx, struct pt_regs *regs)
{
    if (unlikely(!fiq_init_done))
        return -1;
        
    FIQ_IDX_CHECK(fiq_idx);
    
    if (!fiq_info->valid[fiq_idx])
    {
        printk("This fiq_idx %d is not active! \n", fiq_idx);
        return -1;
    }
    memcpy(regs, &fiq_info->fiq_regs[fiq_idx], sizeof(struct pt_regs));
    
    return 0;
}

/*copy function body 
 */
int set_mfiq_handler(int fiq_idx, void *start, unsigned int length)
{
    void *body;
            
    if (fiq_init_done != FIQ_DISPATCH_ASSEMBLY)
        return -1;
    
    FIQ_IDX_CHECK(fiq_idx);
    
    if (unlikely(!fiq_info->valid[fiq_idx]))
    {
        printk("This fiq_idx %d is not active! \n", fiq_idx);
        return -1; 
    }
    
    if (unlikely(length > FIQ_CODE_LEN))
    {
        printk("The code size used by %s is %d bytes which is larger than %d! \n", fiq_info->dev_name[fiq_idx], length, FIQ_CODE_LEN);
        return -1;
    }
                    
    body = (void *)kzalloc(length + 0x100, GFP_ATOMIC); // add margin
    if (unlikely(!body))
    {
        printk("%s, fail to allocate memory! \n", __FUNCTION__);
        return -1;
    }
    
    printk("MFIQ: %s registers FIQ handler with code size = 0x%x bytes. \n", fiq_info->dev_name[fiq_idx], length);
    
    memcpy(body, start, length);
    fiq_info->frame_addr[fiq_idx] = (unsigned int)body;
    
    flush_icache_range((u32)body, (u32)body + length);    //?? Should flush data cache and invalid icache
       
    /* Clean FIQ */
#ifdef CONFIG_FTINTC010EX
    if (fiq_info->irq[fiq_idx] >= 32)
    {
        outl(1 << (fiq_info->irq[fiq_idx]-32), INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAREX);
    	/* Enable FIQ/IRQ handler */
    	outl(1 << (fiq_info->irq[fiq_idx]-32), INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);
    }
	else
	{
	    outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
    	/* Enable FIQ/IRQ handler */
    	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
	}
#else    
	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
	/* Enable FIQ/IRQ handler */
	outl(1 << fiq_info->irq[fiq_idx], INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
#endif    
    return 0;
}

/**
 *	request_mfiq - allocate a FIQ interrupt line
 *	@fiq: Interrupt line to allocate
 *	@handler: Function to be called when the FIQ occurs
 *	@fiqflags: Interrupt type flags
 *	@devname: An ascii name for the claiming device
 *	@dev_id: A cookie passed back to the handler function
 *
 *  If you use FIQ, please be aware that all linux related function calls
 *  are forbidden to use. In FIQ mode, we don't support nested interrupt, so
 *  if you occupy CPU for a long time, whole system will get stuck until you 
 *  free the CPU.
 */
int request_mfiq(unsigned int fiq, irqreturn_t (*handler)(int, void *, struct pt_regs *),
		 unsigned long irq_flags, const char *devname, void *dev_id, int handler_len)
{
    int             i, retval = -ENOMEM;
    fiq_action_t    *action;
    unsigned long   flags;
    
    if (unlikely((handler == NULL) || (!handler_len)))
        return -1;
    
    if (unlikely(handler_len >= FIQ_CODE_LEN))
    {
        printk("FIQ handler size = 0x%x bytes, larger than 0x%x bytes! \n", handler_len, FIQ_CODE_LEN);
        return -1;
    }
    
    init_mFIQ(&mfiq_chandler_start, &mfiq_chandler_end);
    if (fiq_init_done != FIQ_DISPATCH_C)
        return -1;
            
    spin_lock_irqsave(&fiq_database_lock, flags);
            
    for (i = 0; i < NR_FIQS; i ++)
    {                
        if (!fiq_info->fiq_action[i].valid)
        {
            action = &fiq_info->fiq_action[i];
            
            action->fiq = fiq;
            action->handler = kzalloc((handler_len + 0x10), GFP_KERNEL);
            if (action->handler == NULL)
                goto exit;
            
            memcpy((unsigned char *)action->handler, handler, handler_len);
            flush_icache_range((unsigned int)action->handler, (unsigned int)action->handler + handler_len);
            
            action->flags = irq_flags;  //useless
            action->name = devname;
            action->dev_id = dev_id;
            action->valid = 1;
            
            /* Clean FIQ */
#ifdef CONFIG_FTINTC010EX
            if (fiq >= 32)
            {
                outl(1 << (fiq-32), INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAREX);
    	        /* Enable FIQ/IRQ handler */
    	        outl(1 << (fiq-32), INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);	        
    	    }
    	    else
    	    {
    	        outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
    	        /* Enable FIQ/IRQ handler */
    	        outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);	        
    	    }
#else            
	        outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
	        /* Enable FIQ/IRQ handler */
	        outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);	        
#endif	        
            retval = 0;
            break;
        }
    }
    
exit:        
    spin_unlock_irqrestore(&fiq_database_lock, flags);

    return retval;
}

/**
 *	free_mfiq - free a FIQ interrupt
 *	@fiq: Interrupt line to free
 *	@dev_id: Device identity to free 
 *
 *	This function must not be called from interrupt context.
 */
void free_mfiq(unsigned int fiq, void *dev_id)
{
    unsigned long   flags;
    int     i, bFound = 0;
    u32     tmp;
    
    spin_lock_irqsave(&fiq_database_lock, flags);
    for (i = 0; i < NR_FIQS; i ++)
    {
        if (!fiq_info->fiq_action[i].valid)
            continue;
        
        if (fiq_info->fiq_action[i].fiq == fiq)
        {
            memset(&fiq_info->fiq_action[i], 0, sizeof(fiq_action_t));
            bFound = 1;
            
            /* Mask this interrupt, 0 for mask */
#ifdef CONFIG_FTINTC010EX
            if (fiq >= 32)
            {
                tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);
                tmp &= ~(1 << (fiq-32));
                outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASKEX);
            	/* Clean FIQ */
            	outl(1 << (fiq-32), INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAREX);
            }
            else
            {
                tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
                tmp &= ~(1 << fiq);
                outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
            	/* Clean FIQ */
            	outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
            }
#else            
            tmp = inl(INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
            tmp &= ~(1 << fiq);
            outl(tmp, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQMASK);
        	/* Clean FIQ */
        	outl(1 << fiq, INTC_FTINTC010_VA_BASE + FTINTC010_OFFSET_FIQCLEAR);
#endif
            break;
        }
    }
    spin_unlock_irqrestore(&fiq_database_lock, flags);
    
    if (unlikely(!bFound))
        printk("FIQ:%d is not found! \n", fiq);
}

/*
 * asm_do_FIQ handles one of  hardware FIQ's.  This function is called
 * in FIQ mode in which both IRQ and FIQ are disabled.
 * The FIQ source is cleared before calling this function.
 */
asmlinkage void asm_do_FIQ(unsigned int fiq, struct pt_regs *regs)
{    
    int             i;
    fiq_action_t    *action;
    irqreturn_t     ret;
    C_HANDLER_T     c_handler;
    
    for (i = 0; i < NR_FIQS; i ++)
    {
        if (!fiq_info->fiq_action[i].valid)
            continue;
        
        if ((fiq_info->fiq_action[i].fiq == fiq) && fiq_info->fiq_action[i].handler)        
        {
            action = &fiq_info->fiq_action[i];
            c_handler = (asmlinkage C_HANDLER_T)action->handler;
            ret = c_handler(fiq, action->dev_id, regs);
            break;
        }
    }
    
    return;
}


/*For multiple FIQ */
EXPORT_SYMBOL(set_mfiq_handler);
EXPORT_SYMBOL(set_mfiq_regs);
EXPORT_SYMBOL(get_mfiq_regs);
EXPORT_SYMBOL(claim_mfiq);
EXPORT_SYMBOL(release_mfiq);

EXPORT_SYMBOL(request_mfiq);
EXPORT_SYMBOL(free_mfiq);

#endif /* CONFIG_MFIQ */

#ifndef CONFIG_MFIQ
EXPORT_SYMBOL(set_fiq_handler);
EXPORT_SYMBOL(set_fiq_regs);
EXPORT_SYMBOL(get_fiq_regs);
EXPORT_SYMBOL(claim_fiq);
EXPORT_SYMBOL(release_fiq);
#endif /* CONFIG_MFIQ */

EXPORT_SYMBOL(enable_fiq);
EXPORT_SYMBOL(disable_fiq);

void __init init_FIQ(void)
{
	no_fiq_insn = *(unsigned long *)0xffff001c;
}
