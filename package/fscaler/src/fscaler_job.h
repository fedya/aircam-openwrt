/**
 * @file fscaler_job.h
 * @brief Videograph device routines header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_JOB_H_
#define _FSCALER_JOB_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
union yuv_buf;
struct color_buf;
struct fscaler_subjob;
struct fscaler_job;
struct fscaler_table;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_table.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO
#define FSCALER_JOB_LAYOUT_Y422       0 ///< Y422 sequence layout
#define FSCALER_JOB_LAYOUT_H2642D     1 ///< I420 M0 MB16x16 y-uv 2 buffers layout
#define FSCALER_JOB_LAYOUT_MPEG42D    2 ///< I420 M0 MB16x16 y-u-v 3 buffers layout

#define FSCALER_JOB_ORDER_CRYCB       0 ///< Color order CrYCb
#define FSCALER_JOB_ORDER_CBYCR       1 ///< Color order CbYCr
#define FSCALER_JOB_ORDER_YCRCB       2 ///< Color order YCrCb
#define FSCALER_JOB_ORDER_YCBCR       3 ///< Color order YCbCr

#define FSCALER_JOB_TYPE_SINGLE       0 ///< Scaler single job type, do normal scale
#define FSCALER_JOB_TYPE_TWICE        1 ///< Scaler twice job type, do 60i/Top/Bottom composited scale
#define FSCALER_JOB_TYPE_TRIPLE       2 ///< Scaler twice job type, do 60i/Top/Bottom composited scale
#define FSCALER_JOB_TYPE_PASS         3 ///< Scaler twice job type, pass this job
#define FSCALER_JOB_TYPE_DROP         4 ///< Scaler drop job type, drop this job

#define FSCALER_JOB_STATUS_IDLE      0  ///< Scaler job status idle
#define FSCALER_JOB_STATUS_ONPROC    1  ///< Scaler job status on-proc
#define FSCALER_JOB_STATUS_DONE      2  ///< Scaler job status done
#define FSCALER_JOB_STATUS_ERROR     3  ///< Scaler job ststus error
#define FSCALER_JOB_STATUS_ABORT     4  ///< Scaler job status abort

/* Priority range is -19 ~ 20, normal priority is 0 */
#define FSCALER_JOB_PRIO_HIGHEST     (-19)      ///< Highest priority
#define FSCALER_JOB_PRIO_NORMAL      0  ///< Normal priority
#define FSCALER_JOB_PRIO_LOWEST      20 ///< Lowest priority

#define COLOR_BUF_CHECK_LAYOUT(p, l)   \
    ((p)->layout == (l))
#define COLOR_BUF_IS_YC_SWAP(p)  \
    ((p)->order == FSCALER_JOB_ORDER_YCRCB || (p)->order == FSCALER_JOB_ORDER_YCRCB)
#define COLOR_BUF_IS_RB_SWAP(p)  \
    ((p)->order == FSCALER_JOB_ORDER_CBYCR || (p)->order == FSCALER_JOB_ORDER_YCBCR)

/* If __REDUCE_DMA_BUS_LOADING is defined, use the trick to reduce DMA
 * laoding caused by source memory size */
#define __REDUCE_DMA_BUS_LOADING

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
union yuv_buf {
    unsigned int yuv[1];        ///< One YUV buffer
    unsigned int y_u_v[3];      ///< Three split YUV buffers
    unsigned int y_uv[2];       ///< Two split YUV buffers
};

struct color_buf {
    int layout;                 ///< Color memory layout, Y422/MPEG42D/H2642D
    int order;                  ///< Color order, YCbCr/YCrCb/CbYCr/CrYCb
    int yuv_n;                  ///< Number of YUV buffer sets of scaler, max = 2
    int sample_down;            ///< Memory sample_down factor, real sample_down = ptch * pixel bytes of source
    union yuv_buf yuv[2];       ///< YUV buffer
};

