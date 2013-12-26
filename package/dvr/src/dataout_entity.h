
#ifndef __DATAOUT_ENTITY_H__
#define __DATAOUT_ENTITY_H__

#define MAX_DATAOUT_ENTITY_NUM   (DVR_RECORD_CHANNEL_NUM*(DVR_ENC_REPD_BT_NUM+1))
#define MAX_DATAOUT_FIFO_LENGTH  20

#define DTO_JOB_AP_GET      1
#define DTO_JOB_CALLBACK    2
#define DTO_JOB_TIMEOUT     3
#include <linux/semaphore.h>
typedef void  (*fnNotifyFnc_t)(int ch_num, int status);

typedef struct dataout_data_tag 
{
    int   id;
    int   is_active;
    struct kfifo *fifo;
    struct video_entity_t entity;
    struct semaphore  oper_sem;
    spinlock_t lock;
    wait_queue_head_t   read_queue;
    int intra_state;        //STATE_AUTO,STATE_FORCE_I_SKIP_P,STATE_FORCE_I_KEEP_P
    int flow_time_ms;
    int new_rate;
    int ap_get_count;
    int need_timeout;
    int cur_session_id;
    int swap_intra;
    int queue_threshold;
	struct delayed_work process_dummy;
}dataout_data_st;

#define INTRA_AUTO          -1
#define INTRA_FORCE_I       1
#define INTRA_ONCE_I        2

#define STATE_AUTO              0
#define STATE_FORCE_I_SKIP_P    1
#define STATE_FORCE_I_KEEP_P    2

extern void idataout_init(fnNotifyFnc_t fnNotifyFnc);
extern void idataout_terminate(void);
extern int idataout_open(int id, wait_queue_head_t **wait_queue);
extern int idataout_close(int id);
extern int idataout_get(int id, struct v_job_t **out_job, int *swap);
extern int idataout_put(int id, struct v_job_t *out_job);
extern int idataout_reset(int id);
extern int idataout_setactive(int id, int is_active);
extern int idataout_getlength(int id);
extern int idataout_reset_intra(int id);
extern int idataout_set_flow_rate(int fd,int rate);
extern int idataout_swap_intra(int id);

#endif /* __DATAOUT_ENTITY_H__ */



