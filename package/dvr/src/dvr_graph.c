#define __DVR_GRAPH_C__

#include <linux/module.h>
//#include <linux/config.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/time.h>
#include <linux/miscdevice.h>
#include <linux/kallsyms.h>
#include <linux/workqueue.h>
#include <linux/vmalloc.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "videograph_api.h"
#include "dvr_graph.h"
#include "dvr_type_define.h"
#include "util_cfg.h"
#include "dvr_common_api.h"

struct proc_dir_entry *DvrProcRoot = NULL;
static struct proc_dir_entry *proc_cmn_dbg;
static struct proc_dir_entry *proc_last_api;
static int dvr_common_dbgmode = 0;
static int last_api_num = 0;
static int flow_rate_flag = 0;
static int dvr_common_open_cnt = 0;

EXPORT_SYMBOL(DvrProcRoot);

#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_common_dbgmode) printk(fmt, ## args);}


static struct semaphore dvr_graph_oper_sem;
 static struct semaphore dvr_graph_suspend_sem;

int dvr_graph_is_initialize = 0;
struct list_head sys_all_graph;
struct list_head sys_all_queue;
struct list_head sys_all_vgfn;

static LIST_HEAD(swap_notify_list);
static DECLARE_MUTEX(swap_notify_lock);

void dvr_active_nodirty(int fd);
void dvr_remove_nodirty(int fd);
void apply_nodirty_all_list(void);

void register_swap_notify(dvr_swap_notify_info *pSwapNotify)
{
    down(&swap_notify_lock);
    list_add_tail(&pSwapNotify->list, &swap_notify_list);
    up(&swap_notify_lock);
}
EXPORT_SYMBOL(register_swap_notify);

void unregister_swap_notify(dvr_swap_notify_info *pSwapNotify)
{
    down(&swap_notify_lock);
    list_del(&pSwapNotify->list);
    up(&swap_notify_lock);
}
EXPORT_SYMBOL(unregister_swap_notify);

static void swap_notify(void)
{
    dvr_swap_notify_info *pSwapNotify;
    list_for_each_entry(pSwapNotify, &swap_notify_list, list){
        if(pSwapNotify->callback) {
            pSwapNotify->callback(pSwapNotify->private);
        }
    }
}


int dvr_graph_count=0;
int dvr_graph_create(dvr_graph **graph, int graph_id)
{
    dvr_graph *t_graph;

    t_graph = kzalloc(sizeof(dvr_graph), GFP_KERNEL);
    if(!t_graph)
        return -EFAULT;

    snprintf(t_graph->linecache_str, 16, "dlc_%d", ++dvr_graph_count);
    t_graph->line_data_cache = kmem_cache_create(t_graph->linecache_str, sizeof(dvr_graph_line), 0, 0, NULL);
    if(!t_graph->line_data_cache)
    {
        kfree(t_graph);
        return -EFAULT;
    }

    snprintf(t_graph->propcache_str, 16, "dpc_%d", dvr_graph_count);
    t_graph->prop_data_cache = kmem_cache_create(t_graph->propcache_str, sizeof(dvr_graph_prop), 0, 0, NULL);
    if(!t_graph->prop_data_cache)
    {
        kmem_cache_destroy(t_graph->line_data_cache);
        kfree(t_graph);
        return -EFAULT;
    }
    INIT_LIST_HEAD(&t_graph->line_entry);
    INIT_LIST_HEAD(&t_graph->prop_entry);

    t_graph->id = graph_id;
    *graph = t_graph;

    INIT_LIST_HEAD(&t_graph->list);
    list_add_tail(&t_graph->list, &sys_all_graph);


    return 0;
}

int dvr_graph_destroy(dvr_graph *graph)
{
    dvr_graph_line *graph_line, *tmp_line;
    dvr_graph_prop *graph_prop, *tmp_prop;

    list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
    {
        list_del(&graph_line->list);
        kmem_cache_free(graph->line_data_cache, graph_line);
    }

    list_for_each_entry_safe(graph_prop, tmp_prop, &graph->prop_entry, list)
    {
        list_del(&graph_prop->list);
        kmem_cache_free(graph->prop_data_cache, graph_prop);
    }

    if(graph->line_data_cache)
        kmem_cache_destroy(graph->line_data_cache);
    if(graph->prop_data_cache)
        kmem_cache_destroy(graph->prop_data_cache);

    // remove myself from system graph list
    list_del(&graph->list);

    if(graph)
        kfree(graph);

    return 0;
}

void dvr_graph_set_flow_flag(void)
{
    flow_rate_flag = 1;
}

void dvr_graph_apply_flow_list(void)
{
    dvr_graph *one_graph, *tmp_one_graph;
    dvr_graph_line *graph_line, *tmp_line;
    int src_fd, src_flow, dst_fd, dst_flow, need_update=0;
    
    VDBGPRINT(DBG_GRAPH_DATA, "%s\n", __FUNCTION__);

    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {
        list_for_each_entry_safe(graph_line, tmp_line, &one_graph->line_entry, list)
        {
            if(graph_line->data.state != GFL_DONE) {
                continue;
            }
            src_fd = graph_line->data.src_fd;
            src_flow = graph_line->data.src_flow;
            dst_fd = graph_line->data.dst_fd;
            dst_flow = graph_line->data.dst_flow;
            video_update_flow(src_fd,src_flow,dst_fd,dst_flow);
            need_update=1;
        }
    }
    if(need_update == 1)
        video_apply_flow();
}

void dvr_graph_apply_flow(void)
{
    VDBGPRINT(DBG_GRAPH_DATA, "video_apply_flow\n");

    down(&dvr_graph_suspend_sem);
    video_apply_flow();
    up(&dvr_graph_suspend_sem);
}


int dvr_get_flow(int fd)
{
    return video_get_flow(fd);
}

int dvr_graph_update_flow_list(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow)
{
    dvr_graph_line *graph_line;

    list_for_each_entry(graph_line,&graph->line_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_line->data.tag, tag)||graph_line->data.gfd==0)
            continue;

        if(graph_line->data.src_fd == src_fd && graph_line->data.dst_fd == dst_fd)
        {
            graph_line->data.src_flow = src_flow;
            graph_line->data.dst_flow = dst_flow;
            break;
        }
    }
    VDBGPRINT(DBG_GRAPH_DATA, "%s(0x%x,%d,%x,%d)\n", __FUNCTION__, src_fd, src_flow, dst_fd, dst_flow);
    return 0;
}

int dvr_graph_update_flow(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow)
{
    dvr_graph_line *graph_line;

    list_for_each_entry(graph_line,&graph->line_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_line->data.tag, tag))
            continue;

        if(graph_line->data.src_fd == src_fd && graph_line->data.dst_fd == dst_fd)
        {
            graph_line->data.src_flow = src_flow;
            graph_line->data.dst_flow = dst_flow;
            break;
        }
    }
    VDBGPRINT(DBG_GRAPH_DATA, "%s(0x%x,%d,%x,%d)\n", __FUNCTION__, src_fd, src_flow, dst_fd, dst_flow);
    
    if(graph_line->data.state != GFL_DONE) {
        return 0;
    }

    down(&dvr_graph_suspend_sem);

    down(&dvr_graph_oper_sem);
    video_update_flow(src_fd,src_flow,dst_fd,dst_flow);
    up(&dvr_graph_oper_sem);

    up(&dvr_graph_suspend_sem);
    
    return 0;
}


