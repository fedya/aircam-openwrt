/**
 * @file videograph_device.c
 * @brief video graph entity device
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"
#include "videograph_device.h"

#define VIDEOGRAPH_GET_MAJOR(fd)      ENTITY_FD_MAJOR(fd)
#define VIDEOGRAPH_GET_MINOR(fd)      ENTITY_FD_MINOR(fd)

typedef struct videograph_entity_node {
    struct video_entity_t entity;
    struct list_head node;
} videograph_entity_node_t;

static int videograph_entity_list_count = 0;
LIST_HEAD(videograph_entity_list);

STATIC_INLINE void videograph_entiry_list_add(struct videograph_entity_node *p_node)
{
    list_add(&p_node->node, &videograph_entity_list);
    ++videograph_entity_list_count;
}

STATIC_INLINE void videograph_entity_list_remove(struct videograph_entity_node *p_node)
{
    list_del_init(&p_node->node);
    --videograph_entity_list_count;
}

STATIC_INLINE struct videograph_entity_node *videograph_entity_list_find(int fd)
{
    struct videograph_entity_node *pos = NULL;
    list_for_each_entry(pos, &videograph_entity_list, node) {
        if (VIDEOGRAPH_ENTITY_TO_FD(&pos->entity) == fd) {
            break;
        }
        pos = NULL;
    }
    return pos;
}

STATIC_INLINE void videograph_entity_init(struct video_entity_t *p_ve, char *prefix, int fd,
                                          struct v_operations *p_v_ops)
{
    char name[32] = { 0 };
    p_ve->major = VIDEOGRAPH_DYNAMIC_MAJOR;
    p_ve->minor = VIDEOGRAPH_DYNAMIC_MINOR + fd;
    p_ve->flushable = 0;
    if (prefix) {
        sprintf(name, "%s_%d", prefix, fd);
    } else {
        sprintf(name, "scaler_%d", fd);
    }
    strcpy(p_ve->name, name);
    p_ve->v_ops = p_v_ops;
}

STATIC_INLINE void videograph_entity_node_init(struct videograph_entity_node *p_node,
                                               int fd, char *name, struct v_operations *p_ops)
{
    if (p_node) {
        INIT_LIST_HEAD(&p_node->node);
        videograph_entity_init(&p_node->entity, name, fd, p_ops);
    }
}

STATIC_INLINE void videograph_entity_node_free(struct videograph_entity_node **pp_node)
{
    struct videograph_entity_node *p_node = pp_node ? (*pp_node) : NULL;
    if (p_node) {
        videograph_entity_list_remove(p_node);
        FSCALER_FREE(p_node);
        *pp_node = p_node = NULL;
    }
}

STATIC_INLINE struct videograph_entity_node *videograph_entity_node_alloc(int fd, char *name, struct v_operations
                                                                          *p_ops)
{
    struct videograph_entity_node *p_node = FSCALER_SALLOC(videograph_entity_node);
    if (p_node) {
        videograph_entity_node_init(p_node, fd, name, p_ops);
        videograph_entiry_list_add(p_node);
    }
    return p_node;
}

int videograph_device_register(struct videograph_device *p_dev)
{
    struct videograph_entity_node *p_node = NULL;
    int i = 0;
    p_dev->first = videograph_entity_list_count;
    for (i = 0; i < p_dev->max; ++i) {
        p_node = videograph_entity_node_alloc(p_dev->first + i, p_dev->name, p_dev->fops);
        if (unlikely(NULL == p_node)) {
            for (; i > 0; --i) {
                p_node = videograph_entity_list_find(p_dev->first + (i - 1));
                if (p_node) {
                    video_entity_deregister(&p_node->entity);
                    videograph_entity_node_free(&p_node);
                }
            }
            p_dev->first = -1;
            return FSCALER_ERR_INSTALL;
        }
        (void)video_entity_register(&p_node->entity);
    }
    return FSCALER_OK;
}

EXPORT_SYMBOL(videograph_device_register);

int videograph_device_deregister(struct videograph_device *p_dev)
{
    int i = 0;
    struct videograph_entity_node *p_node = NULL;
    if (p_dev->first > 0 && p_dev->first < videograph_entity_list_count) {
        for (i = 0; i < p_dev->max; ++i) {
            p_node = videograph_entity_list_find(p_dev->first + i);
            if (p_node) {
                video_entity_deregister(&p_node->entity);
                videograph_entity_node_free(&p_node);
            }
        }
        p_dev->first = -1;
        return FSCALER_OK;
    }
    return FSCALER_ERR_UNINSTALL;
}

EXPORT_SYMBOL(videograph_device_deregister);