struct fscaler_subjob {
    int index;                  ///< Scaler sub-job index
    int in_bg_w;                ///< Input background width
    int in_bg_h;                ///< Input background height
    int in_x;                   ///< Input x
    int in_y;                   ///< Input y
    int in_w;                   ///< Input width
    int in_h;                   ///< Input height
    struct color_buf in_buf;    ///< Input color buffer
    int out_bg_w;               ///< Output background width
    int out_bg_h;               ///< Output background height
    int out_x;                  ///< Output x
    int out_y;                  ///< Output y
    int out_w;                  ///< Output width
    int out_h;                  ///< Output height
    struct color_buf out_buf;   ///< Output color buffer
};

struct fscaler_job {
    struct fscaler_table *parent;
    struct klist_node knode_table;
    struct klist_node knode_ent;
    struct klist_node knode_dev;
    void (*done) (struct fscaler_job *);
    void (*abort) (struct fscaler_job *);
    void (*error) (struct fscaler_job *);
    void (*destruct) (struct fscaler_job *);
    int fd;
    int id;
    int curr;
    int max;
    int type;
    int status;
    int prio;
    int retry;
    unsigned int bonus;
    unsigned int start_time;
    unsigned int end_time;
    unsigned long data;
    struct fscaler_subjob *group[FSCDRV_JOBGROUP_MAX];
    int m;
    const char *magic[16];
};

///////////////////////////////////////////////////////////////////////////////
// EXTERN PROTOTYPE
extern int fscaler_job_table_request(struct fscaler_driver *drv);
extern void fscaler_job_table_remove(struct fscaler_driver *drv);
extern const char *fscaler_job_table_name(void);
extern struct fscaler_job *fscaler_job_request(struct fscaler_table *table, int fd, int type,
                                               int max);
extern void fscaler_job_remove(struct fscaler_table *table, struct fscaler_job *job);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline void fscaler_job_set_magic(struct fscaler_job *job, const char *expr)
{
    if (job) {
        job->m = (job->m + 1) % ARRAY_SIZE(job->magic);
        job->magic[job->m] = expr;
    }
}

static inline struct fscaler_table *fscaler_job_to_table(struct fscaler_job *job)
{
    return job ? job->parent : NULL;
}

static inline void fscaler_job_callback_register(struct fscaler_job *job,
                                                 void (*done) (struct fscaler_job *),
                                                 void (*abort) (struct fscaler_job *),
                                                 void (*error) (struct fscaler_job *),
                                                 void (*destruct) (struct fscaler_job *))
{
    if (job) {
        job->done = done;
        job->abort = abort;
        job->error = error;
        job->destruct = destruct;
    }
}

static inline int fscaler_job_check_finished(struct fscaler_job *job)
{
    if (job) {
        M_DEVELOPP("SCL: job cur:%d(%d)\n", job->curr, job->max);
        if (job->curr + 1 >= job->max) {
            return 1;
        }
    }
    return 0;
}

static inline void fscaler_job_to_next_subjob(struct fscaler_job *job)
{
    if (job) {
        ++job->curr;
    }
}

static inline struct fscaler_subjob *fscaler_job_get_subjob(struct fscaler_job *job, int index)
{
    if (job) {
        if (index >= 0 && index < job->max) {
            return job->group[index];
        }
    }
    return NULL;
}

static inline void fscaler_job_set_timestamp(struct fscaler_job *job, unsigned int start,
                                             unsigned int end)
{
    if (job) {
        job->start_time = start;
        job->end_time = end;
    }
}

static inline unsigned int fscaler_job_start_time(struct fscaler_job *job)
{
    return job ? job->start_time : (unsigned int)-1;
}

static inline unsigned int fscaler_job_end_time(struct fscaler_job *job)
{
    return job ? job->end_time : (unsigned int)-1;
}

