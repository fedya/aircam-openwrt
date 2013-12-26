/**
 * @file fscaler_module.c
 * @brief scaler module
 * @author slchen <slchen@grain-media.com>
 * @version v0.1.04 (release v1.04)
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#include "fscaler_module.h"
#include "fscaler_driver.h"
#include "fscaler_vg.h"
#include "fscaler_osd.h"
#include "fscaler_proc.h"
#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"

#include <linux/device.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15))
#include <linux/platform_device.h>
#endif /* LINUX_VERSION_CODE */

static void fscaler_module_release(struct device *p_dev);
static int fscaler_module_probe(struct device *dev);
static int fscaler_module_remove(struct device *dev);
static int fscaler_module_driver_match(struct klist_node *n, void *data);

static struct resource FSCALER_RESOURCES[] = {
    [0] = {
           .start = SCAL_FTSCAL010_PA_BASE,
           .end = SCAL_FTSCAL010_PA_LIMIT,
           .flags = IORESOURCE_MEM,
           },

    [1] = {
           .start = SCAL_FTSCAL010_0_IRQ,
           .end = SCAL_FTSCAL010_0_IRQ,
           .flags = IORESOURCE_IRQ,
           },
};

u64 FSCALER_PLATFORM_DMAMASK = ~(u32) 0;
struct platform_device FSCALER_PLATFORM = {
    .id = -1,
    .name = MPFX_NAME,
    .num_resources = ARRAY_SIZE(FSCALER_RESOURCES),
    .resource = FSCALER_RESOURCES,
    .dev = {
            .dma_mask = &FSCALER_PLATFORM_DMAMASK,
            .coherent_dma_mask = 0xffffffff,
            .release = fscaler_module_release,
            }
};

struct device_driver FSCALER_DRIVER = {
    .owner = THIS_MODULE,
    .name = MPFX_NAME,
    .bus = &platform_bus_type,
    .probe = fscaler_module_probe,
    .remove = __devexit_p(fscaler_module_remove),
};

STATIC_INLINE void fscaler_module_panic(void)
{
    if (unlikely(FSCFD_MAX < max_entity)) {
        panic("Invalid max_entity:%d\n", max_entity);
    }
    if (unlikely(FSCIP_MAX < max_device)) {
        panic("Invalid max_device:%d\n", max_device);
    }
    if (unlikely(supp_io && supp_vg)) {
        panic("Can not install supp_io:%d supp_vg:%d at the same time\n", supp_io, supp_vg);
    }
    if (unlikely(!supp_io && !supp_vg && !supp_osd)) {
        panic("Must install supp_io:%d supp_vg:%d supp_osd:%d at least one\n", supp_io, supp_vg,
              supp_osd);
    }
}

STATIC_INLINE void fscaler_module_set_drvdata(struct device *dev, struct fscaler_module *m)
{
    if (dev) {
        dev_set_drvdata(dev, (void *)m);
    }
}

STATIC_INLINE struct fscaler_module *fscaler_module_get_drvdata(struct device *dev)
{
    return dev ? (struct fscaler_module *)dev_get_drvdata(dev) : NULL;
}

