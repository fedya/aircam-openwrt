
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "dvr_graph.h"
#include "dec_main.h"
#include "dvr_dec_api.h"
#include "dvr_disp_api.h"
#include "datain_entity.h"
#include "util_cfg.h"
#include "dvr_mod_define.h"

static unsigned int dec_max = 1;   //1: D1, 2: CIF, 3: QCIF
static unsigned int max_width=DEFAULT_D1_WIDTH, max_height=DEFAULT_D1_HEIGHT;
module_param(dec_max, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(dec_max, "1:D1 2:CIF 3:QCIF");

struct semaphore    dvr_dec_oper_sem;
struct semaphore    dvr_dec_cfg_decoder_sem;
dvr_dec_data    dec_private_data[DVR_PLAYBACK_CHANNEL_NUM];
QueMemCfg       default_decq_cfg, default_pb_sclq_cfg;
#if TWO_STAGES_SCALER
QueMemCfg       default_pb_scl2q_cfg;
#endif
int             dec_share_scaler=0;
struct workqueue_struct *dvrdec_wq;

struct list_head datain_callback_head[MAX_DATAIN_ENTITY_NUM];

static struct proc_dir_entry *dvr_dec_proc=NULL; 
static struct proc_dir_entry *dvr_dec_perf=NULL; 
static struct proc_dir_entry *proc_last_api=NULL;
static int last_api_num = 0;
static int last_api_ch = 0;

int dvr_dec_dbgmode = 0;
 
#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_dec_dbgmode) printk(fmt, ## args);}

void dvr_add_or_modify_nodirty(int fd,int enable,int region_check,int x,int y,int w,int h);
void force_nodirty_mode(int fd,int nodirty,int disabled);


static int bShowPerf = 0;

#include "videograph_api.h"
extern unsigned int video_gettime_max_ms(void);
typedef struct dec_perf{
    u32 cnt;
    u32 max_latance;
    u32 min_latance;
    u32 avg_latance;
    u32 time;
}dec_perf_t;

static dec_perf_t gperf_info[DVR_PLAYBACK_CHANNEL_NUM];

static void reset_perf(dec_perf_t *pperf)
{
    pperf->cnt = 0;
    pperf->max_latance = 0;
    pperf->min_latance = 0xffffffff;
    pperf->avg_latance = 0;
}

static u32 perf_time_diff(u32 sTime, u32 eTime)
{
    static u32 max_val=0;

    if(!max_val){
        max_val = video_gettime_max_ms();
    }

    if(sTime > eTime){
        return (max_val-sTime+eTime);
    }
    else{
        return (eTime - sTime);
    }
}

static void update_perf(dec_perf_t *pperf)
{
    if(!bShowPerf)
        return;

    pperf->cnt++;
    pperf->time = video_gettime_ms();
}

static void update_latance(dec_perf_t *pperf)
{
    u32 diff;
    diff = perf_time_diff(pperf->time, video_gettime_ms());
    pperf->max_latance = max(diff, pperf->max_latance);
    pperf->min_latance = min(diff, pperf->min_latance);
    pperf->avg_latance += diff;
}

static void show_perf(void)
{
    int i;
    static u32 StartTime=0;
    u32 diff;

    if(!bShowPerf)
        return;

    if(!StartTime)
        StartTime = video_gettime_ms();

    diff = perf_time_diff(StartTime, video_gettime_ms());
    if(diff>=10000){
        printk("DecPerf(%d): stream.id, frm.count, max.latence, min.latence, avg.latence\n", diff);
        for(i=0;i<DVR_PLAYBACK_CHANNEL_NUM;i++) {
            if(gperf_info[i].cnt)
                printk("\t([%d]%d,%d,%d,%d)\n", i, gperf_info[i].cnt, gperf_info[i].max_latance, gperf_info[i].min_latance, gperf_info[i].avg_latance);
            reset_perf(&gperf_info[i]);
        }
        StartTime = video_gettime_ms();
    }
}


static int proc_read_mode(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    printk("Current Debug Mode: 0x%x\n", dvr_dec_dbgmode);
    printk(" Values(combinative): 0x0:Off, 0x1:DTI Fnc, 0x2:DTI Job, 0x4:DEC Fnc, 0x8:DEC data flow\n");
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
    dvr_dec_dbgmode = tmp;
    printk("Debug Mode:%d\n", dvr_dec_dbgmode);
    return count;
}

static int proc_perf_show(struct seq_file *sfile, void *v)
{   
    seq_printf(sfile, "Show DEC performance:%s\n",bShowPerf?"Enable":"Disable");
    return 0;
}

static int proc_perf_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_perf_show, NULL);
}

static ssize_t proc_perf_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    int len = size;
    char value[20];
    u32 tmp;
        
    if(copy_from_user(value, buf, len))
        return 0;
    value[len] = '\0';
    sscanf(value, "%u \n", &tmp);
    if(tmp) {
        bShowPerf = 1;
    }
    else {
        bShowPerf = 0;
    }
    return size;
}

static struct file_operations proc_perf_ops = {
    .owner  = THIS_MODULE,
    .open   = proc_perf_open,
    .read   = seq_read,
    .llseek = seq_lseek,
    .release= single_release,
    .write  = proc_perf_write,
};

static int proc_read_lastapi(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    int i;
    for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
        if(dec_private_data[i].ch_num == last_api_ch)
            break;

    if(i>=DVR_PLAYBACK_CHANNEL_NUM)
    {
        printk("Invalid arguments. ch(%d)\n", i);
        return 0;
    }

    switch (last_api_num)
    {
      case 2:
        print_user_last_api_data("DEC:DVR_DEC_SET_CHANNEL_PARAM", last_api_num, sizeof(dvr_dec_channel_param), (unsigned char*)&dec_private_data[i].la_ch_param_set, 1);
        break;
      case 3:  // DVR_DEC_GET_CHANNEL_PARAM
        printk("No structure.\n");
        break;
#if LOG_API_QUEUE_FLOW
      case 5:
        print_user_last_api_data("DEC:DVR_DEC_QUEUE_GET", last_api_num, sizeof(dvr_dec_queue_get), (unsigned char*)&dec_private_data[i].la_q_data_get, 0);
        break;
      case 6:
        print_user_last_api_data("DEC:DVR_DEC_QUEUE_PUT", last_api_num, sizeof(dvr_dec_queue_get), (unsigned char*)&dec_private_data[i].la_q_data_put, 1);
        break;
#endif /* LOG_API_QUEUE_FLOW */
      case 7:
        print_user_last_api_data("DEC:DVR_DEC_CONTROL", last_api_num, sizeof(dvr_dec_control), (unsigned char*)&dec_private_data[i].la_dec_ctrl, 1);
        break;
      case 8:
        print_user_last_api_data("DEC:DVR_DEC_QUERY_OUTPUT_BUFFER_SIZE", last_api_num, sizeof(int), (unsigned char*)&dec_private_data[i].la_buf_size, 0);
        break;
      case 18:
        print_user_last_api_data("DEC:DVR_DEC_CLEAR_WIN", last_api_num, sizeof(dvr_dec_clear_param), (unsigned char*)&dec_private_data[i].la_dec_clear_param, 1);
        break;
      case 19:
        print_user_last_api_data("DEC:DVR_DEC_CLEAR_WIN2", last_api_num, sizeof(dvr_dec_clear_param), (unsigned char*)&dec_private_data[i].la_dec_clear_param2, 1);
        break;
      default:
        printk("Invalid arguments. api(%d)\n", last_api_num);
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
    sscanf(value, "%d %d\n", &last_api_ch, &last_api_num);
    return size;
}

static void DtiNotify(int ch_num, int status)
{
    dvr_info_buf_flow_info  buf_flow_info;

    if(status==DTI_JOB_AP_GET)
    {
        buf_flow_info.fnc = BFI_FNC_PB(ch_num);
        buf_flow_info.in_out = 1;
        dvr_common_store_info(INFO_SET_BUF_FLOW_COUNT, &buf_flow_info);
    }
    else if(status==DTI_JOB_CALLBACK)
    {
        buf_flow_info.fnc = BFI_FNC_PB(ch_num);
        buf_flow_info.in_out = -1;
        dvr_common_store_info(INFO_SET_BUF_FLOW_COUNT, &buf_flow_info);
    }
    else if(status==DTI_JOB_TIMEOUT)
    {
        dvr_common_store_info(INFO_SET_FLOW_TIMEOUT, (void*)BFI_FNC_PB(ch_num));
    }
}



static int do_preparation(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
    dvr_dec_channel_param *ch_param;
    int ret, disp_num, size, cnt, ddr;
    unsigned char tmp_str[32];

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)ch(%d)] %s -- IN\n", (int)dec_data, dec_data->ch_num, __FUNCTION__);
    ch_param = &dec_data->ch_param_data;
    
    dec_data->dec_type = ch_param->dec_type;
    dec_data->dest_type = ch_param->dec_dest_type;
    dec_data->ch_num = ch_param->channel;
    //dec_data->plane_num = GET_PLANE_NUM_FROM_PLANE_ID(dec_ctrl->dst_param.plane_id);
    dec_data->is_blocked = ch_param->is_blocked;
    dec_data->is_use_scaler = ch_param->is_use_scaler;
    dvr_common_store_info(INFO_RESET_BUF_FLOW_IN_OUT, (void*)BFI_FNC_PB(dec_data->ch_num));

    dec_data->datain_fd = ENTITY_FD(DATA_IN_FD_MAJOR, DATA_IN_FD_MINOR(dec_data->ch_num));
    if(ch_param->dec_type==ENC_TYPE_H264)
        dec_data->dec_fd = ENTITY_FD(DEC_H264_FD_MAJOR, DEC_H264_FD_MINOR(dec_data->ch_num));
    else if(ch_param->dec_type==ENC_TYPE_MPEG)
        dec_data->dec_fd = ENTITY_FD(DEC_MPEG4_FD_MAJOR, DEC_MPEG4_FD_MINOR(dec_data->ch_num));
    else if(ch_param->dec_type==ENC_TYPE_MJPEG)
        dec_data->dec_fd = ENTITY_FD(DEC_MJPEG_FD_MAJOR, DEC_MJPEG_FD_MINOR(dec_data->ch_num));
    if(dec_data->dest_type == DEC_TYPE_TO_DISPLAY)
    {
        disp_num = GET_DISP_NUM_FROM_PLANE_ID(dec_ctrl->dst_param.plane_id);
        dec_data->scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_PB(dec_data->ch_num));
        dec_data->disp_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(disp_num));
        dec_data->lcd_dim.width = dvr_graph_get_property(NULL, dec_data->disp_fd, PROPID(dec_data->disp_fd, "width"), 1);
        dec_data->lcd_dim.height = dvr_graph_get_property(NULL, dec_data->disp_fd, PROPID(dec_data->disp_fd, "height"), 1);
        if(dec_data->disp_info.dup_disp)
        {
            dec_data->disp2_fd = ENTITY_FD(LCD_FD_MAJOR, LCD_FD_MINOR(1));
            //dec_data->lcd_dim.width = dvr_graph_get_property(NULL, dec_data->disp2_fd, PROPID(dec_data->disp2_fd, "width"), 1);
            //dec_data->lcd_dim.height = dvr_graph_get_property(NULL, dec_data->disp2_fd, PROPID(dec_data->disp2_fd, "height"), 1);
        }

        snprintf(tmp_str, 32, "%s", QNAME_LCD);
        dec_data->lcd_qst.que = dvr_queue_find_by_name(tmp_str, &size, &cnt, &ddr);