static inline void fscaler_job_set_priority(struct fscaler_job *job, int prio, unsigned int bonus)
{
    if (job) {
        job->prio = prio;
        if (job->prio < -19) {
            job->prio = -19;
        } else if (job->prio > 20) {
            job->prio = 20;
        }
        job->bonus = bonus;
    }
}

static inline void fscaler_job_set_private_data(struct fscaler_job *job, unsigned long data)
{
    if (job) {
        job->data = data;
    }
}

static inline unsigned long fscaler_job_get_private_data(struct fscaler_job *job)
{
    if (job) {
        return job->data;
    }
    return 0;
}

static inline int fscaler_format_to_layout(unsigned char fmt)
{
    int layout = -1;
    switch (fmt) {
    case 2:
        layout = FSCALER_JOB_LAYOUT_H2642D;
        break;
    case 5:
        layout = FSCALER_JOB_LAYOUT_Y422;
        break;
    case 6:
        layout = FSCALER_JOB_LAYOUT_MPEG42D;
        break;
    default:
        break;
    }
    M_DEVELOPP("SCL: fmt:%d, layout:%d\n", fmt, layout);
    return layout;
}

static inline int fscaler_subjob_check_source_layout(struct fscaler_subjob *sjob)
{
    int yesno = 0;
    if (sjob) {
        switch (sjob->in_buf.layout) {
        case FSCALER_JOB_LAYOUT_Y422:
            yesno = 1;
            break;
        default:
            break;
        }
    }
    return yesno;
}

static inline int fscaler_subjob_check_target_layout(struct fscaler_subjob *sjob)
{
    int yesno = 0;
    if (sjob) {
        switch (sjob->out_buf.layout) {
        case FSCALER_JOB_LAYOUT_Y422:
        case FSCALER_JOB_LAYOUT_H2642D:
        case FSCALER_JOB_LAYOUT_MPEG42D:
            yesno = 1;
            break;
        default:
            break;
        }
    }
    return yesno;
}

