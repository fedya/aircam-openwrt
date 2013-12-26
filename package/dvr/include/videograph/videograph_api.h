/**
 * @file videograph_api.h
 *  The header file of videograph API for DVR layer and Video entity.
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.7 $
 * $Date: 2011/09/21 02:59:10 $
 *
 * ChangeLog:
 *  $Log: videograph_api.h,v $
 *  Revision 1.7  2011/09/21 02:59:10  ivan
 *  fix backward compatible issue
 *
 *  Revision 1.6  2011/08/31 08:27:11  ivan
 *  update to v0.44
 *
 *  Revision 1.5  2011/06/20 07:10:43  ivan
 *  update to v0.43
 *
 *  Revision 1.4  2011/04/11 08:01:20  ivan
 *  add max buffer threshold number API
 *
 *  Revision 1.3  2011/02/22 05:16:05  ivan
 *  Update to v0.41
 *
 *  Revision 1.2  2010/09/28 06:04:26  ivan
 *  update to v0.36a
 *
 *  Revision 1.1  2010/09/08 06:16:45  ivan
 *  initial add for 2.6.28
 *
 *  Revision 1.64  2010/09/07 07:11:26  ivan
 *  modify the job record structure number for 16ch conexant
 *
 *  Revision 1.63  2010/09/02 06:04:04  ivan
 *  update to v0.35b
 *
 *  Revision 1.62  2010/08/20 03:35:25  ivan
 *  add function comment
 *
 *  Revision 1.119  2010/08/03 08:44:52  ivan
 *  Make source code compatible with linux 2.6.14 and 2.6.28
 *
 */
 
#ifndef _VIDEOGRAPH_H_
#define _VIDEOGRAPH_H_

#define MAX_PROPERTY    13
#define MAX_NAME_SIZE   20
#define MAX_ELEM_SIZE   3096

#define DRVINFO_SZ  0x200

/* callback job status for Video Entity */
#define JOB_STATUS_FAIL     -1 //(USE BY Video Entity) put job fail, use handle_head (use to order response)
#define JOB_STATUS_ONGOING  0  //(USE BY Video Entity) job is ongoing and need to use callback to notify, callback should not return this
#define JOB_STATUS_FINISH   1  //(USE BY Video Entity) job is finished right now
#define JOB_STATUS_DISABLE  3  //(USE BY Video Entity) this job is disabled, determine it meaning by videograph

/* callback job status internal used */
#define JOB_STATUS_DUMMY    2  //(Internal Use) dummy job
#define JOB_STATUS_SKIP     4  //(Internal Use) skip job, JOB_STATUS_FAIL must by order to return, but this needn't
#define JOB_STATUS_BECOME_DUMMY    5  //(Internal Use) becoming dummy job

#define DISABLE_WITH_FINISH    1  //disable treat as JOB_STATUS_FINISH
#define DISABLE_WITH_PASS_ONLY 2  //disable treat as pass graph without keep buffer, will translate to DUMMY
#define DISABLE_WITH_PASS_KEEP 3  //disable treat as pass graph and keep buffer

#define ENTITY_FD(major,minor)  ((major<<16)|(minor))
#define ENTITY_FD_MAJOR(fd)     ((fd>>16)&0xffff)
#define ENTITY_FD_MINOR(fd)     (fd&0xffff)

struct v_property_t {
    unsigned int    id;
    unsigned int    value;
    int             fd;
    struct list_head    list;
};

/* connect(,,type) */
#define CONNECT_FROM    0
#define CONNECT_TO      1

struct v_operations {
    int (*putjob) (void *);     //return >=0 OK, <0 fail
    int (*flushjob) (void *);   //return >=0 OK, <0 fail
    int (*stop) (void *);       //stop engine running
    unsigned int (*getproperty) (void *,unsigned int);
    int (*setproperty) (void *,unsigned int,unsigned int);
    int (*queryid) (void *,char *);
    int (*connect) (void *,int,void *);    //(self video_entity,direction,video_entity), return >=0 OK, <0 fail
    int (*clear_old)(void *,int);   //clear old job
    int reserved1[3];
};    

#define MAX_GRAPH_GROUP         18  //16CH Enc+ 1CH LV/PB
#define MAX_GRAPH               196

/* freeway, 0:normal 1:free by lower job 2:free by putjob */
#define FREE_NORMAL 0       //Free the buffer queue after this job is finsished.
#define FREE_BY_LOW 1       //Free the buffer queue after the lower video entity finished its job.
#define FREE_BY_PUT 2       //Free the buffer queue after next job is put.

#define MAX_REC_NUM 72      //support 5fps, 30/5=6, number lcd_q is 6, so the total needs 36 (don't update it cause entity structure over-size)

//nodirty only effect in period of RESTART--->RUNNING, after RUNNING state, all nodirty will be set true (if not NODIRTY_MODE_DISABLE)
#define NODIRTY_MODE_DISABLE    0   //disable nodirty mode
#define NODIRTY_MODE_TRUE       1   //nodirty is true
#define NODIRTY_MODE_FALSE      2   //nodirty is false

#define DEFAULT_WG_INDEX        1

#define NO_NEED_SWAP            0
#define NEED_SWAP_BY_NEW        1
#define NEED_SWAP_EVERYWHERE    2

#define VIDEO_ENTITY_SIZE       976

struct video_entity_record_t {
    int record_start;
    int record_end;
    unsigned int record_job[MAX_REC_NUM];
};

struct video_entity_t {
    int major,minor;   //16 bit value, should be unsigned short
    char name[MAX_NAME_SIZE];
    struct v_operations  *v_ops;
    int     flushable;      //1:flushable 0:un-flushable
    void    *private;
    struct  list_head   list;
    int     fd;     //fd for upper layer use
    int     workg;
    struct  list_head   unhandle_head;
    int     keep_buffer;
    int     need_swap;
    int     (*preprocess)(void *,void *);  //before putjob, video entity may use preprocess to handle
    int     (*postprocess)(void *,void *); //after putjob, video entity may use preprocess to handle
    int     src_nodirty;  //stay graph nodirty mode, 0: not nodirty mode 1: nodirty mode(enable) 2: nodirty mode(disable
    int     src_disabled;  //disable the entity action
    int     group_id;   //-1
    int     update_param;   //update parameter from DEFAULT_WG_INDEX
    
/* may dynamic change by workgroup 0,1 */    
    int     src_serial[2]; //seriail num for bitmap to be src in graph
    int     dst_serial[2]; //seriail num for bitmap to be dest in graph
    int     type[2];       //ENTRY_NORMAL=0,ENTRY_FIRST=1,ENTRY_LAST=2
    int     flow[2];
    unsigned int count[2]; //job count for flow rate control
    struct v_property_t param[2][MAX_PROPERTY];

    void    *reserved_in_buf;   //reserved buffer for repeat buffer, need use with FREE_BY_PUT
    void    *reserved_in_header;  //reserved driver info

    int     disable_mode[2];
    int     disable_mode_setup; //set to disable_mode after graph start, -1 means no setup
    int     freeway[2];      //able to free upper buffers/jobs
    int     freeway_setup; //set to freeway after graph start, -1 means no setup
    
    int     need_stop;    //0:no need  1: need do stop  2: stoped (take graph out to indicate entity stop)
    void    *job_mempool;
    
    void	*in_queue[2];			//out queue for entity
    void	*out_queue[2];			//out queue for entity
    
    int		out_queue_index[2];		//indicate out_index

    int     debug_file_direction; //1:IN  2:OUT
    int     debug_pause_function;   //1:pause, now noly for ENTRY_FIRST
    int     debug_max;//temp
    unsigned int statistics_finished; //finished job callback counts
    unsigned int statistics_failed; //failed job callback counts
    unsigned int statistics_dummy; //dummy job callback counts
    unsigned int statistics_max;    //max process time
    unsigned int statistics_aver;  //average process time
    unsigned int statistics_cleared;
    unsigned int statistics_missorder;
    unsigned int statistics_flush;
    
