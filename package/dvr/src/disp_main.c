#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>

#include <linux/mm.h>
#include <asm/pgtable.h>

#include "lcd_fb.h"

/*
    <use scaler>         <not use scaler>
         datain                 datain
        /                       /
  scl_in_q                  dec_in_q
        \                       \
         dec                     dec
        /                       /
  scl_in_dup_q              lcd_q
        \                        \
         scaler                   lcd
        /
   lcd_q
        \
         lcd


 <No 3DI, No scaler>      <3DI enabled, No scaler>

         cap                     cap
        /                       /
   lcd_q                    lcd_q
        \                       \
         lcd                     3di
                                /
                            lcd_dup_q
                                \
                                 lcd

                                                
<No 3DI, scaler enabled>  <3DI enabled, scaler enabled>
         cap                         cap 
        /                           /
   scl_in_q                     scl_in_q
        \                           \
        scaler                       3di
        /                           /
   lcd_q                       scl_in_dup_q
        \                           \
         lcd                         scaler
                                    /
                                lcd_q
                                    \
                                      lcd
*/

#include "dvr_graph.h"
#include "disp_main.h"
#include "dvr_disp_api.h"
#include "util_cfg.h"
#include "dvr_mod_define.h"

//#define DEBUG_FLOW_RATE
#define SUPPORT_LV_SRC_WIN              0
#define AP_SPECIFY_CAP_BUF              1
#define SUPPORT_VGA_60FPS_LV            1
#define MAX_VIDEO_SOURCE_NUM            2

static ushort lcd_cnt = DVR_LCD_NUM;
static ushort plane_cnt = DVR_PLANE_NUM_PER_LCD;
static ushort dvr_disp_cascade_output_mode = 0;
static ushort dup_disp = FALSE;


module_param(lcd_cnt, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(lcd_cnt, "Number of LCDs");
module_param(plane_cnt, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(plane_cnt, "Number of planes of each LCD");
module_param(dvr_disp_cascade_output_mode, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(dvr_disp_cascade_output_mode, "Cascade output mode");
module_param(dup_disp, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(dup_disp, "Duplicate Display(1:TRUE/0:FALSE)");
static unsigned int vsrc = 1;
module_param(vsrc, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(vsrc, "Video Source number.");

struct semaphore    dvr_disp_sem;
QueMemCfg           default_disp1_cfg, default_disp2_cfg, default_lv_sclq_cfg, default_3di_sclq_cfg;
dvr_disp_data       disp_private_data[1];
int                 disp_share_scaler=FALSE;

QueMemCfg           default_osd_cfg;

static struct proc_dir_entry *dvr_disp_proc; 
static struct proc_dir_entry *proc_last_api;
static int last_api_num = 0;

int dvr_disp_dbgmode = 0;

#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_disp_dbgmode) printk(fmt, ## args);}

void dvr_add_or_modify_nodirty(int fd,int enable,int region_check,int x,int y,int w,int h);


static int proc_read_mode(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    printk("Debug Mode:%d (0:Quiet 1:Trace 2:Error 3:Dump)\n", dvr_disp_dbgmode);
    *eof = 1;       //end of file
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
    dvr_disp_dbgmode = tmp;
    printk("Debug Mode:%d\n", dvr_disp_dbgmode);
    return count;
}

static int proc_read_lastapi(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    switch (last_api_num)
    {
      case 1:  // DVR_DISP_INITIATE
      case 2:  // DVR_DISP_TERMINATE
        printk("No structure.\n");
        break;
      case 4:
        print_user_last_api_data("DISP:DVR_DISP_GET_DISP_PARAM", last_api_num, sizeof(dvr_disp_disp_param), (unsigned char*)&disp_private_data[0].la_disp_param_get, 0);
        break;
      case 5:
        print_user_last_api_data("DISP:DVR_DISP_SET_DISP_PARAM", last_api_num, sizeof(dvr_disp_disp_param), (unsigned char*)&disp_private_data[0].la_disp_param_set, 1);
        break;
      case 6:
        print_user_last_api_data("DISP:DVR_DISP_UPDATE_DISP_PARAM", last_api_num, sizeof(dvr_disp_update_disp_param), (unsigned char*)&disp_private_data[0].la_disp_param_update, 1);
        break;
      case 7:
        print_user_last_api_data("DISP:DVR_DISP_GET_PLANE_PARAM", last_api_num, sizeof(dvr_disp_plane_param), (unsigned char*)&disp_private_data[0].la_plane_param_get, 0);
        break;
      case 8:
        print_user_last_api_data("DISP:DVR_DISP_SET_PLANE_PARAM", last_api_num, sizeof(dvr_disp_plane_param), (unsigned char*)&disp_private_data[0].la_plane_param_set, 1);
        break;
      case 9:
        print_user_last_api_data("DISP:DVR_DISP_UPDATE_PLANE_PARAM", last_api_num, sizeof(dvr_disp_update_plane_param), (unsigned char*)&disp_private_data[0].la_plane_param_update, 1);
        break;
      case 10:
        print_user_last_api_data("DISP:DVR_DISP_CONTROL", last_api_num, sizeof(dvr_disp_control), (unsigned char*)&disp_private_data[0].la_disp_ctrl, 1);
        break;
      case 11:
        print_user_last_api_data("DISP:DVR_DISP_CLEAR_WIN", last_api_num, sizeof(dvr_disp_clear_param), (unsigned char*)&disp_private_data[0].la_disp_clear_param, 1);
        break;
      default:
        break;
    }
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

static unsigned int get_cap_input_ch(int ch_num)
{
    int input_ch = 0;

    if(vsrc > MAX_VIDEO_SOURCE_NUM || vsrc == 0) 
        panic("%s: Max video source number is %d, but vsrc=%d!\n", __FUNCTION__, MAX_VIDEO_SOURCE_NUM, vsrc);
        
    if(vsrc > 1) {
        /* ch_num=0/1 -> input_ch=0, ch_num=2,3->input_ch=1 */
        input_ch = ch_num/vsrc;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <vsrc=%d, ch_num=%d>\n",__FUNCTION__,__LINE__, vsrc, ch_num);
    return input_ch;
}

static int check_liveview_parameters(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    if(!disp_ctrl || !disp_data)
        return -1;


    if(disp_ctrl->src_param.lv.is_use_scaler)
    {
        if( (disp_ctrl->src_param.lv.dim.width<=0 && disp_ctrl->src_param.lv.dim.width!=GMVAL_DO_NOT_CARE)
          || (disp_ctrl->src_param.lv.dim.height<=0 && disp_ctrl->src_param.lv.dim.height!=GMVAL_DO_NOT_CARE) )
        {
            printk("Invalid Liveview source dim. WH(%d,%d)\n", disp_ctrl->src_param.lv.dim.width, disp_ctrl->src_param.lv.dim.height);
            return -1;
        }
#if SUPPORT_LV_SRC_WIN
        if(!disp_ctrl->src_param.lv.win.x && !disp_ctrl->src_param.lv.win.y && !disp_ctrl->src_param.lv.win.width && !disp_ctrl->src_param.lv.win.height)
        {
            disp_ctrl->src_param.lv.win.x = 0;
            disp_ctrl->src_param.lv.win.y = 0;
            disp_ctrl->src_param.lv.win.width = disp_ctrl->src_param.lv.dim.width;
            disp_ctrl->src_param.lv.win.height = disp_ctrl->src_param.lv.dim.height;
        }
        else if( ( (disp_ctrl->src_param.lv.win.x<0 || disp_ctrl->src_param.lv.win.x>=disp_ctrl->src_param.lv.dim.width) && disp_ctrl->src_param.lv.win.x!=GMVAL_DO_NOT_CARE)
          || ( (disp_ctrl->src_param.lv.win.y<0 || disp_ctrl->src_param.lv.win.y>=disp_ctrl->src_param.lv.dim.height) && disp_ctrl->src_param.lv.win.y!=GMVAL_DO_NOT_CARE) 
          || (disp_ctrl->src_param.lv.win.width<=0 && disp_ctrl->src_param.lv.win.width!=GMVAL_DO_NOT_CARE)
          || (disp_ctrl->src_param.lv.win.height<=0 && disp_ctrl->src_param.lv.win.height!=GMVAL_DO_NOT_CARE) )
        {
            printk("Invalid Liveview source win. XYWH(%d,%d,%d,%d)\n",
                disp_ctrl->src_param.lv.win.x, disp_ctrl->src_param.lv.win.y, disp_ctrl->src_param.lv.win.width, disp_ctrl->src_param.lv.win.height);
            return -1;
        }
#endif /* SUPPORT_LV_SRC_WIN */
    }
    return 0;
}

/*
 * @ update pip parameter for liveview pip feature 
 *
 * @ int update_pip_param(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
 * @ param disp_data is used to indicate structure dvr_disp_data.
 * @ param disp_ctrl is used to indicate structure dvr_disp_control. 
 */
#define PIP_SCALER_NUM 4
#define OSD_SCALER_NUM 5
int update_pip_param(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int scl2_fd;
    FuncTag tag, *ptag;
    dvr_disp_control *disp_ctrl_new;
    
    disp_ctrl_new = &disp_data->lv_info[disp_ctrl->channel].disp_ctrl_params;
    
    scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(PIP_SCALER_NUM));
    
    ptag = &tag;
    FN_RESET_TAG(ptag);    
    FN_SET_LV_CH(ptag, disp_ctrl->channel);
    // PIP mode doesn't support to update the following parameters
    if(disp_ctrl->src_param.lv.di_mode != disp_ctrl_new->src_param.lv.di_mode) {
        if(disp_ctrl->src_param.lv.di_mode != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.di_mode parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.mode != disp_ctrl_new->src_param.lv.mode) {
        if(disp_ctrl->src_param.lv.mode != GMVAL_DO_NOT_CARE) { 
            printk("PIP mode does not support update lv.mode parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.dma_order != disp_ctrl_new->src_param.lv.dma_order) {
        if(disp_ctrl->src_param.lv.dma_order != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.dma_order parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scale_indep != disp_ctrl_new->src_param.lv.scale_indep) {
        if(disp_ctrl->src_param.lv.scale_indep!=GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.scale_indep parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.input_system != disp_ctrl_new->src_param.lv.input_system) {
        if(disp_ctrl->src_param.lv.input_system != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.input_system parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.cap_rate != disp_ctrl_new->src_param.lv.cap_rate) {
        if(disp_ctrl->src_param.lv.cap_rate != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.cap_rate parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.color_mode != disp_ctrl_new->src_param.lv.color_mode) {
        if(disp_ctrl->src_param.lv.color_mode != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.color_mode parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.is_use_scaler != disp_ctrl_new->src_param.lv.is_use_scaler) {
        if(disp_ctrl->src_param.lv.is_use_scaler != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.is_use_scaler parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.dim.width != disp_ctrl_new->src_param.lv.dim.width) {
        if(disp_ctrl->src_param.lv.dim.width != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.dim.width parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.dim.height != disp_ctrl_new->src_param.lv.dim.height) {
        if(disp_ctrl->src_param.lv.dim.height != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update lv.dim.height parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.win.x != disp_ctrl_new->src_param.lv.win.x) {
        if(disp_ctrl->src_param.lv.win.x != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update src lv.win.x parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.win.y != disp_ctrl_new->src_param.lv.win.y) {
        if(disp_ctrl->src_param.lv.win.y != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update src lv.win.y parameter\n");
            return -1;
        }
    }    
    if(disp_ctrl->src_param.lv.win.width != disp_ctrl_new->src_param.lv.win.width) {
        if(disp_ctrl->src_param.lv.win.width != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update src lv.win.width parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.win.height != disp_ctrl_new->src_param.lv.win.height) {
        if(disp_ctrl->src_param.lv.win.height != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update src lv.win.height parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.is_3DI != disp_ctrl_new->src_param.lv.vp_param.is_3DI) {
        if(disp_ctrl->src_param.lv.vp_param.is_3DI != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update is_3DI parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.is_denoise != disp_ctrl_new->src_param.lv.vp_param.is_denoise) {
        if(disp_ctrl->src_param.lv.vp_param.is_denoise != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update is_denoise parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.denoise_mode != disp_ctrl_new->src_param.lv.vp_param.denoise_mode) {
        if(disp_ctrl->src_param.lv.vp_param.denoise_mode != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update denoise_mode parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.src_fmt != disp_ctrl_new->src_param.lv.scl_param.src_fmt) {
        if(disp_ctrl->src_param.lv.scl_param.src_fmt != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update src_fmt parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.dst_fmt != disp_ctrl_new->src_param.lv.scl_param.dst_fmt) {
        if(disp_ctrl->src_param.lv.scl_param.dst_fmt != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update dst_fmt parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.scale_mode != disp_ctrl_new->src_param.lv.scl_param.scale_mode) {
        if(disp_ctrl->src_param.lv.scl_param.scale_mode != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update scale_mode parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_dither != disp_ctrl_new->src_param.lv.scl_param.is_dither) {
        if(disp_ctrl->src_param.lv.scl_param.is_dither != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update is_dither parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_correction != disp_ctrl_new->src_param.lv.scl_param.is_correction) {
        if(disp_ctrl->src_param.lv.scl_param.is_correction != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update is_correction parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_album != disp_ctrl_new->src_param.lv.scl_param.is_album) {
        if(disp_ctrl->src_param.lv.scl_param.is_album != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update is_album parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.des_level != disp_ctrl_new->src_param.lv.scl_param.des_level) {
        if(disp_ctrl->src_param.lv.scl_param.des_level != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update des_level parameter\n");
            return -1;
        }
    }
    if(disp_ctrl->src_param.lv.cap_buf_id != disp_ctrl_new->src_param.lv.cap_buf_id) {
        if(disp_ctrl->src_param.lv.cap_buf_id != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update cap_buf_id parameter\n");
            return -1;
        }            
    }    
    if(disp_ctrl->dst_param.lv.plane_id != disp_ctrl_new->dst_param.lv.plane_id) {
        if(disp_ctrl->dst_param.lv.plane_id != GMVAL_DO_NOT_CARE) {
            printk("PIP mode does not support update plane_id parameter\n");
            return -1;
        }
    }
    /* PIP mode just allow to update the following 4 parameters
       win.x, win.y, win.width, win.height
    */
    if(disp_ctrl->dst_param.lv.win.x != disp_ctrl_new->dst_param.lv.win.x) {
        if(disp_ctrl->dst_param.lv.win.x != GMVAL_DO_NOT_CARE) 
            disp_ctrl_new->dst_param.lv.win.x = disp_ctrl->dst_param.lv.win.x;
    }
    if(disp_ctrl->dst_param.lv.win.y != disp_ctrl_new->dst_param.lv.win.y) {
        if(disp_ctrl->dst_param.lv.win.y != GMVAL_DO_NOT_CARE) 
            disp_ctrl_new->dst_param.lv.win.y = disp_ctrl->dst_param.lv.win.y;
    }
    if(disp_ctrl->dst_param.lv.win.width != disp_ctrl_new->dst_param.lv.win.width) {
        if(disp_ctrl->dst_param.lv.win.width != GMVAL_DO_NOT_CARE) 
            disp_ctrl_new->dst_param.lv.win.width = disp_ctrl->dst_param.lv.win.width;
    }
    if(disp_ctrl->dst_param.lv.win.height != disp_ctrl_new->dst_param.lv.win.height) {
        if(disp_ctrl->dst_param.lv.win.height != GMVAL_DO_NOT_CARE)
            disp_ctrl_new->dst_param.lv.win.height = disp_ctrl->dst_param.lv.win.height;        
    }
                    
    dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_x"), disp_ctrl->dst_param.lv.win.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_y"), disp_ctrl->dst_param.lv.win.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_width"), disp_ctrl->dst_param.lv.win.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_height"), disp_ctrl->dst_param.lv.win.height, 1);
    
    return 0;
}

/*
 * @ setup pip parameter for liveview pip feature 
 *
 * @ int setup_pip_parameter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl, int disable_scaler2)
 * @ param disp_data is used to indicate structure dvr_disp_data.
 * @ param disp_ctrl is used to indicate structure dvr_disp_control. 
 * @ param disable_scaler2 is used to indicate to disable scaler2 or not. 
 */
int setup_pip_parameter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl, int disable_scaler2)
{    
    int cap_fd, disp_fd, scl2_fd, scl_fd;
    int i, disp_num, lcd_width = 0, lcd_height = 0;
    FuncTag tag, *ptag;
    DispParam_Ext1 *disp_param_ext;    
    dvr_disp_control *disp_ctrl_pip;
    disp_ctrl_pip = disp_ctrl;        
    printk("setup_pip_parameter\n");
    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.lv.plane_id);
    
    disp_param_ext = &disp_data->lv_info[disp_ctrl->channel].disp_ext;
    
    if(DVR_DISP_CHECK_MAGIC(disp_ctrl->src_param.lv.ext_size) && DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) && disp_ctrl->src_param.lv.pext_data){    
        if(DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) <= sizeof(DispParam_Ext1)){
            if(copy_from_user((void *)disp_param_ext, disp_ctrl->src_param.lv.pext_data, DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size))) {
                printk("%s: copy_from_user failed!\n", __FUNCTION__);    
                return -1;
            }
        }
    }    
    
    switch (disp_ctrl->channel)
    {
        case 0:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 0));
                break;
        case 1:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
                break;
        case 2:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 0));
                break;
        case 3:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 1));
                break;
        default: 
                printk("%s:%d <error cap_fd ch_num=%d>\n",__FUNCTION__,__LINE__,disp_ctrl->channel);                                
                return -EFAULT;                                
    }
    
    for(i = 0; i < lcd_cnt; i++) {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));
        lcd_width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
        lcd_height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);
    }
    
    scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(disp_ctrl->channel));
    scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(PIP_SCALER_NUM));
    
    ptag = &tag;
    FN_RESET_TAG(ptag);    
    FN_SET_LV_CH(ptag, disp_ctrl->channel);
        
    if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler == 0 && disp_data->lv_info[disp_ctrl->channel].is_link_3DI == TRUE) {        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "buf_offset"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_fmt"),       SCALE_YUV422, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_CbCr"), 1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_interleaved"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_fmt"),       SCALE_YUV422, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_CbCr"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_level"),     0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "album"),         1, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "non_linear"),    SCALE_LINEAR, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "dither"),        0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "correction"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "mem_src_pit"), disp_ctrl->src_param.lv.dim.width/2, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "enable_scale"), 0, 1);

        //coord.
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_height"), lcd_height, 1);

        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), lcd_height, 1);

        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_x"), disp_ctrl->dst_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_y"), disp_ctrl->dst_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_height"), disp_ctrl->dst_param.lv.win.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_height"), disp_ctrl->dst_param.lv.win.height, 1);
    }
    
    if(disp_data->lv_info[disp_ctrl->channel].is_set == 0 && disp_data->lv_info[disp_ctrl->channel].disp_ext.chn_type == DISP_LAYER1_CHN && disable_scaler2 == 1) 
        goto layer0;             
    
    //set capture background
    if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {    
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_width"), lcd_width, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_height"), (lcd_height + disp_ctrl->src_param.lv.dim.height), 1);        
    }
        
    //set capture destination
    if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {                
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_y"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "width"), disp_ctrl->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "height"), disp_ctrl->src_param.lv.dim.height, 1);
    }        
    
    for(i = 0; i < DVR_LIVEVIEW_CHANNEL_NUM; i++) {
        if(disp_data->lv_info[i].is_set == 1) {        
            if(disp_data->lv_info[i].disp_ext.chn_type == DISP_LAYER1_CHN) {                 
                memcpy(disp_ctrl_pip, &disp_data->lv_info[i].disp_ctrl_params, sizeof(dvr_disp_control));                                
                goto layer1;
            }            
        }
    }        
    
    if(disp_param_ext->chn_type == DISP_LAYER0_CHN)
        goto layer0;
    else   //DISP_LAYER1_CHN
        goto layer1;
    
