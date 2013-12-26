/**
 * @file fscaler_table.h
 * @brief Scaler table header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_TABLE_H_
#define _FSCALER_TABLE_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_driver;
struct fscaler_module;
struct fscaler_table;

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
#include <linux/spinlock.h>

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO
#define TIME_INIT               (jiffies_to_msecs(jiffies))
#define TIME_AFTER(a,b)         (typecheck(unsigned int, a) && typecheck(unsigned int, b) && ((int)(b) - (int)(a) < 0))
#define TIME_BEFORE(a,b)        TIME_AFTER(b,a)

#define LKP_CODE(cmd,act)       (int)(((unsigned int)(cmd)<<16)|((unsigned int)(act)))
#define LKP_CTL(hr)             (int)(((unsigned int)(hr)&0xFFFF0000)>>16)
#define LKP_ACT(hr)             (int)((unsigned int)(hr)&0x0000FFFF)
#define LKP_IS_CTL(hr,cmd)      (int)(LKP_CTL(hr) == (int)(cmd))
#define LKP_IS_ACT(hr,cmd)      (int)(LKP_ACT(hr) == (int)(cmd))

#define LKP_CTL_BREAK           0x0001
#define LKP_ACT_EXCHANGE        0x0001

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
struct fscaler_table {
    struct fscaler_driver *parent;
    struct klist_node knode_drv;
    struct klist klist_item;
    int (*cache) (struct fscaler_table *);
    void (*uncache) (struct fscaler_table *);
    void *(*request) (struct fscaler_table *, int);
    void (*remove) (struct fscaler_table *, void *);
    int (*compare) (void *, void *);
    const char *name;
    spinlock_t lock;
    struct kmem_cache *kmem;
    unsigned int max;
    unsigned long *bitmap;
    void **map;
};

///////////////////////////////////////////////////////////////////////////////
// EXTERN PROTOTYPE
extern int fscaler_table_request(struct fscaler_driver *drv, const char *name, unsigned int max,
                                 void (*create) (struct klist_node *),
                                 void (*destroy) (struct klist_node *),
                                 int (*cache) (struct fscaler_table *),
                                 void (*uncache) (struct fscaler_table *),
                                 void *(*request) (struct fscaler_table *, int),
                                 void (*remove) (struct fscaler_table *, void *),
                                 int (*compare) (void *, void *));
extern void fscaler_table_remove(struct fscaler_table *table);
extern void fscaler_table_create(struct klist_node *n);
extern void fscaler_table_destroy(struct klist_node *n);
extern struct fscaler_module *fscaler_table_to_module(struct fscaler_table *table);
extern struct fscaler_table *fscaler_table_to_table(struct fscaler_table *table, const char *name);

///////////////////////////////////////////////////////////////////////////////
// STATIC INLINE FUNCTION
static inline struct fscaler_driver *fscaler_table_to_driver(struct fscaler_table *table)
{
    return table ? table->parent : NULL;
}

static inline void fscaler_table_callback_register(struct fscaler_table *table,
                                                   int (*cache) (struct fscaler_table *),
                                                   void (*uncache) (struct fscaler_table *),
                                                   void *(*request) (struct fscaler_table *, int),
                                                   void (*remove) (struct fscaler_table *, void *),
                                                   int (*compare) (void *, void *))
{
    if (table) {
        table->cache = cache;
        table->uncache = uncache;
        table->request = request;
        table->remove = remove;
        table->compare = compare;
    }
}

static inline void fscaler_table_item_lock(struct fscaler_table *table, int index)
{
    if (table && table->bitmap) {
        if (index >= 0 && index < table->max) {
            if (1 == table->max) {
                table->bitmap[0] = 0;
            } else {
                clear_bit(index, table->bitmap);
            }
        }
    }
}

static inline void fscaler_table_item_unlock(struct fscaler_table *table, int index)
{
    if (table && table->bitmap) {
        if (index >= 0 && index < table->max) {
            if (1 == table->max) {
                table->bitmap[0] = 1;
            } else {
                set_bit(index, table->bitmap);
            }
        }
    }
}

static inline int fscaler_table_item_locked(struct fscaler_table *table, int index)
{
    return (table && table->bitmap && 0 <= index
            && table->max > index) ? (!test_bit(index, table->bitmap)) : 1;
}

static inline int fscaler_table_item_cache(struct fscaler_table *table)
{
    return (table && table->cache) ? table->cache(table) : FSCALER_ERR_CREATE;
}

static inline void fscaler_table_item_uncache(struct fscaler_table *table)
{
    if (table && table->uncache) {
        table->uncache(table);
    }
}

static inline void *fscaler_table_item_request(struct fscaler_table *table, int index)
{
    return (table && table->request) ? table->request(table, index) : NULL;
}

static inline void fscaler_table_item_remove(struct fscaler_table *table, void *data)
{
    if (table && table->remove) {
        table->remove(table, data);
    }
}

static inline void *fscaler_table_item_find(struct fscaler_table *table, int index)
{
    return (table && table->map && 0 <= index && table->max > index) ? table->map[index] : NULL;
}

static inline void *fscaler_table_item_lookup(struct fscaler_table *table)
{
    int nr = 0, hr = 0;
    void *data = NULL, *found = NULL;
    if (table && table->bitmap && table->compare) {
        M_DEVELOPP("SCL: table, name:%s, max:%d, bitmap[0]:%lx\n", table->name, table->max,
                   table->bitmap[0]);
        if (1 == table->max) {
            nr = table->bitmap[0] ? 0 : 1;
            M_DEVELOPP("SCL: nr:%d, locked:%d\n", nr, fscaler_table_item_locked(table, nr));
            data = fscaler_table_item_find(table, nr);
            if (data) {
                hr = table->compare(data, found);
                M_DEVELOPP("SCL: compare(), hr, %x\n", hr);
                if (LKP_IS_ACT(hr, LKP_ACT_EXCHANGE)) {
                    found = data;
                }
            } else {
                M_DEVELOPP("SCL: lookup, out of range, %d\n", nr);
            }
        } else {
            for (nr = find_first_bit(table->bitmap, table->max); nr < table->max;
                 nr = find_next_bit(table->bitmap, table->max, nr + 1)) {
                if (nr >= table->max) {
                    M_DEVELOPP("SCL: lookup out of range, %d\n", nr);
                    break;
                }
                data = fscaler_table_item_find(table, nr);
                if (data) {
                    hr = table->compare(data, found);
                    M_DEVELOPP("SCL: compare(), hr, %x\n", hr);
                    if (LKP_IS_ACT(hr, LKP_ACT_EXCHANGE)) {
                        found = data;
                    }
                    if (LKP_IS_CTL(hr, LKP_CTL_BREAK)) {
                        break;
                    }
                }
            }
        }
    }
    M_DEVELOPP("SCL: %s, found:%p\n", table->name, found);
    return found;
}

#endif /* _FSCALER_TABLE_H_ */
