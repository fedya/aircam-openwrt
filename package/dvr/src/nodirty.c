/* check scaler to display nodirty overlap */
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
#include <asm/uaccess.h>

#include "videograph_api.h"

//#define DEBUG_NODIRTY

//#define DEBUG_PRINT printk
#define DEBUG_PRINT printm

struct nodirty_item_t
{
    int used;
    int active;
    int fd;
    int x,y,w,h;
    int nodirty;
    int disabled;
    int region_check; //if need to check same fd with different regsion
};

#define INDEX_OLD   0
#define INDEX_NEW   1
#define MAX_NODIRTY 16
struct nodirty_item_t nodirty_item[2][MAX_NODIRTY]; //0:old 1:new


void print_nodirty(void)
{
#ifdef DEBUG_NODIRTY
    int i;

    DEBUG_PRINT("===========\nOLD:\n");
    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_OLD][i].used==0)
            continue;
        if(nodirty_item[INDEX_OLD][i].active==0)
            continue;
        DEBUG_PRINT("           [%d]0x%x (%d,%d)/(%d,%d) nodirty=%d disabled=%d\n",i,
            nodirty_item[INDEX_OLD][i].fd,nodirty_item[INDEX_OLD][i].x,nodirty_item[INDEX_OLD][i].y,
            nodirty_item[INDEX_OLD][i].w,nodirty_item[INDEX_OLD][i].h,nodirty_item[INDEX_OLD][i].nodirty,
            nodirty_item[INDEX_OLD][i].disabled);
    }

    DEBUG_PRINT("NEW:\n");
    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==0)
            continue;
        if(nodirty_item[INDEX_NEW][i].active==0)
            continue;
        DEBUG_PRINT("           [%d]0x%x (%d,%d)/(%d,%d) nodirty=%d disabled=%d\n",i,
            nodirty_item[INDEX_NEW][i].fd,nodirty_item[INDEX_NEW][i].x,nodirty_item[INDEX_NEW][i].y,
            nodirty_item[INDEX_NEW][i].w,nodirty_item[INDEX_NEW][i].h,nodirty_item[INDEX_NEW][i].nodirty,
            nodirty_item[INDEX_NEW][i].disabled);
    }
#endif
}

int check_overlap(int sx,int sy,int sw,int sh,int lx,int ly,int lw,int lh)
{
#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ check_overlap %d,%d/%d,%d to %d,%d/%d,%d\n",sx,sy,sw,sh,lx,ly,lw,lh);
#endif

    if((sx==lx)&&(sy==ly)&&(sw==lw)&&(sh==lh))
        return 1;
        
    if((sx>lx)&&(sx<lx+lw)&&
       (sy>ly)&&(sy<ly+lh))
    {
        return 1;
    }
    if((sx+sw>lx)&&(sx+sw<lx+lw)&&
       (sy>ly)&&(sy<ly+lh))
    {
        return 1;
    }
    if((sx+sw>lx)&&(sx+sw<lx+lw)&&
       (sy+sh>ly)&&(sy+sh<ly+lh))
    {
        return 1;
    }
    if((sx>lx)&&(sx<lx+lw)&&
       (sy+sh>ly)&&(sy+sh<ly+lh))
    {
        return 1;
    }
    return 0;
}




//check INDEX_OLD if any nodirty exist
int check_and_set_conflict_nodirty(struct nodirty_item_t *check_item)
{
    int i;
    int x1,y1,w1,h1,x2,y2,w2,h2;

    x1=check_item->x;
    y1=check_item->y;
    w1=check_item->w;
    h1=check_item->h;


    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_OLD][i].used==0)
            continue;
        if(nodirty_item[INDEX_OLD][i].active==0)
            continue;
        if(nodirty_item[INDEX_OLD][i].disabled==1)
            continue;

        x2=nodirty_item[INDEX_OLD][i].x;
        y2=nodirty_item[INDEX_OLD][i].y;
        w2=nodirty_item[INDEX_OLD][i].w;
        h2=nodirty_item[INDEX_OLD][i].h;

#ifdef DEBUG_NODIRTY
        DEBUG_PRINT("________ verify check %d overlay fd 0x%x(%d,%d)/(%d,%d) with fd 0x%x(%d,%d)/(%d,%d)\n",
            check_item->region_check,check_item->fd,x1,y1,w1,h1,nodirty_item[INDEX_OLD][i].fd,x2,y2,w2,h2);