layer0:                        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "disable_scale"), 0, 1);                
        return 0;    
layer1:                            
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "buf_offset"), 0, 1);       
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_fmt"),       disp_ctrl_pip->src_param.lv.scl_param.src_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_CbCr"), 1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_interleaved"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_fmt"),       disp_ctrl_pip->src_param.lv.scl_param.dst_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_CbCr"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_level"),     disp_ctrl_pip->src_param.lv.scl_param.des_level, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "album"),         disp_ctrl_pip->src_param.lv.scl_param.is_album, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "non_linear"),    disp_ctrl_pip->src_param.lv.scl_param.scale_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "dither"),        disp_ctrl_pip->src_param.lv.scl_param.is_dither, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "correction"),    disp_ctrl_pip->src_param.lv.scl_param.is_correction, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "mem_src_pit"), disp_ctrl_pip->src_param.lv.dim.width/2, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "enable_scale"), 0, 1);

        //coord.
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_width"), disp_ctrl_pip->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_height"), disp_ctrl_pip->src_param.lv.dim.height, 1);
        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_y"), lcd_height, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_width"), disp_ctrl_pip->src_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_height"), disp_ctrl_pip->src_param.lv.win.height, 1);
        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_x"), disp_ctrl_pip->dst_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_y"), disp_ctrl_pip->dst_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_width"), disp_ctrl_pip->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_height"), disp_ctrl_pip->dst_param.lv.win.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_width"), disp_ctrl_pip->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_height"), disp_ctrl_pip->dst_param.lv.win.height, 1);                        
        memcpy(disp_ctrl, &disp_data->la_disp_ctrl, sizeof(dvr_disp_control));    
        return 0;
}

/*
 * @ transform user virtual address to physical address if AP use mmap from frammap
 *
 * @ unsigned int dvr_user_va_to_pa(unsigned int addr)
 * @ param addr is used to indicate virtual address from user space.
 */
unsigned int dvr_user_va_to_pa(unsigned int addr)
{
    pmd_t *pmd;
    pte_t *pte;
    pgd_t *pgd;
    pgd = pgd_offset(current->mm,addr);
    if(!pgd_none(*pgd))
    {
        pmd = pmd_offset(pgd, addr);
        if (!pmd_none(*pmd))
        {
            pte = pte_offset_map(pmd, addr);
            if(!pmd_none(*pmd))
            {
                pte = pte_offset_map(pmd, addr);
                if(pte_present(*pte))
                    return (unsigned int) page_address(pte_page(*pte)) + (addr & (PAGE_SIZE - 1)) - PAGE_OFFSET;
            }
        }
    }
    return 0;
}

/*
 * @ setup osd parameter for liveview osd feature 
 *
 * @ int setup_osd_parameter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl, int disable_scaler2)
 * @ param disp_data is used to indicate structure dvr_disp_data.
 * @ param disp_ctrl is used to indicate structure dvr_disp_control. 
 * @ param disable_scaler2 is used to indicate to disable scaler2 or not. 
 */
int setup_osd_parameter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl, int disable_scaler2)
{
    int val;
    int i, disp_num, lcd_width = 0, lcd_height = 0;
    int osd_fd, scl_fd, disp_fd;
    int scl3_fd, scl2_fd, cap_fd;
    unsigned int obj_pa;
    FuncTag tag, *ptag;
    DispParam_Ext1 *disp_param_ext;
    dvr_disp_control *disp_ctrl_pip;
    disp_ctrl_pip = disp_ctrl;        
    
    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.lv.plane_id);
    disp_param_ext = &disp_data->lv_info[disp_ctrl->channel].disp_ext;
    
    switch (disp_ctrl->channel)
    {
        case 0:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 0));
                break;
        case 1:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
                break;
        case 2:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 0));
                break;
        case 3:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 1));
                break;
        default: 
                printk("%s:%d <error cap_fd ch_num=%d>\n",__FUNCTION__,__LINE__,disp_ctrl->channel);                                
                return -EFAULT;                                
    }    
    
    scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(disp_ctrl->channel));
    osd_fd = ENTITY_FD(OSD_FD_MAJOR, OSD_FD_MINOR(0));
    scl3_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(OSD_SCALER_NUM));
    scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(PIP_SCALER_NUM));
    
    for(i = 0; i < lcd_cnt; i++) {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));
        lcd_width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
        lcd_height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);
    }
    
    ptag = &tag;
    FN_RESET_TAG(ptag);    
    FN_SET_LV_CH(ptag, disp_ctrl->channel);
    
    if(disp_data->lv_info[disp_ctrl->channel].is_set == 0 && disp_data->lv_info[disp_ctrl->channel].disp_ext.chn_type == DISP_LAYER1_CHN && disable_scaler2 == 1) 
        goto layer0;             
    
    if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
        obj_pa = dvr_user_va_to_pa(disp_param_ext->osd_setting.obj_va);    
        val = dvr_graph_get_property(disp_data->graph, osd_fd, PROPID(osd_fd, "VGID_OSD_End"), 1);
	    if(val != 0) {
		    printk("osd %x is not ready to set\n", osd_fd);
		    return -EFAULT;
	    }        
        
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_bg_W"), disp_param_ext->osd_setting.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_bg_H"), disp_param_ext->osd_setting.dim.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_x"), disp_param_ext->osd_setting.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_y"), disp_param_ext->osd_setting.win.y, 1);    
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_W"), disp_param_ext->osd_setting.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_H"), disp_param_ext->osd_setting.win.height, 1);    
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_va"), disp_param_ext->osd_setting.obj_va, 1);    
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_obj_pa"), obj_pa, 1);    
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_Alpha"), (disp_param_ext->osd_setting.alpha & 0xFF), 1);
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_magic_number"), disp_param_ext->osd_setting.magic_number, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, osd_fd, PROPID(osd_fd, "VGID_OSD_End"), 1, 1);
    }

    if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler == 0 && disp_data->lv_info[disp_ctrl->channel].is_link_3DI == TRUE) {
        //printk("set scaler for 3di with no scaler osd path\n");
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "buf_offset"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_fmt"),       SCALE_YUV422, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_CbCr"), 1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_interleaved"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_fmt"),       SCALE_YUV422, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_CbCr"), 0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_level"),     0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "album"),         1, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "non_linear"),    SCALE_LINEAR, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "dither"),        0, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "correction"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "mem_src_pit"), disp_ctrl->src_param.lv.dim.width/2, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "enable_scale"), 0, 1);

        //coord.
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_height"), lcd_height, 1);

        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), lcd_height, 1);

        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_x"), disp_ctrl->dst_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_y"), disp_ctrl->dst_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_height"), disp_ctrl->dst_param.lv.win.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_height"), disp_ctrl->dst_param.lv.win.height, 1);
    }

    //printk("set fake scaler8 for osd path\n");
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "buf_offset"), 0, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_fmt"),       SCALE_YUV422, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_swap_Y"),    0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_swap_CbCr"), 1, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_interleaved"), 0, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_fmt"),       SCALE_YUV422, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_swap_Y"),    0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_swap_CbCr"), 0, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_level"),     0, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "album"),         1, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "non_linear"),    SCALE_LINEAR, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "dither"),        0, 1);        
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "correction"),    0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "mem_src_pit"), lcd_width/2, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "enable_scale"), 0, 1);

    //coord.
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "vp_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "vp_height"), lcd_height, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "src_height"), lcd_height, 1);

    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "swc_axis_x"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "swc_axis_y"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "swc_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "swc_height"), lcd_height, 1);
    
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_height"), lcd_height, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_axis_x"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "des_axis_y"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "target_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "target_height"), lcd_height, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "crop_axis_x"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "crop_axis_y"), 0, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "crop_width"), lcd_width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, scl3_fd, PROPID(scl3_fd, "crop_height"), lcd_height, 1);    
                                              
    if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {    
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_width"), lcd_width, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_height"), (lcd_height + disp_ctrl->src_param.lv.dim.height), 1);        
    }
        
    //set capture destination
    if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {                
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_y"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "width"), disp_ctrl->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "height"), disp_ctrl->src_param.lv.dim.height, 1);
    }
    
    for(i = 0; i < DVR_LIVEVIEW_CHANNEL_NUM; i++) {
        if(disp_data->lv_info[i].is_set == 1) {        
            if(disp_data->lv_info[i].disp_ext.chn_type == DISP_LAYER1_CHN) {                 
                memcpy(disp_ctrl_pip, &disp_data->lv_info[i].disp_ctrl_params, sizeof(dvr_disp_control));                                
                goto layer1;
            }            
        }
    }
    
    if(disp_param_ext->chn_type == DISP_LAYER0_CHN)        
        goto layer0;        
    else 
        goto layer1;
    
layer0:                        
        //printk("layer1 closed, disable scaler2\n");
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "disable_scale"), 0, 1);                
        video_set_disable_mode(scl2_fd, DISABLE_WITH_FINISH); 
        return 0;            
    
layer1:
        //printk("set scaler4 for layer1\n");        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "buf_offset"), 0, 1);       
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_fmt"),       disp_ctrl_pip->src_param.lv.scl_param.src_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_CbCr"), 1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_interleaved"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_fmt"),       disp_ctrl_pip->src_param.lv.scl_param.dst_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_CbCr"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_level"),     disp_ctrl_pip->src_param.lv.scl_param.des_level, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "album"),         disp_ctrl_pip->src_param.lv.scl_param.is_album, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "non_linear"),    disp_ctrl_pip->src_param.lv.scl_param.scale_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "dither"),        disp_ctrl_pip->src_param.lv.scl_param.is_dither, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "correction"),    disp_ctrl_pip->src_param.lv.scl_param.is_correction, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "mem_src_pit"), disp_ctrl_pip->src_param.lv.dim.width/2, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "enable_scale"), 0, 1);

        //coord.
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_width"), disp_ctrl_pip->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "src_height"), disp_ctrl_pip->src_param.lv.dim.height, 1);
    
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_y"), lcd_height, 1);        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_width"), disp_ctrl_pip->src_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_height"), disp_ctrl_pip->src_param.lv.win.height, 1);
        
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_x"), disp_ctrl_pip->dst_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_y"), disp_ctrl_pip->dst_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_width"), disp_ctrl_pip->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "target_height"), disp_ctrl_pip->dst_param.lv.win.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_width"), disp_ctrl_pip->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_height"), disp_ctrl_pip->dst_param.lv.win.height, 1);   
        memcpy(disp_ctrl, &disp_data->la_disp_ctrl, sizeof(dvr_disp_control));    
    
        return 0;    
}