#if TWO_STAGES_SCALER
        dec_data->scl2_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_PB2(dec_data->ch_num));
  #if SHARE_LV_PB_SCL_BUF
        snprintf(tmp_str, 32, "%s%d", QNAME_LV_SCL, dec_data->ch_num);
        dec_data->pb_scl2_qst.que = dvr_queue_find_by_name(tmp_str, &dec_data->pb_scl2_qst.size, &dec_data->pb_scl2_qst.count, &dec_data->pb_scl2_qst.ddr);
  #endif
#endif
    }
    else
    {
        panic("[dvr_dec] This version only supports DEC_TYPE_TO_DISPLAY.\n");
    }

    //set tag... 
    FN_RESET_TAG(&dec_data->tag);
    FN_SET_PB_CH(&dec_data->tag, dec_data->ch_num);
    FN_RESET_TAG(&dec_data->tag_scl);
    FN_SET_PB_CH(&dec_data->tag_scl, dec_data->ch_num);
    FN_SET_FUNC(&dec_data->tag_scl, FN_PB_SCL_LINK);
    FN_SET_FUNC(&dec_data->tag_scl, FN_METHOD_USE_CMP);

    //set graph...
    if(dec_data->dest_type == DEC_TYPE_TO_DISPLAY)
    {
        dec_data->graph = dvr_graph_find_by_id(GFID_DISP(GET_PLANE_NUM_FROM_PLANE_ID(dec_ctrl->dst_param.plane_id)));
    }
    else
    {
        ret = dvr_graph_create(&dec_data->graph, GFID_DEC(dec_data->ch_num));
        if(ret<0)
            return -EFAULT;
    }

    ret = idatain_open(dec_data->ch_num, &dec_data->wait_queue_stream);
    if(ret<0)
        return -EFAULT;

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);

    return 0;
}

static int setup_mj_decoder_parameter(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
    dvr_dec_channel_param *ch_param;
    int dec_fd, val;
    dvr_graph *graph;
    FuncTag *ptag;
	unsigned int U_offset, V_offset;
		
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- IN\n", dec_data->ch_num, __FUNCTION__);
    dec_fd = dec_data->dec_fd;
    ch_param = &dec_data->ch_param_data;
    graph = dec_data->graph;
    ptag = &dec_data->tag;

    //give max width,height
	down(&dvr_dec_cfg_decoder_sem);
    val = dvr_graph_get_property(NULL, dec_fd, PROPID(dec_fd, "VGID_MJD_End"), 1);
	if (val != 0) {
		printk("mjd %x is not ready to set\n", dec_fd);
		return -EFAULT;
	}
	U_offset = dec_ctrl->src_param.dim.width *dec_ctrl->src_param.dim.height;
	V_offset = dec_ctrl->src_param.dim.width *dec_ctrl->src_param.dim.height * 5/ 4;
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_Start"), 0, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_frame_width"), dec_ctrl->src_param.dim.width, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_frame_height"), dec_ctrl->src_param.dim.height, 1);

	/*  VGID_MJD_output_format
	 *  OUTPUT_YUV=0, 	    // for all sampling
	 *  OUTPUT_420_YUV=1,	// only for 420, 422_0, 422_1
	 *  OUTPUT_CbYCrY=2,    // only for 420, 422_0, 422_1		*/
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_output_format"), 2, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_U_offset"), U_offset, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_V_offset"), V_offset, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MJD_End"), 1, 1);
	up(&dvr_dec_cfg_decoder_sem);
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);
    return 0;
}

static int setup_mp4_decoder_parameter(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
    dvr_dec_channel_param *ch_param;
    int dec_fd, val;
    dvr_graph *graph;
    FuncTag *ptag;
	unsigned int U_offset, V_offset;
    
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- IN\n", dec_data->ch_num, __FUNCTION__);
    dec_fd = dec_data->dec_fd;
    ch_param = &dec_data->ch_param_data;
    graph = dec_data->graph;
    ptag = &dec_data->tag;

    //give max width,height
	down(&dvr_dec_cfg_decoder_sem);
    val = dvr_graph_get_property(NULL, dec_fd, PROPID(dec_fd, "VGID_MP4D_End"), 1);
	if (val != 0) {
		printk("mp4d %x is not ready to set\n", dec_fd);
		return -EFAULT;
	}
	U_offset = dec_ctrl->src_param.dim.width * dec_ctrl->src_param.dim.height;
	V_offset = dec_ctrl->src_param.dim.width * dec_ctrl->src_param.dim.height * 5 / 4;
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_Start"), 0, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_u32MaxWidth"), max_width, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_u32MaxHeight"), max_height, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_u32FrameWidth"), dec_ctrl->src_param.dim.width, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_u32FrameHeight"), dec_ctrl->src_param.dim.height, 1);

	/*  VGID_MP4_output_format
	 *  if 0, YUV422
	 *  if 4, YUV420
	 *  */
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_output_image_format"), 0, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_U_offset"), U_offset, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_V_offset"), V_offset, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VGID_MP4D_End"), 1, 1);
	up(&dvr_dec_cfg_decoder_sem);
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);
    return 0;
}

static int setup_h264_decoder_parameter(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
    dvr_dec_channel_param *ch_param;
    int dec_fd;
    dvr_graph *graph;
    FuncTag *ptag;

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- IN\n", dec_data->ch_num, __FUNCTION__);
    dec_fd = dec_data->dec_fd;
    ch_param = &dec_data->ch_param_data;
    graph = dec_data->graph;
    ptag = &dec_data->tag;

    //give max width,height
  down(&dvr_dec_cfg_decoder_sem);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_Start"), 0, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_MaxWidth"), max_width, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_MaxHeight"), max_height, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_FrameWidth"), dec_ctrl->src_param.dim.width, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_FrameHeight"), dec_ctrl->src_param.dim.height, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_ImgType"), ch_param->dec_param.output_type, 1);
//    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_timescale"), 27000000, 1);
//    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_NumUnitsInTick"), 27000000/dec_ctrl->src_param.bs_rate, 1);
    dvr_graph_set_property(graph, ptag, dec_fd, PROPID(dec_fd, "VG_D_ID_End"), 0, 1);
  up(&dvr_dec_cfg_decoder_sem);

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);

    return 0;
}

#if TWO_STAGES_SCALER
static int check_link_two_scaler(int src_w, int src_h, int dest_w, int dest_h, int scl2_size, int src_rate, int dest_rate)
{
  #if 0
    if(src_w*src_h*2>scl2_size && dest_w*dest_h*2<scl2_size  && src_rate<dest_rate)
        return TRUE;
    else
        return FALSE;
  #else
    int ret=FALSE;
    int bw_b, bw_a; //band-width(before and after)

    if(dest_w*dest_h*2<=scl2_size)
    {
        bw_b = (src_w*src_h+dest_w*dest_h)*dest_rate;
        bw_a = (src_w*src_h+dest_w*dest_h)*src_rate + 2*dest_w*dest_h*dest_rate;
        if(bw_a<bw_b)
            ret = TRUE;
    }
    return ret;

  #endif
}
#endif

