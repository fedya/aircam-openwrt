/**
 * @file fscaler_proc.c
 * @brief Scaler driver proc filesystem
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_proc.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"
#include "fscaler_property.h"
#include "fscaler_job.h"
#include "fscaler_entity.h"
#include "fscaler_device.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <asm/uaccess.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
#include <linux/uaccess.h>
#endif /*LINUX_VERSION_CODE */

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
void fscaler_proc_uninstall(struct fscaler_interface *inter);
int fscaler_proc_install(struct fscaler_interface *inter);
static int fscaler_proc_property_show(struct seq_file *sfile, void *v);
static int fscaler_proc_register_show(struct seq_file *sfile, void *v);
static int fscaler_proc_device_show(struct seq_file *sfile, void *v);
static int fscaler_proc_grabjob_devid_show(struct seq_file *sfile, void *v);
static int fscaler_proc_grabjob_fd_show(struct seq_file *sfile, void *v);
static int fscaler_proc_kpi_show(struct seq_file *sfile, void *v);
static int fscaler_proc_fops_property_open(struct inode *inode, struct file *file);
static ssize_t fscaler_proc_fops_property_write(struct file *file, const char __user * buf,
                                                size_t size, loff_t * off);
static int fscaler_proc_fops_register_open(struct inode *inode, struct file *file);
static int fscaler_proc_fops_device_open(struct inode *inode, struct file *file);
static ssize_t fscaler_proc_fops_device_write(struct file *file, const char __user * buf,
                                              size_t size, loff_t * off);
static int fscaler_proc_fops_grabjob_devid_open(struct inode *inode, struct file *file);
static ssize_t fscaler_proc_fops_grabjob_devid_write(struct file *file, const char __user * buf,
                                                     size_t size, loff_t * off);
static int fscaler_proc_fops_grabjob_fd_open(struct inode *inode, struct file *file);
static ssize_t fscaler_proc_fops_grabjob_fd_write(struct file *file, const char __user * buf,
                                                  size_t size, loff_t * off);
static int fscaler_proc_fops_kpi_open(struct inode *inode, struct file *file);
static ssize_t fscaler_proc_fops_kpi_write(struct file *file, const char __user * buf,
                                           size_t size, loff_t * off);
static int klist_proc_device_joblist_show(struct klist_node *n, void *data);
static int klist_proc_float_joblist_show(struct klist_node *n, void *data);
int klist_proc_grabjob_joblist_show(struct klist_node *n, void *data);
int klist_proc_grabjob_devid_joblist_show(struct klist_node *n, void *data);
int klist_proc_grabjob_fd_joblist_show(struct klist_node *n, void *data);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static struct proc_dir_entry *FSCALER_PROC_ROOT = NULL;
static struct proc_dir_entry *FSCALER_PROC_PROPERTY = NULL;
static struct proc_dir_entry *FSCALER_PROC_REGISTER = NULL;
static struct proc_dir_entry *FSCALER_PROC_DEVICE = NULL;
static struct proc_dir_entry *FSCALER_PROC_GRABJOB = NULL;
static struct proc_dir_entry *FSCALER_PROC_GRABJOB_IDLEAF = NULL;
static struct proc_dir_entry *FSCALER_PROC_GRABJOB_FDLEAF = NULL;
static struct proc_dir_entry *FSCALER_PROC_KPI = NULL;
static struct fscaler_driver *FSCALER_PROC_OWNER = NULL;
static char FSCALER_PROC_NAME[] = "scaler";
static int FSCALER_PROC_PROPERTY_FD = 0;
static int FSCALER_PROC_DEVICE_ID = 0;
static int FSCALER_PROC_GRABJOB_ID = 0;
static int FSCALER_PROC_GRABJOB_FD = 0;
static spinlock_t FSCALER_PROC_LOCK;

static struct file_operations FSCALER_PROC_FOPS_PROPERTY = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_property_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = fscaler_proc_fops_property_write
};

static struct file_operations FSCALER_PROC_FOPS_REGISTER = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_register_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release
};

static struct file_operations FSCALER_PROC_FOPS_DEVICE = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_device_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = fscaler_proc_fops_device_write
};

static struct file_operations FSCALER_PROC_FOPS_GRABJOB_DEVID = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_grabjob_devid_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = fscaler_proc_fops_grabjob_devid_write
};

static struct file_operations FSCALER_PROC_FOPS_GRABJOB_FD = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_grabjob_fd_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = fscaler_proc_fops_grabjob_fd_write
};

