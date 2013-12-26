/**
 * @file fscaler_io.c
 * @brief Scaler driver ioctl interface implementation
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 * @todo This interface is not complete and verified. Need more work it out.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15))
#include <linux/platform_device.h>
#endif /*LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16))
#include <linux/io.h>
#endif /*LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
#include <linux/uaccess.h>
#endif /*LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28))
#ifdef FSCDRV_FRAMMAP
#include "frammap_if.h"
#endif /* FSCDRV_FRAMMAP */
#else
#ifdef FSCDRV_FRAMMAP
#include "frammap.h"
#endif /* FSCDRV_FRAMMAP */
#endif /*LINUX_VERSION_CODE */
#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"
#include "fscaler_module.h"
#include "fscaler_param.h"
#include "fscaler_version.h"
#include "fscaler_api.h"
#include "fscaler_io.h"

#ifdef FSCDRV_FRAMMAP
static struct frammap_buf_info ddr_info = { 0 };
#endif /* FSCDRV_FRAMMAP */

#define CACHE_LINE          16

typedef struct fscaler_io_private_info {
    int cmd;
#define CMD_WAKEUP   1
    int drop;
    wait_queue_head_t wq;
    fscaler_param param;
    fscaler_frame frame;
} fscaler_io_private_info_t;

fscaler_io_private_info_t fscaler_io_private_info_map[FSCFD_MAX];
int fscaler_io_interface_is_installed = 0;

static inline fscaler_io_private_info_t *fscaler_io_private_info_find(int fd)
{
    if (0 <= fd && max_entity > fd) {
        return &fscaler_io_private_info_map[fd];
    }
    return NULL;
}

static inline void fscaler_io_private_info_init(int fd)
{
    fscaler_io_private_info_t *p_info = fscaler_io_private_info_find(fd);
    if (p_info) {
        p_info->cmd = 0;
        p_info->drop = 0;
        init_waitqueue_head(&p_info->wq);
    }
}

static inline unsigned int fscaler_io_va_convert_to_pa(unsigned int addr)
{
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    pgd_t *pgd = NULL;
    pgd = pgd_offset(current->mm, addr);
    if (pgd_none(*pgd)) {
        goto ERROR_EXIT;
    }
    pmd = pmd_offset(pgd, addr);
    if (pmd_none(*pmd)) {
        goto ERROR_EXIT;
    }
    pte = pte_offset_map(pmd, addr);
    if (pmd_none(*pmd)) {
        goto ERROR_EXIT;
    }
    pte = pte_offset_map(pmd, addr);
    if (!pte_present(*pte)) {
        goto ERROR_EXIT;
    }
    return (unsigned int)page_address(pte_page(*pte)) + (addr & (PAGE_SIZE - 1)) - PAGE_OFFSET;
  ERROR_EXIT:
    return 0;
}

static inline void fscaler_io_va_to_pa(fscaler_frame * p_frame)
{
    p_frame->memsrc = fscaler_io_va_convert_to_pa(p_frame->memsrc);
    p_frame->pu8Display0_addr[0] = fscaler_io_va_convert_to_pa(p_frame->pu8Display0_addr[0]);
    p_frame->pu8Display0_addr[1] = fscaler_io_va_convert_to_pa(p_frame->pu8Display0_addr[1]);
    p_frame->pu8Display0_addr[2] = fscaler_io_va_convert_to_pa(p_frame->pu8Display0_addr[2]);
    p_frame->pu8Display1_addr[0] = fscaler_io_va_convert_to_pa(p_frame->pu8Display1_addr[0]);
    p_frame->pu8Display1_addr[1] = fscaler_io_va_convert_to_pa(p_frame->pu8Display1_addr[1]);
    p_frame->pu8Display1_addr[2] = fscaler_io_va_convert_to_pa(p_frame->pu8Display1_addr[2]);
}

