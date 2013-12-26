#define __DATAIN_ENTITY_C__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/semaphore.h>

#include "videograph_api.h"
#include "dvr_type_define.h"
#include "datain_entity.h"
#include "dvr_mod_define.h"
#include "dvr_graph.h"

//NOTE: datain uses [dvr_dec]'s dbgmode
extern int dvr_dec_dbgmode;
#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_dec_dbgmode) printk(fmt, ## args);}

/*
Time to Skip
1.check time: putjob
2.kifo full, is_ap_get=0, putjob_timestamp > MIN_FLOW_TICK  ==> JOB_STATUS_DUMMY

Time to Error
1.check time: error_handler (queue size * MIN_FLOW_TICK *2)
2.is_ap_get=1, ap_get_timestamp > queue size*MIN_FLOW_TICK ==> panic
 */

#define MIN_FLOW_TICK   (30)    //30ms
#define AP_TIEMOUT      2       //2sec
#define FLOW_BONUS 100

//#define DEBUG_USER_AP

struct callback_item
{
    char            allocated;
    unsigned int    ap_get_timestamp;
    unsigned int    putjob_timestamp;
    struct list_head    callback_list;
    void            *job;
};

extern struct list_head datain_callback_head[MAX_DATAIN_ENTITY_NUM];
extern struct workqueue_struct *dvrdec_wq;
static int callback_handler(struct work_struct *work);
static void timeout_handler(void *);
static DECLARE_DELAYED_WORK(process_timeout, (void *)timeout_handler);
static struct semaphore handler_sem;

#define STATUS_VG_PUT   1
#define STATUS_AP_GET   2
#define STATUS_NEED_CB  3
#define STATUS_CB_DONE  4


#define MAX_CALLBACK_POOL (MAX_DATAIN_ENTITY_NUM*15)   //As 30fps, be 1/2 sec pool
static int search_start=0;
static spinlock_t pool_lock=SPIN_LOCK_UNLOCKED;
static struct callback_item *callback_pool;
static datain_data_st datain_private[MAX_DATAIN_ENTITY_NUM];
static int is_datain_init=0;
static fnNotifyFnc_t fnDtiNotifyFnc=NULL;

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
        printk("#### Error to allocate datain job pool %d\n",search_start);
    return item;
}

struct callback_item *get_first_item(int id)
{
    struct callback_item *cl;
    list_for_each_entry(cl,&datain_callback_head[id],callback_list)
        return cl;
    return 0;
}


struct callback_item *get_next_item(int id,struct callback_item *item)
{
    struct callback_item *cl;
    int next_found=0;
    struct v_job_t *job2;
    
    if(item==0)
        next_found=1;

    list_for_each_entry(cl,&datain_callback_head[id],callback_list) {
        
        job2=(struct v_job_t *)cl->job;
        
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
    list_for_each_entry(cl,&datain_callback_head[id],callback_list)
        if(cl->job==job)
            return cl;
    return 0;
}


int check_if_ap_get(int id)
{
    struct callback_item *cl;
    list_for_each_entry(cl,&datain_callback_head[id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_AP_GET)
            return 1;
    }
    return 0;
}

struct callback_item *get_free_item(int id)
{
    struct callback_item *cl;