int dvr_register_postprocess(int entity_fd,int (*func)(void *,void *))
{
    return video_register_postprocess(entity_fd,func);
}


int dvr_register_preprocess(int entity_fd,int (*func)(void *,void *))
{
    return video_register_preprocess(entity_fd,func);
}
extern void print_graph_all(int function,int debug);
int dvr_graph_add_link(dvr_graph *graph, FuncTag *tag, int src_fd, int src_flow, int dst_fd, int dst_flow, struct v_queue_t *que)
{
    int found=FALSE;
    dvr_graph_line *graph_line = NULL;
    
    if(!graph && !tag && !src_fd && !src_flow && !dst_fd && !dst_flow && !que) 
        return -EFAULT;
    down(&dvr_graph_oper_sem);
    // find the link in the current list
    if(!list_empty(&graph->line_entry)) {        
        list_for_each_entry(graph_line, &graph->line_entry, list) {            
            if(graph_line->data.src_fd==src_fd && graph_line->data.dst_fd==dst_fd) {                
                found = TRUE;                
                break;            
            }        
        }    
    }
    // add link to list if it is not existed
    if(!found)
    {
        graph_line = kmem_cache_alloc(graph->line_data_cache, GFP_KERNEL);
        INIT_LIST_HEAD(&graph_line->list);
        
        FN_COPY_TAG(&graph_line->data.tag, tag);
        graph_line->data.state = GFL_NEW;
        graph_line->data.src_fd = src_fd;
        graph_line->data.src_flow = src_flow;
        graph_line->data.dst_fd = dst_fd;
        graph_line->data.dst_flow = dst_flow;
        graph_line->data.que = que;
        graph_line->data.cnt = 1;
        list_add_tail(&graph_line->list, &graph->line_entry);        
    } else {
        if( (graph_line->data.state == GFL_DEL) || 
            (graph_line->data.state == GFL_ONLY_DEL_TAG) ||
            (graph_line->data.state == GFL_DONE)) {//Note different with sdk1.1
            graph_line->data.state = GFL_UPDATE;
        }
        FN_SET_FUNC(&graph_line->data.tag, tag->func);
        graph_line->data.src_flow = src_flow;
        graph_line->data.dst_flow = dst_flow;
        graph_line->data.cnt++;
        /* Stanley add for Liveview PIP func. record every channel that will use the same link, 
           so "OR" channel number to tag->lv_ch */    
        graph_line->data.tag.lv_ch |= tag->lv_ch;       
    }
    up(&dvr_graph_oper_sem);

    return 0;
}


int dvr_graph_del_link(dvr_graph *graph, FuncTag *tag)
{
    int n=0;
    dvr_graph_line *graph_line, *tmp_line;
    if(!graph)
        return -1;
    
    down(&dvr_graph_oper_sem);

    if(FN_IS_FN(tag, FN_METHOD_USE_CMP))
    {
        list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
        {            
            if(!FN_COMPARE(&graph_line->data.tag, tag))
                continue;

            if(graph_line->data.state == GFL_NEW)
                continue;
            
            if(FN_IS_UPDATE(tag))
                graph_line->data.state = GFL_DEL_AND_RESTART;
            else
                graph_line->data.state = GFL_DEL;
            n++;
        }
    }
    else
    {
        list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
        {            
            if(!FN_CHECK_MASK(&graph_line->data.tag, tag))             
                continue;            

            if(graph_line->data.state == GFL_NEW)                
                continue;            
                     
            if((--graph_line->data.cnt)==0) {
                if(FN_IS_UPDATE(tag)) {
                    graph_line->data.state = GFL_DEL_AND_RESTART;
                } else {
                    graph_line->data.state = GFL_DEL;
                }
            } else {
	      		
            	        FN_REMOVE_FUNC(&graph_line->data.tag, tag->func);
			graph_line->data.state = GFL_ONLY_DEL_TAG;
            }            
            n++;
        }
			
    }
    up(&dvr_graph_oper_sem);
    return n;
}

int dvr_graph_set_property_directly(int fd, unsigned int id, unsigned int value, int direct)
{
    int ret; 

    ret = set_property(fd, id, value, direct);
    if(ret<0) 
        panic("[%s] set property 0x%x,id=%d,val=%d,direct=%d failed.\n",__FUNCTION__, fd, id, value, direct);

    return ret;        
}

int dvr_graph_set_property(dvr_graph *graph, FuncTag *tag, int fd, unsigned int id, unsigned int value, int direct)
{
    dvr_graph_prop *graph_prop;

    if(!graph)
        return -EFAULT;

    graph_prop = kmem_cache_alloc(graph->prop_data_cache, GFP_KERNEL);
    INIT_LIST_HEAD(&graph_prop->list);

    //graph_prop->data.tag = tag;
    FN_COPY_TAG(&graph_prop->data.tag, tag);
    //graph_prop->data.flag = flag;
    graph_prop->data.fd = fd;
    graph_prop->data.id = id;
    graph_prop->data.value = value;
    graph_prop->data.direct = direct;
  down(&dvr_graph_oper_sem);
    list_add_tail(&graph_prop->list, &graph->prop_entry);
  up(&dvr_graph_oper_sem);


    return 0;

}


unsigned int dvr_graph_get_property(dvr_graph *graph, int fd, unsigned int id, int direct)
{
    int ret;
    ret = get_property(fd, id, direct);
    return ret;
}


int dvr_graph_write_property(dvr_graph *graph, FuncTag *tag_mask)
{
    int ret=0;
    dvr_graph_prop *graph_prop, *tmp_prop;

    list_for_each_entry_safe(graph_prop, tmp_prop, &graph->prop_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_prop->data.tag, tag_mask))
            continue;

        ret = set_property(graph_prop->data.fd, graph_prop->data.id, graph_prop->data.value, graph_prop->data.direct);
        VDBGPRINT(DBG_GRAPH_DATA, "set_property(0x%x,%d,%d,%d)=ret(%d)\n", graph_prop->data.fd, graph_prop->data.id, graph_prop->data.value, graph_prop->data.direct, ret);
        if(ret<0)
        {
            printk("[dvr_graph] set property 0x%x,id=%d,val=%d,direct=%d failed.\n", graph_prop->data.fd, graph_prop->data.id, graph_prop->data.value, graph_prop->data.direct);
            //break;
        }

        list_del(&graph_prop->list);
        kmem_cache_free(graph->prop_data_cache, graph_prop);
    }

    return ret;
}

