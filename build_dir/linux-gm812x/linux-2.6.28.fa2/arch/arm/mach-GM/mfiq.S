#include <linux/linkage.h>
#include <mach/spec.h>
#include <mach/ftintc010.h>
#include <mach/entry-macro.S>
#include <asm/asm-offsets.h>
#include <mach/mfiq.h>
    
    /*
     * In FIQ mode, R8-R14 are used only. 
     */
	.global	mfiq_handler_end
	.global	mfiq_chandler_end
			
	.macro clear_mfiq_source, fiq_nr, base, tmp
	    ldr   \base, [sp, #STACK_OFFSET(INTC_VBASE)]   @interruupt controller base
	    cmp   \fiq_nr, #32
	    subge \fiq_nr, \fiq_nr, #32
	    mov   \tmp, #1
	    mov   \tmp, \tmp, lsl \fiq_nr
	    strlt \tmp, [\base, #FTINTC010_OFFSET_FIQCLEAR]
#ifdef CONFIG_FTINTC010EX	    
	    strge \tmp, [\base, #FTINTC010_OFFSET_FIQCLEAREX]
#endif	    
	.endm
	
		
	/* use r7, r8, r9, r10, fp(r11)
	 * Called with fiq_nr, and the return value will be \entry_idx and Z flag
	 */
	.macro find_mfiq_match_table, ret_idx, exp_fiq_nr, entry_idx, valid, fiq_num, tmp1
	    mov   \ret_idx, #0xFF         @init value
	    ldr   \tmp1, [sp, #STACK_OFFSET(FIQ_CNT)]
	    ldr   \tmp1, [\tmp1]
	    cmp   \tmp1, #0
	    beq   label2
        
        mov   \entry_idx, #0                            @for (i = 0; i < fiq_cnt; i ++)
        ldr   \valid, [sp, #STACK_OFFSET(FIQ_VALID)]    @valid
        ldr   \fiq_num, [sp, #STACK_OFFSET(FIQ_NUM)]    @fiq number
label1:
        cmp   \entry_idx, #NR_FIQS
        beq   label2
                
        ldr   \tmp1, [\valid]
        cmp   \tmp1, #1                 @valid?
        addne \entry_idx, \entry_idx, #1
        bne   label1                    @continue
        
        ldr   \tmp1, [\fiq_num]         @get fiq number
        cmp   \tmp1, \exp_fiq_nr
        moveq \ret_idx, \entry_idx
        beq   label2
        
        add   \entry_idx, \entry_idx, #1            @next loop
        add   \valid, \valid, #4
        add   \fiq_num, \fiq_num, #4
        b     label1
label2:
        cmp   \ret_idx, #0xFF
    .endm
        
    /* This macro restore dispatch registers
     */
    .macro restore_dispatch_regs, dispatch_sp, database
        ldr   \database, [\dispatch_sp, #STACK_OFFSET(PT_REGS)]  @dispatch pt_regs
        add   \database, \database, #S_R8       @r8
        ldmia \database, {r8-r14}               @back to dispatch. Note: ldmia r11, {r8-r13} is an error due to r11 covered
    .endm
    
	.text
		
ENTRY(mfiq_handler_start)
    /*
     * We save the registers and restore them before exit. 
     * r0: a backup of sp of dispatcher
     * r1: fiq source
     * r2: entry index to fiq table
     * r3: pt_regs of fiq handler
     * r4: size of pt_regs structure
     * r5: fiq handler address
     * r13: dispatcher sp
     */
loop1:     
    ldr   fp, [sp, #STACK_OFFSET(STACK_DATA)]
    @b  loop1
    stmia fp, {r0-lr}                   @save registers first.
    mov   r0, sp                        @backup sp register, because it will be overwrited later

fiq_loop:
    get_irqnr_preamble r9, lr
    get_fiqnr_and_base r1, r8, r9, r10  @r1: fiq_nr
    beq   loop_end                      @no interrupt pending    
    find_mfiq_match_table   r2, r1, r8, r9, r10, ip       @r2 is the return entry index and Zflag
    bne   find_mfiq_src
    clear_mfiq_source       r1, r8, r9  @clear the interrupt source
    b     loop_end

find_mfiq_src:    
    /* Load r8-r13 of fiq handler to r8-r13 
     */
    ldr   r3, [sp, #STACK_OFFSET(HANDLER_PTREGS)]
    mov   r4, #S_FRAME_SIZE                         @pt_regs size
    ldr   r5, [sp, #STACK_OFFSET(FIQ_HANDLER)]      @function address
    mul   r6, r2, r4                @entry_idx * sizeof(pt_regs)
    add   r6, r6, #S_R8             @r8 offset in pt_regs
    add   r7, r3, r6                @r7 points to r8 of pt_regs
    ldmia r7, {r8-r13}              @copy the pt_regs of client and ready to pass to the callee
    
	/* 
	 * Calculate fiq handler address
	 */
	mov   r6, r2, lsl #2            @jump address = function table base + entry_idx*4
	add   r5, r5, r6
	ldr   r5, [r5]
	cmp   r5, #0
	
	/*is going to callee
	 */
	ldr   r6, [sp, #STACK_OFFSET(CPSR_REG)]
	mrs   r6, cpsr                  @just prevent someone from changing mode
    movne lr, pc    
    movne pc, r5                    @jump to callee
    msr   cpsr_c, r6
    restore_dispatch_regs  r0, r6   @restore dispatch registers
    clear_mfiq_source      r1, r6, r7  @clear the interrupt source
    b     fiq_loop
    
loop_end:
    mov   sp, r0                    @resotre sp
    ldr   fp, [sp, #STACK_OFFSET(STACK_DATA)]
    ldmia fp, {r0-lr}               @restore r0-r14
    
	subs  pc, lr, #4                @pc is the target, change mode to SVC or ...
	.ltorg
mfiq_handler_end:



ENTRY(mfiq_chandler_start)
loop:
    ldr   fp, [sp, #STACK_OFFSET(STACK_DATA)]    
    @b loop
    stmia fp, {r0-r14}                  @backcup register first
    
    /* ATPCS defines that r0 - r3 is the parameters and return value 
     * r0 = irq number, r1 = struct pt_regs *     
     */
    ldr   r4, [sp, #STACK_OFFSET(PT_REGS_PARM)]
    stmia r4, {r0-r14}                  @prepare pt_regs to r1
    
parse_fiq:
    get_irqnr_preamble r9, lr
1:  get_fiqnr_and_base r0, r8, r9, lr
    beq   cloop_end                     @no interrupt pending
    mov   r5, r0                        @backup the interupt source
    mov   r1, r4                        @pass pt_regs to c handler
    ldr   r6, [sp, #STACK_OFFSET(ASM_DO_FIQ)]   @asm_do_FIQ    
    mov   r10, sp                       @backup sp    
    ldr   sp, [sp, #STACK_OFFSET(CSTACK)]   @stack for c-handler
    mov   lr, pc
    mov   pc, r6                        @jump to asm_do_FIQ        
    mov   sp, r10
        
    clear_mfiq_source r5, r8, r9        @clear the interrupt source    
    b     parse_fiq
      
cloop_end:
    ldr   fp, [sp, #STACK_OFFSET(STACK_DATA)]
    ldmia fp, {r0-r14}
    
    subs  pc, lr, #4                    @return to original mode
	.ltorg
mfiq_chandler_end:
