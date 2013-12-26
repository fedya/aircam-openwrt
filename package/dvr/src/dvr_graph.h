#ifndef __DVR_GRAPH_H__
#define __DVR_GRAPH_H__

#include <linux/slab.h>
#include <linux/list.h>
#include "videograph_api.h"
#include "dvr_type_define.h"


#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif


#define GRAPH_STATE_STOP        0
#define GRAPH_STATE_RUN         1
#define GRAPH_STATE_SUSPEND     2

#define G_KEEP             0x0  /*default*/
#define G_SET_ONCE         0x1
#define G_REPLACE          0x2

//Graph Line state
#define GFL_DONE            0
#define GFL_NEW             1
#define GFL_DEL             2
#define GFL_DEL_AND_RESTART 3
#define GFL_UPDATE          4
#define GFL_ONLY_DEL_TAG    5


// Line data
typedef struct dvr_graph_line_data_tag{
    // set_property() arguments 
    int src_fd;
    int src_flow;
    int dst_fd;
    int dst_flow;
    struct v_queue_t *que;
    //for application state control
    FuncTag tag;
    int gfd;
    int state;
    int cnt;
}dvr_graph_line_data;

typedef struct dvr_graph_line_tag {
    struct list_head    list;
    int   status;
    dvr_graph_line_data data;
}dvr_graph_line;


// Property data
typedef struct dvr_graph_prop_data_tag{
    // video_setgraph() arguments 
    int fd;
    unsigned int id;
    unsigned int value;
    int direct;
    //for application state control
    FuncTag tag;
    int flag;
}dvr_graph_prop_data;

typedef struct dvr_graph_prop_tag {
    struct list_head    list;
    dvr_graph_prop_data data;
}dvr_graph_prop;


// One Graph 
typedef struct dvr_graph_tag {
    struct list_head    list;   // this links all graphs in this system

    // private data
    int id;
    struct kmem_cache *line_data_cache;
    struct kmem_cache *prop_data_cache;
    struct list_head line_entry;
    struct list_head prop_entry;
    unsigned char linecache_str[16], propcache_str[16];

}dvr_graph;


typedef struct dvr_queue_data_tag{
    int type;
    int num;
    int elem_size;
    char name[MAX_NAME_SIZE];
    struct v_queue_t *refer;
    int ddr;
    unsigned int pattern;
}dvr_queue_data;


typedef struct dvr_queue_tag {
    struct list_head    list;

    // private data
    dvr_queue_data data;
    struct v_queue_t *queue;
}dvr_queue;


/* all other VG functions */
typedef struct dvr_vgfn_rearrange_queue_tag{
    struct v_queue_t *q;
    int num;
    int elem_size;
    FuncTag tag;
}dvr_vgfn_rearrange_queue;

typedef struct dvr_vgfn_setfreetype_tag{
    int fd;
    int freeway;
    int param;
    FuncTag tag;
}dvr_vgfn_setfreetype;


typedef enum dvr_vgfn_type_tag {
    VGFN_REARRANGE_QUEUE = 0,
    VGFN_VG_SETFREE,
    VGFN_COUNT
}dvr_vgfn_type;


typedef struct dvr_vgfn_tag {
    struct list_head    list;

    dvr_vgfn_type   fn_type;
    union
    {
        dvr_vgfn_rearrange_queue    rearrange_queue;
        dvr_vgfn_setfreetype        set_vg_freetype;
    }data;

}dvr_vgfn;


#define INFO_INDEX_DISP_COUNT       0
#define INFO_SET_BUF_FLOW_COUNT     1
#define INFO_CHECK_BUF_FLOW_PAIR    2
#define INFO_RESET_BUF_FLOW_IN_OUT  3
#define INFO_SET_FLOW_TIMEOUT       4
typedef struct dvr_info_disp_count_tag
{
    int lcd_count;
    int plane_count;
    int dup_disp;
}dvr_info_disp_count;

typedef struct buf_flow_count_tag
{
    int count;
    int in_out; 
    int timeout;
}buf_flow_count_t;

typedef struct dvr_info_buf_flow_count_tag
{
    buf_flow_count_t    rec[DVR_RECORD_CHANNEL_NUM][DVR_ENC_REPD_BT_NUM+1];
    buf_flow_count_t    pb[DVR_PLAYBACK_CHANNEL_NUM];
}dvr_info_buf_flow_count;

#define BFI_FNC_REC(ch,repd)   (0x010000|(repd<<8)|ch)
#define BFI_FNC_PB(ch)         (0x020000|ch)
#define BFI_GET_FNC(val)       (val&0xFF0000)
#define BFI_GET_CH(val)        (val&0x0000FF)
#define BFI_GET_REPD(val)      ((val&0x00FF00)>>8)
typedef struct dvr_info_buf_flow_info_tag
{
    int fnc;
    int in_out; //get(+1), put(-1)
}dvr_info_buf_flow_info;

typedef struct dvr_common_data_tag
{
    int     is_used;
    struct semaphore    dvr_common_sem;
    dvr_info_disp_count disp_cnt_info;
    dvr_info_buf_flow_count buf_flow_cnt;

    //last user call api
    FuncTag la_APPLY;
    
}dvr_common_data;

typedef struct dvr_swap_notify_info_tag
{
    struct list_head    list;
    void (*callback)(void *priv);
    void *private; 
}dvr_swap_notify_info;

extern void dvr_common_initialize(void);
extern void dvr_common_terminate(void);
extern int dvr_graph_create(dvr_graph **graph, int graph_id);
extern int dvr_graph_destroy(dvr_graph *graph);
int dvr_register_preprocess(int entity_fd,int (*func)(void *,void *));
int dvr_register_postprocess(int entity_fd,int (*func)(void *,void *));
extern int dvr_graph_add_link(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow, struct v_queue_t *que);
extern int dvr_graph_update_flow_list(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow);
extern void dvr_graph_set_flow_flag(void);
extern void dvr_graph_apply_flow(void);
extern int dvr_graph_update_flow(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow);
extern int dvr_get_flow(int);
extern int dvr_graph_del_link(dvr_graph *graph, FuncTag *tag);
extern int dvr_graph_set_property_directly(int fd, unsigned int id, unsigned int value, int direct);
extern int dvr_graph_set_property(dvr_graph *graph, FuncTag *tag, int fd, unsigned int id, unsigned int value, int direct);
extern unsigned int dvr_graph_get_property(dvr_graph *graph, int fd, unsigned int id, int direct);
extern int dvr_graph_write_property(dvr_graph *graph, FuncTag *tag);
extern int dvr_graph_write_graphline(dvr_graph *graph, FuncTag *tag_mask);
extern int dvr_graph_apply_property(dvr_graph *graph, FuncTag *tag);
extern int dvr_graph_run(dvr_graph *graph);
extern int dvr_graph_apply_by_tag(FuncTag *tag_mask);
extern dvr_graph* dvr_graph_find_by_id(int graph_id);
extern int dvr_graph_setfree(FuncTag *tag, int fd,int freeway,int param);
extern int PROPID(int fd,char *name);
extern struct v_queue_t *dvr_queue_allocate(int type,int num,int elem_size,char name[], struct v_queue_t *refer,int ddr,unsigned int pattern);
//++ fullhd
//extern int dvr_set_queue_threshold(struct v_queue_t *q, int threshold);
// -- fullhd
extern int dvr_queue_rearrange(FuncTag *tag, struct v_queue_t *q,int num,int elem_size, int direct);
extern int dvr_queue_destroy(struct v_queue_t *q);
extern struct v_queue_t* dvr_queue_find_by_name(char name[], int *size, int *count, int *ddr);
extern void register_swap_notify(dvr_swap_notify_info *pSwapNotify);
extern void unregister_swap_notify(dvr_swap_notify_info *pSwapNotify);
extern int dvr_common_store_info(int id, void *data);
extern int dvr_common_retrieve_info(int id, void *data);
extern void print_user_last_api_data(unsigned char *msg, int api_num, int length, unsigned char *data, int is_input);
extern struct proc_dir_entry *DvrProcRoot;

#endif /* __DVR_GRAPH_H__ */