    list_for_each_entry(cl,&datain_callback_head[id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        
        if(job->user_parameter1==STATUS_VG_PUT) {
            job->user_parameter1=STATUS_AP_GET;
            return cl;
        }
    }
    return 0;
}


int get_item_count(datain_data_st *datain)
{
    struct callback_item *cl;
    int counter=0;

    list_for_each_entry(cl,&datain_callback_head[datain->id],callback_list)
        counter++;
    return counter;
}

int get_item_ap_count(datain_data_st *datain)
{
    struct callback_item *cl=0;
    struct v_job_t *job;
    int counter=0;

    list_for_each_entry(cl,&datain_callback_head[datain->id],callback_list) {
        job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_AP_GET)
            counter++;
    }
    return counter;
}

int get_item_free_count(datain_data_st *datain)
{
    struct callback_item *cl=0;
    struct v_job_t *job;
    int counter=0;

    list_for_each_entry(cl,&datain_callback_head[datain->id],callback_list) {
        job=(struct v_job_t *)cl->job;
        if(job->user_parameter1==STATUS_VG_PUT)
            counter++;
    }
    return counter;
}


static void datain_reset(datain_data_st *datain,int timeout_tick)
{
    unsigned long flags;
    struct callback_item *cl;
    int need_callback=0;

    spin_lock_irqsave(&datain_private[datain->id].lock,flags);

    list_for_each_entry(cl,&datain_callback_head[datain->id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        
        if(job->user_parameter1==STATUS_VG_PUT) {
            job->user_parameter1=STATUS_NEED_CB;
            job->status=JOB_STATUS_FAIL;
            need_callback=1;
        }
    }

  	if(need_callback) {        
        PREPARE_DELAYED_WORK(&datain->process_dummy,(void *)callback_handler);        
        queue_delayed_work(dvrdec_wq, &datain->process_dummy,timeout_tick); 
    }

    spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
}




static void timeout_handler(void *param)
{
    int ch,need_callback=0,global_need_timeout=0;
    struct callback_item *cl=0;
    datain_data_st *datain;
    struct v_job_t *skip_job;
    unsigned long flags;

    for(ch=0;ch<MAX_DATAIN_ENTITY_NUM;ch++) {
        datain = (datain_data_st *)&datain_private[ch];
        if(datain->need_timeout==0)
            continue;
        if(datain->is_active==0)
            continue;
    
        spin_lock_irqsave(&datain_private[datain->id].lock,flags);
        cl=0;
check_next:
        cl=get_next_item(datain->id,cl);
        if(cl==0){
            spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
            continue;
        }

        skip_job=(struct v_job_t *)cl->job;

        datain->need_timeout=0;
        need_callback=0;
        if(datain->ap_get_count==0) {
            unsigned int start_time;
            unsigned int timeout;
            unsigned int overflow;
            
            timeout=datain->flow_time_ms;
            if(timeout>(TIMEOUT_FACTOR/2))
                timeout=(TIMEOUT_FACTOR/2); //must less than TIMEOUT_FACTOR/2
            overflow=0xffffffff-timeout;

#if 0
            printm("datain timeout CH%d job %d ap %d pa %d last=0x%x jiffies=0x%x\n",ch,skip_job->id,
                datain->ap_get_count,skip_job->user_parameter1,cl->putjob_timestamp&0xffff,jiffies&0xffff);
#endif

            if(skip_job->user_parameter1!=STATUS_VG_PUT) {
                goto check_next;
            }

            if((datain->last_ap_put_timestamp>overflow)||(jiffies<datain->last_ap_put_timestamp))
                datain->last_ap_put_timestamp=0;
            if((cl->putjob_timestamp>overflow)||(jiffies<cl->putjob_timestamp))
                cl->putjob_timestamp=0;

            if(cl->putjob_timestamp>datain->last_ap_put_timestamp)
                start_time=cl->putjob_timestamp;
            else
                start_time=datain->last_ap_put_timestamp;

            if(jiffies>start_time+timeout) {
                skip_job->status=JOB_STATUS_DUMMY;
                skip_job->user_parameter1=STATUS_NEED_CB;
                need_callback=1;
            }
            global_need_timeout=datain->need_timeout=1;
        } else {
            if(skip_job->user_parameter1==STATUS_AP_GET) {
                if((jiffies<cl->ap_get_timestamp)||(cl->ap_get_timestamp>0xffffffff-(HZ*AP_TIEMOUT)))
                    cl->ap_get_timestamp=jiffies;
                else if(jiffies>cl->ap_get_timestamp+(HZ*AP_TIEMOUT)) { //2sec timeout
                    printk("########## (%s) Fatal Error AP handling timeout %d sec (job %d)!!!!\n",
                        skip_job->entity->name,AP_TIEMOUT,skip_job->id);
                    wake_up(&datain->read_queue);
                    if(fnDtiNotifyFnc)
                        fnDtiNotifyFnc(ch, DTI_JOB_TIMEOUT);
                } else if(jiffies>cl->ap_get_timestamp+(datain->flow_time_ms*2)) //2 frames timeout
                    wake_up(&datain->read_queue);
                global_need_timeout=datain->need_timeout=1;
            }
        }

        spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
    	if(need_callback) {        	
        	PREPARE_DELAYED_WORK(&datain->process_dummy,(void *)callback_handler);
        	queue_delayed_work(dvrdec_wq, &datain->process_dummy,HZ/1000);
        }
    }

    if(global_need_timeout) {        
        PREPARE_DELAYED_WORK(&process_timeout,(void *)timeout_handler);
        queue_delayed_work(dvrdec_wq, &process_timeout,MIN_FLOW_TICK);
    }
}

static int callback_handler(struct work_struct *work)
{
    unsigned long flags;
    datain_data_st *datain = container_of(work, datain_data_st, process_dummy.work);
    int i, cnt=0;
    struct callback_item *cl,*nl;
    struct v_job_t *job=0;
    
    i = datain->id;
    do {
        spin_lock_irqsave(&datain_private[i].lock,flags);
        list_for_each_entry_safe(cl,nl,&datain_callback_head[i],callback_list) {
            job=(struct v_job_t *)cl->job;
            if(job->user_parameter1==STATUS_NEED_CB) {
                list_del(&cl->callback_list);
                job->user_parameter1=STATUS_CB_DONE;
                if(job->callback(job, job->status)) {
                    cnt++;
                    if(fnDtiNotifyFnc && job->status==JOB_STATUS_FINISH)
                        fnDtiNotifyFnc(i, DTI_JOB_CALLBACK);
                }
                free_pool(cl);
			    continue;
            }
            break;
        }
		spin_unlock_irqrestore(&datain_private[i].lock,flags);	
    } while(0);

    return cnt;
}



static int checking_and_do_skip(datain_data_st *datain,int threshold)
{
    int count;

    if(datain->ap_get_count==0) { //ap is get, trigger timeout_headler
        count=get_item_count(datain);

        if((count>=threshold)&&    //buffer full
           (check_if_ap_get(datain->id)==0)) { //no body get
            datain->need_timeout=1;            
            PREPARE_DELAYED_WORK(&process_timeout,(void *)timeout_handler);
            queue_delayed_work(dvrdec_wq, &process_timeout,MIN_FLOW_TICK);
        }
    } else {
        datain->need_timeout=1;        
        PREPARE_DELAYED_WORK(&process_timeout,(void *)timeout_handler);
        queue_delayed_work(dvrdec_wq, &process_timeout,500);    //500ms
    }
    return 0;
}

static int datain_putjob(void *param)
{
    unsigned long flags;
    struct callback_item *cl;
    struct v_job_t *job=(struct v_job_t *)param;
    datain_data_st  *datain;
    int threshold;

    datain = (datain_data_st*) (job->entity->private);
    
    spin_lock_irqsave(&datain_private[datain->id].lock,flags);
    
    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s \n", datain->id, __FUNCTION__);

    cl=alloc_pool(datain->id);
    if(cl==0)
        panic("No more datain pool\n");
    cl->ap_get_timestamp=0;
    cl->putjob_timestamp=jiffies;
    cl->job=(struct v_job_t *)job;
    job->user_parameter1=STATUS_VG_PUT;

    if(job->swap && datain->new_rate) {
        int buf_num=video_get_queue_number(job->out_buf);
        datain->flow_time_ms=((1000-FLOW_BONUS)/datain->new_rate)*buf_num;
        datain->new_rate = 0;
    }

    if(datain->is_active) {
        list_add_tail(&cl->callback_list,&datain_callback_head[datain->id]);
        wake_up(&datain->read_queue);
        threshold=video_get_queue_number(job->out_buf);
        checking_and_do_skip(datain,threshold);
    } else {
        if(get_item_ap_count(datain))
            wake_up(&datain->read_queue);
        job->user_parameter1=STATUS_NEED_CB;
        job->status=JOB_STATUS_FAIL;
        
        list_add_tail(&cl->callback_list,&datain_callback_head[datain->id]);
        
        PREPARE_DELAYED_WORK(&datain->process_dummy,(void *)callback_handler);
        queue_delayed_work(dvrdec_wq, &datain->process_dummy,HZ/1000);
    }

    spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
    
    return JOB_STATUS_ONGOING;
}


//clear old workg
static int datain_clear_old(void *parm1,int workg)
{
    unsigned long flags;
    struct callback_item *cl;
    int need_callback=0;
    struct video_entity_t *entity;
    datain_data_st  *datain;

    entity=(struct video_entity_t *)parm1;
    datain = (datain_data_st*)entity->private;
    
    spin_lock_irqsave(&datain_private[datain->id].lock,flags);

    list_for_each_entry(cl,&datain_callback_head[datain->id],callback_list) {
        struct v_job_t *job=(struct v_job_t *)cl->job;
        if((job->user_parameter1==STATUS_VG_PUT)&&(job->workg==workg)) {
            job->user_parameter1=STATUS_NEED_CB;
            job->status=JOB_STATUS_DUMMY;
            need_callback=1;
        }
    }

  	if(need_callback) {  
        PREPARE_DELAYED_WORK(&datain->process_dummy,(void *)callback_handler);
        queue_delayed_work(dvrdec_wq, &datain->process_dummy,HZ/1000);
    }

    spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
    return 0;
}


static int datain_flushjob(void *job)
{
    printk("[datain] doesn't support flush!\n");

    return 0;
}


static int datain_stop(void *var)
{
    struct video_entity_t *entity = (struct video_entity_t*)var;
    datain_data_st  *datain = (datain_data_st*) entity->private;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s \n", datain->id, __FUNCTION__);

    datain_reset(datain,0); //reset right now
    //printk("########[%d]Stop(%#x)\n",datain->id , video_gettime_ms());
    printm("########[%d]Stop(%#x)\n",datain->id , video_gettime_ms());
    return 0;
}

static unsigned int datain_getproperty(void *entity, unsigned int id)
{
    printk("[datain] doesn't support getproperty!\n");
    return 0;
}

static int datain_setproperty(void *entity, unsigned int id, unsigned int value)
{
    printk("[datain] doesn't support setproperty!\n");
    return 0;
}

static int datain_queryid(void *entity, char *string)
{
    printk("[datain] doesn't support queryid!\n");
    return 0;
}



static struct v_operations datain_ops ={
    putjob:     &datain_putjob,
    flushjob:   &datain_flushjob,
    stop:        &datain_stop,
    getproperty: &datain_getproperty,
    setproperty: &datain_setproperty,
    queryid:     &datain_queryid,
    clear_old:   &datain_clear_old,
};    

static void idatain_reset_all(void *prive)
{
#if 0    
    int ii;
    datain_data_st  *datain;
    unsigned long flags;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain] %s \n", __FUNCTION__);
    for(ii=0; ii<MAX_DATAIN_ENTITY_NUM; ii++){
         datain = &datain_private[ii];
         if(datain->is_active){
            datain_reset(datain,100);
        }
    }
    printk("########Resetall(%#x)\n", video_gettime_ms());
#endif
}

static dvr_swap_notify_info datain_swap_notify = {
    .callback = idatain_reset_all,
    .private = NULL,
};


//////////////////////////////////////////////
//         Interface to DVR driver          //
//////////////////////////////////////////////

void idatain_init(fnNotifyFnc_t fnNotifyFnc)
{
    int i;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain] %s, is_init(%d)\n",  __FUNCTION__, is_datain_init);
    
