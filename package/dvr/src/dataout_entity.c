#define __DATAOUT_ENTITY_C__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

#include "videograph_api.h"
#include "dvr_type_define.h"
#include "dataout_entity.h"
#include "dvr_mod_define.h"

//NOTE: dataout uses [dvr_enc]'s dbgmode
extern int dvr_enc_dbgmode;
#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_enc_dbgmode) printk(fmt, ## args);}

//#define DEBUG_USER_AP

#define TIMEOUT_VALUE ((TIMEOUT_FACTOR*2)/3)

struct callback_item
{
    char            allocated;
    unsigned int    ap_get_timestamp;
    unsigned int    putjob_timestamp;
    struct list_head    callback_list;
    void            *job;
    int             session_id;
};

int isKeyframe(struct v_job_t *job);
int set_enc_intra_from_dataout(int dataout_fd,int intra);
extern struct list_head dataout_callback_head[MAX_DATAOUT_ENTITY_NUM];
extern struct workqueue_struct *dvrenc_wq;

static int callback_handler(struct work_struct *work);

static void timeout_handler(void *);
static DECLARE_DELAYED_WORK(process_timeout, (void *)timeout_handler);

static struct semaphore handler_sem;

#define MAX_CALLBACK_POOL (MAX_DATAOUT_ENTITY_NUM*15)   //As 30fps, be 1/2 sec pool
static int search_start=0;
static spinlock_t pool_lock=SPIN_LOCK_UNLOCKED;
static struct callback_item *callback_pool;
static dataout_data_st dataout_private[MAX_DATAOUT_ENTITY_NUM];
static int is_dataout_init=0;
static int ses_id[MAX_DATAOUT_ENTITY_NUM];
static fnNotifyFnc_t fnDtoNotifyFnc=NULL;

#define STATUS_VG_PUT   1
#define STATUS_AP_GET   2
#define STATUS_NEED_CB  3
#define STATUS_CB_DONE  4

/*
Timeout value=(Min:100ms ~  Buffer num X Flow Rate X 2  ~  Max:MIN_FLOW_TIME_MS)
MIN_FLOW_TIME_MS=100ms: if no this,flow_time_ms may be very small for AP to handle (may be <10ms timeout)
 */
#define MIN_SCHEDULE_TICK   ((1000/HZ)*30) //30ms
#define MIN_FLOW_TIME_MS    100
#define MAX_FLOW_TIME_MS    300

static void free_pool(struct callback_item *item)
{
    unsigned long flags;
    spin_lock_irqsave(&pool_lock,flags);
    item->allocated=0;
    spin_unlock_irqrestore(&pool_lock,flags);
}

static struct callback_item *alloc_pool(int ch)
{
    struct callback_item *item=0;
    unsigned long flags;
    int search_from=search_start;
    
    spin_lock_irqsave(&pool_lock,flags);

    while(callback_pool[search_start].allocated==1) {
        if(search_start==MAX_CALLBACK_POOL-1)
            search_start=0;
        else
            search_start++;
        if(search_from==search_start)
            break;
    }
    
    if(callback_pool[search_start].allocated==0) {
        callback_pool[search_start].allocated=1;
        item=&callback_pool[search_start];

        if(search_start==MAX_CALLBACK_POOL-1)
            search_start=0;
        else
            search_start++;
    }
    
    spin_unlock_irqrestore(&pool_lock,flags);

    if(item==0)
        printk("#### Error to allocate dataout job pool %d\n",search_start);
    return item;
}

struct callback_item *get_first_item(int id)
{
    struct callback_item *cl;
    list_for_each_entry(cl,&dataout_callback_head[id],callback_list)
        return cl;
    return 0;
}

struct callback_item *get_next_item(int id,struct callback_item *item)
{
    struct callback_item *cl;
    int next_found=0;
    
    if(item==0)
        next_found=1;

    list_for_each_entry(cl,&dataout_callback_head[id],callback_list) {
        if(cl==item) {
            next_found=1;
            continue;
        }
        
        if(next_found)
            return cl;
    }
    return 0;
}