    struct video_entity_record_t    *rec;
    int     clear_out_buf_method;
    int     clear_out_buf_enable;
    int     clear_out_buf_xres;
    int     clear_out_buf_yres;
    int     clear_out_buf_x;
    int     clear_out_buf_y;
    int     clear_out_buf_w;
    int     clear_out_buf_h;
    int     clear_out_buf_p;
    unsigned int repeat_buf_bitmap;
    unsigned int repeat_buf_in_idx;
    struct v_graph_t    *graph_src[2]; //entity to be src of graph
    struct  list_head   pos_list[2]; //speed up first entity search
    int     swap_put;       //< wait swap=1 job return to prevent set_property interference
    unsigned int frame_time;
    unsigned int src_trunk_bitmap[2];   //to indicate the trunk bitmap
    struct  list_head   list2;  //for speedup entity search
    int     reserved3[29];   		//reserved
};


   
struct v_graph_t {
    struct video_entity_t   *src;
    int                     src_flow;
    struct video_entity_t   *dst;
    int                     dst_flow;
    struct v_queue_t        *q;
    struct list_head        list;   //link from graph_head
    int                     gfd;
    int                     group_id;   //graph group
    struct list_head        ungroup_list;
    int                     reserved[7];
};


#define FLAG_BUFFER_SKIP 0x55997755

struct v_buffer_t {
    int             index;
    unsigned int    addr_va;
    unsigned int    addr_pa;
    int             size;       //buffer size
    int             type;       //0:normal 1:direct 2:duplicate 3:duped
    int             workg;      //workgroup 0,1
    unsigned int    in_bitmap_value[2];     //totoal entity bitmap from graph
    unsigned int    in_bitmap_used;      //notify which entry is going to do job
    unsigned int    in_bitmap_done;      //notify which entry is going to do job
                                         //for input node use, need to sync all IN ready, let out go
    unsigned int    out_bitmap_value[2];    //totoal entity bitmap from graph
    unsigned int    out_bitmap_used;     //notify which entry has been put in
    unsigned int    out_bitmap_done;     //notify which out entry is done, used to free this buffer
    
    unsigned int    dup_bitmap_value[2];  //for dup buffer bitmap usage (TYPE_DUP)
    unsigned int    dup_bitmap_used;      //for 
    unsigned int    dup_bitmap_done;      //indicate dup buffer done, only use for static dup buffer

    struct list_head    used_list;
    unsigned int    in_nodirty_bitmap;
    unsigned int    remap_offset;
    int             debug_owner;

/*
in_job_head:
    add: after job find output buffer with get_next_buf()
    del: In free_job()
    init: free_buffer() with all bitmap ready
    
out_job_head:
    add: after create_and_handle_job() to allocate new output job
    del: free_buf() without all bitmap ready
    init: free_buf() when all bitmap ready

job destroy: free_job
 */    
    struct list_head    in_job_head;
    struct list_head    out_job_head;
    struct list_head    list;   //dynamic duplicate list
    
    struct v_buffer_t   *refer; //pointer to first normal buffer by duped buffer
    void                *q;     //belong to (struct v_graph_t), 0 means dynamic duplicated
//dup_index for dynamic duplicate buffer, allocated from duped buffer dup_bitmap
//support most 32 dup queue buffer, see get_next_dup_index()
    int                 dup_index[2];  //the index of dup_bitmap_value/dup_bitmap_used
    unsigned int        remap;
    unsigned int        job_out_header; //drvinfo to record to be job out_header
    unsigned int        entity_out_header; //drvinfo to record to be entity out_header
    unsigned int        cmd_flag;   //FLAG_BUFFER_SKIP
    int                 job_diff_wg; //default=0,indicate the job has different workg to buf
    struct video_entity_t   *reserved_for;
};


