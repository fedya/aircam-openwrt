/*
 *  Copyright (C) 2010 Grain-Media Technology.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/module.h> 
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <mach/ftpmu010.h>

#define  REG_ARRAY_SZ   30  //Max value for register array

/* Local Variables
 */
static u32  fd = 1;
static struct proc_dir_entry *pmu_proc_root = NULL;
static struct proc_dir_entry *attribute_proc = NULL;
static struct proc_dir_entry *regInfo_proc = NULL;

/* Local Functions
 */
static int ftpmu010_proc_init(void);

/* MACROs
 */
#define ATTR_LIST               ftpmu10.attr_list
#define REGINFO_LIST            ftpmu10.reginfo_list    
#define LIST_LOCK               down(&ftpmu10.sema)
#define LIST_UNLOCK             up(&ftpmu10.sema)
#define REGINFO_OFFSET(x,y)     (x)->pmuReg_info.pRegArray[(y)].reg_off
#define REGINFO_BITSMASK(x,y)   (x)->pmuReg_info.pRegArray[(y)].bits_mask
#define REGINFO_LOCKBITS(x,y)   (x)->pmuReg_info.pRegArray[(y)].lock_bits
#define REGINFO_NAME(x)         (x)->pmuReg_info.name
#define REGINFO_REGCNT(x)       (x)->pmuReg_info.num
#define REGINFO_CLKSRC          (x)->pmuReg_info.clock_src

typedef struct
{
    attrInfo_t          attr_info;
    struct list_head    list;
} attrInfo_node_t;

typedef struct
{
    u32                 fd; /* unique number */
    pmuRegInfo_t        pmuReg_info;
    struct list_head    list;
    wait_queue_head_t   wait_queue;     /* wait queue */
    int	                waiting;
} pmuRegInfo_node_t;
    
/* main structure
 */
static struct ftpmu10_s
{
    void __iomem        *base;
    struct semaphore    sema;
    /* attribute list */
    struct list_head    attr_list;
    /* register list */
    struct list_head    reginfo_list;
} ftpmu10;

/* Register a clok node.
 * return value: 0 for success, < 0 for fail.
 */
int ftpmu010_register_attr(attrInfo_t *attr)
{
    attrInfo_node_t *node;    
    int             ret = 0;
    
    /* lock the database */
    LIST_LOCK;
    
    /* walk through whole attribute list */
    list_for_each_entry(node, &ATTR_LIST, list)
    {
        //printk("PMU: %s \n", node->attr_info.name);
        
        if (node->attr_info.attr_type == attr->attr_type)
        {
            ret = -1;
            break;
        }
    }
    /* unlock */
    LIST_UNLOCK;
    
    if (ret < 0)
        return ret;
        
    ret = -1;
    node = kzalloc(sizeof(attrInfo_node_t), GFP_KERNEL);
    if (node)
    {
        memcpy(&node->attr_info, attr, sizeof(attrInfo_t));
        INIT_LIST_HEAD(&node->list);
        list_add_tail(&node->list, &ATTR_LIST);
        try_module_get(THIS_MODULE);
        ret = 0;
    }
    
    return ret;
}

/* DeRegister a clok node.
 * return value: 0 for success, < 0 for fail.
 */
int ftpmu010_deregister_attr(attrInfo_t *attr)
{
    attrInfo_node_t  *node, *ne;
    int              ret = -1;
    
    /* lock */
    LIST_LOCK;
    
    /* walk through whole attribute list */
    list_for_each_entry_safe(node, ne, &ATTR_LIST, list)
    {
        if (node->attr_info.attr_type == attr->attr_type)
        {
            list_del_init(&node->list);
            kfree(node);            
            module_put(THIS_MODULE);
            ret = 0;
            break;
        }
    }
    /* unlock */
    LIST_UNLOCK;
            
    return ret;
}

/* get the content of the attribute
 * return value: 0 for fail, > 0 for success
 */
unsigned int ftpmu010_get_attr(ATTR_TYPE_T  attr_type)
{
    attrInfo_node_t  *node;    
    unsigned int     value = -1;
    
    /* walk through whole attribute list */
    list_for_each_entry(node, &ATTR_LIST, list)
    {
        if (node->attr_info.attr_type == attr_type)
        {
            value = node->attr_info.value;
            break;
        }
    }
    
    return value;
}

/* register/de-register the register table
 * return value:
 *  give an unique fd if return value >= 0, otherwise < 0 if fail.
 */
