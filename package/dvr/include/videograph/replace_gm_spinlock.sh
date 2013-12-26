grep -rl spin_lock_irqsave *.c|xargs sed -i '1i #include "irqlock_detection.h"'
grep -rl spin_lock_irqsave *.c|xargs sed -i '2i static unsigned int stime,etime;'
find ./ -name "*.c" -type f -exec sed -i 's/spin_lock_irqsave/gm_spin_lock_it_irqsave/' {} \;
find ./ -name "*.c" -type f -exec sed -i 's/spin_unlock_irqrestore/gm_spin_unlock_it_irqrestore/' {} \;