int setup_liveview_paramerter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int i, disp_num, plane_num, val=1;
    int cap_fd, disp_fd, di_fd, scl_fd,scl2_fd;
    int cap_rate, lcd_width=0, lcd_height=0;
    int y_offset=0, align_height;
    FuncTag tag, *ptag;
    DispParam_Ext1 *disp_param_ext;
    
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.lv.plane_id);
    plane_num = GET_PLANE_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.lv.plane_id);
//++ stanley add for pip feature    
    disp_param_ext = &disp_data->lv_info[disp_ctrl->channel].disp_ext;
    
    if(DVR_DISP_CHECK_MAGIC(disp_ctrl->src_param.lv.ext_size) && DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) && disp_ctrl->src_param.lv.pext_data){      
        if(DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) <= sizeof(DispParam_Ext1)){
            if(copy_from_user((void *)disp_param_ext, disp_ctrl->src_param.lv.pext_data, DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size))) {
                printk("%s: copy_from_user failed!\n", __FUNCTION__);    
                return -1;
            }
        }
    }
//--
//++ fullhd
#if 1    
    switch (disp_ctrl->channel)
    {
        case 0:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 0));
                break;
        case 1:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
                break;
        case 2:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 0));
                break;
        case 3:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 1));
                break;
        default: 
                printk("%s:%d <error cap_fd ch_num=%d>\n",__FUNCTION__,__LINE__,disp_ctrl->channel);                                
                return -EFAULT;                                
    }
printk("%s:%d <cap_fd=%x>\n",__FUNCTION__,__LINE__, cap_fd);
#else
    cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(disp_ctrl->channel, 0));
#endif
//-- fullhd
    di_fd = ENTITY_FD(GM3DI_FD_MAJOR, GM3DI_FD_MINOR_LV(disp_ctrl->channel));
    scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(disp_ctrl->channel));
//++ stanley add for pip feature
    scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(PIP_SCALER_NUM));
//--    
    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));
        lcd_width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
        lcd_height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);
    }
    
    if(check_liveview_parameters(disp_data, disp_ctrl)<0)
        return -1;

    //tag = TAG_CAP_DISP+disp_ctrl->channel;
    ptag = &tag;
    FN_RESET_TAG(ptag);    
    FN_SET_LV_CH(ptag, disp_ctrl->channel);
    disp_data->lv_info[disp_ctrl->channel].is_use_scaler = FALSE;
    if(disp_ctrl->src_param.lv.is_use_scaler)
        disp_data->lv_info[disp_ctrl->channel].is_use_scaler = TRUE;

    disp_data->lv_info[disp_ctrl->channel].is_link_3DI = FALSE;
    if(disp_ctrl->src_param.lv.vp_param.is_3DI)
    {
        if(disp_ctrl->src_param.lv.di_mode != LVFRAME_GM3DI_FORMAT)
        {
            printk("[dvr_disp] Enable 3DI, but di_mode is not LVFRAME_GM3DI_FORMAT.\n");
            return -1;
        }
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {
            align_height = ((disp_ctrl->src_param.lv.dim.height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
            y_offset = disp_ctrl->src_param.lv.dim.width * align_height * 2;
        }
        else
        {
            align_height = ((lcd_height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
            y_offset = lcd_width * align_height * 2;
        }
        disp_data->lv_info[disp_ctrl->channel].is_link_3DI = TRUE;
    }
    else
    {
        if(disp_ctrl->src_param.lv.di_mode==LVFRAME_EVEN_ODD)
        {
            align_height = ((disp_ctrl->src_param.lv.dim.height+31)/32)*32;
            y_offset = disp_ctrl->src_param.lv.dim.width * align_height;
        }
        else
            y_offset = 0;
    }

    if(disp_ctrl->src_param.lv.vp_param.is_denoise)
        disp_data->lv_info[disp_ctrl->channel].is_link_3DI = TRUE;

    //LCD target    
    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "videograph_plane"), plane_num, 1);
    }

    //Capture
    VDBGPRINT(DBG_DVR_FNC, "%s:%d ch=%d, disp_param_ext->chn_type=%d\n", __FUNCTION__, __LINE__, disp_ctrl->channel, disp_param_ext->chn_type);
    if(disp_param_ext->chn_type != DISP_WITH_ENC) {                                
        cap_rate=disp_ctrl->src_param.lv.cap_rate;

        if(disp_ctrl->src_param.lv.input_system==MCP_VIDEO_NTSC||disp_ctrl->src_param.lv.input_system==MCP_VIDEO_VGA)
        {

            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), 30, 1);//90
           dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Numerator"),disp_ctrl->src_param.lv.cap_rate, 1);//89
            if(cap_rate>30)
                cap_rate=30;
        }
        else
        {
   
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), 25, 1);//90
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Numerator"),disp_ctrl->src_param.lv.cap_rate, 1);//74
            if(cap_rate>25)
                cap_rate=25;
						
        }

        //capture background    
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {        
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_width"), disp_ctrl->src_param.lv.dim.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_height"), disp_ctrl->src_param.lv.dim.height, 1);           
        }
        else
        {
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_width"), lcd_width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_height"), lcd_height, 1);
        }

        //capture mode
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "DI_mode"), disp_ctrl->src_param.lv.di_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "mode"), disp_ctrl->src_param.lv.mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "color"), disp_ctrl->src_param.lv.color_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "dma_order"), disp_ctrl->src_param.lv.dma_order, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "scaler_indep"), disp_ctrl->src_param.lv.scale_indep, 1);

//        if(disp_ctrl->src_param.lv.input_system==MCP_VIDEO_NTSC)
    //        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "input_mode"), 1, 1);
       // else
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "input_mode"), 1, 1);
        
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "input_ch"), get_cap_input_ch(disp_ctrl->src_param.lv.cap_path), 1);    
    /*  // Not implement //
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "src_x"), disp_ctrl->src_param.rect.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "src_y"), disp_ctrl->src_param.rect.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "src_width"), disp_ctrl->src_param.rect.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "src_height"), disp_ctrl->src_param.rect.height, 1);
    */

        //capture destination
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_y0"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_u0"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_v0"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_y1"), y_offset, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_u1"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "buf_v1"), 0, 1);
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_x"), 0, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_y"), 0, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "width"), disp_ctrl->src_param.lv.dim.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "height"), disp_ctrl->src_param.lv.dim.height, 1);
        }
        else
        {
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_x"), disp_ctrl->dst_param.lv.win.x, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "bg_y"), disp_ctrl->dst_param.lv.win.y, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "width"), disp_ctrl->dst_param.lv.win.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "height"), disp_ctrl->dst_param.lv.win.height, 1);
        }
        //capture update parameter
        dvr_graph_set_property(disp_data->graph, ptag, cap_fd, PROPID(cap_fd, "updata"), 0, 1);
    }  //if(disp_param_ext->chn_type != DISP_WITH_ENC) {

    // Update parameters for 3DI
    if(disp_data->lv_info[disp_ctrl->channel].is_link_3DI)
    {
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {
            dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "width"), disp_ctrl->src_param.lv.dim.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "height"), disp_ctrl->src_param.lv.dim.height, 1);
        }
        else
        {
            dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "width"), lcd_width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "height"), lcd_height, 1);
        }
        dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "field_offset"), y_offset, 1);

        if(disp_ctrl->src_param.lv.vp_param.is_3DI && disp_ctrl->src_param.lv.vp_param.is_denoise)
            val = 1;
        else if(disp_ctrl->src_param.lv.vp_param.is_denoise)
            val = 2;
        dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "denoise_only"), val, 1);
        dvr_graph_set_property(disp_data->graph, ptag, di_fd, PROPID(di_fd, "denoise_field_frame"), disp_ctrl->src_param.lv.vp_param.denoise_mode, 1);
    }
    
    // setup scaler
    if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
    {
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "buf_offset"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_fmt"),       disp_ctrl->src_param.lv.scl_param.src_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_CbCr"), 1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_interleaved"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_fmt"),       disp_ctrl->src_param.lv.scl_param.dst_fmt, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_Y"),    0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_CbCr"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_level"),     disp_ctrl->src_param.lv.scl_param.des_level, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "album"),         disp_ctrl->src_param.lv.scl_param.is_album, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "non_linear"),    disp_ctrl->src_param.lv.scl_param.scale_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "dither"),        disp_ctrl->src_param.lv.scl_param.is_dither, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "correction"),    disp_ctrl->src_param.lv.scl_param.is_correction, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "mem_src_pit"), disp_ctrl->src_param.lv.dim.width/2, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "enable_scale"), 0, 1);

        //coord.
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_width"), disp_ctrl->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "vp_height"), disp_ctrl->src_param.lv.dim.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_width"), disp_ctrl->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "src_height"), disp_ctrl->src_param.lv.dim.height, 1);
#if SUPPORT_LV_SRC_WIN
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), disp_ctrl->src_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), disp_ctrl->src_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), disp_ctrl->src_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), disp_ctrl->src_param.lv.win.height, 1);
#else /* SUPPORT_LV_SRC_WIN */
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), disp_ctrl->src_param.lv.dim.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), disp_ctrl->src_param.lv.dim.height, 1);
#endif /* SUPPORT_LV_SRC_WIN */
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_width"), lcd_width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_height"), lcd_height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_x"), disp_ctrl->dst_param.lv.win.x, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_y"), disp_ctrl->dst_param.lv.win.y, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "target_height"), disp_ctrl->dst_param.lv.win.height, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_x"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_y"), 0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_width"), disp_ctrl->dst_param.lv.win.width, 1);
        dvr_graph_set_property(disp_data->graph, ptag, scl_fd, PROPID(scl_fd, "crop_height"), disp_ctrl->dst_param.lv.win.height, 1);

        dvr_add_or_modify_nodirty(scl_fd,1,1,disp_ctrl->dst_param.lv.win.x,disp_ctrl->dst_param.lv.win.y
            ,disp_ctrl->dst_param.lv.win.width,disp_ctrl->dst_param.lv.win.height);
//++ stanley add for pip feature
        //if(disp_param_ext->chn_type == DISP_LAYER0_CHN || disp_param_ext->chn_type == DISP_LAYER1_CHN)
            //setup_pip_parameter(disp_data, disp_ctrl, 0);
//--            
    }
    if(disp_param_ext->chn_type == DISP_LAYER0_CHN || disp_param_ext->chn_type == DISP_LAYER1_CHN)
        if(disp_param_ext->osd_setting.enable != 1) 
            setup_pip_parameter(disp_data, disp_ctrl, 0);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);
    
    return 0;
}


static struct v_queue_t* get_cap_buf(dvr_disp_data *disp_data, QueueID id, int ch_num, int dup)
{
    struct v_queue_t* ret_que;

    if(dup)
    {
        switch(id)
        {
          case QID_3DI_SCL: ret_que=disp_data->di_scl_in_dup_qst[0].que;    break;
          case QID_LV_SCL:  ret_que=disp_data->scl_in_dup_qst[ch_num].que;    break;
          default: 
            printk("specify_cap_buf, Invalid parameter. id(%d) ch(%d) dup(%d)\n", id, ch_num, dup);
            panic("Wrong specify_cap_buf param2.");
        }
    }
    else
    {
        switch(id)
        {
          case QID_3DI_SCL: ret_que=disp_data->di_scl_in_qst[0].que;    break;
          case QID_LV_SCL:  ret_que=disp_data->scl_in_qst[ch_num].que;    break;
          default: 
            printk("specify_cap_buf, Invalid parameter. id(%d) ch(%d) dup(%d)\n", id, ch_num, dup);
            panic("Wrong specify_cap_buf param1.");
        }
    }
    return ret_que;
}


int set_liveview_start_graph(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int disp_num, ret;
    int cap_fd, disp_fd, di_fd, scl_fd, disp2_fd, osd_fd, scl2_fd=0;
    int scl3_fd;
    int cap_rate, display_rate=0;
    FuncTag tag, tag2, *ptag, *ptag2;
    struct v_queue_t    *lcd_q, *lcd_dup_q, *di_scl_in_q, *di_scl_dup_in_q, *scl_in_q;
    struct v_queue_t    *osd_q, *osd_dup_q;
    DispParam_Ext1 *disp_param_ext;    
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);
    
    ptag = &tag;
    ptag2 = &tag2;
    FN_RESET_TAG(ptag);
    FN_RESET_TAG(ptag2);
    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.lv.plane_id);
//++ stanley add for pip feature    
    disp_param_ext = &disp_data->lv_info[disp_ctrl->channel].disp_ext;
    
    if(DVR_DISP_CHECK_MAGIC(disp_ctrl->src_param.lv.ext_size) && DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) && disp_ctrl->src_param.lv.pext_data) {
        if(DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) <= sizeof(DispParam_Ext1)) {
            if(copy_from_user((void *)disp_param_ext, disp_ctrl->src_param.lv.pext_data, DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size))) {
                printk("%s: copy_from_user failed!\n", __FUNCTION__);    
                return -1;
            }
        }
    }                    
//--    
    switch (disp_ctrl->channel)
    {
        case 0:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 0));
                break;
        case 1:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
                break;
        case 2:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 0));
                break;
        case 3:
                cap_fd = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 1));
                break;
        default: 
                printk("%s:%d <error cap_fd ch_num=%d>\n",__FUNCTION__,__LINE__,disp_ctrl->channel);                                
                return -EFAULT;                                
    }
    VDBGPRINT(DBG_DVR_FNC, " %s:%d cap_fd=%x\n", __FUNCTION__, __LINE__, cap_fd);
    disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(disp_num));
    di_fd = ENTITY_FD(GM3DI_FD_MAJOR, GM3DI_FD_MINOR_LV(disp_ctrl->channel));
    scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(disp_ctrl->channel));
//++ stanley add for pip feature
    scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(PIP_SCALER_NUM));
//++ stanley add for osd feature            
    osd_fd = ENTITY_FD(OSD_FD_MAJOR, OSD_FD_MINOR(0));
    scl3_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_LV(OSD_SCALER_NUM));