int ftpmu010_register_reg(pmuRegInfo_t *info)
{
    pmuRegInfo_node_t   *node, *new_node;
    int     i, j, ret = -1;  
    u32     reg_off;
        
    /* sanity check */
    if (unlikely(!strlen(info->name)))
        return -1;
    
    /* check if the register offset is duplicated */
    for (i = 0; i < info->num; i ++)
    {
        reg_off = info->pRegArray[i].reg_off;
        
        for (j = 0; j < info->num; j ++)
        {
            if (i == j) 
                continue;
            if (reg_off == info->pRegArray[j].reg_off)
            {
                printk("PMU: reg_offset 0x%x is duplicated! \n", reg_off);
                return -1;
            }
        }
    }
    
    /* self-test, bits_mask must cover lock_bits and init_val */
    for (i = 0; i < info->num; i ++)
    {
        if ((~info->pRegArray[i].bits_mask) & info->pRegArray[i].lock_bits)
        {
            printk("PMU: %s wrong lock_bits! \n", info->name);
            return -1;
        }                
        if ((~info->pRegArray[i].bits_mask) & info->pRegArray[i].init_val)
        {
            printk("PMU: %s wrong init_val! \n", info->name);
            return -1;
        }        
        if ((~info->pRegArray[i].bits_mask) & info->pRegArray[i].init_mask)
        {
            printk("PMU: %s wrong init_mask! \n", info->name);
            return -1;
        }        
        if ((~info->pRegArray[i].init_mask) & info->pRegArray[i].init_val)
        {
            printk("PMU: %s wrong init_val or init_mask! \n", info->name);
            return -1;
        }
    }
    
    if (info->clock_src != ATTR_TYPE_NONE)
    {
        if (ftpmu010_get_attr(info->clock_src) < 0) 
        {
            printk("PMU: %s registers non-existence clock source! \n", info->name);
            return -1;
        }
    }
    
    /* lock */
    LIST_LOCK;
    
    /* 1. check if duplicate registeration
     */
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        if (!strcmp(REGINFO_NAME(node), info->name))
        {
            printk("PMU: %s was registed already! \n", info->name);
            goto exit;
        }
    }
    
    /* 2. check if the lockbits is overlap. 
     */
    for (i = 0; i < info->num; i ++)
    {
        /* check the registers in each node */        
        list_for_each_entry(node, &REGINFO_LIST, list)
        {
            for (j = 0; j < REGINFO_REGCNT(node); j ++)
            {
                if (info->pRegArray[i].reg_off != REGINFO_OFFSET(node, j))
                    continue;
                    
                if (info->pRegArray[i].lock_bits & REGINFO_LOCKBITS(node, j))
                {
                    printk("PMU: lock_bits of %s is overlapped with %s in offset 0x%x! \n", 
                            info->name, REGINFO_NAME(node), info->pRegArray[i].reg_off);
                    goto exit;
                }
            } /* loop j */
        }
    } /* loop i */
    
    /* 3. sanity check is ok, create new node and chan it to the list
     */
    new_node = kzalloc(sizeof(pmuRegInfo_node_t), GFP_KERNEL);
    if (unlikely(!new_node)) 
    {
        ret = -ENOMEM;
        goto exit;
    }
    new_node->fd = fd++;
    INIT_LIST_HEAD(&new_node->list);
    memcpy(&new_node->pmuReg_info, info, sizeof(pmuRegInfo_t));
    if (info->num >= REG_ARRAY_SZ)
        panic("%s, the array size %d is too small! \n", __func__, REG_ARRAY_SZ);
    new_node->pmuReg_info.pRegArray = kzalloc(REG_ARRAY_SZ * sizeof(pmuReg_t), GFP_KERNEL);
    if (unlikely(!new_node->pmuReg_info.pRegArray))
    {
        kfree(new_node);
        ret = -ENOMEM;
        goto exit;
    }
    
    /* init waitQ */
    init_waitqueue_head(&new_node->wait_queue);
    
    /* copy register array body */
    if (info->num)
        memcpy(new_node->pmuReg_info.pRegArray, info->pRegArray, info->num * sizeof(pmuReg_t));
    
    list_add_tail(&new_node->list, &REGINFO_LIST);
    
    /* unlock */
    LIST_UNLOCK;
    
    ret = new_node->fd;
    try_module_get(THIS_MODULE);
    
    /* update to hardware */
    for (i = 0; i < info->num; i ++)    
        ftpmu010_write_reg(new_node->fd, info->pRegArray[i].reg_off, info->pRegArray[i].init_val, info->pRegArray[i].init_mask);
    
    return ret;
    