static inline void fscaler_io_set_param(int fd, fscaler_param * p_param)
{
    if (p_param->disable_dispaly) {
        fscaler_param_disable_display(fd);
        fscaler_param_reset_snapshot_count(fd);
    }
    fscaler_param_set_property(fd, ID_DES_WIDTH, p_param->des_width);
    fscaler_param_set_property(fd, ID_DES_HEIGHT, p_param->des_height);
    fscaler_param_set_property(fd, ID_TARGET_WIDTH, p_param->target_width);
    fscaler_param_set_property(fd, ID_TARGET_HEIGHT, p_param->target_height);
    fscaler_param_set_property(fd, ID_CROP_AXIS_X, p_param->crop_axis_x);
    fscaler_param_set_property(fd, ID_CROP_AXIS_Y, p_param->crop_axis_y);
    fscaler_param_set_property(fd, ID_CROP_WIDTH, p_param->crop_width);
    fscaler_param_set_property(fd, ID_CROP_HEIGHT, p_param->crop_height);
    fscaler_param_set_property(fd, ID_SRC_BUF_OFFSET, p_param->buf_offset);
    fscaler_param_set_property(fd, ID_SRC_WIDTH, p_param->src_width);
    fscaler_param_set_property(fd, ID_SRC_HEIGHT, p_param->src_height);
    fscaler_param_set_property(fd, ID_VP_WIDTH, p_param->vp_width);
    fscaler_param_set_property(fd, ID_VP_HEIGHT, p_param->vp_height);
    fscaler_param_set_property(fd, ID_SWC_AXIS_X, p_param->swc_axis_x);
    fscaler_param_set_property(fd, ID_SWC_AXIS_Y, p_param->swc_axis_y);
    fscaler_param_set_property(fd, ID_SWC_WIDTH, p_param->swc_width);
    fscaler_param_set_property(fd, ID_SWC_HEIGHT, p_param->swc_height);
    fscaler_param_set_property(fd, ID_DES_FMT, p_param->des_fmt);
    fscaler_param_set_property(fd, ID_DES_SWAP_CBCR, p_param->des_swap_CbCr);
    fscaler_param_set_property(fd, ID_DES_ORDER, p_param->des_order);
    fscaler_param_set_property(fd, ID_DES_LEVEL, p_param->des_level);
    fscaler_param_set_property(fd, ID_DES_SWAP_Y, p_param->des_swap_Y);
    fscaler_param_set_property(fd, ID_SRC_FMT, p_param->src_fmt);
    fscaler_param_set_property(fd, ID_MEM_SRC_PIT, p_param->mem_src_pit);
    fscaler_param_set_property(fd, ID_SRC_HBLANK, p_param->src_hblank);
    fscaler_param_set_property(fd, ID_SRC_INTERLEAVED, p_param->src_interleaved);
    fscaler_param_set_property(fd, ID_SRC_SWAP_CBCR, p_param->src_swap_CbCr);
    fscaler_param_set_property(fd, ID_SRC_SWAP_Y, p_param->src_swap_Y);
    fscaler_param_set_property(fd, ID_ALBUM, p_param->album);
    fscaler_param_set_property(fd, ID_NON_LINEAR, p_param->non_linear);
    fscaler_param_set_property(fd, ID_DITHER, p_param->dither);
    fscaler_param_set_property(fd, ID_US_EN, p_param->us_en);
    fscaler_param_set_property(fd, ID_CORRECTION, p_param->correction);
}

static inline void fscaler_io_set_frame(int fd, fscaler_frame * p_frame)
{
    fscaler_param_set_property(fd, ID_DES_AXIS_X, p_frame->des_axis_x);
    fscaler_param_set_property(fd, ID_DES_AXIS_Y, p_frame->des_axis_y);
}

static inline int fscaler_io_format_convert_to_layout(unsigned char fmt)
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
    return layout;
}

static inline void fscaler_io_set_source_param(struct fscaler_subjob *p_job,
                                               struct fscaler_param_prop *p_prop)
{
    /*
     * in_bg_w = src_width
     * in_bg_h = src_height
     * in_x = swc_axis_x + (crop_x * crop_width / target_width)
     * in_y = swc_axis_y + (crop_y * crop_height / target_height)
     * in_w = swc_width * crop_width / target_width
     * in_h = swc_height * crop_height / target_height
     */
    p_job->in_bg_w = p_prop->value[ID_SRC_WIDTH];
    p_job->in_bg_h = p_prop->value[ID_SRC_HEIGHT];
    p_job->in_x =
        p_prop->value[ID_SWC_AXIS_X] +
        p_prop->value[ID_CROP_AXIS_X] * p_prop->value[ID_CROP_WIDTH] /
        p_prop->value[ID_TARGET_WIDTH];
    p_job->in_y =
        p_prop->value[ID_SWC_AXIS_Y] +
        p_prop->value[ID_CROP_AXIS_Y] * p_prop->value[ID_CROP_HEIGHT] /
        p_prop->value[ID_TARGET_HEIGHT];
    p_job->in_w =
        p_prop->value[ID_SWC_WIDTH] * p_prop->value[ID_CROP_WIDTH] / p_prop->value[ID_TARGET_WIDTH];
    p_job->in_h =
        p_prop->value[ID_SWC_HEIGHT] * p_prop->value[ID_CROP_HEIGHT] /
        p_prop->value[ID_TARGET_HEIGHT];
    p_job->in_buf.order = (p_prop->value[ID_SRC_SWAP_Y] << 1 | p_prop->value[ID_SRC_SWAP_CBCR]);
    p_job->in_buf.yuv_n = 1;
    p_job->in_buf.layout = fscaler_io_format_convert_to_layout(p_prop->value[ID_SRC_FMT]);
}