int dvr_graph_run_vg_fnc(FuncTag *tag_mask, int b_pre_run)
{
    int ret=0;
    dvr_vgfn *one_vgfn, *tmp_one_vgfn;


    if(b_pre_run)
    {
        list_for_each_entry_safe(one_vgfn, tmp_one_vgfn, &sys_all_vgfn, list)
        {
            if(one_vgfn->fn_type == VGFN_REARRANGE_QUEUE)
            {
                if(!FN_CHECK_MASK(&one_vgfn->data.rearrange_queue.tag, tag_mask))
                    continue;
                if(one_vgfn->data.rearrange_queue.q==0)
                    continue;
                    
                ret = video_rearrange_queue(one_vgfn->data.rearrange_queue.q,one_vgfn->data.rearrange_queue.num,one_vgfn->data.rearrange_queue.elem_size);
                VDBGPRINT(DBG_GRAPH_DATA, "DVR video_rearrange_queue(0x%x,%d,%d)=ret(%d)\n", (int)one_vgfn->data.rearrange_queue.q, one_vgfn->data.rearrange_queue.num, one_vgfn->data.rearrange_queue.elem_size, ret);
                if(ret<0)
                    return -1;
                else
                {
                    list_del(&one_vgfn->list);
                    kfree(one_vgfn);
                }
            }
        }
    }
    else
    {
        list_for_each_entry_safe(one_vgfn, tmp_one_vgfn, &sys_all_vgfn, list)
        {
            if(one_vgfn->fn_type == VGFN_VG_SETFREE)
            {
                if(!FN_CHECK_MASK(&one_vgfn->data.set_vg_freetype.tag, tag_mask))
                    continue;

                ret = video_setfree(one_vgfn->data.set_vg_freetype.fd,one_vgfn->data.set_vg_freetype.freeway,one_vgfn->data.set_vg_freetype.param);
                VDBGPRINT(DBG_GRAPH_DATA, "DVR video_setfree(0x%x,%d,%d)=ret(%d)\n", one_vgfn->data.set_vg_freetype.fd, one_vgfn->data.set_vg_freetype.freeway, one_vgfn->data.set_vg_freetype.param, ret);
                if(ret<0)
                    return -1;
                else
                {
                    list_del(&one_vgfn->list);
                    kfree(one_vgfn);
                }
            }
        }
    }

    return ret;
}




int dvr_graph_write_graphline(dvr_graph *graph, FuncTag *tag_mask)
{
    int ret=0;
    dvr_graph_line *graph_line, *tmp_line, *graph_line2;
    list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_line->data.tag, tag_mask)&&graph_line->data.state != GFL_ONLY_DEL_TAG)
            continue;
        if(graph_line->data.state == GFL_UPDATE)
        {
            ret = video_cleargraph(graph_line->data.gfd);
            VDBGPRINT(DBG_GRAPH_DATA, "DVR video_cleargraph(%d)=ret(%d)\n", graph_line->data.gfd, ret);
            if(ret<0)
            {
                ret = -1;
                printk("[dvr_graph:GFL_UPDATE] clear graph(gfd:%d) failed.\n", graph_line->data.gfd);
            }
            graph_line->data.state = GFL_NEW;
        }
	    if(graph_line->data.state == GFL_ONLY_DEL_TAG) 
        {
            /* Stanley add for Liveview PIP func. because dvr_graph_add_link will "OR" channel number to tag.lv_ch, 
               so here we have to remove channel number */
            if((tag_mask->func == FN_LIVEVIEW) || (tag_mask->func == FN_PLAYBACK)) 
                FN_REMOVE_LV_CH(&graph_line->data.tag, tag_mask->lv_ch);                          
          //  else    
             //   FN_REMOVE_FUNC(&graph_line->data.tag, tag_mask->func);
            graph_line->data.state = GFL_DONE;
        }

        if(graph_line->data.state == GFL_DEL)
        {
            int is_same=0;

            ret = video_cleargraph(graph_line->data.gfd);
            VDBGPRINT(DBG_GRAPH_DATA, "DVR video_cleargraph(%d)=ret(%d)\n", graph_line->data.gfd, ret);
            if(ret<0)
            {
                ret = -1;
                printk("[dvr_graph:GFL_DEL] clear graph(gfd:%d) failed.\n", graph_line->data.gfd);
            }
            list_del(&graph_line->list);
            kmem_cache_free(graph->line_data_cache, graph_line);

            list_for_each_entry(graph_line2, &graph->line_entry, list)
            {
                if(!FN_CHECK_MASK(&graph_line2->data.tag, tag_mask))
                    continue;

                if(graph_line2->data.state == GFL_NEW)
                {
                    if((graph_line2->data.src_fd==graph_line->data.src_fd)&&
                       (graph_line2->data.dst_fd==graph_line->data.dst_fd)) 
                    {
                        is_same=1;
                        break;
                    }
                }
            }

            if(is_same==0)
                dvr_remove_nodirty(graph_line->data.src_fd);
        }
    }

    list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_line->data.tag, tag_mask))
            continue;

        if(graph_line->data.state == GFL_NEW)
        {
            graph_line->data.gfd = video_setgraph(graph_line->data.src_fd, graph_line->data.src_flow, graph_line->data.dst_fd, graph_line->data.dst_flow, graph_line->data.que);
            VDBGPRINT(DBG_GRAPH_DATA, "DVR video_setgraph(0x%x,%d,0x%x,%d,0x%x)=ret(%d)\n", graph_line->data.src_fd, graph_line->data.src_flow, graph_line->data.dst_fd, graph_line->data.dst_flow, (int)graph_line->data.que, graph_line->data.gfd);
            if(graph_line->data.gfd==-1)
            {
                ret = -1;
                printk("[dvr_graph] set graph(0x%x,0x%x,0x%x) failed.\n", graph_line->data.src_fd, graph_line->data.dst_fd, (int)graph_line->data.que);
//                panic("[dvr_graph] video_setgraph failed. check code flow.\n");
            }            
            graph_line->data.state = GFL_DONE;
            dvr_active_nodirty(graph_line->data.src_fd);
        }
    }

    return ret;
}


int dvr_graph_try_update_and_restart(dvr_graph *graph, FuncTag *tag_mask)
{
    int ret=0, cnt=0;
    dvr_graph_line *graph_line, *tmp_line;

    list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
    {
        if(!FN_CHECK_MASK(&graph_line->data.tag, tag_mask))
            continue;

        if(graph_line->data.state == GFL_DEL_AND_RESTART)
        {
            ret = video_cleargraph(graph_line->data.gfd);
            VDBGPRINT(DBG_GRAPH_DATA, "DVR video_cleargraph2(%d)=ret(%d)\n", graph_line->data.gfd, ret);
            if(ret<0)
            {
                return (-1);
                printk("[dvr_graph] clear graph(gfd:%d) failed.\n", graph_line->data.gfd);
                break;
            }
            cnt++;
            list_del(&graph_line->list);
            kmem_cache_free(graph->line_data_cache, graph_line);
            dvr_remove_nodirty(graph_line->data.src_fd);
        }
    }

    return cnt;
}