    if(!is_datain_init) {
        memset(datain_private, 0x0, sizeof(datain_private));
        is_datain_init = TRUE;

        for(i=0; i<MAX_DATAIN_ENTITY_NUM; i++) {
            spin_lock_init(&datain_private[i].lock);
            datain_private[i].id = i;
            datain_private[i].entity.private = &datain_private[i];
            datain_private[i].fifo = kfifo_alloc(sizeof(unsigned int)*MAX_DATAIN_FIFO_LENGTH, GFP_KERNEL, &datain_private[i].lock);

            datain_private[i].entity.major = DATA_IN_FD_MAJOR;
            datain_private[i].entity.minor = DATA_IN_FD_MINOR(i);
            snprintf(datain_private[i].entity.name, MAX_NAME_SIZE, "datain_%d", i);
            datain_private[i].entity.v_ops = &datain_ops;
            video_entity_register(&datain_private[i].entity);
            init_MUTEX(&datain_private[i].oper_sem);            
            INIT_DELAYED_WORK(&datain_private[i].process_dummy, 0);
            init_waitqueue_head(&datain_private[i].read_queue);
        }

        callback_pool=kmalloc(sizeof(struct callback_item)*MAX_CALLBACK_POOL,GFP_KERNEL);
        if(callback_pool==0)
            panic("Error to allcoate pool");
    
        memset(callback_pool,0,sizeof(struct callback_item)*MAX_CALLBACK_POOL);
        for(i=0;i<MAX_CALLBACK_POOL;i++)
            INIT_LIST_HEAD(&callback_pool[i].callback_list);
        
        init_MUTEX(&handler_sem);
        INIT_LIST_HEAD(&datain_swap_notify.list);
        fnDtiNotifyFnc = fnNotifyFnc;
        register_swap_notify(&datain_swap_notify); 
    }
}