static struct file_operations FSCALER_PROC_FOPS_KPI = {
    .owner = THIS_MODULE,
    .open = fscaler_proc_fops_kpi_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
    .write = fscaler_proc_fops_kpi_write
};

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE void fscaler_proc_set_owner(struct fscaler_driver *drv)
{
    FSCALER_PROC_OWNER = drv;
}

STATIC_INLINE struct fscaler_driver *fscaler_proc_get_owner(void)
{
    return FSCALER_PROC_OWNER;
}

STATIC_INLINE struct fscaler_table *fscaler_proc_get_table(const char *name)
{
    return fscaler_driver_table_find(FSCALER_PROC_OWNER, name);
}

STATIC_INLINE void *fscaler_proc_table_item_find(const char *name, int index)
{
    return fscaler_table_item_find(fscaler_proc_get_table(name), index);
}

STATIC_INLINE struct fscaler_property *fscaler_proc_get_property(int fd)
{
    return (struct fscaler_property *)fscaler_proc_table_item_find(fscaler_property_table_name(),
                                                                   fd);
}

STATIC_INLINE struct fscaler_entity *fscaler_proc_get_entity(int fd)
{
    return (struct fscaler_entity *)fscaler_proc_table_item_find(fscaler_entity_table_name(), fd);
}

STATIC_INLINE struct fscaler_device *fscaler_proc_get_device(int id)
{
    return (struct fscaler_device *)fscaler_proc_table_item_find(fscaler_device_table_name(), id);
}

STATIC_INLINE struct proc_dir_entry *fscaler_proc_request(const char *name, mode_t mode,
                                                          struct proc_dir_entry *parent)
{
    return create_proc_entry(name, mode, (parent == NULL) ? FSCALER_PROC_ROOT : parent);
}

STATIC_INLINE void fscaler_proc_remove(struct proc_dir_entry *parent, struct proc_dir_entry *del)
{
    if (del) {
        remove_proc_entry(del->name, (parent == NULL) ? FSCALER_PROC_ROOT : parent);
    }
}

STATIC_INLINE int fscaler_proc_create(const char *name)
{
    struct proc_dir_entry *p = create_proc_entry(name, S_IFDIR | S_IRUGO | S_IXUGO, NULL);
    if (p) {
        p->owner = THIS_MODULE;
        FSCALER_PROC_ROOT = p;
        return FSCALER_OK;
    }
    return FSCALER_ERR_PREPARE;
}

STATIC_INLINE void fscaler_proc_destroy(void)
{
    if (FSCALER_PROC_ROOT != NULL) {
        remove_proc_entry(FSCALER_PROC_ROOT->name, FSCALER_PROC_ROOT);
    }
}

STATIC_INLINE int fscaler_proc_property_request(void)
{
    FSCALER_PROC_PROPERTY = fscaler_proc_request("property", S_IRUGO | S_IXUGO, 0);
    if (unlikely(!FSCALER_PROC_PROPERTY)) {
        M_WARN("SCL: Failed to request proc fs/property\n");
        return FSCALER_ERR_CREATE;
    }
    FSCALER_PROC_PROPERTY->proc_fops = &FSCALER_PROC_FOPS_PROPERTY;
    FSCALER_PROC_PROPERTY->owner = THIS_MODULE;
    return FSCALER_OK;
}

STATIC_INLINE int fscaler_proc_register_request(void)
{
    FSCALER_PROC_REGISTER = fscaler_proc_request("dumpreg", S_IRUGO | S_IXUGO, 0);
    if (unlikely(!FSCALER_PROC_REGISTER)) {
        M_WARN("SCL: Failed to request proc fs/register\n");
        return FSCALER_ERR_CREATE;
    }
    FSCALER_PROC_REGISTER->proc_fops = &FSCALER_PROC_FOPS_REGISTER;
    FSCALER_PROC_REGISTER->owner = THIS_MODULE;
    return FSCALER_OK;
}

STATIC_INLINE int fscaler_proc_device_request(void)
{
    FSCALER_PROC_DEVICE = fscaler_proc_request("device", S_IRUGO | S_IXUGO, 0);
    if (unlikely(!FSCALER_PROC_DEVICE)) {
        M_WARN("SCL: Failed to request proc fs/device\n");
        return FSCALER_ERR_CREATE;
    }
    FSCALER_PROC_DEVICE->proc_fops = &FSCALER_PROC_FOPS_DEVICE;
    FSCALER_PROC_DEVICE->owner = THIS_MODULE;
    return FSCALER_OK;
}

