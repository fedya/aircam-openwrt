/**
 * @file fscaler_interface.c
 * @brief Scaler interface system
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_interface.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
STATIC_INLINE struct fscaler_interface *fscaler_interface_allocate(struct fscaler_driver *drv,
                                                                   const char *name, int supp)
{
    struct fscaler_interface *inter = NULL;
    if (drv && supp && name) {
        inter = FSCALER_SALLOC(fscaler_interface);
        if (inter) {
            inter->parent = drv;
            inter->name = name;
            inter->install = NULL;
            inter->uninstall = NULL;
            inter->attached = 0;
            return inter;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_interface_free(struct fscaler_interface *inter)
{
    if (inter) {
        inter->parent = NULL;
        FSCALER_FREE(inter);
        inter = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION

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
int fscaler_interface_request(struct fscaler_driver *drv, const char *name, int supp,
                              int (*install) (struct fscaler_interface *),
                              void (*uninstall) (struct fscaler_interface *))
{
    struct fscaler_interface *inter = NULL;
    if (drv && name) {
        inter = fscaler_interface_allocate(drv, name, supp);
        if (inter) {
            fscaler_interface_callback_register(inter, install, uninstall);
            klist_add_tail(&inter->knode_drv, &drv->klist_inter);
            if (inter->attached) {
                return FSCALER_OK;
            }
            klist_del(&inter->knode_drv);
            fscaler_interface_callback_register(inter, 0, 0);
            fscaler_interface_free(inter);
            inter = NULL;
        }
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
void fscaler_interface_remove(struct fscaler_interface *inter)
{
    if (inter) {
        klist_del(&inter->knode_drv);
        fscaler_interface_callback_register(inter, 0, 0);
        fscaler_interface_free(inter);
        inter = NULL;
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
void fscaler_interface_create(struct klist_node *n)
{
    int hr = 0;
    struct fscaler_interface *inter = klist_entry(n, struct fscaler_interface, knode_drv);
    if (inter) {
        hr = fscaler_interface_install(inter);
        if (FSCALER_SUCCEED(hr)) {
            return;
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
void fscaler_interface_destroy(struct klist_node *n)
{
    struct fscaler_interface *inter = klist_entry(n, struct fscaler_interface, knode_drv);
    if (inter) {
        fscaler_interface_uninstall(inter);
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
struct fscaler_module *fscaler_interface_to_module(struct fscaler_interface *inter)
{
    return fscaler_driver_to_module(fscaler_interface_to_driver(inter));
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_interface_request);
EXPORT_SYMBOL(fscaler_interface_remove);
EXPORT_SYMBOL(fscaler_interface_create);
EXPORT_SYMBOL(fscaler_interface_destroy);
EXPORT_SYMBOL(fscaler_interface_to_module);