static inline int fscaler_subjob_check_source_param(struct fscaler_subjob *sjob,
                                                    unsigned int vp_width, unsigned int vp_height,
                                                    unsigned int swc_width, unsigned int swc_height)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == vp_width)) {
        printk("SCL: vp_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == vp_height)) {
        printk("SCL: vp_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == swc_width)) {
        printk("SCL: swc_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == swc_height)) {
        printk("SCL: swc_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(vp_width < swc_width)) {
        printk("SCL: vp_width:%d is smaller than swc_width:%d\n", vp_width, swc_width);
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(vp_height < swc_height)) {
        printk("SCL: vp_height:%d is smaller than swc_height:%d\n", vp_height, swc_height);
        return FSCALER_ERR_FAIL;
    }
    return FSCALER_OK;
}

static inline int fscaler_subjob_set_source_param(struct fscaler_subjob *sjob,
                                                  unsigned int vp_width, unsigned int vp_height,
                                                  unsigned int target_width,
                                                  unsigned int target_height,
                                                  unsigned int swc_axis_x, unsigned int swc_axis_y,
                                                  unsigned int swc_width, unsigned int swc_height,
                                                  unsigned int crop_axis_x,
                                                  unsigned int crop_axis_y,
                                                  unsigned int crop_width,
                                                  unsigned int crop_height,
                                                  unsigned int src_swap_y,
                                                  unsigned int src_swap_cbcr, unsigned int src_fmt,
                                                  unsigned int src_sample_down)
{
    /*
     * in_bg_w = vp_width
     * in_bg_h = vp_height
     * in_x = swc_axis_x + (crop_axis_x * swc_width / target_width)
     * in_y = swc_axis_y + (crop_axis_y * swc_height / target_height)
     * in_w = swc_width * crop_width / target_width
     * in_h = swc_height * crop_height / target_height
     */
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    sjob->in_bg_w = vp_width;
    sjob->in_bg_h = vp_height;
#ifdef __REDUCE_DMA_BUS_LOADING
    sjob->in_x = 0;
    sjob->in_y = 0;
#else
    sjob->in_x = swc_axis_x + crop_axis_x * swc_width / target_width;
    sjob->in_y = swc_axis_y + crop_axis_y * swc_height / target_height;
#endif /* __REDUCE_DMA_BUS_LOADING */
    sjob->in_w = swc_width * crop_width / target_width;
    sjob->in_h = ((swc_height * crop_height / target_height) / src_sample_down) / 2 * 2;
    sjob->in_buf.order = (src_swap_y << 1 | src_swap_cbcr);
    sjob->in_buf.yuv_n = 1;
    sjob->in_buf.layout = fscaler_format_to_layout(src_fmt);
    sjob->in_buf.sample_down = src_sample_down;
    return FSCALER_OK;
}

static inline int fscaler_subjob_check_target_param(struct fscaler_subjob *sjob,
                                                    unsigned int target_width,
                                                    unsigned int target_height,
                                                    unsigned int crop_width,
                                                    unsigned int crop_height)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == target_width)) {
        printk("SCL: target_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == target_height)) {
        printk("SCL: target_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == crop_width)) {
        printk("SCL: crop_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == crop_height)) {
        printk("SCL: crop_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(target_width < crop_width)) {
        printk("SCL: target_width:%d is smaller than crop_width:%d\n", target_width, crop_width);
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(target_height < crop_height)) {
        printk("SCL: target_height:%d is smaller than crop_height:%d\n", target_height,
               crop_height);
        return FSCALER_ERR_FAIL;
    }
    return FSCALER_OK;
}

static inline int fscaler_subjob_set_target_param(struct fscaler_subjob *sjob,
                                                  unsigned int des_axis_x, unsigned int des_axis_y,
                                                  unsigned int des_width, unsigned int des_height,
                                                  unsigned int crop_axis_x,
                                                  unsigned int crop_axis_y,
                                                  unsigned int crop_width,
                                                  unsigned int crop_height,
                                                  unsigned int des_swap_y,
                                                  unsigned int des_swap_cbcr, unsigned int des_fmt,
                                                  unsigned int des_sample_down)
{
    /*
     * out_bg_w = des_width
     * out_bg_h = des_height
     * out_x = des_axis_x + crop_axis_x
     * out_y = des_axis_y + crop_axis_y
     * out_w = crop_width
     * out_h = crop_height
     */
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    sjob->out_bg_w = des_width;
    sjob->out_bg_h = des_height;
    sjob->out_x = des_axis_x + crop_axis_x;
    sjob->out_y = des_axis_y + crop_axis_y;
    sjob->out_w = crop_width;
    sjob->out_h = (crop_height / des_sample_down) / 2 * 2;
    sjob->out_buf.order = (des_swap_y << 1 | (!des_swap_cbcr));
    sjob->out_buf.yuv_n = 1;
    sjob->out_buf.layout = fscaler_format_to_layout(des_fmt);
    sjob->out_buf.sample_down = des_sample_down;
    return FSCALER_OK;
}

static inline int fscaler_subjob_check_source_frame(struct fscaler_subjob *sjob,
                                                    unsigned int vp_width, unsigned int vp_height,
                                                    unsigned int swc_width, unsigned int swc_height)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == vp_width)) {
        printk("SCL: vp_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == vp_height)) {
        printk("SCL: vp_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == swc_width)) {
        printk("SCL: swc_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == swc_height)) {
        printk("SCL: swc_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(vp_width < swc_width)) {
        printk("SCL: vp_width:%d is smaller than swc_width:%d\n", vp_width, swc_width);
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(vp_height < swc_height)) {
        printk("SCL: vp_height:%d is smaller than swc_height:%d\n", vp_height, swc_height);
        return FSCALER_ERR_FAIL;
    }
    return FSCALER_OK;
}

static inline int fscaler_subjob_set_source_frame(struct fscaler_subjob *sjob,
                                                  unsigned int paddr, unsigned int buf_offset,
                                                  unsigned int vp_width, unsigned int vp_height,
                                                  unsigned int target_width,
                                                  unsigned int target_height,
                                                  unsigned int swc_axis_x, unsigned int swc_axis_y,
                                                  unsigned int swc_width, unsigned int swc_height,
                                                  unsigned int crop_axis_x,
                                                  unsigned int crop_axis_y,
                                                  unsigned int crop_width, unsigned int crop_height)
{
    if (!sjob || !paddr) {
        printk("SCL: Null scaler sub-job or 0 address\n");
        return FSCALER_ERR_FAIL;
    }
    if (sjob->in_buf.layout == FSCALER_JOB_LAYOUT_Y422) {
        if (sjob->in_buf.yuv_n > 0) {
#ifdef __REDUCE_DMA_BUS_LOADING
            sjob->in_buf.yuv[0].yuv[0] =
                paddr + buf_offset
                + 2 * vp_width * (swc_axis_y + crop_axis_y * swc_height / target_height)
                + 2 * 1 * (swc_axis_x + crop_axis_x * swc_width / target_width);
#else
            sjob->in_buf.yuv[0].yuv[0] = paddr + buf_offset;
#endif
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

static inline int fscaler_subjob_check_target_frame(struct fscaler_subjob *sjob,
                                                    unsigned int des_width,
                                                    unsigned int des_height,
                                                    unsigned int crop_width,
                                                    unsigned int crop_height)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == des_width)) {
        printk("SCL: des_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == des_height)) {
        printk("SCL: des_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == crop_width)) {
        printk("SCL: crop_width is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(0 == crop_height)) {
        printk("SCL: crop_height is 0\n");
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(des_width < crop_width)) {
        printk("SCL: des_width:%d is smaller than crop_width:%d\n", des_width, crop_width);
        return FSCALER_ERR_FAIL;
    }
    if (unlikely(des_height < crop_height)) {
        printk("SCL: des_height:%d is smaller than crop_height:%d\n", des_height, crop_height);
        return FSCALER_ERR_FAIL;
    }
    return FSCALER_OK;
}

static inline int fscaler_subjob_set_target_frame(struct fscaler_subjob *sjob,
                                                  unsigned int paddr,
                                                  unsigned int buf_offset,
                                                  unsigned int des_width, unsigned int des_height)
{
    unsigned int offset = 0;
    if (!sjob || !paddr) {
        printk("SCL: Null scaler sub-job or 0 address\n");
        return FSCALER_ERR_FAIL;
    }
    offset = des_width * des_height;
    if (sjob->out_buf.layout == FSCALER_JOB_LAYOUT_MPEG42D) {
        if (sjob->out_buf.yuv_n > 0) {
            sjob->out_buf.yuv[0].y_u_v[0] = paddr + buf_offset;
            sjob->out_buf.yuv[0].y_u_v[1] = paddr + (offset * 5 / 4) + buf_offset / 4;
            sjob->out_buf.yuv[0].y_u_v[2] = paddr + offset + buf_offset / 4;
        }
        if (sjob->out_buf.yuv_n > 1) {
            sjob->out_buf.yuv[1].y_u_v[0] = paddr + (offset * 3 / 2) + buf_offset;
            sjob->out_buf.yuv[1].y_u_v[1] = paddr + (offset * 4 / 11) + buf_offset / 4;
            sjob->out_buf.yuv[1].y_u_v[2] = paddr + (offset * 5 / 2) + buf_offset / 4;
        }
        return FSCALER_OK;
    }
    if (sjob->out_buf.layout == FSCALER_JOB_LAYOUT_Y422) {
        if (sjob->out_buf.yuv_n > 0) {
            sjob->out_buf.yuv[0].yuv[0] = paddr + buf_offset;
        }
        if (sjob->out_buf.yuv_n > 1) {
            sjob->out_buf.yuv[1].yuv[0] = paddr + (offset * 2) + buf_offset;
        }
        return FSCALER_OK;
    }
    if (sjob->out_buf.layout == FSCALER_JOB_LAYOUT_H2642D) {
        if (sjob->out_buf.yuv_n > 0) {
            sjob->out_buf.yuv[0].y_uv[0] = paddr + buf_offset;
            sjob->out_buf.yuv[0].y_uv[1] = paddr + offset + buf_offset;
        }
        if (sjob->out_buf.yuv_n > 1) {
            sjob->out_buf.yuv[1].y_uv[0] = paddr + (offset * 2) + buf_offset;
            sjob->out_buf.yuv[1].y_uv[1] = paddr + (offset * 3) + buf_offset;
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

static inline void fscaler_job_callstack(char *desc, struct fscaler_job *job)
{
    int i = 0, j = 0;
    if (desc) {
        printk("\n<Job callstack>: %s", desc);
    } else {
        printk("\n<Job callstack>:");
    }
    for (i = 0, j = 0; i < ARRAY_SIZE(job->magic); ++i) {
        if (job->magic[(job->m + i + 1) % ARRAY_SIZE(job->magic)]) {
            if (0 == j % 3) {
                printk("\n          ");
            }
            printk("-> %s ", job->magic[(job->m + i + 1) % ARRAY_SIZE(job->magic)]);
            ++j;
        }
    }
    printk("\n");
}

static inline void fscaler_job_print(char *desc, struct fscaler_job *job)
{
    int fd = -1;
    struct fscaler_subjob *sjob = NULL;
    if (job) {
        fd = job->fd;
        sjob = fscaler_job_get_subjob(job, job->curr);
        switch (sjob->out_buf.layout) {
        case FSCALER_JOB_LAYOUT_Y422:
            M_DEBUG("SCL: [%s](%d)v=%p{%x}{%x}\n", desc, fd, job, sjob->in_buf.yuv[0].yuv[0],
                    sjob->out_buf.yuv[0].yuv[0]);
            break;
        case FSCALER_JOB_LAYOUT_H2642D:
            M_DEBUG("SCL: [%s](%d)v=%p{%x}{%x,%x}\n", desc, fd, job, sjob->in_buf.yuv[0].yuv[0],
                    sjob->out_buf.yuv[0].y_uv[0], sjob->out_buf.yuv[0].y_uv[1]);
            break;

        case FSCALER_JOB_LAYOUT_MPEG42D:
            M_DEBUG("SCL: [%s](%d)v=%p{%x}{%x,%x,%x}\n", desc, fd, job, sjob->in_buf.yuv[0].yuv[0],
                    sjob->out_buf.yuv[0].y_u_v[0], sjob->out_buf.yuv[0].y_u_v[1],
                    sjob->out_buf.yuv[0].y_u_v[2]);
            break;
        default:
            break;
        }
    }
}

static inline void fscaler_job_print_verbos(char *desc, struct fscaler_job *job)
{
    int fd = -1;
    struct fscaler_subjob *sjob = NULL;
    if (job) {
        fd = job->fd;
        sjob = fscaler_job_get_subjob(job, job->curr);
        M_DEBUG("SCL: [%s](%d)v=%p(%lx){%d-%d,%d,%d,%d,%d,%d}{%d-%d,%d,%d,%d,%d,%d}\n", desc, fd,
                job, job->data, sjob->in_buf.layout, sjob->in_bg_w, sjob->in_bg_h, sjob->in_x,
                sjob->in_y, sjob->in_w, sjob->in_h, sjob->out_buf.layout, sjob->out_bg_w,
                sjob->out_bg_h, sjob->out_x, sjob->out_y, sjob->out_w, sjob->out_h);
    }
}

#endif /* _FSCALER_JOB_H_ */