STATIC_INLINE int fscaler_proc_grabjob_request(void)
{
    FSCALER_PROC_GRABJOB = fscaler_proc_request("grabjob", S_IFDIR | S_IRUGO | S_IXUGO, 0);
    if (unlikely(!FSCALER_PROC_GRABJOB)) {
        printk("SCL: Failed to request /proc/scaler/grabjob\n");
        goto ERROR_EXIT;
    }
    FSCALER_PROC_GRABJOB->owner = THIS_MODULE;
    FSCALER_PROC_GRABJOB_IDLEAF =
        fscaler_proc_request("devid", S_IRUGO | S_IXUGO, FSCALER_PROC_GRABJOB);
    if (unlikely(!FSCALER_PROC_GRABJOB_IDLEAF)) {
        printk("SCL: Failed to request /proc/scaler/grabjob/devid\n");
        goto ERROR_EXIT;
    }
    FSCALER_PROC_GRABJOB_IDLEAF->proc_fops = &FSCALER_PROC_FOPS_GRABJOB_DEVID;
    FSCALER_PROC_GRABJOB_IDLEAF->owner = THIS_MODULE;
    FSCALER_PROC_GRABJOB_FDLEAF =
        fscaler_proc_request("fd", S_IRUGO | S_IXUGO, FSCALER_PROC_GRABJOB);
    if (unlikely(!FSCALER_PROC_GRABJOB_FDLEAF)) {
        printk("SCL: Failed to request /proc/scaler/grabjob/fd\n");
        goto ERROR_EXIT;
    }
    FSCALER_PROC_GRABJOB_FDLEAF->proc_fops = &FSCALER_PROC_FOPS_GRABJOB_FD;
    FSCALER_PROC_GRABJOB_FDLEAF->owner = THIS_MODULE;
    return FSCALER_OK;

  ERROR_EXIT:
    if (FSCALER_PROC_GRABJOB_IDLEAF) {
        fscaler_proc_remove(NULL, FSCALER_PROC_GRABJOB_IDLEAF);
        FSCALER_PROC_GRABJOB_IDLEAF = NULL;
    }
    if (FSCALER_PROC_GRABJOB) {
        fscaler_proc_remove(NULL, FSCALER_PROC_GRABJOB);
        FSCALER_PROC_GRABJOB = NULL;
    }
    return FSCALER_ERR_CREATE;
}

STATIC_INLINE int fscaler_proc_kpi_request(void)
{
    FSCALER_PROC_KPI = fscaler_proc_request("kpi", S_IRUGO | S_IXUGO, 0);
    if (unlikely(!FSCALER_PROC_KPI)) {
        printk("SCL: Failed to request /proc/scaler/kpi\n");
        return FSCALER_ERR_CREATE;
    }
    FSCALER_PROC_KPI->proc_fops = &FSCALER_PROC_FOPS_KPI;
    FSCALER_PROC_KPI->owner = THIS_MODULE;
    return FSCALER_OK;
}

STATIC_INLINE void fscaler_proc_property_remove(void)
{
    if (FSCALER_PROC_PROPERTY) {
        fscaler_proc_remove(NULL, FSCALER_PROC_PROPERTY);
        FSCALER_PROC_PROPERTY = NULL;
    }
}

STATIC_INLINE void fscaler_proc_register_remove(void)
{
    if (FSCALER_PROC_REGISTER) {
        fscaler_proc_remove(NULL, FSCALER_PROC_REGISTER);
        FSCALER_PROC_REGISTER = NULL;
    }
}

STATIC_INLINE void fscaler_proc_device_remove(void)
{
    if (FSCALER_PROC_DEVICE) {
        fscaler_proc_remove(NULL, FSCALER_PROC_DEVICE);
        FSCALER_PROC_DEVICE = NULL;
    }
}

STATIC_INLINE void fscaler_proc_grabjob_remove(void)
{
    if (FSCALER_PROC_GRABJOB_FDLEAF) {
        fscaler_proc_remove(NULL, FSCALER_PROC_GRABJOB_FDLEAF);
        FSCALER_PROC_GRABJOB_FDLEAF = NULL;
    }
    if (FSCALER_PROC_GRABJOB_IDLEAF) {
        fscaler_proc_remove(NULL, FSCALER_PROC_GRABJOB_IDLEAF);
        FSCALER_PROC_GRABJOB_IDLEAF = NULL;
    }
    if (FSCALER_PROC_GRABJOB) {
        fscaler_proc_remove(NULL, FSCALER_PROC_GRABJOB);
        FSCALER_PROC_GRABJOB = NULL;
    }
}