/* video buffer tag */
#define VIDEOGRAPH_TAG  0x41715454
#define VIDEOGRAPH_LEN  128  //128 bytes
#define MAX_VBI_CH  32
struct v_graph_info {
    unsigned int tag;
    unsigned int drvinfo_offset;
    unsigned int drvinfo_length;
    unsigned int itime;
    unsigned int otime;
    struct timeval first_tv; //8 bytes, mark for first entity's timstamp 
    int flow_update;  //0:no change 25: 25fps  30: 30fps
    unsigned int designed_output_fd;
//<36 bytes
    int final_src_flow; //0:dont care flow rate
    int final_dst_flow; //0:dont care flow rate
    unsigned int first_jiffies;
};


#define MAX_SPLIT   16  //1MB x 16

struct v_resource_t {
    int             split_num;
    unsigned int    start_va[MAX_SPLIT];
    unsigned int    start_pa[MAX_SPLIT];
    int             split_size;
    unsigned int    priv[MAX_SPLIT];
};


struct v_qshare_t {
    char                name[MAX_NAME_SIZE];
    int                 num;
    int                 elem_size;
    int                 type;
    int                 use_count;
    int                 actual_count;
    struct v_resource_t res;
    struct v_buffer_t   *buf[MAX_ELEM_SIZE];
    struct list_head    list;
    struct list_head    unused_head;
};

struct v_queue_t {
    char        name[MAX_NAME_SIZE];
    struct v_resource_t res;
    int         type;
    int         num;
    int         elem_size;
    int         start_idx;   //search idx
    unsigned int src_serial[2];  //src serial number for bitmap
    unsigned int dst_serial[2];  //dst serial number for bitmap
    struct v_buffer_t *buf[MAX_ELEM_SIZE];
    struct list_head  list;
    struct v_queue_t  *refer; //use for static dup
    int         use;
    int         qflow[2];
    unsigned int count[2];
    unsigned int pattern;  //for display
    int         start_idx_max;  //for build_new
    int         workg;
    struct list_head  unused_head;
    struct list_head  used_head;
    int         group_id;
    int         qtype[2];
    int         max_graph[2];
    struct v_qshare_t *qshare;
    unsigned int    dup_index[2];
    unsigned int    in_bitmap_value[2];  //totoal entity bitmap from graph
    unsigned int    out_bitmap_value[2]; //totoal entity bitmap from graph
    unsigned int    dup_bitmap_value[2]; //totoal entity bitmap from graph
    unsigned int    in_nodirty_bitmap;
    unsigned int    buffer_out_sync_threshold;  //0:disable value:sync threshold
    struct v_graph_t *graph[2][MAX_GRAPH];
    int         max_buf_number[2];  //used by qshare for max buffer number
    int         trunk_bitmap_use[2];
    int         keep_buffer[2];

#define THRESHOLD_AUTO      -1
#define THRESHOLD_DISABLE   0
    int         max_threshold; //0:disable -1:auto N:max threshold
};

#define IN_PROCESS_LIST     0
#define IN_HANDLE_LIST      1
#define IN_UNHANDLE_LIST    2

#define CMD_DUMMY           2   //map to JOB_STATUS_DUMMY
#define CMD_TRANSLATION     3   //status translation

#define PRIORITY_HIGH       0
#define PRIORITY_NORMAL     1

#define JOB_SPECIAL_SHOOT   0x56795566