exit:
    /* unlock */
    LIST_UNLOCK;
    return ret;
}

int ftpmu010_deregister_reg(int fd)
{
    pmuRegInfo_node_t   *node, *ne;
    int     ret = -1;
    
    /* lock */
    LIST_LOCK;    
    
    list_for_each_entry_safe(node, ne, &REGINFO_LIST, list)
    {
        if (node->fd == fd)
        {
            //printk("PMU: %s is deregistered. \n", node->pmuReg_info.name);
            list_del_init(&node->list);
            if (node->pmuReg_info.pRegArray)
                kfree(node->pmuReg_info.pRegArray);
            kfree(node);
            module_put(THIS_MODULE);
            ret = 0;
            break;
        }
    }

    /* unlock */
    LIST_UNLOCK;
    return ret;
}

int ftpmu010_update_lockbits(int fd, unsigned int reg_off, unsigned int new_lock_bits)
{
    pmuRegInfo_node_t   *node;
    u32     *pLock_bits = NULL;
    int     i, ret = -1;
    
    /* lock */
    LIST_LOCK;
    
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        if (node->fd == fd)
        {
            for (i = 0; i < REGINFO_REGCNT(node); i ++)
            {                
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                
                pLock_bits = &REGINFO_LOCKBITS(node, i);
                
                /* is mask within bits_mask ? */
                if (~REGINFO_BITSMASK(node, i) & new_lock_bits)
                {
                    printk("PMU: %s new 0x%x is out of bits_mask 0x%x!\n", REGINFO_NAME(node), new_lock_bits, REGINFO_BITSMASK(node, i));
                    goto exit;
                }
                ret = 0;
                break;
            } /* for i */
        }
        else
        {
            /* conflict check */
            for (i = 0; i < REGINFO_REGCNT(node); i ++)
            {                
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                
                if (REGINFO_LOCKBITS(node, i) & new_lock_bits)
                {
                    printk("PMU: new 0x%x conflicts with lock_bits 0x%x of %s\n", new_lock_bits,
                            REGINFO_LOCKBITS(node, i), REGINFO_NAME(node));                    
                    ret = -1;
                    goto exit;
                }
                break;
            }
        }
    }    
    
    if (!ret)    
        *pLock_bits = new_lock_bits;
exit:    
    /* unlock */
    LIST_UNLOCK;
    return ret;
}

/* @int ftpmu010_bits_is_locked(int fd, int reg_off, unsigned int bits)
 * @Purpose: This function is used to check if the bits are locked by any module or not.
 * @Parameter:
 *   reg_off: register offset
 *   bits: the checked bits  
 * @Return: 
 *      If the any bit in bits is locked, then the returned value will be 0
 *      otherwise, -1 is returned to indicates all bits are available.
 *
 */
int ftpmu010_bits_is_locked(int fd, unsigned int reg_off, unsigned int bits)
{
    pmuRegInfo_node_t   *node;    
    int     i, ret = -1;
    
    /* lock */
    LIST_LOCK;
    
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        if (node->fd == fd)
            continue;
            
        /* conflict check */
        for (i = 0; i < REGINFO_REGCNT(node); i ++)
        {                
            if (REGINFO_OFFSET(node, i) != reg_off)
                continue;
            
            if (REGINFO_LOCKBITS(node, i) & bits)
            {
                ret = 0;
                goto exit;
            }
            break;
        }
    } 
    
exit:    
    /* unlock */
    LIST_UNLOCK;
    return ret;
}

/* PMU register read/write
 */
unsigned int ftpmu010_read_reg(unsigned int reg_off)
{
    return ioread32(ftpmu10.base + reg_off);
}