void idatain_terminate(void)
{
    int i;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain] %s, is_init(%d)\n", __FUNCTION__, is_datain_init);
    if(is_datain_init) {
        for(i=0; i<MAX_DATAIN_ENTITY_NUM; i++) {
            kfifo_free(datain_private[i].fifo);
            video_entity_deregister(&datain_private[i].entity);
        }
        is_datain_init = 0;
        fnDtiNotifyFnc = NULL;
        
        if(callback_pool)
            kfree(callback_pool);

        unregister_swap_notify(&datain_swap_notify); 
    }
}

int idatain_open(int id, wait_queue_head_t **wait_queue)
{
    datain_data_st  *datain;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s \n", id, __FUNCTION__);

    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];

    datain->is_active = TRUE;
    datain->flow_time_ms=30*5;  //30ms x 5 buffers
    datain->ap_get_count=0;
    
    if(wait_queue)
        *wait_queue = &datain->read_queue;
    
    return 0;
}

int idatain_close(int id)
{
    datain_data_st  *datain;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s \n", id, __FUNCTION__);
    printm("##### idatain_close %d\n",id);
    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];

    down(&datain->oper_sem);
    datain->is_active = FALSE;
    up(&datain->oper_sem);

    return 0;
}

//return -2: no buffer
int idatain_get(int id, struct v_job_t **out_job)
{
    int ret;
    unsigned long flags;
    struct callback_item *cl;

    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    spin_lock_irqsave(&datain_private[id].lock,flags);
    
    ret=-2;
    cl=get_free_item(id);
    if(cl) {
        datain_data_st  *datain;
        datain = (datain_data_st *)&datain_private[id];

        datain->ap_get_count++;

        cl->ap_get_timestamp=jiffies;
        *out_job=(struct v_job_t *)cl->job;
        (*out_job)->user_parameter1=STATUS_AP_GET;
        ret=0;
        if(fnDtiNotifyFnc)
            fnDtiNotifyFnc(id, DTI_JOB_AP_GET);

#ifdef DEBUG_USER_AP
        printm("    idatain_get CH%d %s job %d at 0x%x\n",id,
            (*out_job)->entity->name,(*out_job)->id,(int)jiffies&0xffff);
#endif
    }
    
    spin_unlock_irqrestore(&datain_private[id].lock,flags);
    return ret;
}