int setup_display_paramerter(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl, int b_two_scls)
{
    int disp_fd, scl_fd;
#if TWO_STAGES_SCALER
    int scl2_fd;
#endif    
    dvr_graph *graph;
    FuncTag *ptag;
    dvr_dec_channel_param *ch_param;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    disp_fd = dec_data->disp_fd;
    scl_fd = dec_data->scl_fd;
#if TWO_STAGES_SCALER
    scl2_fd = dec_data->scl2_fd;
#endif
    ch_param = &dec_data->ch_param_data;
    graph = dec_data->graph;
    ptag = &dec_data->tag;

    //setup LCD target    
    dvr_graph_set_property(dec_data->graph, ptag, disp_fd, PROPID(disp_fd, "videograph_plane"), GET_PLANE_NUM_FROM_PLANE_ID(dec_ctrl->dst_param.plane_id), 1);
    if(dec_data->disp_info.dup_disp)
        dvr_graph_set_property(dec_data->graph, ptag, dec_data->disp2_fd, PROPID(dec_data->disp2_fd, "videograph_plane"), GET_PLANE_NUM_FROM_PLANE_ID(dec_ctrl->dst_param.plane_id), 1);

    //setup Scaler
    if(dec_data->is_use_scaler)
    {
        if(ch_param->scl_param.src_fmt!=SCALE_YUV422 || ch_param->scl_param.dst_fmt!=SCALE_YUV422)
            panic("Only support YUV422 now!\n");
        
#if TWO_STAGES_SCALER
        if(b_two_scls)
        {
            //prior scaler (do scaling)
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "buf_offset"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "vp_width"), dec_ctrl->src_param.dim.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "vp_height"), dec_ctrl->src_param.dim.height, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_width"), dec_ctrl->src_param.dim.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_height"), dec_ctrl->src_param.dim.height, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_fmt"),  ch_param->scl_param.src_fmt, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_Y"),    0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_CbCr"), 1, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_interleaved"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_fmt"),       ch_param->scl_param.dst_fmt, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_Y"),    0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_CbCr"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_level"),     ch_param->scl_param.des_level, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "album"),         ch_param->scl_param.is_album, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "non_linear"),    ch_param->scl_param.scale_mode, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "dither"),        ch_param->scl_param.is_dither, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "correction"),    ch_param->scl_param.is_correction, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "mem_src_pit"), dec_ctrl->src_param.dim.width/2, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), dec_ctrl->src_param.win.x, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), dec_ctrl->src_param.win.y, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), dec_ctrl->src_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), dec_ctrl->src_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_x"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_y"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "target_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "target_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_x"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_y"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_height"), dec_ctrl->dst_param.win.height, 1);
//            if(dec_ctrl->dst_param.is_display)
            dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "enable_scale"), 0, 1);
//            else
//                dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "disable_scale"), 0, 1);

            //posterior scaler (only copy image)
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "buf_offset"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "vp_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_fmt"),  ch_param->scl_param.src_fmt, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_Y"),    0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_swap_CbCr"), 1, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "src_interleaved"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_fmt"), ch_param->scl_param.dst_fmt, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_Y"),    0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_swap_CbCr"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_level"),     ch_param->scl_param.des_level, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "album"),         ch_param->scl_param.is_album, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "non_linear"),    ch_param->scl_param.scale_mode, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "dither"),        ch_param->scl_param.is_dither, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "correction"),    ch_param->scl_param.is_correction, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "mem_src_pit"), dec_ctrl->dst_param.win.width/2, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_x"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_axis_y"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "swc_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_width"), dec_data->lcd_dim.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_height"), dec_data->lcd_dim.height, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_x"), dec_ctrl->dst_param.win.x, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "des_axis_y"), dec_ctrl->dst_param.win.y, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "target_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "target_height"), dec_ctrl->dst_param.win.height, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_x"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_axis_y"), 0, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_width"), dec_ctrl->dst_param.win.width, 1);
            dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "crop_height"), dec_ctrl->dst_param.win.height, 1);
            if(dec_ctrl->dst_param.is_display)
                dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "enable_scale"), 0, 1);
            else
                dvr_graph_set_property(graph, ptag, scl2_fd, PROPID(scl2_fd, "disable_scale"), 0, 1);
        }
        else
#endif
       {
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "buf_offset"), 0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "vp_width"), dec_ctrl->src_param.dim.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "vp_height"), dec_ctrl->src_param.dim.height, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_width"), dec_ctrl->src_param.dim.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_height"), dec_ctrl->src_param.dim.height, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_fmt"),  ch_param->scl_param.src_fmt, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_Y"),    0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_swap_CbCr"), 1, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "src_interleaved"), 0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_fmt"),       ch_param->scl_param.dst_fmt, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_Y"),    0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_swap_CbCr"), 0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_level"),     ch_param->scl_param.des_level, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "album"),         ch_param->scl_param.is_album, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "non_linear"),    ch_param->scl_param.scale_mode, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "dither"),        ch_param->scl_param.is_dither, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "correction"),    ch_param->scl_param.is_correction, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "mem_src_pit"), dec_ctrl->src_param.dim.width/2, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_x"), dec_ctrl->src_param.win.x, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_axis_y"), dec_ctrl->src_param.win.y, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_width"), dec_ctrl->src_param.win.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "swc_height"), dec_ctrl->src_param.win.height, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_width"), dec_data->lcd_dim.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_height"), dec_data->lcd_dim.height, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_x"), dec_ctrl->dst_param.win.x, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "des_axis_y"), dec_ctrl->dst_param.win.y, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "target_width"), dec_ctrl->dst_param.win.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "target_height"), dec_ctrl->dst_param.win.height, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_x"), 0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_axis_y"), 0, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_width"), dec_ctrl->dst_param.win.width, 1);
          dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "crop_height"), dec_ctrl->dst_param.win.height, 1);
  
          if(dec_ctrl->dst_param.is_display)
              dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "enable_scale"), 0, 1);
          else
              dvr_graph_set_property(graph, ptag, scl_fd, PROPID(scl_fd, "disable_scale"), 0, 1);
       }

#if TWO_STAGES_SCALER
        if(b_two_scls)
        {
            force_nodirty_mode(scl_fd,NODIRTY_MODE_DISABLE,1);
            video_set_disable_mode(scl2_fd, DISABLE_WITH_FINISH);	// do not block LCD
            dvr_add_or_modify_nodirty(scl2_fd,dec_ctrl->dst_param.is_display,0,
                dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
                dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);
        }
        else
#endif
       {
          video_set_disable_mode(scl_fd, DISABLE_WITH_FINISH);	// do not block LCD
          dvr_add_or_modify_nodirty(scl_fd,dec_ctrl->dst_param.is_display,0,
              dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
              dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);
       }
    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}


void update_bs_rate(dvr_dec_data *dec_data,int target_rate)
{
    dvr_graph *graph;
    FuncTag *ptag;
    int display_rate=0;

    graph = dec_data->graph;
    ptag = &dec_data->tag;

    display_rate=dec_data->display_rate;
    if(display_rate==0)
    {
        printk("Error to set display_rate=%d\n",display_rate);
        panic("display rate");
    }
    if(target_rate==0)
    {
        printk("Error to set bs_rate=%d\n",target_rate);
        panic("display rate");
    }

    if(dec_data->is_use_scaler)
    {
#if TWO_STAGES_SCALER
        if(dec_data->b_two_scls)
        {
            dvr_graph_update_flow(graph,ptag,dec_data->datain_fd,target_rate,dec_data->dec_fd,target_rate);
            dvr_graph_update_flow(graph,ptag,dec_data->dec_fd,target_rate,dec_data->scl_fd,target_rate);
            dvr_graph_update_flow(graph,&dec_data->tag_scl,dec_data->scl_fd,target_rate,dec_data->scl2_fd,display_rate);
        }
        else
#endif
       {
          dvr_graph_update_flow(graph,ptag,dec_data->datain_fd,target_rate,dec_data->dec_fd,target_rate);
          dvr_graph_update_flow(graph,ptag,dec_data->dec_fd,target_rate,dec_data->scl_fd,display_rate);
       }
    }
    else
    {
        dvr_graph_update_flow(graph,ptag,dec_data->datain_fd,target_rate,dec_data->dec_fd,target_rate);
        dvr_graph_update_flow(graph,ptag,dec_data->dec_fd,target_rate,dec_data->disp_fd,display_rate);
        if(dec_data->disp_info.dup_disp)
            dvr_graph_update_flow(graph,ptag,dec_data->dec_fd,target_rate,dec_data->disp2_fd,display_rate);
    }
    dvr_graph_apply_flow();
    idatain_set_flow_rate(dec_data->datain_fd,target_rate);
}


void dvr_dec_clear_win(dvr_dec_data *dec_data,int scl_fd,int xres,int yres,int x,int y,int w,int h)
{
    if(dec_data->clear_trigger!=SET_CLEAR_TRIGGER)
        return;
    
    dec_data->clear_trigger=0;
    video_clear_out_buf_by_dummy(scl_fd,xres,yres,x,y,w,h,dec_data->clear_pattern);        
}

int do_clear_win(dvr_dec_data *dec_data, dvr_dec_clear_param *clr_param)
{
    int fd;
#if TWO_STAGES_SCALER
    if(dec_data->b_two_scls)
        fd = dec_data->scl2_fd;
    else
#endif    
        fd = dec_data->scl_fd;

    video_clear_out_buf(fd, dec_data->lcd_dim.width, dec_data->lcd_dim.height,
        clr_param->win.x, clr_param->win.y, clr_param->win.width, clr_param->win.height, clr_param->pattern);
    return 0;
}


