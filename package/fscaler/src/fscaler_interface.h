/**
 * @file fscaler_interface.h
 * @brief Scaler interface header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_INTERFACE_H_
#define _FSCALER_INTERFACE_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_driver;
struct fscaler_module;
struct fscaler_interface;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"
#include "fscaler_list.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
struct fscaler_interface {
    struct fscaler_driver *parent;
    struct klist_node knode_drv;
    int (*install) (struct fscaler_interface *);
    void (*uninstall) (struct fscaler_interface *);
    const char *name;
    int attached;
};

///////////////////////////////////////////////////////////////////////////////
// EXTERN PROTOTYPE
extern int fscaler_interface_request(struct fscaler_driver *drv, const char *name, int supp,
                                     int (*install) (struct fscaler_interface *),
                                     void (*uninstall) (struct fscaler_interface *));
extern void fscaler_interface_remove(struct fscaler_interface *inter);
extern void fscaler_interface_create(struct klist_node *n);
extern void fscaler_interface_destroy(struct klist_node *n);
extern struct fscaler_module *fscaler_interface_to_module(struct fscaler_interface *inter);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline struct fscaler_driver *fscaler_interface_to_driver(struct fscaler_interface *inter)
{
    return inter ? inter->parent : NULL;
}

static inline void fscaler_interface_callback_register(struct fscaler_interface *inter,
                                                       int (*install) (struct fscaler_interface *),
                                                       void (*uninstall) (struct fscaler_interface
                                                                          *))
{
    if (inter) {
        inter->install = install;
        inter->uninstall = uninstall;
    }
}

static inline int fscaler_interface_install(struct fscaler_interface *inter)
{
    int hr = 0;
    if (inter) {
        if (!inter->attached) {
            if (!inter->install) {
                goto ERROR_EXIT;
            }
            hr = inter->install(inter);
            if (FSCALER_FAILED(hr)) {
                goto ERROR_EXIT;
            }
            inter->attached = 1;
        }
        return FSCALER_OK;
    }
  ERROR_EXIT:
    return FSCALER_ERR_CREATE;
}

static inline void fscaler_interface_uninstall(struct fscaler_interface *inter)
{
    if (inter) {
        if (inter->attached) {
            if (inter->uninstall) {
                inter->uninstall(inter);
            }
            inter->attached = 0;
        }
    }
}

static inline int fscaler_interface_installed(struct fscaler_interface *inter)
{
    return inter ? inter->attached : 0;
}

#endif /* _FSCALER_INTERFACE_H_ */
