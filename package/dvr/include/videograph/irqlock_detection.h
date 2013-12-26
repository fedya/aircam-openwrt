#ifndef __IRQLOCK_DETECTION_H__
#define __IRQLOCK_DETECTION_H__

#include <linux/spinlock.h>
#include <linux/kallsyms.h>

unsigned int video_gettime_ms(void);
#define LOCK_PERIOD 10 //10ms detection

#define gm_spin_lock_it_irqsave(a,b) { \
    stime=(unsigned int)video_gettime_ms(); \
    spin_lock_irqsave(a,b); \
}

#define gm_spin_unlock_it_irqrestore(a,b) { \
    etime=(unsigned int)video_gettime_ms(); \
    spin_unlock_irqrestore(a,b);    \
    if((etime>stime)&&(etime-stime>LOCK_PERIOD)) {  \
        unsigned long pfun; \
        __asm__ ("mov %0,pc\n\t": "=r" (pfun)); \
        print_symbol("SpinLock at:(%s)", pfun); \
        printk("Spends %dms\n",etime-stime);    \
    }   \
}

#endif
