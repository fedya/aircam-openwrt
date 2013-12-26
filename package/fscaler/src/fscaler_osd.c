/**
 * @file fscaler_osd.c
 * @brief Scaler driver OSD control interface implementation
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_osd.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"
#include "fscaler_property.h"
#include "fscaler_job.h"
#include "fscaler_entity.h"
#include "fscaler_device.h"
#include "fscaler_osdapi.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18))
#include <linux/uaccess.h>
#endif /*LINUX_VERSION_CODE */

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO
#define FSCOSD_DYNAMIC_MAJOR                10
#define FSCOSD_DYNAMIC_MINOR                (30<<8)
#define FSCOSD_CH_TO_FD(minor)              (((minor) >= FSCALER_OSD_MINOR) ? ((minor)-FSCALER_OSD_MINOR) : (minor))
#define FSCOSD_MAX_FONT_W                   12
#define FSCOSD_MAX_FONT_H                   18
///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static int fscaler_osd_fops_open(struct inode *inode, struct file *file);
static int fscaler_osd_fops_release(struct inode *inode, struct file *file);
static int fscaler_osd_fops_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
                                  unsigned long arg);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_OSD_NAME[] = "fscosd";
static char FSCALER_OSD_DEVICE_NAME[] = "scl_osd0";
static struct fscaler_driver *FSCALER_OSD_OWNER = NULL;
struct file_operations FSCALER_OSD_FOPS = {
    .owner = THIS_MODULE,
    .open = fscaler_osd_fops_open,
    .release = fscaler_osd_fops_release,
    .ioctl = fscaler_osd_fops_ioctl,
};

struct miscdevice FSCALER_OSD_DEVICE = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = FSCALER_OSD_DEVICE_NAME,
    .fops = &FSCALER_OSD_FOPS
};

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE void fscaler_osd_set_owner(struct fscaler_driver *drv)
{
    FSCALER_OSD_OWNER = drv;
}

STATIC_INLINE struct fscaler_driver *fscaler_osd_get_owner(void)
{
    return FSCALER_OSD_OWNER;
}

STATIC_INLINE struct fscaler_table *fscaler_osd_get_table(const char *name)
{
    return fscaler_driver_table_find(FSCALER_OSD_OWNER, name);
}

STATIC_INLINE void *fscaler_osd_table_item_find(const char *name, int index)
{
    return fscaler_table_item_find(fscaler_osd_get_table(name), index);
}

STATIC_INLINE struct fscaler_property *fscaler_osd_get_property(int fd)
{
    return (struct fscaler_property *)fscaler_osd_table_item_find(fscaler_property_table_name(),
                                                                  fd);
}

STATIC_INLINE struct fscaler_entity *fscaler_osd_get_entity(int fd)
{
    return (struct fscaler_entity *)fscaler_osd_table_item_find(fscaler_entity_table_name(), fd);
}