/* force writing parameters */
int dvr_graph_apply_property(dvr_graph *graph, FuncTag *tag)
{
    int ret=0;

  down(&dvr_graph_suspend_sem);

    ret = video_suspend();
    if(ret<0)
    {
        printk("[dvr_graph] suspend error1!\n");
        goto DVR_GRAPH_APPLY_PROP_EXIT;
    }

  down(&dvr_graph_oper_sem);
    ret = dvr_graph_write_property(graph, tag);
    if(ret<0)
    {
/*
        dvr_graph_prop *graph_prop, *tmp_prop;
        list_for_each_entry_safe(graph_prop, tmp_prop, &graph->prop_entry, list)
        {
            list_del(&graph_prop->list);
            kmem_cache_free(graph->prop_data_cache, graph_prop);
        }
*/
    }
  up(&dvr_graph_oper_sem);
    apply_nodirty_all_list();
    ret = video_startgraph();
    if(ret<0)
        printk("[dvr_graph] start graph failed1!(0x%x)\n", ret);
    swap_notify();

DVR_GRAPH_APPLY_PROP_EXIT:
  up(&dvr_graph_suspend_sem);
    return ret;
}


/* Original API, apply only this graph and don't care the tag */
int dvr_graph_run(dvr_graph *graph)
{
    panic("This function is not used anymore.\n");
    return -1;

#if 0
    int ret=0;
    dvr_graph_line *graph_line, *tmp_line;
    dvr_graph_prop *graph_prop, *tmp_prop;
    FuncTag tag;

    FN_SET_ALL(&tag);

	down(&dvr_graph_oper_sem);

    ret = video_suspend();
    if(ret<0)
    {
        printk("[dvr_graph] suspend error2!\n");
        goto DVR_GRAPH_RUN_EXIT;
    }
    printk("#####%s:suspend\n", __FUNCTION__);

    ret = dvr_graph_write_graphline(graph, &tag);
    if(ret<0)
        goto DVR_GRAPH_RUN_ERROR;

    ret = dvr_graph_write_property(graph, &tag);
    if(ret<0)
        goto DVR_GRAPH_RUN_ERROR;

    apply_nodirty_all_list();

    ret = video_startgraph();
    if(ret<0)
        printk("[dvr_graph] start graph failed2!(0x%x)\n", ret);
    swap_notify();
    printk("#####%s:start\n", __FUNCTION__);
	up(&dvr_graph_oper_sem);
    return 0;


DVR_GRAPH_RUN_ERROR:
    //Once error, clear unhandled graph and property
    list_for_each_entry_safe(graph_line, tmp_line, &graph->line_entry, list)
    {
        if(graph_line->data.state == GFL_NEW)
        {
            list_del(&graph_line->list);
            kmem_cache_free(graph->line_data_cache, graph_line);
        }
    }
    list_for_each_entry_safe(graph_prop, tmp_prop, &graph->prop_entry, list)
    {
        list_del(&graph_prop->list);
        kmem_cache_free(graph->prop_data_cache, graph_prop);
    }
    printk("[dvr_graph] Error handling, clear unhandled properties and graph!\n");

    apply_nodirty_all_list();

    ret = video_startgraph();
    if(ret<0)
        printk("[dvr_graph] Error handling, start graph failed(0x%x)\n", ret);
    swap_notify();

DVR_GRAPH_RUN_EXIT:
	up(&dvr_graph_oper_sem);
    return -1;
#endif

}

//extern int global_state;

/* Traverse all graphs and apply it to VG when matching 'tag_mask' */
int dvr_graph_apply_by_tag(FuncTag *tag_mask)
{
    int ret=0;
    dvr_graph *one_graph, *tmp_one_graph;
    int need_restart = FALSE;
    int gfl_state_chg;
    dvr_graph_line *graph_line, *tmp_line;


  down(&dvr_graph_suspend_sem);
  down(&dvr_graph_oper_sem);
	#if 1
////////////////////////////
    if(flow_rate_flag == 1) {
        dvr_graph_apply_flow_list();
        flow_rate_flag = 0;
    }
/////////////////////////////
#endif
    if(list_empty(&sys_all_graph) && list_empty(&sys_all_vgfn)){
        VDBGPRINT(DBG_GRAPH_FNC, "%s tag(0x%x-%x-%x-%x-%x) do nothing.\n", __FUNCTION__, FN_ITEMS(tag_mask));
        goto end;
    }



    VDBGPRINT(DBG_GRAPH_FNC, "%s tag(0x%x-%x-%x-%x-%x) start\n", __FUNCTION__, FN_ITEMS(tag_mask));
    gfl_state_chg = 0;
    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {
        list_for_each_entry_safe(graph_line, tmp_line, &one_graph->line_entry, list)
        {
            if(graph_line->data.state != GFL_DONE) {
                gfl_state_chg = 1;
            }
        }
    }
    if(gfl_state_chg == 0) {     
        list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list) {
            if(list_empty(&one_graph->prop_entry)) 
                goto end;                           
        }            
    }

  up(&dvr_graph_oper_sem);
    ret = video_suspend();
    if(ret<0)
    {
        printk("[dvr_graph] suspend error2!\n");
        goto DVR_GRAPH_RUN_EXIT;
    }

  down(&dvr_graph_oper_sem);
    // find the tag with "FN_UPDATE_METHOD", if found, graph needs to restart!
    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {

        ret = dvr_graph_try_update_and_restart(one_graph, tag_mask);
        if(ret>0)
            need_restart = TRUE;
        else if(ret<0)
            goto DVR_GRAPH_RUN_ERROR;
    }
  up(&dvr_graph_oper_sem);

    if(need_restart)
    {
        VDBGPRINT(DBG_GRAPH_DATA, "DVR graph restart.\n");
        
        apply_nodirty_all_list();
        ret = video_startgraph();
        if(ret<0)
            printk("[dvr_graph] update! restart graph failed3!(0x%x)\n", ret);
        swap_notify();

        ret = video_suspend();
        if(ret<0)
        {
            printk("[dvr_graph] update! suspend graph failed3!(0x%x)\n", ret);
            up(&dvr_graph_oper_sem);
            goto DVR_GRAPH_RUN_EXIT;
        }
    }
  down(&dvr_graph_oper_sem);
    ret =dvr_graph_run_vg_fnc(tag_mask, TRUE);
    if(ret<0)
        goto DVR_GRAPH_RUN_ERROR;

    // Here, apply all otehr properties and graphs
    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {
        ret = dvr_graph_write_graphline(one_graph, tag_mask);
        if(ret<0)
            goto DVR_GRAPH_RUN_ERROR;

        ret = dvr_graph_write_property(one_graph, tag_mask);
        if(ret<0)
            goto DVR_GRAPH_RUN_ERROR;
    }

    ret =dvr_graph_run_vg_fnc(tag_mask, FALSE);
    if(ret<0)
        goto DVR_GRAPH_RUN_ERROR;

    apply_nodirty_all_list();

    ret = video_startgraph();

    if(ret<0)
        printk("[dvr_graph] start graph failed2!(0x%x)\n", ret);
    swap_notify();
    //printk("######%s:start\n", __FUNCTION__);
