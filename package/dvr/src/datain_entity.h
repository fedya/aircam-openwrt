
#ifndef __DATAIN_ENTITY_H__
#define __DATAIN_ENTITY_H__


#define MAX_DATAIN_ENTITY_NUM   DVR_PLAYBACK_CHANNEL_NUM
#define MAX_DATAIN_FIFO_LENGTH  20

#define DTI_JOB_AP_GET      1
#define DTI_JOB_CALLBACK    2
#define DTI_JOB_TIMEOUT     3
typedef void  (*fnNotifyFnc_t)(int ch_num, int status);

typedef struct datain_data_tag 
{
    int   id;
    int   is_active;
    struct kfifo *fifo;
    struct video_entity_t entity;
    struct semaphore  oper_sem;
    spinlock_t lock;
    wait_queue_head_t   read_queue;
    int new_rate;
    int flow_time_ms;
    int ap_get_count;
    unsigned int last_ap_put_timestamp;
    int need_timeout;
//++ fullhd
	struct delayed_work process_dummy;
//-- fullhd    
}datain_data_st;

extern void idatain_init(fnNotifyFnc_t fnNotifyFnc);
extern void idatain_terminate(void);
extern int idatain_open(int id, wait_queue_head_t **wait_queue);
extern int idatain_close(int id);
extern int idatain_get(int id, struct v_job_t **out_job);
extern int idatain_put(int id, struct v_job_t *out_job, int status);
extern int idatain_reset(int id);
extern int idatain_setactive(int id, int is_active);
extern int idatain_getlength(int id);
extern int idatain_set_flow_rate(int fd,int rate);

#endif /* __DATAIN_ENTITY_H__ */