//--    
    if(dup_disp)
        disp2_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(1));
    
    ret = setup_liveview_paramerter(disp_data, disp_ctrl);
    if(ret<0)
        return -1;
//++ stanley add for OSD feature                
    if(disp_param_ext->osd_setting.enable == 1) {
        ret = setup_osd_parameter(disp_data, disp_ctrl, 0);
        if(ret<0)
            return -1;
    }
//--    
    FN_SET_FUNC(ptag, FN_LCD_PARAM);
    
    cap_rate=disp_ctrl->src_param.lv.cap_rate;
#if 0
    display_rate=disp_data->lcd[disp_num].display_rate;
#else
    // In all case, the ratio of display to capture is always 1:1
    // normal case, 
    display_rate = cap_rate;
#endif

    //setup graph line
    FN_SET_LV_CH(ptag2, disp_ctrl->channel);
    
    if(disp_data->lv_info[disp_ctrl->channel].is_link_3DI)
    {
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {
            if((disp_ctrl->src_param.lv.cap_buf_id>>DVR_PARAM_MAGIC_SHIFT)==DVR_PARAM_MAGIC)
            {
                di_scl_in_q = get_cap_buf(disp_data, (disp_ctrl->src_param.lv.cap_buf_id&((1UL << DVR_PARAM_MAGIC_SHIFT)-1)), disp_ctrl->channel, FALSE);
                di_scl_dup_in_q = get_cap_buf(disp_data, (disp_ctrl->src_param.lv.cap_buf_id&((1UL << DVR_PARAM_MAGIC_SHIFT)-1)), disp_ctrl->channel, TRUE);
            }
            else
            {
                di_scl_in_q = disp_data->di_scl_in_qst[0].que;
                di_scl_dup_in_q = disp_data->di_scl_in_dup_qst[0].que;
            }
            
            lcd_q = disp_data->lcd_qst.que;

            lcd_dup_q = disp_data->lcd_dup_qst.que;    
            
            osd_q = disp_data->osd_qst.que;
            osd_dup_q = disp_data->osd_dup_qst.que;
            
            if(disp_param_ext->osd_setting.enable == 1) {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
                    //printk("cap0-->3di-->scl-->scl4-->scl8-->osd-->lcd, with scaler with 3di\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, osd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, display_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if (disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    panic("pip channel not support 3di\n");
                }
                else {  //NORMAL_CHN
                    //printk("cap-->3di-->scl-->osd-->lcd, with scaler with 3di\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
            }
            else {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {                                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    panic("pip channel not support 3di\n");                    
                }
                else {                                    
#if SUPPORT_VGA_60FPS_LV
                    if(disp_ctrl->src_param.lv.mode==LVFRAME_FIELD_MODE2){
                    if (!dvr_disp_cascade_output_mode)
                        printk("[DVR DISP] Can't support LVFRAME_FIELD_MODE2 mode when 3DI is enabled.\n");
            }
#endif /* SUPPORT_VGA_60FPS_LV */
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, disp_fd, display_rate, lcd_q);
                    if(dup_disp)
                        dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, disp2_fd, display_rate, lcd_q);
                    video_setfree(scl_fd, FREE_BY_PUT, 0);
                }
            }
        }
        else
        {
            if((disp_ctrl->src_param.lv.cap_buf_id>>DVR_PARAM_MAGIC_SHIFT)==DVR_PARAM_MAGIC)
            {
                di_scl_in_q = get_cap_buf(disp_data, (disp_ctrl->src_param.lv.cap_buf_id&((1UL << DVR_PARAM_MAGIC_SHIFT)-1)), disp_ctrl->channel, FALSE);
                di_scl_dup_in_q = get_cap_buf(disp_data, (disp_ctrl->src_param.lv.cap_buf_id&((1UL << DVR_PARAM_MAGIC_SHIFT)-1)), disp_ctrl->channel, TRUE);
            }
            else
            {
                di_scl_in_q = disp_data->di_scl_in_qst[0].que;
                di_scl_dup_in_q = disp_data->di_scl_in_dup_qst[0].que;
            }
            
            lcd_q = disp_data->lcd_qst.que;
            lcd_dup_q = disp_data->lcd_dup_qst.que;
            
            osd_q = disp_data->osd_qst.que;
            osd_dup_q = disp_data->osd_dup_qst.que;
            
            if(disp_param_ext->osd_setting.enable == 1) {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
                    //printk("cap0-->3di-->scl-->scl4-->scl8-->osd-->lcd, no scaler with 3di\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, osd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, display_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if (disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    panic("pip channel not support 3di\n");
                }
                else {  //NORMAL_CHN
                    //printk("cap-->3di-->osd-->lcd , no scaler with 3di\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
            }
            else {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, cap_rate, di_scl_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, cap_rate, scl_fd, cap_rate, di_scl_dup_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    panic("pip channel not support 3di\n");
                }
                else {                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, di_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, display_rate, disp_fd, display_rate, lcd_dup_q);
                }
                if(dup_disp)
                    dvr_graph_add_link(disp_data->graph, ptag2, di_fd, display_rate, disp2_fd, display_rate, lcd_dup_q);
            }                
        }
    }
    else
    {
        if(disp_data->lv_info[disp_ctrl->channel].is_use_scaler)
        {
            if((disp_ctrl->src_param.lv.cap_buf_id>>DVR_PARAM_MAGIC_SHIFT)==DVR_PARAM_MAGIC){
                scl_in_q = get_cap_buf(disp_data, (disp_ctrl->src_param.lv.cap_buf_id&((1UL << DVR_PARAM_MAGIC_SHIFT)-1)), disp_ctrl->channel, FALSE);
            }
            else
                scl_in_q = disp_data->scl_in_qst[disp_ctrl->channel].que;
            lcd_q = disp_data->lcd_qst.que;
//++ stanley add for pip feature
            lcd_dup_q = disp_data->lcd_dup_qst.que;
            
            osd_q = disp_data->osd_qst.que;
            osd_dup_q = disp_data->osd_dup_qst.que;
            
            if(disp_param_ext->osd_setting.enable == 1) {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
                    //printk("cap-->scl-->scl4-->scl8-->osd-->lcd layer0\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl_fd, cap_rate, scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, osd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, cap_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if (disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    //printk("cap-->scl4-->scl8-->osd-->lcd layer1\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, osd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, cap_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else {
                    //printk("cap-->scl-->osd-->lcd\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl_fd, cap_rate, scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, osd_fd, display_rate, lcd_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }                
            }
            else {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl_fd, cap_rate, scl_in_q);                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);                
                }
                else if(disp_param_ext->chn_type == DISP_WITH_ENC) {
                    struct v_queue_t    *enc_q;
                    dvr_graph   *enc_graph;
                    unsigned char tmp_str[32];
                    int size_tmp, cnt_tmp, ddr_tmp, ch_num=1, cap_fd_tmp;

                    VDBGPRINT(DBG_DVR_FNC, "%s:%d ch=%d, disp_param_ext->chn_type=%d\n", __FUNCTION__, __LINE__, disp_ctrl->channel, disp_param_ext->chn_type);
                    snprintf(tmp_str, 32, "%s%d", QNAME_ENC_IN, ch_num);
                    enc_q = dvr_queue_find_by_name(tmp_str, &size_tmp, &cnt_tmp, &ddr_tmp);
                    enc_graph = dvr_graph_find_by_id(GFID_ENC(1));
                    if(enc_graph == NULL) {
                        printk("%s:%d please turn on encoder first, ch=%d!\n",__FUNCTION__,__LINE__, disp_ctrl->channel);
                        return -1;
                    }
                    cap_fd_tmp = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
                    VDBGPRINT(DBG_DVR_FNC, "%s:%d <enc_graph=%p, %s, size=%d, cnt=%d, ddr=%d>\n",__FUNCTION__,__LINE__,
                                    enc_graph,tmp_str, size_tmp, cnt_tmp, ddr_tmp);
                    dvr_graph_add_link(enc_graph, ptag2, cap_fd_tmp, cap_rate, scl_fd, cap_rate, enc_q);
                    dvr_graph_add_link(enc_graph, ptag2, scl_fd, cap_rate, disp_fd, display_rate, lcd_q);                                    
                }
                else {  //  DISP_NORMAL_CHN     
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl_fd, cap_rate, scl_in_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, disp_fd, display_rate, lcd_q);                                    
                }
            }
//-- 
#if 0
#if SUPPORT_VGA_60FPS_LV
            if(disp_ctrl->src_param.lv.mode==LVFRAME_FIELD_MODE2)
                cap_rate*=2;    // LVFRAME_FIELD_MODE2 treats every one field as frame, so cap_rate is double.
            display_rate = cap_rate;
#endif /* SUPPORT_VGA_60FPS_LV */

            dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl_fd, cap_rate, scl_in_q);
            dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, disp_fd, display_rate, lcd_q);
            if(dup_disp)
                dvr_graph_add_link(disp_data->graph, ptag2, scl_fd, cap_rate, disp2_fd, display_rate, lcd_q);
            video_setfree(scl_fd, FREE_BY_PUT, 0);
#endif            
        }
        else
        {
#if SUPPORT_VGA_60FPS_LV
            if(disp_ctrl->src_param.lv.mode==LVFRAME_FIELD_MODE2)
                cap_rate*=2;    // LVFRAME_FIELD_MODE2 treats every one field as frame, so cap_rate is double.
            display_rate = cap_rate;
#endif /* SUPPORT_VGA_60FPS_LV */

            lcd_q = disp_data->lcd_qst.que;
//++ stanley add for pip feature
            lcd_dup_q = disp_data->lcd_dup_qst.que;
            
            osd_q = disp_data->osd_qst.que;
            osd_dup_q = disp_data->osd_dup_qst.que;
            
            if(disp_param_ext->osd_setting.enable == 1) {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
                    //printk("cap-->scl4-->scl8-->osd-->lcd layer0\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, osd_q);                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, display_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, cap_rate, lcd_q);                                
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {
                    //printk("cap-->scl4-->scl8-->osd-->lcd layer1\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, osd_q);                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, scl3_fd, display_rate, osd_dup_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, scl3_fd, cap_rate, osd_fd, cap_rate, lcd_q);                                
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else {
                    //printk("cap-->osd-->lcd\n");
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, osd_fd, display_rate, lcd_q);
                    dvr_graph_add_link(disp_data->graph, ptag2, osd_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }                
            }
            else {
                if(disp_param_ext->chn_type == DISP_LAYER0_CHN) {
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);
                }
                else if(disp_param_ext->chn_type == DISP_LAYER1_CHN) {                
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, scl2_fd, cap_rate, lcd_q);                                                
                    dvr_graph_add_link(disp_data->graph, ptag2, scl2_fd, cap_rate, disp_fd, display_rate, lcd_dup_q);                
                }
                else {  //  DISP_NORMAL_CHN                                    
                    dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, disp_fd, display_rate, lcd_q);
                    if(dup_disp)
                        dvr_graph_add_link(disp_data->graph, ptag2, cap_fd, cap_rate, disp2_fd, display_rate, lcd_q);
                }
            }
//--            
        }
    }
//++ stanley add for pip feature
    disp_data->lv_info[disp_ctrl->channel].is_set = 1;
    disp_data->lv_info[disp_ctrl->channel].disp_ext.chn_type = disp_param_ext->chn_type;    
//--    
    //keep current setting
    memcpy(&disp_data->lv_info[disp_ctrl->channel].disp_ctrl_params, disp_ctrl, sizeof(dvr_disp_control));

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}

int set_liveview_stop_graph(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    FuncTag tag, *ptag;
//++ fullhd
    DispParam_Ext1 *disp_param_ext;    
//-- fullhd

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);
    
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_LV_CH(ptag, disp_ctrl->channel);

    disp_param_ext = &disp_data->lv_info[disp_ctrl->channel].disp_ext;
    if(DVR_DISP_CHECK_MAGIC(disp_ctrl->src_param.lv.ext_size) && DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) && disp_ctrl->src_param.lv.pext_data) {
        if(DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size) <= sizeof(DispParam_Ext1)) {
            if(copy_from_user((void *)disp_param_ext, disp_ctrl->src_param.lv.pext_data, DVR_DISP_GET_VALUE(disp_ctrl->src_param.lv.ext_size))) {
                printk("%s: copy_from_user failed!\n", __FUNCTION__);    
                return -1;
            }
        }
    }                    

//++ fullhd
#if 1
{
    VDBGPRINT(DBG_DVR_FNC, "%s disp_param_ext->chn_type=%d\n", __FUNCTION__, disp_param_ext->chn_type);
    if(disp_param_ext->chn_type == DISP_WITH_ENC) {
        dvr_graph   *enc_graph;
        enc_graph = dvr_graph_find_by_id(GFID_ENC(1));
        dvr_graph_del_link(enc_graph, ptag);
    } else {
        dvr_graph_del_link(disp_data->graph, ptag);
    }
}
#else
    dvr_graph_del_link(disp_data->graph, ptag);
#endif
//-- fullhd

//++ stanley add for pip feature            
    disp_data->lv_info[disp_ctrl->channel].is_set = 0;    
    if(disp_data->lv_info[disp_ctrl->channel].disp_ext.chn_type == DISP_LAYER1_CHN) {
        if(disp_data->lv_info[disp_ctrl->channel].disp_ext.osd_setting.enable != 1) 
            setup_pip_parameter(disp_data, disp_ctrl, 1);
//++ stanley add for osd feature            
        if(disp_data->lv_info[disp_ctrl->channel].disp_ext.osd_setting.enable == 1) 
            setup_osd_parameter(disp_data, disp_ctrl, 1);
    }
//--    
    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}