static inline void fscaler_io_set_target_param(struct fscaler_subjob *p_job,
                                               struct fscaler_param_prop *p_prop)
{
    /*
     * out_bg_w = des_width
     * out_bg_h = des_height
     * out_x = des_axis_x + crop_axis_x
     * out_y = des_axis_y + crop_axis_y
     * out_w = crop_width
     * out_h = crop_height
     */
    p_job->out_bg_w = p_prop->value[ID_DES_WIDTH];
    p_job->out_bg_h = p_prop->value[ID_DES_HEIGHT];
    p_job->out_x = p_prop->value[ID_DES_AXIS_X] + p_prop->value[ID_CROP_AXIS_X];
    p_job->out_y = p_prop->value[ID_DES_AXIS_Y] + p_prop->value[ID_CROP_AXIS_Y];
    p_job->out_w = p_prop->value[ID_CROP_WIDTH];
    p_job->out_h = p_prop->value[ID_CROP_HEIGHT];
    p_job->out_buf.order = (p_prop->value[ID_DES_SWAP_Y] << 1 | (!p_prop->value[ID_DES_SWAP_CBCR]));
    p_job->out_buf.yuv_n = 1;
    p_job->out_buf.layout = fscaler_io_format_convert_to_layout(p_prop->value[ID_DES_FMT]);
}

static inline void fscaler_io_set_source_frame(struct fscaler_subjob *p_job,
                                               fscaler_frame * p_frame,
                                               struct fscaler_param_prop *p_prop)
{
    switch (p_job->in_buf.layout) {
    case FSCALER_JOB_LAYOUT_Y422:
        if (p_job->in_buf.yuv_n > 0) {
            p_job->in_buf.yuv[0].yuv[0] = p_frame->memsrc + p_prop->value[ID_SRC_BUF_OFFSET];
        }
        break;
    default:
        break;
    }
}

static inline void fscaler_io_set_target_frame(struct fscaler_subjob *p_job,
                                               fscaler_frame * p_frame,
                                               struct fscaler_param_prop *p_prop)
{
    UNUSED_ARG(p_prop);
    switch (p_job->out_buf.layout) {
    case FSCALER_JOB_LAYOUT_MPEG42D:
        if (p_job->out_buf.yuv_n > 0) {
            p_job->out_buf.yuv[0].y_u_v[0] = p_frame->pu8Display0_addr[0];
            p_job->out_buf.yuv[0].y_u_v[1] = p_frame->pu8Display0_addr[1];
            p_job->out_buf.yuv[0].y_u_v[2] = p_frame->pu8Display0_addr[2];
        }
        if (p_job->out_buf.yuv_n > 1) {
            p_job->out_buf.yuv[1].y_u_v[0] = p_frame->pu8Display1_addr[0];
            p_job->out_buf.yuv[1].y_u_v[1] = p_frame->pu8Display1_addr[1];
            p_job->out_buf.yuv[1].y_u_v[2] = p_frame->pu8Display1_addr[2];
        }
        break;

    case FSCALER_JOB_LAYOUT_Y422:
        if (p_job->out_buf.yuv_n > 0) {
            p_job->out_buf.yuv[0].yuv[0] = p_frame->pu8Display0_addr[0];
        }
        if (p_job->out_buf.yuv_n > 1) {
            p_job->out_buf.yuv[1].yuv[0] = p_frame->pu8Display1_addr[0];
        }
        break;

    case FSCALER_JOB_LAYOUT_H2642D:
        if (p_job->out_buf.yuv_n > 0) {
            p_job->out_buf.yuv[0].y_uv[0] = p_frame->pu8Display0_addr[0];
            p_job->out_buf.yuv[0].y_uv[1] = p_frame->pu8Display0_addr[1];
        }
        if (p_job->out_buf.yuv_n > 1) {
            p_job->out_buf.yuv[1].y_uv[0] = p_frame->pu8Display1_addr[0];
            p_job->out_buf.yuv[1].y_uv[1] = p_frame->pu8Display1_addr[1];
        }
        break;

    default:
        break;
    }
}