#endif

        //same setting, don't do check overlay (liveview,playback)
        if((nodirty_item[INDEX_OLD][i].fd==check_item->fd)&&(x1==x2)&&(y1==y2)&&(w1==w2)&&(h1==h2))
        {
            nodirty_item[INDEX_OLD][i].disabled=1;
#ifdef DEBUG_NODIRTY
                DEBUG_PRINT("________ Same region set fd 0x%x to disabled=1\n",
                    nodirty_item[INDEX_OLD][i].fd);
#endif
            continue;
        }

        if(check_item->region_check==0)
        {
            if(nodirty_item[INDEX_OLD][i].fd==check_item->fd)
            {
                nodirty_item[INDEX_OLD][i].disabled=1;

#ifdef DEBUG_NODIRTY
                DEBUG_PRINT("________ set fd 0x%x to disabled=1\n",
                    nodirty_item[INDEX_OLD][i].fd);
#endif
                
                if(w1<w2)
                {
                    if(check_overlap(x1,y1,w1,h1,x2,y2,w2,h2))
                    {
                        nodirty_item[INDEX_OLD][i].nodirty=NODIRTY_MODE_FALSE;
#ifdef DEBUG_NODIRTY
                        DEBUG_PRINT("________ set fd 0x%x to NODIRTY_MODE_FALSE, disabled=1\n",
                            nodirty_item[INDEX_OLD][i].fd);
#endif
                    }
                }
                else
                {
                    if(check_overlap(x2,y2,w2,h2,x1,y1,w1,h1))
                    {
                        nodirty_item[INDEX_OLD][i].nodirty=NODIRTY_MODE_FALSE;
#ifdef DEBUG_NODIRTY
                        DEBUG_PRINT("________ set fd 0x%x to NODIRTY_MODE_FALSE, disabled=1\n",
                            nodirty_item[INDEX_OLD][i].fd);
#endif
                    }
                }
                continue;
            }
        }

        if(w1<w2)
        {
            if(check_overlap(x1,y1,w1,h1,x2,y2,w2,h2))
            {
#ifdef DEBUG_NODIRTY
                DEBUG_PRINT("________ set fd 0x%x to NODIRTY_MODE_FALSE\n",nodirty_item[INDEX_OLD][i].fd);
#endif
                nodirty_item[INDEX_OLD][i].nodirty=NODIRTY_MODE_FALSE;
                continue; //must see all nodirty_item[INDEX_OLD][i]
            }
        }
        else
        {
            if(check_overlap(x2,y2,w2,h2,x1,y1,w1,h1))
            {
#ifdef DEBUG_NODIRTY
                DEBUG_PRINT("________ set fd 0x%x to NODIRTY_MODE_FALSE\n",nodirty_item[INDEX_OLD][i].fd);
#endif
                nodirty_item[INDEX_OLD][i].nodirty=NODIRTY_MODE_FALSE;
                continue; //must see all nodirty_item[INDEX_OLD][i]
            }
        }
    }
   
    return 0;
}


void dvr_remove_nodirty(int fd)
{
    int i;

    if((fd&0xfff00)!=0xa1e00)
        return;

#ifdef DEBUG_NODIRTY
        DEBUG_PRINT("________ dvr_remove_nodirty 0x%x\n",fd);
#endif

    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==1)
        {
            if(fd==nodirty_item[INDEX_NEW][i].fd)
            {
                memset(&nodirty_item[INDEX_NEW][i],0,sizeof(struct nodirty_item_t));
                break;
            }
        }
    }
    
    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_OLD][i].used==1)
        {
            if(fd==nodirty_item[INDEX_OLD][i].fd)
            {
                memset(&nodirty_item[INDEX_OLD][i],0,sizeof(struct nodirty_item_t));
                break;
            }
        }
    }
}

void dvr_recalculate_nodirty(void)
{
    int i;

#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ dvr_recalculate_nodirty\n");
#endif

    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==1)
        {
            check_and_set_conflict_nodirty(&nodirty_item[INDEX_NEW][i]);
        }
    }
}

void dvr_active_nodirty(int fd)
{
    int i;

    if((fd&0xfff00)!=0xa1e00)
        return;

#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ dvr_active_nodirty 0x%x\n",fd);
#endif

    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==1)
        {
            if(fd==nodirty_item[INDEX_NEW][i].fd)
            {
                nodirty_item[INDEX_NEW][i].active=1;
                check_and_set_conflict_nodirty(&nodirty_item[INDEX_NEW][i]);
                return;
            }
        }
    }
}


