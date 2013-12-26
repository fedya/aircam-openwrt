/**
 * @file fscaler_driver.h
 * @brief Scaler driver header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_DRIVER_H_
#define _FSCALER_DRIVER_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_driver;
struct fscaler_module;
struct fscaler_job;
struct fscaler_entity;
struct fscaler_interface;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"
#include "fscaler_list.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
struct fscaler_driver {
    struct fscaler_module *parent;
    struct klist_node knode_mod;
    struct klist klist_table;
    struct klist klist_inter;
    const char *name;
    int attached;
};

///////////////////////////////////////////////////////////////////////////////
// EXTERN FUNCTION PROTOTYPE
extern int fscaler_driver_request(struct fscaler_module *mod);
extern void fscaler_driver_remove(struct fscaler_driver *drv);
extern void fscaler_driver_create(struct klist_node *n);
extern void fscaler_driver_destroy(struct klist_node *n);
extern int fscaler_driver_open(struct fscaler_entity *ent);
extern void fscaler_driver_close(struct fscaler_entity *ent);
extern void fscaler_driver_dispatch(struct fscaler_entity *ent, struct fscaler_job *job);
extern const char *fscaler_driver_name(void);
extern struct fscaler_table *fscaler_driver_table_find(struct fscaler_driver *drv,
                                                       const char *name);
extern struct fscaler_interface *fscaler_driver_interface_find(struct fscaler_driver *drv,
                                                               const char *name);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline struct fscaler_module *fscaler_driver_to_module(struct fscaler_driver *drv)
{
    return drv ? drv->parent : NULL;
}

#endif /* _FSCALER_DRIVER_H_ */