static inline int fscaler_io_check_source_layout(struct fscaler_subjob *p_job)
{
    int yesno = 0;
    switch (p_job->in_buf.layout) {
    case FSCALER_JOB_LAYOUT_Y422:
        yesno = 1;
        break;
    default:
        break;
    }
    return yesno;
}

static inline int fscaler_io_check_target_layout(struct fscaler_subjob *p_job)
{
    int yesno = 0;
    switch (p_job->out_buf.layout) {
    case FSCALER_JOB_LAYOUT_Y422:
    case FSCALER_JOB_LAYOUT_H2642D:
    case FSCALER_JOB_LAYOUT_MPEG42D:
        yesno = 1;
        break;
    default:
        break;
    }
    return yesno;
}

static void fscaler_io_on_abort(struct fscaler_job *p_sjob)
{
    fscaler_io_private_info_t *p_info;
    if (p_sjob) {
        p_info = (fscaler_io_private_info_t *) p_sjob->data;
        if (p_info) {
            p_info->drop = 0;
        }
    }
}

static void fscaler_io_on_error(struct fscaler_job *p_sjob)
{
    fscaler_io_private_info_t *p_info;
    if (p_sjob) {
        p_info = (fscaler_io_private_info_t *) p_sjob->data;
        if (p_info) {
            p_info->drop = 1;
        }
    }
}

static void fscaler_io_on_done(struct fscaler_job *p_sjob)
{
    fscaler_io_private_info_t *p_info;
    if (p_sjob) {
        p_info = (fscaler_io_private_info_t *) p_sjob->data;
        if (p_info) {
            p_info->drop = 0;
        }
    }
}

static void fscaler_io_after_dispatch(struct fscaler_job *p_sjob)
{
    fscaler_io_private_info_t *p_info;
    if (p_sjob) {
        p_info = (fscaler_io_private_info_t *) p_sjob->data;
        if (p_info) {
            p_info->cmd = wait_event_interruptible(p_info->wq, (p_info->cmd == CMD_WAKEUP));
        }
    }
}

static void fscaler_io_after_render(struct fscaler_job *p_sjob)
{
    fscaler_io_private_info_t *p_info;
    if (p_sjob) {
        p_info = (fscaler_io_private_info_t *) p_sjob->data;
        if (p_info) {
            p_info->cmd = CMD_WAKEUP;
            wake_up_interruptible(&p_info->wq);
        }
    }
    fscaler_job_remove(&p_sjob);
}

static inline int fscaler_io_do_mem2mem(struct fscaler_param_private *p_priv, unsigned long arg)
{
    int ret = 0;
    fscaler_io_private_info_t *p_info = NULL;
    if (0 == arg) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    p_info = (fscaler_io_private_info_t *) p_priv->p_info;
    if (NULL == p_info) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    if (copy_from_user((void *)&p_info->param, (void *)arg, sizeof(fscaler_param))) {
        ret = -EFAULT;
        goto ERROR_EXIT;
    }
    // Convert fscaler_param to id_value
    fscaler_io_set_param(p_priv->fd, &p_info->param);
    if (p_info->param.API_version != FSCALERVER) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    if (copy_to_user((void *)arg, (void *)&p_info->param, sizeof(fscaler_param))) {
        ret = -EFAULT;
        goto ERROR_EXIT;
    }
    return ret;

  ERROR_EXIT:
    return ret;
}

