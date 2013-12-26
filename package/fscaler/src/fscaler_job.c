/**
 * @file fscaler_job.c
 * @brief
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.xx
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_job.h"
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
static int fscaler_job_cache(struct fscaler_table *table);
static void fscaler_job_uncache(struct fscaler_table *table);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_JOB_TABLE_NAME[] = "fscjob";
static struct kmem_cache *FSCALER_KMEM_SUBJOB = NULL;

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE struct fscaler_job *fscaler_job_allocate(struct fscaler_table *table,
                                                       int fd, int type, int max)
{
    int i = 0;
    struct fscaler_job *job = NULL;
    if (unlikely(max > FSCDRV_JOBGROUP_MAX)) {
        M_PANIC("SCL: max > FSCDRV_JOBGROUP_MAX\n");
        return NULL;
    }
    if (table && table->kmem && FSCALER_KMEM_SUBJOB) {
        job = FSCALER_KMEM_ALLOC(table->kmem);
        if (job) {
            job->parent = table;
            job->fd = fd;
            job->id = -1;
            job->curr = 0;
            job->max = max;
            job->type = type;
            job->status = FSCALER_JOB_STATUS_IDLE;
            job->prio = FSCALER_JOB_PRIO_NORMAL;
            job->retry = 0;
            job->bonus = 0;
            job->start_time = TIME_INIT;
            job->end_time = job->start_time + 1;
            job->data = 0;
            memset(job->group, 0, sizeof(job->group));
            job->m = 0;
            memset(job->magic, 0, sizeof(job->magic));
            if (job->max) {
                for (i = 0; i < job->max; ++i) {
                    job->group[i] = FSCALER_KMEM_ALLOC(FSCALER_KMEM_SUBJOB);
                    if (unlikely(!job->group[i])) {
                        break;
                    }
                    job->group[i]->index = i;
                }
                if (i >= job->max) {
                    fscaler_job_set_magic(job, __func__);
                    return job;
                }
                while (--i >= 0) {
                    FSCALER_KMEM_FREE(FSCALER_KMEM_SUBJOB, (void *)job->group[i]);
                    job->group[i] = NULL;
                }
            }
            FSCALER_KMEM_FREE(table->kmem, (void *)job);
            job = NULL;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_job_free(struct fscaler_table *table, struct fscaler_job *job)
{
    int i = 0;
    if (table && table->kmem && FSCALER_KMEM_SUBJOB && job) {
        for (i = 0; i < job->max; ++i) {
            if (job->group[i]) {
                FSCALER_KMEM_FREE(FSCALER_KMEM_SUBJOB, (void *)job->group[i]);
                job->group[i] = NULL;
            }
        }
        if (job->destruct) {
            job->destruct(job);
        }
        job->parent = NULL;
        FSCALER_KMEM_FREE(table->kmem, (void *)job);
        job = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_job_cache(struct fscaler_table *table)
{
    if (table) {
        table->kmem = FSCALER_KMEM_CACHE(fscaler_job, SLAB_HWCACHE_ALIGN);
        if (table->kmem) {
            FSCALER_KMEM_SUBJOB = FSCALER_KMEM_CACHE(fscaler_subjob, SLAB_HWCACHE_ALIGN);
            if (FSCALER_KMEM_SUBJOB) {
                return FSCALER_OK;
            }
        }
    }
    return FSCALER_ERR_CREATE;
}

void fscaler_job_uncache(struct fscaler_table *table)
{
    if (table) {
        if (FSCALER_KMEM_SUBJOB) {
            FSCALER_KMEM_UNCACHE(FSCALER_KMEM_SUBJOB);
            FSCALER_KMEM_SUBJOB = NULL;
        }
        if (table->kmem) {
            FSCALER_KMEM_UNCACHE(table->kmem);
            table->kmem = NULL;
        }
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
int fscaler_job_table_request(struct fscaler_driver *drv)
{
    int hr = 0;
    hr = fscaler_table_request(drv, &FSCALER_JOB_TABLE_NAME[0], 0,
                               0, 0, fscaler_job_cache, fscaler_job_uncache, 0, 0, 0);
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
void fscaler_job_table_remove(struct fscaler_driver *drv)
{
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_job_table_name());
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
const char *fscaler_job_table_name(void)
{
    return &FSCALER_JOB_TABLE_NAME[0];
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
struct fscaler_job *fscaler_job_request(struct fscaler_table *table, int fd, int type, int max)
{
    unsigned long flags;
    struct fscaler_job *job = fscaler_job_allocate(table, fd, type, max);
    if (job) {
        fscaler_job_set_magic(job, __func__);
        fscaler_job_callback_register(job, 0, 0, 0, 0);
        spin_lock_irqsave(&table->lock, flags);
        klist_add_tail(&job->knode_table, &table->klist_item);
        spin_unlock_irqrestore(&table->lock, flags);
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
void fscaler_job_remove(struct fscaler_table *table, struct fscaler_job *job)
{
    unsigned long flags;
    if (table && job) {
        fscaler_job_set_magic(job, __func__);
        spin_lock_irqsave(&table->lock, flags);
        klist_del(&job->knode_table);
        spin_unlock_irqrestore(&table->lock, flags);
        fscaler_job_free(table, job);
        job = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_job_table_request);
EXPORT_SYMBOL(fscaler_job_table_remove);
EXPORT_SYMBOL(fscaler_job_table_name);
EXPORT_SYMBOL(fscaler_job_request);
EXPORT_SYMBOL(fscaler_job_remove);
