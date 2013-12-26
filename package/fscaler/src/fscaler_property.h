/**
 * @file fscaler_property.h
 * @brief Scaler property system
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_PROPERTY_H_
#define _FSCALER_PROPERTY_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_driver;
struct fscaler_table;
struct fscaler_property;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_table.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/spinlock.h>

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO

#define FSCFD_JOBTYPE_SINGLE    0       ///< Single job
#define FSCFD_JOBTYPE_TWICE     1       ///< Twice job
#define FSCFD_JOBTYPE_TRIPLE	2       ///< Triple job
#define FSCFD_JOBTYPE_PASS      3       ///< Pass job, do nothing then pass to scaler engine and downstream (callback JOB_STATUS_FINISH)
#define FSCFD_JOBTYPE_REJECT    4       ///< Reject job, pass to downstream directly (callback JOB_STATUS_FAIL)
#define FSCFD_JOBTYPE_DROP      5       ///< Drop job, pass to videograph manager (callback JOB_STATUS_DISABLE)
#define FSCFD_JOBTYPE_MAX       6       ///< Max

#define FSCFD_STREAM_TYPE(major,minor)          (int)(((unsigned int)(major)<<8)|((unsigned int)(minor)))
#define FSCFD_STREAM_GET_TYPE(type)             (int)(((unsigned int)(type)&0x0000FF00)>>8)
#define FSCFD_STREAM_GET_SUBTYPE(type)          (int)((unsigned int)(type)&0x000000FF)
#define FSCFD_STREAM_IS_TYPE(type,major)        (int)(FSCFD_STREAM_GET_TYPE(type) == (int)major)
#define FSCFD_STREAM_IS_SUBTYPE(type,sub)       (int)(FSCFD_STREAM_GET_SUBTYPE(type) == (int)sub)

#define FSCFD_STREAM_TYPE_NULL                  0x00    ///< Any stream type
#define FSCFD_STREAM_TYPE_NONE                  0x01    ///< none stream type
#define FSCFD_STREAM_TYPE_CAPTURE               0x02    ///< Capture stream type
#define FSCFD_STREAM_TYPE_ENCODER               0x03    ///< Encoder stream type
#define FSCFD_STREAM_TYPE_DECODER               0x04    ///< Decoder stream type
#define FSCFD_STREAM_TYPE_SCALER                0x05    ///< Scaler stream type
#define FSCFD_STREAM_TYPE_3DI                   0x06    ///< 3DI stream type
#define FSCFD_STREAM_TYPE_LCD                   0x07    ///< LCD stream type

#define FSCFD_STREAM_SUBTYPE_NULL               0x00    ///< Any stream subtype
#define FSCFD_STREAM_SUBTYPE_NONE               0x01    ///< None stream subtype
#define FSCFD_STREAM_SUBTYPE_TB                 0x02    ///< Top/Bottom Y422 stream subtype
#define FSCFD_STREAM_SUBTYPE_HTB                0x03    ///< half Top/Bottom Y422 stream subtype

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
enum {
    ID_NULL = 0,
    ID_JOBTYPE,
    ID_UPSTREAM,
    ID_DOWNSTREAM,
    ID_RUN,
    ID_DISABLE,
    ID_ENABLE,
    ID_SYNC_MODE,
    ID_SNAPSHOT,
    ID_DES_FMT,
    ID_DES_SWAP_CBCR,
    ID_DES_ORDER,
    ID_DES_LEVEL,
    ID_DES_SWAP_Y,
    ID_SRC_FMT,
    ID_SRC_HBLANK,
    ID_SRC_INTERLEAVED,
    ID_SRC_SWAP_CBCR,
    ID_SRC_SWAP_Y,
    ID_ALBUM,
    ID_NON_LINEAR,
    ID_DITHER,
    ID_US_EN,
    ID_CORRECTION,
    ID_SWAP,

    ID_SRC_BUF_OFFSET,
    ID_SRC_WIDTH,
    ID_SRC_HEIGHT,
    ID_DES_WIDTH,
    ID_DES_HEIGHT,

    ID_DES_AXIS_X,
    ID_DES_AXIS_Y,
    ID_SWC_AXIS_X,
    ID_SWC_AXIS_Y,
    ID_SWC_WIDTH,
    ID_SWC_HEIGHT,
    ID_CROP_AXIS_X,
    ID_CROP_AXIS_Y,
    ID_CROP_WIDTH,
    ID_CROP_HEIGHT,
    ID_VP_WIDTH,
    ID_VP_HEIGHT,
    ID_TARGET_WIDTH,
    ID_TARGET_HEIGHT,
    ID_MEM_SRC_PIT,
    ID_MEM_SRC_SAMPLE_DOWN,
    ID_DES_BUF_OFFSET,
    ID_MEM_DES_SAMPLE_DOWN,

    // twice
    ID_TWICE_EN,
    ID_DES_AXIS_X_2,
    ID_DES_AXIS_Y_2,
    ID_SWC_AXIS_X_2,
    ID_SWC_AXIS_Y_2,
    ID_SWC_WIDTH_2,
    ID_SWC_HEIGHT_2,
    ID_CROP_AXIS_X_2,
    ID_CROP_AXIS_Y_2,
    ID_CROP_WIDTH_2,
    ID_CROP_HEIGHT_2,
    ID_VP_WIDTH_2,
    ID_VP_HEIGHT_2,
    ID_TARGET_WIDTH_2,
    ID_TARGET_HEIGHT_2,
    ID_MEM_SRC_PIT_2,
    ID_MEM_SRC_SAMPLE_DOWN_2,
    ID_DES_BUF_OFFSET_2,
    ID_MEM_DES_SAMPLE_DOWN_2,

    // triple
    ID_TRIPLE_EN,
    ID_DES_AXIS_X_3,
    ID_DES_AXIS_Y_3,
    ID_SWC_AXIS_X_3,
    ID_SWC_AXIS_Y_3,
    ID_SWC_WIDTH_3,
    ID_SWC_HEIGHT_3,
    ID_CROP_AXIS_X_3,
    ID_CROP_AXIS_Y_3,
    ID_CROP_WIDTH_3,
    ID_CROP_HEIGHT_3,
    ID_VP_WIDTH_3,
    ID_VP_HEIGHT_3,
    ID_TARGET_WIDTH_3,
    ID_TARGET_HEIGHT_3,
    ID_MEM_SRC_PIT_3,
    ID_MEM_SRC_SAMPLE_DOWN_3,
    ID_DES_BUF_OFFSET_3,
    ID_MEM_DES_SAMPLE_DOWN_3,

    ID_MAX,
};

struct fscaler_property {
    struct fscaler_table *parent;
    struct klist_node knode_table;
    unsigned int (*get) (struct fscaler_property *, int);
    void (*set) (struct fscaler_property *, int, unsigned int);
    void (*commit) (struct fscaler_property *);
    int fd;
    int attached;
    unsigned int dirty[ID_MAX];
    unsigned int value[ID_MAX];
    spinlock_t lock;
};

///////////////////////////////////////////////////////////////////////////////
// EXTERN PROTOTYPE
extern int fscaler_property_table_request(struct fscaler_driver *drv);
extern void fscaler_property_table_remove(struct fscaler_driver *drv);
extern const char *fscaler_property_table_name(void);
extern int fscaler_property_query_id(const char *name);
extern const char *fscaler_property_name(int id);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline struct fscaler_table *fscaler_property_to_table(struct fscaler_property *prop)
{
    return prop ? prop->parent : NULL;
}

static inline void fscaler_property_callback_register(struct fscaler_property *prop,
                                                      unsigned int (*get) (struct fscaler_property
                                                                           *, int),
                                                      void (*set) (struct fscaler_property *, int,
                                                                   unsigned int),
                                                      void (*commit) (struct fscaler_property *))
{
    if (prop) {
        prop->get = get;
        prop->set = set;
        prop->commit = commit;
    }
}

static inline struct fscaler_property *fscaler_property_find(struct fscaler_table *table, int fd)
{
    return (struct fscaler_property *)fscaler_table_item_find(table, fd);
}

static inline unsigned int fscaler_property_get(struct fscaler_property *prop, int id)
{
    return (prop && prop->get && ID_NULL < id
            && ID_MAX > id) ? prop->get(prop, id) : (unsigned int)-1;
}

static inline void fscaler_property_set(struct fscaler_property *prop, int id, unsigned int value)
{
    unsigned long flags;
    if (prop && prop->set && ID_NULL < id && ID_MAX > id) {
        spin_lock_irqsave(&prop->lock, flags);
        prop->set(prop, id, value);
        spin_unlock_irqrestore(&prop->lock, flags);
    }
}

static inline void fscaler_property_commit(struct fscaler_property *prop)
{
    unsigned long flags;
    if (prop && prop->commit) {
        spin_lock_irqsave(&prop->lock, flags);
        prop->commit(prop);
        spin_unlock_irqrestore(&prop->lock, flags);
    }
}

static inline void fscaler_property_snapshot_reset(struct fscaler_property *prop)
{
    fscaler_property_set(prop, ID_SNAPSHOT, (unsigned int)-1);
}

static inline void fscaler_property_snapshot_decrease(struct fscaler_property *prop)
{
    int snap = 0;
    unsigned long flags;
    if (prop && prop->get && prop->set) {
        spin_lock_irqsave(&prop->lock, flags);
        snap = (int)prop->get(prop, ID_SNAPSHOT);
        if (snap > 0) {
            --snap;
            prop->set(prop, ID_SNAPSHOT, snap);
        }
        spin_unlock_irqrestore(&prop->lock, flags);
    }
}

static inline int fscaler_property_jobtype(struct fscaler_property *prop)
{
    return fscaler_property_get(prop, ID_JOBTYPE);
}

static inline int fscaler_property_upstream_type(struct fscaler_property *prop)
{
    return FSCFD_STREAM_GET_TYPE(fscaler_property_get(prop, ID_UPSTREAM));
}

static inline int fscaler_property_downstream_type(struct fscaler_property *prop)
{
    return FSCFD_STREAM_GET_TYPE(fscaler_property_get(prop, ID_DOWNSTREAM));
}

static inline int fscaler_property_upstream_subtype(struct fscaler_property *prop)
{
    return FSCFD_STREAM_GET_SUBTYPE(fscaler_property_get(prop, ID_UPSTREAM));
}

static inline int fscaler_property_downstream_subtype(struct fscaler_property *prop)
{
    return FSCFD_STREAM_GET_SUBTYPE(fscaler_property_get(prop, ID_DOWNSTREAM));
}

#endif /* _FSCALER_PROPERTY_H_ */