STATIC_INLINE struct fscaler_module *fscaler_module_request(struct device *dev)
{
    struct fscaler_module *mod = NULL;
    if (dev) {
        mod = FSCALER_SALLOC(fscaler_module);
        if (mod) {
            mod->dev = dev;
            klist_init(&mod->klist_drv, fscaler_driver_create, fscaler_driver_destroy);
            fscaler_module_set_drvdata(dev, mod);
            M_DEBUG("SCL: setup %p=%d-%d-%d-%d-%d-%d=%d-%d-%d\n", dev, dbg_type, supp_proc,
                    supp_kpi, supp_vg, supp_io, supp_osd, max_entity, max_device, timeout);
            return mod;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_module_free(struct fscaler_module *mod)
{
    if (mod) {
        fscaler_module_set_drvdata(mod->dev, 0);
        mod->dev = 0;
        FSCALER_FREE(mod);
    }
}

int fscaler_module_probe(struct device *dev)
{
    int hr = 0;
    struct fscaler_driver *drv = NULL;
    struct fscaler_module *mod = NULL;
    mod = fscaler_module_request(dev);
    M_DEVELOPP("SCL: fscaler_module_request() mod = %p\n", mod);
    if (mod) {
        hr = fscaler_driver_request(mod);
        M_DEVELOPP("SCL: fscaler_driver_request() hr = %x\n", hr);
        if (FSCALER_SUCCEED(hr)) {
            drv = fscaler_module_driver_find(mod, fscaler_driver_name());
            M_DEVELOPP("SCL: find drv:%p\n", drv);
            if (drv) {
                fscaler_vg_interface_request(drv);
                fscaler_osd_interface_request(drv);
                fscaler_proc_interface_request(drv);
            }
            return 0;
        }
        fscaler_module_free(mod);
    }
    return -EINVAL;
}

int fscaler_module_remove(struct device *dev)
{
    struct fscaler_driver *drv = NULL;
    struct fscaler_module *mod = NULL;
    mod = fscaler_module_get_drvdata(dev);
    if (mod) {
        drv = fscaler_module_driver_find(mod, fscaler_driver_name());
        if (drv) {
            fscaler_proc_interface_remove(drv);
            fscaler_osd_interface_remove(drv);
            fscaler_vg_interface_remove(drv);
            fscaler_driver_remove(drv);
        }
        fscaler_module_free(mod);
        return 0;
    }
    return -EINVAL;
}

void fscaler_module_release(struct device *dev)
{
    M_DEBUG("SCL: Notice to release platform\n");
    return;
}

int fscaler_module_driver_match(struct klist_node *n, void *data)
{
    struct fscaler_driver *drv = klist_entry(n, struct fscaler_driver, knode_mod);
    const char *name = (const char *)data;
    if (drv && name) {
        return (int)(0 == strcmp(drv->name, name));
    }
    return 0;
}

struct fscaler_driver *fscaler_module_driver_find(struct fscaler_module *mod, const char *name)
{
    struct klist_node *n = NULL;
    if (mod) {
        n = klist_find(&mod->klist_drv, 0, (void *)name, fscaler_module_driver_match);
        return klist_entry(n, struct fscaler_driver, knode_mod);
    }
    return NULL;
}

static int __init fscaler_module_init(void)
{
    int ret = 0;
    printk("SCL: Version, %s\n", MPFX_VERS);
    fscaler_module_panic();
    ret = platform_device_register(&FSCALER_PLATFORM);
    if (0 <= ret) {
        ret = driver_register(&FSCALER_DRIVER);
        if (0 <= ret) {
            goto EXIT;
        } else {
            M_ERROR("SCL: Failed to register driver\n");
        }
        platform_device_unregister(&FSCALER_PLATFORM);
    } else {
        M_ERROR("SCL: Failed to register platform\n");
    }
  EXIT:
    M_DEBUG("SCL: %s to insert module\n", ret >= 0 ? "Notice" : "Failed");
    return ret;
}

static void __exit fscaler_module_exit(void)
{
    driver_unregister(&FSCALER_DRIVER);
    platform_device_unregister(&FSCALER_PLATFORM);
    M_DEBUG("SCL: Notice to exit module\n");
}

module_init(fscaler_module_init);
module_exit(fscaler_module_exit);

int dbg_type = FSCDRV_DEBUG;
module_param(dbg_type, int, 0);
MODULE_PARM_DESC(dbg_type, "debug");

int supp_proc = FSCDRV_PROC;
module_param(supp_proc, int, 0);
MODULE_PARM_DESC(supp_proc, "support proc");

int supp_kpi = FSCDRV_KPI;
module_param(supp_kpi, int, 0);
MODULE_PARM_DESC(supp_kpi, "support KPI");

int supp_vg = FSCDRV_VG;
module_param(supp_vg, int, 0);
MODULE_PARM_DESC(supp_vg, "support video graph");

int supp_io = FSCDRV_IOCTL;
module_param(supp_io, int, 0);
MODULE_PARM_DESC(supp_io, "support io control");

int supp_osd = FSCDRV_OSD;
module_param(supp_osd, int, 0);
MODULE_PARM_DESC(supp_osd, "support osd");

int pmu_clock = FSCDRV_PMU_CLOCK;
module_param(pmu_clock, int, 0);
MODULE_PARM_DESC(pmu_clock, "pmu target clock");

unsigned int timeout = FSCDRV_TIMEOUT;
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, "timeout");

unsigned int max_retry = FSCDRV_MAX_RETRY;
module_param(max_retry, uint, 0);
MODULE_PARM_DESC(max_retry, "retry max");

unsigned int max_width = FSCDRV_MAX_WIDTH;
module_param(max_width, uint, 0);
MODULE_PARM_DESC(max_width, "width max");

unsigned int max_height = FSCDRV_MAX_HEIGHT;
module_param(max_height, uint, 0);
MODULE_PARM_DESC(max_height, "height max");

unsigned int max_entity = FSCFD_INIT;
module_param(max_entity, uint, 0);
MODULE_PARM_DESC(max_entity, "entity max");

unsigned int max_device = FSCIP_INIT;
module_param(max_device, uint, 0);
MODULE_PARM_DESC(max_device, "device max");

MODULE_AUTHOR("GM Corporation");
MODULE_DESCRIPTION("GM Scaler V3 Module");
MODULE_LICENSE("GPL");

EXPORT_SYMBOL(fscaler_module_driver_find);