int set_playback_start_graph(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl, int is_from_update)
{
    int ret;
    int bs_rate, display_rate, scl_in_size, scl_in_cnt, dec_in_cnt;
    FuncTag *ptag, *ptag_scl;
    struct v_queue_t    *dec_in_q, *lcd_q, *pb_scl_in_q;
#if TWO_STAGES_SCALER
    struct v_queue_t    *pb_scl2_in_q;
#endif

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    ptag = &dec_data->tag;
    ptag_scl = &dec_data->tag_scl;

    ret = do_preparation(dec_data, dec_ctrl);
    if(ret<0)
        return -EFAULT;

	switch(dec_data->dec_type)
	{
		case ENC_TYPE_H264: 
			ret = setup_h264_decoder_parameter(dec_data, dec_ctrl); 
			break;
		case ENC_TYPE_MJPEG: 
			ret = setup_mj_decoder_parameter(dec_data, dec_ctrl); 
			break;
		case ENC_TYPE_MPEG: 
		    ret = setup_mp4_decoder_parameter(dec_data, dec_ctrl); 
		    break;
		default: 
	        panic("%s: dec_type=%d do not support.\n", __FUNCTION__, dec_data->dec_type);
	}
    if(ret<0)
        return -EFAULT;

#if TWO_STAGES_SCALER
    dec_data->b_two_scls = check_link_two_scaler(dec_ctrl->src_param.dim.width, dec_ctrl->src_param.dim.height,
                                        dec_ctrl->dst_param.win.width, dec_ctrl->dst_param.win.height,
                                        dec_data->pb_scl2_qst.size, dec_ctrl->src_param.bs_rate,
                                        dec_ctrl->dst_param.display_rate );
#else
    dec_data->b_two_scls = FALSE;
#endif
    ret = setup_display_paramerter(dec_data, dec_ctrl, dec_data->b_two_scls);
    if(ret<0)
        return -EFAULT;

    display_rate=dec_ctrl->dst_param.display_rate;
    bs_rate=dec_ctrl->src_param.bs_rate;
    
    dec_data->display_rate=display_rate;
    dec_data->bs_rate=bs_rate;


    if(display_rate==0)
        panic("Error display rate in playback_start_graph\n");
    if(bs_rate==0)
        panic("Error bs rate in playback_start_graph\n");

    //setup graph line
    if(dec_data->is_use_scaler) {
        dec_in_q = dec_data->dec_in_qst.que;
        pb_scl_in_q = dec_data->pb_scl_qst.que;
        lcd_q = dec_data->lcd_qst.que;

        scl_in_size = dec_ctrl->src_param.dim.width*dec_ctrl->src_param.dim.height*2;
        scl_in_cnt = (dec_data->pb_scl_qst.size*dec_data->pb_scl_qst.count)/scl_in_size;
        dec_in_cnt = dec_data->dec_in_qst.count;
        if(scl_in_cnt>bs_rate)
            scl_in_cnt = bs_rate;
        if(dec_in_cnt>bs_rate)
            dec_in_cnt = bs_rate;
        if(scl_in_cnt<2)
            scl_in_cnt=2;
        if(scl_in_cnt > dec_data->pb_scl_qst.limit_count)
            scl_in_cnt = dec_data->pb_scl_qst.limit_count;
        dvr_queue_rearrange(ptag, pb_scl_in_q, scl_in_cnt, scl_in_size, FALSE);
        dvr_queue_rearrange(ptag, dec_in_q, dec_in_cnt, dec_data->dec_in_qst.size, FALSE);

#if TWO_STAGES_SCALER
        pb_scl2_in_q = dec_data->pb_scl2_qst.que;

        if(dec_data->b_two_scls)
        {
            dvr_graph_add_link(dec_data->graph, ptag, dec_data->datain_fd, bs_rate, dec_data->dec_fd, bs_rate, dec_in_q);
            dvr_graph_add_link(dec_data->graph, ptag, dec_data->dec_fd, bs_rate, dec_data->scl_fd, bs_rate, pb_scl_in_q);
            dvr_graph_add_link(dec_data->graph, ptag_scl, dec_data->scl_fd, bs_rate, dec_data->scl2_fd, display_rate, pb_scl2_in_q);
            dvr_graph_add_link(dec_data->graph, ptag_scl, dec_data->scl2_fd, display_rate, dec_data->disp_fd, display_rate, lcd_q);
            if(dec_data->disp_info.dup_disp)
                dvr_graph_add_link(dec_data->graph, ptag_scl, dec_data->scl2_fd, display_rate, dec_data->disp2_fd, display_rate, lcd_q);
            dvr_graph_setfree(ptag_scl, dec_data->scl_fd, FREE_BY_PUT, 0);
            dvr_graph_setfree(ptag_scl, dec_data->scl2_fd, FREE_BY_PUT, 0);
        }
        else
#endif /* TWO_STAGES_SCALER */
       {
          dvr_graph_add_link(dec_data->graph, ptag, dec_data->datain_fd, bs_rate, dec_data->dec_fd, bs_rate, dec_in_q);
          dvr_graph_add_link(dec_data->graph, ptag, dec_data->dec_fd, bs_rate, dec_data->scl_fd, display_rate, pb_scl_in_q);
          dvr_graph_add_link(dec_data->graph, ptag_scl, dec_data->scl_fd, display_rate, dec_data->disp_fd, display_rate, lcd_q);
          if(dec_data->disp_info.dup_disp)
              dvr_graph_add_link(dec_data->graph, ptag_scl, dec_data->scl_fd, display_rate, dec_data->disp2_fd, display_rate, lcd_q);
          dvr_graph_setfree(ptag_scl, dec_data->scl_fd, FREE_BY_PUT, 0);
       }
    }
    else
    {
        dec_in_q = dec_data->dec_in_qst.que;
        lcd_q = dec_data->lcd_qst.que;
        dvr_graph_add_link(dec_data->graph, ptag, dec_data->datain_fd, bs_rate, dec_data->dec_fd, bs_rate, dec_in_q);
        dvr_graph_add_link(dec_data->graph, ptag, dec_data->dec_fd, bs_rate, dec_data->disp_fd, display_rate, lcd_q);
        if(dec_data->disp_info.dup_disp)
            dvr_graph_add_link(dec_data->graph, ptag, dec_data->dec_fd, bs_rate, dec_data->disp2_fd, display_rate, lcd_q);
    }

    idatain_set_flow_rate(dec_data->datain_fd,bs_rate);

    //keep current setting
    memcpy(&dec_data->dec_ctrl_data, dec_ctrl, sizeof(dvr_dec_control));
#if TWO_STAGES_SCALER
    if(dec_data->b_two_scls)
        dvr_dec_clear_win(dec_data,dec_data->scl2_fd,
            dec_data->lcd_dim.width,dec_data->lcd_dim.height,
            dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
            dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);
    else
#endif    
        dvr_dec_clear_win(dec_data,dec_data->scl_fd,
            dec_data->lcd_dim.width,dec_data->lcd_dim.height,
            dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
            dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}


int set_playback_stop_graph(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
    FuncTag *ptag;

    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    ptag = &dec_data->tag;

    dvr_graph_del_link(dec_data->graph, ptag);

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}


int update_playback_graph_param(dvr_dec_data *dec_data, dvr_dec_control *dec_ctrl)
{
#if TWO_STAGES_SCALER
    int b_two_scls = FALSE;
#endif    
    int need_restart=FALSE, need_update_input_rate=FALSE, need_update_param=FALSE;
    FuncTag tag2;
    dvr_dec_control *my_dec_ctrl = &dec_data->dec_ctrl_data;
    
    VDBGPRINT(DBG_DVR_FNC, " %s -- IN\n", __FUNCTION__);

    if(dec_ctrl->src_param.dim.width!=GMVAL_DO_NOT_CARE)    
    {
        if(my_dec_ctrl->src_param.dim.width!=dec_ctrl->src_param.dim.width)     need_restart=TRUE;
        my_dec_ctrl->src_param.dim.width = dec_ctrl->src_param.dim.width;
    }
    if(dec_ctrl->src_param.dim.height!=GMVAL_DO_NOT_CARE)    
    {
        if(my_dec_ctrl->src_param.dim.height != dec_ctrl->src_param.dim.height) need_restart=TRUE;
        my_dec_ctrl->src_param.dim.height = dec_ctrl->src_param.dim.height;
    }
    if(dec_ctrl->src_param.win.x!=GMVAL_DO_NOT_CARE)    
    {
        if(my_dec_ctrl->src_param.win.x != dec_ctrl->src_param.win.x)           need_update_param=TRUE;
        my_dec_ctrl->src_param.win.x = dec_ctrl->src_param.win.x;
    }
    if(dec_ctrl->src_param.win.y!=GMVAL_DO_NOT_CARE)    
    {
        if(my_dec_ctrl->src_param.win.y != dec_ctrl->src_param.win.y)           need_update_param=TRUE;
        my_dec_ctrl->src_param.win.y = dec_ctrl->src_param.win.y;
    }
    if(dec_ctrl->src_param.win.width!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->src_param.win.width != dec_ctrl->src_param.win.width)   need_update_param=TRUE;
        my_dec_ctrl->src_param.win.width = dec_ctrl->src_param.win.width;
    }
    if(dec_ctrl->src_param.win.height!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->src_param.win.height != dec_ctrl->src_param.win.height) need_update_param=TRUE;
        my_dec_ctrl->src_param.win.height = dec_ctrl->src_param.win.height;
    }
    if(dec_ctrl->src_param.bs_rate!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->src_param.bs_rate != dec_ctrl->src_param.bs_rate)       need_update_input_rate = TRUE;
        my_dec_ctrl->src_param.bs_rate = dec_ctrl->src_param.bs_rate;
    }
    if(dec_ctrl->dst_param.plane_id!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.plane_id != dec_ctrl->dst_param.plane_id)     need_update_param=TRUE;
        my_dec_ctrl->dst_param.plane_id = dec_ctrl->dst_param.plane_id;
    }
    if(dec_ctrl->dst_param.win.x!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.win.x != dec_ctrl->dst_param.win.x)           need_update_param=TRUE;
        my_dec_ctrl->dst_param.win.x = dec_ctrl->dst_param.win.x;
    }
    if(dec_ctrl->dst_param.win.y!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.win.y != dec_ctrl->dst_param.win.y)           need_update_param=TRUE;
        my_dec_ctrl->dst_param.win.y = dec_ctrl->dst_param.win.y;
    }
    if(dec_ctrl->dst_param.win.width!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.win.width != dec_ctrl->dst_param.win.width)   need_update_param=TRUE;
        my_dec_ctrl->dst_param.win.width = dec_ctrl->dst_param.win.width;
    }
    if(dec_ctrl->dst_param.win.height!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.win.height != dec_ctrl->dst_param.win.height) need_update_param=TRUE;
        my_dec_ctrl->dst_param.win.height = dec_ctrl->dst_param.win.height;
    }
    if(dec_ctrl->dst_param.is_display!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.is_display != dec_ctrl->dst_param.is_display) need_update_param=TRUE;
        my_dec_ctrl->dst_param.is_display = dec_ctrl->dst_param.is_display;
    }
    if(dec_ctrl->dst_param.display_rate!=GMVAL_DO_NOT_CARE)
    {
        if(my_dec_ctrl->dst_param.display_rate != dec_ctrl->dst_param.display_rate)    need_update_param=TRUE;
        my_dec_ctrl->dst_param.display_rate = dec_ctrl->dst_param.display_rate;
    }

    if(need_restart)
    {
        FN_COPY_TAG(&tag2, &dec_data->tag);
        FN_SET_FUNC(&tag2, FN_UPDATE_METHOD);

        dvr_graph_del_link(dec_data->graph, &tag2);
        set_playback_start_graph(dec_data, my_dec_ctrl, TRUE);
    }
    else
    {
        if(need_update_param)
        {
#if TWO_STAGES_SCALER
            struct v_queue_t    *pb_scl2_in_q, *lcd_q;
            b_two_scls = check_link_two_scaler(dec_ctrl->src_param.dim.width, dec_ctrl->src_param.dim.height,
                                                dec_ctrl->dst_param.win.width, dec_ctrl->dst_param.win.height,
                                                dec_data->pb_scl2_qst.size, dec_ctrl->src_param.bs_rate,
                                                dec_ctrl->dst_param.display_rate );

            if(dec_ctrl->dst_param.is_display && b_two_scls && !dec_data->b_two_scls)
            {
                //insert secondary scaler
                pb_scl2_in_q = dec_data->pb_scl2_qst.que;
                lcd_q = dec_data->lcd_qst.que;
                dvr_graph_del_link(dec_data->graph, &dec_data->tag_scl);
                dvr_graph_add_link(dec_data->graph, &dec_data->tag_scl, dec_data->scl_fd, dec_data->bs_rate, dec_data->scl2_fd, dec_data->display_rate, pb_scl2_in_q);
                dvr_graph_add_link(dec_data->graph, &dec_data->tag_scl, dec_data->scl2_fd, dec_data->display_rate, dec_data->disp_fd, dec_data->display_rate, lcd_q);
                if(dec_data->disp_info.dup_disp)
                    dvr_graph_add_link(dec_data->graph, &dec_data->tag_scl, dec_data->scl2_fd, dec_data->display_rate, dec_data->disp2_fd, dec_data->display_rate, lcd_q);
                dvr_graph_setfree(&dec_data->tag_scl, dec_data->scl_fd, FREE_BY_PUT, 0);
                dvr_graph_setfree(&dec_data->tag_scl, dec_data->scl2_fd, FREE_BY_PUT, 0);
                dec_data->b_two_scls = TRUE;
            }
            else if((!dec_ctrl->dst_param.is_display && dec_data->b_two_scls) || (!b_two_scls && dec_data->b_two_scls))
            {
                //remove secondary scaler
                lcd_q = dec_data->lcd_qst.que;
                dvr_graph_del_link(dec_data->graph, &dec_data->tag_scl);
                dvr_graph_add_link(dec_data->graph, &dec_data->tag_scl, dec_data->scl_fd, dec_data->display_rate, dec_data->disp_fd, dec_data->display_rate, lcd_q);
                if(dec_data->disp_info.dup_disp)
                    dvr_graph_add_link(dec_data->graph, &dec_data->tag_scl, dec_data->scl_fd, dec_data->display_rate, dec_data->disp2_fd, dec_data->display_rate, lcd_q);
                dvr_graph_setfree(&dec_data->tag_scl, dec_data->scl_fd, FREE_BY_PUT, 0);
                dec_data->b_two_scls = FALSE;
            }
#endif
            setup_display_paramerter(dec_data, my_dec_ctrl, dec_data->b_two_scls);
        }
        if(need_update_input_rate)
            update_bs_rate(dec_data, my_dec_ctrl->src_param.bs_rate);
#if TWO_STAGES_SCALER
        if(dec_data->b_two_scls)
            dvr_dec_clear_win(dec_data,dec_data->scl2_fd,
                dec_data->lcd_dim.width,dec_data->lcd_dim.height,
                dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
                dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);
        else
#endif        
            dvr_dec_clear_win(dec_data,dec_data->scl_fd,
                dec_data->lcd_dim.width,dec_data->lcd_dim.height,
                dec_ctrl->dst_param.win.x,dec_ctrl->dst_param.win.y,
                dec_ctrl->dst_param.win.width,dec_ctrl->dst_param.win.height);

    }

    VDBGPRINT(DBG_DVR_FNC, " %s -- OUT\n", __FUNCTION__);

    return 0;
}