struct callback_item *get_item_by_job(int id,struct v_job_t *job)
{
    struct callback_item *cl;
    list_for_each_entry(cl,&dataout_callback_head[id],callback_list)
        if(cl->job==job)
            return cl;
    return 0;
}


int check_if_ap_get(int id)
{
    struct callback_item *cl;
    list_for_each_entry(cl,&dataout_callback_head[id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_AP_GET)
            return 1;
    }
    return 0;
}

struct callback_item *get_free_item(int id)
{
    struct callback_item *cl;

    list_for_each_entry(cl,&dataout_callback_head[id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_VG_PUT) {
            job->user_parameter1=STATUS_AP_GET;
            return cl;
        }
    }
    return 0;
}


int get_item_count(dataout_data_st *dataout)
{
    struct callback_item *cl;
    int counter=0;

    list_for_each_entry(cl,&dataout_callback_head[dataout->id],callback_list)
        counter++;
    return counter;
}

int get_item_ap_count(dataout_data_st *dataout)
{
    struct callback_item *cl;
    int counter=0;

    list_for_each_entry(cl,&dataout_callback_head[dataout->id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_AP_GET)
            counter++;
    }
    return counter;
}

int get_item_free_count(dataout_data_st *dataout)
{
    struct callback_item *cl;
    int counter=0;

    list_for_each_entry(cl,&dataout_callback_head[dataout->id],callback_list) {
         struct v_job_t *job=(struct v_job_t *)cl->job;
         if(job->user_parameter1==STATUS_VG_PUT)
            counter++;
    }
    return counter;
}


void set_dataout_intra(dataout_data_st *dataout,int state,int function)
{
    int dataout_fd, ret;
    
    dataout_fd=ENTITY_FD(DATA_OUT_FD_MAJOR, DATA_OUT_FD_MINOR(dataout->id));
    ret = set_enc_intra_from_dataout(dataout_fd,function);
    if(ret) dataout->intra_state=state;
    else dataout->intra_state=STATE_AUTO;
}

static void dataout_reset(dataout_data_st *dataout,int timeout_tick)
{
    unsigned long flags;
    struct callback_item *cl;
    int need_callback=0;

    spin_lock_irqsave(&dataout_private[dataout->id].lock,flags);

    list_for_each_entry(cl,&dataout_callback_head[dataout->id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;

        if(job->user_parameter1==STATUS_VG_PUT) {
            job->user_parameter1=STATUS_NEED_CB;
            job->status=JOB_STATUS_FAIL;
            need_callback=1;
        }
    }

    if(need_callback) {
        PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
        queue_delayed_work(dvrenc_wq, &dataout->process_dummy,timeout_tick); 
    }

    spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);
}


