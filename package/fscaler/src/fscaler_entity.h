/**
 * @file fscaler_entity.h
 * @brief Scaler entity table
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_ENTITY_H_
#define _FSCALER_ENTITY_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_driver;
struct fscaler_table;
struct fscaler_entity;
struct fscaler_property;
struct fscaler_job;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_table.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/spinlock.h>

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION

typedef struct fscaler_entity {
    struct fscaler_table *parent;
    struct klist_node knode_table;
    struct klist klist_job;
    struct fscaler_property *prop;
    int fd;
    int opened;
    spinlock_t lock;
} fscaler_entity_t;

///////////////////////////////////////////////////////////////////////////////
// EXTERN PROTOTYPE
extern int fscaler_entity_table_request(struct fscaler_driver *drv);
extern void fscaler_entity_table_remove(struct fscaler_driver *drv);
extern int fscaler_entity_open(struct fscaler_entity *ent);
extern void fscaler_entity_close(struct fscaler_entity *ent);
extern void fscaler_entity_putjob(struct fscaler_entity *ent, struct fscaler_job *job);
extern struct fscaler_job *fscaler_entity_getjob(struct fscaler_entity *ent);
extern const char *fscaler_entity_table_name(void);
extern int fscaler_entity_empty_all(void);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline struct fscaler_table *fscaler_entity_to_table(struct fscaler_entity *ent)
{
    return ent ? ent->parent : NULL;
}

static inline struct fscaler_entity *fscaler_entity_find(struct fscaler_table *table, int fd)
{
    return (struct fscaler_entity *)fscaler_table_item_find(table, fd);
}

static inline void fscaler_entity_lock(struct fscaler_entity *ent)
{
    struct fscaler_table *table = fscaler_entity_to_table(ent);
    if (table && ent->opened) {
        fscaler_table_item_lock(table, ent->fd);
    }
}

static inline void fscaler_entity_unlock(struct fscaler_entity *ent)
{
    struct fscaler_table *table = fscaler_entity_to_table(ent);
    if (table && ent->opened) {
        fscaler_table_item_unlock(table, ent->fd);
    }
}

static inline int fscaler_entity_locked(struct fscaler_entity *ent)
{
    return ent ? fscaler_table_item_locked(fscaler_entity_to_table(ent), ent->fd) : 1;
}

static inline struct fscaler_entity *fscaler_entity_sched(struct fscaler_table *table)
{
    return (struct fscaler_entity *)fscaler_table_item_lookup(table);
}

#endif /* _FSCALER_ENTITY_H_ */
