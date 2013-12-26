/**
 * @file fscaler_driver.c
 * @brief Scaler driver implementation.
 *        Scaler driver is core engine to provide the interface.
 *        Scaler device is as hardware entity.
 *        Scaler entity is as the channel concepts recognized by index.
 *        The videograph entities are all registered when the driver interface is installed.
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_driver.h"
#include "fscaler_interface.h"
#include "fscaler_table.h"
#include "fscaler_property.h"
#include "fscaler_job.h"
#include "fscaler_entity.h"
#include "fscaler_device.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/spinlock.h>
#include <linux/device.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15))
#include <linux/platform_device.h>
#endif /* LINUX_VERSION_CODE */
#include <asm/io.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,16))
#include <linux/io.h>
#endif /* LINUX_VERSION_CODE */

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static int klist_device_get_resource(struct klist_node *n, void *data);
static int klist_device_put_resource(struct klist_node *n, void *data);
static void fscaler_driver_grab(struct fscaler_device *dev);
static int klist_device_open(struct klist_node *n, void *data);
static int klist_device_close(struct klist_node *n, void *data);
static int fscaler_driver_table_match(struct klist_node *n, void *data);
static int fscaler_driver_interface_match(struct klist_node *n, void *data);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_DRIVER_NAME[] = "fscdrv3";
static spinlock_t FSCALER_DRIVER_DISPATCH_LOCK;

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE struct fscaler_driver *fscaler_driver_allocate(struct fscaler_module *mod,
                                                             const char *name)
{
    struct fscaler_driver *drv = NULL;
    if (mod && name) {
        drv = FSCALER_SALLOC(fscaler_driver);
        if (drv) {
            drv->parent = mod;
            drv->name = name;
            drv->attached = 0;
            spin_lock_init(&FSCALER_DRIVER_DISPATCH_LOCK);
            klist_init(&drv->klist_table, fscaler_table_create, fscaler_table_destroy);
            klist_init(&drv->klist_inter, fscaler_interface_create, fscaler_interface_destroy);
            return drv;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_driver_free(struct fscaler_driver *drv)
{
    if (drv) {
        drv->parent = NULL;
        FSCALER_FREE(drv);
        drv = NULL;
    }
}

STATIC_INLINE void fscaler_resource_remove(struct fscaler_driver *drv)
{
    int hr = 0;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table) {
        hr = klist_for_each(&table->klist_item, 0, table, klist_device_put_resource);
    }
}

STATIC_INLINE int fscaler_resource_request(struct fscaler_driver *drv)
{
    int hr = 0;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table) {
        hr = klist_for_each(&table->klist_item, 0, table, klist_device_get_resource);
        if (unlikely(FSCALER_FAILED(hr))) {
            fscaler_resource_remove(drv);
        }
        return hr;
    }
    return FSCALER_ERR_CREATE;
}

STATIC_INLINE void fscaler_driver_unprepare(struct fscaler_driver *drv)
{
    int hr = 0;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table) {
        hr = klist_for_each(&table->klist_item, 0, table, klist_device_close);
    }
}

STATIC_INLINE int fscaler_driver_prepare(struct fscaler_driver *drv)
{
    int hr = 0;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table) {
        hr = klist_for_each(&table->klist_item, 0, table, klist_device_open);
        if (FSCALER_SUCCEED(hr)) {
            return FSCALER_OK;
        }
        fscaler_driver_unprepare(drv);
    }
    return FSCALER_ERR_PREPARE;
}

STATIC_INLINE struct fscaler_job *fscaler_driver_schedule_payload(struct fscaler_table *table)
{
    struct fscaler_job *job = NULL;
    struct fscaler_entity *ent = NULL;
    if (table) {
        ent = fscaler_entity_sched(table);
        if (ent) {
            job = fscaler_entity_getjob(ent);
            if (job) {
                M_DEVELOPP("SCL:  selected job:%p, fd:%d\n", job, job->fd);
                return job;
            } else {
                M_ERROR("SCL: selected entity:fd:%d, no job?\n", ent->fd);
                M_PANIC("SCL: selected entity without job, impossible\n");
            }
        } else {
            M_DEVELOPP("SCL:  empty entity, bitmap[0]:%lx\n", table->bitmap[0]);
        }
    }
    return NULL;
}

STATIC_INLINE struct fscaler_device *fscaler_driver_schedule_device(struct fscaler_table *table)
{
    struct fscaler_device *dev = NULL;
    if (table) {
        dev = fscaler_device_sched(table);
        if (dev) {
            return dev;
        } else {
            M_DEVELOPP("SCL: empty selected device, %lx\n", table->bitmap[0]);
        }
    }
    return NULL;
}