struct v_job_t {
/* video entity use */
    struct v_property_t param[MAX_PROPERTY];
    struct video_entity_t   *entity;
    int                     (*callback) (struct v_job_t *,int status);
    int                     swap;        //0:normal case  1:swapping to new graph after this job
    struct v_buffer_t       *in_buf;
    struct v_buffer_t       *out_buf;
    struct v_buffer_t       *in_header;  //pointer of driver info block address, not support N to 1
    struct v_buffer_t       *out_header; //Allocate Out, Out pass to IN, Free IN
    int                     cmd;
    int                     priority;   //0:high 1:normal
    unsigned int            job_special;
    int                     (*shoot_callback)(int status);
    int                     (*shoot_callback2)(int id,int status);
    int                     reserved1[6];
    
/* internal use */
    struct list_head    buf_out_job_list;    //add to v_buffer_t->out_job_head
    struct list_head    buf_in_job_list;   //add to v_buffer_t->in_job_head
    struct list_head    list;           //unhandle/handle job list by unhandle_head/handle_head
    unsigned int        id; //+356, DVR dec_main can use *(int *)(data->bs.p_job+356)
    int                 workg;
    int                 callbacked;     //has callbacked
    unsigned int        itime;      //**** may reference by video entity
    unsigned int        otime;
    int                 status;     //record job return value
    int                 in_list;    //0:no 1:handled list 2:unhandle list
    int                 user_parameter1;
    unsigned int        user_parameter2;
};

#define TIMEOUT_FACTOR          3000    //ms
#define MAX_BUFFER_TIMEOUT      300     //ms

#define FUNCTION_NULL           0
#define FUNCTION_FLOW_UPDATE    1

#define MAX_FLOW_VALUE          65535

struct v_graph_group_t {
    int     group_id;   //graph group
    int     used;       //0: not used
    int     wg_now;
    int     wg_new;
    int     graph_state;
    int     function_state; //0: normal, 1:update
    int     max_use;
    unsigned int restart_tick;
    struct list_head handle_head;   //by order handle
    struct list_head skip_head;     //not by order handle
    struct v_graph_t *graph[2][MAX_GRAPH];
    int     src_min;
    int     reverse_scheduler;
    unsigned int checking_timestamp;
    unsigned int handle_first_timestamp;
    unsigned int handle_last_timestamp;
    unsigned int prev_sche_timestamp;
};


struct work_graph_t {
    struct v_graph_t *graph[MAX_GRAPH];
    int max_used;
};

/* queue & buffer type */
#define TYPE_NORMAL     0
#define TYPE_DUP        1   //support dyn dup with previous queue, dynamic dup by buf->q=0
#define TYPE_DYN_DUP    2   //only for dynamic dup buffer
//#define TYPE_DYN_DUPED  3   //only for dynamic duped buffer (merge to TYPE_NORMAL)
#define TYPE_RESERVED   4   //TYPE_RESERVED is one kind of TYPE_DUP
#define TYPE_SHARED     5   //this is used to shared queue
#define TYPE_DRVINFO    6   //use for driver info

#define PATTERN_NONE  0x98654311
#define PATTERN_CACHE 0x98654312

#define METHOD_SKIP     0x911


#define MAX_TRUNK_LIST_NUM      32 //max trunk list item
#define MAX_TRUNK_TARGET_NUM    5  //max target item
struct graph_trunk_t {
    int used;
    struct video_entity_t *first_entity;
    struct video_entity_t *last_entity;
    unsigned int target_use_idx;    //indixate target[] used number
    struct video_entity_t *target[MAX_TRUNK_TARGET_NUM];
    unsigned int bitmap;
};