/* return value < 0 for fail */
int ftpmu010_write_reg(int fd, unsigned int reg_off, unsigned int val, unsigned int mask)
{
    pmuRegInfo_node_t   *node;    
    int                 i, ret = -1;
        
    /* sanity check */
    if (unlikely(val & (~mask))) {
        printk("%s: wrong mask:%#x or value:%#x in offset:%#x! \n", __func__, mask, val, reg_off);
        goto exit;
    }
    
    if (unlikely(!mask)) {
        ret = 0;
	    goto exit;	/* do nothing */
    }
    
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        if (node->fd == fd)
        {
            ret = -1;
            /* check if reg_off had been registered already */
            for (i = 0; i < REGINFO_REGCNT(node); i ++)
            {
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                
                /* is mask within bits_mask ? */
                if (~REGINFO_BITSMASK(node, i) & mask)
                {
                    printk("PMU: %s writes with wrong mask 0x%x in off:%#x! \n", 
                        REGINFO_NAME(node), mask, reg_off);
                    ret = -1;
                    goto exit;
                }
                ret = 0;
            }
        }
        else
        {
            /* check if the bits is on lock_bits of others */
            for (i = 0; i < REGINFO_REGCNT(node); i ++)
            {
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                
                /* is lock_bits conflicts with the input mask ? */
                if (REGINFO_LOCKBITS(node, i) & mask)
                {
                    printk("PMU: Wrong mask 0x%x conflicts with %s in off:%#x! \n", mask, 
                            REGINFO_NAME(node), reg_off);
                    ret = -1;
                    goto exit;
                }
                break;
            }
        }
    }
    
    if (!ret)
    {
        u32     tmp;
        
        tmp = (ftpmu010_read_reg(reg_off) & (~mask));
        tmp |= (val & mask);
        iowrite32(tmp, ftpmu10.base + reg_off);
    }
    
exit:
    if (ret < 0) {
        dump_stack();
        panic("Configure PMU fail! \n");
    }
        
    return ret;                
}

/* PMU init function
 * Input parameters: virtual address of PMU
 * Return: 0 for success, < 0 for fail
 */
int ftpmu010_init(void __iomem  *base)
{
    ftpmu10.base = base;
    init_MUTEX(&ftpmu10.sema);
    INIT_LIST_HEAD(&ftpmu10.attr_list);
    INIT_LIST_HEAD(&ftpmu10.reginfo_list);
    /* proc function */
    ftpmu010_proc_init();
    return 0;
}

/* Purpose: calculate the divisor by input clock
 * Input: fd, in_clock, near
 * Output: None
 * Return: quotient if > 0, 0 for fail
 * Note: The return value will take the nearest value if near is 1. For example: 17.6 will be treated as 18, 
 *          but 17.4 will be treated as 17.
 */
unsigned int ftpmu010_clock_divisor(int fd, unsigned int in_clock, int near)
{
    pmuRegInfo_node_t   *node;
    unsigned int        clock = 0;
    
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        if (node->fd != fd)
            continue;
        
        clock = ftpmu010_get_attr(node->pmuReg_info.clock_src);
        if (clock == (u32)-1)
            return 0;
        
        if (near)
            clock += (in_clock >> 1);
        /* n = n / base; return rem; */
        do_div(clock, in_clock);
        break;
    }
    
    return clock;
}

/* @Purpose: request the pmu PIN
 * @Parameter:
 *      fd: unique identifier
 *      reg_off: register offset
 *      req_bits: request registers
 *      b_wait: 1 for blocking until the resource is available
 * Output: None
 * Return: 0 for success, !0 for fail
 */