static int get_channel_param_data(dvr_dec_data *dec_data, dvr_dec_channel_param *ch_param)
{
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)ch(%d)] %s -- IN\n", (int)dec_data, dec_data->ch_num, __FUNCTION__);

    memcpy(ch_param, &dec_data->ch_param_data, sizeof(dvr_dec_channel_param));

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)ch(%d)] %s -- OUT\n", (int)dec_data, dec_data->ch_num, __FUNCTION__);
    return 0;
}

static int check_set_channel_param_data(dvr_dec_data *dec_data, dvr_dec_channel_param *ch_param)
{
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)ch(%d)] %s -- IN\n", (int)dec_data, dec_data->ch_num, __FUNCTION__);

    if(ch_param->dec_type<0 || ch_param->dec_type>=ENC_TYPE_COUNT)
    {
        printk("Error! Invalid decode type(%d)!\n", ch_param->dec_type);
        return -1;
    }
    if(ch_param->dec_dest_type<0 || ch_param->dec_dest_type>=DEC_TYPE_COUNT)
    {
        printk("Error! Invalid dest. type(%d)!\n", ch_param->dec_dest_type);
        return -1;
    }
    if(ch_param->channel<0 || ch_param->channel>=DVR_PLAYBACK_CHANNEL_NUM)
    {
        printk("Error! Invalid channel number(%d)!\n", ch_param->channel);
        return -1;
    }
    if(ch_param->is_blocked!=TRUE && ch_param->is_blocked!=FALSE)
    {
        printk("Error! Invalid is_blocked value(%d)!\n", ch_param->is_blocked);
        return -1;
    }
    if(ch_param->is_use_scaler!=TRUE && ch_param->is_use_scaler!=FALSE)
    {
        printk("Error! Invalid is_use_scaler value(%d)!\n", ch_param->is_use_scaler);
        return -1;
    }
    if(ch_param->dec_param.output_type!=DEC_OUTPUT_COLOR_YUV422)
    {
        printk("Error! dec_param.output_type value(%d) is not DEC_OUTPUT_COLOR_YUV422!\n", ch_param->dec_param.output_type);
        return -1;
    }
    if(ch_param->scl_param.src_fmt!=DEC_OUTPUT_COLOR_YUV422 || ch_param->scl_param.dst_fmt!=DEC_OUTPUT_COLOR_YUV422 
       || (ch_param->scl_param.scale_mode<0 || ch_param->scl_param.scale_mode>=SCALE_METHOD_COUNT)
       || (ch_param->scl_param.is_dither!=TRUE && ch_param->scl_param.is_dither!=FALSE)
       || (ch_param->scl_param.is_correction!=TRUE && ch_param->scl_param.is_correction!=FALSE)
       || (ch_param->scl_param.is_album!=TRUE && ch_param->scl_param.is_album!=FALSE)  )
    {
        printk("Error! Wrong scaler parameters!\n");
        return -1;
    }
    dec_data->ch_num = ch_param->channel;
    memcpy(&dec_data->ch_param_data, ch_param, sizeof(dvr_dec_channel_param));

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)ch(%d)] %s -- OUT\n", (int)dec_data, dec_data->ch_num, __FUNCTION__);
    return 0;
}