int update_liveview_graph_param(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int ret=0;
    int need_restart=FALSE, need_update_param=FALSE;
    dvr_disp_control *disp_ctrl_new;
    
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

#if 1
    if(check_liveview_parameters(disp_data, disp_ctrl)<0)
        return -1;
    disp_ctrl_new = &disp_data->lv_info[disp_ctrl->channel].disp_ctrl_params;

    if(disp_ctrl->src_param.lv.di_mode!=disp_ctrl_new->src_param.lv.di_mode) {
        if(disp_ctrl->src_param.lv.di_mode!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.di_mode=disp_ctrl->src_param.lv.di_mode;
        }
    }
    if(disp_ctrl->src_param.lv.mode!=disp_ctrl_new->src_param.lv.mode) {
        if(disp_ctrl->src_param.lv.mode!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.mode=disp_ctrl->src_param.lv.mode;
        }
    }
    if(disp_ctrl->src_param.lv.dma_order!=disp_ctrl_new->src_param.lv.dma_order) {
        if(disp_ctrl->src_param.lv.dma_order!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.dma_order=disp_ctrl->src_param.lv.dma_order;
        }
    }
    if(disp_ctrl->src_param.lv.scale_indep!=disp_ctrl_new->src_param.lv.scale_indep) {
        if(disp_ctrl->src_param.lv.scale_indep!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.scale_indep=disp_ctrl->src_param.lv.scale_indep;
        }
    }
    if(disp_ctrl->src_param.lv.input_system!=disp_ctrl_new->src_param.lv.input_system) {
        if(disp_ctrl->src_param.lv.input_system!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.input_system=disp_ctrl->src_param.lv.input_system;
        }
    }
    if(disp_ctrl->src_param.lv.cap_rate!=disp_ctrl_new->src_param.lv.cap_rate) {
        if(disp_ctrl->src_param.lv.cap_rate!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.cap_rate=disp_ctrl->src_param.lv.cap_rate;
        }
    }
    if(disp_ctrl->src_param.lv.color_mode!=disp_ctrl_new->src_param.lv.color_mode) {
        if(disp_ctrl->src_param.lv.color_mode!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.color_mode=disp_ctrl->src_param.lv.color_mode;
        }
    }
    if(disp_ctrl->src_param.lv.is_use_scaler!=disp_ctrl_new->src_param.lv.is_use_scaler) {
        if(disp_ctrl->src_param.lv.is_use_scaler!=GMVAL_DO_NOT_CARE) {
            need_restart=TRUE;
            disp_ctrl_new->src_param.lv.is_use_scaler=disp_ctrl->src_param.lv.is_use_scaler;
        }
    }
    if(disp_ctrl->src_param.lv.dim.width!=disp_ctrl_new->src_param.lv.dim.width) {
        if(disp_ctrl->src_param.lv.dim.width!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.dim.width=disp_ctrl->src_param.lv.dim.width;
        }
    }
    if(disp_ctrl->src_param.lv.dim.height!=disp_ctrl_new->src_param.lv.dim.height) {
        if(disp_ctrl->src_param.lv.dim.height!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.dim.height=disp_ctrl->src_param.lv.dim.height;
        }
    }
    if(disp_ctrl->src_param.lv.win.x!=disp_ctrl_new->src_param.lv.win.x) {
        if(disp_ctrl->src_param.lv.win.x!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.win.x=disp_ctrl->src_param.lv.win.x;
        }
    }
    if(disp_ctrl->src_param.lv.win.y!=disp_ctrl_new->src_param.lv.win.y) {
        if(disp_ctrl->src_param.lv.win.y!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.win.y=disp_ctrl->src_param.lv.win.y;
        }
    }
    if(disp_ctrl->src_param.lv.win.width!=disp_ctrl_new->src_param.lv.win.width) {
        if(disp_ctrl->src_param.lv.win.width!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.win.width=disp_ctrl->src_param.lv.win.width;
        }
    }
    if(disp_ctrl->src_param.lv.win.height!=disp_ctrl_new->src_param.lv.win.height) {
        if(disp_ctrl->src_param.lv.win.height!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->src_param.lv.win.height=disp_ctrl->src_param.lv.win.height;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.is_3DI!=disp_ctrl_new->src_param.lv.vp_param.is_3DI) {
        if(disp_ctrl->src_param.lv.vp_param.is_3DI!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.vp_param.is_3DI=disp_ctrl->src_param.lv.vp_param.is_3DI;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.is_denoise!=disp_ctrl_new->src_param.lv.vp_param.is_denoise) {
        if(disp_ctrl->src_param.lv.vp_param.is_denoise!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.vp_param.is_denoise=disp_ctrl->src_param.lv.vp_param.is_denoise;
        }
    }
    if(disp_ctrl->src_param.lv.vp_param.denoise_mode!=disp_ctrl_new->src_param.lv.vp_param.denoise_mode) {
        if(disp_ctrl->src_param.lv.vp_param.denoise_mode!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.vp_param.denoise_mode=disp_ctrl->src_param.lv.vp_param.denoise_mode;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.src_fmt!=disp_ctrl_new->src_param.lv.scl_param.src_fmt) {
        if(disp_ctrl->src_param.lv.scl_param.src_fmt!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.src_fmt=disp_ctrl->src_param.lv.scl_param.src_fmt;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.dst_fmt!=disp_ctrl_new->src_param.lv.scl_param.dst_fmt) {
        if(disp_ctrl->src_param.lv.scl_param.dst_fmt!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.dst_fmt=disp_ctrl->src_param.lv.scl_param.dst_fmt;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.scale_mode!=disp_ctrl_new->src_param.lv.scl_param.scale_mode) {
        if(disp_ctrl->src_param.lv.scl_param.scale_mode!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.scale_mode=disp_ctrl->src_param.lv.scl_param.scale_mode;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_dither!=disp_ctrl_new->src_param.lv.scl_param.is_dither) {
        if(disp_ctrl->src_param.lv.scl_param.is_dither!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.is_dither=disp_ctrl->src_param.lv.scl_param.is_dither;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_correction!=disp_ctrl_new->src_param.lv.scl_param.is_correction) {
        if(disp_ctrl->src_param.lv.scl_param.is_correction!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.is_correction=disp_ctrl->src_param.lv.scl_param.is_correction;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.is_album!=disp_ctrl_new->src_param.lv.scl_param.is_album) {
        if(disp_ctrl->src_param.lv.scl_param.is_album!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.is_album=disp_ctrl->src_param.lv.scl_param.is_album;
        }
    }
    if(disp_ctrl->src_param.lv.scl_param.des_level!=disp_ctrl_new->src_param.lv.scl_param.des_level) {
        if(disp_ctrl->src_param.lv.scl_param.des_level!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.scl_param.des_level=disp_ctrl->src_param.lv.scl_param.des_level;
        }
    }
    if(disp_ctrl->src_param.lv.cap_buf_id!=disp_ctrl_new->src_param.lv.cap_buf_id) {
        if(disp_ctrl->src_param.lv.cap_buf_id!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->src_param.lv.cap_buf_id=disp_ctrl->src_param.lv.cap_buf_id;
        }
    }
    if(disp_ctrl->dst_param.lv.win.x!=disp_ctrl_new->dst_param.lv.win.x) {
        if(disp_ctrl->dst_param.lv.win.x!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->dst_param.lv.win.x=disp_ctrl->dst_param.lv.win.x;
        }
    }
    if(disp_ctrl->dst_param.lv.win.y!=disp_ctrl_new->dst_param.lv.win.y) {
        if(disp_ctrl->dst_param.lv.win.y!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->dst_param.lv.win.y=disp_ctrl->dst_param.lv.win.y;
        }
    }
    if(disp_ctrl->dst_param.lv.win.width!=disp_ctrl_new->dst_param.lv.win.width) {
        if(disp_ctrl->dst_param.lv.win.width!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->dst_param.lv.win.width=disp_ctrl->dst_param.lv.win.width;
        }
    }
    if(disp_ctrl->dst_param.lv.win.height!=disp_ctrl_new->dst_param.lv.win.height) {
        if(disp_ctrl->dst_param.lv.win.height!=GMVAL_DO_NOT_CARE) {
            need_update_param = TRUE;
            disp_ctrl_new->dst_param.lv.win.height=disp_ctrl->dst_param.lv.win.height;
        }
    }
    if(disp_ctrl->dst_param.lv.plane_id!=disp_ctrl_new->dst_param.lv.plane_id) {
        if(disp_ctrl->dst_param.lv.plane_id!=GMVAL_DO_NOT_CARE) {
            need_restart = TRUE;
            disp_ctrl_new->dst_param.lv.plane_id=disp_ctrl->dst_param.lv.plane_id;
        }
    }

    if(need_restart)
    {
        set_liveview_stop_graph(disp_data, disp_ctrl_new);
        ret = set_liveview_start_graph(disp_data, disp_ctrl_new);
    }
    else if(need_update_param)
    {        
        ret = setup_liveview_paramerter(disp_data, disp_ctrl_new);
    }
    else
        ret = 0;

#else
    set_liveview_stop_graph(disp_data, disp_ctrl);
    ret = set_liveview_start_graph(disp_data, disp_ctrl);
/*
    if( (disp_data->lv_info[disp_ctrl->channel].is_link_3DI && disp_ctrl->src_param.lv.di_mode!=LVFRAME_GM3DI_FORMAT)
        ||(!disp_data->lv_info[disp_ctrl->channel].is_link_3DI && disp_ctrl->src_param.lv.di_mode==LVFRAME_GM3DI_FORMAT) 
        ||(disp_ctrl->src_param.lv.mode==LVFRAME_FIELD_MODE2)
        || (disp_data->lv_info[disp_ctrl->channel].is_use_scaler != disp_ctrl->src_param.lv.is_use_scaler) )
    {
        set_liveview_stop_graph(disp_data, disp_ctrl);
        ret = set_liveview_start_graph(disp_data, disp_ctrl);
    }
    else
    {
        ret = setup_liveview_paramerter(disp_data, disp_ctrl);
    }
*/
#endif


    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);
    return ret;
}




#if 1
int setup_cascade_paramerter(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int disp_num, plane_num;
    int cascade_fd, disp_fd;
    int y_offset=0, lcd_width, lcd_height;
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.cas.plane_id);
    plane_num = GET_PLANE_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.cas.plane_id);

    cascade_fd = ENTITY_FD(CASCADE_FD_MAJOR, CASCADE_FD_MINOR(disp_ctrl->channel, disp_ctrl->dst_param.cas.path));
    disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(disp_num));
    lcd_width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
    lcd_height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);


    //tag = TAG_CASCADE_DISP+disp_ctrl->channel;
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_CAS_CH(ptag, disp_ctrl->channel);

    //setup LCD target    
    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "videograph_plane"), plane_num, 1);
//    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "framebuff_size"), disp_data->lcd_qst.size, 1);

    //background
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg_x"), disp_ctrl->dst_param.cas.bg_rect0.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg_y"), disp_ctrl->dst_param.cas.bg_rect0.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg_width"), disp_ctrl->dst_param.cas.bg_rect0.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg_height"), disp_ctrl->dst_param.cas.bg_rect0.height, 1);

    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg1_x"), disp_ctrl->dst_param.cas.bg_rect1.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg1_y"), disp_ctrl->dst_param.cas.bg_rect1.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg1_width"), disp_ctrl->dst_param.cas.bg_rect1.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bg1_height"), disp_ctrl->dst_param.cas.bg_rect1.height, 1);

    //source
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "DI_mode"), disp_ctrl->src_param.cas.di_mode, 1);
#ifdef ENABLE_3DI_DEMO
    disp_data->is_3DI = FALSE;
    if(disp_ctrl->src_param.cas.di_mode == LVFRAME_GM3DI_FORMAT)
    {
        align_height = ((lcd_height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
        y_offset = lcd_width * align_height * 2;
        disp_data->is_3DI = TRUE;
    } else
#endif
    {
        y_offset = 0;
    }
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "color"), disp_ctrl->src_param.cas.color_mode, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "dma_order"), disp_ctrl->src_param.cas.dma_order, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "scaler_indep"), disp_ctrl->src_param.cas.scale_indep, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "input_mode"), disp_ctrl->src_param.cas.input_system, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "input_ch"), 0, 1);
/*  // Not implement //
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "src_x"), disp_ctrl->src_param.rect.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "src_y"), disp_ctrl->src_param.rect.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "src_width"), disp_ctrl->src_param.rect.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "src_height"), disp_ctrl->src_param.rect.height, 1);
*/                                                
                                                  
    //destination
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bCrop"), 1, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "bCrop1"), 1, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop_x"), disp_ctrl->dst_param.cas.swc_rect0.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop_y"), disp_ctrl->dst_param.cas.swc_rect0.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop_w"), disp_ctrl->dst_param.cas.swc_rect0.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop_h"), disp_ctrl->dst_param.cas.swc_rect0.height, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop1_x"), disp_ctrl->dst_param.cas.swc_rect1.x, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop1_y"), disp_ctrl->dst_param.cas.swc_rect1.y, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop1_w"), disp_ctrl->dst_param.cas.swc_rect1.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "crop1_h"), disp_ctrl->dst_param.cas.swc_rect1.height, 1);

    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_y0"), (disp_ctrl->dst_param.cas.rect0.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect0.x)*2, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_u0"), (disp_ctrl->dst_param.cas.rect0.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect0.x), 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_v0"), (disp_ctrl->dst_param.cas.rect0.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect0.x), 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_y1"), (disp_ctrl->dst_param.cas.rect1.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect1.x)*2 + y_offset, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_u1"), (disp_ctrl->dst_param.cas.rect1.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect1.x), 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "buf_v1"), (disp_ctrl->dst_param.cas.rect1.y * disp_ctrl->src_param.cas.win.width + disp_ctrl->dst_param.cas.rect1.x), 1);
                                                  
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "width"), disp_ctrl->dst_param.cas.rect0.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "height"), disp_ctrl->dst_param.cas.rect0.height, 1);
                                                  
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "width1"), disp_ctrl->dst_param.cas.rect1.width, 1);
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "height1"), disp_ctrl->dst_param.cas.rect1.height, 1);
                                                  
    //update capture parameter
    dvr_graph_set_property(disp_data->graph, ptag, cascade_fd, PROPID(cascade_fd, "updata"), 0, 1);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);
    
    return 0;
}



int set_cascade_start_graph(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    int disp_num;
    int cascade_fd, disp_fd;
    int cap_rate, disp_rate;
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_ctrl->dst_param.cas.plane_id);
    cascade_fd = ENTITY_FD(CASCADE_FD_MAJOR, CASCADE_FD_MINOR(disp_ctrl->channel, disp_ctrl->dst_param.cas.path));
    disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(disp_num));
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_CAS_CH(ptag, disp_ctrl->channel);

    setup_cascade_paramerter(disp_data, disp_ctrl);
    
     if(disp_ctrl->src_param.cas.input_system == MCP_VIDEO_NTSC)
    {
        cap_rate=disp_rate=30;
    }
    else
    {
        cap_rate=disp_rate=25;
    }
    //setup graph line
    dvr_graph_add_link(disp_data->graph, ptag, cascade_fd, cap_rate, disp_fd, disp_rate, disp_data->lcd_qst.que);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}


int set_cascade_stop_graph(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_CAS_CH(ptag, disp_ctrl->channel);

    dvr_graph_del_link(disp_data->graph, ptag);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}

int update_cascade_graph_param(dvr_disp_data *disp_data, dvr_disp_control *disp_ctrl)
{
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    setup_cascade_paramerter(disp_data, disp_ctrl);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);
    return 0;
}
#endif

static int do_initialize(dvr_disp_data  *disp_data)
{
    int ret;
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    ret = dvr_graph_create(&disp_data->graph, GFID_DISP(0));
    if(ret<0)
        return -EFAULT;

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}