STATIC_INLINE void fscaler_proc_kpi_remove(void)
{
    if (FSCALER_PROC_KPI) {
        fscaler_proc_remove(NULL, FSCALER_PROC_KPI);
        FSCALER_PROC_KPI = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_proc_property_show(struct seq_file *sfile, void *v)
{
    int i = ID_NULL + 1;
    const char *name = NULL;
    struct fscaler_property *prop = fscaler_proc_get_property(FSCALER_PROC_PROPERTY_FD);
    unsigned long flags;
    if (prop) {
        spin_lock_irqsave(&FSCALER_PROC_LOCK, flags);
        seq_printf(sfile, "SCL: Entity %d proeprties:\n", FSCALER_PROC_PROPERTY_FD);
        name = fscaler_property_name(i);
        while (name) {
            seq_printf(sfile, "     %s:%d\n", name, fscaler_property_get(prop, i));
            name = fscaler_property_name(++i);
        }
        spin_unlock_irqrestore(&FSCALER_PROC_LOCK, flags);
    }
    return 0;
}

int fscaler_proc_register_show(struct seq_file *sfile, void *v)
{
    int i = 0, j = 0, d = 0;
    volatile u32 *preg = NULL;
    struct fscaler_device *dev = NULL;
    unsigned long flags;
    dev = fscaler_proc_get_device(d);
    while (dev) {
        spin_lock_irqsave(&FSCALER_PROC_LOCK, flags);
        preg = (u32 *) dev->base;
        seq_printf(sfile, "SCL: Device %d addr:%x:\n", d, dev->base);
        for (i = 0, j = 0; i < (0x400 >> 2); i++, j++) {
            if (j == 0 || j >= 4) {
                seq_printf(sfile, "\n");
                seq_printf(sfile, "0x%04x:", i << 2);
                j = 0;
            }
            seq_printf(sfile, "  %08x", *preg);
            preg++;
        }
        seq_printf(sfile, "\n");
        spin_unlock_irqrestore(&FSCALER_PROC_LOCK, flags);
        dev = fscaler_proc_get_device(++d);
    }
    return 0;
}

int fscaler_proc_device_show(struct seq_file *sfile, void *v)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_job *job = NULL;
    struct fscaler_subjob *sjob = NULL;
    unsigned long flags;
    spin_lock_irqsave(&FSCALER_PROC_LOCK, flags);
    dev = fscaler_proc_get_device(FSCALER_PROC_DEVICE_ID);
    if (dev) {
        seq_printf(sfile, "SCL: Dump device %d\n", dev->id);
        seq_printf(sfile, "     addr:%x st:%d open:%d irq:%d reboot:%d\n", dev->base, dev->status,
                   dev->opened, dev->irq, dev->retry);
        job = fscaler_device_fetch_job(dev);
        if (job) {
            seq_printf(sfile,
                       "     job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, retry:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr,
                       job->type, job->status, job->prio, job->retry, job->start_time,
                       job->end_time);
            for (i = 0; i < dev->job->max; ++i) {
                sjob = fscaler_job_get_subjob(job, i);
                seq_printf(sfile, "     sjob[%d]:\n", sjob->index);
                seq_printf(sfile,
                           "     IN:bg_w:%d bg_h:%d x:%d y:%d w:%d h:%d layout:%d order:%d sample_down:%d\n",
                           sjob->in_bg_w, sjob->in_bg_h, sjob->in_x, sjob->in_y, sjob->in_w,
                           sjob->in_h, sjob->in_buf.layout, sjob->in_buf.order,
                           sjob->in_buf.sample_down);
                seq_printf(sfile,
                           "     OUT:bg_w:%d bg_h:%d x:%d y:%d w:%d h:%d layout:%d order:%d sample_down:%d\n",
                           sjob->out_bg_w, sjob->out_bg_h, sjob->out_x, sjob->out_y, sjob->out_w,
                           sjob->out_h, sjob->out_buf.layout, sjob->out_buf.order,
                           sjob->out_buf.sample_down);
            }
        }
        if (!klist_empty(&dev->klist_job)) {
            klist_for_each(&dev->klist_job, 0, sfile, klist_proc_device_joblist_show);
        }
        if (supp_osd) {
            seq_printf(sfile, "SCL: Dump OSD %d\n", dev->id);
            seq_printf(sfile, "     plug:%d, chk:%d\n", dev->osd.pluged, dev->osd.check);
            seq_printf(sfile, "     map: count:%d\n", dev->osd.map.count);
            seq_printf(sfile, "     map: bitmap:");
            for (i = ARRAY_SIZE(dev->osd.map.bitmap); i > 0; --i) {
                seq_printf(sfile, " %08x", dev->osd.map.bitmap[i - 1]);
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     map: fbitmap:");
            for (i = ARRAY_SIZE(dev->osd.map.fbitmap); i > 0; --i) {
                seq_printf(sfile, " %08x", dev->osd.map.fbitmap[i - 1]);
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     map: index:");
            for (i = 0; i < ARRAY_SIZE(dev->osd.map.index); ++i) {
                if (0 == i % 16) {
                    seq_printf(sfile, "\n     0x%02x:", i);
                }
                if (0xff == dev->osd.map.index[i]) {
                    seq_printf(sfile, " ---");
                } else {
                    seq_printf(sfile, " %03d", dev->osd.map.index[i]);
                }
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     disp ram:index:");
            for (i = 0; i < ARRAY_SIZE(dev->osd.disp[0].font); ++i) {
                if (0 == i % 16) {
                    seq_printf(sfile, "\n     0x%02x:", i);
                }
                if (0xff == dev->osd.disp[0].font[i]) {
                    seq_printf(sfile, " ---");
                } else {
                    seq_printf(sfile, " %03d", dev->osd.disp[0].index[i]);
                }
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     disp ram:font:");
            for (i = 0; i < ARRAY_SIZE(dev->osd.disp[0].font); ++i) {
                if (0 == i % 16) {
                    seq_printf(sfile, "\n     0x%02x:", i);
                }
                if (0xff == dev->osd.disp[0].font[i]) {
                    seq_printf(sfile, " ---");
                } else {
                    seq_printf(sfile, " %02xh", dev->osd.disp[0].font[i]);
                }
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     disp ram:symbol:");
            for (i = 0; i < ARRAY_SIZE(dev->osd.disp[0].font); ++i) {
                if (0 == i % 16) {
                    seq_printf(sfile, "\n     0x%02x:", i);
                }
                if (0xff == dev->osd.disp[0].font[i]) {
                    seq_printf(sfile, " ---");
                } else {
                    seq_printf(sfile, " %c  ", dev->osd.disp[0].font[i]);
                }
            }
            seq_printf(sfile, "\n");
            seq_printf(sfile, "     transparnet %d\n", dev->osd.trans[0].level);
            for (i = 0; i < ARRAY_SIZE(dev->osd.palette); ++i) {
                if (dev->osd.palette[i].index < ARRAY_SIZE(dev->osd.palette)) {
                    seq_printf(sfile, "     palette %d:%03d %03d %03d\n", dev->osd.palette[i].index,
                               dev->osd.palette[i].y, dev->osd.palette[i].cb,
                               dev->osd.palette[i].cr);
                }
            }
            seq_printf(sfile, "     transparnet %d\n", dev->osd.trans[0].level);
            for (i = 0; i < max_entity; ++i) {
                if (dev->osd.active[i]) {
                    seq_printf(sfile, "     ch:%d, active:%d", i, dev->osd.active[i]);
                    seq_printf(sfile, " win:%d %d %d %d", dev->osd.win[i].x,
                               dev->osd.win[i].y, dev->osd.win[i].hdim, dev->osd.win[i].vdim);
                    seq_printf(sfile, " string:%03d %d %d <%s>", dev->osd.str[i].start,
                               dev->osd.str[i].fg_color, dev->osd.str[i].bg_color,
                               dev->osd.str[i].string);
                    seq_printf(sfile, " font:%d %d %d %d", dev->osd.font[i].width,
                               dev->osd.font[i].height, dev->osd.font[i].row_space,
                               dev->osd.font[i].col_space);
                    seq_printf(sfile, " \n");
                }
            }
            seq_printf(sfile, "\n");
        }
    }
    spin_unlock_irqrestore(&FSCALER_PROC_LOCK, flags);
    return 0;
}

int fscaler_proc_grabjob_devid_show(struct seq_file *sfile, void *v)
{
    struct fscaler_device *dev = NULL;
    struct fscaler_job *job = NULL;
    struct fscaler_subjob *sjob = NULL;
    unsigned long flags;
    spin_lock_irqsave(&FSCALER_PROC_LOCK, flags);
    dev = fscaler_proc_get_device(FSCALER_PROC_GRABJOB_ID);
    if (dev) {
        seq_printf(sfile, " SCL: Grab job from device %d\n", dev->id);
        job = fscaler_device_fetch_job(dev);
        if (job) {
            seq_printf(sfile,
                       "     job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       dev->job, dev->job->id, dev->job->fd, dev->job->max, dev->job->curr,
                       dev->job->type, dev->job->status, dev->job->prio, dev->job->start_time,
                       dev->job->end_time);
            sjob = fscaler_job_get_subjob(job, job->curr);
            if (sjob) {
                seq_printf(sfile,
                           "     subjob[%d]: fmt:%d, odr:%d IN:{ %d, %d, %d, %d, %d, %d } OUT:{ %d, %d, %d, %d, %d, %d }\n",
                           sjob->index, sjob->out_buf.layout, sjob->out_buf.order, sjob->in_bg_w,
                           sjob->in_bg_h, sjob->in_x, sjob->in_y, sjob->in_w, sjob->in_h,
                           sjob->out_bg_w, sjob->out_bg_h, sjob->out_x, sjob->out_y, sjob->out_w,
                           sjob->out_h);
            } else {
                seq_printf(sfile, "     current subjob[%d] null\n", job->curr);
            }
        } else {
            seq_printf(sfile, "     fetch job null\n");
        }
        if (!klist_empty(&dev->klist_job)) {
            klist_for_each(&dev->klist_job, 0, sfile, klist_proc_grabjob_devid_joblist_show);
        }
    } else {
        seq_printf(sfile, "     device null\n");
    }
    spin_unlock_irqrestore(&FSCALER_PROC_LOCK, flags);
    return 0;
}

int fscaler_proc_grabjob_fd_show(struct seq_file *sfile, void *v)
{
    int i = 0;
    struct fscaler_entity *ent = NULL;
    struct fscaler_device *dev = NULL;
    struct fscaler_job *job = NULL;
    struct fscaler_subjob *sjob = NULL;
    struct fscaler_table *table = NULL;
    unsigned long flags;
    spin_lock_irqsave(&FSCALER_PROC_LOCK, flags);
    ent = fscaler_proc_get_entity(FSCALER_PROC_GRABJOB_FD);
    if (ent) {
        seq_printf(sfile, " SCL: Grab job form entity %d\n", ent->fd);
        table = fscaler_proc_get_table(fscaler_job_table_name());
        if (table && !klist_empty(&table->klist_item)) {
            klist_for_each(&table->klist_item, 0, sfile, klist_proc_float_joblist_show);
        }
        if (!klist_empty(&ent->klist_job)) {
            klist_for_each(&ent->klist_job, 0, sfile, klist_proc_grabjob_fd_joblist_show);
        } else {
            seq_printf(sfile, "     entity joblist empty\n");
        }
        for (i = 0; i < max_device; ++i) {
            dev = fscaler_proc_get_device(i);
            if (dev) {
                job = fscaler_device_fetch_job(dev);
                if (job && job->fd == FSCALER_PROC_GRABJOB_FD) {
                    seq_printf(sfile,
                               "     job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                               dev->job, dev->job->id, dev->job->fd, dev->job->max, dev->job->curr,
                               dev->job->type, dev->job->status, dev->job->prio,
                               dev->job->start_time, dev->job->end_time);
                    sjob = fscaler_job_get_subjob(job, job->curr);
                    if (sjob) {
                        seq_printf(sfile,
                                   "     subjob[%d]: fmt:%d, odr:%d IN:{ %d, %d, %d, %d, %d, %d } OUT:{ %d, %d, %d, %d, %d, %d }\n",
                                   sjob->index, sjob->out_buf.layout, sjob->out_buf.order,
                                   sjob->in_bg_w, sjob->in_bg_h, sjob->in_x, sjob->in_y, sjob->in_w,
                                   sjob->in_h, sjob->out_bg_w, sjob->out_bg_h, sjob->out_x,
                                   sjob->out_y, sjob->out_w, sjob->out_h);
                    }
                } else {
                    if (!job) {
                        seq_printf(sfile, "     job null\n");
                    } else {
                        seq_printf(sfile, "     job is not from fd:%d, its fd:%d, device:%d\n",
                                   FSCALER_PROC_GRABJOB_FD, job->fd, job->id);
                    }
                }
                if (!klist_empty(&dev->klist_job)) {
                    klist_for_each(&dev->klist_job, 0, sfile, klist_proc_grabjob_joblist_show);
                } else {
                    seq_printf(sfile, "     device joblist is empty\n");
                }
            } else {
                seq_printf(sfile, "     device null\n");
            }
        }
    }
    spin_unlock_irqrestore(&FSCALER_PROC_LOCK, flags);
    return 0;
}

int fscaler_proc_kpi_show(struct seq_file *sfile, void *v)
{
    if (sfile) {
        seq_printf(sfile, "echo 1 > /scaler/kpi to output the performance on console\n");
    }
    return 0;
}

int klist_proc_device_joblist_show(struct klist_node *n, void *data)
{
    struct seq_file *sfile = (struct seq_file *)data;
    struct fscaler_job *job = NULL;
    if (sfile && n) {
        job = klist_entry(n, struct fscaler_job, knode_dev);
        if (job) {
            seq_printf(sfile,
                       "     <oq>job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr, job->type, job->status,
                       job->prio, job->start_time, job->end_time);
        }
    }
    return FSCALER_OK;
}

int klist_proc_float_joblist_show(struct klist_node *n, void *data)
{
    int i = 0, j = 0;
    struct seq_file *sfile = (struct seq_file *)data;
    struct fscaler_job *job = NULL;
    if (sfile && n) {
        job = klist_entry(n, struct fscaler_job, knode_table);
        if (job) {
            seq_printf(sfile,
                       "     <float>job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr, job->type, job->status,
                       job->prio, job->start_time, job->end_time);
            seq_printf(sfile, "            job callstack:");
            for (i = 0, j = 0; i < ARRAY_SIZE(job->magic); ++i) {
                if (job->magic[(job->m + i + 1) % ARRAY_SIZE(job->magic)]) {
                    if (0 == j % 3) {
                        seq_printf(sfile, "\n            ");
                    }
                    seq_printf(sfile, "-> %s ",
                               job->magic[(job->m + i + 1) % ARRAY_SIZE(job->magic)]);
                    ++j;
                }
            }
            seq_printf(sfile, "\n");
        } else {
            seq_printf(sfile, "     <float>job null\n");
        }
    }
    return FSCALER_OK;
}

int klist_proc_grabjob_joblist_show(struct klist_node *n, void *data)
{
    struct seq_file *sfile = (struct seq_file *)data;
    struct fscaler_job *job = NULL;
    if (sfile && n) {
        job = klist_entry(n, struct fscaler_job, knode_dev);
        if (job && job->id == FSCALER_PROC_GRABJOB_ID && job->fd == FSCALER_PROC_GRABJOB_FD) {
            seq_printf(sfile,
                       "     <oq>job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr, job->type, job->status,
                       job->prio, job->start_time, job->end_time);
        } else {
            if (!job) {
                seq_printf(sfile, "     <oq>job null\n");
            } else {
                seq_printf(sfile,
                           "     <oq>job is not from fd%d to device:%d, its fd;%d, device:%d\n",
                           FSCALER_PROC_GRABJOB_FD, FSCALER_PROC_GRABJOB_ID, job->fd, job->id);
            }
        }
    }
    return FSCALER_OK;
}

int klist_proc_grabjob_devid_joblist_show(struct klist_node *n, void *data)
{
    struct seq_file *sfile = (struct seq_file *)data;
    struct fscaler_job *job = NULL;
    if (sfile && n) {
        job = klist_entry(n, struct fscaler_job, knode_dev);
        if (job && job->id == FSCALER_PROC_GRABJOB_ID) {
            seq_printf(sfile,
                       "     <oq>job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr, job->type, job->status,
                       job->prio, job->start_time, job->end_time);
        } else {
            if (!job) {
                seq_printf(sfile, "     <oq>job null\n");
            } else {
                seq_printf(sfile, "     <oq>job is not to device:%d, its fd:%d, device:%d\n",
                           FSCALER_PROC_GRABJOB_ID, job->fd, job->id);
            }
        }
    }
    return FSCALER_OK;
}

int klist_proc_grabjob_fd_joblist_show(struct klist_node *n, void *data)
{
    int i = 0;
    struct seq_file *sfile = (struct seq_file *)data;
    struct fscaler_job *job = NULL;
    struct fscaler_subjob *sjob = NULL;
    if (sfile && n) {
        job = klist_entry(n, struct fscaler_job, knode_ent);
        if (job && job->fd == FSCALER_PROC_GRABJOB_FD) {
            seq_printf(sfile,
                       "     <nq>job:%p id:%d fd:%d max:%d curr:%d type:%d status:%d prio:%d, time:%x->%x\n",
                       job, job->id, job->fd, job->max, job->curr, job->type, job->status,
                       job->prio, job->start_time, job->end_time);
            for (i = 0; i < job->max; ++i) {
                sjob = fscaler_job_get_subjob(job, i);
                if (sjob) {
                    seq_printf(sfile,
                               "     <nq>subjob[%d]: fmt:%d, odr:%d IN:{ %d, %d, %d, %d, %d, %d } OUT:{ %d, %d, %d, %d, %d, %d }\n",
                               sjob->index, sjob->out_buf.layout, sjob->out_buf.order,
                               sjob->in_bg_w, sjob->in_bg_h, sjob->in_x, sjob->in_y, sjob->in_w,
                               sjob->in_h, sjob->out_bg_w, sjob->out_bg_h, sjob->out_x, sjob->out_y,
                               sjob->out_w, sjob->out_h);
                }
            }
        } else {
            if (!job) {
                seq_printf(sfile, "     <nq>job null\n");
            } else {
                seq_printf(sfile, "     <nq>job is not from fd:%d, its fd:%d, device:%d\n",
                           FSCALER_PROC_GRABJOB_FD, job->fd, job->id);
            }
        }
    }
    return FSCALER_OK;
}

int fscaler_proc_fops_property_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_property_show, NULL);
}

ssize_t fscaler_proc_fops_property_write(struct file * file, const char __user * buf, size_t size,
                                         loff_t * off)
{
    char value[8];
    if (copy_from_user(value, buf, size))
        return 0;
    value[size] = '\0';
    sscanf(value, "%u \n", &FSCALER_PROC_PROPERTY_FD);
    return size;
}

int fscaler_proc_fops_register_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_register_show, NULL);
}

int fscaler_proc_fops_device_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_device_show, NULL);
}

