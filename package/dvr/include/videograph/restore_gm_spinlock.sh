find ./ -name "*.c" -type f -exec sed -i '/irqlock_detection/d' {} \;
find ./ -name "*.c" -type f -exec sed -i '/static unsigned int stime,etime;/d' {} \;
find ./ -name "*.c" -type f -exec sed -i 's/gm_spin_lock_it_irqsave/spin_lock_irqsave/' {} \;
find ./ -name "*.c" -type f -exec sed -i 's/gm_spin_unlock_it_irqrestore/spin_unlock_irqrestore/' {} \;