static int do_terminate(dvr_disp_data  *disp_data)
{
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    if(disp_data->graph)
    {
        dvr_graph_destroy(disp_data->graph);
        disp_data->graph=NULL;
    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}

static int do_get_disp_param(dvr_disp_data  *disp_data, dvr_disp_disp_param *disp_param_get)
{
    int i, disp_no, disp_fd;
    int val;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);
    disp_no = disp_param_get->disp_num;

    if(disp_no<0 || disp_no>lcd_cnt)
    {
        printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_no, lcd_cnt);
        return -1;
    }

    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_no)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        //general
        disp_param_get->target_id = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "tve100_source"), 1);
        disp_param_get->plane_comb = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "pip_mode"), 1);
        disp_param_get->output_system = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "tve100_ntsc_pal"), 1);
        disp_param_get->output_mode = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "output_fmt"), 1);
        //color attributes
        disp_param_get->color_attrib.brightness = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "brightness"), 1);
        disp_param_get->color_attrib.saturation = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "saturation"), 1);
        disp_param_get->color_attrib.contrast = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "contrast"), 1);
        disp_param_get->color_attrib.huesin = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "huesin"), 1);
        disp_param_get->color_attrib.huecos = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "huecos"), 1);
        disp_param_get->color_attrib.sharpnessk0 = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "sharpnessk0"), 1);
        disp_param_get->color_attrib.sharpnessk1 = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "sharpnessk1"), 1);
        disp_param_get->color_attrib.sharpness_thres0 = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "sharpness_thres0"), 1);
        disp_param_get->color_attrib.shaprness_thres1 = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "shaprness_thres1"), 1);
        //transparent color(color key)
        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "colorkey1_state"), 1);
        if(val == 1)
        {
            disp_param_get->transparent_color[0].is_enable = TRUE;
            disp_param_get->transparent_color[0].color = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "colorkey1"), 1);
        }
        else
        {
            disp_param_get->transparent_color[0].is_enable = FALSE;
        }
        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "colorkey2_state"), 1);
        if(val == 1)
        {
            disp_param_get->transparent_color[1].is_enable = TRUE;
            disp_param_get->transparent_color[1].color = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "colorkey2"), 1);
        }
        else
        {
            disp_param_get->transparent_color[1].is_enable = FALSE;
        }
        //vbi
        disp_param_get->vbi_info.filler = 0;
        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "vbi_state"), 1);
        if(val == 1)
        {
            disp_param_get->vbi_info.lineno = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "vbi_lineno"), 1);
            disp_param_get->vbi_info.lineheight = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "vbi_lineheight"), 1);
            disp_param_get->vbi_info.fb_offset = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "vbi_fb_offset"), 1);
            disp_param_get->vbi_info.fb_size = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "vbi_fb_size"), 1);
        }
        else
        {
            memset(&disp_param_get->vbi_info, 0x0, sizeof(dvr_disp_vbi_info));
        }
        //lcd scaler
        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "scalar_state"), 1);
        if(val == 1)
        {
            disp_param_get->scl_info.is_enable = TRUE;
            disp_param_get->scl_info.dim.width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "scalar_horout"), 1);
            disp_param_get->scl_info.dim.height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "scalar_verout"), 1);
        }
        else
        {
            disp_param_get->scl_info.is_enable = FALSE;
        }

        disp_param_get->dim.width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
        disp_param_get->dim.height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);

        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "input_res"), 1);
        switch(val)
        {
          case VIN_D1:          
            disp_param_get->res.input_res=LCD_RES_D1;   
            break;
          case VIN_800x600:
            disp_param_get->res.input_res=LCD_RES_SVGA;   
            break;  
          case VIN_1024x768:    
            disp_param_get->res.input_res=LCD_RES_XGA;  
            break;
          default:              
            disp_param_get->res.input_res=-1;           
            break;
        }
        
        val = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "output_type"), 1);
        switch(val)
        {
          case OUTPUT_TYPE_DAC_D1_0:         disp_param_get->res.output_type=LCD_RES_D1;       break;
          case OUTPUT_TYPE_VGA_800x600_15:   disp_param_get->res.output_type=LCD_RES_SVGA;     break;
          case OUTPUT_TYPE_VGA_1024x768_8:   disp_param_get->res.output_type=LCD_RES_XGA;      break;
          case OUTPUT_TYPE_VGA_1280x960_10:  disp_param_get->res.output_type=LCD_RES_XVGA;     break;
          case OUTPUT_TYPE_VGA_1280x1024_13: disp_param_get->res.output_type=LCD_RES_SXGA;     break;
          case OUTPUT_TYPE_VGA_1360x768_14:  disp_param_get->res.output_type=LCD_RES_1360x768; break;
          default:                           disp_param_get->res.output_type=-1;            break;
        }

        disp_data->lcd[i].plane_comb = disp_param_get->plane_comb;
        disp_data->lcd[i].out_system = disp_param_get->output_system;
        disp_data->lcd[i].out_format = disp_param_get->output_mode;
    
    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}


static int do_set_disp_param(dvr_disp_data  *disp_data, dvr_disp_disp_param *disp_param_set)
{
    int i, disp_no, disp_fd, ret=0;
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);
    disp_no = disp_param_set->disp_num;
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_FUNC(ptag, FN_LCD_PARAM);

    if(disp_no<0 || disp_no>lcd_cnt)
    {
        printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_no, lcd_cnt);
        return -1;
    }

    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_no)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        //vbi
        if(disp_param_set->vbi_info.filler == 1)
        {
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_state"), 1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_lineno"), disp_param_set->vbi_info.lineno, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_lineheight"), disp_param_set->vbi_info.lineheight, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_fb_offset"), disp_param_set->vbi_info.fb_offset, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_fb_size"), disp_param_set->vbi_info.fb_size, 1);
        }
        else
        {
    //TODO: non realtime case...
    /*        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_state"), 1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_lineno"), disp_param_set->vbi_info.lineno, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_lineheight"), disp_param_set->vbi_info.lineheight, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_fb_offset"), disp_param_set->vbi_info.fb_offset, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "vbi_fb_size"), disp_param_set->vbi_info.fb_size, 1);
    */
        }
        //lcd scaler
    /*
        if(disp_param_set->scl_info.is_enable == TRUE)
        {
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_state"), 1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_horin"), disp_data->lcd[disp_no].dim.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_verin"), disp_data->lcd[disp_no].dim.height, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_horout"), disp_param_set->scl_info.dim.width, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_verout"), disp_param_set->scl_info.dim.height, 1);
        }
        else
        {
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "scalar_state"), 0, 1);
        }
    */
        //color attributes
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "brightness"), disp_param_set->color_attrib.brightness, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "saturation"), disp_param_set->color_attrib.saturation, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "contrast"), disp_param_set->color_attrib.contrast, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "huesin"), disp_param_set->color_attrib.huesin, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "huecos"), disp_param_set->color_attrib.huecos, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpnessk0"), disp_param_set->color_attrib.sharpnessk0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpnessk1"), disp_param_set->color_attrib.sharpnessk1, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpness_thres0"), disp_param_set->color_attrib.sharpness_thres0, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "shaprness_thres1"), disp_param_set->color_attrib.shaprness_thres1, 1);

        //transparent color(color key)
        if(disp_param_set->transparent_color[0].is_enable)
        {
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1_state"), 1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1"), disp_param_set->transparent_color[0].color, 1);
        }
        else
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1_state"), 0, 1);

        if(disp_param_set->transparent_color[1].is_enable)
        {
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2_state"), 1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2"), disp_param_set->transparent_color[1].color, 1);
        }
        else
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2_state"), 0, 1);
        
        //general
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "pip_mode"), disp_param_set->plane_comb, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "output_fmt"), disp_param_set->output_mode, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "d1_ntsc_pal"), disp_param_set->output_system, 1);
        //dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "tve100_source"), disp_param_set->target_id, 1);
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "tve100_ntsc_pal"), disp_param_set->output_system, 1);

        ret = dvr_graph_apply_property(disp_data->graph, ptag);
        if(ret<0)
            printk("[dvr_disp] set display parameters failed(%d)\n", ret);
    }
    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return ret;
}


static int do_update_disp_param(dvr_disp_data  *disp_data, dvr_disp_update_disp_param *disp_param_update)
{
    int i, disp_no, disp_fd, ret=0, res;
    FuncTag tag, *ptag;
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_no = disp_param_update->disp_num;
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_FUNC(ptag, FN_LCD_PARAM);

    if( (disp_no<0 || disp_no>lcd_cnt) && (disp_param_update->param!=DISP_PARAM_APPLY))
    {
        printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_no, lcd_cnt);
        return -1;
    }

    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_no)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        switch(disp_param_update->param)
        {
    /*      case DISP_PARAM_TARGET:
            dvr_graph_set_property(disp_data->graph, TAG_LCD_SETTING, disp_fd, PROPID(disp_fd, "tve100_source"), disp_param_update->val.target_id, 1);
            break;*/
            
          case DISP_PARAM_PLANE_COMBINATION:
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "pip_mode"), disp_param_update->val.plane_comb, 1);
            disp_data->lcd[i].plane_comb = disp_param_update->val.plane_comb;
            break;
            
          case DISP_PARAM_OUTPUT_MODE:
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "output_fmt"), disp_param_update->val.output_mode, 1);
            disp_data->lcd[i].out_format = disp_param_update->val.output_mode;
            break;
            
          case DISP_PARAM_OUTPUT_SYSTEM:
            {
                int den_fps=30;
                if(disp_param_update->val.output_system!=MCP_VIDEO_VGA)
                {
                    int lcd_mode;
                    if(disp_param_update->val.output_system==MCP_VIDEO_NTSC)
                        lcd_mode=17; //NTSC
                    else
                        lcd_mode=16; //PAL
                    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "d1_ntsc_pal"),lcd_mode, 1);
                    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "tve100_ntsc_pal"),lcd_mode, 1);
                }
                
                disp_data->lcd[i].out_system = disp_param_update->val.output_system;
                disp_data->lcd[i].display_rate = disp_param_update->val.display_rate;
                switch(disp_data->lcd[i].out_system)
                {
                    case MCP_VIDEO_NTSC:
                        den_fps = 30; //ntsc
                        break;
                    case MCP_VIDEO_PAL:
                        den_fps = 25; //pal
                        break;
                    case MCP_VIDEO_VGA:
                    {
#if SUPPORT_VGA_60FPS_LV
                        int real_outsys = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "output_type"), 1);
                        if(real_outsys == OUTPUT_TYPE_DAC_D1_0)
                        {
                            int d1_mode = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "d1_ntsc_pal"), 1);
                            if(d1_mode == 17)   
                                den_fps = 30; //30Hz
                            else
                                den_fps = 25; //25Hz
                        }
                        else
                        {
                            if (dvr_disp_cascade_output_mode == 0)
                                den_fps = 60; //60Hz
                            else if (dvr_disp_cascade_output_mode == 1)                             
                                den_fps = 30; //30Hz
                            else
                                den_fps = 25; //25Hz
                        }
#else
                        den_fps = 30; //30Hz
#endif
                        break;
                    }
                    default:
                        printk("##### Error lcd %d output system %d\n",i,disp_data->lcd[i].out_system);
                        panic("out_system");
                }
                dvr_graph_set_property(disp_data->graph, ptag,disp_fd, PROPID(disp_fd, "FrameRate_Denominator"), den_fps, 1);
                dvr_graph_set_property(disp_data->graph, ptag,disp_fd, PROPID(disp_fd, "FrameRate_Numerator"), disp_data->lcd[i].display_rate, 1); //ntsc
            
                printk("##### Set display %d out_system %d, fps %d\n",i, disp_data->lcd[i].out_system, disp_data->lcd[i].display_rate);
                
                break;
            }        
          case DISP_PARAM_COLOR_ATTRIBUTE:
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "brightness"), disp_param_update->val.color_attrib.brightness, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "saturation"), disp_param_update->val.color_attrib.saturation, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "contrast"), disp_param_update->val.color_attrib.contrast, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "huesin"), disp_param_update->val.color_attrib.huesin, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "huecos"), disp_param_update->val.color_attrib.huecos, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpnessk0"), disp_param_update->val.color_attrib.sharpnessk0, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpnessk1"), disp_param_update->val.color_attrib.sharpnessk1, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "sharpness_thres0"), disp_param_update->val.color_attrib.sharpness_thres0, 1);
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "shaprness_thres1"), disp_param_update->val.color_attrib.shaprness_thres1, 1);
            break;
            
          case DISP_PARAM_TRANSPARENT_COLOR:
            if(disp_param_update->val.transparent_color[0].is_enable)
            {
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1_state"), 1, 1);
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1"), disp_param_update->val.transparent_color[0].color, 1);
            }
            else
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey1_state"), 0, 1);

            if(disp_param_update->val.transparent_color[1].is_enable)
            {
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2_state"), 1, 1);
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2"), disp_param_update->val.transparent_color[1].color, 1);
            }
            else
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "colorkey2_state"), 0, 1);
            break;

          case DISP_PARAM_RESOLUTION:
            if(disp_param_update->val.res.input_res!=GMVAL_DO_NOT_CARE)
            {
                switch(disp_param_update->val.res.input_res)
                {
                  case LCD_RES_D1:   res=VIN_D1;        break;
                  case LCD_RES_SVGA: res=VIN_800x600;   break;   
                  case LCD_RES_XGA:  res=VIN_1024x768;  break;              
                  default:           res=-1;            break;
                }
                if(res<0)
                    printk("LCD Can't support this input resolution. res(%d)\n", disp_param_update->val.res.input_res);
                else
                    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "input_res"), res, 1);
            }
            if(disp_param_update->val.res.output_type!=GMVAL_DO_NOT_CARE)
            {
                switch(disp_param_update->val.res.output_type)
                {
                  case LCD_RES_D1:        res=OUTPUT_TYPE_DAC_D1_0;            break;
                  case LCD_RES_SVGA:      res=OUTPUT_TYPE_VGA_800x600_15;      break;  
                  case LCD_RES_XGA:       res=OUTPUT_TYPE_VGA_1024x768_8;      break;
                  case LCD_RES_XVGA:      res=OUTPUT_TYPE_VGA_1280x960_10;     break;
                  case LCD_RES_SXGA:      res=OUTPUT_TYPE_VGA_1280x1024_13;    break;
                  case LCD_RES_1360x768:  res=OUTPUT_TYPE_VGA_1360x768_14;     break;
                  default:                res=-1;                              break;
                }
                if(res<0)
                    printk("LCD Can't support this output resolution. res(%d)\n", disp_param_update->val.res.output_type);
                else
                    dvr_graph_set_property(disp_data->graph, ptag, disp_fd, PROPID(disp_fd, "output_type"), res, 1);
            }
            break;

          case DISP_PARAM_APPLY:
            ret = dvr_graph_apply_property(disp_data->graph, ptag);
            if(ret<0)
                printk("[dvr_disp] update display parameters failed(%d)\n", ret);
            break;

          default:
            printk("This display pamameter(%d) is not supported!", disp_param_update->param);
            break;
        }
    }
    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return ret;
}