static void timeout_handler(void *param)
{
    int ch,need_callback=0;
    struct callback_item *cl=0;
    struct v_job_t *skip_job;
    dataout_data_st *dataout;
    unsigned long flags;

    for(ch=0;ch<MAX_DATAOUT_ENTITY_NUM;ch++) {
        dataout = &dataout_private[ch];
        if(dataout->is_active==0)
            continue;
        spin_lock_irqsave(&dataout_private[dataout->id].lock,flags);
        cl=get_first_item(dataout->id);
        if(cl==0) {
            spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);
            continue;
        }
        
        skip_job=(struct v_job_t *)cl->job;

#ifdef DEBUG_USER_AP
        printm("dataout timeout CH%d job %d ap %d flow %d last=0x%x jiffies=0x%x\n",ch,skip_job->id,
            dataout->ap_get_count,dataout->flow_time_ms,cl->putjob_timestamp&0xffff,jiffies&0xffff);
#endif

        need_callback=0;

        if(dataout->ap_get_count==0) {
            if(skip_job->user_parameter1!=STATUS_VG_PUT) {
                spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);
                continue;
            }

            if((jiffies<cl->putjob_timestamp)||(cl->putjob_timestamp>0xffffffff-(dataout->flow_time_ms)))
                cl->putjob_timestamp=jiffies;
            else if(jiffies>cl->putjob_timestamp+(dataout->flow_time_ms)) {//timeout
                struct callback_item *next_cl=get_next_item(dataout->id,cl);
                struct v_job_t *next_job;
                if(next_cl) {
                    next_job=(struct v_job_t *)next_cl->job;
                    if(isKeyframe(next_job)==0) { //next is not key frame
                        set_dataout_intra(dataout,STATE_FORCE_I_SKIP_P,INTRA_ONCE_I);
                        dataout_reset(dataout,1);
                    }
                }
                else
                    set_dataout_intra(dataout,STATE_FORCE_I_SKIP_P,INTRA_ONCE_I);

                skip_job->user_parameter1=STATUS_NEED_CB;
                skip_job->status=JOB_STATUS_FAIL;
                need_callback=1;
                
                if(printk_ratelimit())
                    printk("(%s) Timeout to wait AP buffer get, skip! (0x%x,0x%x flow %d)\n",
                        skip_job->entity->name,cl->putjob_timestamp&0xffff,
                        (int)jiffies&0xffff,dataout->flow_time_ms);
            } else if(jiffies>cl->putjob_timestamp+(dataout->flow_time_ms/2)) { //half timeout
                wake_up(&dataout->read_queue);
            }
        } else {
            if(skip_job->user_parameter1==STATUS_AP_GET) {
                if((jiffies<cl->ap_get_timestamp)||(cl->ap_get_timestamp>0xffffffff-TIMEOUT_VALUE))
                    cl->ap_get_timestamp=jiffies;
                else if(jiffies>cl->ap_get_timestamp+TIMEOUT_VALUE) {
                    if(printk_ratelimit())
                        printk("(%s) FATAL AP ERROR!!! Job %d AP buffer handling timeout for %d msec! (0x%x vs 0x%x)\n",
                            skip_job->entity->name,skip_job->id,TIMEOUT_VALUE,
                            cl->ap_get_timestamp&0xffff,(int)jiffies&0xffff);
                    if(fnDtoNotifyFnc)
                        fnDtoNotifyFnc(ch, DTO_JOB_TIMEOUT);
                }
            }
        }

        spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);

        if(need_callback) {
	        PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
        	queue_delayed_work(dvrenc_wq, &dataout->process_dummy,HZ/1000);
        }
    }
    
    PREPARE_DELAYED_WORK(&process_timeout,(void *)timeout_handler);
    queue_delayed_work(dvrenc_wq, &process_timeout,MIN_SCHEDULE_TICK);
}


static int callback_handler(struct work_struct *work)
{
    unsigned long flags;
    dataout_data_st *dataout = container_of(work, dataout_data_st, process_dummy.work);
    int i, cnt=0;
    struct callback_item *cl,*nl;
    struct v_job_t *job=0;

    i=dataout->id;
    do {
        spin_lock_irqsave(&dataout_private[i].lock,flags);
        list_for_each_entry_safe(cl,nl,&dataout_callback_head[i],callback_list) {
            job=(struct v_job_t *)cl->job;
            if(job->user_parameter1==STATUS_NEED_CB) {
                list_del(&cl->callback_list);
                job->user_parameter1=STATUS_CB_DONE;
                if(job->callback(job, job->status)) {
                    cnt++;
                    if(fnDtoNotifyFnc && job->status==JOB_STATUS_FINISH)
                        fnDtoNotifyFnc(i, DTO_JOB_CALLBACK);
                }
                free_pool(cl);
                continue;
            }
            break;
        }
        spin_unlock_irqrestore(&dataout_private[i].lock,flags);
    }while(0);

    return cnt;    
}