#if 0
    if(flow_rate_flag == 1) {
        dvr_graph_apply_flow_list();
        flow_rate_flag = 0;
    }
#endif
end:
	
    VDBGPRINT(DBG_GRAPH_FNC, "%s tag(0x%x-%x-%x-%x-%x) end\n", __FUNCTION__, FN_ITEMS(tag_mask));
  up(&dvr_graph_oper_sem);
  up(&dvr_graph_suspend_sem);
	//printk("@@Start to dump graph\n");
//	print_graph_all(0,0);
    return 0;


DVR_GRAPH_RUN_ERROR:
    printk("[dvr_graph] Apply graph failed! tag(0x%x-%x-%x-%x-%x)\n", FN_ITEMS(tag_mask));


  up(&dvr_graph_oper_sem);

    ret = video_startgraph();
    if(ret<0)
        printk("[dvr_graph] Error handling, start graph failed(0x%x)\n", ret);
    swap_notify();

DVR_GRAPH_RUN_EXIT:
  up(&dvr_graph_suspend_sem);
    return -1;
}

dvr_graph* dvr_graph_find_by_id(int graph_id)
{
    dvr_graph *one_graph, *tmp_one_graph;

    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {
        if(one_graph->id == graph_id)
            return one_graph;
    }
    printk("[dvr_common] Can't find graph. graphID(0x%x)\n", graph_id);
    return NULL;
}

int dvr_graph_setfree(FuncTag *tag, int fd,int freeway,int param)
{
    dvr_vgfn *t_vgfn;

	t_vgfn = kzalloc(sizeof(dvr_vgfn), GFP_KERNEL);

    t_vgfn->fn_type = VGFN_VG_SETFREE;
    t_vgfn->data.set_vg_freetype.fd = fd;
    t_vgfn->data.set_vg_freetype.freeway = freeway;
    t_vgfn->data.set_vg_freetype.param = param;
    FN_COPY_TAG(&t_vgfn->data.set_vg_freetype.tag, tag);

    INIT_LIST_HEAD(&t_vgfn->list);
    list_add_tail(&t_vgfn->list, &sys_all_vgfn);
    return 0;
}

int PROPID(int fd,char *name)
{
    int ret;
    ret=query_propertyid(fd, name);
    if(ret<0)
    {
//        panic("Error fd 0x%x property name %s\n",fd,name);
        return -1;
    }
    VDBGPRINT(DBG_GRAPH_DATA, "%s, [0x%x,%s]=>%d\n", __FUNCTION__, fd, name, ret);
    return ret;
}

struct v_queue_t *dvr_queue_allocate(int type,int num,int elem_size,char name[], struct v_queue_t *refer,int ddr,unsigned int pattern)
{
    dvr_queue *t_q;

    printk("    %s=(buf: %d,%d,%d)\n", name, elem_size, num, type);
	t_q = kzalloc(sizeof(dvr_queue), GFP_KERNEL);

    t_q->data.type = type;
    t_q->data.num = num;
    t_q->data.elem_size = elem_size;
    strcpy(t_q->data.name, name);
    t_q->data.refer = refer;
    t_q->data.ddr = ddr;
    t_q->data.pattern = pattern;

    t_q->queue = video_create_queue(type, num, elem_size, name, refer, ddr, pattern);
    INIT_LIST_HEAD(&t_q->list);
    list_add_tail(&t_q->list, &sys_all_queue);

    return t_q->queue;
}

int dvr_queue_destroy(struct v_queue_t *q)
{
    dvr_queue *one_queue, *tmp_one_queue;
    list_for_each_entry_safe(one_queue, tmp_one_queue, &sys_all_queue, list)
    {
        if(one_queue->queue == q)
        {
            list_del(&one_queue->list);
            kfree(one_queue);
        }
    }
    return video_destroy_queue(q);
}

struct v_queue_t* dvr_queue_find_by_name(char name[], int *size, int *count, int *ddr)
{
    dvr_queue *one_queue, *tmp_one_queue;

    list_for_each_entry_safe(one_queue, tmp_one_queue, &sys_all_queue, list)
    {
        if(!strcmp(one_queue->data.name, name))
        {
            *size = one_queue->data.elem_size;
            *count = one_queue->data.num;
            *ddr = one_queue->data.ddr;
            return one_queue->queue;
        }
    }

    printk("[dvr_common] Can't find queue ""%s""\n", name);
    return NULL;
}

int dvr_queue_rearrange(FuncTag *tag, struct v_queue_t *q,int num,int elem_size, int direct)
{
    dvr_vgfn *t_vgfn;

    if(q==0)
        return 0;
        
    if(direct)
    {
        return video_rearrange_queue(q, num, elem_size);
    }
    else
    {
    	t_vgfn = kzalloc(sizeof(dvr_vgfn), GFP_KERNEL);

        t_vgfn->fn_type = VGFN_REARRANGE_QUEUE;
        t_vgfn->data.rearrange_queue.q = q;
        t_vgfn->data.rearrange_queue.num = num;
        t_vgfn->data.rearrange_queue.elem_size = elem_size;
        FN_COPY_TAG(&t_vgfn->data.rearrange_queue.tag, tag);

        INIT_LIST_HEAD(&t_vgfn->list);
        list_add_tail(&t_vgfn->list, &sys_all_vgfn);
        return 0;
    }
}

static void dvr_clear_running_data(void)
{
    dvr_graph *one_graph, *tmp_one_graph;
    dvr_vgfn *one_vgfn, *tmp_one_vgfn;

    list_for_each_entry_safe(one_graph, tmp_one_graph, &sys_all_graph, list)
    {
        printk("Warning!! Graph(0x%x) is still left in the system.\n", (int)one_graph);
        dvr_graph_destroy(one_graph);
    }
    list_for_each_entry_safe(one_vgfn, tmp_one_vgfn, &sys_all_vgfn, list)
    {
        panic("Error!! Some videograph function doesn't run expectably. Check application flow!\n");
    }
}

void dvr_common_initialize(void)
{
    if(dvr_graph_is_initialize)
        return;
    dvr_graph_is_initialize = 1;

    INIT_LIST_HEAD(&sys_all_graph);
    INIT_LIST_HEAD(&sys_all_queue);
    INIT_LIST_HEAD(&sys_all_vgfn);

    init_MUTEX(&dvr_graph_oper_sem);
    init_MUTEX(&dvr_graph_suspend_sem);

    video_startgraph();
    swap_notify();
}