static int do_get_plane_param(dvr_disp_data  *disp_data, dvr_disp_plane_param *plane_param_get)
{
    int i, disp_num, plane_num, disp_fd, id=0;
    dvr_disp_plane_param_st      *p_data;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_num = plane_param_get->disp_num;
    plane_num = plane_param_get->plane_num;
    p_data = &plane_param_get->param;

    if(disp_num<0 || disp_num>lcd_cnt)
    {
        printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_num, lcd_cnt);
        return -1;
    }
    if(plane_num<0 || plane_num>plane_cnt)
    {
        printk("[dvr_disp] Incorrect plane number(%d) range(0~%d)\n", plane_num, plane_cnt);
        return -1;
    }

    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        p_data->plane_id = DVR_PLANE_ID(disp_num, plane_num);

        if(plane_num==0)
        {
            p_data->win.x = 0;
            p_data->win.y = 0;
            p_data->win.width = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
            p_data->win.height = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);
        }
        else
        {
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winx");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winx");
            p_data->win.x = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winy");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winy");
            p_data->win.y = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winw");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winw");
            p_data->win.width = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winh");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winh");
            p_data->win.height = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);
        }

        if(plane_num==0)        id = PROPID(disp_fd, "fb0_format");
        else if(plane_num==1)   id = PROPID(disp_fd, "fb1_format");
        else if(plane_num==2)   id = PROPID(disp_fd, "fb2_format");
        p_data->data_mode = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);

        if(plane_num==0)        id = PROPID(disp_fd, "pip_fb0input");
        else if(plane_num==1)   id = PROPID(disp_fd, "pip_fb1input");
        else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2input");
        p_data->color_mode = dvr_graph_get_property(disp_data->graph, disp_fd, id, 1);
    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}

static int do_set_plane_param(dvr_disp_data  *disp_data, dvr_disp_plane_param *plane_param_set)
{
    int i, disp_num, plane_num, disp_fd, id=0, ret=0;
    dvr_disp_plane_param_st      *p_data;
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);
    disp_num = plane_param_set->disp_num;
    plane_num = plane_param_set->plane_num;
    p_data = &plane_param_set->param;
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_FUNC(ptag, FN_PLANE_PARAM);

    if(disp_num<0 || disp_num>lcd_cnt)
    {
        printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_num, lcd_cnt);
        return -1;
    }
    if(plane_num<0 || plane_num>plane_cnt)
    {
        printk("[dvr_disp] Incorrect plane number(%d) range(0~%d)\n", plane_num, plane_cnt);
        return -1;
    }

    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        p_data->plane_id = DVR_PLANE_ID(disp_num, plane_num);

        if(plane_num!=0)
        {
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winx");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winx");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.win.x, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winy");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winy");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.win.y, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winw");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winw");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.win.width, 1);
            if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winh");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winh");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.win.height, 1);
        }

        if(plane_num==0)        id = PROPID(disp_fd, "fb0_format");
        else if(plane_num==1)   id = PROPID(disp_fd, "fb1_format");
        else if(plane_num==2)   id = PROPID(disp_fd, "fb2_format");
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.data_mode, 1);

        if(plane_num==0)        id = PROPID(disp_fd, "pip_fb0input");
        else if(plane_num==1)   id = PROPID(disp_fd, "pip_fb1input");
        else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2input");
        dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_set->param.color_mode, 1);

        ret = dvr_graph_apply_property(disp_data->graph, ptag);
        if(ret<0)
            printk("[dvr_disp] set plane parameters failed(%d)\n", ret);
    }
    return ret;
}

static int do_update_plane_param(dvr_disp_data  *disp_data, dvr_disp_update_plane_param *plane_param_update)
{
    int i, ret=0, disp_num, plane_num, disp_fd, id=0;
    FuncTag tag, *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_num = GET_DISP_NUM_FROM_PLANE_ID(plane_param_update->plane_id);
    plane_num = GET_PLANE_NUM_FROM_PLANE_ID(plane_param_update->plane_id);
    ptag = &tag;
    FN_RESET_TAG(ptag);
    FN_SET_FUNC(ptag, FN_PLANE_PARAM);

    if(plane_param_update->param != PLANE_PARAM_APPLY)
    {
        if(disp_num<0 || disp_num>lcd_cnt)
        {
            printk("[dvr_disp] Incorrect LCD number(%d) range(0~%d)\n", disp_num, lcd_cnt);
            return -1;
        }
        if(plane_num<0 || plane_num>plane_cnt)
        {
            printk("[dvr_disp] Incorrect plane number(%d) range(0~%d)\n", plane_num, plane_cnt);
            return -1;
        }
    }


    for(i=0; i<lcd_cnt; i++)
    {
        if(!dup_disp && i!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(i));

        switch(plane_param_update->param)
        {
          case PLANE_PARAM_COLOR_MODE:
            if(plane_num==0)        id = PROPID(disp_fd, "pip_fb0input");
            else if(plane_num==1)   id = PROPID(disp_fd, "pip_fb1input");
            else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2input");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.color_mode, 1);
            break;
            
          case PLANE_PARAM_WINDOW:
            if(plane_num==0)
            {
                printk("Error! Change background dimension is forbidden. plane_id(%d)\n", plane_param_update->plane_id);
                return -1;  //fb0 is always background and can't change its size
            }
            else
            {
                if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winx");
                else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winx");
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.win.x, 1);

                if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winy");
                else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winy");


                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.win.y, 1);

                if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winw");
                else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winw");
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.win.width, 1);

                if(plane_num==1)        id = PROPID(disp_fd, "pip_fb1winh");
                else if(plane_num==2)   id = PROPID(disp_fd, "pip_fb2winh");
                dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.win.height, 1);
            }
            break;
            
          case PLANE_PARAM_DATA_MODE:
            if(plane_num==0)        id = PROPID(disp_fd, "fb0_format");
            else if(plane_num==1)   id = PROPID(disp_fd, "fb1_format");
            else if(plane_num==2)   id = PROPID(disp_fd, "fb2_format");
            dvr_graph_set_property(disp_data->graph, ptag, disp_fd, id, plane_param_update->val.data_mode, 1);
            break;

          case PLANE_PARAM_APPLY:
            ret = dvr_graph_apply_property(disp_data->graph, ptag);
            if(ret<0)
                printk("[dvr_disp] update plane parameters failed(%d)\n", ret);
            break;
            
          default:
            printk("This plane pamameter(%d) is not supported!", plane_param_update->param);
            ret = -1;
            break;
        }
    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return ret;
}

static int do_clear_win(dvr_disp_data *disp_data, dvr_disp_clear_param *disp_clear_param)
{  
    int d, i, j, no, disp_num, disp_fd;
    int xres, yres;
    char *tmp_ptr;
    unsigned int *pbuf;

    disp_num = GET_DISP_NUM_FROM_PLANE_ID(disp_clear_param->plane_id);

    for(d=0; d<lcd_cnt; d++)
    {
        if(!dup_disp && d!=disp_num)
            continue;
        disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(d));
        xres = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "width"), 1);
        yres = dvr_graph_get_property(disp_data->graph, disp_fd, PROPID(disp_fd, "height"), 1);
        for(no=0; no < disp_data->lcd_qst.count; no++) 
        {
            tmp_ptr = (char*)disp_data->lcd_qst.que->buf[no]->addr_va+disp_clear_param->win.y*(xres<<1)+(disp_clear_param->win.x>>1)*4;
            for(j = 0; j < disp_clear_param->win.height; j++) 
            {
                pbuf = (unsigned int *) tmp_ptr;
                tmp_ptr += (xres << 1);
                for (i = 0; i < (disp_clear_param->win.width >>1); i++)
                    pbuf[i] = disp_clear_param->pattern;
            }
        }
    }
    return 0;
}


int dvr_disp_open(struct inode *inode, struct file *filp)
{
    int ret=0;
    dvr_disp_data  *disp_data;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    down(&dvr_disp_sem);

    if(disp_private_data[0].is_used)
    {
        printk("[dvr_disp] Not support multi-open!\n");
        up(&dvr_disp_sem);
        return -EFAULT;
    }

    disp_data = &disp_private_data[0];
    filp->private_data = disp_data;
    disp_data->is_used = TRUE;
    memset(&disp_data->lv_info[0], 0x0, sizeof(disp_data->lv_info[0])*DVR_LIVEVIEW_CHANNEL_NUM);
    up(&dvr_disp_sem);
    do_initialize(disp_data);
    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return ret;
}

int dvr_disp_release(struct inode *inode, struct file *filp)
{
    dvr_disp_data  *disp_data = filp->private_data;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    down(&dvr_disp_sem);
    disp_data->is_used = FALSE;

    up(&dvr_disp_sem);
    do_terminate(disp_data);
    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;

}

int dvr_disp_mmap(struct file *filp, struct vm_area_struct *vma)
{
    printk("DVR DISP: mmap does not implement!");

    return 0;
}