static int dataout_putjob(void *param)
{
    unsigned long flags;
    struct callback_item *cl;
    struct v_job_t *job=(struct v_job_t *)param;
    dataout_data_st  *dataout;
    struct timeval current_tv;
    int skip_it=0;
    struct v_graph_info *ptr=(struct v_graph_info *)job->in_header->addr_va;
    
    do_gettimeofday(&current_tv);
    if(current_tv.tv_sec>ptr->first_tv.tv_sec) {
        if((current_tv.tv_sec-ptr->first_tv.tv_sec>1)&&(current_tv.tv_usec-ptr->first_tv.tv_usec)) { //>1sec
            skip_it=1;
        } else if(current_tv.tv_sec-ptr->first_tv.tv_sec>2) { //>2sec
            skip_it=1;
        }
    }

    dataout = (dataout_data_st*) (job->entity->private);
    spin_lock_irqsave(&dataout_private[dataout->id].lock,flags);
    
    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s \n", dataout->id, __FUNCTION__);

    cl=alloc_pool(dataout->id);
    if(cl==0)
        panic("No more dataout pool\n");
    cl->ap_get_timestamp=0;
    cl->putjob_timestamp=jiffies;
    cl->job=(struct v_job_t *)job;
    job->user_parameter1=STATUS_VG_PUT;

    if(job->swap)
        cl->session_id=++ses_id[dataout->id];
    else
        cl->session_id=ses_id[dataout->id];
    
    if(dataout->is_active) {
        list_add_tail(&cl->callback_list,&dataout_callback_head[dataout->id]);
        wake_up(&dataout->read_queue);
        if(dataout->new_rate) {
            dataout->queue_threshold=video_get_queue_number(job->in_buf);
            dataout->flow_time_ms=2*dataout->queue_threshold*(1000/dataout->new_rate);//double timout

            if(dataout->flow_time_ms<MIN_FLOW_TIME_MS)
                dataout->flow_time_ms=MIN_FLOW_TIME_MS;
            else if(dataout->flow_time_ms>MAX_FLOW_TIME_MS) {
                int div=dataout->flow_time_ms/MAX_FLOW_TIME_MS;
                dataout->flow_time_ms=dataout->flow_time_ms/div;
                dataout->queue_threshold=dataout->queue_threshold/div;
            }

#ifdef DEBUG_USER_AP
            printm("##### idataout_put_job CH%d flow=%d threshold=%d rate=%d\n",
                dataout->id,dataout->flow_time_ms,dataout->queue_threshold,dataout->new_rate);
#endif
            dataout->new_rate=0;
        }
        
        if(skip_it) {
            set_dataout_intra(dataout,STATE_FORCE_I_SKIP_P,INTRA_ONCE_I);
            dataout_reset(dataout,0);
            PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
            queue_delayed_work(dvrenc_wq, &dataout->process_dummy,HZ/1000);
        }
    } else {
        if(get_item_ap_count(dataout))
            wake_up(&dataout->read_queue);

        job->user_parameter1=STATUS_NEED_CB;
        job->status=JOB_STATUS_FAIL;
        list_add_tail(&cl->callback_list,&dataout_callback_head[dataout->id]);
        PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
        queue_delayed_work(dvrenc_wq, &dataout->process_dummy,HZ/1000);
    }

    spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);
    
    return JOB_STATUS_ONGOING;
}


static int dataout_flushjob(void *job)
{
    printk("[dto,0x%x] doesn't support flush!\n", (int)((struct v_job_t *)job)->entity->private);
    return 0;
}


static int dataout_stop(void *var)
{
    struct video_entity_t *entity = (struct video_entity_t*)var;
    dataout_data_st  *dataout = (dataout_data_st*) entity->private;
    
    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s \n", dataout->id, __FUNCTION__);

    dataout_reset(dataout,0); //reset right now
    return 0;
}


static unsigned int dataout_getproperty(void *entity, unsigned int id)
{
    printk("[dto,id:%d] doesn't support getproperty!\n", id);
    return 0;
}

static int dataout_setproperty(void *entity, unsigned int id, unsigned int value)
{
    printk("[dto,id:%d] doesn't support setproperty!\n", id);
    return 0;
}

static int dataout_queryid(void *entity, char *string)
{
    printk("[dto,0x%x] doesn't support queryid!\n", (int)((struct video_entity_t*)entity)->private);
    return 0;
}



static struct v_operations dataout_ops ={
    putjob:     &dataout_putjob,
    flushjob:   &dataout_flushjob,
    stop:        &dataout_stop,
    getproperty: &dataout_getproperty,
    setproperty: &dataout_setproperty,
    queryid:     &dataout_queryid,
};    


