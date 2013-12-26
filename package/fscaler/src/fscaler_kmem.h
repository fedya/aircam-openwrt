/**
 * @file fscaler_kmem.h
 * @brief Scaler kmem cache routines/macros
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_KMEM_H_
#define _FSCALER_KMEM_H_

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
#define kmem_cache kmem_cache_s
#endif /* LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                sizeof(struct __struct), __alignof__(struct __struct),\
                (__flags), NULL, NULL)
#endif /* LINUX_VERSION_CODE */

#define FSCALER_KMEM_CACHE  KMEM_CACHE
#define FSCALER_KMEM_UNCACHE(__cache) \
({  \
    int __ret = !typecheck(struct kmem_cache *, (__cache));    \
    kmem_cache_destroy((__cache));    \
    __ret;  \
})

#define FSCALER_KMEM_ALLOC(__cache) \
({  \
    void* __ret = NULL;  \
    typecheck(struct kmem_cache *, (__cache));   \
    __ret = kmem_cache_alloc((__cache), (in_interrupt() || in_atomic()) ? GFP_ATOMIC : GFP_KERNEL);   \
    __ret;  \
})

#define FSCALER_KMEM_FREE(__cache, __mem)   \
({  \
    int __ret = !typecheck(struct kmem_cache *, (__cache));  \
    kmem_cache_free((__cache), (void *)(__mem)); __ret;    \
})

#define FSCALER_SALLOC(__struct) \
    (struct __struct*)kmalloc(sizeof(struct __struct), (in_interrupt() || in_atomic()) ? GFP_ATOMIC : GFP_KERNEL)

#define FSCALER_ALLOC(__size, __len) \
({  \
    void * __ret = NULL; \
    if (((__size) *( __len)) > 0) { \
        __ret = (void *)kmalloc((__size)/sizeof(unsigned char)*(__len), (in_interrupt() || in_atomic()) ? GFP_ATOMIC : GFP_KERNEL); \
    } \
    __ret; \
})

#define FSCALER_FREE(__mem) \
    kfree((void *)(__mem))

#endif /* _FSCALER_KMEM_H_ */