STATIC_INLINE struct fscaler_device *fscaler_osd_get_device(int fd)
{
    return (struct fscaler_device *)fscaler_osd_table_item_find(fscaler_device_table_name(), fd);
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_osd_fops_open(struct inode *inode, struct file *file)
{
    M_DEVELOPP("SCL: OSD open()!\n");
    file->private_data = NULL;
    return 0;
}

int fscaler_osd_fops_release(struct inode *inode, struct file *file)
{
    M_DEVELOPP("SCL: OSD close()!\n");
    file->private_data = NULL;
    return 0;
}

int fscaler_osd_fops_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
                           unsigned long arg)
{
    int ret = FSCALER_OK;
    struct fscaler_driver *drv = fscaler_osd_get_owner();
    switch (cmd) {
    case FIOSDS_ON:{
            struct fiosd_ch tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_ON\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            tmp.channel = FSCOSD_CH_TO_FD(tmp.channel);
            M_DEVELOPP("SCL: ch:%d\n", tmp.channel);
            ret = fscaler_osd_set_enable(drv, &tmp, 1);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_ERROR("SCL: Failed to turn on scaler osd\n");
            }
            break;
        }
    case FIOSDS_OFF:{
            struct fiosd_ch tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_OFF\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            tmp.channel = FSCOSD_CH_TO_FD(tmp.channel);
            M_DEVELOPP("SCL: ch:%d\n", tmp.channel);
            ret = fscaler_osd_set_enable(drv, &tmp, 0);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_ERROR("SCL: Failed to turn on scaler osd\n");
            }
            break;
        }
    case FIOSDS_WIN:{
            struct fiosd_win tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_WIN\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            tmp.channel = FSCOSD_CH_TO_FD(tmp.channel);
            M_DEVELOPP("SCL: ch:%d\n", tmp.channel);
            ret = fscaler_osd_set_win(drv, &tmp);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_ERROR("SCL: Failed to set scaler osd window\n");
            }
            break;
        }
    case FIOSDS_FONTSETTING:{
            struct fiosd_font_info tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_FONTSETTING\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            tmp.channel = FSCOSD_CH_TO_FD(tmp.channel);
            M_DEVELOPP("SCL: ch:%d\n", tmp.channel);
            ret = fscaler_osd_set_font_size(drv, &tmp);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_ERROR("SCL: Failed to set scaler osd font info\n");
            }
            break;
        }
    case FIOSDS_TRANSPARENT:{
            struct fiosd_transparent tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_TRANSPARENT\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            ret = fscaler_osd_set_transparent(drv, &tmp);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_WARN("SCL: Failed to set scaler osd transparency\n");
            }
            break;
        }
    case FIOSDS_STRING:{
            struct fiosd_string tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_STRING\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            tmp.channel = FSCOSD_CH_TO_FD(tmp.channel);
            M_DEVELOPP("SCL: ch:%d\n", tmp.channel);
            ret = fscaler_osd_set_string(drv, &tmp);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_ERROR("SCL: Failed to set scaler osd string\n");
            }
            break;
        }
    case FIOSDS_PALTCOLOR:{
            struct fiosd_palette tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_PALCOLOR\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            ret = fscaler_osd_set_palette(drv, &tmp);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_WARN("SCL: Failed to set scaler osd palette\n");
            }
            break;
        }
    case FIOSDS_CHAR:{
            fiosd_char_t tmp;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_CHAR\n");
            if (copy_from_user(&tmp, (void __user *)arg, sizeof(tmp))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = FSCALER_ERR_FAIL;
                break;
            }
            ret = fscaler_osd_set_font(drv, &tmp, 1);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_WARN("SCL: Failed to set scaler osd font char\n");
            }
            break;
        }
    case FIOSDS_RMCHAR:{
            fiosd_char_t tmp;
            char data;
            M_DEVELOPP("SCL: IOCTL, FIOSDS_RMCHAR\n");
            if (copy_from_user(&data, (void __user *)arg, sizeof(data))) {
                M_ERROR("SCL: Failed to ccopy data from user\n");
                ret = -EFAULT;
                break;
            }
            tmp.font = data;
            ret = fscaler_osd_set_font(drv, &tmp, 0);
            if (unlikely(FSCALER_FAILED(ret))) {
                M_WARN("SCL: Failed to remove scaler osd font char\n");
            }
            break;
        }
    default:
        M_DEVELOPP("SCL: IOCTL, known ioctl:%x\n", cmd);
        ret = FSCALER_ERR_UNSUPPORTED;
        M_ERROR("SCL: Unsupport scaler ioctl\n");
        break;
    }
    M_DEVELOPP("SCL: IOCTL, ret:%x\n", ret);
    if (FSCALER_FAILED(ret)) {
        if (FSCALER_ERR_FAIL == ret) {
            return -EFAULT;
        } else if (FSCALER_ERR_UNSUPPORTED == ret) {
            return -ENOIOCTLCMD;
        }
        return -EINVAL;
    }
    return 0;
}

void fscaler_osd_uninstall(struct fscaler_interface *inter)
{
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    if (drv && fscaler_interface_installed(inter)) {
        if (0 > misc_deregister(&FSCALER_OSD_DEVICE)) {
            M_ERROR("SCL: deregister misc device failed !!!\n");
        }
        fscaler_osd_remove(drv);
        fscaler_osd_set_owner(NULL);
    }
}

int fscaler_osd_install(struct fscaler_interface *inter)
{
    int hr = 0;
    struct fscaler_driver *drv = fscaler_interface_to_driver(inter);
    M_DEVELOPP("SCL: Try to install OSD interface\n");
    if (drv && !fscaler_interface_installed(inter)) {
        hr = fscaler_osd_request(drv);
        if (FSCALER_SUCCEED(hr)) {
            if (0 <= misc_register(&FSCALER_OSD_DEVICE)) {
                fscaler_osd_set_owner(drv);
                return FSCALER_OK;
            }
            M_ERROR("SCL: Failed to register OSD IOCTL\n");
        }
        M_ERROR("SCL: Failed to request OSD resource\n");
    }
    M_ERROR("SCL: Failed to install OSD interface\n");
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
int fscaler_osd_interface_request(struct fscaler_driver *drv)
{
    int hr = FSCALER_OK;
    hr = fscaler_interface_request(drv, &FSCALER_OSD_NAME[0], supp_osd,
                                   fscaler_osd_install, fscaler_osd_uninstall);
    if (FSCALER_SUCCEED(hr)) {
        M_DEVELOPP("SCL: Request OSD interface, OK\n");
        return FSCALER_OK;
    }
    M_ERROR("SCL: Failed to request OSD interface\n");
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
void fscaler_osd_interface_remove(struct fscaler_driver *drv)
{
    struct fscaler_interface *inter = fscaler_driver_interface_find(drv, &FSCALER_OSD_NAME[0]);
    if (!fscaler_osd_get_owner()) {
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
const char *fscaler_osd_interface_name(void)
{
    return &FSCALER_OSD_NAME[0];
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_osd_interface_request);
EXPORT_SYMBOL(fscaler_osd_interface_remove);
EXPORT_SYMBOL(fscaler_osd_interface_name);