/* manager API */
int query_propertyid(int fd,char *name);
int set_property(int fd,unsigned int id,unsigned int value,int direct);
unsigned int get_property(int fd,unsigned int id,int direct);
struct v_queue_t *video_create_queue(int type,int num,int elem_size,char name[],struct v_queue_t *refer,int ddr,unsigned int pattern);
int video_rearrange_queue(struct v_queue_t *q,int num,int elem_size);
int video_destroy_queue(struct v_queue_t *q);
struct v_qshare_t *video_create_qshare(int num,int elem_size,char name[],int ddr,int reserved);
int video_destroy_qshare(struct v_qshare_t *qshare);
int video_add_to_qshare(struct v_queue_t *q,struct v_qshare_t *qshare);
int video_remove_from_qshare(struct v_queue_t *q);
int video_setgraph(int src_fd,int src_flow,int dst_fd,int dst_flow,struct v_queue_t *q); //return gfd
int video_update_flow(int src_fd,int src_flow,int dst_fd,int dst_flow);
int video_get_flow(int fd);
void video_apply_flow(void);
int video_startgraph(void);
int video_cleargraph(int gfd);//video restart
int video_setfree(int fd,int freeway,int param); //0:normal 1:free by lower job 2:free by putjob
int video_register_preprocess(int entity_fd,int (*func)(void *,void *));
int video_register_postprocess(int entity_fd,int (*func)(void *,void *));
int video_set_disable_mode(int entity_fd,int mode);
int video_get_buffer_depth(int fd);
int video_set_buffer_out_sync(struct v_queue_t *q,int parameter,int method);
int video_shoot_job(int fd,unsigned int in_pa,unsigned int in_va,unsigned int out_pa,
    unsigned int out_va,unsigned int drvinfo_va,int param,int (*callback)(int));
int video_shoot_job2(int fd,
    unsigned int in_pa,unsigned int in_size,
    unsigned int out_pa,unsigned int out_size,
    unsigned int in_drvinfo_pa,unsigned int out_drvinfo_pa,
    int param,int (*callback)(int));    

int video_shoot_job3(int fd,int seq_id,
    unsigned int in_pa,unsigned int in_size,
    unsigned int out_pa,unsigned int out_size,
    unsigned int in_drvinfo_pa,unsigned int out_drvinfo_pa,
    void *param,int (*callback)(int,int));
    
/*
video_suspend:
   1.Enter SUSPEND state (it will transfer to RESTART state, scheduler will not handle if SUSPEND mode)
     RESTART mode will transfer to RUNNING mode if all buffer is transfered.
   2.DVR layer will prepare graph switching (setgraph, setproperty...) and call startgraph()

video_stop:
   1.Enter STOP state
   2.send all entity stop()
   3.wait all job done
   4.DVR layer can startgraph() to start it
 */
int video_stopgraph(void);  //all graph information will be free, wait all finished
int video_suspend(void);    //start to build new graph information, the old will be free in the next time

/* video entity API */
int video_entity_register(struct video_entity_t *);    //return fd;
int video_entity_deregister(struct video_entity_t *entity);
int video_flush_input(struct video_entity_t *);
int video_preprocess(struct v_job_t *job,void *param);
int video_postprocess(struct v_job_t *job,void *param);

int video_remap_pfn_range(struct v_queue_t *q,struct vm_area_struct *vma,unsigned long addr,pgprot_t prot);
int video_set_queue_threshold(struct v_queue_t *q,int threshold);
                    
int video_get_queue_number(struct v_buffer_t *buf);
int video_get_buffer_remap_offset(struct v_buffer_t *buf);
int video_get_buffer_size(struct v_buffer_t *buf);
int video_buf_arrange_size(struct v_queue_t *q);
int video_set_src_nodirty(int fd,int nodirty,int disabled);
void video_clear_out_buf_by_dummy(int fd,int xres,int yres,int x,int y,int w,int h,unsigned int p);
void video_clear_out_buf(int fd,int xres,int yres,int x,int y,int w,int h,unsigned int p);
char *video_get_name_by_fd(int fd);
int video_get_input_number(char *qname);
struct v_buffer_t *video_get_used_buf_by_queue(struct v_queue_t *q);

void video_time_init(void);
void video_time_close(void);
unsigned int video_gettime_ms(void);
unsigned int video_max_time(void);
unsigned int video_gettime_us(void);
void printm(const char *fmt, ...);
void printm_with_panic(char *ptr);
void print_debug(void);
void enter_panic(int status);

void print_buf_used_by_group(int group_id,int function,int debug,int skip_drvinfo);
void print_unhandle_list(int function,int debug);
void print_buffer_dup(int,int);

#include "drvinfo.h"
#include "irqlock_detection.h"

#endif