int ftpmu010_request_pins(int fd, unsigned int reg_off, unsigned int req_bits, int b_wait)
{
    pmuRegInfo_node_t   *node;
    int     i, ofs, is_new, ret = 0;
    
    if (b_wait) {
        if (in_interrupt() || in_atomic())
            panic("%s, wrong context in interrupt or atomic \n", __func__);
    }
    
    if (ftpmu010_bits_is_locked(fd, reg_off, req_bits) == -1) { /* the bits are available */
        /* lock */
        LIST_LOCK;

        list_for_each_entry(node, &REGINFO_LIST, list) {
            if (node->fd != fd)
                continue;
            
            /* check if the offset exists ? */
            is_new = 1;
            for (i = 0; i < REGINFO_REGCNT(node); i ++) {
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                    
                REGINFO_BITSMASK(node, i) |= req_bits;
                REGINFO_LOCKBITS(node, i) |= req_bits;
                is_new = 0; /* this is not a new offset */
                break;
            }
            
            if (is_new) { /* new node */
                ofs = REGINFO_REGCNT(node);
                if (ofs >= REG_ARRAY_SZ)
                    panic("%s, the array size %d is too small! \n", __func__, REG_ARRAY_SZ);

                REGINFO_OFFSET(node, ofs) = reg_off;
                REGINFO_BITSMASK(node, ofs) = req_bits;
                REGINFO_LOCKBITS(node, ofs) = req_bits;
                
                REGINFO_REGCNT(node) ++;
            }
            break;
        }
        
        /* unlock */
        LIST_UNLOCK;
    } else {
        LIST_LOCK;
               
        /* the bits are locked by others */
        if (!b_wait) {
            ret = -1;
            LIST_UNLOCK;
            goto exit;
        }
        
        ret = -1;
        list_for_each_entry(node, &REGINFO_LIST, list) {
            if (node->fd != fd)
                continue;
            ret = 0;
            break;
        }
        
        if (ret == -1)
            panic("%s, can't find fd in pmu module! \n", __func__);
        
        node->waiting = 0;
        
        LIST_UNLOCK;
        
keep_wait:
        wait_event_interruptible(node->wait_queue, node->waiting);
        if (signal_pending(current))
			return -ERESTARTSYS;
        node->waiting = 0;
        
        /* bits are freed */
        if (ftpmu010_bits_is_locked(fd, reg_off, req_bits) == -1) {
            LIST_LOCK;
            
            ret = -1;
            /* find the existed offset */
            for (i = 0; i < REGINFO_REGCNT(node); i ++) {
                if (REGINFO_OFFSET(node, i) != reg_off)
                    continue;
                    
                REGINFO_BITSMASK(node, i) |= req_bits;
                REGINFO_LOCKBITS(node, i) |= req_bits;
                ret = 0;
                break;
            }
            
            if (ret == -1) {
                /* new offset */
                ofs = REGINFO_REGCNT(node);
                if (ofs >= REG_ARRAY_SZ)
                    panic("%s, the array size %d is too small! \n", __func__, REG_ARRAY_SZ);
                REGINFO_OFFSET(node, ofs) = reg_off;
                REGINFO_BITSMASK(node, ofs) = req_bits;
                REGINFO_LOCKBITS(node, ofs) = req_bits;
                REGINFO_REGCNT(node) ++;
                ret = 0;
            }
            
            LIST_UNLOCK;
        } else {
            goto keep_wait;
        }
    }

exit:
    return ret;
}

/* Purpose: release the pmu PIN
 * Input: fd, reg_off, req_bits
 * Output: None
 * Return: 0 for success, !0 for fail
 */
int ftpmu010_release_pins(int fd, unsigned int reg_off, unsigned int req_bits)
{
    pmuRegInfo_node_t   *node;
    int     i, ret = -1;
    
    /* lock */
    LIST_LOCK;
    
    list_for_each_entry(node, &REGINFO_LIST, list) {
        if (node->fd != fd)
            continue;
                
        for (i = 0; i < REGINFO_REGCNT(node); i ++) {
            if (REGINFO_OFFSET(node, i) != reg_off)
                continue;
            
            if (REGINFO_LOCKBITS(node, i) & req_bits) {
                REGINFO_BITSMASK(node, i) &= ~req_bits;
                REGINFO_LOCKBITS(node, i) &= ~req_bits;
                ret = 0;
            }
            break;
        }
        
        break;
    }
    
    /* wake up the ones who are waiting for the pins */
    list_for_each_entry(node, &REGINFO_LIST, list) {
        if (node->fd == fd)
            continue;
        
        node->waiting = 1;
        wake_up(&node->wait_queue);
    }
    
    /* unlock */
    LIST_UNLOCK;
    
    return ret;
}

/* Purpose: check if the PINs was requested by others except myself.
 * Input: fd, reg_off, req_bits
 * Output: None
 * Return: those pins occupied by others. zero indicates the pin are available.
 */
unsigned int ftpmu010_pins_is_requested(int fd, unsigned int reg_off, unsigned int req_bits)
{
    pmuRegInfo_node_t   *node;
    int     i;
    
    /* lock */
    LIST_LOCK;
    
    list_for_each_entry(node, &REGINFO_LIST, list) {
        if (node->fd == fd)
            continue;
        
        for (i = 0; i < REGINFO_REGCNT(node); i ++) {
            if (REGINFO_OFFSET(node, i) != reg_off)
                continue;
            
            req_bits &= REGINFO_LOCKBITS(node, i);
            break;
        }
    }
    
    /* unlock */
    LIST_UNLOCK;
    
    return req_bits;
}

/* ------------------------------------------------------------------------------------
 *  Proc function
 * ------------------------------------------------------------------------------------
 */
 
 /* Attribute info
  */