//////////////////////////////////////////////
//         Interface to DVR driver          //
//////////////////////////////////////////////

void idataout_init(fnNotifyFnc_t fnNotifyFnc)
{
    int i;

    VDBGPRINT(DBG_ENTITY_FNC, "[dataout] %s, is_init(%d)\n",  __FUNCTION__, is_dataout_init);

    if(!is_dataout_init) {
        memset(dataout_private, 0x0, sizeof(dataout_private));
        is_dataout_init = 1;

        for(i=0; i<MAX_DATAOUT_ENTITY_NUM; i++) {
            spin_lock_init(&dataout_private[i].lock);
            dataout_private[i].id = i;
            dataout_private[i].entity.private = &dataout_private[i];
            dataout_private[i].fifo = kfifo_alloc(sizeof(unsigned int)*MAX_DATAOUT_FIFO_LENGTH, GFP_KERNEL, &dataout_private[i].lock);

            dataout_private[i].entity.major = DATA_OUT_FD_MAJOR;
            dataout_private[i].entity.minor = DATA_OUT_FD_MINOR(i);
            snprintf(dataout_private[i].entity.name, MAX_NAME_SIZE, "dataout_%d", i);
            dataout_private[i].entity.v_ops = &dataout_ops;
            video_entity_register(&dataout_private[i].entity);
            init_MUTEX(&dataout_private[i].oper_sem);

            dataout_private[i].intra_state=STATE_AUTO;
			ses_id[i]=0;
            INIT_DELAYED_WORK(&dataout_private[i].process_dummy, 0);
            init_waitqueue_head(&dataout_private[i].read_queue);
        }

        callback_pool=kmalloc(sizeof(struct callback_item)*MAX_CALLBACK_POOL,GFP_KERNEL);
        if(callback_pool==0)
            panic("Error to allcoate pool");
        
        memset(callback_pool,0,sizeof(struct callback_item)*MAX_CALLBACK_POOL);
        for(i=0;i<MAX_CALLBACK_POOL;i++)
            INIT_LIST_HEAD(&callback_pool[i].callback_list);
        
        fnDtoNotifyFnc = fnNotifyFnc;
        init_MUTEX(&handler_sem);

        PREPARE_DELAYED_WORK(&process_timeout,(void *)timeout_handler);
        queue_delayed_work(dvrenc_wq, &process_timeout,MIN_SCHEDULE_TICK);
    }
}

void idataout_terminate(void)
{
    int i;

    VDBGPRINT(DBG_ENTITY_FNC, "[dataout] %s, is_init(%d)\n", __FUNCTION__, is_dataout_init);

	cancel_delayed_work(&process_timeout);
    if(is_dataout_init) {
        for(i=0; i<MAX_DATAOUT_ENTITY_NUM; i++) {
            kfifo_free(dataout_private[i].fifo);
            video_entity_deregister(&dataout_private[i].entity);
		cancel_delayed_work(&dataout_private[i].process_dummy);
        }
        is_dataout_init = 0;
        fnDtoNotifyFnc = NULL;

        if(callback_pool)
            kfree(callback_pool);
    }
}

int idataout_open(int id, wait_queue_head_t **wait_queue)
{
    dataout_data_st  *dataout;

    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s \n", id, __FUNCTION__);

    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;

    dataout = &dataout_private[id];
    dataout->ap_get_count=0;
    dataout->is_active = TRUE;
    dataout->intra_state=STATE_AUTO;
    if(wait_queue)
        *wait_queue = &dataout->read_queue;

    return 0;
}

int idataout_close(int id)
{
    dataout_data_st  *dataout;

    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s \n", id, __FUNCTION__);

    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;

    dataout = &dataout_private[id];

    kfifo_reset(dataout->fifo);
    dataout->is_active = FALSE;

    
    return 0;
}