ssize_t fscaler_proc_fops_device_write(struct file * file, const char __user * buf, size_t size,
                                       loff_t * off)
{
    char value[8];
    if (copy_from_user(value, buf, size))
        return 0;
    value[size] = '\0';
    sscanf(value, "%u \n", &FSCALER_PROC_DEVICE_ID);
    return size;
}

int fscaler_proc_fops_grabjob_devid_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_grabjob_devid_show, NULL);
}

ssize_t fscaler_proc_fops_grabjob_devid_write(struct file * file, const char __user * buf,
                                              size_t size, loff_t * off)
{
    char value[8];
    if (copy_from_user(value, buf, size))
        return 0;
    value[size] = '\0';
    sscanf(value, "%u \n", &FSCALER_PROC_GRABJOB_ID);
    return size;
}

int fscaler_proc_fops_grabjob_fd_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_grabjob_fd_show, NULL);
}

ssize_t fscaler_proc_fops_grabjob_fd_write(struct file * file, const char __user * buf, size_t size,
                                           loff_t * off)
{
    char value[8];
    if (copy_from_user(value, buf, size))
        return 0;
    value[size] = '\0';
    sscanf(value, "%u \n", &FSCALER_PROC_GRABJOB_FD);
    return size;
}

static int fscaler_proc_fops_kpi_open(struct inode *inode, struct file *file)
{
    return single_open(file, fscaler_proc_kpi_show, NULL);
}

