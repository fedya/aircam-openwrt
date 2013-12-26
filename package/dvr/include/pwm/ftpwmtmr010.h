#ifndef _FTPWMTMR010_H_
#define _FTPWMTMR010_H_

#define PWM_EXTCLK          240000000   // 24Mhz

#define PWM_CLKSRC_PCLK     0
#define PWM_CLKSRC_EXTCLK   1

#define PWM_DEF_CLKSRC      PWM_CLKSRC_PCLK
#define PWM_DEF_FREQ        20000
#define PWM_DEF_DUTY_STEPS  100
#define PWM_DEF_DUTY_RATIO  50
//============================================================================
// PWM API
//============================================================================
bool pwm_dev_request(int id);
void pwm_tmr_start(int id);
void pwm_tmr_stop(int id);
void pwm_dev_release(int id);
void pwm_clksrc_switch(int id, int clksrc);
void pwm_set_freq(int id, u32 freq);
void pwm_set_duty_steps(int id, u32 duty_steps);
void pwm_set_duty_ratio(int id, u32 duty_ratio);
void pwm_set_interrupt_mode(int id);

#endif /*_FTPWMTMR010_H_*/
