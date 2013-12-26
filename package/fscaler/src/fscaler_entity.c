/**
 * @file fscaler_entity.c
 * @brief Scaler entity table
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.xx
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_entity.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"
#include "fscaler_property.h"
#include "fscaler_job.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static int fscaler_entity_cache(struct fscaler_table *table);
static void fscaler_entity_uncache(struct fscaler_table *table);
static void *fscaler_entity_request(struct fscaler_table *table, int index);
static void fscaler_entity_remove(struct fscaler_table *table, void *data);
static int fscaler_entity_compare(void *c0, void *c1);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_ENTITY_TABLE_NAME[] = "fscent";
static unsigned int FSCALER_ENTITY_JOBNUM = 0;

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE struct fscaler_entity *fscaler_entity_allocate(struct fscaler_table *table, int index)
{
    struct fscaler_entity *ent = NULL;
    if (table && table->map && table->kmem && 0 <= index && table->max > index) {
        ent = FSCALER_KMEM_ALLOC(table->kmem);
        if (ent) {
            ent->parent = table;
            ent->prop =
                fscaler_property_find(fscaler_table_to_table(table, fscaler_property_table_name()),
                                      index);
            ent->fd = index;
            ent->opened = 0;
            spin_lock_init(&ent->lock);
            klist_init(&ent->klist_job, 0, 0);
            M_DEVELOPP("SCL: ent->fd:%d, ent->prop:%p, ent->prop->fd:%d\n", ent->fd, ent->prop,
                       ent->prop ? ent->prop->fd : -1);
            return ent;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_entity_free(struct fscaler_table *table, struct fscaler_entity *ent)
{
    if (table && table->kmem && ent) {
        ent->parent = NULL;
        FSCALER_KMEM_FREE(table->kmem, (void *)ent);
        ent = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_entity_cache(struct fscaler_table *table)
{
    if (table) {
        table->kmem = FSCALER_KMEM_CACHE(fscaler_entity, SLAB_HWCACHE_ALIGN);
        if (table->kmem) {
            return FSCALER_OK;
        }
    }
    return FSCALER_ERR_CREATE;
}

void fscaler_entity_uncache(struct fscaler_table *table)
{
    if (table && table->kmem) {
        FSCALER_KMEM_UNCACHE(table->kmem);
        table->kmem = NULL;
    }
}

void *fscaler_entity_request(struct fscaler_table *table, int index)
{
    unsigned long flags;
    struct fscaler_entity *ent = fscaler_entity_allocate(table, index);
    if (ent) {
        spin_lock_irqsave(&table->lock, flags);
        klist_add_tail(&ent->knode_table, &table->klist_item);
        spin_unlock_irqrestore(&table->lock, flags);
        return ent;
    }
    return NULL;
}

void fscaler_entity_remove(struct fscaler_table *table, void *data)
{
    unsigned long flags;
    struct fscaler_entity *ent = (struct fscaler_entity *)data;
    if (table && ent) {
        spin_lock_irqsave(&table->lock, flags);
        klist_del(&ent->knode_table);
        spin_unlock_irqrestore(&table->lock, flags);
        fscaler_entity_free(table, ent);
        ent = NULL;
    }
}

int fscaler_entity_compare(void *c0, void *c1)
{
    struct fscaler_entity *ent = (struct fscaler_entity *)c0;
    struct fscaler_entity *ent_found = (struct fscaler_entity *)c1;
    struct fscaler_job *job = NULL, *found = NULL;
    unsigned int time0 = 0, time1 = 0;
    if (ent) {
        if (NULL == ent_found) {
            /* First available compare job, keep it */
            return LKP_CODE(0, LKP_ACT_EXCHANGE);
        }
        job = klist_entry(klist_front(&ent->klist_job), struct fscaler_job, knode_ent);
        found = klist_entry(klist_front(&ent_found->klist_job), struct fscaler_job, knode_ent);
        if (job && found) {
            /* Compare timestamp with priority bonus, if find the more prior job, keep it */
            if (job->prio == found->prio) {
                time0 = fscaler_job_start_time(job);
                time1 = fscaler_job_start_time(found);
            } else if (job->prio < found->prio) {
                if (job->start_time >= job->bonus) {
                    time0 = job->start_time - job->bonus;
                } else {
                    time0 = 0;
                }
                time1 = found->start_time;
            } else if (job->prio > found->prio) {
                if (found->start_time >= found->bonus) {
                    time1 = found->start_time - found->bonus;
                } else {
                    time1 = 0;
                }
                time0 = job->start_time;
            }
            if (time0 != time1 && TIME_BEFORE(time0, time1)) {
                return LKP_CODE(0, LKP_ACT_EXCHANGE);
            }
        }
    }
    return LKP_CODE(0, 0);
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
int fscaler_entity_table_request(struct fscaler_driver *drv)
{
    int hr = 0;
    hr = fscaler_table_request(drv, &FSCALER_ENTITY_TABLE_NAME[0],
                               max_entity, 0, 0,
                               fscaler_entity_cache, fscaler_entity_uncache,
                               fscaler_entity_request, fscaler_entity_remove,
                               fscaler_entity_compare);
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
void fscaler_entity_table_remove(struct fscaler_driver *drv)
{
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_entity_table_name());
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
int fscaler_entity_open(struct fscaler_entity *ent)
{
    if (ent) {
        if (!ent->opened) {
            ent->opened = 1;
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_OPEN;
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
void fscaler_entity_close(struct fscaler_entity *ent)
{
    struct fscaler_job *job = NULL;
    if (ent && ent->opened) {
        job = fscaler_entity_getjob(ent);
        do {
            if (!job) {
                break;
            }
            fscaler_job_remove(fscaler_job_to_table(job), job);
            job = fscaler_entity_getjob(ent);
        } while (1);
        ent->opened = 0;
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
void fscaler_entity_putjob(struct fscaler_entity *ent, struct fscaler_job *job)
{
    unsigned long flags;
    if (ent && job) {
        spin_lock_irqsave(&ent->lock, flags);
        fscaler_job_set_magic(job, __func__);
        if (ent->opened) {
            job->fd = ent->fd;
            ++FSCALER_ENTITY_JOBNUM;
            klist_add_tail(&job->knode_ent, &ent->klist_job);
            fscaler_entity_unlock(ent);
            M_DEVELOPP("SCL: push job %p(s:%d e:%d) to entity %d, total: %d\n",
                       job, job->start_time, job->end_time, ent->fd, FSCALER_ENTITY_JOBNUM);
        }
        spin_unlock_irqrestore(&ent->lock, flags);
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
struct fscaler_job *fscaler_entity_getjob(struct fscaler_entity *ent)
{
    struct fscaler_job *job = NULL;
    unsigned long flags;
    if (ent && ent->opened) {
        spin_lock_irqsave(&ent->lock, flags);
        job = klist_entry(klist_pop(&ent->klist_job, 0), struct fscaler_job, knode_ent);
        if (job) {
            fscaler_job_set_magic(job, __func__);
            if (FSCALER_ENTITY_JOBNUM > 0) {
                --FSCALER_ENTITY_JOBNUM;
            }
            if (klist_empty(&ent->klist_job)) {
                fscaler_entity_lock(ent);
            }
            M_DEVELOPP("SCL: pop job %p(s:%d e:%d) from entity %d, total: %d\n",
                       job, job->start_time, job->end_time, ent->fd, FSCALER_ENTITY_JOBNUM);
        }
        spin_unlock_irqrestore(&ent->lock, flags);
        return job;
    }
    return NULL;
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
const char *fscaler_entity_table_name(void)
{
    return &FSCALER_ENTITY_TABLE_NAME[0];
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
int fscaler_entity_empty_all(void)
{
    if (FSCALER_ENTITY_JOBNUM <= 16 / 2 || FSCALER_ENTITY_JOBNUM > 16 * 3) {
        M_DEVELOPP("SCL: q:%d\n", FSCALER_ENTITY_JOBNUM);
    }
    return (int)(0 == FSCALER_ENTITY_JOBNUM);
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_entity_table_request);
EXPORT_SYMBOL(fscaler_entity_table_remove);
EXPORT_SYMBOL(fscaler_entity_open);
EXPORT_SYMBOL(fscaler_entity_close);
EXPORT_SYMBOL(fscaler_entity_putjob);
EXPORT_SYMBOL(fscaler_entity_getjob);
EXPORT_SYMBOL(fscaler_entity_table_name);
EXPORT_SYMBOL(fscaler_entity_empty_all);
