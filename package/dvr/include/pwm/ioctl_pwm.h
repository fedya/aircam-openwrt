#ifndef _FTPWMTMR010_API_H_
#define _FTPWMTMR010_API_H_

#include <linux/ioctl.h>

//============================================================================
// data struct
//============================================================================

typedef struct pwm_info {
	unsigned int id;
	unsigned int clksrc;
	unsigned int freq;
	unsigned int duty_steps;
	unsigned int duty_ratio;
	unsigned int cnt;
} pwm_info_t;

//============================================================================
// I/O control ID
//============================================================================
/* Use 'p' as magic number */
#define PWM_IOC_MAGIC			'p'
#define PWM_IOCTL_REQUEST		_IOW(PWM_IOC_MAGIC, 1, int)
#define PWM_IOCTL_START			_IOW(PWM_IOC_MAGIC, 2, int)
#define PWM_IOCTL_STOP			_IOW(PWM_IOC_MAGIC, 3, int)
#define PWM_IOCTL_GET_INFO		_IOWR(PWM_IOC_MAGIC, 4, pwm_info_t)
#define PWM_IOCTL_SET_CLKSRC		_IOW(PWM_IOC_MAGIC, 5, pwm_info_t)
#define PWM_IOCTL_SET_FREQ		_IOW(PWM_IOC_MAGIC, 6, pwm_info_t)
#define PWM_IOCTL_SET_DUTY_STEPS	_IOW(PWM_IOC_MAGIC, 7, pwm_info_t)
#define PWM_IOCTL_SET_DUTY_RATIO	_IOW(PWM_IOC_MAGIC, 8, pwm_info_t)
#define PWM_IOCTL_SET_INTR_MODE		_IOW(PWM_IOC_MAGIC, 9, pwm_info_t)
//temp
#define PWM_IOCTL_VCM_ENABLE		_IOW(PWM_IOC_MAGIC, 20, int)
#define PWM_IOCTL_VCM_DISABLE		_IOW(PWM_IOC_MAGIC, 21, int)
#define PWM_IOC_MAXNR			21

#endif /*_FTPWMTMR010_API_H_*/