void apply_nodirty_all_list(void)
{
    int i;

    dvr_recalculate_nodirty();

    for(i=0;i<MAX_NODIRTY;i++)
    {
//note: will not set when nodirty_item is cleared, stoped() because item removed
        if((nodirty_item[INDEX_OLD][i].used==1)&&(nodirty_item[INDEX_OLD][i].active))
        {
            int nodirty=0,disabled=0;
#ifdef DEBUG_NODIRTY
            DEBUG_PRINT("________ dvr video_set_src_nodirty 0x%x to nodirty %d disabled %d\n",
                nodirty_item[INDEX_OLD][i].fd,nodirty_item[INDEX_OLD][i].nodirty,
                nodirty_item[INDEX_OLD][i].disabled);
#endif
            disabled=nodirty_item[INDEX_OLD][i].disabled;
            if(disabled==0)
                nodirty=nodirty_item[INDEX_OLD][i].nodirty;
            video_set_src_nodirty(nodirty_item[INDEX_OLD][i].fd,nodirty,disabled);
        }
    }
    
    print_nodirty();
    
#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("######## apply_nodirty_all_list copy new to old\n");
#endif
    memcpy(nodirty_item[INDEX_OLD],nodirty_item[INDEX_NEW],sizeof(struct nodirty_item_t)*MAX_NODIRTY);
}


void dvr_add_or_modify_nodirty(int fd,int is_display,int region_check,int x,int y,int w,int h)
{
    int i;

    print_nodirty();
    
    if((fd&0xfff00)!=0xa1e00)
        return;

    //if(is_display==0)
    {   //update it in "OLD"
        for(i=0;i<MAX_NODIRTY;i++)
        {
            if(nodirty_item[INDEX_OLD][i].used==1)
            {
                if(fd==nodirty_item[INDEX_OLD][i].fd)
                {
                    nodirty_item[INDEX_OLD][i].disabled=1-is_display;
#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ update %x to disabled %d\n",fd,nodirty_item[INDEX_OLD][i].disabled);
#endif
                }
            }
        }
    }
        
    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==1)
        {
            if(fd==nodirty_item[INDEX_NEW][i].fd)
            {

                nodirty_item[INDEX_NEW][i].nodirty=NODIRTY_MODE_TRUE;
                nodirty_item[INDEX_NEW][i].disabled=1-is_display;
 #ifdef DEBUG_NODIRTY
        DEBUG_PRINT("________ dvr_add_or_modify_nodirty >>>>> %d 0x%x at 0x%x (%d,%d)/(%d,%d) disabled %d\n",
            i,fd,(unsigned int)&nodirty_item[INDEX_NEW][i],x,y,w,h,nodirty_item[INDEX_NEW][i].disabled);
#endif                               
                nodirty_item[INDEX_NEW][i].region_check=region_check;
                nodirty_item[INDEX_NEW][i].x=x;
                nodirty_item[INDEX_NEW][i].y=y;
                nodirty_item[INDEX_NEW][i].w=w;
                nodirty_item[INDEX_NEW][i].h=h;
                return;
            }
        }
    }

    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_NEW][i].used==0)
        {
#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ dvr_add_or_modify_nodirty +++++ %d 0x%x at 0x%x (%d,%d)/(%d,%d)\n",
        i,fd,(unsigned int)&nodirty_item[INDEX_NEW][i],x,y,w,h);
#endif
            nodirty_item[INDEX_NEW][i].active=0;
            nodirty_item[INDEX_NEW][i].fd=fd;
            nodirty_item[INDEX_NEW][i].used=1;
            nodirty_item[INDEX_NEW][i].nodirty=NODIRTY_MODE_TRUE;
            nodirty_item[INDEX_NEW][i].disabled=1-is_display;
            
#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ add %x to disabled %d\n",fd,nodirty_item[INDEX_OLD][i].disabled);
#endif
            nodirty_item[INDEX_NEW][i].region_check=region_check;
            nodirty_item[INDEX_NEW][i].x=x;
            nodirty_item[INDEX_NEW][i].y=y;
            nodirty_item[INDEX_NEW][i].w=w;
            nodirty_item[INDEX_NEW][i].h=h;

            return;
        }
    }
    return;
}


void force_nodirty_mode(int fd,int nodirty,int disabled)
{
    int i;

#ifdef DEBUG_NODIRTY
    DEBUG_PRINT("________ force_nodirty_mode 0x%x nodirty %d disabled %d\n",fd,nodirty,disabled);
#endif

    //search for current list
    for(i=0;i<MAX_NODIRTY;i++)
    {
        if(nodirty_item[INDEX_OLD][i].used==1)
        {
            if(fd==nodirty_item[INDEX_OLD][i].fd)
            {
                nodirty_item[INDEX_OLD][i].disabled=disabled;
                nodirty_item[INDEX_OLD][i].nodirty=nodirty;
                return;
            }
        }
    }
    //if not found, set it directly
    video_set_src_nodirty(fd,nodirty,disabled);
}

EXPORT_SYMBOL(dvr_add_or_modify_nodirty);
EXPORT_SYMBOL(force_nodirty_mode);