//return -2: no buffer
int idataout_get(int id, struct v_job_t **out_job, int *swap)
{
    int ret;
    unsigned long flags;
    struct v_job_t *job;
    struct callback_item *cl;
    dataout_data_st  *dataout=0;
    int schedule_it=0;
    int keyframe;

    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;

    spin_lock_irqsave(&dataout_private[id].lock,flags);
    ret=-2;
    dataout = &dataout_private[id];

    keyframe=0;
idataout_get_again:
    cl=get_free_item(id);
    if(cl) {
        int swap_with_intra=0;
        
        cl->ap_get_timestamp=jiffies;
        job=(struct v_job_t *)cl->job;
        if(dataout->cur_session_id==cl->session_id)
            *swap=0;
        else
            *swap=1;

        if((dataout->swap_intra==1)&&(*swap==1))
            swap_with_intra=1;
        
        if((dataout->intra_state==STATE_FORCE_I_SKIP_P)||(swap_with_intra==1)) {
            keyframe=isKeyframe(job);
            if(keyframe==0) {
                job->user_parameter1=STATUS_NEED_CB;
                job->status=JOB_STATUS_FAIL;
                schedule_it=1;
                if(swap_with_intra==1) { //need to set intra and reset
                    dataout->swap_intra=0;
                    set_dataout_intra(dataout,STATE_FORCE_I_SKIP_P,INTRA_ONCE_I);
                    dataout_reset(dataout,0);
                }
                goto idataout_get_again;
            }
            
            if(swap_with_intra==1)
                dataout->swap_intra=0;
        }

    
        if((dataout->intra_state==STATE_FORCE_I_SKIP_P)||
           (dataout->intra_state==STATE_FORCE_I_KEEP_P)) {
            keyframe=isKeyframe(job);
            if(keyframe)
                set_dataout_intra(dataout,STATE_AUTO,INTRA_AUTO);
        }
        
        dataout->ap_get_count++;
        dataout->cur_session_id=cl->session_id;
        
        *out_job=job;
        (*out_job)->user_parameter1=STATUS_AP_GET;

        if(fnDtoNotifyFnc)
            fnDtoNotifyFnc(id, DTO_JOB_AP_GET);

#ifdef DEBUG_USER_AP
    printm("##### idataout_get CH%d %s job %d at 0x%x\n",id,job->entity->name,job->id,jiffies&0xffff);
#endif
       ret=0;
    }

    if(schedule_it) {
        PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
        queue_delayed_work(dvrenc_wq, &dataout->process_dummy,HZ/1000); //right now handle
    }

    spin_unlock_irqrestore(&dataout_private[id].lock,flags);

    return ret;
}


int idataout_put(int id, struct v_job_t *out_job)
{
    dataout_data_st  *dataout;
    unsigned long flags;
    struct callback_item *cl;
    
    dataout = (dataout_data_st*) (out_job->entity->private);
    
    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;

    spin_lock_irqsave(&dataout_private[id].lock,flags);
    
    cl=get_item_by_job(id,out_job);
    if(cl==0) {
        struct callback_item *cl2;
        struct v_job_t *job2;
        //XXX do not panic
        //enter_panic(991);
        printk("Error to find %s job %d\n",out_job->entity->name,out_job->id);
        printm("Error to find %s job %d\n",out_job->entity->name,out_job->id);
        list_for_each_entry(cl2,&dataout_callback_head[id],callback_list) {
            job2=cl2->job;
            printk("0x%x:%s job %d\n",(int)cl2,job2->entity->name,job2->id);
            printm("0x%x:%s job %d\n",(int)cl2,job2->entity->name,job2->id);
        }
        print_debug();
        spin_unlock_irqrestore(&dataout_private[id].lock,flags);
        return -1;
    }
    
#ifdef DEBUG_USER_AP
    printm("##### idataout_put CH%d %s job %d flow=%d (0x%x) at 0x%x\n",
        id,out_job->entity->name,out_job->id,dataout->flow_time_ms,
        cl->putjob_timestamp,jiffies&0xffff);
#endif

    dataout->ap_get_count--;
    out_job->user_parameter1=STATUS_NEED_CB;
    out_job->status=JOB_STATUS_FINISH;

    if(cl==0) {
        struct callback_item *cl2;
        struct v_job_t *job2;
        
        enter_panic(119);
        printk("Try to find %s job %d\n",out_job->entity->name,out_job->id);
        printm("Try to find %s job %d\n",out_job->entity->name,out_job->id);
        list_for_each_entry(cl2,&dataout_callback_head[id],callback_list) {
            job2=cl2->job;
            printk("0x%x:%s job %d\n",(int)cl2,job2->entity->name,job2->id);
            printm("0x%x:%s job %d\n",(int)cl2,job2->entity->name,job2->id);
        }
        print_debug();
    }
        
    if((jiffies<cl->putjob_timestamp)||(cl->putjob_timestamp>0xffffffff-(dataout->flow_time_ms*2)))
        cl->putjob_timestamp=jiffies;
    else if(jiffies>cl->putjob_timestamp+(dataout->flow_time_ms*2)) {//timeout when x2 timeout value
#ifdef DEBUG_USER_AP
        printm("##### CH%d %s job %d timeout!\n",id,out_job->entity->name,out_job->id);
#endif
        set_dataout_intra(dataout,STATE_FORCE_I_SKIP_P,INTRA_ONCE_I);
        dataout_reset(dataout,0);
    }
    PREPARE_DELAYED_WORK(&dataout->process_dummy,(void *)callback_handler);
    queue_delayed_work(dvrenc_wq, &dataout->process_dummy,0); //right now handle
    spin_unlock_irqrestore(&dataout_private[id].lock,flags);
    return 0;
}


