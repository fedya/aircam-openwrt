/**
 * @file fscaler_vg.c
 * @brief Scaler videograph interface
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.xx
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_vg.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"
#include "fscaler_property.h"
#include "fscaler_job.h"
#include "fscaler_entity.h"
#include "fscaler_device.h"
#include "videograph_device.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO
#define VENTITY_TO_FD(ent) \
    (int)(ent ? VIDEOGRAPH_ENTITY_TO_FD((struct video_entity_t *)ent) : -1)
#define VJOB_TO_FD(vjob) \
    (int)(vjob ? VIDEOGRAPH_ENTITY_TO_FD(((struct v_job_t *)(vjob))->entity) : -1)

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static void fscaler_vg_do_abort(struct fscaler_job *job);
static void fscaler_vg_do_error(struct fscaler_job *job);
static void fscaler_vg_do_done(struct fscaler_job *job);
static void fscaler_vg_do_destruct(struct fscaler_job *job);
static int fscaler_vg_fops_putjob(void *data);
static int fscaler_vg_fops_flushjob(void *data);
static int fscaler_vg_fops_stop(void *data);
static unsigned int fscaler_vg_fops_getproperty(void *data, unsigned int id);
static int fscaler_vg_fops_setproperty(void *data, unsigned int id, unsigned int value);
static int fscaler_vg_fops_queryid(void *data, char *name);
static void fscaler_vg_uninstall(struct fscaler_interface *inter);
static int fscaler_vg_install(struct fscaler_interface *inter);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_VG_NAME[] = "fscvg";
static struct v_operations FSCALER_VG_FOPS = {
    .putjob = fscaler_vg_fops_putjob,
    .flushjob = fscaler_vg_fops_flushjob,
    .stop = fscaler_vg_fops_stop,
    .getproperty = fscaler_vg_fops_getproperty,
    .setproperty = fscaler_vg_fops_setproperty,
    .queryid = fscaler_vg_fops_queryid
};

static struct videograph_device FSCALER_VG_DEVICE = {
    .name = MPFX_NAME,
    .fops = &FSCALER_VG_FOPS
};

static struct fscaler_driver *FSCALER_VG_OWNER = NULL;

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE int fscaler_vg_check_source_param_single(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_subjob_check_source_param(sjob,
                                                 prop->value[ID_VP_WIDTH],
                                                 prop->value[ID_VP_HEIGHT],
                                                 prop->value[ID_SWC_WIDTH],
                                                 prop->value[ID_SWC_HEIGHT]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_param_twice(struct fscaler_subjob *sjob,
                                                      struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_source_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_check_source_param(sjob,
                                                 prop->value[ID_VP_WIDTH_2],
                                                 prop->value[ID_VP_HEIGHT_2],
                                                 prop->value[ID_SWC_WIDTH_2],
                                                 prop->value[ID_SWC_HEIGHT_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_param_triple(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_source_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_check_source_param_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_check_source_param(sjob,
                                                 prop->value[ID_VP_WIDTH_3],
                                                 prop->value[ID_VP_HEIGHT_3],
                                                 prop->value[ID_SWC_WIDTH_3],
                                                 prop->value[ID_SWC_HEIGHT_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_param(int type, struct fscaler_subjob *sjob,
                                                struct fscaler_property *prop)
{
    int ret = FSCALER_OK;
    switch (type) {
    case FSCALER_JOB_TYPE_SINGLE:
        ret = fscaler_vg_check_source_param_single(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TWICE:
        ret = fscaler_vg_check_source_param_twice(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TRIPLE:
        ret = fscaler_vg_check_source_param_triple(sjob, prop);
        break;
    default:
        printk("SCL: Check source parameter with unknown type %d\n", type);
        ret = FSCALER_ERR_FAIL;
        break;
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_set_source_param_single(struct fscaler_subjob *sjob,
                                                     struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_subjob_set_source_param(sjob,
                                               prop->value[ID_VP_WIDTH], prop->value[ID_VP_HEIGHT],
                                               prop->value[ID_TARGET_WIDTH],
                                               prop->value[ID_TARGET_HEIGHT],
                                               prop->value[ID_SWC_AXIS_X],
                                               prop->value[ID_SWC_AXIS_Y],
                                               prop->value[ID_SWC_WIDTH],
                                               prop->value[ID_SWC_HEIGHT],
                                               prop->value[ID_CROP_AXIS_X],
                                               prop->value[ID_CROP_AXIS_Y],
                                               prop->value[ID_CROP_WIDTH],
                                               prop->value[ID_CROP_HEIGHT],
                                               prop->value[ID_SRC_SWAP_Y],
                                               prop->value[ID_SRC_SWAP_CBCR],
                                               prop->value[ID_SRC_FMT],
                                               prop->value[ID_MEM_SRC_SAMPLE_DOWN]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_param_twice(struct fscaler_subjob *sjob,
                                                    struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_set_source_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_set_source_param(sjob,
                                               prop->value[ID_VP_WIDTH_2],
                                               prop->value[ID_VP_HEIGHT_2],
                                               prop->value[ID_TARGET_WIDTH_2],
                                               prop->value[ID_TARGET_HEIGHT_2],
                                               prop->value[ID_SWC_AXIS_X_2],
                                               prop->value[ID_SWC_AXIS_Y_2],
                                               prop->value[ID_SWC_WIDTH_2],
                                               prop->value[ID_SWC_HEIGHT_2],
                                               prop->value[ID_CROP_AXIS_X_2],
                                               prop->value[ID_CROP_AXIS_Y_2],
                                               prop->value[ID_CROP_WIDTH_2],
                                               prop->value[ID_CROP_HEIGHT_2],
                                               prop->value[ID_SRC_SWAP_Y],
                                               prop->value[ID_SRC_SWAP_CBCR],
                                               prop->value[ID_SRC_FMT],
                                               prop->value[ID_MEM_SRC_SAMPLE_DOWN_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_param_triple(struct fscaler_subjob *sjob,
                                                     struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_set_source_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_set_source_param_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_set_source_param(sjob,
                                               prop->value[ID_VP_WIDTH_3],
                                               prop->value[ID_VP_HEIGHT_3],
                                               prop->value[ID_TARGET_WIDTH_3],
                                               prop->value[ID_TARGET_HEIGHT_3],
                                               prop->value[ID_SWC_AXIS_X_3],
                                               prop->value[ID_SWC_AXIS_Y_3],
                                               prop->value[ID_SWC_WIDTH_3],
                                               prop->value[ID_SWC_HEIGHT_3],
                                               prop->value[ID_CROP_AXIS_X_3],
                                               prop->value[ID_CROP_AXIS_Y_3],
                                               prop->value[ID_CROP_WIDTH_3],
                                               prop->value[ID_CROP_HEIGHT_3],
                                               prop->value[ID_SRC_SWAP_Y],
                                               prop->value[ID_SRC_SWAP_CBCR],
                                               prop->value[ID_SRC_FMT],
                                               prop->value[ID_MEM_SRC_SAMPLE_DOWN_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_param(int type, struct fscaler_subjob *sjob,
                                              struct fscaler_property *prop)
{
    int ret = fscaler_vg_check_source_param(type, sjob, prop);
    if (FSCALER_SUCCEED(ret)) {
        switch (type) {
        case FSCALER_JOB_TYPE_SINGLE:
            ret = fscaler_vg_set_source_param_single(sjob, prop);
            break;
        case FSCALER_JOB_TYPE_TWICE:
            ret = fscaler_vg_set_source_param_twice(sjob, prop);
            break;
        case FSCALER_JOB_TYPE_TRIPLE:
            ret = fscaler_vg_set_source_param_triple(sjob, prop);
            break;
        default:
            printk("SCL: Unkown job type:%d\n", type);
            ret = FSCALER_ERR_FAIL;
            break;
        }
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_check_target_param_single(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_subjob_check_target_param(sjob,
                                                 prop->value[ID_TARGET_WIDTH],
                                                 prop->value[ID_TARGET_HEIGHT],
                                                 prop->value[ID_CROP_WIDTH],
                                                 prop->value[ID_CROP_HEIGHT]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_param_twice(struct fscaler_subjob *sjob,
                                                      struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_target_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_check_target_param(sjob,
                                                 prop->value[ID_TARGET_WIDTH_2],
                                                 prop->value[ID_TARGET_HEIGHT_2],
                                                 prop->value[ID_CROP_WIDTH_2],
                                                 prop->value[ID_CROP_HEIGHT_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_param_triple(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_target_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_check_target_param_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_check_target_param(sjob,
                                                 prop->value[ID_TARGET_WIDTH_3],
                                                 prop->value[ID_TARGET_HEIGHT_3],
                                                 prop->value[ID_CROP_WIDTH_3],
                                                 prop->value[ID_CROP_HEIGHT_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_param(int type, struct fscaler_subjob *sjob,
                                                struct fscaler_property *prop)
{
    int ret = FSCALER_OK;
    switch (type) {
    case FSCALER_JOB_TYPE_SINGLE:
        ret = fscaler_vg_check_target_param_single(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TWICE:
        ret = fscaler_vg_check_target_param_twice(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TRIPLE:
        ret = fscaler_vg_check_target_param_triple(sjob, prop);
        break;
    default:
        printk("SCL: Check target parameter with unknown type %d\n", type);
        ret = FSCALER_ERR_FAIL;
        break;
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_set_target_param_single(struct fscaler_subjob *sjob,
                                                     struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_subjob_set_target_param(sjob,
                                               prop->value[ID_DES_AXIS_X],
                                               prop->value[ID_DES_AXIS_Y],
                                               prop->value[ID_DES_WIDTH],
                                               prop->value[ID_DES_HEIGHT],
                                               prop->value[ID_CROP_AXIS_X],
                                               prop->value[ID_CROP_AXIS_Y],
                                               prop->value[ID_CROP_WIDTH],
                                               prop->value[ID_CROP_HEIGHT],
                                               prop->value[ID_DES_SWAP_Y],
                                               prop->value[ID_DES_SWAP_CBCR],
                                               prop->value[ID_DES_FMT],
                                               prop->value[ID_MEM_DES_SAMPLE_DOWN]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_param_twice(struct fscaler_subjob *sjob,
                                                    struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_set_target_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_set_target_param(sjob,
                                               prop->value[ID_DES_AXIS_X_2],
                                               prop->value[ID_DES_AXIS_Y_2],
                                               prop->value[ID_DES_WIDTH],
                                               prop->value[ID_DES_HEIGHT],
                                               prop->value[ID_CROP_AXIS_X_2],
                                               prop->value[ID_CROP_AXIS_Y_2],
                                               prop->value[ID_CROP_WIDTH_2],
                                               prop->value[ID_CROP_HEIGHT_2],
                                               prop->value[ID_DES_SWAP_Y],
                                               prop->value[ID_DES_SWAP_CBCR],
                                               prop->value[ID_DES_FMT],
                                               prop->value[ID_MEM_DES_SAMPLE_DOWN_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_param_triple(struct fscaler_subjob *sjob,
                                                     struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_set_target_param_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_set_target_param_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_set_target_param(sjob,
                                               prop->value[ID_DES_AXIS_X_3],
                                               prop->value[ID_DES_AXIS_Y_3],
                                               prop->value[ID_DES_WIDTH],
                                               prop->value[ID_DES_HEIGHT],
                                               prop->value[ID_CROP_AXIS_X_3],
                                               prop->value[ID_CROP_AXIS_Y_3],
                                               prop->value[ID_CROP_WIDTH_3],
                                               prop->value[ID_CROP_HEIGHT_3],
                                               prop->value[ID_DES_SWAP_Y],
                                               prop->value[ID_DES_SWAP_CBCR],
                                               prop->value[ID_DES_FMT],
                                               prop->value[ID_MEM_DES_SAMPLE_DOWN_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_param(int type, struct fscaler_subjob *sjob,
                                              struct fscaler_property *prop)
{
    int ret = fscaler_vg_check_target_param(type, sjob, prop);
    if (FSCALER_SUCCEED(ret)) {
        switch (type) {
        case FSCALER_JOB_TYPE_SINGLE:
            ret = fscaler_vg_set_target_param_single(sjob, prop);
            break;
        case FSCALER_JOB_TYPE_TWICE:
            ret = fscaler_vg_set_target_param_twice(sjob, prop);
            break;
        case FSCALER_JOB_TYPE_TRIPLE:
            ret = fscaler_vg_set_target_param_triple(sjob, prop);
            break;
        default:
            printk("SCL: Unkown job type:%d\n", type);
            ret = FSCALER_ERR_FAIL;
            break;
        }
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_check_source_frame_single(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_subjob_check_source_frame(sjob,
                                                 prop->value[ID_VP_WIDTH],
                                                 prop->value[ID_VP_HEIGHT],
                                                 prop->value[ID_SWC_WIDTH],
                                                 prop->value[ID_SWC_HEIGHT]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_frame_twice(struct fscaler_subjob *sjob,
                                                      struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_source_frame_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_check_source_frame(sjob,
                                                 prop->value[ID_VP_WIDTH_2],
                                                 prop->value[ID_VP_HEIGHT_2],
                                                 prop->value[ID_SWC_WIDTH_2],
                                                 prop->value[ID_SWC_HEIGHT_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_frame_triple(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_source_frame_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_check_source_frame_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_check_source_frame(sjob,
                                                 prop->value[ID_VP_WIDTH_3],
                                                 prop->value[ID_VP_HEIGHT_3],
                                                 prop->value[ID_SWC_WIDTH_3],
                                                 prop->value[ID_SWC_HEIGHT_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_source_frame(int type, struct fscaler_subjob *sjob,
                                                struct fscaler_property *prop)
{
    int ret = FSCALER_OK;
    switch (type) {
    case FSCALER_JOB_TYPE_SINGLE:
        ret = fscaler_vg_check_source_frame_single(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TWICE:
        ret = fscaler_vg_check_source_frame_twice(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TRIPLE:
        ret = fscaler_vg_check_source_frame_triple(sjob, prop);
        break;
    default:
        printk("SCL: Check source frame with unknown type %d\n", type);
        ret = FSCALER_ERR_FAIL;
        break;
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_set_source_frame_single(struct fscaler_subjob *sjob,
                                                     struct v_job_t *vjob,
                                                     struct fscaler_property *prop)
{
    if (sjob && vjob && vjob->in_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_subjob_set_source_frame(sjob,
                                                   vjob->in_buf->addr_pa,
                                                   prop->value[ID_SRC_BUF_OFFSET],
                                                   prop->value[ID_VP_WIDTH],
                                                   prop->value[ID_VP_HEIGHT],
                                                   prop->value[ID_TARGET_WIDTH],
                                                   prop->value[ID_TARGET_HEIGHT],
                                                   prop->value[ID_SWC_AXIS_X],
                                                   prop->value[ID_SWC_AXIS_Y],
                                                   prop->value[ID_SWC_WIDTH],
                                                   prop->value[ID_SWC_HEIGHT],
                                                   prop->value[ID_CROP_AXIS_X],
                                                   prop->value[ID_CROP_AXIS_Y],
                                                   prop->value[ID_CROP_WIDTH],
                                                   prop->value[ID_CROP_HEIGHT]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_frame_twice(struct fscaler_subjob *sjob,
                                                    struct v_job_t *vjob,
                                                    struct fscaler_property *prop)
{
    if (sjob && vjob && vjob->in_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_vg_set_source_frame_single(sjob, vjob, prop);
        } else if (1 == sjob->index) {
            return fscaler_subjob_set_source_frame(sjob,
                                                   vjob->in_buf->addr_pa,
                                                   prop->value[ID_SRC_BUF_OFFSET],
                                                   prop->value[ID_VP_WIDTH_2],
                                                   prop->value[ID_VP_HEIGHT_2],
                                                   prop->value[ID_TARGET_WIDTH_2],
                                                   prop->value[ID_TARGET_HEIGHT_2],
                                                   prop->value[ID_SWC_AXIS_X_2],
                                                   prop->value[ID_SWC_AXIS_Y_2],
                                                   prop->value[ID_SWC_WIDTH_2],
                                                   prop->value[ID_SWC_HEIGHT_2],
                                                   prop->value[ID_CROP_AXIS_X_2],
                                                   prop->value[ID_CROP_AXIS_Y_2],
                                                   prop->value[ID_CROP_WIDTH_2],
                                                   prop->value[ID_CROP_HEIGHT_2]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_frame_triple(struct fscaler_subjob *sjob,
                                                     struct v_job_t *vjob,
                                                     struct fscaler_property *prop)
{
//#define LET_TOP_BLACK
#ifdef LET_TOP_BLACK
    unsigned int i = 0;
    unsigned char *vaddr = (unsigned char *)vjob->in_buf->addr_va;
    unsigned int buf_offset = 0, vp_width = 0, vp_height = 0;
    unsigned int swc_axis_y = 0, crop_axis_y = 0;
    unsigned int swc_height = 0, target_height = 0;
    unsigned int swc_axis_x = 0, crop_axis_x = 0;
    unsigned int swc_width = 0, target_width = 0;
#endif /* LET_TOP_BLACK */

    if (sjob && vjob && vjob->in_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_vg_set_source_frame_single(sjob, vjob, prop);
        } else if (1 == sjob->index) {
            return fscaler_vg_set_source_frame_twice(sjob, vjob, prop);
        } else if (2 == sjob->index) {
#ifdef LET_TOP_BLACK
            buf_offset = prop->value[ID_SRC_BUF_OFFSET];
            vp_width = prop->value[ID_VP_WIDTH_3];
            vp_height = prop->value[ID_VP_HEIGHT_3];
            swc_axis_y = prop->value[ID_SWC_AXIS_Y_3];
            crop_axis_y = prop->value[ID_CROP_AXIS_Y_3];
            swc_height = prop->value[ID_SWC_HEIGHT_3];
            target_height = prop->value[ID_TARGET_HEIGHT_3];
            swc_axis_x = prop->value[ID_SWC_AXIS_X_3];
            crop_axis_x = prop->value[ID_CROP_AXIS_X_3];
            swc_width = prop->value[ID_SWC_WIDTH_3];
            target_width = prop->value[ID_TARGET_WIDTH_3];
#ifdef __REDUCE_DMA_BUS_LOADING
            vaddr =
                vaddr + buf_offset
                + 2 * vp_width * (swc_axis_y + crop_axis_y * swc_height / target_height)
                + 2 * 1 * (swc_axis_x + crop_axis_x * swc_width / target_width);
#else
            vaddr = vaddr + buf_offset;
#endif /* __REDUCE_DMA_BUS_LOADING */
            for (i = 0; i < vp_width * vp_height; ++i) {
                if (i % 2 == 0) {
                    *(vaddr + i) = 0x80;
                } else {
                    *(vaddr + i) = 0x10;
                }
            }
#endif /* LET_TOP_BLACK */
            return fscaler_subjob_set_source_frame(sjob,
                                                   vjob->in_buf->addr_pa,
                                                   prop->value[ID_SRC_BUF_OFFSET],
                                                   prop->value[ID_VP_WIDTH_3],
                                                   prop->value[ID_VP_HEIGHT_3],
                                                   prop->value[ID_TARGET_WIDTH_3],
                                                   prop->value[ID_TARGET_HEIGHT_3],
                                                   prop->value[ID_SWC_AXIS_X_3],
                                                   prop->value[ID_SWC_AXIS_Y_3],
                                                   prop->value[ID_SWC_WIDTH_3],
                                                   prop->value[ID_SWC_HEIGHT_3],
                                                   prop->value[ID_CROP_AXIS_X_3],
                                                   prop->value[ID_CROP_AXIS_Y_3],
                                                   prop->value[ID_CROP_WIDTH_3],
                                                   prop->value[ID_CROP_HEIGHT_3]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_source_frame(int type, struct fscaler_subjob *sjob,
                                              struct v_job_t *vjob, struct fscaler_property *prop)
{
    int ret = fscaler_vg_check_source_frame(type, sjob, prop);
    if (FSCALER_SUCCEED(ret)) {
        switch (type) {
        case FSCALER_JOB_TYPE_SINGLE:
            fscaler_vg_set_source_frame_single(sjob, vjob, prop);
            break;
        case FSCALER_JOB_TYPE_TWICE:
            fscaler_vg_set_source_frame_twice(sjob, vjob, prop);
            break;
        case FSCALER_JOB_TYPE_TRIPLE:
            fscaler_vg_set_source_frame_triple(sjob, vjob, prop);
            break;
        default:
            printk("SCL: Unknown job type %d\n", type);
            ret = FSCALER_ERR_FAIL;
            break;
        }
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_check_target_frame_single(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    return fscaler_subjob_check_target_frame(sjob,
                                             prop->value[ID_DES_WIDTH], prop->value[ID_DES_HEIGHT],
                                             prop->value[ID_CROP_WIDTH],
                                             prop->value[ID_CROP_HEIGHT]);
  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_frame_twice(struct fscaler_subjob *sjob,
                                                      struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_target_frame_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_subjob_check_target_frame(sjob,
                                                 prop->value[ID_DES_WIDTH],
                                                 prop->value[ID_DES_HEIGHT],
                                                 prop->value[ID_CROP_WIDTH_2],
                                                 prop->value[ID_CROP_HEIGHT_2]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_frame_triple(struct fscaler_subjob *sjob,
                                                       struct fscaler_property *prop)
{
    if (!sjob) {
        printk("SCL: Null scaler sub-job pointer\n");
        goto ERROR_EXIT;
    }
    if (!prop) {
        printk("SCL: Null scaler property pointer\n");
        goto ERROR_EXIT;
    }
    if (0 == sjob->index) {
        return fscaler_vg_check_target_frame_single(sjob, prop);
    } else if (1 == sjob->index) {
        return fscaler_vg_check_target_frame_twice(sjob, prop);
    } else if (2 == sjob->index) {
        return fscaler_subjob_check_target_frame(sjob,
                                                 prop->value[ID_DES_WIDTH],
                                                 prop->value[ID_DES_HEIGHT],
                                                 prop->value[ID_CROP_WIDTH_3],
                                                 prop->value[ID_CROP_HEIGHT_3]);
    }
    return FSCALER_OK;

  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_check_target_frame(int type, struct fscaler_subjob *sjob,
                                                struct fscaler_property *prop)
{
    int ret = FSCALER_OK;
    switch (type) {
    case FSCALER_JOB_TYPE_SINGLE:
        ret = fscaler_vg_check_target_frame_single(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TWICE:
        ret = fscaler_vg_check_target_frame_twice(sjob, prop);
        break;
    case FSCALER_JOB_TYPE_TRIPLE:
        ret = fscaler_vg_check_target_frame_triple(sjob, prop);
        break;
    default:
        printk("SCL: Check target frame with unknown type %d\n", type);
        ret = FSCALER_ERR_FAIL;
        break;
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_set_target_frame_single(struct fscaler_subjob *sjob,
                                                     struct v_job_t *vjob,
                                                     struct fscaler_property *prop)
{
    if (sjob && vjob && vjob->out_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_subjob_set_target_frame(sjob, vjob->out_buf->addr_pa,
                                                   prop->value[ID_DES_BUF_OFFSET],
                                                   prop->value[ID_DES_WIDTH],
                                                   prop->value[ID_DES_HEIGHT]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_frame_twice(struct fscaler_subjob *sjob,
                                                    struct v_job_t *vjob,
                                                    struct fscaler_property *prop)
{
    if (sjob && vjob && vjob->out_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_vg_set_target_frame_single(sjob, vjob, prop);
        } else if (1 == sjob->index) {
            return fscaler_subjob_set_target_frame(sjob, vjob->out_buf->addr_pa,
                                                   prop->value[ID_DES_BUF_OFFSET_2],
                                                   prop->value[ID_DES_WIDTH],
                                                   prop->value[ID_DES_HEIGHT]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_frame_triple(struct fscaler_subjob *sjob,
                                                     struct v_job_t *vjob,
                                                     struct fscaler_property *prop)
{
    if (sjob && vjob && vjob->out_buf && prop) {
        if (0 == sjob->index) {
            return fscaler_vg_set_target_frame_single(sjob, vjob, prop);
        } else if (1 == sjob->index) {
            return fscaler_vg_set_target_frame_twice(sjob, vjob, prop);
        } else if (2 == sjob->index) {
            return fscaler_subjob_set_target_frame(sjob, vjob->out_buf->addr_pa,
                                                   prop->value[ID_DES_BUF_OFFSET_3],
                                                   prop->value[ID_DES_WIDTH],
                                                   prop->value[ID_DES_HEIGHT]);
        }
    }
    return FSCALER_ERR_FAIL;
}

STATIC_INLINE int fscaler_vg_set_target_frame(int type, struct fscaler_subjob *sjob,
                                              struct v_job_t *vjob, struct fscaler_property *prop)
{
    int ret = fscaler_vg_check_target_frame(type, sjob, prop);
    if (FSCALER_SUCCEED(ret)) {
        switch (type) {
        case FSCALER_JOB_TYPE_SINGLE:
            ret = fscaler_vg_set_target_frame_single(sjob, vjob, prop);
            break;
        case FSCALER_JOB_TYPE_TWICE:
            ret = fscaler_vg_set_target_frame_twice(sjob, vjob, prop);
            break;
        case FSCALER_JOB_TYPE_TRIPLE:
            ret = fscaler_vg_set_target_frame_triple(sjob, vjob, prop);
            break;
        default:
            printk("SCL: Unknown job type %d\n", type);
            ret = FSCALER_ERR_FAIL;
            break;
        }
    }
    return ret;
}

STATIC_INLINE int fscaler_vg_check_source_layout(struct fscaler_subjob *sjob)
{
    return fscaler_subjob_check_source_layout(sjob);
}

STATIC_INLINE int fscaler_vg_check_target_layout(struct fscaler_subjob *sjob)
{
    return fscaler_subjob_check_target_layout(sjob);
}

STATIC_INLINE void fscaler_vg_set_owner(struct fscaler_driver *drv)
{
    FSCALER_VG_OWNER = drv;
}

STATIC_INLINE struct fscaler_driver *fscaler_vg_get_owner(void)
{
    return FSCALER_VG_OWNER;
}

STATIC_INLINE struct fscaler_table *fscaler_vg_get_table(const char *name)
{
    return fscaler_driver_table_find(FSCALER_VG_OWNER, name);
}

STATIC_INLINE void *fscaler_vg_table_item_find(const char *name, int index)
{
    return fscaler_table_item_find(fscaler_vg_get_table(name), index);
}

STATIC_INLINE struct fscaler_property *fscaler_vg_get_property(int fd)
{
    return (struct fscaler_property *)fscaler_vg_table_item_find(fscaler_property_table_name(), fd);
}

STATIC_INLINE struct fscaler_entity *fscaler_vg_get_entity(int fd)
{
    return (struct fscaler_entity *)fscaler_vg_table_item_find(fscaler_entity_table_name(), fd);
}

STATIC_INLINE struct fscaler_device *fscaler_vg_get_device(int fd)
{
    return (struct fscaler_device *)fscaler_vg_table_item_find(fscaler_device_table_name(), fd);
}

STATIC_INLINE void fscaler_vg_close(struct fscaler_entity *ent)
{
    if (ent && ent->prop) {
        fscaler_driver_close(ent);
        fscaler_property_set(ent->prop, ID_SWAP, 1);
        M_DEBUG("SCL: Close, %d\n", ent->fd);
    }
}

STATIC_INLINE int fscaler_vg_open(struct fscaler_entity *ent)
{
    int hr = 0;
    if (ent && ent->prop) {
        hr = fscaler_driver_open(ent);
        if (FSCALER_SUCCEED(hr)) {
            fscaler_property_set(ent->prop, ID_SWAP, 1);
            M_DEBUG("SCL: Open, %d\n", ent->fd);
            return FSCALER_OK;
        }
    }
    return FSCALER_ERR_OPEN;
}

STATIC_INLINE void fscaler_vg_check_vjob(struct v_job_t *vjob)
{
    if (vjob) {
        if (unlikely(0 == vjob->in_buf || 0 == vjob->out_buf)) {
            M_ERROR("SCL: error %s job %d in_buf %p out_buf %p\n", vjob->entity->name,
                    vjob->id, vjob->in_buf, vjob->out_buf);
            M_PANIC("SCL: input buf or output buf is 0\n");
        }
    }
}

STATIC_INLINE int fscaler_vg_vjob_get_type(struct fscaler_entity *ent, unsigned long arg)
{
    int type = FSCFD_JOBTYPE_REJECT;
    int up_type = FSCFD_STREAM_TYPE_NONE;
    int up_subtype = FSCFD_STREAM_SUBTYPE_NONE;
    struct v_job_t *vjob = (struct v_job_t *)arg;
    struct v_graph_info *vinfo = vjob ? (struct v_graph_info *)vjob->in_header->addr_va : NULL;
    DecoderOut_info *decinfo =
        vinfo ? (DecoderOut_info *) ((void *)vinfo + vinfo->drvinfo_offset) : NULL;
    M_DEVELOPP("SCL: vjob:%p vinfo:%p decinfo:%p\n", vjob, vinfo, decinfo);
    if (ent && ent->prop) {
        type = fscaler_property_jobtype(ent->prop);
        if (vinfo) {
            if (vinfo->tag != VIDEOGRAPH_TAG) {
                type = FSCFD_JOBTYPE_REJECT;
            } else {
#if (PLATFORM_VERSION==8181)
                up_type = fscaler_property_upstream_type(ent->prop);
                if (FSCFD_STREAM_TYPE_DECODER == up_type) {
                    up_subtype = fscaler_property_upstream_subtype(ent->prop);
                    if (FSCFD_STREAM_SUBTYPE_TB == up_subtype) {
                        if (decinfo && decinfo->HeightOrg != decinfo->Height) {
                            type = FSCFD_JOBTYPE_DROP;
                        }
                    } else if (FSCFD_STREAM_SUBTYPE_HTB == up_subtype) {
                        if (decinfo && decinfo->HeightOrg == decinfo->Height) {
                            type = FSCFD_JOBTYPE_DROP;
                        }
                    }
                }
#elif (PLATFORM_VERSION==8126)
                UNUSED_ARG(up_type);
                UNUSED_ARG(up_subtype);
                UNUSED_ARG(decinfo);
#endif /* PLATFORM_VERSION */
            }
        }
    }
    M_DEVELOPP("SCL: job type:%d\n", type);
    return type;
}

STATIC_INLINE struct fscaler_job *fscaler_vg_vjob_alloc(struct fscaler_entity *ent,
                                                        unsigned long arg)
{
    struct fscaler_job *job = NULL;
    struct fscaler_table *table = NULL;
    int type = fscaler_vg_vjob_get_type(ent, arg);
    if (ent) {
        table = fscaler_table_to_table(fscaler_entity_to_table(ent), fscaler_job_table_name());
        switch (type) {
        case FSCFD_JOBTYPE_SINGLE:
            job = fscaler_job_request(table, ent->fd, FSCALER_JOB_TYPE_SINGLE, 1);
            break;
        case FSCFD_JOBTYPE_TWICE:
            job = fscaler_job_request(table, ent->fd, FSCALER_JOB_TYPE_TWICE, 2);
            break;
        case FSCFD_JOBTYPE_TRIPLE:
            job = fscaler_job_request(table, ent->fd, FSCALER_JOB_TYPE_TRIPLE, 3);
            break;
        case FSCFD_JOBTYPE_PASS:
            job = fscaler_job_request(table, ent->fd, FSCALER_JOB_TYPE_PASS, 1);
            break;
        case FSCFD_JOBTYPE_DROP:
            job = fscaler_job_request(table, ent->fd, FSCALER_JOB_TYPE_DROP, 1);
            break;
        case FSCFD_JOBTYPE_REJECT:
        default:
            break;
        }
    }
    return job;
}

STATIC_INLINE int fscaler_vg_vjob_setup(struct fscaler_job *job, struct fscaler_property *prop,
                                        unsigned long arg)
{
    int ret = FSCALER_OK, i = 0;
    struct v_job_t *vjob = (struct v_job_t *)arg;
    struct fscaler_subjob *sjob = NULL;
    if (unlikely(NULL == job || NULL == prop || NULL == vjob)) {
        M_ERROR("SCL: Reject job, job:%p prop:%p, vjob:%p\n", job, prop, vjob);
        ret = FSCALER_ERR_REJECTED;
        goto ERROR_EXIT;
    }
    if (FSCALER_JOB_TYPE_DROP == job->type) {
        M_DEVELOPP("SCL: Reject job, type:FSCALER_JOB_TYPE_DROP\n");
        ret = FSCALER_ERR_DROPPED;
        goto ERROR_EXIT;
    }
    if (unlikely(NULL == vjob->in_buf || NULL == vjob->out_buf)) {
        M_ERROR("SCL: Reject job, in_buf:%p, out_buf:%p\n", vjob->in_buf, vjob->out_buf);
        ret = FSCALER_ERR_REJECTED;
        goto ERROR_EXIT;
    }
    fscaler_job_set_timestamp(job, vjob->itime, vjob->otime);
    fscaler_job_set_priority(job, vjob->priority - 1, 100);
    for (i = 0; i < job->max; ++i) {
        sjob = fscaler_job_get_subjob(job, i);
        if (unlikely(NULL == sjob)) {
            M_ERROR("SCL: Reject job, sjob:%d is NULL\n", i);
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        ret = fscaler_vg_set_source_param(job->type, sjob, prop);
        if (FSCALER_FAILED(ret)) {
            M_ERROR("SCL: Failed to set source parameter\n");
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        if (!fscaler_vg_check_source_layout(sjob)) {
            M_ERROR("SCL: Reject job, invalid source layout:%d\n", sjob->in_buf.layout);
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        ret = fscaler_vg_set_target_param(job->type, sjob, prop);
        if (FSCALER_FAILED(ret)) {
            M_ERROR("SCL: Failed to set target parameter\n");
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        if (!fscaler_vg_check_target_layout(sjob)) {
            M_ERROR("SCL: Reject job, invalid target layout:%d\n", sjob->out_buf.layout);
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        ret = fscaler_vg_set_source_frame(job->type, sjob, vjob, prop);
        if (FSCALER_FAILED(ret)) {
            M_ERROR("SCL: Failed to set source frame\n");
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
        ret = fscaler_vg_set_target_frame(job->type, sjob, vjob, prop);
        if (FSCALER_FAILED(ret)) {
            M_ERROR("SCL: Failed to set target frame\n");
            ret = FSCALER_ERR_REJECTED;
            goto ERROR_EXIT;
        }
    }
    fscaler_property_snapshot_decrease(prop);
    fscaler_job_set_private_data(job, (unsigned long)vjob);
    M_DEVELOPP("SCL: setup job OK\n");
    return ret;

  ERROR_EXIT:
    return ret;
}

STATIC_INLINE void fscaler_vg_vjob_callback_register(struct fscaler_job *job)
{
    if (job) {
        fscaler_job_callback_register(job, fscaler_vg_do_done,
                                      fscaler_vg_do_abort, fscaler_vg_do_error,
                                      fscaler_vg_do_destruct);
    }
}

STATIC_INLINE int fscaler_vg_putjob(struct fscaler_entity *ent, unsigned long arg)
{
    int ret = JOB_STATUS_ONGOING, hr = 0;
    struct v_job_t *vjob = (struct v_job_t *)arg;
    struct fscaler_job *job = NULL;
    struct fscaler_table *table = NULL;
    fscaler_vg_check_vjob(vjob);
    if (!ent || !ent->prop || !vjob) {
        M_ERROR("SCL: resource leakage, JOB_STATUS_FAIL\n");
        ret = JOB_STATUS_FAIL;
        goto ERROR_EXIT;
    }
    if (vjob->swap) {
        M_DEBUG("SCL: Entity:%s fd:%d, vid:%d swap\n",
                (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id);
        fscaler_property_set(ent->prop, ID_SWAP, 1);
    }
    if (0 < fscaler_property_get(ent->prop, ID_SWAP)) {
        if (!vjob->swap) {
            M_DEBUG("SCL: Entity:%s fd:%d, vid:%d fake swap\n",
                    (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id);
        }
        M_DEBUG("SCL: Entity:%s fd:%d, vid:%d commit properties\n",
                (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id);
        fscaler_property_commit(ent->prop);
        fscaler_property_set(ent->prop, ID_SWAP, 0);
    }
    M_DEVELOPP("SCL: ent:%p, fd:%d\n", ent, ent->fd);
    M_DEVELOPP("SCL: prop:%p, fd:%d\n", ent->prop, ent->prop->fd);
    table = fscaler_entity_to_table(ent);
    if ((0 == fscaler_property_get(ent->prop, ID_SNAPSHOT)) ||
        fscaler_property_get(ent->prop, ID_DISABLE)) {
        M_DEBUG("SCL: Entity:%s fd:%d, vid:%d skip, snap:%d, disable:%d\n",
                (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id,
                fscaler_property_get(ent->prop, ID_SNAPSHOT),
                fscaler_property_get(ent->prop, ID_DISABLE));
        ret = JOB_STATUS_DISABLE;
    } else {
        M_DEBUG("SCL: Entity:%s fd:%d, vid:%d receive, snap:%d, disable:%d\n",
                (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id,
                fscaler_property_get(ent->prop, ID_SNAPSHOT),
                fscaler_property_get(ent->prop, ID_DISABLE));
        job = fscaler_vg_vjob_alloc(ent, arg);
        if (job) {
            fscaler_job_set_magic(job, __func__);
            M_DEVELOPP("SCL: job:%p, fd:%d, type:%d \n", job, job ? job->fd : -1,
                       job ? job->type : -1);
        }
        hr = fscaler_vg_vjob_setup(job, ent->prop, arg);
        if (FSCALER_FAILED(hr)) {
            if (FSCALER_ERR_REJECTED == hr) {
                M_DEVELOPP("SCL: Reject scaler job\n");
                ret = JOB_STATUS_FAIL;
            } else if (FSCALER_ERR_DROPPED == hr) {
                M_DEVELOPP("SCL: Disable scaler job\n");
                ret = JOB_STATUS_DISABLE;
            }
            goto ERROR_EXIT_VJOB_FREE;
        }
        fscaler_vg_vjob_callback_register(job);
        M_DEBUG("SCL: Entity:%s fd:%d, vid:%d dispatch, time:[%x,%x]\n",
                (vjob->entity ? vjob->entity->name : ""), ent->fd, vjob->id,
                fscaler_job_start_time(job), fscaler_job_end_time(job));
        fscaler_driver_dispatch(ent, job);
        ret = JOB_STATUS_ONGOING;
    }
    return ret;

  ERROR_EXIT_VJOB_FREE:
    /* NOTICE: Only setup job ok, the on destruction callabck is work */
    fscaler_job_remove(fscaler_job_to_table(job), job);
  ERROR_EXIT:
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
void fscaler_vg_do_abort(struct fscaler_job *job)
{
    struct v_job_t *vjob = job ? (struct v_job_t *)job->data : NULL;
    if (vjob) {
        M_DEVELOPP("SCL: abort:%x\n", vjob->itime);
    }
}

void fscaler_vg_do_error(struct fscaler_job *job)
{
    struct v_job_t *vjob = job ? (struct v_job_t *)job->data : NULL;
    if (vjob) {
        M_DEVELOPP("SCL: error:%x\n", vjob->itime);
    }
}

void fscaler_vg_do_done(struct fscaler_job *job)
{
    struct v_job_t *vjob = job ? (struct v_job_t *)job->data : NULL;
    if (vjob) {
        M_DEVELOPP("SCL: done:%x\n", vjob->itime);
    }
}

void fscaler_vg_do_destruct(struct fscaler_job *job)
{
    struct v_job_t *vjob = job ? (struct v_job_t *)job->data : NULL;
    if (vjob) {
        switch (job->status) {
        case FSCALER_JOB_STATUS_DONE:
            M_DEVELOPP("SCL:  JOB_STATUS_FINISH, %d %d\n", job->fd, vjob->id);
            vjob->callback(vjob, JOB_STATUS_FINISH);
            break;
        case FSCALER_JOB_STATUS_ERROR:
            printk("SCL: device error, JOB_STATUS_FAIL, %d %d\n", job->fd, vjob->id);
            fscaler_job_callstack("error", job);
            vjob->callback(vjob, JOB_STATUS_FAIL);
            break;
        case FSCALER_JOB_STATUS_ABORT:
            printk("SCL: device abort, JOB_STATUS_FAIL, %d %d\n", job->fd, vjob->id);
            fscaler_job_callstack("abort", job);
            vjob->callback(vjob, JOB_STATUS_FAIL);
            break;
        case FSCALER_JOB_STATUS_ONPROC:
            fscaler_job_callstack("onproc", job);
            M_PANIC("SCL: Destroy on process job\n");
        case FSCALER_JOB_STATUS_IDLE:
            //printk("SCL: device idle, JOB_STATUS_FAIL, %d %d, status:%d\n", job->fd, vjob->id,
            //       job->status);
            //fscaler_job_callstack("idle", job);
            vjob->callback(vjob, JOB_STATUS_FAIL);
            break;
        default:
            printk("SCL: device unkown status, JOB_STATUS_FAIL, %d %d, status:%d\n", job->fd,
                   vjob->id, job->status);
            fscaler_job_callstack("unkown", job);
            vjob->callback(vjob, JOB_STATUS_FAIL);
            break;
        }
    } else {
        if (job) {
            fscaler_job_callstack("v_job 0", job);
            printk("SCL: destruct, no v_job_t, %d\n", job->fd);
        }
    }
}

int fscaler_vg_fops_putjob(void *data)
{
    int hr = 0;
    int fd = VJOB_TO_FD(data);
    struct fscaler_entity *ent = fscaler_vg_get_entity(fd);
    if (ent) {
        M_DEVELOPP("SCL: ent:%p fd:%d(%d,%d), opened:%d\n",
                   ent, ent->fd, ent->prop ? ent->prop->fd : -1, fd, ent->opened);
        if (!ent->opened) {
            hr = fscaler_vg_open(ent);
            if (FSCALER_FAILED(hr)) {
                printk("SCL: Failed to open device, JOB_STATUS_FAIL");
                goto ERROR_EXIT;
            }
        }
        if (!ent->opened) {
            printk("SCL: Device is not open, JOB_STATUS_FAIL");
            goto ERROR_EXIT;
        }
        return fscaler_vg_putjob(ent, (unsigned long)data);
    }
  ERROR_EXIT:
    return JOB_STATUS_FAIL;
}

int fscaler_vg_fops_flushjob(void *data)
{
    int fd = VENTITY_TO_FD(data);
    if (fd >= 0) {
        M_DEVELOPP("SCL:  flush, fd:%d isn't flushable\n", fd);
        return JOB_STATUS_ONGOING;
    }
    return JOB_STATUS_ONGOING;
}

int fscaler_vg_fops_stop(void *data)
{
    int fd = VENTITY_TO_FD(data);
    struct fscaler_entity *ent = fscaler_vg_get_entity(fd);
    if (ent && ent->opened) {
        fscaler_property_snapshot_reset(ent->prop);
        fscaler_vg_close(ent);
    }
    return JOB_STATUS_FINISH;
}

unsigned int fscaler_vg_fops_getproperty(void *data, unsigned int id)
{
    int fd = VENTITY_TO_FD(data);
    struct fscaler_entity *ent = fscaler_vg_get_entity(fd);
    if (ent) {
        return fscaler_property_get(ent->prop, id);
    }
    printk("SCL: prop entity null when get property, JOB_STATUS_FAIL");
    return JOB_STATUS_FAIL;
}

#define DIV_4_BUG_ON(v, name) do {   \
    if (v%4) {    \
        M_DEVELOPP("SCL:  %s is not multiple of 4\n", name);  \
    }   \
} while (0)
#define DIV_2_BUG_ON(v, name) do {   \
    if (v%2) {    \
        M_DEVELOPP("SCL:  %s is not multiple of 2\n", name);  \
        v -= 1; \
    }   \
} while (0)
int fscaler_vg_fops_setproperty(void *data, unsigned int id, unsigned int value)
{
    int fd = VENTITY_TO_FD(data);
    struct fscaler_entity *ent = fscaler_vg_get_entity(fd);
    M_DEVELOPP("##vgs## %d %d %u\n", fd, id, value);
    if (ent && ent->prop) {
        switch (id) {
        case ID_ENABLE:
        case ID_DISABLE:
            M_DEVELOPP("ch %d, display %s ========\n", fd, id == ID_ENABLE ? "enable" : "disable");
            fscaler_property_set(ent->prop, id, 1);
            fscaler_property_snapshot_reset(ent->prop);
            break;
        case ID_SWC_WIDTH:
        case ID_CROP_WIDTH:
            DIV_4_BUG_ON(value, fscaler_property_name(ent->prop, id));
        case ID_SWC_HEIGHT:
        case ID_CROP_HEIGHT:
        case ID_SWC_AXIS_X:
        case ID_SWC_AXIS_Y:
            DIV_2_BUG_ON(value, fscaler_property_name(ent->prop, id));
            fscaler_property_set(ent->prop, id, value);
            break;
        default:
            fscaler_property_set(ent->prop, id, value);
            break;
        }
    }
    return JOB_STATUS_ONGOING;
}

int fscaler_vg_fops_queryid(void *data, char *name)
{
    UNUSED_ARG(data);
    return fscaler_property_query_id(name);
}

void fscaler_vg_uninstall(struct fscaler_interface *inter)
{
    int hr = 0;
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    if (drv && fscaler_interface_installed(inter)) {
        hr = videograph_device_deregister(&FSCALER_VG_DEVICE);
        if (FSCALER_FAILED(hr)) {
            M_ERROR("SCL: Failed to uninstall videograph interface\n");
        }
        fscaler_vg_set_owner(NULL);
    }
}

int fscaler_vg_install(struct fscaler_interface *inter)
{
    int i = 0, hr = 0;
    struct fscaler_property *prop = NULL;
    struct fscaler_table *table = NULL;
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    if (drv && !fscaler_interface_installed(inter)) {
        videograph_device_init(&FSCALER_VG_DEVICE, max_entity);
        hr = videograph_device_register(&FSCALER_VG_DEVICE);
        if (FSCALER_SUCCEED(hr)) {
            table = fscaler_driver_table_find(drv, fscaler_property_table_name());
            if (table) {
                for (i = 0; i < table->max; ++i) {
                    prop = fscaler_property_find(table, i);
                    if (!prop) {
                        break;
                    }
                    fscaler_property_set(prop, ID_DISABLE, 1);
                    fscaler_property_snapshot_reset(prop);
                }
                if (i >= table->max) {
                    fscaler_vg_set_owner(drv);
                    return FSCALER_OK;
                }
            }
            videograph_device_deregister(&FSCALER_VG_DEVICE);
        }
    }
    return FSCALER_ERR_INSTALL;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORTED FUCNTION IMPLAMENTATION
/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_vg_interface_request(struct fscaler_driver *drv)
{
    int hr = 0;
    hr = fscaler_interface_request(drv, &FSCALER_VG_NAME[0], supp_vg,
                                   fscaler_vg_install, fscaler_vg_uninstall);
    if (FSCALER_SUCCEED(hr)) {
        return FSCALER_OK;
    }
    return FSCALER_ERR_INSTALL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
void fscaler_vg_interface_remove(struct fscaler_driver *drv)
{
    struct fscaler_interface *inter = fscaler_driver_interface_find(drv, &FSCALER_VG_NAME[0]);
    if (inter) {
        fscaler_interface_remove(inter);
    }
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
const char *fscaler_vg_interface_name(void)
{
    return &FSCALER_VG_NAME[0];
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_vg_interface_request);
EXPORT_SYMBOL(fscaler_vg_interface_remove);
EXPORT_SYMBOL(fscaler_vg_interface_name);