static int check_dec_ctrl_data(dvr_dec_control *dec_ctrl)
{
    if(dec_ctrl->command<0 || dec_ctrl->command>2)
    {
        printk("Error! Invalid dvr_dec_control command(%d)!\n", dec_ctrl->command);
        return -1;
    }
    if(dec_ctrl->command!=DEC_STOP)
    {
        if( (dec_ctrl->src_param.dim.width<=0 && dec_ctrl->src_param.dim.width!=GMVAL_DO_NOT_CARE)
          ||(dec_ctrl->src_param.dim.height<=0 && dec_ctrl->src_param.dim.height!=GMVAL_DO_NOT_CARE) )
        {
            printk("Error! Invalid dvr_dec_control dim WH(%d,%d)!\n", dec_ctrl->src_param.dim.width, dec_ctrl->src_param.dim.height);
            return -1;
        }
        if( (dec_ctrl->src_param.win.x<0 && dec_ctrl->src_param.win.x!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->src_param.win.y<0 && dec_ctrl->src_param.win.y!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->src_param.win.width<=0 && dec_ctrl->src_param.win.width!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->src_param.win.height<=0 && dec_ctrl->src_param.win.height!=GMVAL_DO_NOT_CARE) )
        {
            printk("Error! Invalid dvr_dec_control win XY(%d,%d), WY(%d,%d)!\n",
                dec_ctrl->src_param.win.x, dec_ctrl->src_param.win.y, dec_ctrl->src_param.win.width, dec_ctrl->src_param.win.height);
            return -1;
        }
        if(dec_ctrl->src_param.bs_rate<=0 && dec_ctrl->src_param.bs_rate!=GMVAL_DO_NOT_CARE)
        {
            printk("Error! Invalid dvr_dec_control bs_rate(%d)!\n", dec_ctrl->src_param.bs_rate);
            return -1;
        }
        if( (dec_ctrl->dst_param.win.x<0 && dec_ctrl->dst_param.win.x!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->dst_param.win.y<0 && dec_ctrl->dst_param.win.y!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->dst_param.win.width<=0 && dec_ctrl->dst_param.win.width!=GMVAL_DO_NOT_CARE)
          || (dec_ctrl->dst_param.win.height<=0&& dec_ctrl->dst_param.win.height!=GMVAL_DO_NOT_CARE) )
        {
            printk("Error! Invalid dvr_dec_control win XY(%d,%d), WY(%d,%d)!\n",
                dec_ctrl->dst_param.win.x, dec_ctrl->dst_param.win.y, dec_ctrl->dst_param.win.width, dec_ctrl->dst_param.win.height);
            return -1;
        }
        if(dec_ctrl->dst_param.is_display!=TRUE && dec_ctrl->dst_param.is_display!=FALSE && dec_ctrl->dst_param.is_display!=GMVAL_DO_NOT_CARE)
        {
            printk("Error! Invalid is_display value(%d)!\n", dec_ctrl->dst_param.is_display);
            return -1;
        }
        if(dec_ctrl->dst_param.display_rate<=0 && dec_ctrl->dst_param.display_rate!=GMVAL_DO_NOT_CARE)
        {
            printk("Error! Invalid dvr_dec_control display_rate(%d)!\n", dec_ctrl->dst_param.display_rate);
            return -1;
        }
    }
    return 0;
}



static int do_queue_get(dvr_dec_data *dec_data, dvr_dec_queue_get *data)
{
    int ret;
    struct v_job_t *job;
    
    //printm(">>>>>>>[dvr_dec:%d] %s \n", dec_data->ch_num, __FUNCTION__);

TRY_DATAIN_GET_AGAIN:
    ret = idatain_get(dec_data->ch_num, &job);

    if(ret==0)
    {
        data->bs.offset = video_get_buffer_remap_offset(job->out_buf);
        data->bs.length = video_get_buffer_size(job->out_buf);
        data->bs.p_job = (void*) job;
    }
    else if(dec_data->is_blocked && ret==-2)
    {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAIN_GET_AGAIN;
    }
    else
    {
        return -1;
    }

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_dec:%d]: QUEUE GET, job(%d) offset(0x%x) length(%d)\n", dec_data->ch_num, job->id, data->bs.offset, data->bs.length);
    return 0;
}

void fill_datain_job(struct v_job_t *job, int data_offset, int data_size)
{
    struct v_graph_info  *graph_info = (struct v_graph_info*) job->out_header->addr_va;
    Datain_info *datain_info = (Datain_info*) (job->out_header->addr_va + VIDEOGRAPH_LEN/*sizeof(struct v_graph_info)*/);

    graph_info->drvinfo_length = sizeof(Datain_info);

    datain_info->DatainTag = GMDVR_MAKE_FOURCC('D','T','I','N');
    datain_info->DataSize = data_size;
    datain_info->DataOffset = 0/*0x1000*/;
}


/* do_queue_put() returns the count of jobs that really callback for vg. */
static int do_queue_put(dvr_dec_data *dec_data, dvr_dec_queue_get *data)
{
    int ret, st=0;
    unsigned char for_sync;

    if(data->bs.p_job == NULL)
        return -1;

    /* read the last byte to flush buffer */
    if(data->bs.length>0 && data->bs.length<=video_get_buffer_size(((struct v_job_t*)data->bs.p_job)->out_buf) )
    {
        for_sync = *( (unsigned char*)(dec_data->vma_start+data->bs.offset+data->bs.length-1) );
        fill_datain_job((struct v_job_t*)data->bs.p_job, data->bs.offset, data->bs.length);
    }
    else
        st=-1;
    ret = idatain_put(dec_data->ch_num, (struct v_job_t*)data->bs.p_job, st);

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_dec:%d]: QUEUE PUT, job(%d), st(%d), ret(%d)\n",
        dec_data->ch_num, ((struct v_job_t*)data->bs.p_job)->id, st, ret);

    return ret;
}

static int do_queue_copy(dvr_dec_data *pt_dec_data, dvr_dec_copy_buf *pt_data)
{
    int ret, st=0;
    unsigned char for_sync;   
    struct v_job_t *p_job;
    int offset,length;
    unsigned char *p_buf;
    
    ret = idatain_get(pt_dec_data->ch_num, &p_job);
    if(ret==0){
        offset = video_get_buffer_remap_offset(p_job->out_buf);
        length = video_get_buffer_size(p_job->out_buf);
    }
    else{
        printk("[ERROR]data in get fail(%d)\n",ret);
        return -1;
    }

    if(pt_data->bs_buf_length>0 && pt_data->bs_buf_length<=length){
        for_sync = *( (unsigned char*)(pt_dec_data->vma_start+offset+length-1) );
        p_buf=(unsigned char*)pt_dec_data->vma_start;
        if(copy_from_user((unsigned char *)((unsigned char*)p_buf+offset), (unsigned char *)pt_data->p_bs_buf, pt_data->bs_buf_length)) { /* copy buf content to que */
            st=-1;
            printk("[ERROE]copy_from_user fail, bs_buf_length=0x%x length=0x%x\n",pt_data->bs_buf_length,length);
	    } else {
            //fill_datain_job(p_job, offset, length);
            fill_datain_job(p_job, offset, pt_data->bs_buf_length);
        }
    }
    else{
        if(pt_data->bs_buf_length!=0)
            printk("[ERROE]buf is too small bs_buf_length=0x%x length=0x%x\n",pt_data->bs_buf_length,length);
        st=-1;
    }
        
    ret = idatain_put(pt_dec_data->ch_num, p_job, st);
    if(ret<0){
        printk("[ERROR]data in put job fail(%d)\n",ret);
        return ret;
    }

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_dec:%d]: QUEUE COPY, job(%d) offset(0x%x) length(%d)\n", pt_dec_data->ch_num, p_job->id, offset, length);
    
    return ret;
}

int dvr_dec_open(struct inode *inode, struct file *filp)
{
    dvr_dec_data  *dec_data=NULL;
    int i;

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:(undefined)] %s -- IN\n", __FUNCTION__);

	down(&dvr_dec_oper_sem);
	
    for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
    {
        if(dec_private_data[i].is_used == FALSE)
        {
            dec_data = &dec_private_data[i];
            break;
        }
    }
    if(i==DVR_PLAYBACK_CHANNEL_NUM)
    {
        printk("Reach MAX dec entity count!\n");
        goto DVR_DEC_OPEN_EXIT;
    }

    filp->private_data = dec_data;
    dec_data->is_used = TRUE;

    dec_data->dec_type = 0;
    dec_data->dest_type = 0;
    dec_data->ch_num = -1;
    dec_data->is_blocked = FALSE;
    dec_data->bs_rate = 30;
    dec_data->datain_fd = 0;
    dec_data->dec_fd = 0;
    dec_data->is_use_scaler = TRUE;
    dec_data->graph = NULL;
    dec_data->wait_queue_stream = NULL;

    dvr_common_retrieve_info(INFO_INDEX_DISP_COUNT, &dec_data->disp_info);

    up(&dvr_dec_oper_sem);

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:entry(%x)] %s -- OUT\n", (int)dec_data, __FUNCTION__);

    return 0;

DVR_DEC_OPEN_EXIT:
    up(&dvr_dec_oper_sem);
    return EFAULT;

}

int dvr_dec_release(struct inode *inode, struct file *filp)
{
    int ret;
    FuncTag *ptag;
    dvr_dec_data  *dec_data = filp->private_data;
    down(&dec_data->dvr_dec_sem);

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- IN\n", dec_data->ch_num, __FUNCTION__);

    ptag = &dec_data->tag;
    if(dvr_graph_del_link(dec_data->graph, ptag))
    {
        ret = dvr_graph_apply_by_tag(ptag);
        if(ret<0)
        {
            up(&dec_data->dvr_dec_sem);
            return -1;
        }
    }

    printm("#### dvr_dec_release ch%d dec_data->dest_type=%d\n",dec_data->ch_num,dec_data->dest_type);
    idatain_reset(dec_data->ch_num);   // must move all job back to graph...
    idatain_close(dec_data->ch_num);

    if(dec_data->dest_type != DEC_TYPE_TO_DISPLAY)
    {
        dvr_graph_destroy(dec_data->graph);
    }
    dec_data->graph = NULL;

    dec_data->is_used = FALSE;
    //memset(dec_data, 0x0, sizeof(dvr_dec_data));

    up(&dec_data->dvr_dec_sem);

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);

    return 0;
}