static inline int fscaler_io_do_frame(struct fscaler_param_private *p_priv, unsigned long arg)
{
    int ret = 0;
    fscaler_io_private_info_t *p_info = NULL;
    struct fscaler_job *p_sjob = NULL;
    struct fscaler_subjob *p_job = NULL;
    if (0 == arg) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    p_info = (fscaler_io_private_info_t *) p_priv->p_info;
    if (NULL == p_info) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    if (copy_from_user((void *)&p_info->frame, (void *)arg, sizeof(fscaler_frame))) {
        ret = -EFAULT;
        goto ERROR_EXIT;
    }
    fscaler_io_va_to_pa(&p_info->frame);
    fscaler_io_set_frame(p_priv->fd, &p_info->frame);
    p_sjob = fscaler_job_alloc(p_priv->fd, FSCALER_JOB_TYPE_SINGLE, 1);
    if (NULL == p_sjob) {
        ret = -ENOMEM;
        goto ERROR_EXIT;
    }
    p_sjob->abort = fscaler_io_on_abort;
    p_sjob->error = fscaler_io_on_error;
    p_sjob->done = fscaler_io_on_done;
    p_job = p_sjob->smap[0];
    fscaler_param_commit_property_all(p_priv->fd);
    fscaler_io_set_source_param(p_job, &p_priv->prop);
    if (!fscaler_io_check_source_layout(p_job)) {
        ret = -EINVAL;
        goto ERROR_FREE_VJOB;
    }
    fscaler_io_set_target_param(p_job, &p_priv->prop);
    if (!fscaler_io_check_target_layout(p_job)) {
        M_ERROR("SCL: Invalid target layout %d!!!\n", p_job->out_buf.layout);
        ret = -EINVAL;
        goto ERROR_FREE_VJOB;
    }
    fscaler_io_set_source_frame(p_job, &p_info->frame, &p_priv->prop);
    fscaler_io_set_target_frame(p_job, &p_info->frame, &p_priv->prop);
    fscaler_driver_dispatch(p_sjob, (unsigned long)p_info);
    if (!p_info->drop) {
        p_info->frame.done = 1;
    } else {
        p_info->frame.done = 0;
    }
    if (copy_to_user((void *)arg, (void *)&p_info->frame, sizeof(fscaler_frame))) {
        p_info->frame.done = 0;
        ret = -EFAULT;
        goto ERROR_EXIT;
    }
    return ret;

  ERROR_FREE_VJOB:
    fscaler_job_remove(&p_sjob);

  ERROR_EXIT:
    return ret;
}

static int fscaler_io_fops_open(struct inode *inode, struct file *file)
{
    int ret = 0;
    struct fscaler_param_private *p_priv = NULL;
    int fd = fscaler_param_get_entity_index(FSCFD_AUTO_SEED);
    if (0 > fd || max_entity <= fd) {
        ret = -EINVAL;
        goto ERROR_EXIT;
    }
    p_priv = fscaler_param_private_find(fd);
    if (unlikely(NULL == p_priv)) {
        ret = -EINVAL;
        goto ERROR_EXIT_PUT_FD;
    }
    p_priv->p_info = fscaler_io_private_info_find(fd);
    ret = fscaler_driver_open(p_priv->fd);
    if (unlikely(FSCALER_FAILED(ret))) {
        ret = -ENOTSUPP;
        goto ERROR_EXIT_PUT_FD;
    }
    file->private_data = (void *)p_priv;
    return ret;

  ERROR_EXIT_PUT_FD:
    fscaler_param_put_entity_index(fd);

  ERROR_EXIT:
    file->private_data = NULL;
    return ret;
}

static int fscaler_io_fops_release(struct inode *inode, struct file *file)
{
    struct fscaler_param_private *p_priv = NULL;
    if (ddr_info.va_addr) {
        frm_release_buf_info(FRM_ID(FRMIDX_SCAL0, 0), &ddr_info);
        ddr_info.va_addr = 0;
    }
    p_priv = (struct fscaler_param_private *)file->private_data;
    file->private_data = NULL;
    if (p_priv) {
        fscaler_driver_close(p_priv->fd);
        p_priv->p_info = NULL;
        fscaler_param_put_entity_index(p_priv->fd);
    }
    return 0;
}

static int
fscaler_io_fops_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    struct fscaler_param_private *p_priv = (struct fscaler_param_private *)file->private_data;
    if (p_priv) {
        switch (cmd) {
        case FSCALER_IOCTL_MEM2MEM:
            ret = fscaler_io_do_mem2mem(p_priv, arg);
            break;
        case FSCALER_IOCTL_SHARP:
            ret = -ENOTSUPP;
            break;
        case FSCALER_IOCTL_FRAME:
            ret = fscaler_io_do_frame(p_priv, arg);
            break;
        default:
            ret = -ENOIOCTLCMD;
            break;
        }
    }
    return ret;

  ERROR_EXIT:
    return ret;
}

