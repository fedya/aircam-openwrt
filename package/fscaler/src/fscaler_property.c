/**
 * @file fscaler_property.c
 * @brief Scaler property table
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.06
 * @date 2010 Nov 15
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_property.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static int fscaler_property_cache(struct fscaler_table *table);
static void fscaler_property_uncache(struct fscaler_table *table);
static void *fscaler_property_request(struct fscaler_table *table, int index);
static void fscaler_property_remove(struct fscaler_table *table, void *data);
static unsigned int fscaler_property_get_value(struct fscaler_property *prop, int id);
static void fscaler_property_set_value(struct fscaler_property *prop, int id, unsigned int value);
static void fscaler_property_commit_values(struct fscaler_property *prop);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_PROPERTY_TABLE_NAME[] = "fscprop";

const char *FSCALER_PROPERTY_NAMES[] = {
    [ID_NULL] = "",
    [ID_JOBTYPE] = "job_type",
    [ID_UPSTREAM] = "upstream",
    [ID_DOWNSTREAM] = "downstream",
    [ID_RUN] = "run",
    [ID_DISABLE] = "disable_scale",
    [ID_ENABLE] = "enable_scale",
    [ID_SYNC_MODE] = "sync_mode",
    [ID_SNAPSHOT] = "snapshot",
    [ID_DES_FMT] = "des_fmt",
    [ID_DES_SWAP_CBCR] = "des_swap_CbCr",
    [ID_DES_ORDER] = "des_order",
    [ID_DES_LEVEL] = "des_level",
    [ID_DES_SWAP_Y] = "des_swap_Y",
    [ID_SRC_FMT] = "src_fmt",
    [ID_SRC_HBLANK] = "src_hblank",
    [ID_SRC_INTERLEAVED] = "src_interleaved",
    [ID_SRC_SWAP_CBCR] = "src_swap_CbCr",
    [ID_SRC_SWAP_Y] = "src_swap_Y",
    [ID_ALBUM] = "album",
    [ID_NON_LINEAR] = "non_linear",
    [ID_DITHER] = "dither",
    [ID_US_EN] = "us_en",
    [ID_CORRECTION] = "correction",
    [ID_SWAP] = "swap",

    [ID_SRC_BUF_OFFSET] = "buf_offset",
    [ID_SRC_WIDTH] = "src_width",
    [ID_SRC_HEIGHT] = "src_height",
    [ID_DES_WIDTH] = "des_width",
    [ID_DES_HEIGHT] = "des_height",

    [ID_DES_AXIS_X] = "des_axis_x",
    [ID_DES_AXIS_Y] = "des_axis_y",
    [ID_SWC_AXIS_X] = "swc_axis_x",
    [ID_SWC_AXIS_Y] = "swc_axis_y",
    [ID_SWC_WIDTH] = "swc_width",
    [ID_SWC_HEIGHT] = "swc_height",
    [ID_CROP_AXIS_X] = "crop_axis_x",
    [ID_CROP_AXIS_Y] = "crop_axis_y",
    [ID_CROP_WIDTH] = "crop_width",
    [ID_CROP_HEIGHT] = "crop_height",
    [ID_VP_WIDTH] = "vp_width",
    [ID_VP_HEIGHT] = "vp_height",
    [ID_TARGET_WIDTH] = "target_width",
    [ID_TARGET_HEIGHT] = "target_height",
    [ID_MEM_SRC_PIT] = "mem_src_pit",
    [ID_MEM_SRC_SAMPLE_DOWN] = "mem_src_sample_down",
    [ID_DES_BUF_OFFSET] = "des_buf_offset",
    [ID_MEM_DES_SAMPLE_DOWN] = "mem_des_sample_down",

    [ID_TWICE_EN] = "twice_enable",
    [ID_DES_AXIS_X_2] = "des_second_axis_x",
    [ID_DES_AXIS_Y_2] = "des_second_axis_y",
    [ID_SWC_AXIS_X_2] = "swc_second_axis_x",
    [ID_SWC_AXIS_Y_2] = "swc_second_axis_y",
    [ID_SWC_WIDTH_2] = "swc_second_width",
    [ID_SWC_HEIGHT_2] = "swc_second_height",
    [ID_CROP_AXIS_X_2] = "crop_second_axis_x",
    [ID_CROP_AXIS_Y_2] = "crop_second_axis_y",
    [ID_CROP_WIDTH_2] = "crop_second_width",
    [ID_CROP_HEIGHT_2] = "crop_second_height",
    [ID_VP_WIDTH_2] = "vp_second_width",
    [ID_VP_HEIGHT_2] = "vp_second_height",
    [ID_TARGET_WIDTH_2] = "target_second_width",
    [ID_TARGET_HEIGHT_2] = "target_second_height",
    [ID_MEM_SRC_PIT_2] = "mem_src_second_pit",
    [ID_MEM_SRC_SAMPLE_DOWN_2] = "mem_src_second_sample_down",
    [ID_DES_BUF_OFFSET_2] = "des_second_buf_offset",
    [ID_MEM_DES_SAMPLE_DOWN_2] = "mem_des_second_sample_down",

    [ID_TRIPLE_EN] = "triple_enable",
    [ID_DES_AXIS_X_3] = "des_third_axis_x",
    [ID_DES_AXIS_Y_3] = "des_third_axis_y",
    [ID_SWC_AXIS_X_3] = "swc_third_axis_x",
    [ID_SWC_AXIS_Y_3] = "swc_third_axis_y",
    [ID_SWC_WIDTH_3] = "swc_third_width",
    [ID_SWC_HEIGHT_3] = "swc_third_height",
    [ID_CROP_AXIS_X_3] = "crop_third_axis_x",
    [ID_CROP_AXIS_Y_3] = "crop_third_axis_y",
    [ID_CROP_WIDTH_3] = "crop_third_width",
    [ID_CROP_HEIGHT_3] = "crop_third_height",
    [ID_VP_WIDTH_3] = "vp_third_width",
    [ID_VP_HEIGHT_3] = "vp_third_height",
    [ID_TARGET_WIDTH_3] = "target_third_width",
    [ID_TARGET_HEIGHT_3] = "target_third_height",
    [ID_MEM_SRC_PIT_3] = "mem_src_third_pit",
    [ID_MEM_SRC_SAMPLE_DOWN_3] = "mem_src_third_sample_down",
    [ID_DES_BUF_OFFSET_3] = "des_third_buf_offset",
    [ID_MEM_DES_SAMPLE_DOWN_3] = "mem_des_third_sample_down",
};

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE struct fscaler_property *fscaler_property_allocate(struct fscaler_table *table,
                                                                 int index)
{
    struct fscaler_property *prop = NULL;
    if (table && table->map && table->kmem && 0 <= index && table->max > index) {
        prop = FSCALER_KMEM_ALLOC(table->kmem);
        if (prop) {
            prop->parent = table;
            prop->fd = index;
            memset(prop->dirty, 0, sizeof(prop->dirty));
            memset(prop->value, 0, sizeof(prop->value));
            prop->dirty[ID_SNAPSHOT] = prop->value[ID_SNAPSHOT] = (unsigned int)-1;
            prop->dirty[ID_SYNC_MODE] = prop->value[ID_SYNC_MODE] = 1;
            prop->dirty[ID_MEM_SRC_SAMPLE_DOWN] = prop->value[ID_MEM_SRC_SAMPLE_DOWN] = 1;
            prop->dirty[ID_MEM_SRC_SAMPLE_DOWN_2] = prop->value[ID_MEM_SRC_SAMPLE_DOWN_2] = 1;
            prop->dirty[ID_MEM_SRC_SAMPLE_DOWN_3] = prop->value[ID_MEM_SRC_SAMPLE_DOWN_3] = 1;
            prop->dirty[ID_MEM_DES_SAMPLE_DOWN] = prop->value[ID_MEM_DES_SAMPLE_DOWN] = 1;
            prop->dirty[ID_MEM_DES_SAMPLE_DOWN_2] = prop->value[ID_MEM_DES_SAMPLE_DOWN_2] = 1;
            prop->dirty[ID_MEM_DES_SAMPLE_DOWN_3] = prop->value[ID_MEM_DES_SAMPLE_DOWN_3] = 1;
            spin_lock_init(&prop->lock);
            return prop;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_property_free(struct fscaler_table *table, struct fscaler_property *prop)
{
    if (table && table->kmem && prop) {
        prop->parent = NULL;
        FSCALER_KMEM_FREE(table->kmem, (void *)prop);
        prop = NULL;
    }
}

STATIC_INLINE void fscaler_property_dump(struct fscaler_property *prop)
{
    int i = 0;
    if (prop) {
        for (i = ID_NULL; i < ID_MAX; ++i) {
            switch (i) {
            case ID_MAX:
            case ID_JOBTYPE:
            case ID_UPSTREAM:
            case ID_DOWNSTREAM:
            case ID_DISABLE:
            case ID_ENABLE:
            case ID_SYNC_MODE:
            case ID_SNAPSHOT:
            case ID_TWICE_EN:
            case ID_TRIPLE_EN:
            case ID_SWAP:
            case ID_SRC_FMT:
            case ID_VP_WIDTH:
            case ID_VP_HEIGHT:
            case ID_SWC_WIDTH:
            case ID_SWC_HEIGHT:
            case ID_MEM_SRC_SAMPLE_DOWN:
            case ID_MEM_DES_SAMPLE_DOWN:
            case ID_VP_WIDTH_2:
            case ID_VP_HEIGHT_2:
            case ID_SWC_WIDTH_2:
            case ID_SWC_HEIGHT_2:
            case ID_MEM_SRC_SAMPLE_DOWN_2:
            case ID_MEM_DES_SAMPLE_DOWN_2:
            case ID_VP_WIDTH_3:
            case ID_VP_HEIGHT_3:
            case ID_SWC_WIDTH_3:
            case ID_SWC_HEIGHT_3:
            case ID_MEM_SRC_SAMPLE_DOWN_3:
            case ID_MEM_DES_SAMPLE_DOWN_3:
                M_DEBUG("SCL: ch:%d, %s:%d[%d]\n", prop->fd, FSCALER_PROPERTY_NAMES[i],
                        prop->value[i], prop->dirty[i]);
                break;
            default:
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_property_cache(struct fscaler_table *table)
{
    if (table) {
        table->kmem = FSCALER_KMEM_CACHE(fscaler_property, SLAB_HWCACHE_ALIGN);
        if (table->kmem) {
            return FSCALER_OK;
        }
    }
    return FSCALER_ERR_CREATE;
}

void fscaler_property_uncache(struct fscaler_table *table)
{
    if (table && table->kmem) {
        FSCALER_KMEM_UNCACHE(table->kmem);
        table->kmem = NULL;
    }
}

void *fscaler_property_request(struct fscaler_table *table, int index)
{
    unsigned long flags;
    struct fscaler_property *prop = fscaler_property_allocate(table, index);
    if (prop) {
        fscaler_property_callback_register(prop, fscaler_property_get_value,
                                           fscaler_property_set_value,
                                           fscaler_property_commit_values);
        spin_lock_irqsave(&table->lock, flags);
        klist_add_tail(&prop->knode_table, &table->klist_item);
        spin_unlock_irqrestore(&table->lock, flags);
        return prop;
    }
    return NULL;
}

void fscaler_property_remove(struct fscaler_table *table, void *data)
{
    unsigned long flags;
    struct fscaler_property *prop = (struct fscaler_property *)data;
    if (table && prop) {
        spin_lock_irqsave(&table->lock, flags);
        klist_del(&prop->knode_table);
        spin_unlock_irqrestore(&table->lock, flags);
        fscaler_property_free(table, prop);
        prop = NULL;
    }
}

unsigned int fscaler_property_get_value(struct fscaler_property *prop, int id)
{
    unsigned int value = 0;
    if (prop && id >= 0 && id < ID_MAX) {
        switch (id) {
        case ID_ENABLE:
            if (prop->value[ID_SYNC_MODE] > 0) {
                value = prop->value[id];
            } else {
                value = prop->dirty[id];
            }
            break;
        case ID_DISABLE:
            if (prop->value[ID_SYNC_MODE] > 0) {
                if (1 <= prop->value[id] && -1 == prop->dirty[ID_SNAPSHOT]) {
                    value = 1;
                } else {
                    value = 0;
                }
            } else {
                if (1 <= prop->dirty[id] && -1 == prop->dirty[ID_SNAPSHOT]) {
                    value = 1;
                } else {
                    value = 0;
                }
            }
            break;
        case ID_JOBTYPE:
        case ID_UPSTREAM:
        case ID_DOWNSTREAM:
        case ID_SYNC_MODE:
        case ID_SNAPSHOT:
        case ID_TWICE_EN:
        case ID_TRIPLE_EN:
        case ID_SWAP:
            value = prop->dirty[id];
            if (id == ID_SNAPSHOT) {
                M_DEVELOPP("<%d>get snap:%d\n", prop->fd, value);
            }
            if (id == ID_SWAP) {
                M_DEVELOPP("<%d>get swap:%d\n", prop->fd, value);
            }
            break;
        default:
            value = prop->value[id];
            break;
        }
        if (dbg_type) {
            switch (id) {
            case ID_DISABLE:
            case ID_ENABLE:
            case ID_SYNC_MODE:
            case ID_SNAPSHOT:
                M_DEBUG("SCL: ch:%d, get property %s:%d\n", prop->fd,
                        FSCALER_PROPERTY_NAMES[id], value);
                fscaler_property_dump(prop);
                break;
            default:
                break;
            }
        }
    }
    return value;
}

void fscaler_property_set_value(struct fscaler_property *prop, int id, unsigned int value)
{
    if (prop && id >= 0 && id < ID_MAX) {
        prop->dirty[id] = value;
        if (ID_DISABLE == id) {
            prop->dirty[ID_ENABLE] = value > 0 ? 0 : 1;
            fscaler_property_snapshot_reset(prop);
        }
        if (ID_ENABLE == id) {
            prop->dirty[ID_DISABLE] = value > 0 ? 0 : 1;
            fscaler_property_snapshot_reset(prop);
        }
        if (ID_TWICE_EN == id) {
            prop->dirty[ID_JOBTYPE] = value ? FSCFD_JOBTYPE_TWICE : FSCFD_JOBTYPE_SINGLE;
        }
        if (ID_JOBTYPE == id) {
            prop->dirty[ID_JOBTYPE] = value;
        }
        //if (ID_TRIPLE_EN == id) {
        //    prop->dirty[ID_JOBTYPE] = value ? FSCFD_JOBTYPE_TRIPLE : FSCFD_JOBTYPE_SINGLE;
        //}
        if (ID_MEM_SRC_SAMPLE_DOWN == id ||
            ID_MEM_SRC_SAMPLE_DOWN_2 == id || ID_MEM_SRC_SAMPLE_DOWN_3 == id) {
            if (prop->dirty[id] > 16) {
                prop->dirty[id] = 16;
            } else if (prop->dirty[id] == 0) {
                prop->dirty[id] = 1;
            }
        }
        if (ID_MEM_DES_SAMPLE_DOWN == id ||
            ID_MEM_DES_SAMPLE_DOWN_2 == id || ID_MEM_DES_SAMPLE_DOWN_3 == id) {
            if (prop->dirty[id] > 16) {
                prop->dirty[id] = 16;
            } else if (prop->dirty[id] == 0) {
                prop->dirty[id] = 1;
            }
        }
        switch (id) {
        case ID_UPSTREAM:
        case ID_DOWNSTREAM:
            //case ID_SYNC_MODE:
        case ID_SNAPSHOT:
        case ID_TWICE_EN:
        case ID_TRIPLE_EN:
        case ID_SWAP:
            prop->value[id] = value;
            if (id == ID_SNAPSHOT) {
                M_DEBUG("SCL: ch:%d, set snap:%d\n", prop->fd, value);
            }
            if (id == ID_SWAP) {
                M_DEBUG("SCL: ch:%d, set swap:%d\n", prop->fd, value);
            }
            break;
        default:
            break;
        }
        if (dbg_type) {
            switch (id) {
            case ID_DISABLE:
            case ID_ENABLE:
            case ID_SYNC_MODE:
            case ID_SNAPSHOT:
                M_DEBUG("SCL: ch:%d, set property %s:%d\n", prop->fd,
                        FSCALER_PROPERTY_NAMES[id], value);
                fscaler_property_dump(prop);
                break;
            default:
                break;
            }
        }
    }
}

void fscaler_property_commit_values(struct fscaler_property *prop)
{
    if (prop && 0 <= prop->fd && max_entity > prop->fd) {
        memcpy(prop->value, prop->dirty, sizeof(prop->dirty));
        fscaler_property_dump(prop);
    }
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
int fscaler_property_table_request(struct fscaler_driver *drv)
{
    int hr = 0;
    hr = fscaler_table_request(drv, &FSCALER_PROPERTY_TABLE_NAME[0], max_entity,
                               0, 0,
                               fscaler_property_cache, fscaler_property_uncache,
                               fscaler_property_request, fscaler_property_remove, 0);
    if (FSCALER_SUCCEED(hr)) {
        return FSCALER_OK;
    }
    return FSCALER_ERR_CREATE;
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
void fscaler_property_table_remove(struct fscaler_driver *drv)
{
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_property_table_name());
    if (table) {
        fscaler_table_remove(table);
        table = NULL;
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
const char *fscaler_property_table_name(void)
{
    return &FSCALER_PROPERTY_TABLE_NAME[0];
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
int fscaler_property_query_id(const char *name)
{
    int i = 0;
    if (name) {
        for (i = 0; i < ID_MAX; ++i) {
            if (0 == strcmp(FSCALER_PROPERTY_NAMES[i], name)) {
                return i;
            }
        }
    }
    return -1;
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
const char *fscaler_property_name(int id)
{
    if (id >= ID_NULL && id < ID_MAX) {
        return FSCALER_PROPERTY_NAMES[id];
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_property_table_request);
EXPORT_SYMBOL(fscaler_property_table_remove);
EXPORT_SYMBOL(fscaler_property_table_name);
EXPORT_SYMBOL(fscaler_property_query_id);
EXPORT_SYMBOL(fscaler_property_name);