STATIC_INLINE struct fscaler_device *fscaler_driver_do_schedule(struct fscaler_table *table,
                                                                struct fscaler_device *dev)
{
    struct fscaler_job *job = NULL;
    if (table) {
        if (NULL == dev) {
            /* No specified device, search an available device */
            dev = fscaler_driver_schedule_device(table);
        }
        if (dev) {
            if (unlikely(NULL != dev->job)) {
                M_PANIC("SCL: Device with job is invalid before shcedule payload\n");
                return NULL;
            }
            table = fscaler_table_to_table(table, fscaler_entity_table_name());
            job = fscaler_driver_schedule_payload(table);
            if (job) {
                M_DEVELOPP("SCL: Selected job:fd:%d vid:%d st:%x et:%x\n", job->fd,
                           job->data ? ((struct v_job_t *)job->data)->id : -1,
                           job->data ? ((struct v_job_t *)job->data)->itime : -1,
                           job->data ? ((struct v_job_t *)job->data)->otime : -1);
                fscaler_device_receive_job(dev, job);
            } else {
                dev = NULL;
            }
        } else {
            M_DEVELOPP("SCL: All devices are in use\n");
        }
    } else {
        M_ERROR("SCL: No table in scheduler\n");
    }
    return dev;
}

STATIC_INLINE void fscaler_driver_do_dispatch(struct fscaler_table *table,
                                              struct fscaler_device *dev)
{
    unsigned long flags, flags1;
    if (table && !fscaler_entity_empty_all()) {
        spin_lock_irqsave(&FSCALER_DRIVER_DISPATCH_LOCK, flags);
        dev = fscaler_driver_do_schedule(table, dev);
        if (dev) {
            spin_lock_irqsave(&dev->timer_lcok, flags1);
            M_DEVELOPP("SCL: match job:fd:%d, vid:%d\n", fscaler_device_fetch_job(dev)->fd,
                       ((struct v_job_t *)fscaler_device_fetch_job(dev)->data)->id);
            fscaler_device_process(dev);        ///< Do it
            spin_unlock_irqrestore(&dev->timer_lcok, flags1);
        }
        spin_unlock_irqrestore(&FSCALER_DRIVER_DISPATCH_LOCK, flags);
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int klist_device_get_resource(struct klist_node *n, void *data)
{
    struct resource *res = NULL;
    struct platform_device *devp = NULL;
    struct fscaler_module *mod = fscaler_table_to_module((struct fscaler_table *)data);
    struct fscaler_device *dev = klist_entry(n, struct fscaler_device, knode_table);
    if (mod && mod->dev) {
        devp = to_platform_device(mod->dev);
        if (dev) {
            res = platform_get_resource(devp, IORESOURCE_MEM, dev->id);
            if (unlikely(NULL == res)) {
                goto ERROR_EXIT;
            }
            dev->base = (unsigned int)ioremap_nocache(res->start, (res->end - res->start));
            if (unlikely(0 == dev->base)) {
                goto ERROR_EXIT;
            }
            res = platform_get_resource(devp, IORESOURCE_IRQ, dev->id);
            if (unlikely(NULL == res)) {
                goto ERROR_EXIT;
            }
            dev->irq = (int)res->start;
            return FSCALER_OK;
        }
    }
  ERROR_EXIT:
    return FSCALER_ERR_FAIL;
}

int klist_device_put_resource(struct klist_node *n, void *data)
{
    struct fscaler_device *dev = klist_entry(n, struct fscaler_device, knode_table);
    if (dev) {
        if (dev->base) {
            iounmap((void __iomem *)dev->base);
            dev->base = 0;
        }
        if (0 <= dev->irq) {
            dev->irq = -1;
        }
    }
    return FSCALER_OK;
}

void fscaler_driver_grab(struct fscaler_device *dev)
{
    struct fscaler_table *table = fscaler_device_to_table(dev);
    if (table) {
        M_DEVELOPP("## grab ##\n");
        fscaler_driver_do_dispatch(table, dev);
    }
}

int klist_device_open(struct klist_node *n, void *data)
{
    int hr = 0;
    struct fscaler_device *dev = NULL;
    dev = klist_entry(n, struct fscaler_device, knode_table);
    if (dev) {
        hr = fscaler_device_open(dev);
        if (FSCALER_SUCCEED(hr)) {
            fscaler_device_callback_register(dev, fscaler_driver_grab);
        }
        return hr;
    }
    return FSCALER_ERR_OPEN;
}

int klist_device_close(struct klist_node *n, void *data)
{
    struct fscaler_device *dev = NULL;
    dev = klist_entry(n, struct fscaler_device, knode_table);
    if (dev) {
        fscaler_device_callback_register(dev, 0);
        fscaler_device_close(dev);
    }
    return FSCALER_OK;
}

int fscaler_driver_table_match(struct klist_node *n, void *data)
{
    struct fscaler_table *table = klist_entry(n, struct fscaler_table, knode_drv);
    const char *name = (const char *)data;
    if (table && name) {
        return (int)(0 == strcmp(table->name, name));
    }
    return 0;
}

int fscaler_driver_interface_match(struct klist_node *n, void *data)
{
    struct fscaler_interface *inter = klist_entry(n, struct fscaler_interface, knode_drv);
    const char *name = (const char *)data;
    if (inter && name) {
        return (int)(0 == strcmp(inter->name, name));
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT FUCNTION IMPLAMENTATION
/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_driver_request(struct fscaler_module *mod)
{
    int hr = 0;
    struct fscaler_driver *drv = NULL;
    if (mod) {
        drv = fscaler_driver_allocate(mod, FSCALER_DRIVER_NAME);
        if (drv) {
            klist_add_tail(&drv->knode_mod, &mod->klist_drv);
            if (drv->attached) {
                hr = fscaler_driver_prepare(drv);
                if (FSCALER_SUCCEED(hr)) {
                    return FSCALER_OK;
                }
            }
            klist_del(&drv->knode_mod);
            fscaler_driver_free(drv);
            drv = NULL;
        }
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
void fscaler_driver_remove(struct fscaler_driver *drv)
{
    if (drv) {
        fscaler_driver_unprepare(drv);
        klist_del(&drv->knode_mod);
        fscaler_driver_free(drv);
        drv = NULL;
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
void fscaler_driver_create(struct klist_node *n)
{
    int hr = 0;
    struct fscaler_driver *drv = klist_entry(n, struct fscaler_driver, knode_mod);
    struct fscaler_module *mod = fscaler_driver_to_module(drv);
    if (mod && !drv->attached) {
        hr = fscaler_property_table_request(drv);
        if (FSCALER_SUCCEED(hr)) {
            hr = fscaler_job_table_request(drv);
            if (FSCALER_SUCCEED(hr)) {
                hr = fscaler_entity_table_request(drv);
                if (FSCALER_SUCCEED(hr)) {
                    hr = fscaler_device_table_request(drv);
                    if (FSCALER_SUCCEED(hr)) {
                        hr = fscaler_resource_request(drv);
                        if (FSCALER_SUCCEED(hr)) {
                            drv->attached = 1;
                            return;
                        }
                        fscaler_device_table_remove(drv);
                    }
                    fscaler_entity_table_remove(drv);
                }
                fscaler_job_table_remove(drv);
            }
            fscaler_property_table_remove(drv);
        }
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
void fscaler_driver_destroy(struct klist_node *n)
{
    struct fscaler_driver *drv = klist_entry(n, struct fscaler_driver, knode_mod);
    struct fscaler_module *mod = fscaler_driver_to_module(drv);
    if (mod && drv->attached) {
        drv->attached = 0;
        fscaler_resource_remove(drv);
        fscaler_device_table_remove(drv);
        fscaler_entity_table_remove(drv);
        fscaler_job_table_remove(drv);
        fscaler_property_table_remove(drv);
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
int fscaler_driver_open(struct fscaler_entity *ent)
{
    if (ent) {
        return fscaler_entity_open(ent);
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
void fscaler_driver_close(struct fscaler_entity *ent)
{
    if (ent) {
        fscaler_entity_close(ent);
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
void fscaler_driver_dispatch(struct fscaler_entity *ent, struct fscaler_job *job)
{
    struct fscaler_table *table = NULL;
    table = fscaler_table_to_table(fscaler_entity_to_table(ent), fscaler_device_table_name());
    if (table) {
        fscaler_job_set_magic(job, __func__);
        fscaler_entity_putjob(ent, job);
        fscaler_driver_do_dispatch(table, NULL);
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
const char *fscaler_driver_name(void)
{
    return &FSCALER_DRIVER_NAME[0];
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
struct fscaler_table *fscaler_driver_table_find(struct fscaler_driver *drv, const char *name)
{
    struct klist_node *n = NULL;
    if (drv) {
        n = klist_find(&drv->klist_table, 0, (void *)name, fscaler_driver_table_match);
        return klist_entry(n, struct fscaler_table, knode_drv);
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
struct fscaler_interface *fscaler_driver_interface_find(struct fscaler_driver *drv,
                                                        const char *name)
{
    struct klist_node *n = NULL;
    if (drv) {
        n = klist_find(&drv->klist_inter, 0, (void *)name, fscaler_driver_interface_match);
        return klist_entry(n, struct fscaler_interface, knode_drv);
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_driver_request);
EXPORT_SYMBOL(fscaler_driver_remove);
EXPORT_SYMBOL(fscaler_driver_create);
EXPORT_SYMBOL(fscaler_driver_destroy);
EXPORT_SYMBOL(fscaler_driver_open);
EXPORT_SYMBOL(fscaler_driver_close);
EXPORT_SYMBOL(fscaler_driver_dispatch);
EXPORT_SYMBOL(fscaler_driver_name);
EXPORT_SYMBOL(fscaler_driver_table_find);
EXPORT_SYMBOL(fscaler_driver_interface_find);