static int fscaler_io_fops_mmap(struct file *file, struct vm_area_struct *vma)
{
#ifdef FSCDRV_FRAMMAP
    unsigned long size = vma->vm_end - vma->vm_start;
    unsigned long memmap_max_size = (max_width * max_height * 2 + 0x3f) * 4;
    if (size > memmap_max_size) {
        M_ERROR("SCL: size = %ld, must less than %ld\n", size, memmap_max_size);
    }
    if (!ddr_info.va_addr) {
        ddr_info.size = size;
        if (0 > frm_get_buf_info(FRM_ID(FRMIDX_SCAL0, 0), &ddr_info)) {
            M_ERROR("SCL: Frammap allocate failed, get buffer info failed !!!\n");
            ddr_info.va_addr = 0;
            return -EFAULT;
        }

        if (ddr_info.size < size) {
            M_ERROR("SCL: Frammap allocate failed, frame size too small !!!\n");
            ddr_info.va_addr = 0;
            return -EFAULT;
        }

        if (!ddr_info.va_addr) {
            M_ERROR("SCL: Frammap allocate failed, null va_addr !!!\n");
            return -EFAULT;
        }
    }
    vma->vm_pgoff = ddr_info.phy_addr >> PAGE_SHIFT;
    vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    vma->vm_flags |= VM_RESERVED;
    if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, size, vma->vm_page_prot)) {
        M_ERROR("SCL: Remap_pfn_range() failed\n");
        ddr_info.va_addr = 0;
        return -EFAULT;
    }
    return 0;
#else
    return -EPERM;
#endif /* FSCDRV_FRAMMAP */
}

struct file_operations fscaler_io_fops = {
    .owner = THIS_MODULE,
    .open = fscaler_io_fops_open,
    .release = fscaler_io_fops_release,
    .ioctl = fscaler_io_fops_ioctl,
    .mmap = fscaler_io_fops_mmap
};

struct miscdevice fscaler_io_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = MPFX_NAME,
    .fops = &fscaler_io_fops
};

int fscaler_io_uninstall_interface(void)
{
    int i = 0;
    if (fscaler_io_interface_is_installed) {
        if (0 > misc_deregister(&fscaler_io_device)) {
            M_ERROR("SCL: deregister misc device failed !!!\n");
        }
        for (i = 0; i < FSCIP_MAX; ++i) {
            fscaler_driver_unprepare(i);
        }
        fscaler_resource_destroy();
        fscaler_io_interface_is_installed = 0;
    }
    return FSCALER_IO_OK;
}

EXPORT_SYMBOL(fscaler_io_uninstall_interface);

int fscaler_io_install_interface(struct device *p_dev, struct platform_device *p_plat_dev)
{
    struct resource *p_res = NULL;
    unsigned int base = 0;
    int irq = -1, i = 0, j = 0;
    if (!fscaler_io_interface_is_installed) {
        if (FSCALER_SUCCEED(fscaler_resource_create())) {
            for (i = 0; i < FSCIP_MAX; ++i) {
                p_res = platform_get_resource(p_plat_dev, IORESOURCE_MEM, i);
                if (NULL == p_res) {
                    j = i;
                    break;
                }
                base = (unsigned int)ioremap_nocache(p_res->start, (p_res->end - p_res->start));
                if (0 == base) {
                    j = i;
                    break;
                }
                p_res = platform_get_resource(p_plat_dev, IORESOURCE_IRQ, i);
                if (NULL == p_res) {
                    j = i;
                    break;
                }
                irq = (int)p_res->start;
                if (FSCALER_FAILED(fscaler_driver_prepare(i, base, irq))) {
                    j = i;
                    break;
                }
            }
            if (j == 0) {
                dev_set_drvdata(p_dev, NULL);
                if (0 <= misc_register(&fscaler_io_device)) {
                    for (i = 0; i < max_entity; ++i) {
                        fscaler_io_private_info_init(i);
                    }
                    return FSCALER_IO_OK;
                }
                j = FSCIP_MAX;
            }
            for (i = 0; i < j; ++i) {
                fscaler_driver_unprepare(i);
            }
            fscaler_resource_destroy();
        }
        fscaler_io_interface_is_installed = 1;
    }
    return FSCALER_IO_ERR_INSTALL;
}

EXPORT_SYMBOL(fscaler_io_install_interface);