ssize_t fscaler_proc_fops_kpi_write(struct file * file, const char __user * buf, size_t size,
                                    loff_t * off)
{
    char value[8];
    if (copy_from_user(value, buf, size))
        return 0;
    value[size] = '\0';
    sscanf(value, "%u \n", &supp_kpi);
    return size;
}

void fscaler_proc_uninstall(struct fscaler_interface *inter)
{
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    if (drv && fscaler_interface_installed(inter)) {
        fscaler_proc_kpi_remove();
        fscaler_proc_grabjob_remove();
        fscaler_proc_device_remove();
        fscaler_proc_register_remove();
        fscaler_proc_property_remove();
        fscaler_proc_destroy();
        fscaler_proc_set_owner(NULL);
    }
}

int fscaler_proc_install(struct fscaler_interface *inter)
{
    int hr = 0;
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    M_DEVELOPP("SCL: Try to install proc interface\n");
    if (drv && !fscaler_interface_installed(inter)) {
        hr = fscaler_proc_create(&FSCALER_PROC_NAME[0]);
        if (FSCALER_SUCCEED(hr)) {
            spin_lock_init(&FSCALER_PROC_LOCK);
            fscaler_proc_property_request();
            fscaler_proc_register_request();
            fscaler_proc_device_request();
            fscaler_proc_grabjob_request();
            fscaler_proc_kpi_request();
            fscaler_proc_set_owner(drv);
            return FSCALER_OK;
        }
        M_ERROR("SCL: Failed to createl proc fs\n");
    }
    M_ERROR("SCL: Failed to install proc interface\n");
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
int fscaler_proc_interface_request(struct fscaler_driver *drv)
{
    int hr = FSCALER_OK;
    hr = fscaler_interface_request(drv, &FSCALER_PROC_NAME[0], supp_proc,
                                   fscaler_proc_install, fscaler_proc_uninstall);
    if (FSCALER_SUCCEED(hr)) {
        M_DEVELOPP("SCL: Request proc interface, OK\n");
        return FSCALER_OK;
    }
    M_ERROR("SCL: Failed to request proc interface\n");
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
void fscaler_proc_interface_remove(struct fscaler_driver *drv)
{
    struct fscaler_interface *inter = fscaler_driver_interface_find(drv, &FSCALER_PROC_NAME[0]);
    if (!fscaler_proc_get_owner()) {
        if (inter) {
            fscaler_interface_remove(inter);
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
const char *fscaler_proc_interface_name(void)
{
    return &FSCALER_PROC_NAME[0];
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_proc_request);
EXPORT_SYMBOL(fscaler_proc_remove);
