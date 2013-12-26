#ifndef __MFIQ_H__
#define __MFIQ_H__

/*
 * The following is for multiple FIQ
 */
#define NR_FIQS         2 

#define INTC_VBASE      0   //intc virtual base
#define FIQ_CNT         1   //how many fiq users
#define FIQ_HANDLER     2   //function address 
#define FIQ_NUM         3   //IRQ NO
#define FIQ_VALID       4
#define HANDLER_PTREGS  5   //handler pt_regs
#define PT_REGS         6   //dispatcher pt_regs
#define STACK_DATA      7
#define CPSR_REG        8
#define PT_REGS_PARM    9
#define CSTACK          10
#define ASM_DO_FIQ      11
#define FIQ_DATA_LEN    12

#define S_R8      	    32
#define STACK_OFFSET(x) (x << 2)
#define FIQ_CODE_LEN    0x200   //configurable, but less is better
#define CSTACK_SZ       (0x4000/4)   //stack size, 4k

#endif /* __MFIQ_H__ */ 