int idataout_reset(int id)
{
    dataout_data_st  *dataout;
    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s \n", id, __FUNCTION__);
    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;
    dataout = &dataout_private[id];
    dataout_reset(dataout,100);
    return 0;
}


int idataout_setactive(int id, int is_active)
{
    dataout_data_st  *dataout;

    VDBGPRINT(DBG_ENTITY_FNC, "[dataout,%d] %s, active(%d)\n", id, __FUNCTION__, is_active);
    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;
    dataout = &dataout_private[id];

    down(&dataout->oper_sem);
    dataout->is_active = is_active;
    up(&dataout->oper_sem);
    
    if(dataout->is_active==0)
        dataout_reset(dataout,0); //reset right now
    return 0;
}

int idataout_getlength(int id)
{
    int count;
    unsigned long flags;
    dataout_data_st  *dataout;

    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;
    dataout = &dataout_private[id];
    
    spin_lock_irqsave(&dataout_private[dataout->id].lock,flags);
    count=get_item_free_count(dataout);
    spin_unlock_irqrestore(&dataout_private[dataout->id].lock,flags);
    return count;
}

int idataout_reset_intra(int id)
{
    dataout_data_st *dataout;
    
    dataout = &dataout_private[id];
    set_dataout_intra(dataout,STATE_FORCE_I_KEEP_P,INTRA_ONCE_I);
    return 0;
}

//force intra when swap
int idataout_swap_intra(int id)
{
    dataout_data_st *dataout;
    
    dataout = &dataout_private[id];
    dataout->swap_intra=1;
    return 0;
}


int idataout_set_flow_rate(int fd,int rate)
{
    int id;
    dataout_data_st  *dataout;

    id=fd&0xff;
    if(id<0 || id>=MAX_DATAOUT_ENTITY_NUM)
        return -1;

    dataout = &dataout_private[id];
    dataout->new_rate=rate;
    dataout->queue_threshold=5;  //first init to buffer count=5
    dataout->flow_time_ms=MIN_FLOW_TIME_MS; //multiple-queue number timout
        
#ifdef DEBUG_USER_AP
    printm("##### idataout_set_flow_rate CH%d 0x%x rate %d\n",id,fd,rate);
#endif
    return 0;
}


EXPORT_SYMBOL(idataout_init);
EXPORT_SYMBOL(idataout_terminate);
EXPORT_SYMBOL(idataout_open);
EXPORT_SYMBOL(idataout_close);
EXPORT_SYMBOL(idataout_get);
EXPORT_SYMBOL(idataout_put);
EXPORT_SYMBOL(idataout_reset);
EXPORT_SYMBOL(idataout_setactive);
EXPORT_SYMBOL(idataout_getlength);
EXPORT_SYMBOL(idataout_reset_intra);
