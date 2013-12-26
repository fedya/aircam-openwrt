/**
 * @file fscaler_list.h
 * @brief Scaler macro header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Nov 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_LIST_H_
#define _FSCALER_LIST_H_

#include <linux/klist.h>

#define klist_entry(ptr,type,member) \
({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *) (__mptr ? ((char *)__mptr - offsetof(type,member)) : NULL); \
})

static inline struct klist_node *klist_find(struct klist *k, struct klist_node *start, void *data,
                                            int (*match) (struct klist_node *, void *))
{
    struct klist_iter i;
    struct klist_node *pos = NULL;
    if (k) {
        klist_iter_init_node(k, &i, start);
        pos = klist_next(&i);
        do {
            if (!pos) {
                break;
            }
            if (match(pos, data)) {
                break;
            }
            pos = klist_next(&i);
        } while (1);
        klist_iter_exit(&i);
    }
    return pos;
}

static inline int klist_for_each(struct klist *k, struct klist_node *start, void *data,
                                 int (*fn) (struct klist_node *, void *))
{
    int hr = 0;
    struct klist_iter i;
    struct klist_node *pos = NULL;
    if (k) {
        klist_iter_init_node(k, &i, start);
        pos = klist_next(&i);
        do {
            if (!pos) {
                break;
            }
            hr = fn(pos, data);
            if (0 > hr) {
                break;
            }
            pos = klist_next(&i);
        } while (1);
        klist_iter_exit(&i);
    }
    return hr;
}

static inline struct klist_node *klist_front(struct klist *k)
{
    struct klist_iter i;
    struct klist_node *pos = NULL;
    if (k) {
        klist_iter_init(k, &i);
        pos = klist_next(&i);
        klist_iter_exit(&i);
    }
    //printk("SCL: klist_front(): pos:%p\n", pos);
    return pos;
}

static inline struct klist_node *klist_pop(struct klist *k, int wait)
{
    struct klist_node *pos = klist_front(k);
    if (pos) {
        wait ? klist_remove(pos) : klist_del(pos);
        return pos;
    }
    return NULL;
}

static inline int klist_empty(struct klist *k)
{
    return (!klist_front(k)) ? 1 : 0;
}

#endif /* _FSCALER_LIST_H_ */