int idatain_put(int id, struct v_job_t *out_job, int status)
{
    struct callback_item *cl;
    datain_data_st  *datain;
    unsigned long flags;
    
    datain = (datain_data_st*) (out_job->entity->private);
    
    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

#ifdef DEBUG_USER_AP
    {
        char *buf=(char *)out_job->out_buf->addr_va;
        int fd=ENTITY_FD(out_job->entity->major,out_job->entity->minor);
        
        printm("    idatain_put CH%d %s job %d depth %d data %02X %02X %02X %02X\n",
            id,out_job->entity->name,out_job->id,video_get_buffer_depth(fd),buf[4],buf[5],buf[6],buf[7]);
    }
#endif

    spin_lock_irqsave(&datain_private[id].lock,flags);
    datain->ap_get_count--;
    datain->last_ap_put_timestamp=jiffies;
    if(status==0)
        out_job->status=JOB_STATUS_FINISH;
    else if(status==2) {//dummy
        out_job->status=JOB_STATUS_DUMMY;
    } else
        out_job->status=JOB_STATUS_FAIL;
    out_job->user_parameter1=STATUS_NEED_CB;
    
    cl=get_item_by_job(id,out_job);
    if((cl)&&(cl->putjob_timestamp>jiffies)&&((cl->putjob_timestamp-jiffies)>datain->flow_time_ms))
        timeout_handler(0);

    spin_unlock_irqrestore(&datain_private[id].lock,flags);

    return callback_handler(&datain_private[id].process_dummy.work);//return callback_handler((void *)id);
}