void dvr_common_terminate(void)
{
    dvr_queue *one_queue, *tmp_one_queue;

    if(!dvr_graph_is_initialize)
        return;
//2012_03_06 for ins/del module crash
   // video_stopgraph();
    dvr_clear_running_data();

    list_for_each_entry_safe(one_queue, tmp_one_queue, &sys_all_queue, list)
    {
        printk("Warning!! Queue(0x%x,""%s"") is still left in the system.\n", (int)one_queue->queue, one_queue->data.name);
        dvr_queue_destroy(one_queue->queue);
    }

    dvr_graph_is_initialize = 0;
}


EXPORT_SYMBOL(dvr_common_initialize);
EXPORT_SYMBOL(dvr_common_terminate);
EXPORT_SYMBOL(dvr_graph_create);
EXPORT_SYMBOL(dvr_graph_destroy);
EXPORT_SYMBOL(dvr_graph_add_link);
EXPORT_SYMBOL(dvr_graph_del_link);
EXPORT_SYMBOL(dvr_graph_update_flow_list);
EXPORT_SYMBOL(dvr_graph_apply_flow_list);
EXPORT_SYMBOL(dvr_graph_set_flow_flag);
EXPORT_SYMBOL(dvr_graph_update_flow);
EXPORT_SYMBOL(dvr_get_flow);
EXPORT_SYMBOL(dvr_graph_apply_flow);
EXPORT_SYMBOL(dvr_graph_set_property);
EXPORT_SYMBOL(dvr_graph_set_property_directly);
EXPORT_SYMBOL(dvr_graph_get_property);
EXPORT_SYMBOL(dvr_graph_apply_property);
EXPORT_SYMBOL(dvr_graph_run);
EXPORT_SYMBOL(dvr_graph_apply_by_tag);
EXPORT_SYMBOL(dvr_graph_find_by_id);
EXPORT_SYMBOL(dvr_queue_allocate);
EXPORT_SYMBOL(dvr_queue_destroy);
EXPORT_SYMBOL(dvr_queue_find_by_name);
EXPORT_SYMBOL(dvr_queue_rearrange);
EXPORT_SYMBOL(dvr_register_postprocess);
EXPORT_SYMBOL(dvr_register_preprocess);
EXPORT_SYMBOL(PROPID);
EXPORT_SYMBOL(dvr_graph_setfree);
EXPORT_SYMBOL(dvr_common_store_info);
EXPORT_SYMBOL(dvr_common_retrieve_info);



/*****************************************************************************************************/
/*****************************************************************************************************/
char TextData_gmdvr_mem_cfg[] =
"[dvr_disp]                    \n"
"lcd_size            = 1572864 \n"
"lcd_count           = 5       \n"
"lcd_ddr             = 0       \n"
"3di_scl_in_size     = 1244160 \n"
"3di_scl_in_count    = 5       \n"
"3di_scl_in_ddr      = 1       \n"
"lv_scl_in_size      = 202752  \n"
"lv_scl_in_count     = 3       \n"
"lv_scl_in_ddr       = 1       \n"
"                              \n"
"[dvr_enc]                     \n"
"enc_in_size         = 1244160 \n"
"enc_in_count        = 5       \n"
"enc_in_ddr          = 1       \n"
"enc_in_limit_count  = 6       \n"
"enc_out_size        = 327680  \n"
"enc_out_count       = 5       \n"
"enc_out_ddr         = 0       \n"
"sub1_enc_in_size    = 202752  \n"
"sub1_enc_in_count   = 3       \n"
"sub1_enc_in_ddr     = 1       \n"
"sub1_enc_in_limit_count = 6    \n"
"sub1_enc_out_size   = 65536   \n"
"sub1_enc_out_count  = 3       \n"
"sub1_enc_out_ddr    = 0       \n"
"sub2_enc_in_size    = 50688   \n"
"sub2_enc_in_count   = 3       \n"
"sub2_enc_in_ddr     = 1       \n"
"sub2_enc_in_limit_count = 6    \n"
"sub2_enc_out_size   = 30720   \n"
"sub2_enc_out_count  = 3       \n"
"sub2_enc_out_ddr    = 0       \n"
"ss_enc_in_size      = 622080  \n"
"ss_enc_in_count     = 1       \n"
"ss_enc_in_ddr       = 1       \n"
"ss_enc_out_size     = 158464  \n"
"ss_enc_out_count    = 1       \n"
"ss_enc_out_ddr      = 0       \n"
"                              \n"
"[dvr_dec]                     \n"
"dec_in_size         = 158464  \n"
"dec_in_count        = 5       \n"
"dec_in_ddr          = 1       \n"
"pb_scl_in_size      = 829440  \n"
"pb_scl_in_count     = 3       \n"
"pb_scl_in_ddr       = 1       \n"
"pb_scl_in_limit_count = 5     \n"
#if (TWO_STAGES_SCALER && !SHARE_LV_PB_SCL_BUF)
"pb_scl2_in_size     = 202752  \n"
"pb_scl2_in_count    = 3       \n"
"pb_scl2_in_ddr      = 1       \n"
"pb_scl2_in_limit_count = 5     \n"
#endif
;

dvr_common_data    dvr_common_private_data;


static int dvr_graph_write_cfgfile(char *filename)
{
	mm_segment_t    fs;
    int fd1;

	/* get current->addr_limit. It should be 0-3G */
	fs = get_fs();
	/* set current->addr_limit to 4G */
	set_fs(KERNEL_DS);

	fd1 = sys_open(filename, O_WRONLY|O_CREAT, S_IRWXU);
	if(fd1 < 0)
	{
	    set_fs(fs);
		printk("Frammap fails to open file %s \n", filename);
		return -1;
	}

	sys_write(fd1, TextData_gmdvr_mem_cfg , sizeof(TextData_gmdvr_mem_cfg));
	sys_close(fd1);

	/* restore the addr_limit */
	set_fs(fs);

	return 0;
}

int dvr_common_store_info(int id, void *data)
{
    buf_flow_count_t *bfc;
    if(id == INFO_SET_BUF_FLOW_COUNT)
    {
        dvr_info_buf_flow_info  *bf_info=(dvr_info_buf_flow_info*)data;
        if(BFI_GET_FNC(bf_info->fnc)==0x010000)
            bfc = &dvr_common_private_data.buf_flow_cnt.rec[BFI_GET_CH(bf_info->fnc)][BFI_GET_REPD(bf_info->fnc)];
        else
            bfc = &dvr_common_private_data.buf_flow_cnt.pb[BFI_GET_CH(bf_info->fnc)];
        bfc->in_out += bf_info->in_out;
        if(bf_info->in_out>0)
        {
            bfc->timeout = 0;
            bfc->count ++;
        }
    }
    else if(id == INFO_SET_FLOW_TIMEOUT)
    {
        int  fnc = (int) data;
        if(BFI_GET_FNC(fnc)==0x010000)
            dvr_common_private_data.buf_flow_cnt.rec[BFI_GET_CH(fnc)][BFI_GET_REPD(fnc)].timeout = 1;
        else
            dvr_common_private_data.buf_flow_cnt.pb[BFI_GET_CH(fnc)].timeout = 1;
    }
    else if(id == INFO_RESET_BUF_FLOW_IN_OUT)
    {
        int  fnc = (int) data;
        if(BFI_GET_FNC(fnc)==0x010000)
        {
            bfc = &dvr_common_private_data.buf_flow_cnt.rec[BFI_GET_CH(fnc)][BFI_GET_REPD(fnc)];
            bfc->timeout = 0;
            bfc->in_out = 0;
        }
        else
        {
            bfc = &dvr_common_private_data.buf_flow_cnt.pb[BFI_GET_CH(fnc)];
            bfc->timeout = 0;
            bfc->in_out = 0;
        }
    }
    else if(id == INFO_INDEX_DISP_COUNT)
    {
        memcpy(&dvr_common_private_data.disp_cnt_info, data, sizeof(dvr_info_disp_count));
    }
    else
        printk("[dvr_common] Unknown info ID(%d).\n", id);
    return 0;    
}