static int dvr_disp_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret=-1;
    dvr_disp_data  *disp_data = filp->private_data;

    down(&dvr_disp_sem);

    switch(cmd)
    {
      case DVR_DISP_INITIATE:
        ret = 0;
        break;

      case DVR_DISP_TERMINATE:
        ret = 0;
        break;

      case DVR_DISP_GET_DISP_PARAM:
      {
        dvr_disp_disp_param  disp_param_get;
        if(copy_from_user((void *)&disp_param_get, (void *)arg, sizeof(dvr_disp_disp_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        ret = do_get_disp_param(disp_data, &disp_param_get);
        if(ret>=0)
        {
            if(copy_to_user((void *)arg,(void *)&disp_param_get,sizeof(dvr_disp_disp_param))) {
                ret = -EFAULT;
                goto exit1;
            }
            memcpy(&disp_data->la_disp_param_get, &disp_param_get, sizeof(dvr_disp_disp_param));
        }
        break;
      }

      case DVR_DISP_SET_DISP_PARAM:
      {
        dvr_disp_disp_param  disp_param_set;

        if(copy_from_user((void *)&disp_param_set, (void *)arg, sizeof(dvr_disp_disp_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_disp_param_set, &disp_param_set, sizeof(dvr_disp_disp_param));
        ret = do_set_disp_param(disp_data, &disp_param_set);
        if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&disp_param_set,sizeof(dvr_disp_disp_param))) {
                ret = -EFAULT;
                goto exit1;
            }
        }
        break;
      }

      case DVR_DISP_UPDATE_DISP_PARAM:
      {
        dvr_disp_update_disp_param  disp_param_update;
        if(copy_from_user((void *)&disp_param_update, (void *)arg, sizeof(dvr_disp_update_disp_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_disp_param_update, &disp_param_update, sizeof(dvr_disp_update_disp_param));
        ret = do_update_disp_param(disp_data, &disp_param_update);
        if(ret>=0) {
            if(copy_to_user((void *)arg, (void *)&disp_param_update, sizeof(dvr_disp_update_disp_param))) {
                ret = -EFAULT;
                goto exit1;
            }
        }
        break;
      }

      case DVR_DISP_GET_PLANE_PARAM:
      {
        dvr_disp_plane_param plane_param_get;
        if(copy_from_user((void *)&plane_param_get, (void *)arg, sizeof(dvr_disp_plane_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        ret = do_get_plane_param(disp_data, &plane_param_get);
        if(ret>=0)
        {
            if(copy_to_user((void *)arg,(void *)&plane_param_get,sizeof(dvr_disp_plane_param))) {
                ret = -EFAULT;
                goto exit1;
            }
            memcpy(&disp_data->la_plane_param_get, &plane_param_get, sizeof(dvr_disp_plane_param));
        }
        break;
      }

      case DVR_DISP_SET_PLANE_PARAM:
      {
        dvr_disp_plane_param plane_param_set;
        if(copy_from_user((void *)&plane_param_set, (void *)arg, sizeof(dvr_disp_plane_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_plane_param_set, &plane_param_set, sizeof(dvr_disp_plane_param));
        ret = do_set_plane_param(disp_data, &plane_param_set);
        if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&plane_param_set,sizeof(dvr_disp_plane_param))) {
                ret = -EFAULT;
                goto exit1;
            }
        }
        break;
      }

      case DVR_DISP_UPDATE_PLANE_PARAM:
      {
        dvr_disp_update_plane_param plane_param_update;
        if(copy_from_user((void *)&plane_param_update, (void *)arg, sizeof(dvr_disp_update_plane_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_plane_param_update, &plane_param_update, sizeof(dvr_disp_update_plane_param));
        ret = do_update_plane_param(disp_data, &plane_param_update);
        if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&plane_param_update,sizeof(dvr_disp_update_plane_param))) {
                ret = -EFAULT;
                goto exit1;
            }
        }
        break;
      }

      case DVR_DISP_CONTROL:
      {
        dvr_disp_control disp_ctrl;
        
        if(copy_from_user((void *)&disp_ctrl, (void *)arg, sizeof(dvr_disp_control))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_disp_ctrl, &disp_ctrl, sizeof(dvr_disp_control));                
        
        if(disp_ctrl.command == DISP_RUN)
        {
            //ret = dvr_graph_run(disp_data->graph);
            panic("DVR_DISP_CONTROL(DISP_RUN) is replaced by DVR_COMMON_APPLY.\n");
        }
        else if(disp_ctrl.command == DISP_START)
        {
            if(disp_ctrl.type==DISP_TYPE_LIVEVIEW)
            {
                ret = set_liveview_start_graph(disp_data, &disp_ctrl);
            }
            else if(disp_ctrl.type==DISP_TYPE_CASCADE)
            {
                ret = set_cascade_start_graph(disp_data, &disp_ctrl);
            }
        }
        else if(disp_ctrl.command == DISP_STOP)
        {
            if(disp_ctrl.type==DISP_TYPE_LIVEVIEW)
            {
                ret = set_liveview_stop_graph(disp_data, &disp_ctrl);
            }
            else if(disp_ctrl.type==DISP_TYPE_CASCADE)
            {
                ret = set_cascade_stop_graph(disp_data, &disp_ctrl);
            }
        }
        else if(disp_ctrl.command == DISP_UPDATE)
        {
            if(disp_ctrl.type==DISP_TYPE_LIVEVIEW)
            {
                if(disp_data->lv_info[disp_ctrl.channel].disp_ext.chn_type == DISP_LAYER1_CHN && disp_data->lv_info[disp_ctrl.channel].is_set == 1) {
                    ret = update_pip_param(disp_data, &disp_ctrl);
                    if(ret < 0)
                        goto exit1;
                }
                else    
                    ret = update_liveview_graph_param(disp_data, &disp_ctrl);
            }
            else if(disp_ctrl.type==DISP_TYPE_CASCADE)
            {
                ret = update_cascade_graph_param(disp_data, &disp_ctrl);
            }
        }
        else
            printk("[dvr_disp] Wrong DVR_DISP_CONTROL parameter, cmd(%d) type(%d)\n", disp_ctrl.command, disp_ctrl.type);
        break;
      }

      case DVR_DISP_CLEAR_WIN:
      {
        dvr_disp_clear_param disp_clear_param;
        if(copy_from_user((void *)&disp_clear_param, (void *)arg, sizeof(dvr_disp_clear_param))) {
            ret = -EFAULT;
            goto exit1;
        }
        memcpy(&disp_data->la_disp_clear_param, &disp_clear_param, sizeof(dvr_disp_clear_param));
        ret = do_clear_win(disp_data, &disp_clear_param);
        break;
      }

      default:
        printk("disp_ioctl cmd(0x%x) no define!\n", cmd);
        break;
    }

exit1:
    up(&dvr_disp_sem);
    return ret;
}




struct file_operations dvr_disp_fops = {
    owner:      THIS_MODULE,
    ioctl:      dvr_disp_ioctl,
    mmap:       dvr_disp_mmap,
    open:       dvr_disp_open,
    release:    dvr_disp_release,
};

    
struct miscdevice dvr_disp_dev = {
    minor: MISC_DYNAMIC_MINOR,
    name: "dvr_disp",
    fops: &dvr_disp_fops,
};


static int __init dvr_disp_init(void)
{
    int i, ret=0;
    file_t *cfg_file;
    int buf_size=0, count=0, ddr_num=0;
    unsigned char tmp_str[32], tmp_str2[32];
    unsigned char str_dvr_disp[]="dvr_disp";
    QueMemCfg   ch_cfg_scl;
    dvr_info_disp_count disp_info;

    //read default config
    cfg_file = gmcfg_open(GMDVR_MEM_CFG_FILE, O_RDONLY);
    if(!cfg_file)
    {
        printk("[dvr_disp] open [%s] failed!\n", GMDVR_MEM_CFG_FILE);
        return -EFAULT;
    }
    if(gmcfg_getfieldint( str_dvr_disp, "lcd_size", &default_disp1_cfg.size, cfg_file) < 0)   ret=-1;
    if(gmcfg_getfieldint( str_dvr_disp, "lcd_count", &default_disp1_cfg.count, cfg_file) < 0)   ret=-2;
    if(gmcfg_getfieldint( str_dvr_disp, "lcd_ddr", &default_disp1_cfg.ddr_num, cfg_file) < 0)   ret=-3;
    if(gmcfg_getfieldint( str_dvr_disp, "3di_scl_in_size", &default_3di_sclq_cfg.size, cfg_file) < 0)   ret=-4;
    if(gmcfg_getfieldint( str_dvr_disp, "3di_scl_in_count", &default_3di_sclq_cfg.count, cfg_file) < 0)   ret=-5;
    if(gmcfg_getfieldint( str_dvr_disp, "3di_scl_in_ddr", &default_3di_sclq_cfg.ddr_num, cfg_file) < 0)   ret=-6;
    if(gmcfg_getfieldint( str_dvr_disp, "lv_scl_in_size", &default_lv_sclq_cfg.size, cfg_file) < 0)   ret=-7;
    if(gmcfg_getfieldint( str_dvr_disp, "lv_scl_in_count", &default_lv_sclq_cfg.count, cfg_file) < 0)   ret=-8;
    if(gmcfg_getfieldint( str_dvr_disp, "lv_scl_in_ddr", &default_lv_sclq_cfg.ddr_num, cfg_file) < 0)   ret=-9;
#if 0
    if(gmcfg_getfieldint( str_dvr_disp, "osd_in_size", &default_osd_cfg.size, cfg_file) < 0)   ret=-10;
    if(gmcfg_getfieldint( str_dvr_disp, "osd_in_count", &default_osd_cfg.count, cfg_file) < 0)   ret=-11;
    if(gmcfg_getfieldint( str_dvr_disp, "osd_in_ddr", &default_osd_cfg.ddr_num, cfg_file) < 0)   ret=-12;
#endif    
    if(ret<0)
    {
        panic("[dvr_disp] Wrong config content! err(%d)\n", ret);
        return -EFAULT;
    }

    if(default_disp1_cfg.count && default_disp1_cfg.size)
    {
        snprintf(tmp_str, 32, "%s", QNAME_LCD);
        disp_private_data[0].lcd_qst.size = default_disp1_cfg.size;
        disp_private_data[0].lcd_qst.count = default_disp1_cfg.count;
        disp_private_data[0].lcd_qst.ddr = default_disp1_cfg.ddr_num;

        disp_private_data[0].lcd_qst.que = dvr_queue_allocate(TYPE_NORMAL, default_disp1_cfg.count, default_disp1_cfg.size, tmp_str, NULL, default_disp1_cfg.ddr_num,PATTERN_CACHE);

        snprintf(tmp_str, 32, "%s_d", QNAME_LCD);
        disp_private_data[0].lcd_dup_qst.size = disp_private_data[0].lcd_qst.size;
        disp_private_data[0].lcd_dup_qst.count = disp_private_data[0].lcd_qst.count;
        disp_private_data[0].lcd_dup_qst.ddr = disp_private_data[0].lcd_qst.ddr;
        disp_private_data[0].lcd_dup_qst.que = dvr_queue_allocate(TYPE_DUP, disp_private_data[0].lcd_qst.count, disp_private_data[0].lcd_qst.size, tmp_str, disp_private_data[0].lcd_qst.que, disp_private_data[0].lcd_qst.ddr,PATTERN_NONE);
    }
    
    if(default_3di_sclq_cfg.count && default_3di_sclq_cfg.size)
    {
        snprintf(tmp_str, 32, "%s", QNAME_3DI_SCL);
        disp_private_data[0].di_scl_in_qst[0].size = default_3di_sclq_cfg.size;
        disp_private_data[0].di_scl_in_qst[0].count = default_3di_sclq_cfg.count;
        disp_private_data[0].di_scl_in_qst[0].ddr = default_3di_sclq_cfg.ddr_num;
        //disp_private_data[0].di_scl_in_qst[0].que = dvr_queue_allocate(TYPE_NORMAL, default_3di_sclq_cfg.count, default_3di_sclq_cfg.size, tmp_str, NULL, default_3di_sclq_cfg.ddr_num,PATTERN_NONE);
        disp_private_data[0].di_scl_in_qst[0].que = dvr_queue_allocate(TYPE_NORMAL, default_3di_sclq_cfg.count, default_3di_sclq_cfg.size, tmp_str, NULL, default_3di_sclq_cfg.ddr_num,0x10801080);

        snprintf(tmp_str, 32, "%s_d", QNAME_3DI_SCL);
        disp_private_data[0].di_scl_in_dup_qst[0].size = default_3di_sclq_cfg.size;
        disp_private_data[0].di_scl_in_dup_qst[0].count = default_3di_sclq_cfg.count;
        disp_private_data[0].di_scl_in_dup_qst[0].ddr = default_3di_sclq_cfg.ddr_num;
        disp_private_data[0].di_scl_in_dup_qst[0].que = dvr_queue_allocate(TYPE_DUP, default_3di_sclq_cfg.count, default_3di_sclq_cfg.size, tmp_str, disp_private_data[0].di_scl_in_qst[0].que, default_3di_sclq_cfg.ddr_num,PATTERN_NONE);
    }
#if 0    
    if(default_osd_cfg.count && default_osd_cfg.size)
    {
        printk("read osd cfg\n");
        snprintf(tmp_str, 32, "%s", QNAME_OSD);
        disp_private_data[0].osd_qst.size = default_osd_cfg.size;
        disp_private_data[0].osd_qst.count = default_osd_cfg.count;
        disp_private_data[0].osd_qst.ddr = default_osd_cfg.ddr_num;

        disp_private_data[0].osd_qst.que = dvr_queue_allocate(TYPE_NORMAL, default_osd_cfg.count, default_osd_cfg.size, tmp_str, NULL, default_osd_cfg.ddr_num,PATTERN_CACHE);

        snprintf(tmp_str, 32, "%s_d", QNAME_OSD);
        disp_private_data[0].osd_dup_qst.size = disp_private_data[0].osd_qst.size;
        disp_private_data[0].osd_dup_qst.count = disp_private_data[0].osd_qst.count;
        disp_private_data[0].osd_dup_qst.ddr = disp_private_data[0].osd_qst.ddr;
        disp_private_data[0].osd_dup_qst.que = dvr_queue_allocate(TYPE_DUP, disp_private_data[0].osd_qst.count, disp_private_data[0].osd_qst.size, tmp_str, disp_private_data[0].osd_qst.que, disp_private_data[0].osd_qst.ddr,PATTERN_NONE);
    }
#endif    
    for(i=0; i<DVR_LIVEVIEW_CHANNEL_NUM; i++)
    {
        // Use sclq config
        snprintf(tmp_str2, 32, "lv_scl_in%d_size", i);
        ret = gmcfg_getfieldint( str_dvr_disp, tmp_str2, &ch_cfg_scl.size, cfg_file);
        if(ret<0)   buf_size = default_lv_sclq_cfg.size;
        else        buf_size = ch_cfg_scl.size;
        snprintf(tmp_str2, 32, "lv_scl_in%d_count", i);
        ret = gmcfg_getfieldint( str_dvr_disp, tmp_str2, &ch_cfg_scl.count, cfg_file);
        if(ret<0)   count = default_lv_sclq_cfg.count;
        else        count = ch_cfg_scl.count;
        snprintf(tmp_str2, 32, "lv_scl_in%d_ddr", i);
        ret = gmcfg_getfieldint( str_dvr_disp, tmp_str2, &ch_cfg_scl.ddr_num, cfg_file);
        if(ret<0)   ddr_num = default_lv_sclq_cfg.ddr_num;
        else        ddr_num = ch_cfg_scl.ddr_num;

        if(count && buf_size)
        {
            snprintf(tmp_str, 32, "%s%d", QNAME_LV_SCL, i);
            disp_private_data[0].scl_in_qst[i].size = buf_size;
            disp_private_data[0].scl_in_qst[i].count = count;
            disp_private_data[0].scl_in_qst[i].ddr = ddr_num;
            //disp_private_data[0].scl_in_qst[i].que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
            disp_private_data[0].scl_in_qst[i].que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,0x10801080);

            snprintf(tmp_str, 32, "%s_d%d", QNAME_LV_SCL, i);
            disp_private_data[0].scl_in_dup_qst[i].size = buf_size;
            disp_private_data[0].scl_in_dup_qst[i].count = count;
            disp_private_data[0].scl_in_dup_qst[i].ddr = ddr_num;
            disp_private_data[0].scl_in_dup_qst[i].que = dvr_queue_allocate(TYPE_DUP, count, buf_size, tmp_str, disp_private_data[0].scl_in_qst[i].que, ddr_num,PATTERN_NONE);
        }
    }
    gmcfg_close( cfg_file ) ;

    // Add proc node
    dvr_disp_proc = create_proc_entry("disp_log", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_disp_proc==NULL)
        return -EFAULT;
    dvr_disp_proc->read_proc = (read_proc_t *) proc_read_mode;
    dvr_disp_proc->write_proc = (write_proc_t *) proc_write_mode;
    dvr_disp_proc->owner = THIS_MODULE;

    proc_last_api = create_proc_entry("disp_api", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(proc_last_api==NULL)
        return -EFAULT;
    proc_last_api->read_proc = (read_proc_t *) proc_read_lastapi;
    proc_last_api->write_proc = (write_proc_t *) proc_write_lastapi;
    proc_last_api->owner = THIS_MODULE;

    init_MUTEX(&dvr_disp_sem);

    /* Platform dependent */
    ///////////////////////////////////////////////////////
//++ fullhd
#if 0
    if ((ft_get_version() & 0xF0) == 0x10) /* 8181 series */
    {
        if(lcd_cnt>1)
        {
            printk("Warning!! DVR DISP driver can't support %d LCD in this platform."
                    " Driver argument 'lcd_cnt' will be reset as 1. \n", lcd_cnt);
            lcd_cnt = 1;
        }
        if(dup_disp==1)
        {
            printk("Warning!! DVR DISP driver will change the argument 'dup_disp' to 0.\n");
            dup_disp = 0;
        }
    }
    else /* 8181T/8186 */
    {
    }
#endif
//-- fullhd
    ///////////////////////////////////////////////////////

    disp_info.lcd_count = lcd_cnt;
    disp_info.plane_count = plane_cnt;
    disp_info.dup_disp = dup_disp;
    dvr_common_store_info(INFO_INDEX_DISP_COUNT, &disp_info);

    if(dup_disp)
        video_set_buffer_out_sync(disp_private_data[0].lcd_qst.que, 2, METHOD_SKIP);
    printk("Insert dvr_disp driver done.\n");

    return misc_register(&dvr_disp_dev);
}

static void __exit dvr_disp_clearnup(void)
{
    int i;


    if(disp_private_data[0].lcd_qst.que)
    {
        dvr_queue_destroy(disp_private_data[0].lcd_qst.que);
        disp_private_data[0].lcd_qst.que = NULL;
    }
    if(disp_private_data[0].lcd_dup_qst.que)
    {
        dvr_queue_destroy(disp_private_data[0].lcd_dup_qst.que);
        disp_private_data[0].lcd_dup_qst.que = NULL;
    }
    if(disp_private_data[0].di_scl_in_qst[0].que)
    {
        dvr_queue_destroy(disp_private_data[0].di_scl_in_qst[0].que);
        disp_private_data[0].di_scl_in_qst[0].que = NULL;
    }
    if(disp_private_data[0].di_scl_in_dup_qst[0].que)
    {
        dvr_queue_destroy(disp_private_data[0].di_scl_in_dup_qst[0].que);
        disp_private_data[0].di_scl_in_dup_qst[0].que = NULL;
    }
    if(disp_private_data[0].osd_qst.que)
    {
        dvr_queue_destroy(disp_private_data[0].osd_qst.que);
        disp_private_data[0].osd_qst.que = NULL;
    }
    if(disp_private_data[0].osd_dup_qst.que)
    {
        dvr_queue_destroy(disp_private_data[0].osd_dup_qst.que);
        disp_private_data[0].osd_dup_qst.que = NULL;
    }
    for(i=0; i<DVR_LIVEVIEW_CHANNEL_NUM; i++)
    {
        if(disp_private_data[0].scl_in_qst[i].que)
        {
            dvr_queue_destroy(disp_private_data[0].scl_in_qst[i].que);
            disp_private_data[0].scl_in_qst[i].que = NULL;
        }
        if(disp_private_data[0].scl_in_dup_qst[i].que)
        {
            dvr_queue_destroy(disp_private_data[0].scl_in_dup_qst[i].que);
            disp_private_data[0].scl_in_dup_qst[i].que = NULL;
        }
    }

    misc_deregister(&dvr_disp_dev);
    remove_proc_entry("disp_api", DvrProcRoot);
    remove_proc_entry("disp_log", DvrProcRoot);
}


module_init(dvr_disp_init);
module_exit(dvr_disp_clearnup);
MODULE_LICENSE("GPL");