int idatain_reset(int id)
{
    datain_data_st  *datain;
    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s \n", id, __FUNCTION__);
    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];

    datain_reset(datain,100);
    datain->new_rate = 0;

    return 0;
}

int idatain_setactive(int id, int is_active)
{
    datain_data_st  *datain;

    VDBGPRINT(DBG_ENTITY_FNC, "[datain,%d] %s, active(%d)\n", id, __FUNCTION__, is_active);
    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];

    down(&datain->oper_sem);
    datain->is_active = is_active;
    up(&datain->oper_sem);

    if(datain->is_active==0)
        datain_reset(datain,0); //reset right now

    return 0;
}

int idatain_getlength(int id)
{
    int count;
    unsigned long flags;
    datain_data_st  *datain;

    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];

    spin_lock_irqsave(&datain_private[datain->id].lock,flags);
    count=get_item_free_count(datain);
    spin_unlock_irqrestore(&datain_private[datain->id].lock,flags);
    
    return count;
}

int idatain_set_flow_rate(int fd,int rate)
{
    int id;
    datain_data_st  *datain;

    id=fd&0xff;
    if(id<0 || id>=MAX_DATAIN_ENTITY_NUM)
        return -1;

    datain = &datain_private[id];
    datain->new_rate=rate;
    return 0;
}


EXPORT_SYMBOL(idatain_init);
EXPORT_SYMBOL(idatain_terminate);
EXPORT_SYMBOL(idatain_open);
EXPORT_SYMBOL(idatain_close);
EXPORT_SYMBOL(idatain_get);
EXPORT_SYMBOL(idatain_put);
EXPORT_SYMBOL(idatain_reset);
EXPORT_SYMBOL(idatain_setactive);
EXPORT_SYMBOL(idatain_getlength);