int dvr_dec_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long size;
    dvr_dec_data  *dec_data = filp->private_data;
	int ret = 0;

    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- IN\n", dec_data->ch_num, __FUNCTION__);

	if (!(vma->vm_flags & VM_WRITE)) {
		printk("dvr_dec_mmap: please use PROT_WRITE in mmap()\n");
		ret = -EINVAL;
		goto EXIT_DVR_DEC_MMAP;
	}
	if (!(vma->vm_flags & VM_SHARED)) {
		printk("dvr_dec_mmap: please use MAP_SHARED in mmap()\n");
		ret = -EINVAL;
		goto EXIT_DVR_DEC_MMAP;
	}

	/* memory policy: non-cacheable and bufferable */
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
    dec_data->vma_start=vma->vm_start;
    
    size=video_remap_pfn_range(dec_data->dec_in_qst.que,vma,vma->vm_start,vma->vm_page_prot);

EXIT_DVR_DEC_MMAP:
    VDBGPRINT(DBG_DVR_FNC, "[dvr_dec:%d] %s -- OUT\n", dec_data->ch_num, __FUNCTION__);

	return ret;

}

static unsigned int dvr_dec_poll(struct file *filp, poll_table *wait)
{
    unsigned int mask = 0;
    dvr_dec_data  *dec_data = filp->private_data;

    poll_wait(filp, dec_data->wait_queue_stream, wait);
    if( idatain_getlength(dec_data->ch_num))
    {
        mask |= POLLIN | POLLRDNORM;
    }
	return mask;
}


static int dvr_dec_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret=-1, val;
    dvr_dec_data  *dec_data = filp->private_data;
    dvr_info_buf_flow_info  buf_flow_info;


    down(&dec_data->dvr_dec_sem);
    if(dec_data->ch_num>=0 && cmd==DVR_DEC_CONTROL)
    {
        buf_flow_info.fnc = BFI_FNC_PB(dec_data->ch_num);
        val = dvr_common_retrieve_info(INFO_CHECK_BUF_FLOW_PAIR, &buf_flow_info);
        if(val!=0)
        {
            printk("DVR_DEC_QUEUE_GET and DVR_DEC_QUEUE_PUT are not in pairs(in-out:%d).\n", val);
            up(&dec_data->dvr_dec_sem);
            return -2;
        }
    }

    switch(cmd)
    {
        case DVR_DEC_QUERY_OUTPUT_BUFFER_SIZE:
        {
            int   buf_size;	        
	        if(copy_from_user((void *)&buf_size, (void *)arg, sizeof(int))) {
	            ret = -EFAULT;	        
			    goto exit1;
		    }
	        
	        if(dec_data->dec_in_qst.que)
                buf_size = PAGE_ALIGN(video_buf_arrange_size(dec_data->dec_in_qst.que));
            
            ret = 0;            
            
            if(copy_to_user((void *)arg, (void *)&buf_size,sizeof(int))) {
                ret = -EFAULT;	        
			    goto exit1;
		    }
        
            memcpy(&dec_data->la_buf_size, &buf_size, sizeof(int));
            break;
        }

        case DVR_DEC_GET_CHANNEL_PARAM:
        {
            dvr_dec_channel_param   ch_param;		    
		    if(copy_from_user((void *)&ch_param, (void *)arg, sizeof(dvr_dec_channel_param))) {
		        ret = -EFAULT;	           
		        goto exit1;
		    }
            ret = get_channel_param_data(dec_data, &ch_param);
		    if(ret >= 0) {            
                if(copy_to_user((void *)arg,(void *)&ch_param,sizeof(dvr_dec_channel_param))) {
                    ret = -EFAULT;	           
		            goto exit1;
                }
            memcpy(&dec_data->la_ch_param_get, &ch_param, sizeof(dvr_dec_channel_param));
            }
            break;
        }
        
        case DVR_DEC_SET_CHANNEL_PARAM:
        {
            dvr_dec_channel_param   ch_param;		    
		    if(copy_from_user((void *)&ch_param, (void *)arg, sizeof(dvr_dec_channel_param))) {
		        ret = -EFAULT;	           
		        goto exit1;
		    }
            memcpy(&dec_data->la_ch_param_set, &ch_param, sizeof(dvr_dec_channel_param));
            
            ret = check_set_channel_param_data(dec_data, &ch_param);
		    if(ret >= 0) {                
                if(copy_to_user((void *)arg,(void *)&ch_param,sizeof(dvr_dec_channel_param))) {
                    ret = -EFAULT;	           
		            goto exit1;
                }
            }
            break;
        }
#if 0
        case DVR_DEC_DEBUG_PRINT:
        {
            char str[50];            
            if(copy_from_user((void *)str, (void *)arg, 50)) {
                ret = -EFAULT;	           
		        goto exit1;
            }                        
            printm("%s",str);
            break;
        }
#endif    
        case DVR_DEC_QUEUE_GET:
        {
            dvr_dec_queue_get   data;		    
		    if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_dec_queue_get))) {
		        ret = -EFAULT;	           
		        goto exit1;
		    }
            ret = do_queue_get(dec_data, &data);
		    if(ret >= 0) {
		        data.channel=dec_data->ch_num;            
                if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_dec_queue_get))) {
                    ret = -EFAULT;	           
		            goto exit1;
                }

#if LOG_API_QUEUE_FLOW
            memcpy(&dec_data->la_q_data_get, &data, sizeof(dvr_dec_queue_get));
#endif
                update_perf(&gperf_info[dec_data->ch_num]);
            }
            show_perf();
            break;
        }

        case DVR_DEC_QUEUE_PUT:
        {
            dvr_dec_queue_get   data;		    
		    if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_dec_queue_get))) {
		        ret = -EFAULT;	           
		        goto exit1;
		    }
#if LOG_API_QUEUE_FLOW
        memcpy(&dec_data->la_q_data_put, &data, sizeof(dvr_dec_queue_get));
#endif
            ret = do_queue_put(dec_data, &data);
		    if(ret >= 0) {                
                if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_dec_queue_get))) {
                    ret = -EFAULT;	           
		            goto exit1;
                }
            }
            if(bShowPerf)           
                update_latance(&gperf_info[dec_data->ch_num]);           
            break;
        }
        case DVR_DEC_PUT_COPY:
        {
            dvr_dec_copy_buf t_data;

            if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_dec_copy_buf))) {
                ret = -EFAULT;
                goto exit1;
            }
            ret = do_queue_copy(dec_data, &t_data);
            break;
        }

        case DVR_DEC_CONTROL:
        {
            dvr_dec_control dec_ctrl;		    
		    if(copy_from_user((void *)&dec_ctrl, (void *)arg, sizeof(dvr_dec_control))) {
		        ret = -EFAULT;	           
		        goto exit1;
		    }
            memcpy(&dec_data->la_dec_ctrl, &dec_ctrl, sizeof(dvr_dec_control));
            if((ret=check_dec_ctrl_data(&dec_ctrl)) >= 0) {
                if(dec_ctrl.command == DEC_START)
                    ret = set_playback_start_graph(dec_data, &dec_ctrl, FALSE);
                else if(dec_ctrl.command == DEC_STOP)
                    ret = set_playback_stop_graph(dec_data, &dec_ctrl);
                else if(dec_ctrl.command == DEC_UPDATE)
                    ret = update_playback_graph_param(dec_data, &dec_ctrl);
                else
                    printk("[dvr_dec] Wrong DVR_DEC_CONTROL command, cmd(%d)\n", dec_ctrl.command);
            }
            break;
        }

        case DVR_DEC_CLEAR_WIN:
        {
/*
    ioctl(dvr_fd,DVR_COMMON_APPLY,&data);   //stop currnet playback
    ...
    ioctl(dec_fd,DVR_DEC_CLEAR_WIN,&data);  //must after stop, and before start apply
    ioctl(dvr_fd,DVR_COMMON_APPLY,&data);   //start new playback
    
    After apply, buffer will be clear if data not arrival
 */
            dvr_dec_clear_param dec_clear_param;            
            if(copy_from_user((void *)&dec_clear_param, (void *)arg, sizeof(dvr_dec_clear_param))) {
                ret = -EFAULT;	           
		        goto exit1;
            }
            memcpy(&dec_data->la_dec_clear_param, &dec_clear_param, sizeof(dvr_dec_clear_param));

            dec_data->clear_trigger=SET_CLEAR_TRIGGER;
            dec_data->clear_pattern=dec_clear_param.pattern;
            ret = 0;
            break;
        }

        case DVR_DEC_CLEAR_WIN2:
        {
            dvr_dec_clear_param dec_clear_param;            
            if(copy_from_user((void *)&dec_clear_param, (void *)arg, sizeof(dvr_dec_clear_param))) {
                ret = -EFAULT;	           
		        goto exit1;
            }
            memcpy(&dec_data->la_dec_clear_param2, &dec_clear_param, sizeof(dvr_dec_clear_param));
            ret = do_clear_win(dec_data, &dec_clear_param);
            break;
        }

        default:
            printk("dec_ioctl cmd(0x%x) no define!\n", cmd);
            break;
    }

exit1:
    up(&dec_data->dvr_dec_sem);

    return ret;
}


struct file_operations dvr_dec_fops = {
  	owner:      THIS_MODULE,
	ioctl:		dvr_dec_ioctl,
	mmap:       dvr_dec_mmap,
	open:		dvr_dec_open,
	release:	dvr_dec_release,
	poll:       dvr_dec_poll,
};

	
struct miscdevice dvr_dec_dev = {
	minor: MISC_DYNAMIC_MINOR,
	name: "dvr_dec",
	fops: &dvr_dec_fops,
};