int dvr_common_retrieve_info(int id, void *data)
{
    buf_flow_count_t *bfc;

    if(!data)
        return -1;

    if(id == INFO_CHECK_BUF_FLOW_PAIR)
    {
        dvr_info_buf_flow_info  *bf_info=(dvr_info_buf_flow_info*)data;
        if(BFI_GET_FNC(bf_info->fnc)==0x010000)
            bfc = &dvr_common_private_data.buf_flow_cnt.rec[BFI_GET_CH(bf_info->fnc)][BFI_GET_REPD(bf_info->fnc)];
        else
            bfc = &dvr_common_private_data.buf_flow_cnt.pb[BFI_GET_CH(bf_info->fnc)];
        return (bfc->timeout && bfc->in_out>0);
    }
    else if(id == INFO_INDEX_DISP_COUNT)
    {
        memcpy(data, &dvr_common_private_data.disp_cnt_info, sizeof(dvr_info_disp_count));
    }
    else
        printk("[dvr_common] Unknown info ID(%d).\n", id);
    return 0;    
}

static int check_state(dvr_common_data  *dvrcmn_data, FuncTag *ptag)
{
	//int i, j;
    int i;
    //FuncTag grp_tag;
    if(FN_IS_FN(ptag, FN_LIVEVIEW|FN_PLAYBACK))
    {
        for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
            if(dvrcmn_data->buf_flow_cnt.pb[i].in_out>0 && dvrcmn_data->buf_flow_cnt.pb[i].timeout)
                return -1;
    }
    
#if 0
    FN_RESET_TAG(&grp_tag);
    FN_SET_REC_CH(&grp_tag, 0);
    FN_SET_SUB1_REC_CH(&grp_tag, 0);
    FN_SET_SUB2_REC_CH(&grp_tag, 0);
    grp_tag.rec_ch = 0xFFFFFFFF;
    if(FN_CHECK_MASK(&grp_tag, ptag))
    {
        for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++)
          for(j=0; j<DVR_ENC_REPD_BT_NUM+1; j++)
            if(dvrcmn_data->buf_flow_cnt.rec[i][j].in_out!=0)
                return -1;
    }
#endif
    return 0;
}


static int proc_read_mode(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    int i, j;
    dvr_graph *one_graph;
    dvr_graph_line *graph_line;
    dvr_graph_prop *graph_prop;
//    dvr_queue *one_queue;
    dvr_vgfn *one_vgfn;
    buf_flow_count_t *bfc;

    printk("Debug Mode:%d (0x0:Turn off, 0x20:Trace flow)\n", dvr_common_dbgmode);
/*
    printk("Current DVR Queue...\n");
    list_for_each_entry(one_queue, &sys_all_queue, list)
    {
        printk(" Queue(0x%0x): type(%d) num(%d) size(%8d) ddr(%d) refer(0x%8x) name=""%s""\n",
            (int)one_queue->queue, one_queue->data.type, one_queue->data.num, one_queue->data.elem_size, one_queue->data.ddr, (int)one_queue->data.refer, one_queue->data.name);
    }
*/
    printk("\nCurrent DVR Graph...\n");
    list_for_each_entry(one_graph, &sys_all_graph, list)
    {
        list_for_each_entry(graph_line, &one_graph->line_entry, list)
        {
            printk(" Graph(%s,%d) state(%d): tag(0x%x-%x-%x-%x-%x) src(0x%x,%d) dst(0x%x,%d) Que(%s)\n",
                one_graph->linecache_str, graph_line->data.gfd, graph_line->data.state, FN_ITEMS(&graph_line->data.tag),
                graph_line->data.src_fd, graph_line->data.src_flow, graph_line->data.dst_fd, graph_line->data.dst_flow, graph_line->data.que->name);
        }
        list_for_each_entry(graph_prop, &one_graph->prop_entry, list)
        {
            printk("   - prop(%s): tag(0x%x-%x-%x-%x-%x) fd(0x%x) id(%d) value(%d) direct(%d)\n",
                one_graph->propcache_str, FN_ITEMS(&graph_prop->data.tag),
                graph_prop->data.fd, graph_prop->data.id, graph_prop->data.value, graph_prop->data.direct);
        }
    }
    printk("Current VGFN Queue (waiting for apply)...\n");
    list_for_each_entry(one_vgfn, &sys_all_vgfn, list)
    {
        printk(" vgfn_type(%d)\n", one_vgfn->fn_type);
    }

    printk("Current buffer flow info... (cnt/in_out/timeout)\n");
    for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++)
    {
        printk(" REC ch(%d):  ", i);
        for(j=0; j<DVR_ENC_REPD_BT_NUM+1; j++)
        {
            bfc = &dvr_common_private_data.buf_flow_cnt.rec[i][j];
            printk("st%d(%d,%d,%d) ", j, bfc->count, bfc->in_out, bfc->timeout);
        }
        printk("\n");
    }
    for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
    {
        bfc = &dvr_common_private_data.buf_flow_cnt.pb[i];
        printk(" PB ch(%d): (%d,%d,%d)\n", i, bfc->count, bfc->in_out, bfc->timeout);
    }

    *eof = 1;
    return 0;
}


static int proc_write_mode(struct file *file, const char *buffer,unsigned long count, void *data)
{
    int len = count;
    unsigned char value[20];
    uint tmp;

    if (copy_from_user(value, buffer, len))
        return 0;
    value[len] = '\0';

    sscanf(value, "%u\n", &tmp);
    dvr_common_dbgmode = tmp;
    printk("Debug Mode:%d\n", dvr_common_dbgmode);
    return count;
}

