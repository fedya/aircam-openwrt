/**
 * @file fscaler_table.c
 * @brief Scaler table system
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.03
 * @date 2010 Nov 9
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_table.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/bitmap.h>

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
STATIC_INLINE struct fscaler_table *fscaler_table_allocate(struct fscaler_driver *drv,
                                                           const char *name, unsigned int max,
                                                           void (*create) (struct klist_node *),
                                                           void (*destroy) (struct klist_node *))
{
    struct fscaler_table *table = NULL;
    if (drv && name) {
        table = FSCALER_SALLOC(fscaler_table);
        if (table) {
            table->parent = drv;
            table->name = name;
            table->max = max;
            table->cache = NULL;
            table->uncache = NULL;
            table->request = NULL;
            table->remove = NULL;
            table->compare = NULL;
            spin_lock_init(&table->lock);
            klist_init(&table->klist_item, create, destroy);
            return table;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_table_free(struct fscaler_table *table)
{
    if (table) {
        table->max = 0;
        table->parent = NULL;
        FSCALER_FREE(table);
        table = NULL;
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
int fscaler_table_request(struct fscaler_driver *drv, const char *name, unsigned int max,
                          void (*create) (struct klist_node *),
                          void (*destroy) (struct klist_node *),
                          int (*cache) (struct fscaler_table *),
                          void (*uncache) (struct fscaler_table *),
                          void *(*request) (struct fscaler_table *, int),
                          void (*remove) (struct fscaler_table *, void *),
                          int (*compare) (void *, void *))
{
    struct fscaler_table *table = NULL;
    if (drv && name) {
        table = fscaler_table_allocate(drv, name, max, create, destroy);
        if (table) {
            fscaler_table_callback_register(table, cache, uncache, request, remove, compare);
            klist_add_tail(&table->knode_drv, &drv->klist_table);
            if (table->kmem) {
                return FSCALER_OK;
            }
            klist_del(&table->knode_drv);
            fscaler_table_callback_register(table, 0, 0, 0, 0, 0);
            fscaler_table_free(table);
            table = NULL;
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
void fscaler_table_remove(struct fscaler_table *table)
{
    if (table) {
        klist_del(&table->knode_drv);
        fscaler_table_callback_register(table, 0, 0, 0, 0, 0);
        fscaler_table_free(table);
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
void fscaler_table_create(struct klist_node *n)
{
    int hr = 0, i = 0;
    struct fscaler_table *table = klist_entry(n, struct fscaler_table, knode_drv);
    if (table) {
        hr = fscaler_table_item_cache(table);
        if (FSCALER_SUCCEED(hr)) {
            if (table->max) {
                table->bitmap = FSCALER_ALLOC(sizeof(unsigned long), BITS_TO_LONGS(table->max));
                if (table->bitmap) {
                    bitmap_zero(table->bitmap, table->max);
                    table->map = FSCALER_ALLOC(sizeof(void *), table->max);
                    if (table->map) {
                        memset(table->map, 0, (sizeof(void *)) * (table->max));
                        for (i = 0; i < table->max; ++i) {
                            table->map[i] = fscaler_table_item_request(table, i);
                            if (unlikely(!table->map[i])) {
                                break;
                            }
                        }
                        if (i >= table->max) {
                            return;
                        }
                        while (--i >= 0) {
                            fscaler_table_item_remove(table, table->map[i]);
                            table->map[i] = NULL;
                        }
                        FSCALER_FREE(table->map);
                        table->map = NULL;
                    }
                    FSCALER_FREE(table->bitmap);
                    table->bitmap = NULL;
                }
                table->max = 0;
                return;
            }
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
void fscaler_table_destroy(struct klist_node *n)
{
    int i = 0;
    struct fscaler_table *table = klist_entry(n, struct fscaler_table, knode_drv);
    if (table) {
        if (table->max) {
            if (table->map) {
                for (i = 0; i < table->max; ++i) {
                    if (table->map[i]) {
                        fscaler_table_item_remove(table, table->map[i]);
                        table->map[i] = NULL;
                    }
                }
                FSCALER_FREE(table->map);
                table->map = NULL;
            }
            if (table->bitmap) {
                FSCALER_FREE(table->bitmap);
                table->bitmap = NULL;
            }
            table->max = 0;
        }
        fscaler_table_item_uncache(table);
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
struct fscaler_module *fscaler_table_to_module(struct fscaler_table *table)
{
    return fscaler_driver_to_module(fscaler_table_to_driver(table));
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
struct fscaler_table *fscaler_table_to_table(struct fscaler_table *table, const char *name)
{
    if (0 != strcmp(table->name, name)) {
        return fscaler_driver_table_find(fscaler_table_to_driver(table), name);
    }
    return table;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_table_request);
EXPORT_SYMBOL(fscaler_table_remove);
EXPORT_SYMBOL(fscaler_table_create);
EXPORT_SYMBOL(fscaler_table_destroy);
EXPORT_SYMBOL(fscaler_table_to_module);
EXPORT_SYMBOL(fscaler_table_to_table);