static int __init dvr_dec_init(void)
{
    int i, ret=0;
    unsigned char tmp_str[32], tmp_str2[32];
    unsigned char str_dvr_dec[]="dvr_dec";
    int buf_size=0, count=0, ddr_num=0, limit_count=0;
    file_t *cfg_file;
    QueMemCfg   ch_cfg_dec;

    memset(dec_private_data, 0x0, sizeof(dec_private_data));

    // Read DDR config file
    cfg_file = gmcfg_open(GMDVR_MEM_CFG_FILE, O_RDONLY);
    if(!cfg_file)
    {
        printk("[dvr_dec] open [%s] failed!\n", GMDVR_MEM_CFG_FILE);
        return -EFAULT;
    }

    if(gmcfg_getfieldint( str_dvr_dec, "dec_in_size", &default_decq_cfg.size, cfg_file) < 0)   ret=-1;
    if(gmcfg_getfieldint( str_dvr_dec, "dec_in_count", &default_decq_cfg.count, cfg_file) < 0)   ret=-2;
    if(gmcfg_getfieldint( str_dvr_dec, "dec_in_ddr", &default_decq_cfg.ddr_num, cfg_file) < 0)   ret=-3;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl_in_size", &default_pb_sclq_cfg.size, cfg_file) < 0)   ret=-4;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl_in_count", &default_pb_sclq_cfg.count, cfg_file) < 0)   ret=-5;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl_in_ddr", &default_pb_sclq_cfg.ddr_num, cfg_file) < 0)   ret=-6;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl_in_limit_count", &default_pb_sclq_cfg.limit_count, cfg_file) < 0)   ret=-7;
#if (TWO_STAGES_SCALER && !SHARE_LV_PB_SCL_BUF)
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl2_in_size", &default_pb_scl2q_cfg.size, cfg_file) < 0)   ret=-101;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl2_in_count", &default_pb_scl2q_cfg.count, cfg_file) < 0)   ret=-102;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl2_in_ddr", &default_pb_scl2q_cfg.ddr_num, cfg_file) < 0)   ret=-103;
    if(gmcfg_getfieldint( str_dvr_dec, "pb_scl2_in_limit_count", &default_pb_scl2q_cfg.limit_count, cfg_file) < 0)   ret=-104;
#endif /* TWO_STAGES_SCALER */

    if(ret<0)
    {
        panic("[dvr_dec] Wrong config content! err(%d)\n", ret);
        return -EFAULT;
    }

    for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
    {
        snprintf(tmp_str2, 32, "dec_in%d_size", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.size, cfg_file);
        if(ret<0)   buf_size = default_decq_cfg.size;
        else        buf_size = ch_cfg_dec.size;
        snprintf(tmp_str2, 32, "dec_in%d_count", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.count, cfg_file);
        if(ret<0)   count = default_decq_cfg.count;
        else        count = ch_cfg_dec.count;
        snprintf(tmp_str2, 32, "dec_in%d_ddr", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.ddr_num, cfg_file);
        if(ret<0)   ddr_num = default_decq_cfg.ddr_num;
        else        ddr_num = ch_cfg_dec.ddr_num;

        if(count && buf_size)
        {
            snprintf(tmp_str, 32, "%s%d", QNAME_DEC_IN, i);
            dec_private_data[i].dec_in_qst.size = buf_size;
            dec_private_data[i].dec_in_qst.count = count;
            dec_private_data[i].dec_in_qst.ddr = ddr_num;
            dec_private_data[i].dec_in_qst.que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
        }

        snprintf(tmp_str2, 32, "pb_scl_in%d_size", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.size, cfg_file);
        if(ret<0)   buf_size = default_pb_sclq_cfg.size;
        else        buf_size = ch_cfg_dec.size;
        snprintf(tmp_str2, 32, "pb_scl_in%d_count", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.count, cfg_file);
        if(ret<0)   count = default_pb_sclq_cfg.count;
        else        count = ch_cfg_dec.count;
        snprintf(tmp_str2, 32, "pb_scl_in%d_ddr", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.ddr_num, cfg_file);
        if(ret<0)   ddr_num = default_pb_sclq_cfg.ddr_num;
        else        ddr_num = ch_cfg_dec.ddr_num;
        snprintf(tmp_str2, 32, "pb_scl_in%d_limit_count", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.limit_count, cfg_file);
        if(ret<0)   limit_count = default_pb_sclq_cfg.limit_count;
        else        limit_count = ch_cfg_dec.limit_count;

        if(count && buf_size)
        {
            snprintf(tmp_str, 32, "%s%d", QNAME_PB_SCL, i);
            dec_private_data[i].pb_scl_qst.size = buf_size;
            dec_private_data[i].pb_scl_qst.count = count;
            dec_private_data[i].pb_scl_qst.ddr = ddr_num;
            dec_private_data[i].pb_scl_qst.limit_count = limit_count;
            dec_private_data[i].pb_scl_qst.que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
        }

#if TWO_STAGES_SCALER
  #if (!SHARE_LV_PB_SCL_BUF)
        snprintf(tmp_str2, 32, "pb_scl2_in%d_size", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.size, cfg_file);
        if(ret<0)   buf_size = default_pb_scl2q_cfg.size;
        else        buf_size = ch_cfg_dec.size;
        snprintf(tmp_str2, 32, "pb_scl2_in%d_count", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.count, cfg_file);
        if(ret<0)   count = default_pb_scl2q_cfg.count;
        else        count = ch_cfg_dec.count;
        snprintf(tmp_str2, 32, "pb_scl2_in%d_ddr", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.ddr_num, cfg_file);
        if(ret<0)   ddr_num = default_pb_scl2q_cfg.ddr_num;
        else        ddr_num = ch_cfg_dec.ddr_num;
        snprintf(tmp_str2, 32, "pb_scl2_in%d_limit_count", i);
        ret = gmcfg_getfieldint( str_dvr_dec, tmp_str2, &ch_cfg_dec.limit_count, cfg_file);
        if(ret<0)   limit_count = default_pb_scl2q_cfg.limit_count;
        else        limit_count = ch_cfg_dec.limit_count;

        if(count && buf_size)
        {
            snprintf(tmp_str, 32, "%s%d", QNAME_PB_SCL2, i);
            dec_private_data[i].pb_scl2_qst.size = buf_size;
            dec_private_data[i].pb_scl2_qst.count = count;
            dec_private_data[i].pb_scl2_qst.ddr = ddr_num;
            dec_private_data[i].pb_scl2_qst.limit_count = limit_count;
            dec_private_data[i].pb_scl2_qst.que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
        }
  #endif /* !SHARE_LV_PB_SCL_BUF */
#endif /* TWO_STAGES_SCALER */

        init_MUTEX(&dec_private_data[i].dvr_dec_sem);
    }
    gmcfg_close( cfg_file ) ;

    // Add proc node
    dvr_dec_proc = create_proc_entry("dec_log", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_dec_proc==NULL)
        return -EFAULT;
    dvr_dec_proc->read_proc = (read_proc_t *) proc_read_mode;
    dvr_dec_proc->write_proc = (write_proc_t *) proc_write_mode;
    dvr_dec_proc->owner = THIS_MODULE;

    // Add show performance proc node
    dvr_dec_perf = create_proc_entry("dec_perf", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_dec_perf==NULL)
        return -EFAULT;
    dvr_dec_perf->proc_fops = &proc_perf_ops;
    dvr_dec_perf->owner = THIS_MODULE;

    proc_last_api = create_proc_entry("dec_api", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(proc_last_api==NULL)
        return -EFAULT;
    proc_last_api->read_proc = (read_proc_t *) proc_read_lastapi;
    proc_last_api->write_proc = (write_proc_t *) proc_write_lastapi;
    proc_last_api->owner = THIS_MODULE;

    idatain_init(DtiNotify);

    init_MUTEX(&dvr_dec_oper_sem);
    init_MUTEX(&dvr_dec_cfg_decoder_sem);

    //Set max size for decoder
    if(dec_max==1)
    {
        max_width = DEFAULT_D1_WIDTH;
        max_height = DEFAULT_D1_HEIGHT;
    }else if(dec_max==2)
    {
        max_width = DEFAULT_CIF_WIDTH;
        max_height = DEFAULT_CIF_HEIGHT;
    }else if(dec_max==3)
    {
        max_width = DEFAULT_QCIF_WIDTH;
        max_height = DEFAULT_QCIF_HEIGHT;
    }
    
    printk("Insert dvr_dec driver done.\n");

    dvrdec_wq = create_workqueue("dvrdec");
    
    for(i=0;i<MAX_DATAIN_ENTITY_NUM;i++)
        INIT_LIST_HEAD(&datain_callback_head[i]);
    
	return misc_register(&dvr_dec_dev);
}


static void __exit dvr_dec_clearnup(void)
{
    int i;

    for(i=0; i<DVR_PLAYBACK_CHANNEL_NUM; i++)
    {
        if(dec_private_data[i].dec_in_qst.que)
        {
            dvr_queue_destroy(dec_private_data[i].dec_in_qst.que);
            dec_private_data[i].dec_in_qst.que = NULL;
        }
        if(dec_private_data[i].pb_scl_qst.que)
        {
            dvr_queue_destroy(dec_private_data[i].pb_scl_qst.que);
            dec_private_data[i].pb_scl_qst.que = NULL;
        }
    }

    destroy_workqueue(dvrdec_wq);
    
    idatain_terminate();

    misc_deregister(&dvr_dec_dev);

    remove_proc_entry("dec_api", DvrProcRoot);
    remove_proc_entry("dec_perf", DvrProcRoot);
    remove_proc_entry("dec_log", DvrProcRoot);
}



module_init(dvr_dec_init);
module_exit(dvr_dec_clearnup);
MODULE_LICENSE("GPL");