void print_user_last_api_data(unsigned char *msg, int api_num, int length, unsigned char *data, int is_input)
{
    int i=0;

    printk("\n [%s] API No.(%d) length:%d  dir:%s\n", msg, api_num, length, is_input?"Input":"Output");
    printk("-- Offset : --------------- HEX --------------- | ------ DEC ------\n");

    while(length>0)
    {
        if(length>=16)
        {
            printk("0x%03x(%3d): %08x %08x %08x %08x | %d %d %d %d\n", i, i,
                *((int*)data), *((int*)(data+4)), *((int*)(data+8)), *((int*)(data+12)),
                *((int*)data), *((int*)(data+4)), *((int*)(data+8)), *((int*)(data+12))
                );
            data+=16;
            length-=16;
            i+=16;
        }
        else
        {
            printk("0x%03x(%3d): ", i, i);
            for(i=0; i<length; i+=4)
                printk("%08x ", *((int*)(data+i)));
            for(; i<16; i+=4)
                printk("         ");
            printk("| ");
            for(i=0; i<length; i+=4)
                printk("%d ", *((int*)(data+i)));
            printk("\n");
            break;
        }
    }
    printk("-----------------------------------------------------------------------\n");
}
EXPORT_SYMBOL(print_user_last_api_data);


static int proc_read_lastapi(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    if(last_api_num == 1)
        print_user_last_api_data("DISP:DVR_COMMON_APPLY", last_api_num, sizeof(FuncTag), (unsigned char*)&dvr_common_private_data.la_APPLY, 1);
    return 0;
}

static ssize_t proc_write_lastapi(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    int len = size;
    char value[8];

    if(copy_from_user(value, buf, len))
        return 0;
    value[len] = '\0';
    sscanf(value, "%u\n", &last_api_num);
    return size;
}

int dvr_common_open(struct inode *inode, struct file *filp)
{
    if(dvr_common_private_data.is_used == TRUE)
    {
        printk("[dvr_common] Can't open this module twice!\n");
        return -1;
    }

    filp->private_data = &dvr_common_private_data;
    dvr_common_open_cnt++;
    return 0;
}

//extern int video_cleargraph_all(void);
int dvr_common_release(struct inode *inode, struct file *filp)
{

    if(--dvr_common_open_cnt)
        return 0;

    dvr_common_private_data.is_used = FALSE;
    dvr_clear_running_data();

    return 0;
}


static int dvr_common_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret=-1;
    dvr_common_data  *dvrcmn_data = filp->private_data;

    down(&dvrcmn_data->dvr_common_sem);
    switch(cmd)
    {
      case DVR_COMMON_APPLY:
      {
        FuncTag    tag;
		
		if(copy_from_user((void *)&tag, (void *)arg, sizeof(FuncTag))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&dvr_common_private_data.la_APPLY, &tag, sizeof(FuncTag));
        printm("--------- APPLY [0x%x] tag 0x%x/0x%x/0x%x/0x%x ---------\n", jiffies&0xffff,tag.func,tag.lv_ch,tag.rec_ch,tag.pb_ch);
//printk("@@@@@  %s:%s:%d  @@@@@\n",__FILE__,__FUNCTION__,__LINE__);

        // check state before apply
        ret = check_state(dvrcmn_data, &tag);        
        if(ret>=0){
		//	printk("@@dvr_graph_apply_by_tag:fun=%x-ch=%x\n",tag.func,tag.rec_ch);
            ret = dvr_graph_apply_by_tag(&tag);   
        	}
        else
        {
            printk("[dvr_common] DVR_XXX_QUEUE_GET and DVR_XXX_QUEUE_PUT must operate in pairs before applying the graph.\n");
            ret = -2;
        }        
        break;
      }
      case DVR_COMMON_DEBUG:
        {
            char error_msg[100];
            char user_msg[90];
            
            if(copy_from_user((void *)user_msg, (void *)arg, 90)) {
				ret = -EFAULT;
				goto exit1;
			}
            sprintf(error_msg,"##### [0x%x] %s",(int)jiffies&0xffff,user_msg);
            printm(error_msg);
            ret=0;
        }
        break;
      case DVR_COMMON_DEBUG_WITH_PANIC:
        {
            char error_msg[100];
            char user_msg[90];
            
	        if(copy_from_user((void *)user_msg, (void *)arg, 90)) {
				ret = -EFAULT;
				goto exit1;
			}
            sprintf(error_msg,"##### [0x%x] %s",(int)jiffies&0xffff,user_msg);
            printm_with_panic(error_msg);
            ret=0;
        }
        break;
      default:
        printk("[dvr_common] cmd(0x%x) no define!\n", cmd);
        break;
    }

exit1:
    up(&dvrcmn_data->dvr_common_sem);
    return ret;
}



struct file_operations dvr_common_fops = {
  	owner:      THIS_MODULE,
	ioctl:		dvr_common_ioctl,
	open:		dvr_common_open,
	release:	dvr_common_release,
};


struct miscdevice dvr_common_dev = {
	minor: MISC_DYNAMIC_MINOR,
	name: "dvr_common",
	fops: &dvr_common_fops,
};



static int __init dvr_common_init(void)
{
    file_t *cfile;

    memset(&dvr_common_private_data, 0x0, sizeof(dvr_common_data));

    // Add proc node
    DvrProcRoot = create_proc_entry("dvr", S_IFDIR | S_IRUGO | S_IXUGO, NULL);
    proc_cmn_dbg = create_proc_entry("common_log", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(proc_cmn_dbg==NULL)
        return -EFAULT;
    proc_cmn_dbg->read_proc = (read_proc_t *) proc_read_mode;
    proc_cmn_dbg->write_proc = (write_proc_t *) proc_write_mode;
    proc_cmn_dbg->owner = THIS_MODULE;

    proc_last_api = create_proc_entry("common_api", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(proc_cmn_dbg==NULL)
        return -EFAULT;
    proc_last_api->read_proc = (read_proc_t *) proc_read_lastapi;
    proc_last_api->write_proc = (write_proc_t *) proc_write_lastapi;
    proc_last_api->owner = THIS_MODULE;


    cfile = gmcfg_open(GMDVR_MEM_CFG_FILE, O_RDONLY);
    if(cfile)
        gmcfg_close(cfile) ;
    else
    {
        if(dvr_graph_write_cfgfile(GMDVR_MEM_CFG_FILE)<0)
        {
            printk("[dvr_common] create default config file [%s] failed!\n", GMDVR_MEM_CFG_FILE);
            return -EFAULT;
        }
        else
            printk("Warning!! 'dvr_common' creates default config file [%s]. Using this file MAY NOT achieve max performance.\n", GMDVR_MEM_CFG_FILE);
    }

    init_MUTEX(&dvr_common_private_data.dvr_common_sem);
		
    dvr_common_initialize();

    printk("Insert dvr_common driver done.\n");

    return misc_register(&dvr_common_dev);
}

static void __exit dvr_common_clearnup(void)
{
    misc_deregister(&dvr_common_dev);

    dvr_common_terminate();

    remove_proc_entry("common_api", DvrProcRoot);
    remove_proc_entry("common_log", DvrProcRoot);
    remove_proc_entry("dvr", NULL);
}

module_init(dvr_common_init);
module_exit(dvr_common_clearnup);

MODULE_LICENSE("GPL");