static int proc_read_attribute_info(char *page, char **start, off_t off, int count, int *eof, void *data)
{    
    attrInfo_node_t *node; 
    int     len = 0;
     
    /* lock the database */
    LIST_LOCK;
    
    /* walk through whole attribute list */
    list_for_each_entry(node, &ATTR_LIST, list)
    {
        len += sprintf(page+len, "Attribute name : %s \n", node->attr_info.name);
        len += sprintf(page+len, "Attribute type : %d \n", node->attr_info.attr_type);
        len += sprintf(page+len, "Attribute value: %d \n\n", node->attr_info.value);
    }
    /* unlock */
    LIST_UNLOCK;
    
    return len;
}

/* Register info
 */ 
static int proc_read_reginfo(char *page, char **start, off_t off, int count, int *eof, void *data)
{    
    pmuRegInfo_node_t *node;     
    int     i, len = 0;
     
    /* lock the database */
    LIST_LOCK;
    
    /* walk through whole attribute list */
    list_for_each_entry(node, &REGINFO_LIST, list)
    {
        len += sprintf(page+len, "name: %s, fd = %d, attribute type = %d \n", REGINFO_NAME(node), node->fd, node->pmuReg_info.clock_src);
        for (i = 0; i < REGINFO_REGCNT(node); i ++)
        {
            len += sprintf(page+len,"    reg_off: 0x%x, bits_mask: 0x%x, lock_bits: 0x%x\n", 
                        REGINFO_OFFSET(node, i), REGINFO_BITSMASK(node, i), REGINFO_LOCKBITS(node, i));
        }
        len += sprintf(page+len, "\n");
    }
    /* unlock */
    LIST_UNLOCK;
    
    return len;
}

static int ftpmu010_proc_init(void)
{	
    int     ret = 0;    
    struct proc_dir_entry   *p;
    
	p = create_proc_entry("pmu", S_IFDIR | S_IRUGO | S_IXUGO, NULL);
	if (p == NULL) {
		return -ENOMEM;
	}
	p->owner = THIS_MODULE;
	pmu_proc_root = p;
	
	/*
     * attribute
     */
	attribute_proc = create_proc_entry("attribute", S_IRUGO, pmu_proc_root);
	if (attribute_proc == NULL) {
		printk("PMU: Fail to create proc attribute!\n");
		remove_proc_entry(pmu_proc_root->name, NULL);
		ret = -EINVAL;
		goto end;
	}
	attribute_proc->read_proc = (read_proc_t *) proc_read_attribute_info;
	attribute_proc->write_proc = NULL;
	attribute_proc->owner = THIS_MODULE;
    
    /*
     * regInfo
     */
    regInfo_proc = create_proc_entry("reginfo", S_IRUGO, pmu_proc_root);
	if (regInfo_proc == NULL) {
		printk("PMU: Fail to create proc regInfo!\n");
		remove_proc_entry(attribute_proc->name, pmu_proc_root);
		remove_proc_entry(pmu_proc_root->name, NULL);
		ret = -EINVAL;
		goto end;
	}
	regInfo_proc->read_proc = (read_proc_t *) proc_read_reginfo;
	regInfo_proc->write_proc = NULL;
	regInfo_proc->owner = THIS_MODULE;
end:	
	return ret;
}

EXPORT_SYMBOL(ftpmu010_register_attr);
EXPORT_SYMBOL(ftpmu010_deregister_attr);
EXPORT_SYMBOL(ftpmu010_get_attr);
EXPORT_SYMBOL(ftpmu010_register_reg);
EXPORT_SYMBOL(ftpmu010_deregister_reg);
//EXPORT_SYMBOL(ftpmu010_add_lockbits); //please call request_pins
//EXPORT_SYMBOL(ftpmu010_del_lockbits);
EXPORT_SYMBOL(ftpmu010_bits_is_locked);
EXPORT_SYMBOL(ftpmu010_update_lockbits);
EXPORT_SYMBOL(ftpmu010_read_reg);
EXPORT_SYMBOL(ftpmu010_write_reg);
EXPORT_SYMBOL(ftpmu010_clock_divisor);
EXPORT_SYMBOL(ftpmu010_request_pins);
EXPORT_SYMBOL(ftpmu010_release_pins);
EXPORT_SYMBOL(ftpmu010_pins_is_requested);

MODULE_AUTHOR("Grain-Media");
MODULE_DESCRIPTION("FTPMU010 core");
MODULE_LICENSE("GPL");


