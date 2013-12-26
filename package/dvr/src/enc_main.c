
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/pci.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <mach/ftpmu010.h>
/*
    <No 3DI>            <3DI enabled>
         cap                  cap      
        /                    /
  enc_in_q              enc_in_q
        \                   \
         enc                  3di
        /                    /
  enc_out_q            enc_in_dup_q
        \                   \
         dout                enc
                            /
                       enc_out_q
                            \
                             dout

-------------------------------------------------------------------------------------

                           <No 3DI>            
                             cap              
                            /                <QCIF NONE-RealTime>          
                     enc_in_q -------------------+           
                    /       \                    \
             ss_enc0         enc0                 scl1  
           /                /                    /
 enc_out_snap0     enc_out_q0         sub_enc_in_q1
           \                \                    \
            ss_dout0         dout0                 enc1
                                                 /
                                      sub_enc_out_q1
                                                 \
                                                   dout1


                           <3DI enabled>
                                cap                              
                              /                                 
                        enc_in_q                                    
                              \                                       
                                3di                                   
                              /              <QCIF NONE-RealTime>          
                     enc_in_dup_q --------------------+                                           
                       /      \                        \                                
               ss_enc0          enc0                  scl1                              
              /               /                      /                                
  enc_out_snap0      enc_out_q0         sub_enc_in_q1                                
              \               \                      \      
               ss_dout0         dout0                  enc1 
                                                     /      
                                        sub_enc_out_q1       
                                                     \      
                                                       dout1

-------------------------------------------------------------------------------------
                           <TEST_CHIP 3DI enabled D1>
                                cap                              
                              /                                 
                        enc_in_q                                    
                              \                                       
                                3di                                   
                              /   
                     enc_in_dup_q 
                              \             
               					scl0        
                              /        	              <CIF>                      
                  	 ss_enc_in0  ---------------------->+
                     /        \        					 \
               ss_enc0          enc0                    scl1                              
              /               /                        /                                
  enc_out_snap0      enc_out_q0            sub_enc_in_q1 ---------------------->+                               
              \               \                        \                         \
               ss_dout0         dout0                   enc1(H264)               enc2(MPEG4)
                                                       /                         /
                                          sub_enc_out_q1             sub_enc_out_q2
                                                       \                         \
                                                        dout1                    dout2


                           <TEST_CHIP 3DI enabled D1>
                                cap                              
                              /                                 
                        enc_in_q                                    
                              \                                       
                                3di                                   
                              /   
                     enc_in_dup_q 
                              \             
               					scl0        
                              /        	              <CIF>                      
                  	 ss_enc_in0  ---------------------->+
                     /        \        					 \
               ss_enc0          enc0                    scl1                              
              /               /                        /                                
  enc_out_snap0      enc_out_q0            sub_enc_in_q1 ---------------------->+                               
              \               \                        \                         \
               ss_dout0         dout0                   enc1(H264)               enc2(MPEG4)
                                                       /                         /
                                          sub_enc_out_q1             sub_enc_out_q2
                                                       \                         \
                                                        dout1                    dout2


-------------------------------------------------------------------------------------
*/


#include "dvr_graph.h"
#include "enc_main.h"
#include "dvr_enc_api.h"
#include "dataout_entity.h"
#include "util_cfg.h"
#include "dvr_mod_define.h"
#include "dvr_disp_api.h"

#define GM8126_TEST_CHIP_VERSION 0x81261000 

#define FUNC_8CIF_DVR   0
#define FUNC_4D1_DVS    1
#define FUNC_1080P_IPC  2
#define MAX_VIDEO_SOURCE_NUM  2
#define ISP_CHANNEL_NU (4)
static unsigned int enc_func = FUNC_1080P_IPC; //0:8CIF-DVR 1:4D1-DVS 2:FUNC_1080P_IPC
module_param(enc_func, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(enc_func, "0:8CIF-DVR 1:4D1-DVS 2:FUNC_1080P_IPC");

static int bShareCapPath = 0;
module_param(bShareCapPath, int, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(bShareCapPath, "Share capture hardware for two resolution.");

static unsigned int vsrc = 1;
module_param(vsrc, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(vsrc, "Video Source number.");

struct semaphore    dvr_enc_open_sem;
struct semaphore    dvr_enc_cfg_encoder_sem;    // avoid encoder config reentry issue
struct semaphore    dvr_enc_snapshot_sem;   		 // avoid encoder config reentry issue
dvr_enc_data    enc_private_data[DVR_RECORD_CHANNEL_NUM];
QueMemCfg       default_encq_cfg, default_dtoq_cfg;
QueMemCfg       default_sub_encq_cfg, default_sub_dtoq_cfg, default_scl_dtoq_cfg;
DIM             default_max_dim, default_sub_max_dim, default_scl_max_dim;  //for codec max buffer 
QueMemCfg       default_ss_encq_cfg, default_ss_dtoq_cfg;

static struct proc_dir_entry *dvr_enc_max_fps=NULL; 

static struct proc_dir_entry *dvr_enc_proc=NULL; 
static struct proc_dir_entry *dvr_enc_perf=NULL; 
static struct proc_dir_entry *proc_last_api=NULL;
//int dvr_enc_dbgmode = 0x4;
int dvr_enc_dbgmode = 0;

int FrameRate_Denominator[MAX_VIDEO_SOURCE_NUM]={30,30};

static int last_api_num = 0;
static int last_api_ch = 0;
static unsigned char str_dvr_enc[]="dvr_enc";
unsigned int platform_version;

struct workqueue_struct *dvrenc_wq;
struct list_head dataout_callback_head[MAX_DATAOUT_ENTITY_NUM];

extern int p200_h264e_frame_pre(void *entity,void *rc_enc_param);
extern int p200_h264e_frame_post(void *entity,void *rc_rpt_param);
extern int mpeg4_frame_pre(void *entity,void *rc_enc_param);
extern int mpeg4_frame_post(void *entity,void *rc_rpt_param);
extern int mje_frame_pre(void *ent, void * param);
extern int mje_frame_post(void *ent, void * param);
extern void p200_h264e_set_intra(int dev,int intra);
extern void mpeg4_set_intra(int ch,int intra);
extern  void pcap_post_process(void* par1,void* par2);
static int bShowPerf = 0;
//#define ENABLE_CAP_POSTPROCESS
#include "videograph_api.h"
extern unsigned int video_gettime_max_ms(void);
typedef struct enc_perf{
    u32 cnt;
    u32 max_latance;
    u32 min_latance;
    u32 avg_latance;
    u32 time;
    int job_id;
    int repd_bs_num;  //0:main, 1:sub1, 2:sub2
}enc_perf_t;

static enc_perf_t gperf_info[DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM];

#define DV_RESET_TAG(ptag)               {   (ptag)->func=0; (ptag)->lv_ch=0; (ptag)->rec_ch=0; (ptag)->pb_ch=0; (ptag)->cas_ch=0; }
#define DV_SET_REC_CH(ptag, fn, ch_num)  {   (ptag)->func|=(fn); (ptag)->rec_ch|=(0x1<<ch_num); }

static int dvr_enc_fun_rec_table[]={  
                FN_RECORD,		/* main-bitstream */
			    FN_SUB1_RECORD,	/* sub1-bitstream */
                FN_SUB2_RECORD,	/* sub2-bitstream */
                FN_SUB3_RECORD,	/* sub3-bitstream */
                FN_SUB4_RECORD,	/* sub4-bitstream */
                FN_SUB5_RECORD,	/* sub5-bitstream */
                FN_SUB6_RECORD,	/* sub6-bitstream */
                FN_SUB7_RECORD,	/* sub7-bitstream */
                FN_SUB8_RECORD};/* sub8-bitstream */

static void reset_perf(enc_perf_t *pperf)
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

static void update_perf(enc_perf_t *pperf, int job_id, int repd_bs_num)
{
    if(!bShowPerf)
        return;

    pperf->cnt++;
    pperf->time = video_gettime_ms();
    pperf->job_id = job_id;
    pperf->repd_bs_num = repd_bs_num;
}

static void update_latance(enc_perf_t *pperf)
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
        printk("EncPerf(%d): stream.id, frm.count, max.latence, min.latence, avg.latence\n", diff);
        for(i=0;i<DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM;i++) {
            if(gperf_info[i].cnt)
                printk("\t([%d]%d,%d,%d,%d)\n", i, gperf_info[i].cnt, gperf_info[i].max_latance, gperf_info[i].min_latance, gperf_info[i].avg_latance);
            reset_perf(&gperf_info[i]);
        }
        StartTime = video_gettime_ms();
    }
}
#define DEBUG_ENC_MAIN      0

//#define PDEBUG(fmt, args...) printk("%s""(L:%d):" fmt, __FUNCTION__, __LINE__, ##args)
#define PDEBUG(fmt, args...)


#ifdef VDBGPRINT
  #undef VDBGPRINT
#endif
#define VDBGPRINT(mode, fmt, args...)  {if (mode & dvr_enc_dbgmode) printk(fmt, ## args);}

static unsigned int get_max_fps(dvr_enc_data *enc_data)
{
    dvr_enc_bt *ebt;	
    EncParam *enc;
    int sub_num, max_fps=0;

	for(sub_num=0; sub_num<DVR_ENC_REPD_BT_NUM; sub_num++) {
		ebt = &enc_data->ebt[sub_num];  
        enc = &enc_data->repd[sub_num].enc;    
			//	printk("@@frame_rate[%d]=%d-=%d\n",sub_num,enc->frame_rate,ebt->enabled);
        if(ebt->enabled==DVR_ENC_EBST_ENABLE){
            if(max_fps < enc->frame_rate)
                max_fps = enc->frame_rate;
        }
    }
    return max_fps;
}

static unsigned int get_cap_input_ch(int ch_num)
{
    int input_ch=0;

    if(vsrc > MAX_VIDEO_SOURCE_NUM || vsrc ==0) 
        panic("%s: Max video source number is %d, but vsrc=%d!\n", __FUNCTION__, MAX_VIDEO_SOURCE_NUM, vsrc);
        
    if(vsrc > 1) {
        /* ch_num=0/1 -> input_ch=0, ch_num=2,3->input_ch=1 */
        input_ch = ch_num/vsrc;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <vsrc=%d, ch_num=%d>\n",__FUNCTION__,__LINE__, vsrc, ch_num);
    return input_ch;
}

static unsigned int get_cap_src_max_fps(int ch_num, int input_system)
{
    unsigned int video_src_num, frdm;

    video_src_num = ch_num/MAX_VIDEO_SOURCE_NUM;
    
    if(video_src_num >= MAX_VIDEO_SOURCE_NUM) 
        panic("%s: Max video source number is %d, but video_src_num=%d!\n", __FUNCTION__, MAX_VIDEO_SOURCE_NUM, video_src_num);
    if(input_system == MCP_VIDEO_PAL)
        frdm = 25; 
    else 
        frdm = FrameRate_Denominator[video_src_num];
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <video_src_num=%d, ch_num=%d, maxfps=%d>\n",__FUNCTION__,__LINE__, video_src_num, ch_num, frdm);
    return frdm;
}

static int max_fps_read_mode(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    int i; 

    printk("\n");
    printk("vsrc_number\tmax_frame_rate\n");
    for(i=0; i< MAX_VIDEO_SOURCE_NUM; i++) {
        printk("%d\t\t%d\n", i, FrameRate_Denominator[i]);
    }
    *eof = 1;       //end of file
    return 0;
}

static int max_fps_write_mode(struct file *file, const char *buffer,unsigned long count, void *data)
{
    int len = count;
    unsigned char value[20];
    int vsrc_num, max_fps, i;
 
    if (copy_from_user(value, buffer, len))
        return 0;
    value[len] = '\0';
 
    sscanf(value, "%u %u\n", &vsrc_num, &max_fps);
    if (vsrc_num >= MAX_VIDEO_SOURCE_NUM) {
        printk("vsrc_num=%d error!", vsrc_num);
        goto exit1; 
    }
    if(vsrc == 1) {
        for(i=0; i< MAX_VIDEO_SOURCE_NUM; i++) {
            FrameRate_Denominator[i] = max_fps;
        }
    } else {
        FrameRate_Denominator[vsrc_num] = max_fps;
    }
    printk("\n");
    printk("vsrc_number=%d, max_frame_rate=%d\n", vsrc_num, FrameRate_Denominator[vsrc_num]);
exit1: 
    return count;
}

static int proc_read_mode(char *page, char **start, off_t off, int count,int *eof, void *data)
{
    printk("Current Debug Mode: 0x%x\n", dvr_enc_dbgmode);
    printk(" Values(combinative): 0x0:Off, 0x1:DTO Fnc, 0x2:DTO Job, 0x4:ENC Fnc, 0x8:ENC data flow\n");
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
    dvr_enc_dbgmode = tmp;
    printk("Debug Mode:%d\n", dvr_enc_dbgmode);
    return count;
}

static int proc_perf_show(struct seq_file *sfile, void *v)
{   
    seq_printf(sfile, "Show ENC performance:%s\n",bShowPerf?"Enable":"Disable");
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
    for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++)
        if(enc_private_data[i].ch_num == last_api_ch)
            break;

    if(i>=DVR_RECORD_CHANNEL_NUM)
    {
        printk("Invalid arguments. ch(%d)\n", i);
        return 0;
    }

    switch (last_api_num)
    {
      case 8:
        print_user_last_api_data("ENC:DVR_ENC_QUERY_OUTPUT_BUFFER_SIZE", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_buf_size, 0);
        break;
      case 13:
        print_user_last_api_data("ENC:DVR_ENC_QUERY_OUTPUT_BUFFER_SUB1_BS_OFFSET", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_sub1_buf_size, 0);
        break;
      case 14:
        print_user_last_api_data("ENC:DVR_ENC_QUERY_OUTPUT_BUFFER_SUB2_BS_OFFSET", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_sub2_buf_size, 0);
        break;
      case 10:
        print_user_last_api_data("ENC:DVR_ENC_QUERY_OUTPUT_BUFFER_SNAP_OFFSET", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_ss_buf_size, 0);
        break;
      case 3:
        print_user_last_api_data("ENC:DVR_ENC_GET_CHANNEL_PARAM", last_api_num, sizeof(dvr_enc_channel_param), (unsigned char*)&enc_private_data[i].la_ch_param_get, 0);
        break;
      case 2:
        print_user_last_api_data("ENC:DVR_ENC_SET_CHANNEL_PARAM", last_api_num, sizeof(dvr_enc_channel_param), (unsigned char*)&enc_private_data[i].la_ch_param_set, 1);
        break;
      case 16:
        print_user_last_api_data("ENC:DVR_ENC_SET_SUB_BS_PARAM", last_api_num, sizeof(ReproduceBitStream), (unsigned char*)&enc_private_data[i].la_sub_bs, 1);
        break;
#if LOG_API_QUEUE_FLOW
      case 5:
        print_user_last_api_data("ENC:DVR_ENC_QUEUE_GET", last_api_num, sizeof(dvr_enc_queue_get), (unsigned char*)&enc_private_data[i].la_que_get, 0);
        break;
      case 11:
        print_user_last_api_data("ENC:DVR_ENC_QUEUE_GET_SUB1_BS", last_api_num, sizeof(dvr_enc_queue_get), (unsigned char*)&enc_private_data[i].la_sub1_que_get, 0);
        break;
      case 12:
        print_user_last_api_data("ENC:DVR_ENC_QUEUE_GET_SUB2_BS", last_api_num, sizeof(dvr_enc_queue_get), (unsigned char*)&enc_private_data[i].la_sub2_que_get, 0);
        break;
      case 9:
        print_user_last_api_data("ENC:DVR_ENC_QUEUE_GET_SNAP", last_api_num, sizeof(dvr_enc_queue_get), (unsigned char*)&enc_private_data[i].la_ss_que_get, 0);
        break;
      case 6:
        print_user_last_api_data("ENC:DVR_ENC_QUEUE_PUT", last_api_num, sizeof(dvr_enc_queue_get), (unsigned char*)&enc_private_data[i].la_que_put, 1);
        break;
#endif /* LOG_API_QUEUE_FLOW */
      case 7:
        print_user_last_api_data("ENC:DVR_ENC_CONTROL", last_api_num, sizeof(dvr_enc_control), (unsigned char*)&enc_private_data[i].la_enc_ctrl, 1);
        break;
      case 15:
        print_user_last_api_data("ENC:DVR_ENC_RESET_INTRA", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_reset_intra, 1);
        break;
      case 18:
        print_user_last_api_data("ENC:DVR_ENC_SWAP_INTRA", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_swap_intra, 1);
        break;
      case 19:
        print_user_last_api_data("ENC:DVR_ENC_SUB_PATH_DENOISE_CTRL", last_api_num, sizeof(int), (unsigned char*)&enc_private_data[i].la_sub_dn_ctrl, 1);
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

static unsigned int enc_get_platf_ver(void)
{
    unsigned int version;
	
    version = ftpmu010_get_attr(ATTR_TYPE_PMUVER); 
    return(version & 0xffffff00);
}

static void DtoNotify(int ch_num, int status)
{
    dvr_info_buf_flow_info  buf_flow_info;
    int ch = ch_num%DVR_RECORD_CHANNEL_NUM;
    int stream = ch_num/DVR_RECORD_CHANNEL_NUM;

    if(status==DTO_JOB_AP_GET)
    {
        buf_flow_info.fnc = BFI_FNC_REC(ch, stream);
        buf_flow_info.in_out = 1;
        dvr_common_store_info(INFO_SET_BUF_FLOW_COUNT, &buf_flow_info);
    }
    else if(status==DTO_JOB_CALLBACK)
    {
        buf_flow_info.fnc = BFI_FNC_REC(ch, stream);
        buf_flow_info.in_out = -1;
        dvr_common_store_info(INFO_SET_BUF_FLOW_COUNT, &buf_flow_info);
    }
    else if(status==DTO_JOB_TIMEOUT)
    {
        dvr_common_store_info(INFO_SET_FLOW_TIMEOUT, (void*)BFI_FNC_REC(ch, stream));
    }
}

static int Check_SubCap(dvr_enc_data *enc_data, int *pmax_width, int *pmax_height)
{
    int ret = -1;
    int i;
    int max_width = 0;
    int max_height = 0;
    int max_rate = 0;
    dvr_enc_bt *ebt;	
	ReproduceBitStream *repd;
    int ch=1;

    for(i=1; i<DVR_ENC_REPD_BT_NUM; i++) {
        repd = &enc_data->repd[i];
        ebt = &enc_data->ebt[i];  
        if(ebt->enabled == DVR_ENC_EBST_ENABLE) {
            if(ebt->enc_type ==ENC_TYPE_MJPEG){
                ret = -1;
                break;
            }
            else
                ret = 0;
            if(max_width<repd->dim.width && max_height<repd->dim.height){
                max_width = repd->dim.width;
                max_height = repd->dim.height;
                ch = i;
            }
            if(max_rate<repd->enc.frame_rate)
                max_rate = repd->enc.frame_rate;
        }
    }

    if(!ret){
        enc_data->SubCapRate = max_rate;
        enc_data->sub_enc_in_q = &enc_private_data[enc_data->ch_num].ebt[ch].sub_enc_in_q; 
        enc_data->sub_enc_in_dup_q = &enc_private_data[enc_data->ch_num].ebt[ch].sub_enc_in_dup_q; 
        *pmax_width = max_width;
        *pmax_height = max_height;
        for(i=1; i<DVR_ENC_REPD_BT_NUM; i++) {
            repd = &enc_data->repd[i];
            ebt = &enc_data->ebt[i];  
            if(ebt->enabled == DVR_ENC_EBST_ENABLE) {
                if(ebt->enc_type ==ENC_TYPE_MJPEG) {
                    ebt->en_scl=1;  // we use scaler to transfor the video format for MJPEG.
                }else {
                    if((max_width!=repd->dim.width) ||(max_height!=repd->dim.height)){
                        ebt->en_scl=1;  // the resolution of repd is not equal sub cap dimention 
                    }
                    else
                        ebt->en_scl=0;
                }
            }
        }
    }

    return ret;
}

static int do_setup_encoder(dvr_enc_data *enc_data, int stream)
{
    int i, ret, ch_num;
    dvr_enc_bt		*ebt;	
	dvr_enc_src_param *src;
	ReproduceBitStream *repd;
    dvr_enc_scl_t *enc_scl;

	src = &enc_data->src;
    enc_data->ch_num = src->channel;
    enc_data->src_type = src->enc_src_type;

    /*
            DVR         DVS
    CH0 ==> Cap0rec     Cap0rec
    CH1 ==> Cap1rec     Cap1rec
    CH2 ==> Cap2rec     Cap2rec
    CH3 ==> Cap3rec     Cap3rec
    CH4 ==> Cap4rec     Cap0pre
    CH5 ==> Cap5rec     Cap1pre
    CH6 ==> Cap6rec     Cap2pre
    CH7 ==> Cap7rec     Cap3pre
     */
    if(enc_func==FUNC_4D1_DVS)
    {
        if(enc_data->ch_num>3)
            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(enc_data->ch_num-4, 1));
        else
            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(enc_data->ch_num, 0));
    }
	else if(enc_func==FUNC_1080P_IPC)
	{
		switch(enc_data->ch_num) {
			case 0:
	            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 0));
				break;
			case 1: 
	            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(0, 1));
				break;
			case 2: 
	            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 0));
				break;
			case 3: 
	            enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(1, 1));
				break;
			case 4:
				enc_data->isp_fd=ENTITY_FD(0x66, 0);
				break;
			default: 
				printk("%s:%d <error cap_fd ch_num=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num);								
				return -EFAULT;
		}
	}
    else
        enc_data->cap_fd[0] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(enc_data->ch_num, 1));

    enc_data->DI_fd[0] = ENTITY_FD(GM3DI_FD_MAJOR, GM3DI_FD_MINOR_RC(enc_data->ch_num));

	for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
		ebt = &enc_data->ebt[i];  
		repd = &enc_data->repd[i];  
		ch_num = enc_data->ch_num + DVR_RECORD_CHANNEL_NUM * i;
		ebt->enabled = repd->enabled;
		if(ebt->enabled!=DVR_ENC_EBST_ENABLE)
		    continue;

		ebt->enc_type = repd->enc_type;
		ebt->is_blocked = repd->is_blocked;
		if(i==0)
		    ebt->en_snapshot = repd->en_snapshot;
		else
		    ebt->en_snapshot = DVR_ENC_EBST_DISABLE;  // Sub-bistream donot enable snapshot.

		switch(ebt->enc_type) {
			case ENC_TYPE_H264:
				ebt->enc_fd = ENTITY_FD(ENC_H264_FD_MAJOR, ENC_H264_FD_MINOR(ch_num));
				break;
			case ENC_TYPE_MPEG:
				ebt->enc_fd = ENTITY_FD(ENC_MPEG4_FD_MAJOR, ENC_MPEG4_FD_MINOR(ch_num));
				break;
			case ENC_TYPE_MJPEG:
				ebt->enc_fd = ENTITY_FD(ENC_MJPEG_FD_MAJOR, ENC_MJPEG_FD_MINOR(ch_num));
				break;
			case ENC_TYPE_YUV422:
			    ebt->enc_fd = 0;
			    ebt->scl_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_RAW(enc_data->ch_num));
				break;
			default: 
                panic("%s: enc_type failed!\n", __FUNCTION__);
		}

		ebt->dataout_fd = ENTITY_FD(DATA_OUT_FD_MAJOR, DATA_OUT_FD_MINOR(ch_num));
	}
	for(i=0; i<DVR_ENC_SCL_NUM; i++) {
    	enc_scl = &enc_data->enc_scl[i];
		enc_scl->fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_REPD(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*i)));
	}

	/* check main-bitstream snapshot function */
	ebt = &enc_data->ebt[stream];  
	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
		ebt->ss_enc_fd = ENTITY_FD(ENC_MJPEG_FD_MAJOR, ENC_MJPEG_FD_MINOR_SS(enc_data->ch_num));
		ebt->scl_ss_fd = ENTITY_FD(SCALER_FD_MAJOR, SCALER_FD_MINOR_SS(enc_data->ch_num));				
    	ebt->ss_dataout_fd = ENTITY_FD(DATA_OUT_FD_MAJOR, DATA_OUT_FD_MINOR((enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM)));
	}

	if(!enc_data->graph) {
        ret = dvr_graph_create(&enc_data->graph, GFID_ENC(enc_data->ch_num));
        if(ret<0) return -EFAULT;
	}
	for(i=stream; i<(stream+1); i++) {
	    if(enc_data->ebt[i].enabled!=DVR_ENC_EBST_ENABLE)
	        continue;
		ch_num = enc_data->ch_num + DVR_RECORD_CHANNEL_NUM * i;
		ret = idataout_open(ch_num, &enc_data->ebt[i].wait_queue_stream);
		if(ret<0) return -EFAULT;
	}
	ebt = &enc_data->ebt[stream];  
	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
    	ret = idataout_open(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM, &ebt->wait_queue_snapshot);
	    if(ret<0) return -EFAULT;
	}

    dvr_common_store_info(INFO_RESET_BUF_FLOW_IN_OUT, (void*)BFI_FNC_REC(enc_data->ch_num, stream));
    return 0;
}

static void set_cap_path_mode_property(dvr_graph *g, FuncTag *tag, dvr_enc_data *enc_data, int CapPathMode)
{
	dvr_enc_src_param *src;
	
	src = &enc_data->src;
    if(CapPathMode){
        int max_width=0, max_height=0;
        if(!Check_SubCap(enc_data, &max_width, &max_height)){
            int SubCap_fd, SubDI_fd;

            SubCap_fd = enc_data->cap_fd[1] = enc_data->cap_fd[2] = ENTITY_FD(CAP_FD_MAJOR, CAP_FD_MINOR(enc_data->ch_num, 3));
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "bg_width"), max_width, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "bg_height"), max_height, 1);
            if(src->di_mode==0 || src->di_mode==1)
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "DI_mode"), src->di_mode, 1);
            else
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "DI_mode"), 1, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "mode"), 1, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "color"), src->color_mode, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "dma_order"), src->dma_order, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "scaler_indep"), 1, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "bg_x"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "bg_y"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_y0"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_u0"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_v0"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_y1"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_u1"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "buf_v1"), 0, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "width"), max_width, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "height"), max_height, 1);
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "FrameRate_Numerator"), enc_data->SubCapRate, 1);

            if(CapPathMode==2){
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "SharePath"), 1, 1);
            }
            else
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "SharePath"), 0, 1);

            if(src->input_system == MCP_VIDEO_NTSC) {
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "FrameRate_Denominator"), 30, 1);
            }
            else {
                dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "FrameRate_Denominator"), 25, 1);
            }
            dvr_graph_set_property(g, tag, SubCap_fd, PROPID(SubCap_fd, "updata"), 0, 1);   //apply it

            if(enc_data->is_sub_denoise){
                SubDI_fd = enc_data->DI_fd[1] = enc_data->DI_fd[2] = ENTITY_FD(GM3DI_FD_MAJOR, GM3DI_FD_MINOR_SUBRC(enc_data->ch_num));
                dvr_graph_set_property(g, tag, SubDI_fd, PROPID(SubDI_fd, "width"), max_width, 1);
                dvr_graph_set_property(g, tag, SubDI_fd, PROPID(SubDI_fd, "height"), max_height, 1);
                dvr_graph_set_property(g, tag, SubDI_fd, PROPID(SubDI_fd, "field_offset"), 0, 1);
                dvr_graph_set_property(g, tag, SubDI_fd, PROPID(SubDI_fd, "denoise_only"), 2, 1);
                dvr_graph_set_property(g, tag, SubDI_fd, PROPID(SubDI_fd, "denoise_field_frame"), 1, 1);
            }
            else
                enc_data->DI_fd[1] = enc_data->DI_fd[2] = -1;
        }
        else {
            enc_data->cap_fd[1] = -1;
            enc_data->DI_fd[1] = -1;
            enc_data->cap_fd[2] = -1;
            enc_data->DI_fd[2] = -1;
            CapPathMode = 0;
        }
    }
    else {
        enc_data->cap_fd[1] = -1;
        enc_data->DI_fd[1] = -1;
        enc_data->cap_fd[2] = -1;
        enc_data->DI_fd[2] = -1;
    }
    enc_data->CapPathMode = CapPathMode;
}

static int set_cap_property(dvr_graph *g, FuncTag *tag, int fd, dvr_enc_data *enc_data)
{
	dvr_enc_src_param *src;
    int align_height;
	int y0_offset=0,u0_offset=0, v0_offset=0;
	int y1_offset=0,u1_offset=0, v1_offset=0;

	src = &enc_data->src;
    if(src->vp_param.is_3DI)
    {
        if(src->di_mode==LVFRAME_GM3DI_FORMAT)
        {
    	    align_height = ((src->dim.height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
    		y0_offset = 0;
    		y1_offset = src->dim.width * align_height * 2;
        }
        else
        {
            printk("[dvr_enc:%d] Enable 3DI, but di_mode is not LVFRAME_GM3DI_FORMAT.\n", enc_data->ch_num);
            return -1;
        }
    }
    else
    {
        if(src->vp_param.is_denoise)
        {
    	    align_height = ((src->dim.height+15)/16)*16;  //for denoise, y-offset must be aligned with 16
        }

        if(src->di_mode == LVFRAME_WEAVED_TWO_FIELDS)
            align_height = 0;       // y0 and y1 are the same
        else
            align_height = src->dim.height;

		y0_offset = 0;
		u0_offset = src->dim.width * src->dim.height;
		v0_offset = src->dim.width * src->dim.height * 5/4;

        if(src->mode == LVFRAME_FRAME_MODE){
			if(src->dma_order == DMAORDER_PACKET)
				y1_offset = src->dim.width*align_height;
			else {
				y1_offset = src->dim.width * src->dim.height/2;
				if(src->dma_order == DMAORDER_3PLANAR) {
					u1_offset = u0_offset + src->dim.width * src->dim.height /8;
					v1_offset = v0_offset + src->dim.width * src->dim.height /8;
				}
				else {
					u1_offset = u0_offset + src->dim.width * src->dim.height /4;
				}
			}
		}
    }

	dvr_graph_set_property(g, tag, fd, PROPID(fd, "bg_width"), src->dim.width, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "bg_height"), src->dim.height, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "DI_mode"), src->di_mode, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "mode"), src->mode, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "color"), src->color_mode, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "dma_order"), src->dma_order, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "scaler_indep"), src->scale_indep, 1);
	if(src->input_system==MCP_VIDEO_PAL)
	    dvr_graph_set_property(g, tag, fd, PROPID(fd, "input_mode"), 0, 1);
	else
	    dvr_graph_set_property(g, tag, fd, PROPID(fd, "input_mode"), 1, 1);

    dvr_graph_set_property(g, tag, fd, PROPID(fd, "input_ch"), get_cap_input_ch(enc_data->ch_num), 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "bg_x"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "bg_y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_y0"), y0_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_u0"), u0_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_v0"), v0_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_y1"), y1_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_u1"), u1_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_v1"), v1_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "width"), src->dim.width, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "height"), src->dim.height, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "FrameRate_Numerator"), enc_data->fps, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "H_Flip"), 0, 1);  // for Mirror 
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "V_Flip"), 0, 1);  // for Flip
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "FrameRate_Denominator"), get_cap_src_max_fps(enc_data->ch_num, src->input_system), 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "SharePath"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "updata"), 0, 1);   //apply it
	return 0;
}

static int set_3di_property(dvr_graph *g, FuncTag *tag, int fd, dvr_enc_data *enc_data)
{
	dvr_enc_src_param *src;
	int val=1, align_height, height;
	int field_offset=0;


	src = &enc_data->src;
    //height = (src->dim.height > 1024) ? 1024 : src->dim.height;
    height = ((src->dim.height+31)/32)*32;
    if(src->vp_param.is_3DI) {
        if(src->di_mode==LVFRAME_GM3DI_FORMAT) {
    	    align_height = ((src->dim.height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
    		field_offset = src->dim.width * align_height * 2;
        } else {
            printk("[dvr_enc:%d] Enable 3DI, but di_mode is not LVFRAME_GM3DI_FORMAT.\n", enc_data->ch_num);
            return -1;
        }
    } else {
        if(src->vp_param.is_denoise) {
    	    //align_height = ((src->dim.height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
    	    align_height = ((height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
    	    field_offset = 0;
        }
        if(src->di_mode == LVFRAME_WEAVED_TWO_FIELDS)
            align_height = 0;       // y0 and y1 are the same
        else
    	    align_height = ((height+31)/32)*32;  //for 3DI, y-offset must be aligned with 32
#if 0
        if(src->mode == LVFRAME_FRAME_MODE) {
			if(src->dma_order == DMAORDER_PACKET)
				field_offset = src->dim.width*align_height;
			else {
				field_offset = src->dim.width * src->dim.height/2;
			}
		}
#endif
    }

    if(src->vp_param.is_3DI && src->vp_param.is_denoise)
        val = 1;
    else if(src->vp_param.is_denoise)
        val = 2;
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "width"), src->dim.width, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "height"), height, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "field_offset"), field_offset, 1);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <align_height=%d>\n",__FUNCTION__,__LINE__,align_height);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <field_frame=%d, field_offset=%d, width=%d, height=%d>\n",__FUNCTION__,__LINE__,src->vp_param.denoise_mode, field_offset, src->dim.width, height);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "denoise_only"), val, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "denoise_field_frame"), src->vp_param.denoise_mode, 1);
	return 0;
}

static void set_h264_property(dvr_graph *g, FuncTag *tag, int fd, dvr_enc_data *enc_data, int stream, int inp_fmr)
{
	int u, v, height16, height_diff=0;
	ReproduceBitStream *repd;
    EncParam *enc;
    ENC_EXT_STRUCT *enc_ext_parm;
    dvr_enc_bt *ebt;
    int roi_height16, roi_height_diff = 0;
    ebt = &enc_data->ebt[stream];
	repd = &enc_data->repd[stream];
	enc = &enc_data->repd[stream].enc;
    enc_ext_parm = &enc_data->enc_ext[stream];

    if((repd->dim.width > ebt->max_dim.width) || (repd->dim.height > ebt->max_dim.height)) {
        panic("%s: ch=%d, sub=%d, encoder buffer is not enough!(buf:%dx%d input_res:%dx%d)\n", __FUNCTION__, 
                            enc_data->ch_num,
                            stream,
                            ebt->max_dim.width,
                            ebt->max_dim.height,
                            repd->dim.width,
                            repd->dim.height);
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, width=%d, height=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream, repd->dim.width, repd->dim.height);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ROI, %d,%d,%d,%d,%d>\n",__FUNCTION__,__LINE__,enc->is_use_ROI, enc->ROI_win.x, enc->ROI_win.y, enc->ROI_win.width, enc->ROI_win.height);
	u = repd->dim.width * repd->dim.height;
	v = repd->dim.width * repd->dim.height * 5/4;
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_Start"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MaxWidth"),ebt->max_dim.width, 1);
	
    height16 = ((ebt->max_dim.height+15)/16) * 16;
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MaxHeight"),ebt->max_dim.height, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MaxHeight"),height16, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_Input_image_type"), inp_fmr, 1);
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_Input_image_type"), ENC_INPUT_H2642D, 1);
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_Input_image_type"), ENC_INPUT_1D422, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_FrameWidth"), repd->dim.width, 1);
    height16 = ((repd->dim.height+15)/16) * 16;
    height_diff = height16 - repd->dim.height;
    roi_height16 = ((enc->ROI_win.height + 15) / 16) * 16;
    roi_height_diff = roi_height16 - enc->ROI_win.height;
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_FrameHeight"), repd->dim.height, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_FrameHeight"), height16, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_UOffset"), u, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_VOffset"), v, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_IPInterval"), enc->ip_interval, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_Quant"), enc->init_quant, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ROIEnable"), enc->is_use_ROI, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ROIX"), enc->ROI_win.x, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ROIY"), enc->ROI_win.y, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ROIWidth"), enc->ROI_win.width, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ROIHeight"), roi_height16, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MaxSlice"), 4, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MbRowPerSlice"), 0, 1);     /* auto multi_slice */
	//dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_MbRowPerSlice"), -1, 1);  /* force 1_slice */
    if(enc_ext_parm->feature_enable&DVR_ENC_H264_WATERMARK){
		dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_watermark_enable"), enc_ext_parm->watermark_enable, 1);
		dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_watermark_interval"), enc_ext_parm->watermark_interval, 1);
		dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_watermark_init_pattern"), enc_ext_parm->watermark_init_pattern, 1);
		dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_watermark_init_interval"), enc_ext_parm->watermark_init_interval, 1);
	}
	if((height_diff != 0) && (enc->is_use_ROI == 0)) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, height_diff=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream,height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_crop_buttom_offset"), height_diff/2, 1);     
	}
    ///////////////////
    //add for roi support any size not just 16X
    if ((roi_height_diff != 0) && (enc->is_use_ROI != 0))
    {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d, height_diff=%d>\n", __FUNCTION__, __LINE__, enc->ROI_win.height, height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_crop_buttom_offset"), roi_height_diff / 2, 1);
    }

    //////////////
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VG_E_ID_End"), 1, 1);
}

static void set_mpeg4_property(dvr_graph *g, FuncTag *tag, int fd, dvr_enc_data *enc_data, int stream, int inp_fmr)
{
	int u, v, height16, height_diff=0;
	ReproduceBitStream *repd;
    EncParam *enc;
    ENC_EXT_STRUCT *enc_ext_parm;
    dvr_enc_bt *ebt;
    int roi_height16, roi_height_diff = 0;
    ebt = &enc_data->ebt[stream];
	repd = &enc_data->repd[stream];
	enc = &enc_data->repd[stream].enc;
    enc_ext_parm = &enc_data->enc_ext[stream];

    if((repd->dim.width > ebt->max_dim.width) || (repd->dim.height > ebt->max_dim.height))
        panic("%s: ch=%d, sub=%d, encoder buffer is not enough!(buf:%dx%d input_res:%dx%d)\n", __FUNCTION__, 
                            enc_data->ch_num,
                            stream,
                            ebt->max_dim.width,
                            ebt->max_dim.height,
                            repd->dim.width,
                            repd->dim.height);

	u = repd->dim.width * repd->dim.height;
	v = repd->dim.width * repd->dim.height * 5/4;

	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_Start"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32MaxWidth"), ebt->max_dim.width, 1);
    height16 = ((ebt->max_dim.height+15)/16) * 16;
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32MaxHeight"), height16, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32FrameWidth"), repd->dim.width, 1);
    height16 = ((repd->dim.height+15)/16)*16;
    height_diff = height16 - repd->dim.height;
    roi_height16 = ((enc->ROI_win.height + 15) / 16) * 16;
    roi_height_diff = roi_height16 - enc->ROI_win.height;
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32FrameHeight"), repd->dim.height, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32FrameHeight"), height16, 1);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, width=%d, height=%d>\n", __FUNCTION__, __LINE__, enc_data->ch_num, stream, repd->dim.width, height16);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ROI, %d,%d,%d,%d,%d>\n", __FUNCTION__, __LINE__, enc->is_use_ROI, enc->ROI_win.x, enc->ROI_win.y, enc->ROI_win.width, enc->ROI_win.height);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_bROIEnable"), enc->is_use_ROI, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_roi_x"), enc->ROI_win.x, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_roi_y"), enc->ROI_win.y, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_roi_w"), enc->ROI_win.width, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_roi_h"), roi_height16, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_fincr"), 1, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_fbase"), enc->frame_rate, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_u32IPInterval"), enc->ip_interval, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_bShortHeader"), 0, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_bEnable4MV"), 0, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_bH263Quant"), 1, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_bResyncMarker"), 0, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_ac_disable"), 0, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_U_offset"), u, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_V_offset"), v, 1);
    dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_img_fmt"), inp_fmr, 1);

    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_img_fmt"), ENC_INPUT_1D422, 1);
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_img_fmt"), ENC_INPUT_H2642D, 1);
	if((height_diff != 0) && (enc->is_use_ROI == 0)) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, height_diff=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream,height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_CropHeight"), height_diff, 1);     
	}
    ///////////////////
    //add for roi support any size not just 16X
    if ((roi_height_diff != 0) && (enc->is_use_ROI != 0))
    {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d, height_diff=%d>\n", __FUNCTION__, __LINE__, enc->ROI_win.height, roi_height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_CropHeight"), roi_height_diff , 1);
    }

    //////////////
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MP4E_End"), 1, 1);
}

static void set_mjpeg_property(dvr_graph *g, FuncTag *tag, int fd, int ss_count, dvr_enc_data *enc_data, int stream, int inp_fmr)
{
	int mj_w, mj_h, height16, height_diff=0;
	dvr_enc_src_param *src;
	ReproduceBitStream *repd;
    dvr_enc_bt *ebt;	
    EncParam *enc;
    int roi_height16, roi_height_diff = 0;
    src = &enc_data->src;
    repd = &enc_data->repd[stream];
    ebt = &enc_data->ebt[stream];
	enc = &enc_data->repd[stream].enc;
	mj_w = repd->dim.width;
	mj_h = repd->dim.height;
    if((mj_w > ebt->max_dim.width) || (mj_h > ebt->max_dim.height))
        panic("%s: ch=%d, sub=%d, encoder buffer is not enough!(buf:%dx%d input_res:%dx%d)\n", __FUNCTION__, 
                            enc_data->ch_num,
                            stream,
                            ebt->max_dim.width,
                            ebt->max_dim.height,
                            mj_w,
                            mj_h);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_Start"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_sample"), JCS_yuv420, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u32RestartInterval"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u32ImageWidth"), mj_w, 1);
    height16 = ((mj_h+15)/16) * 16;
    height_diff = height16 - mj_h;
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u32ImageHeight"), height16, 1);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, width=%d, height=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream, repd->dim.width, height16);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ROI, %d,%d,%d,%d,%d>\n",__FUNCTION__,__LINE__,enc->is_use_ROI, enc->ROI_win.x, enc->ROI_win.y, enc->ROI_win.width, enc->ROI_win.height);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u82D"), inp_fmr, 1);
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u82D"), JENC_INPUT_H2642D, 1);
    //dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_u82D"), JENC_INPUT_1D422, 1);
    if(enc->is_use_ROI == 1) {
        roi_height16 = ((enc->ROI_win.height + 15) / 16) * 16;
        roi_height_diff = roi_height16 - enc->ROI_win.height;
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_x"), enc->ROI_win.x, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_y"), enc->ROI_win.y, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_w"), enc->ROI_win.width, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_h"), roi_height16, 1);
    } else {
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_x"), 0, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_y"), 0, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_w"), 0, 1);
    	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_roi_h"), 0, 1);
    }
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_U_offset"), mj_w*mj_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_V_offset"), mj_w*mj_h*5/4, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_SS_count"), ss_count, 1);
	if((height_diff != 0) && (enc->is_use_ROI == 0)) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, height_diff=%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream,height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_CropHeight"), height_diff, 1);     
	}
    ///////////////////
    //add for roi support any size not just 16X
    if ((roi_height_diff != 0) && (enc->is_use_ROI == 1))
    {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d, height_diff=%d>\n", __FUNCTION__, __LINE__, enc->ROI_win.height, roi_height_diff);
        dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_CropHeight"), roi_height_diff, 1);
    }

    //////////////
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "VGID_MJE_End"), 1, 1);
	video_set_disable_mode(fd, DISABLE_WITH_PASS_ONLY);	// do not pass to next entity
}

static void set_scalar_property(dvr_graph *g, FuncTag *tag, int fd, int des_fmt, dvr_enc_data *enc_data, int stream, int snapshot)
{
	int s_w, s_h, tg_w, tg_h, buf_offset;
	dvr_enc_src_param *src;
	ReproduceBitStream *repd;
	
	src = &enc_data->src;
	repd = &enc_data->repd[stream];
	s_w = src->dim.width;
	s_h = src->dim.height;
    buf_offset = 0;
    if(src->di_mode==LVFRAME_GM3DI_FORMAT && repd->dim.width<=s_w && repd->dim.height<=(s_h/2) )
    {
        buf_offset = s_w*s_h*2;
        s_h = src->dim.height/2;
    }
	tg_w = repd->dim.width;
	tg_h = repd->dim.height;
	/* scaler for MJPEG format  */ 
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "buf_offset"), buf_offset, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "vp_width"), s_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "vp_height"), s_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_width"), s_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_height"), s_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_fmt"), SCALE_YUV422, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_swap_Y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_swap_CbCr"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "src_interleaved"), 0, 1);				
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_fmt"), des_fmt, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_swap_Y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_swap_CbCr"), 1, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_level"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "album"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "non_linear"), SCALE_LINEAR, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "dither"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "correction"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "mem_src_pit"), s_w/2, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "swc_axis_x"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "swc_axis_y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "swc_width"), s_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "swc_height"), s_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_width"), tg_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_height"), tg_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_axis_x"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "des_axis_y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "target_width"), tg_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "target_height"), tg_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "crop_axis_x"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "crop_axis_y"), 0, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "crop_width"), tg_w, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "crop_height"), tg_h, 1);
	dvr_graph_set_property(g, tag, fd, PROPID(fd, "enable_scale"), 0, 1);
	if(snapshot == 1)
		dvr_graph_set_property(g, tag, fd, PROPID(fd, "snapshot"), 0, 1);
	video_set_disable_mode(fd, DISABLE_WITH_PASS_ONLY); // do not pass to next entity
}

static int chk_graph_link(dvr_enc_data *enc_data, int fd, int stream)
{
    int i;
    dvr_enc_graph_link_t *l;

    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        l=&enc_data->graph_link_info[stream][i];
        if(l->fd==0) break;
        if(fd == l->fd) return 1;
    }
    return 0;
} 

static void set_enc_graph(dvr_enc_data *enc_data, int stream)
{
    int i, j, fd_fit;
    int enc_fd, dataout_fd, ss_enc_fd;
    dvr_enc_graph_link_t *l, *l_next;
    dvr_enc_bt	*ebt;	
    FuncTag tag, *ptag;
    dvr_graph  	*graph;

    graph=enc_data->graph;
    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	idataout_setactive(enc_data->ch_num+ DVR_RECORD_CHANNEL_NUM * stream, TRUE);
	ebt = &enc_data->ebt[stream];  
	enc_fd = ebt->enc_fd;
	dataout_fd = ebt->dataout_fd;
	ss_enc_fd = ebt->ss_enc_fd;
		if(enc_data->ch_num!=ISP_CHANNEL_NU)	{
	#ifdef ENABLE_CAP_POSTPROCESS
    dvr_register_postprocess(enc_data->cap_fd[0] ,pcap_post_process);
	#else
	dvr_register_postprocess(enc_data->cap_fd[0] ,NULL);
	#endif
			}
	switch(enc_data->repd[stream].enc_type) {
		case ENC_TYPE_MJPEG:
        	dvr_register_preprocess(enc_fd ,mje_frame_pre);
        	dvr_register_postprocess(enc_fd ,mje_frame_post);
            break;
		case ENC_TYPE_H264:
            dvr_register_preprocess(enc_fd ,p200_h264e_frame_pre);
            dvr_register_postprocess(enc_fd ,p200_h264e_frame_post);
			
            break;
        case ENC_TYPE_MPEG:
        	dvr_register_preprocess(enc_fd ,mpeg4_frame_pre);
        	dvr_register_postprocess(enc_fd ,mpeg4_frame_post);
            break;
        case ENC_TYPE_YUV422:
            break;
		default: 
			panic("%s: enc_type failed! (%d)\n",__FUNCTION__ ,enc_data->repd[stream].enc_type);
	}
    /* for test chip scalar */
    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        l=&enc_data->graph_link_info[stream][i];
        l_next=&enc_data->graph_link_info[stream][i+1];
        if((l->fd & 0x00ffff00) == (l_next->fd & 0x00ffff00)) {
            for(j=i; j<DVR_ENC_MAX_GRAPH_LINK_NUM; j++) {
                l=&enc_data->graph_link_info[stream][j];
                l_next=&enc_data->graph_link_info[stream][j+1];
                if(l->fd == 0)
                    memset(l,0,sizeof(dvr_enc_graph_link_t));
                else 
                    memcpy(l, l_next, sizeof(dvr_enc_graph_link_t));
            }
        }
    }
//		printk("@@max_fp=%d-enc_data->fps=%d\n",get_max_fps(enc_data),enc_data->fps);
    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        l=&enc_data->graph_link_info[stream][i];
        l_next=&enc_data->graph_link_info[stream][i+1];
        if(l_next->fd == 0) break; // finish link 
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch_num=(%d:%d), i=%d, (%s,%s), (%08x,%08x), (%d,%d), (boundary %d,%d), (%p,%p)>\n",__FUNCTION__,__LINE__,
                    enc_data->ch_num, stream,
                    i, 
                    l->name, l_next->name, 
                    (unsigned int) l->fd, (unsigned int) l_next->fd,
                    l->fps, l_next->fps,
                    l->fps_boundary, l_next->fps_boundary,
                    l->que, l_next->que);
	    dvr_graph_add_link(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps, l->que);
    }
	idataout_set_flow_rate(dataout_fd, ebt->enc_fps);

	/* for snapshot */
	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
		dvr_register_preprocess(ss_enc_fd ,mje_frame_pre);
		dvr_register_postprocess(ss_enc_fd ,mje_frame_post);
        for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
            l=&enc_data->graph_ss_link_info[i];
            l_next=&enc_data->graph_ss_link_info[i+1];
            if(l_next->fd == 0) break; // finish link 
            fd_fit = chk_graph_link(enc_data, l_next->fd, stream);
            if (fd_fit) continue;
            VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch_num=(%d:%d), i=%d, (%s,%s), (%08x,%08x), (%d,%d), (%p,%p)>\n",__FUNCTION__,__LINE__,
                    enc_data->ch_num, stream,
                    i, 
                    l->name, l_next->name, 
                    (unsigned int) l->fd, (unsigned int) l_next->fd,
                    l->fps, l_next->fps,
                    l->que, l_next->que);
    	    dvr_graph_add_link(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps, l->que);
        }
    	idataout_set_flow_rate(ebt->ss_dataout_fd, ebt->enc_fps);
	}
}

static int do_get_channel_param(dvr_enc_data *enc_data, dvr_enc_channel_param *ch_param)
{
    int val1;
    int cap_fd = enc_data->cap_fd[0];
    int di_fd=enc_data->DI_fd[0];

    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- IN\n", enc_data->ch_num, __FUNCTION__);

    // get parameters from capture
    ch_param->src.dim.width = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "width"), 1);
    ch_param->src.dim.height = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "height"), 1);
    ch_param->src.di_mode = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "DI_mode"), 1);
    ch_param->src.mode = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "mode"), 1);
    ch_param->src.dma_order = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "dma_order"), 1);
    ch_param->src.scale_indep = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "scaler_indep"), 1);
    ch_param->src.input_system = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "input_mode"), 1);
    ch_param->src.color_mode = dvr_graph_get_property(enc_data->graph, cap_fd, PROPID(cap_fd, "color"), 1);

    // get parameters from 3DI
    val1 = dvr_graph_get_property(enc_data->graph, di_fd, PROPID(di_fd, "denoise_only"), 1);
    ch_param->src.vp_param.is_3DI = ch_param->src.vp_param.is_denoise = FALSE;
    if(val1 == 1)      ch_param->src.vp_param.is_3DI = ch_param->src.vp_param.is_denoise = TRUE;
    else if(val1 == 2) ch_param->src.vp_param.is_denoise = TRUE;

    ch_param->src.vp_param.denoise_mode = dvr_graph_get_property(enc_data->graph, di_fd, PROPID(di_fd, "denoise_field_frame"), 1);

    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- OUT\n", enc_data->ch_num, __FUNCTION__);
    return 0;
}

void Free_RC(int);
void Init_RC(rc_data *param,int ch);
void Update_RC(rc_data *param,int ch);
void Update_RC_Convergence(rc_data *param,int ch);
void init_mpeg4_rc(rc_data *param,int ch);
void update_mpeg4_rc(rc_data *param,int ch);
void update_mpeg4_rc_convergence(rc_data *param,int ch);
void free_mpeg4_rc(int ch);
void init_mje_rc(mje_process *param, int ch);
void p200_h264e_set_roi_coordinate(int ch, int x, int y);
void mpeg4_set_roi_coordinate(int ch, int x, int y);
void mje_set_roi_coordinate(int ch, int x, int y);

static void update_cap_fps_property(dvr_enc_data *enc_data, int fps)
{
    
    int fps_numerator, cap_fd;

    cap_fd=enc_data->cap_fd[0];
    fps_numerator = get_cap_src_max_fps(enc_data->ch_num, enc_data->src.input_system);



	dvr_graph_set_property_directly(cap_fd, PROPID(cap_fd, "FrameRate_Numerator"), fps, 1);
    dvr_graph_set_property_directly(cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), fps_numerator, 1);
    dvr_graph_set_property_directly(cap_fd, PROPID(cap_fd, "updata"), 0, 1);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch%d, update cap fps=%d>\n",__FUNCTION__,__LINE__, enc_data->ch_num, fps);
} 

static void update_h264_gop_property(int h264_fd, int gop)
{

	dvr_graph_set_property_directly(h264_fd, PROPID(h264_fd, "VG_E_IPInterval"), gop, 1);
	dvr_graph_set_property_directly(h264_fd, PROPID(h264_fd, "VG_E_ID_End"), 1027, 1);
} 

static void update_mpeg4_gop_property(int mpeg4_fd, int gop)
{

	dvr_graph_set_property_directly(mpeg4_fd, PROPID(mpeg4_fd, "VGID_MP4E_u32IPInterval"), gop, 1);
	dvr_graph_set_property_directly(mpeg4_fd, PROPID(mpeg4_fd, "VGID_MP4E_End"), 1027, 1);
} 

static void enc_update_write_cap_linkflow(dvr_enc_data *enc_data, int stream, int max_fps)
{
    int i;
    dvr_enc_graph_link_t *g_link;

    g_link = &enc_data->graph_link_info[stream][0];
    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        g_link = &enc_data->graph_link_info[stream][i];
        if(g_link->fd == 0) break;
        if(g_link->fps_boundary == 0) g_link->fps=max_fps;
    }
}

static void enc_update_write_encoder_linkflow(dvr_enc_data *enc_data, int stream, int fps)
{
    int i;
    dvr_enc_graph_link_t *g_link;
    g_link = &enc_data->graph_link_info[stream][0];
    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        g_link = &enc_data->graph_link_info[stream][i];
        if(g_link->fd == 0) break;
        if(g_link->fps_boundary == 1) g_link->fps=fps;
    }
}

static void enc_update_write_ss_linkflow(dvr_enc_data *enc_data)
{
    int i, enc_fps, max_fps;
    dvr_enc_graph_link_t *g_ss_link_info;

    enc_fps = enc_data->ebt[0].enc_fps;
    max_fps = enc_data->fps;
    
    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        g_ss_link_info = &enc_data->graph_ss_link_info[i];
        if(g_ss_link_info->fd == 0) break;					
        if(g_ss_link_info->fps_boundary == 1) 
            g_ss_link_info->fps=enc_fps;
        else 
           g_ss_link_info->fps=max_fps;
			
    }
}

static void enc_update_write_flow(dvr_enc_data *enc_data, int stream, int direct)
{
    int i;
    dvr_enc_graph_link_t *l, *l_next;
    dvr_graph *graph=enc_data->graph;
    FuncTag tag, *ptag;

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);

    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        l=&enc_data->graph_link_info[stream][i];
        l_next=&enc_data->graph_link_info[stream][i+1];

        if(l_next->fd == 0) break; // finish link 
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch_num=(%d:%d) direct=%d, i=%d, (%s,%s), (%08x,%08x), (%d,%d), (boundary %d,%d), (%p,%p)>\n",__FUNCTION__,__LINE__,
                    enc_data->ch_num, stream, direct,
                    i, 
                    l->name, l_next->name, 
                    (unsigned int) l->fd, (unsigned int) l_next->fd,
                    l->fps, l_next->fps,
                    l->fps_boundary, l_next->fps_boundary,
                    l->que, l_next->que);

        if(direct)
            dvr_graph_update_flow(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps);
        else 
            dvr_graph_update_flow_list(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps);
    }
}

static void enc_update_write_ss_flow(dvr_enc_data *enc_data, int stream, int direct)
{
    int i;
    dvr_enc_graph_link_t *l, *l_next;
    dvr_graph *graph=enc_data->graph;
    FuncTag tag, *ptag;

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);

    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        l=&enc_data->graph_ss_link_info[i];
        l_next=&enc_data->graph_ss_link_info[i+1];

        if(l_next->fd == 0) break; // finish link 
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch_num=(%d:%d) direct=%d, i=%d, (%s,%s), (%08x,%08x), (%d,%d), (boundary %d,%d), (%p,%p)>\n",__FUNCTION__,__LINE__,
                    enc_data->ch_num, stream, direct,
                    i, 
                    l->name, l_next->name, 
                    (unsigned int) l->fd, (unsigned int) l_next->fd,
                    l->fps, l_next->fps,
                    l->fps_boundary, l_next->fps_boundary,
                    l->que, l_next->que);

        if(direct)
            dvr_graph_update_flow(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps);
        else 
            dvr_graph_update_flow_list(graph, ptag, l->fd, l->fps, l_next->fd, l_next->fps);
    }
}

static void do_set_graph_link_info(dvr_enc_graph_link_t *g_link, char *name, int fd, int fps, int fps_boundary, struct v_queue_t *que)
{
    int i;
    dvr_enc_graph_link_t *e;

    for(i=0; i<DVR_ENC_MAX_GRAPH_LINK_NUM; i++) {
        e=&g_link[i];
        if(e->fd) continue;
        strcpy(e->name, name);
        e->fd = fd;
        e->fps = fps;
        e->fps_boundary = fps_boundary;
        e->que = que;
        break;
    }
    if(i>=DVR_ENC_MAX_GRAPH_LINK_NUM) 
    	panic("%s: DVR_ENC_MAX_GRAPH_LINK_NUM is not enough!\n", __FUNCTION__);
    
}

static void enc_update_framerate(dvr_enc_data *enc_data, int stream)
{
    unsigned int cap_fps, max_fps, cap_fd, i, enc_fps;
    dvr_enc_bt		*ebt;	
    
    cap_fd=enc_data->cap_fd[0];
    enc_fps = enc_data->ebt[stream].enc_fps;
    max_fps = get_max_fps(enc_data);
    cap_fps = enc_data->fps;
    
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d, max_fps=%d, enc_fps=%d>\n",__FUNCTION__,__LINE__, 
                        enc_data->ch_num,
                        stream,
                        max_fps,
                        enc_fps);
    if(cap_fps != max_fps) {  /* max_fps change */
        enc_data->fps = max_fps;  /* update capture frame rate information */
        enc_update_write_encoder_linkflow(enc_data, stream, enc_fps);
        for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
            ebt = &enc_data->ebt[i];  
    	    if(ebt->enabled!=DVR_ENC_EBST_ENABLE)
    	        continue;
            enc_update_write_cap_linkflow(enc_data, i, max_fps);
            enc_update_write_flow(enc_data, i, 1);
        	if ((i==0) && (ebt->en_snapshot==DVR_ENC_EBST_ENABLE)) {
        	    enc_update_write_ss_linkflow(enc_data);
        	    enc_update_write_ss_flow(enc_data, i, 1);
        	}
        }
				if(enc_data->ch_num!=ISP_CHANNEL_NU){
			        update_cap_fps_property(enc_data, max_fps);
					}
			else{
						dvr_graph_set_property_directly(enc_data->isp_fd, PROPID(enc_data->isp_fd, "out_fps"), max_fps, 1);//for isp
				}
    } else {
        /* update this stream frame rate */
        enc_update_write_encoder_linkflow(enc_data, stream, enc_fps);
        enc_update_write_flow(enc_data, stream, 1);
        
        ebt = &enc_data->ebt[stream];  
    	if ((stream==0) && (ebt->en_snapshot==DVR_ENC_EBST_ENABLE)) {
    	    enc_update_write_ss_linkflow(enc_data);
    	    enc_update_write_ss_flow(enc_data, i, 1);
    	}
    }
    dvr_graph_apply_flow();
}

static void do_set_MJ_RC(dvr_enc_data *enc_data, int stream)
{
	mje_process param;
    EncParam *enc;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;
	
	enc = &enc_data->repd[stream].enc;
	enc_fd = enc_data->ebt[stream].enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];
    param.roi_x = param.roi_y = -1;

    if(enc_ext_parm->feature_enable&DVR_ENC_MJPEG_FUNCTION) {
		if(enc_ext_parm->MJ_quality < 1 || enc_ext_parm->MJ_quality > 100) 
			panic("%s: sub_nu %d, MJ_quality %d failed!\n",__FUNCTION__, stream, enc_ext_parm->MJ_quality);
        param.u32ImageQuality = enc_ext_parm->MJ_quality;
	} else {
        param.u32ImageQuality = 50;
		printk("%s:%d <ImageQuality not init, use default value: u32ImageQuality=%d>\n",__FUNCTION__,__LINE__, param.u32ImageQuality);
	}
	param.u8JPGPIC = 1;
	init_mje_rc(&param, enc_fd&0xff);
}

static void do_set_H264_RC(dvr_enc_data *enc_data, int stream, int init)
{
    rc_data prc_data;
    EncParam *enc;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;
	
	enc = &enc_data->repd[stream].enc;
	enc_fd = enc_data->ebt[stream].enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];

	memset(&prc_data,0,sizeof(rc_data));
	prc_data.u32BitRate=enc->bit_rate;
    prc_data.num_units_in_tick=1;
    prc_data.time_scale=enc->frame_rate;
	prc_data.u32MaxQuant=enc->max_quant;
	prc_data.u32MinQuant=enc->min_quant;
	prc_data.u32Quant=enc->init_quant;
	if ((enc->max_quant > 51) || (enc->min_quant <= 0)) {
	    panic("%s: ch=%d sub=%d, Input quant is illegal(max=%d, min=%d). (h264 max:51,min:1)\n", __FUNCTION__, 
	                        enc_data->ch_num, 
	                        stream,
	                        enc->max_quant,
	                        enc->min_quant);
    }

    if(enc_ext_parm->feature_enable&DVR_ENC_ENHANCE_H264_RATECONTROL){
        prc_data.u32TargetRateMax = enc_ext_parm->target_rate_max;
        /**
         * The unit of enc_ext_parm->reaction_delay_max is ms.
         * The unit of prc_data.u32ReactionDelayMax is frame count.
         **/
        prc_data.u32ReactionDelayMax = enc_ext_parm->reaction_delay_max*enc->frame_rate/1000;
    }
    else{
        prc_data.u32TargetRateMax = 0;
        prc_data.u32ReactionDelayMax = 0;
        /**
         * Ratecontrol can not accept bitrate=0, target_rate_max=0, and reaction_delay_max=0.
         * If we want to use VBR, we must give bitrate a non zero value.
         **/
        if(!prc_data.u32BitRate)
            prc_data.u32BitRate = 1;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, init=%d tgRt=%d, tgRtMax=%d, reaDyMax=%d, maxQ=%d, minQ=%d, iniQ=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							init, 
							prc_data.u32BitRate,
							prc_data.u32TargetRateMax,
							prc_data.u32ReactionDelayMax,
							prc_data.u32MaxQuant,
							prc_data.u32MinQuant,
							prc_data.u32Quant);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, num_units_in_tick=%d, time_scale=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.num_units_in_tick, 
							prc_data.time_scale);
	if (init)
		Init_RC(&prc_data,enc_fd&0xff);
	else
		Update_RC(&prc_data,enc_fd&0xff);
}

static void do_set_H264_RC_convergence(dvr_enc_data *enc_data, int stream)
{
    rc_data prc_data;
    EncParam *enc;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;
	
	enc = &enc_data->repd[stream].enc;
	enc_fd = enc_data->ebt[stream].enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];

	memset(&prc_data,0,sizeof(rc_data));
	prc_data.u32BitRate=enc->bit_rate;
    prc_data.num_units_in_tick=1;
    prc_data.time_scale=enc->frame_rate;
    if(enc_ext_parm->feature_enable & DVR_ENC_BASE_FRAME_RATE_CONTROL){
        if(enc_ext_parm->base_frame_rate_control.frame_rate != GMVAL_DO_NOT_CARE) {
    	    prc_data.time_scale=enc_ext_parm->base_frame_rate_control.frame_rate;
    	}
    }

	prc_data.u32MaxQuant=enc->max_quant;
	prc_data.u32MinQuant=enc->min_quant;
	//prc_data.u32Quant=enc->init_quant;
	if ((enc->max_quant > 51) || (enc->min_quant <= 0)) {
	    panic("%s: ch=%d sub=%d, Input quant is illegal(max=%d, min=%d). (h264 max:51,min:1)\n", __FUNCTION__, 
	                        enc_data->ch_num, 
	                        stream,
	                        enc->max_quant,
	                        enc->min_quant);
    }

    if(enc_ext_parm->feature_enable&DVR_ENC_ENHANCE_H264_RATECONTROL){
        prc_data.u32TargetRateMax = enc_ext_parm->target_rate_max;
        /**
         * The unit of enc_ext_parm->reaction_delay_max is ms.
         * The unit of prc_data.u32ReactionDelayMax is frame count.
         **/
        prc_data.u32ReactionDelayMax = enc_ext_parm->reaction_delay_max*enc->frame_rate/1000;
    }
    else{
        prc_data.u32TargetRateMax = 0;
        prc_data.u32ReactionDelayMax = 0;
        /**
         * Ratecontrol can not accept bitrate=0, target_rate_max=0, and reaction_delay_max=0.
         * If we want to use VBR, we must give bitrate a non zero value.
         **/
        if(!prc_data.u32BitRate)
            prc_data.u32BitRate = 1;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, tgRt=%d, tgRtMax=%d, reaDyMax=%d, maxQ=%d, minQ=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.u32BitRate,
							prc_data.u32TargetRateMax,
							prc_data.u32ReactionDelayMax,
							prc_data.u32MaxQuant,
							prc_data.u32MinQuant);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, num_units_in_tick=%d, time_scale=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.num_units_in_tick, 
							prc_data.time_scale);
	Update_RC_Convergence(&prc_data,enc_fd&0xff);
}

static void do_set_MPEG4_RC(dvr_enc_data *enc_data, int stream, int init)
{
    rc_data prc_data;
    EncParam *enc;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;
	
	enc = &enc_data->repd[stream].enc;
	enc_fd = enc_data->ebt[stream].enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];
	memset(&prc_data,0,sizeof(rc_data));
	prc_data.u32BitRate=enc->bit_rate;
	prc_data.num_units_in_tick=1;
	prc_data.time_scale=enc->frame_rate;
	prc_data.u32MaxQuant=enc->max_quant;
	prc_data.u32MinQuant=enc->min_quant;
	prc_data.u32Quant=enc->init_quant;
	if ((enc->max_quant > 31) || (enc->min_quant <= 0)) {
	    panic("%s: ch=%d sub=%d, Input quant is illegal(max=%d, min=%d). (mpeg4 max:31,min:1)\n", __FUNCTION__, 
	                        enc_data->ch_num, 
	                        stream,
	                        enc->max_quant,
	                        enc->min_quant);
    }
    if(enc_ext_parm->feature_enable&DVR_ENC_ENHANCE_H264_RATECONTROL){
        prc_data.u32TargetRateMax = enc_ext_parm->target_rate_max;
        /**
         * The unit of enc_ext_parm->reaction_delay_max is ms.
         * The unit of prc_data.u32ReactionDelayMax is frame count.
         **/
        prc_data.u32ReactionDelayMax = enc_ext_parm->reaction_delay_max*enc->frame_rate/1000;
    }
    else{
        prc_data.u32TargetRateMax = 0;
        prc_data.u32ReactionDelayMax = 0;
        /**
         * Ratecontrol can not accept bitrate=0, target_rate_max=0, and reaction_delay_max=0.
         * If we want to use VBR, we must give bitrate a non zero value.
         **/
        if(!prc_data.u32BitRate)
            prc_data.u32BitRate = 1;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, init=%d, tgRt=%d, tgRtMax=%d, reaDyMax=%d, maxQ=%d, minQ=%d, iniQ=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							init,
							prc_data.u32BitRate,
							prc_data.u32TargetRateMax,
							prc_data.u32ReactionDelayMax,
							prc_data.u32MaxQuant,
							prc_data.u32MinQuant,
							prc_data.u32Quant);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, num_units_in_tick=%d, time_scale=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.num_units_in_tick, 
							prc_data.time_scale);
	if (init)
		init_mpeg4_rc(&prc_data,enc_fd&0xff);
	else
	    update_mpeg4_rc(&prc_data,enc_fd&0xff);
}

static void do_set_MPEG4_RC_convergence(dvr_enc_data *enc_data, int stream)
{
    rc_data prc_data;
    EncParam *enc;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;
	
	enc = &enc_data->repd[stream].enc;
	enc_fd = enc_data->ebt[stream].enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];
	memset(&prc_data,0,sizeof(rc_data));
	prc_data.u32BitRate=enc->bit_rate;
	prc_data.num_units_in_tick=1;
	prc_data.time_scale=enc->frame_rate;
    if(enc_ext_parm->feature_enable & DVR_ENC_BASE_FRAME_RATE_CONTROL){
        if(enc_ext_parm->base_frame_rate_control.frame_rate != GMVAL_DO_NOT_CARE) {
    	    prc_data.time_scale=enc_ext_parm->base_frame_rate_control.frame_rate;
    	}
    }
	prc_data.u32MaxQuant=enc->max_quant;
	prc_data.u32MinQuant=enc->min_quant;
//	prc_data.u32Quant=enc->init_quant;
	if ((enc->max_quant > 31) || (enc->min_quant <= 0)) {
	    panic("%s: ch=%d sub=%d, Input quant is illegal(max=%d, min=%d). (mpeg4 max:31,min:1)\n", __FUNCTION__, 
	                        enc_data->ch_num, 
	                        stream,
	                        enc->max_quant,
	                        enc->min_quant);
    }
    if(enc_ext_parm->feature_enable&DVR_ENC_ENHANCE_H264_RATECONTROL){
        prc_data.u32TargetRateMax = enc_ext_parm->target_rate_max;
        /**
         * The unit of enc_ext_parm->reaction_delay_max is ms.
         * The unit of prc_data.u32ReactionDelayMax is frame count.
         **/
        prc_data.u32ReactionDelayMax = enc_ext_parm->reaction_delay_max*enc->frame_rate/1000;
    }
    else{
        prc_data.u32TargetRateMax = 0;
        prc_data.u32ReactionDelayMax = 0;
        /**
         * Ratecontrol can not accept bitrate=0, target_rate_max=0, and reaction_delay_max=0.
         * If we want to use VBR, we must give bitrate a non zero value.
         **/
        if(!prc_data.u32BitRate)
            prc_data.u32BitRate = 1;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, tgRt=%d, tgRtMax=%d, reaDyMax=%d, maxQ=%d, minQ=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.u32BitRate,
							prc_data.u32TargetRateMax,
							prc_data.u32ReactionDelayMax,
							prc_data.u32MaxQuant,
							prc_data.u32MinQuant);
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, num_units_in_tick=%d, time_scale=%d>\n",__FUNCTION__,__LINE__,
							enc_data->ch_num,
							stream,
							prc_data.num_units_in_tick, 
							prc_data.time_scale);
    update_mpeg4_rc_convergence(&prc_data,enc_fd&0xff);
}


static void enc_reset_roi_pos(dvr_enc_data *enc_data, int stream)
{
    EncParam *enc;
    ReproduceBitStream *repd;
    ENC_EXT_STRUCT *enc_ext_parm;
    int x=-1, y=-1;
	int enc_fd;

    repd = &enc_data->repd[stream];
	enc = &repd->enc;
    enc_ext_parm = &enc_data->enc_ext[stream];
	enc_fd = enc_data->ebt[stream].enc_fd;

    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, enc_fd=0x%x, roi=%d, x=%d, y=%d, width=%d, height=%d>\n",__FUNCTION__,__LINE__,
                            enc_data->ch_num, 
                            stream,
                            enc_fd,
                            enc->is_use_ROI, 
                            x,
                            y,
                            enc->ROI_win.width,
                            enc->ROI_win.height);
	if(enc->is_use_ROI) {
	    return;
	} 

	switch(repd->enc_type) {
		case ENC_TYPE_MJPEG:
			down(&dvr_enc_snapshot_sem);
			mje_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_snapshot_sem);
			break;
		case ENC_TYPE_H264:
			down(&dvr_enc_cfg_encoder_sem);
			p200_h264e_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_cfg_encoder_sem);
			break;
        case ENC_TYPE_MPEG:
			down(&dvr_enc_cfg_encoder_sem);
			mpeg4_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_cfg_encoder_sem);
            break;
		default: 
			panic("%s: enc_type failed! (%d)\n", __FUNCTION__, repd->enc_type);
	}
}

static void enc_update_roi_pos(dvr_enc_data *enc_data, int stream)
{
    EncParam *enc;
    ReproduceBitStream *repd;
    ENC_EXT_STRUCT *enc_ext_parm;
    int window_w, window_h, x=-1, y=-1;
	int enc_fd;

    repd = &enc_data->repd[stream];
	enc = &repd->enc;
    enc_ext_parm = &enc_data->enc_ext[stream];
	enc_fd = enc_data->ebt[stream].enc_fd;

    if((enc_ext_parm->feature_enable & (DVR_ENC_ROI_POS|DVR_ENC_ROI_ALL)) == 0)
        return;

    if(enc->is_use_ROI == TRUE) {
        x = enc->ROI_win.x;
        y = enc->ROI_win.y;
    }
        
    window_w = x + enc->ROI_win.width;
    window_h = y + enc->ROI_win.height;
    if((window_w > repd->dim.width) || (window_h > repd->dim.height)) {
        printk("%s: ROI window over ch=%d_%d!\n", __FUNCTION__, enc_data->ch_num, stream);
        printk("%s: ROI window (x=%d, y=%d, width=%d, height=%d) !\n", __FUNCTION__, 
                            x,
                            y,
                            enc->ROI_win.width,
                            enc->ROI_win.height);
        return;
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch=%d_%d, enc_fd=0x%x, roi=%d, x=%d, y=%d, width=%d, height=%d>\n",__FUNCTION__,__LINE__,
                            enc_data->ch_num, 
                            stream,
                            enc_fd,
                            enc->is_use_ROI, 
                            x,
                            y,
                            enc->ROI_win.width,
                            enc->ROI_win.height);

	switch(repd->enc_type) {
		case ENC_TYPE_MJPEG:
			down(&dvr_enc_snapshot_sem);
			mje_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_snapshot_sem);
			break;
		case ENC_TYPE_H264:
			down(&dvr_enc_cfg_encoder_sem);
			p200_h264e_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_cfg_encoder_sem);
			break;
        case ENC_TYPE_MPEG:
			down(&dvr_enc_cfg_encoder_sem);
			mpeg4_set_roi_coordinate(enc_fd&0xff, x, y);
			up(&dvr_enc_cfg_encoder_sem);
            break;
		default: 
			panic("%s: enc_type failed! (%d)\n", __FUNCTION__, repd->enc_type);
	}
}

static void enc_update_rc(dvr_enc_data *enc_data, int stream, int enc_type_change)
{
	switch(enc_data->repd[stream].enc_type) {
		case ENC_TYPE_MJPEG:
			down(&dvr_enc_snapshot_sem);
			do_set_MJ_RC(enc_data, stream);
			up(&dvr_enc_snapshot_sem);
			break;
		case ENC_TYPE_H264:
			down(&dvr_enc_cfg_encoder_sem);
			if(enc_type_change)
				do_set_H264_RC(enc_data, stream, 1);
			else
				do_set_H264_RC(enc_data, stream, 0);
			up(&dvr_enc_cfg_encoder_sem);
			break;
        case ENC_TYPE_MPEG:
			down(&dvr_enc_cfg_encoder_sem);
			if(enc_type_change)
				do_set_MPEG4_RC(enc_data, stream, 1);
			else
				do_set_MPEG4_RC(enc_data, stream, 0);
			up(&dvr_enc_cfg_encoder_sem);
            break;
		default: 
			panic("%s: enc_type failed! (%d)\n", __FUNCTION__, enc_data->repd[stream].enc_type);
	}
}

static void enc_update_rc_convergence(dvr_enc_data *enc_data, int stream)
{
	switch(enc_data->repd[stream].enc_type) {
		case ENC_TYPE_H264:
			down(&dvr_enc_cfg_encoder_sem);
			do_set_H264_RC_convergence(enc_data, stream);
			up(&dvr_enc_cfg_encoder_sem);
			break;
        case ENC_TYPE_MPEG:
			down(&dvr_enc_cfg_encoder_sem);
			do_set_MPEG4_RC_convergence(enc_data, stream);
			up(&dvr_enc_cfg_encoder_sem);
            break;
		case ENC_TYPE_MJPEG:
			break;
		default: 
			panic("%s: enc_type failed! (%d)\n", __FUNCTION__, enc_data->repd[stream].enc_type);
	}
}

static void do_set_channel_H264_param(dvr_enc_data *enc_data, int stream)
{
    dvr_enc_bt *ebt;	
    EncParam *enc;
    FuncTag tag, *ptag;
	int enc_fd;
	dvr_graph	*graph=enc_data->graph;
    ENC_EXT_STRUCT *enc_ext_parm;

	ebt = &enc_data->ebt[stream];  
	enc = &enc_data->repd[stream].enc;
	enc_fd = ebt->enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	ebt->enc_fps = enc->frame_rate;  //for encoder

	down(&dvr_enc_cfg_encoder_sem);
    if(platform_version == GM8126_TEST_CHIP_VERSION) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch%d_%d:ENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__, enc_data->ch_num, stream);
    	set_h264_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_H2642D);
    } else {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch%d_%d:ENC_INPUT_1D422>\n",__FUNCTION__,__LINE__, enc_data->ch_num, stream);
    	set_h264_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_1D422);
    }
	do_set_H264_RC(enc_data, stream, 1);
	up(&dvr_enc_cfg_encoder_sem);
}

static void do_set_channel_MPEG4_param(dvr_enc_data *enc_data, int stream)
{
    dvr_enc_bt *ebt;	
    EncParam *enc;
    FuncTag tag, *ptag;
	int enc_fd;
	dvr_graph	*graph=enc_data->graph;
    ENC_EXT_STRUCT *enc_ext_parm;

	ebt = &enc_data->ebt[stream];  
	enc = &enc_data->repd[stream].enc;
	enc_fd = ebt->enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	/* set frame rate */
	ebt->enc_fps = enc->frame_rate;  //for encoder

	down(&dvr_enc_cfg_encoder_sem);
    if(platform_version == GM8126_TEST_CHIP_VERSION) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch%d_%d: ENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__, enc_data->ch_num, stream);
    	set_mpeg4_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_H2642D);
    } else {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <ch%d_%d: ENC_INPUT_1D422>\n",__FUNCTION__,__LINE__, enc_data->ch_num, stream);
    	set_mpeg4_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_1D422);
    }
	do_set_MPEG4_RC(enc_data, stream, 1);
	up(&dvr_enc_cfg_encoder_sem);
}

static void do_set_channel_MJ_param(dvr_enc_data *enc_data, int stream)
{
	dvr_graph *graph=enc_data->graph;
    dvr_enc_bt *ebt;	
    EncParam *enc;
    FuncTag tag, *ptag;
	int enc_fd;
    ENC_EXT_STRUCT *enc_ext_parm;

	ebt = &enc_data->ebt[stream];  
	enc = &enc_data->repd[stream].enc;
	enc_fd = ebt->enc_fd;
    enc_ext_parm = &enc_data->enc_ext[stream];
    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	ebt->enc_fps = enc->frame_rate;  //for encoder
	/* set MJPEG encoder */
	down(&dvr_enc_snapshot_sem);
	do_set_MJ_RC(enc_data, stream);
    if(platform_version == GM8126_TEST_CHIP_VERSION) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__);
        set_mjpeg_property(graph, ptag, enc_fd, -1, enc_data, stream, JENC_INPUT_H2642D);
    } else {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_1D422>\n",__FUNCTION__,__LINE__);
        set_mjpeg_property(graph, ptag, enc_fd, -1, enc_data, stream, JENC_INPUT_1D422);
    }
	up(&dvr_enc_snapshot_sem);
}

static void do_set_channel_enc(dvr_enc_data *enc_data, int stream)
{
    FuncTag tag, *ptag;
	dvr_graph		*graph=enc_data->graph;
    dvr_enc_bt		*ebt;	
	ReproduceBitStream *repd;
    dvr_enc_graph_link_t      *graph_link_info;
    char tmpstr[DVR_ENC_MAX_STRING];
    unsigned int len;
    dvr_graph_vqueuet *qst;

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);

    graph_link_info = &enc_data->graph_link_info[stream][0];

	repd = &enc_data->repd[stream];
	ebt = &enc_data->ebt[stream];  

    VDBGPRINT(DBG_DVR_FNC, "%s:%d <roi=%d,%d,%d,%d,%d>\n",__FUNCTION__,__LINE__,
                            repd->enc.is_use_ROI,
                            repd->enc.ROI_win.x,
                            repd->enc.ROI_win.y,
                            repd->enc.ROI_win.width,
                            repd->enc.ROI_win.height);

    // Update parameters for encoder
	switch(repd->enc_type) {
		case ENC_TYPE_MPEG:
			do_set_channel_MPEG4_param(enc_data, stream);
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "mpeg4_%d_%d", enc_data->ch_num, stream);
	        do_set_graph_link_info(graph_link_info, tmpstr, ebt->enc_fd, ebt->enc_fps, 1, ebt->enc_out_qst.que);
			break;
		case ENC_TYPE_MJPEG:
 			do_set_channel_MJ_param(enc_data, stream);
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "mjpeg_%d_%d", enc_data->ch_num, stream);
	        do_set_graph_link_info(graph_link_info, tmpstr, ebt->enc_fd, ebt->enc_fps, 1, ebt->enc_out_qst.que);
			break;
		case ENC_TYPE_H264:
			do_set_channel_H264_param(enc_data, stream);
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "h264_%d_%d", enc_data->ch_num, stream);
	        do_set_graph_link_info(graph_link_info, tmpstr, ebt->enc_fd, ebt->enc_fps, 1, ebt->enc_out_qst.que);
			break;
		case ENC_TYPE_YUV422:
            len = repd->dim.width * repd->dim.height * 2;
		    qst = &ebt->enc_out_qst;
            if(len > qst->size) {
                panic("%s: ch_%d_%d YUV422 buffer is not enough! (gmdvr_mem_buf:%d input_buf:%d)\n", __FUNCTION__, 
                                    enc_data->ch_num, stream,
                                    qst->size,
                                    len);
            }
		    ebt->enc_fps = repd->enc.frame_rate;
            set_scalar_property(graph, ptag, ebt->scl_fd, SCALE_YUV422, enc_data, stream, 1);
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "yuv422_%d_%d", enc_data->ch_num, stream);
	        do_set_graph_link_info(graph_link_info, tmpstr, ebt->scl_fd, ebt->enc_fps, 1, ebt->enc_out_qst.que);
    		break;
		default: 
			panic("%s: enc_type failed! (%d)\n", __FUNCTION__, repd->enc_type);
	}
    snprintf(tmpstr, DVR_ENC_MAX_STRING, "dataout_%d_%d", enc_data->ch_num, stream);
    do_set_graph_link_info(graph_link_info, tmpstr, ebt->dataout_fd, ebt->enc_fps, 1, NULL);

}

static void do_init_graph_link_info(dvr_enc_graph_link_t *g_link)
{
    memset(g_link, 0, sizeof(dvr_enc_graph_link_t)*DVR_ENC_MAX_GRAPH_LINK_NUM);
}

static int do_set_channel_scl_param(dvr_enc_data *enc_data, int stream)
{
    int scl_fit, i;
    dvr_enc_scl_t *enc_scl;
	ReproduceBitStream *repd;
    dvr_enc_bt		*ebt;	
    FuncTag tag, *ptag;
	dvr_graph		*graph=enc_data->graph;

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	ebt = &enc_data->ebt[stream];  
	repd = &enc_data->repd[stream];
    scl_fit=0;

    for(i=0; i<DVR_ENC_SCALAR_NUM; i++) {
        enc_scl = &enc_data->enc_scl[i];
        if(enc_scl->dim.width==repd->dim.width) {
            if(enc_scl->enabled == 0) {
                enc_scl->enabled = 1;
                //enc_scl->dim.height = repd->dim.height;
                //enc_scl->dim.width = repd->dim.width;
                if(platform_version == GM8126_TEST_CHIP_VERSION) {  /* test chip use scaler to do format transfer and scalling down */
                    VDBGPRINT(DBG_DVR_FNC, "%s:%d <TC scal_yuv420>\n",__FUNCTION__,__LINE__);
                    enc_scl->format=SCALE_H264_YUV420_MODE0;
                    set_scalar_property(graph, ptag, enc_scl->fd, SCALE_H264_YUV420_MODE0, enc_data, stream, 0);
                } else {
                    VDBGPRINT(DBG_DVR_FNC, "%s:%d <MP scal_yuv422>\n",__FUNCTION__,__LINE__);
                    enc_scl->format=SCALE_YUV422;
                    set_scalar_property(graph, ptag, enc_scl->fd, SCALE_YUV422, enc_data, stream, 0);
                }
            } 
            scl_fit = 1;
            break;
        }
    }
    if(scl_fit == 0) { /* error: no scalar */
        for(i=0; i<DVR_ENC_SCALAR_NUM; i++) {
            enc_scl = &enc_data->enc_scl[i];
            printk("scl%d_out%d_max_w=%d, scl%d_out%d_max_h=%d\n", i, 
                                        enc_data->ch_num,
                                        enc_scl->dim.width, 
                                        i,
                                        enc_data->ch_num, 
                                        enc_scl->dim.height);
        }
        panic("%s:%d <(%d:%d)scalar not found! %d*%d>\n",__FUNCTION__,__LINE__,enc_data->ch_num, stream, repd->dim.width, repd->dim.height);
    }
    return i;
}

static void do_set_channel_snapshot_param(dvr_enc_data *enc_data, int stream)
{
    dvr_enc_bt *ebt;	
	snapshot_param *snap;
    FuncTag tag, *ptag;
	int ss_enc_fd, ss_dataout_fd, fps, i, snap_f;
	dvr_graph	*graph=enc_data->graph;
	mje_process param;
    dvr_enc_graph_link_t *graph_ss_link_info;
    dvr_graph_vqueuet   *q;

	ebt = &enc_data->ebt[stream];  
    /* check snapshot enable state */
    snap_f = 0;
    for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
	    ebt = &enc_data->ebt[i];  
   	    if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
   	        if(snap_f == 0) 
       	        snap_f = 1;
   	        else 
   	            panic("%s: snapshot double enabled! (%d)\n",__FUNCTION__ ,stream);
   	    }
    }
	ebt = &enc_data->ebt[stream];  
	ss_enc_fd = ebt->ss_enc_fd;
	ss_dataout_fd = ebt->ss_dataout_fd;
	fps = enc_data->repd[stream].enc.frame_rate;
    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);

	snap = &enc_data->repd[stream].snap;
	down(&dvr_enc_snapshot_sem);
    param.roi_x = param.roi_y = -1;
	param.u32ImageQuality = snap->quality;
	param.u8JPGPIC = 1;
	init_mje_rc(&param, ss_enc_fd&0xff);
    if(platform_version == GM8126_TEST_CHIP_VERSION) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__);
        set_mjpeg_property(graph, ptag, ss_enc_fd, 0, enc_data, stream, JENC_INPUT_H2642D);
    } else {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_1D422>\n",__FUNCTION__,__LINE__);
        set_mjpeg_property(graph, ptag, ss_enc_fd, 0, enc_data, stream, JENC_INPUT_1D422);
    }
	up(&dvr_enc_snapshot_sem);
	q = &enc_data->ebt[0].enc_out_qst_snap; /* use main-bitstream snapshot queue buffer */
    graph_ss_link_info = &enc_data->graph_ss_link_info[0];
	do_set_graph_link_info(graph_ss_link_info, "ss_mjpeg", ss_enc_fd, fps, 1, q->que);
    do_set_graph_link_info(graph_ss_link_info, "ss_dataout", ss_dataout_fd, fps, 1, NULL);
}

static int do_set_channel_param(dvr_enc_data *enc_data, int stream)
{
    int i, max_fps;
    FuncTag tag, *ptag;
    int cap_fd=enc_data->cap_fd[0];
	 int isp_fd=enc_data->isp_fd;
    int di_fd=enc_data->DI_fd[0];
	dvr_graph		*graph=enc_data->graph;
    dvr_enc_bt		*ebt;	
    EncParam		*enc;
	ReproduceBitStream *repd, *mainRepd;
	dvr_enc_src_param *src;
    int CapPathMode = 0;   //0:monopolize 1:Top/Bottom independent 2:Share path
    dvr_enc_scl_t *enc_scl;
    dvr_enc_graph_link_t      *graph_link_info, *graph_ss_link_info;
    char tmpstr[DVR_ENC_MAX_STRING];


    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- IN\n", enc_data->ch_num, __FUNCTION__);
    graph_link_info = &enc_data->graph_link_info[stream][0];
    do_init_graph_link_info(graph_link_info);
	src = &enc_data->src;
    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);

	for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
	    if(enc_data->ebt[i].enabled!=DVR_ENC_EBST_ENABLE)
	        continue;
		enc = &enc_data->repd[i].enc;
		if(enc->frame_rate == 0) {
			printk("[dvr_enc:%d] frame rate can't be 0. \n", enc_data->ch_num);
			return -1;
		}

		if((src->input_system == MCP_VIDEO_PAL) && (enc->frame_rate > 25))
			enc->frame_rate	= 25;
	}

    if((src->vp_param.is_3DI) && (src->di_mode==LVFRAME_GM3DI_FORMAT)) {
        enc_data->is_link_3DI = TRUE;
    } else  if(src->vp_param.is_denoise) {
        enc_data->is_link_3DI = TRUE;
    } else {
		enc_data->is_link_3DI = FALSE;
	}

    if(bShareCapPath){
        if(src->vp_param.is_3DI){
            int src_FrameRate;
            if(src->input_system == MCP_VIDEO_NTSC) {
                src_FrameRate = 30;
            }
            else
                src_FrameRate = 25;
            if((src_FrameRate/enc_data->repd[0].enc.frame_rate)>=2)
                CapPathMode = 2;
        }
        else if(src->mode == 1){
            CapPathMode = 1;
        }
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <num=%d_%d, cap=%dx%d, enc=%dx%d is_link_3DI=%x, fps=%d>\n",__FUNCTION__,__LINE__,
                                    enc_data->ch_num, stream, 
                                    src->dim.width, src->dim.height,
                                    enc_data->repd[stream].dim.width, enc_data->repd[stream].dim.height,
                                    enc_data->is_link_3DI, 
                                    enc_data->fps);
	mainRepd = &enc_data->repd[0];
	ebt = &enc_data->ebt[0];  
	enc_scl = &enc_data->enc_scl[0];
	if(stream ==0) { /* for main-bitstream */
        // Update parameters for Capture
		enc_data->fps = mainRepd->enc.frame_rate;  
        if((src->dim.width > ebt->max_dim.width) || (src->dim.height > ebt->max_dim.height)) {
            panic("%s: enc_out%d capture buffer is not enough!(buf:%dx%d input_buf:%dx%d)\n", __FUNCTION__, 
                                enc_data->ch_num,
                                ebt->max_dim.width,
                                ebt->max_dim.height,
                                src->dim.width,
                                src->dim.height);
        }
	if(enc_data->ch_num!=ISP_CHANNEL_NU)			
		set_cap_property(graph, ptag, cap_fd, enc_data);
		else{
						dvr_graph_set_property(graph, ptag, isp_fd, PROPID(isp_fd, "out_fps"), enc_data->fps, 1);//for isp
		}	
	    // Update parameters for 3DI
	    if(enc_data->is_link_3DI) {
			set_3di_property(graph, ptag, di_fd, enc_data);
		}
    	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) 
    		ebt->en_scl_ss = DVR_ENC_EBST_ENABLE;
    		
	} else {  /* for sub-bitstream */
    	max_fps = get_max_fps(enc_data);
    	/* max-fps change */
    	if(max_fps != enc_data->fps) {
    	    enc_data->fps = max_fps;
					if(enc_data->ch_num!=ISP_CHANNEL_NU){
	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Numerator"), enc_data->fps, 1);
  	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), get_cap_src_max_fps(enc_data->ch_num, src->input_system), 1);
	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "updata"), 0, 1);   //apply it
						}
					else{
						dvr_graph_set_property(graph, ptag, isp_fd, PROPID(isp_fd, "out_fps"), enc_data->fps, 1);//for isp
						}
            for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
                ebt = &enc_data->ebt[i];  
        	    if(ebt->enabled!=DVR_ENC_EBST_ENABLE || i==stream)
        	        continue;
                enc_update_write_cap_linkflow(enc_data, i, max_fps);
                enc_update_write_flow(enc_data, i, 0);
            }
            dvr_graph_set_flow_flag();
        }
	}
	if(enc_data->ch_num!=ISP_CHANNEL_NU)
    do_set_graph_link_info(graph_link_info, "cap", cap_fd, enc_data->fps, 0, enc_data->enc_in_qst.que);
	else
		do_set_graph_link_info(graph_link_info, "isp", isp_fd, enc_data->fps, 0, enc_data->enc_in_qst.que);
    if(enc_data->is_link_3DI) {
        do_set_graph_link_info(graph_link_info, "3di", di_fd, enc_data->fps, 0, enc_data->enc_in_dup_q.que);
    }
    
    /* for special cap mode */
if(enc_data->ch_num!=ISP_CHANNEL_NU)
    set_cap_path_mode_property(graph, ptag, enc_data, CapPathMode); 
	ebt = &enc_data->ebt[stream];  
	repd = &enc_data->repd[stream];
    if(platform_version == GM8126_TEST_CHIP_VERSION) {  /* test chip use scaler to do format transfer and scalling down */
        i = do_set_channel_scl_param(enc_data, stream);
        if(i != -1) {
            ebt->en_scl = 1;
        	enc_scl = &enc_data->enc_scl[i];
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "scl%d", i);
            do_set_graph_link_info(graph_link_info, tmpstr, enc_scl->fd, enc_data->fps, 0, enc_scl->enc_scl_q.que);
        }
    } else {
    	switch(repd->enc_type) {
    		case ENC_TYPE_MPEG:
    		case ENC_TYPE_MJPEG:
    		case ENC_TYPE_H264:
                if(src->dim.width != repd->dim.width) {     /* use scaler to scalling down */
                    i = do_set_channel_scl_param(enc_data, stream);
                    if(i != -1) {
                        ebt->en_scl = 1;
                    	enc_scl = &enc_data->enc_scl[i];
                        snprintf(tmpstr, DVR_ENC_MAX_STRING, "scl%d", i);
                        do_set_graph_link_info(graph_link_info, tmpstr, enc_scl->fd, enc_data->fps, 0, enc_scl->enc_scl_q.que);
                    }
                } 
    			break;
			case ENC_TYPE_YUV422:
			default: 
    			break;
    	}
    }
    /* don't move it, flow dependent */
   	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
        graph_ss_link_info = &enc_data->graph_ss_link_info[0];
        memcpy(graph_ss_link_info, graph_link_info, sizeof(dvr_enc_graph_link_t)*DVR_ENC_MAX_GRAPH_LINK_NUM);
        do_set_channel_snapshot_param(enc_data, stream);
	}
    do_set_channel_enc(enc_data, stream);
    return 0;
}

static int enc_update_channel_param(dvr_enc_data *enc_data, int stream)
{
    int i, ch_number, max_fps, exist_stream;
    FuncTag tag, *ptag;
    int cap_fd=enc_data->cap_fd[0];
    int di_fd=enc_data->DI_fd[0];
	dvr_graph		*graph=enc_data->graph;
    dvr_enc_bt		*ebt;	
    EncParam		*enc;
	ReproduceBitStream *repd;
    int enc_fd;
	dvr_enc_src_param *src;
    int CapPathMode = 0;
    dvr_enc_scl_t *enc_scl;
    dvr_enc_graph_link_t      *graph_link_info, *graph_ss_link_info;
    char tmpstr[DVR_ENC_MAX_STRING];
    
    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- IN\n", enc_data->ch_num, __FUNCTION__);
	src = &enc_data->src;

    ptag = &tag;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
	    if(enc_data->ebt[i].enabled!=DVR_ENC_EBST_ENABLE)
	        continue;
		enc = &enc_data->repd[i].enc;
		if(enc->frame_rate == 0) {
			printk("[dvr_enc:%d] frame rate can't be 0. \n", enc_data->ch_num);
			return -1;
		}

		if((src->input_system == MCP_VIDEO_PAL) && (enc->frame_rate > 25))
			enc->frame_rate	= 25;
	}

    if((src->vp_param.is_3DI) && (src->di_mode==LVFRAME_GM3DI_FORMAT))
        enc_data->is_link_3DI = TRUE;
    else  if(src->vp_param.is_denoise)
        enc_data->is_link_3DI = TRUE;
    else 
		enc_data->is_link_3DI = FALSE;

    if(bShareCapPath){
        if(src->vp_param.is_3DI){
            int src_FrameRate;
            if(src->input_system == MCP_VIDEO_NTSC) {
                src_FrameRate = 30;
            }
            else
                src_FrameRate = 25;
            if((src_FrameRate/enc_data->repd[0].enc.frame_rate)>=2)
                CapPathMode = 2;
        }
        else if(src->mode == 1){
            CapPathMode = 1;
        }
    }
    /* check whether stream is enabled */
    exist_stream = 0;
    for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
        ebt = &enc_data->ebt[i];  
	    if((ebt->enabled == DVR_ENC_EBST_ENABLE) && (i!= stream)) {
	        exist_stream = 1;
	        break;
	    }
    }
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <exist_stream=%d>\n",__FUNCTION__,__LINE__, exist_stream);
    if(exist_stream == 0) {
		enc = &enc_data->repd[stream].enc;
        ebt = &enc_data->ebt[stream];  
        // Update parameters for Capture
		enc_data->fps = enc->frame_rate;  
        if((src->dim.width > ebt->max_dim.width) || (src->dim.height > ebt->max_dim.height)) {
            panic("%s: enc_out%d capture buffer is not enough!(buf:%dx%d input_buf:%dx%d)\n", __FUNCTION__, 
                                enc_data->ch_num,
                                ebt->max_dim.width,
                                ebt->max_dim.height,
                                src->dim.width,
                                src->dim.height);
        }
	if(enc_data->ch_num!=ISP_CHANNEL_NU)//for isp
		set_cap_property(graph, ptag, cap_fd, enc_data);
	else
		dvr_graph_set_property(graph, ptag, enc_data->isp_fd, PROPID(enc_data->isp_fd, "out_fps"), enc_data->fps, 1);//for isp
	    // Update parameters for 3DI
	    if(enc_data->is_link_3DI) {
			set_3di_property(graph, ptag, di_fd, enc_data);
		}
    	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) 
    		ebt->en_scl_ss = DVR_ENC_EBST_ENABLE;
    } else {
    	max_fps = get_max_fps(enc_data);
    	if(max_fps != enc_data->fps && (max_fps != 0)) {
    	    enc_data->fps = max_fps;
		if(enc_data->ch_num!=ISP_CHANNEL_NU){//for isp
            dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Numerator"), enc_data->fps, 1);
            dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), get_cap_src_max_fps(enc_data->ch_num, src->input_system), 1);
            dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "updata"), 0, 1);   //apply it
			}
       	else{
		 	dvr_graph_set_property(graph, ptag, enc_data->isp_fd, PROPID(enc_data->isp_fd, "out_fps"), enc_data->fps, 1);//for isp
			}
            for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
                ebt = &enc_data->ebt[i];  
        	    if(ebt->enabled!=DVR_ENC_EBST_ENABLE || i==stream) {
        	        continue;
        	    }
                enc_update_write_cap_linkflow(enc_data, i, max_fps);
                enc_update_write_flow(enc_data, i, 1);//Jerry test

            	if ((i==0) && (ebt->en_snapshot==DVR_ENC_EBST_ENABLE)) {
            	    enc_update_write_ss_linkflow(enc_data);
            	    enc_update_write_ss_flow(enc_data, i, 0);
            	}
            }
            dvr_graph_set_flow_flag();
        }
    }
    graph_link_info = &enc_data->graph_link_info[stream][0];
    do_init_graph_link_info(graph_link_info);
	if(enc_data->ch_num!=ISP_CHANNEL_NU)
    	do_set_graph_link_info(graph_link_info, "cap", cap_fd, enc_data->fps, 0, enc_data->enc_in_qst.que);
	else
		do_set_graph_link_info(graph_link_info, "isp", enc_data->isp_fd, enc_data->fps, 0, enc_data->enc_in_qst.que);
    if(enc_data->is_link_3DI) {
        do_set_graph_link_info(graph_link_info, "3di", di_fd, enc_data->fps, 0, enc_data->enc_in_dup_q.que);
    }
    
    /* for special cap mode */
		if(enc_data->ch_num!=ISP_CHANNEL_NU)
    set_cap_path_mode_property(graph, ptag, enc_data, CapPathMode);
	ebt = &enc_data->ebt[stream];  
	repd = &enc_data->repd[stream];
    if(platform_version == GM8126_TEST_CHIP_VERSION) {  /* test chip use scaler to do format transfer and scalling down */
        i = do_set_channel_scl_param(enc_data, stream);
        if(i != -1) {
            ebt->en_scl = 1;
        	enc_scl = &enc_data->enc_scl[i];
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "scl%d", i);
            do_set_graph_link_info(graph_link_info, tmpstr, enc_scl->fd, enc_data->fps, 0, enc_scl->enc_scl_q.que);
        }
    } else {
        if(src->dim.width > repd->dim.width) {     /* use scaler to scalling down */
            i = do_set_channel_scl_param(enc_data, stream);
            if(i != -1) {
                ebt->en_scl = 1;
            	enc_scl = &enc_data->enc_scl[i];
                snprintf(tmpstr, DVR_ENC_MAX_STRING, "scl%d", i);
                do_set_graph_link_info(graph_link_info, tmpstr, enc_scl->fd, enc_data->fps, 0, enc_scl->enc_scl_q.que);
            }
        }
    }
    /* don't move it, flow dependent */
   	if (ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
        graph_ss_link_info = &enc_data->graph_ss_link_info[0];
        memcpy(graph_ss_link_info, graph_link_info, sizeof(dvr_enc_graph_link_t)*DVR_ENC_MAX_GRAPH_LINK_NUM);
        do_set_channel_snapshot_param(enc_data, stream);
	}
	enc = &enc_data->repd[stream].enc;
    DV_RESET_TAG(ptag);
    DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[stream], enc_data->ch_num);
	ebt->enc_fps = enc->frame_rate;  //for encoder
	ch_number = enc_data->ch_num + DVR_RECORD_CHANNEL_NUM * stream;
	switch(repd->enc_type) {
		case ENC_TYPE_MJPEG:
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "mjpeg");
			ebt->enc_fd = ENTITY_FD(ENC_MJPEG_FD_MAJOR, ENC_MJPEG_FD_MINOR(ch_number));
			enc_fd = ebt->enc_fd;
			down(&dvr_enc_snapshot_sem);
            if(platform_version == GM8126_TEST_CHIP_VERSION) {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__);
                set_mjpeg_property(graph, ptag, enc_fd, -1, enc_data, stream, JENC_INPUT_H2642D);
            } else {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <JENC_INPUT_1D422>\n",__FUNCTION__,__LINE__);
                set_mjpeg_property(graph, ptag, enc_fd, -1, enc_data, stream, JENC_INPUT_1D422);
            }
			up(&dvr_enc_snapshot_sem);
			break;
		case ENC_TYPE_MPEG:
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "mpeg4");
			ebt->enc_fd = ENTITY_FD(ENC_MPEG4_FD_MAJOR, ENC_MPEG4_FD_MINOR(ch_number));
			enc_fd = ebt->enc_fd;
			down(&dvr_enc_cfg_encoder_sem);
            if(platform_version == GM8126_TEST_CHIP_VERSION) {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <ENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__);
                set_mpeg4_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_H2642D);
            } else {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <ENC_INPUT_1D422>\n",__FUNCTION__,__LINE__);
                set_mpeg4_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_1D422);
            }
			up(&dvr_enc_cfg_encoder_sem);
		    break;
		case ENC_TYPE_H264:
            snprintf(tmpstr, DVR_ENC_MAX_STRING, "h264");
			ebt->enc_fd = ENTITY_FD(ENC_H264_FD_MAJOR, ENC_H264_FD_MINOR(ch_number));
			enc_fd = ebt->enc_fd;
			down(&dvr_enc_cfg_encoder_sem);
            if(platform_version == GM8126_TEST_CHIP_VERSION) {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <ENC_INPUT_H2642D>\n",__FUNCTION__,__LINE__);
                set_h264_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_H2642D);
            } else {
                VDBGPRINT(DBG_DVR_FNC, "%s:%d <ENC_INPUT_1D422>\n",__FUNCTION__,__LINE__);
                set_h264_property(graph, ptag, enc_fd, enc_data, stream, ENC_INPUT_1D422);
            }
			up(&dvr_enc_cfg_encoder_sem);
			break;
		default: 
			panic("%s: enc_type failed! (%d)\n",__FUNCTION__, repd->enc_type);
	}

	do_set_graph_link_info(graph_link_info, tmpstr, ebt->enc_fd, ebt->enc_fps, 1, ebt->enc_out_qst.que);
    snprintf(tmpstr, DVR_ENC_MAX_STRING, "dataout%d",stream);
    do_set_graph_link_info(graph_link_info, tmpstr, ebt->dataout_fd, ebt->enc_fps, 1, NULL);
    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- OUT\n", enc_data->ch_num, __FUNCTION__);
    return 0;
}

int verify_parameter(dvr_enc_data *enc_data, dvr_enc_control *up_ch_param)
{
   dvr_enc_bt	*ebt;
   int val;
 	if((up_ch_param->stream >= DVR_ENC_REPD_BT_NUM) || (up_ch_param->update_parm.frame_rate > 30)) {
		
		printk("Invalid parameter\n");
		return -1;
	}

	ebt = &enc_data->ebt[up_ch_param->stream];
	if(ebt->enc_fd == 0) {
		printk("The stream hasn't initial, do it before update\n");
		return -1;
	}
    if(ebt->enc_type == ENC_TYPE_YUV422) {
		printk("The raw stream does not support update function.\n");
		return -1;
    }
	if((up_ch_param->update_parm.frame_rate <= 0) &&
	   (up_ch_param->update_parm.frame_rate != GMVAL_DO_NOT_CARE)) {
		  printk("Wrong parameter frame_rate=%d\n",up_ch_param->update_parm.frame_rate);
		  return -1;	
	}
    //EVBR mode: bit_rate=0 and target_rate_max != 0
	if((up_ch_param->update_parm.bit_rate < 0) &&
	   (up_ch_param->update_parm.bit_rate != GMVAL_DO_NOT_CARE)) {
		  printk("Wrong parameter bit_rate=%i\n",up_ch_param->update_parm.bit_rate);
		  return -1;	
	}
	if((up_ch_param->update_parm.ip_interval <= 0) &&
	   (up_ch_param->update_parm.ip_interval != GMVAL_DO_NOT_CARE)) {
		  printk("Wrong parameter ip_interval=%d\n",up_ch_param->update_parm.ip_interval);
		  return -1;	
	}
	if((up_ch_param->update_parm.dim.width < 16) &&
	   (up_ch_param->update_parm.dim.width != GMVAL_DO_NOT_CARE)) {
		  printk("Wrong parameter width=%d\n",up_ch_param->update_parm.dim.width);
		  return -1;	
	}
	if((up_ch_param->update_parm.dim.height < 16) &&
	   (up_ch_param->update_parm.dim.height != GMVAL_DO_NOT_CARE)) {
		  printk("Wrong parameter height=%d\n",up_ch_param->update_parm.dim.height);
		  return -1;	
	}
	if(up_ch_param->update_parm.init_quant != GMVAL_DO_NOT_CARE) {
	   val = up_ch_param->update_parm.init_quant & 0x0000ffff;
	   if(val < 1 || val > 51) {
		  printk("Wrong parameter init_quant=%d\n",val);
		  return -1;	
		}
	}
	if(up_ch_param->update_parm.max_quant != GMVAL_DO_NOT_CARE) {
	   val = up_ch_param->update_parm.max_quant & 0x0000ffff;
	   if(val < 1 || val > 51) {
		  printk("Wrong parameter max_quant=%d\n",val);
		  return -1;	
		}
	}
	if(up_ch_param->update_parm.min_quant != GMVAL_DO_NOT_CARE) {
	   val = up_ch_param->update_parm.min_quant & 0x0000ffff;
	   if(val < 1 || val > 51) {
		  printk("Wrong parameter min_quant=%d\n",val);
		  return -1;	
		}
	}
	return 0;
}

int enc_update_ip_interval(dvr_enc_data *enc_data, int stream)
{
    EncParam		*enc;
    ReproduceBitStream *repd;
	int enc_fd;
	
    enc = &enc_data->repd[stream].enc;
    repd = &enc_data->repd[stream];
    enc_fd = enc_data->ebt[stream].enc_fd;

    VDBGPRINT(DBG_DVR_FNC, "%s:%d <enc_type=%d, fd=%x, gop=%d>\n",__FUNCTION__, __LINE__, repd->enc_type, enc_fd, enc->ip_interval);
	down(&dvr_enc_cfg_encoder_sem);
	switch(repd->enc_type) {
		case ENC_TYPE_H264:
	        update_h264_gop_property(enc_fd, enc->ip_interval);
		    break;
		case ENC_TYPE_MPEG:
    		update_mpeg4_gop_property(enc_fd, enc->ip_interval);
		    break;
	}
	up(&dvr_enc_cfg_encoder_sem);	
	return 0;
}

static int do_update_channel(dvr_enc_data *enc_data, dvr_enc_control *up_ch_param)
{
    FuncTag tag, *ptag;
    EncParam		*enc;
    dvr_graph  	*graph;
    ReproduceBitStream	*stream_parm;
    int res_change=0, framerate_change=0, enabled_change=0;
    int ipinterval_change = 0, rc_change=0, roi_pos_change=0;
    int bfrc=0;
    int ret, i;
    int stream_num = up_ch_param->stream;
    ENC_EXT_STRUCT *enc_ext_parm;
    int enc_type_change=0, old_enc_type=0;
    dvr_enc_bt		*ebt;	
    int cap_fd=enc_data->cap_fd[0];
	dvr_enc_src_param *src;
    
	ret = verify_parameter(enc_data, up_ch_param);
	if(ret < 0) {
		return -1;
	}

	enc = &enc_data->repd[up_ch_param->stream].enc;
	graph = enc_data->graph;
    stream_parm = &enc_data->repd[up_ch_param->stream];
    enc_ext_parm = &enc_data->enc_ext[up_ch_param->stream];

    ptag = &tag;
    
    VDBGPRINT(DBG_DVR_FNC, "%s: stream %d_%d enable %d DI_mode %d, mode %d, scaldep %d\n", __FUNCTION__,
				enc_data->ch_num, up_ch_param->stream, up_ch_param->update_parm.stream_enable, up_ch_param->update_parm.src.di_mode,
				up_ch_param->update_parm.src.mode, up_ch_param->update_parm.src.scale_indep);
        
    VDBGPRINT(DBG_DVR_FNC, "%s: bitrate %d FPS %d, GOP %d, resolution %dx%d\n", __FUNCTION__,
				up_ch_param->update_parm.bit_rate, up_ch_param->update_parm.frame_rate, up_ch_param->update_parm.ip_interval,
				up_ch_param->update_parm.dim.width, up_ch_param->update_parm.dim.height);

    VDBGPRINT(DBG_DVR_FNC, "%s: initq %d, maxq %d, minq %d\n", __FUNCTION__,
				up_ch_param->update_parm.init_quant, up_ch_param->update_parm.max_quant, up_ch_param->update_parm.min_quant);
	if(up_ch_param->update_parm.stream_enable == GMVAL_DO_NOT_CARE) {
        enabled_change = 0;
	} else if(up_ch_param->update_parm.stream_enable == 1) {
	    if(stream_parm->enabled==DVR_ENC_EBST_ENABLE) {   
	        enabled_change = 0;
	    } else {
	        enabled_change =  1;
	        stream_parm->enabled = DVR_ENC_EBST_ENABLE;
    		enc_data->ebt[up_ch_param->stream].enabled = DVR_ENC_EBST_ENABLE;
	    }
	} else if(up_ch_param->update_parm.stream_enable == 0) {
	    if(stream_parm->enabled==DVR_ENC_EBST_ENABLE) {   
    		stream_parm->enabled = DVR_ENC_EBST_DISABLE;
    		enc_data->ebt[up_ch_param->stream].enabled = DVR_ENC_EBST_DISABLE;		
	        enabled_change = 1;
	    } else {
	        enabled_change = 0;
	    }
	}else{
		printk("@@Error:up_ch_param->update_parm.stream_enable =%d\n",up_ch_param->update_parm.stream_enable );
		}
	if((up_ch_param->update_parm.dim.width != GMVAL_DO_NOT_CARE) && (stream_parm->dim.width != up_ch_param->update_parm.dim.width)) {
		stream_parm->dim.width = up_ch_param->update_parm.dim.width;
		rc_change = res_change = 1;
	}
	if((up_ch_param->update_parm.dim.height != GMVAL_DO_NOT_CARE) && (stream_parm->dim.height != up_ch_param->update_parm.dim.height)) {
		stream_parm->dim.height = up_ch_param->update_parm.dim.height;
		rc_change = res_change = 1;
	}			
	if((up_ch_param->update_parm.frame_rate != GMVAL_DO_NOT_CARE) && (stream_parm->enc.frame_rate != up_ch_param->update_parm.frame_rate)) {
		stream_parm->enc.frame_rate = up_ch_param->update_parm.frame_rate;
		enc_data->ebt[up_ch_param->stream].enc_fps = up_ch_param->update_parm.frame_rate;
		rc_change = framerate_change = 1;
	}
	if((up_ch_param->update_parm.bit_rate != GMVAL_DO_NOT_CARE) && (stream_parm->enc.bit_rate != up_ch_param->update_parm.bit_rate)) {
		stream_parm->enc.bit_rate = up_ch_param->update_parm.bit_rate;
		rc_change = 1;
	}
	if((up_ch_param->update_parm.ip_interval != GMVAL_DO_NOT_CARE) && (stream_parm->enc.ip_interval != up_ch_param->update_parm.ip_interval)) {
		stream_parm->enc.ip_interval = up_ch_param->update_parm.ip_interval;
		ipinterval_change = 1;
	}
	if((stream_num == 0) && (up_ch_param->update_parm.src.di_mode != GMVAL_DO_NOT_CARE) && (enc_data->src.di_mode != up_ch_param->update_parm.src.di_mode)) {
		enc_data->src.di_mode = up_ch_param->update_parm.src.di_mode;
		rc_change = res_change = 1;
	}
	if((stream_num == 0) && (up_ch_param->update_parm.src.mode != GMVAL_DO_NOT_CARE) && (enc_data->src.mode != up_ch_param->update_parm.src.mode)) {
		enc_data->src.mode = up_ch_param->update_parm.src.mode;
		rc_change = res_change = 1;
	}
	if((stream_num == 0) && (up_ch_param->update_parm.src.scale_indep != GMVAL_DO_NOT_CARE) && (enc_data->src.scale_indep != up_ch_param->update_parm.src.scale_indep)) {
		enc_data->src.scale_indep = up_ch_param->update_parm.src.scale_indep;
		rc_change = res_change = 1;
	}
	if((stream_num == 0) && (up_ch_param->update_parm.src.is_3DI != GMVAL_DO_NOT_CARE) && (enc_data->src.vp_param.is_3DI != up_ch_param->update_parm.src.is_3DI)) {
		enc_data->src.vp_param.is_3DI = up_ch_param->update_parm.src.is_3DI;
		rc_change = res_change = 1;
	}
	if((stream_num == 0) && (up_ch_param->update_parm.src.is_denoise != GMVAL_DO_NOT_CARE) && (enc_data->src.vp_param.is_denoise != up_ch_param->update_parm.src.is_denoise)) {
		enc_data->src.vp_param.is_denoise = up_ch_param->update_parm.src.is_denoise;
		rc_change = res_change = 1;
	}
	if(up_ch_param->update_parm.init_quant != GMVAL_DO_NOT_CARE) {
		stream_parm->enc.init_quant = up_ch_param->update_parm.init_quant & 0x0000ffff;
		rc_change = 1;
	}
	if(up_ch_param->update_parm.max_quant != GMVAL_DO_NOT_CARE) {
		stream_parm->enc.max_quant = up_ch_param->update_parm.max_quant & 0x0000ffff;
		rc_change = 1;
	}
	if(up_ch_param->update_parm.min_quant != GMVAL_DO_NOT_CARE) {
		stream_parm->enc.min_quant = up_ch_param->update_parm.min_quant & 0x0000ffff;
		rc_change = 1;
	}
    if(DVR_ENC_CHECK_MAGIC(up_ch_param->update_parm.ext_size) && DVR_ENC_GET_VALUE(up_ch_param->update_parm.ext_size) && up_ch_param->update_parm.pext_data){
		ENC_EXT_STRUCT EncParam_ext = {0};
		if(DVR_ENC_GET_VALUE(up_ch_param->update_parm.ext_size) <= sizeof(ENC_EXT_STRUCT)){
			if(copy_from_user((void *)&EncParam_ext, up_ch_param->update_parm.pext_data, DVR_ENC_GET_VALUE(up_ch_param->update_parm.ext_size))) {
            	printk("%s: copy_from_user failed!\n", __FUNCTION__);    
				return -1;
			}
            VDBGPRINT(DBG_DVR_FNC, "%s: EncParam_ext.feature_enable=0x%x, 0x%x\n", __FUNCTION__, 
                                    EncParam_ext.feature_enable, enc_ext_parm->feature_enable);

            if(EncParam_ext.feature_enable != GMVAL_DO_NOT_CARE){
                if(EncParam_ext.feature_enable & DVR_ENC_ENHANCE_H264_RATECONTROL) {
                    //enc_ext_parm->feature_enable = EncParam_ext.feature_enable;
                    enc_ext_parm->feature_enable |= DVR_ENC_ENHANCE_H264_RATECONTROL;
                } 
                if(EncParam_ext.feature_enable & DVR_ENC_MJPEG_FUNCTION) {
                    enc_ext_parm->feature_enable |= DVR_ENC_MJPEG_FUNCTION;
                } 
                if(EncParam_ext.feature_enable & DVR_ENC_ROI_POS) {
                    enc_ext_parm->feature_enable |= DVR_ENC_ROI_POS;
                } 
                if(EncParam_ext.feature_enable & DVR_ENC_ROI_ALL) {
                    enc_ext_parm->feature_enable |= DVR_ENC_ROI_ALL;
                } 
            }
            if(EncParam_ext.target_rate_max != GMVAL_DO_NOT_CARE){
                enc_ext_parm->target_rate_max = EncParam_ext.target_rate_max;
		        rc_change = 1;
            }
            if(EncParam_ext.reaction_delay_max != GMVAL_DO_NOT_CARE){
                enc_ext_parm->reaction_delay_max = EncParam_ext.reaction_delay_max;
		        rc_change = 1;
            }
            if(EncParam_ext.enc_type != GMVAL_DO_NOT_CARE){
				if(stream_parm->enabled == DVR_ENC_EBST_ENABLE){
					enc_type_change = 1;
					old_enc_type = EncParam_ext.enc_type;
			        rc_change = 1;
				}
                stream_parm->enc_type = enc_ext_parm->enc_type = EncParam_ext.enc_type;
            }
            if(EncParam_ext.MJ_quality != GMVAL_DO_NOT_CARE){
                enc_ext_parm->MJ_quality = EncParam_ext.MJ_quality;
                enc_ext_parm->feature_enable |= DVR_ENC_MJPEG_FUNCTION;
		        rc_change = 1;
            }
            if((EncParam_ext.feature_enable & DVR_ENC_ROI_POS) && (EncParam_ext.feature_enable != GMVAL_DO_NOT_CARE)) {
                VDBGPRINT(DBG_DVR_FNC, "%s: roi_x=%d, roi_y=%d\n", __FUNCTION__,
				    EncParam_ext.roi_pos.x, EncParam_ext.roi_pos.y);

                if(EncParam_ext.roi_pos.x != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.x = enc_ext_parm->roi_pos.x = EncParam_ext.roi_pos.x;
			        roi_pos_change = 1;
                }
                if(EncParam_ext.roi_pos.y != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.y = enc_ext_parm->roi_pos.y = EncParam_ext.roi_pos.y;
			        roi_pos_change = 1;
                }
            }
            if((EncParam_ext.feature_enable & DVR_ENC_ROI_ALL) && (EncParam_ext.feature_enable != GMVAL_DO_NOT_CARE)) {
                VDBGPRINT(DBG_DVR_FNC, "%s: is_use_ROI=%d, roi_x=%d, roi_y=%d, roi_w=%d, roi_h=%d\n", __FUNCTION__,
    				    EncParam_ext.roi_all.is_use_ROI, 
    				    EncParam_ext.roi_all.win.x,
    				    EncParam_ext.roi_all.win.y,
    				    EncParam_ext.roi_all.win.width,
    				    EncParam_ext.roi_all.win.height);
                if(EncParam_ext.roi_all.is_use_ROI != GMVAL_DO_NOT_CARE){
                    if(enc->is_use_ROI != EncParam_ext.roi_all.is_use_ROI) {
                        enc_type_change = 1;
    					old_enc_type = stream_parm->enc_type;
                    }
                    enc->is_use_ROI = enc_ext_parm->roi_all.is_use_ROI = EncParam_ext.roi_all.is_use_ROI;
                }
                if(EncParam_ext.roi_all.win.width != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.width = enc_ext_parm->roi_all.win.width = EncParam_ext.roi_all.win.width;
                    enc_type_change = 1;
					old_enc_type = stream_parm->enc_type;
                }
                if(EncParam_ext.roi_all.win.height != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.height = enc_ext_parm->roi_all.win.height = EncParam_ext.roi_all.win.height;
                    enc_type_change = 1;
					old_enc_type = stream_parm->enc_type;
                }
                if(EncParam_ext.roi_all.win.x != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.x = enc_ext_parm->roi_all.win.x = EncParam_ext.roi_all.win.x;
                    if(enc_type_change == 0) {
    			        roi_pos_change = 1;
                    }
                }
                if(EncParam_ext.roi_all.win.y != GMVAL_DO_NOT_CARE){
                    enc->ROI_win.y = enc_ext_parm->roi_all.win.y = EncParam_ext.roi_all.win.y;
                    if(enc_type_change == 0) {
    			        roi_pos_change = 1;
                    }
                }
            }
            if((EncParam_ext.feature_enable & DVR_ENC_BASE_FRAME_RATE_CONTROL) && (EncParam_ext.feature_enable != GMVAL_DO_NOT_CARE)) {

                VDBGPRINT(DBG_DVR_FNC, "%s: baseFrameRate fr=%d, frb=%d\n", __FUNCTION__,
    				    EncParam_ext.base_frame_rate_control.frame_rate,
    				    EncParam_ext.base_frame_rate_control.frame_rate_base);
                enc_ext_parm->feature_enable |= DVR_ENC_BASE_FRAME_RATE_CONTROL;
                if(EncParam_ext.base_frame_rate_control.frame_rate != GMVAL_DO_NOT_CARE) {
                    //stream_parm->enc.frame_rate = EncParam_ext.base_frame_rate_control.frame_rate;
                    enc_ext_parm->base_frame_rate_control.frame_rate = EncParam_ext.base_frame_rate_control.frame_rate;
                }
                if(EncParam_ext.base_frame_rate_control.frame_rate_base != GMVAL_DO_NOT_CARE) {
                    enc_ext_parm->base_frame_rate_control.frame_rate_base = EncParam_ext.base_frame_rate_control.frame_rate_base;
                }
                bfrc = 1;
            }
        }
        else {
            printk("%s: EncParam_Ext struct version is not correct!struct size=(ap=%d,drv=%d)\n", __FUNCTION__, DVR_ENC_GET_VALUE(up_ch_param->update_parm.ext_size), sizeof(ENC_EXT_STRUCT));    
        }
    }
    if(stream_parm->enabled == DVR_ENC_EBST_DISABLE) {
	    int max_fps, cap_fps;

		if(enabled_change == 0) {
            VDBGPRINT(DBG_DVR_FNC, "%s:%d <disable Update>\n",__FUNCTION__,__LINE__);
    	    return 0; 
		}
		max_fps=get_max_fps(enc_data);
	    cap_fps = enc_data->fps;
        if((cap_fps != max_fps) && (max_fps != 0)) {
    	    enc_data->fps = max_fps;
    	    src = &enc_data->src;
            DV_RESET_TAG(ptag);
            DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[up_ch_param->stream], enc_data->ch_num);
#if 0
	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Numerator"), enc_data->fps, 1);
  	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "FrameRate_Denominator"), get_cap_src_max_fps(enc_data->ch_num, src->input_system), 1);
	        dvr_graph_set_property(graph, ptag, cap_fd, PROPID(cap_fd, "updata"), 0, 1);   //apply it
#else
					if(enc_data->ch_num!=ISP_CHANNEL_NU)
				        update_cap_fps_property(enc_data, max_fps);
					else
		 				dvr_graph_set_property_directly(enc_data->isp_fd, PROPID(enc_data->isp_fd, "out_fps"), max_fps, 1);//for isp
#endif

            for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
                ebt = &enc_data->ebt[i];  
                if(i==up_ch_param->stream) {
                    enc_update_write_cap_linkflow(enc_data, i, max_fps);
                    enc_update_write_encoder_linkflow(enc_data, i, max_fps);
                    enc_update_write_flow(enc_data, i, 1);
					if ( (i==0) && (ebt->en_snapshot==DVR_ENC_EBST_ENABLE)) {
	                	    enc_update_write_ss_linkflow(enc_data);
	                	    enc_update_write_ss_flow(enc_data, i, 1);
	                	}
                } else if(ebt->enabled==DVR_ENC_EBST_ENABLE) {
                    enc_update_write_cap_linkflow(enc_data, i, max_fps);
                    enc_update_write_flow(enc_data, i, 1);

                	if ((i==0) && (ebt->en_snapshot==DVR_ENC_EBST_ENABLE)) {
                	    enc_update_write_ss_linkflow(enc_data);
                	    enc_update_write_ss_flow(enc_data, i, 1);
                	}
                }
            }

        //    dvr_graph_set_flow_flag();
            dvr_graph_apply_flow();
        }
			
        DV_RESET_TAG(ptag);
        DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[up_ch_param->stream], enc_data->ch_num);
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <Delete stream %d_%d link>\n",__FUNCTION__,__LINE__,enc_data->ch_num, up_ch_param->stream);
		dvr_graph_del_link(graph, ptag);
		idataout_setactive(enc_data->ch_num+ DVR_RECORD_CHANNEL_NUM * up_ch_param->stream, FALSE);
					
    	switch(enc_data->repd[up_ch_param->stream].enc_type) {
    		case ENC_TYPE_MPEG:
        		free_mpeg4_rc(enc_data->ebt[up_ch_param->stream].enc_fd&0xff);
        		break;
    		case ENC_TYPE_H264:
            	Free_RC(enc_data->ebt[up_ch_param->stream].enc_fd&0xff);
            	break;
    	}
		return 0;

    }

	if(framerate_change && enabled_change==0 && res_change==0 && enc_type_change==0) {
	    VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d framerate_change>\n",__FUNCTION__,__LINE__, enc_data->ch_num, up_ch_param->stream);
        		enc_update_framerate(enc_data, up_ch_param->stream);
    }

	if(res_change || enc_type_change || enabled_change) {
        DV_RESET_TAG(ptag);
        DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[up_ch_param->stream], enc_data->ch_num);
        if(enabled_change == 0)  /* delete old videograph link */
    		dvr_graph_del_link(graph, ptag);
        if(enc_type_change == 1) {
        	switch(old_enc_type) {
        		case ENC_TYPE_MPEG:
		            VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d:old_enc_type mpeg4 rc del>\n",__FUNCTION__,__LINE__, enc_data->ch_num, up_ch_param->stream);
            		free_mpeg4_rc(enc_data->ebt[up_ch_param->stream].enc_fd&0xff);
            		break;
        		case ENC_TYPE_H264:
		            VDBGPRINT(DBG_DVR_FNC, "%s:%d <%d_%d:old_enc_type H264 rc del>\n",__FUNCTION__,__LINE__, enc_data->ch_num, up_ch_param->stream);
                	Free_RC(enc_data->ebt[up_ch_param->stream].enc_fd&0xff);
                	break;
        	}
        }
		//To do: Update memory allocation
 		enc_update_channel_param(enc_data, up_ch_param->stream);
        set_enc_graph(enc_data, up_ch_param->stream);
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <Update %d_%d link>\n",__FUNCTION__,__LINE__,enc_data->ch_num, up_ch_param->stream);
	}
	if(ipinterval_change && !res_change) {
        VDBGPRINT(DBG_DVR_FNC, "%s:%d <Update ip interval>\n",__FUNCTION__,__LINE__);
		enc_update_ip_interval(enc_data, up_ch_param->stream);
	}
	
    if(rc_change || enabled_change || enc_type_change) {
        if(enabled_change) 
            enc_type_change = 1;
        enc_update_rc(enc_data, up_ch_param->stream, enc_type_change);
	}
    if(bfrc) {
        enc_update_rc_convergence(enc_data, up_ch_param->stream);
        enc_ext_parm->feature_enable &= ~DVR_ENC_BASE_FRAME_RATE_CONTROL;
	}
	
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <roi_pos_change=%d, enabled_change=%d, enc_type_change=%d>\n",__FUNCTION__,__LINE__, roi_pos_change, enabled_change, enc_type_change);
	if(roi_pos_change || enabled_change || enc_type_change) {
	    if(enc_type_change == 1) {
	        enc_reset_roi_pos(enc_data, up_ch_param->stream);
	    } else {
    	    enc_update_roi_pos(enc_data, up_ch_param->stream);
	    }
	}
	if(res_change || framerate_change || enc_type_change || enabled_change) {
		enc_data->update_param_flag[up_ch_param->stream] = 1;
	}
    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:%d] %s -- OUT\n", enc_data->ch_num, __FUNCTION__);
    return 0;
}

int isKeyframe(struct v_job_t *job)
{
    struct v_graph_info    *graph_info = (struct v_graph_info*) job->in_header->addr_va;
    unsigned int *drvinfo=(unsigned int *)(job->in_header->addr_va+graph_info->drvinfo_offset);
    unsigned int tag_fd;
    
    tag_fd=*drvinfo;
    if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_H264_FD_MINOR_ID)
    {
        EncoderOut_info *encout_info=(EncoderOut_info *)drvinfo;
        return encout_info->Keyframe;
    }
    else if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_MJPEG_FD_MINOR_ID)
    {
        return 1;
    }
    else if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_MPEG4_FD_MINOR_ID)
    {
        mp4e_OutInfo *encout_info=(mp4e_OutInfo *)drvinfo;
        return encout_info->Keyframe;
    }    
    else
    {
#if 1
        printk("%s:%s:Not support tag_fd 0x%x job %d at 0x%x\n",__FUNCTION__, job->entity->name, tag_fd,job->id,(int)job);
#else
        char error_msg[100];
        sprintf(error_msg,"%s:%s:Not support tag_fd 0x%x job %d at 0x%x\n",__FUNCTION__, job->entity->name, tag_fd,job->id,(int)job);
        printm_with_panic(error_msg);
#endif
    }
    return 1;
}

int parse_encoder_job(int ch_num, struct v_job_t *job, int *bs_offset, int *bs_length, 
                        int *mv_offset, int *mv_length, struct timeval *timestamp, 
                        unsigned int *timejiffies, int *is_keyframe, int *NonRef)
{
    struct v_graph_info    *graph_info = (struct v_graph_info*) job->in_header->addr_va;
    unsigned int *drvinfo=(unsigned int *)(job->in_header->addr_va+graph_info->drvinfo_offset);
    unsigned int tag_fd;
    
    tag_fd=*drvinfo;
    
    if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_H264_FD_MINOR_ID)
    {
        EncoderOut_info *encout_info=(EncoderOut_info *)drvinfo;
        *bs_offset = encout_info->BitstreamOffset;
        *bs_length = encout_info->BitstreamSize;
        *mv_offset = encout_info->MbInfoOffset;
        *mv_length = encout_info->MbInfoLength;
        *is_keyframe = encout_info->Keyframe;
        *timestamp = graph_info->first_tv;
        *timejiffies = graph_info->first_jiffies;
        *NonRef = encout_info->NonRef;
    }
    else if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_MJPEG_FD_MINOR_ID)
    {
        mje_OutInfo *encout_info=(mje_OutInfo *)drvinfo;
        *bs_offset = encout_info->bs_offset;
        *bs_length = encout_info->bs_length;
        *mv_offset = 0;
        *mv_length = 0;
        *is_keyframe = 1;
        *timestamp = graph_info->first_tv;
        *timejiffies = graph_info->first_jiffies;
        *NonRef = 0;
    }
    else if((ENTITY_FD_MINOR(tag_fd)&ENTITY_MINOR_MASK)==ENC_MPEG4_FD_MINOR_ID)
    {
        mp4e_OutInfo *encout_info=(mp4e_OutInfo *)drvinfo;

        *bs_offset = encout_info->bs_offset;
        *bs_length = encout_info->bs_length;
        *mv_offset = 0;
        *mv_length = encout_info->bs_offset;
        *is_keyframe = encout_info->Keyframe;
        *timestamp = graph_info->first_tv;
        *timejiffies = graph_info->first_jiffies;
        *NonRef = 0;        
    }
    else
    {
        char error_msg[100];
        sprintf(error_msg,"%s:Not support tag_fd 0x%x job %d at 0x%x drvinfo 0x%x\n",
                __FUNCTION__,tag_fd,job->id,(int)job,(int)drvinfo);
        printm_with_panic(error_msg);
    }
    return 0;
}

//Sub-bitstream, main-bitstream
static int do_queue_sub_bs_get(dvr_enc_data *enc_data, dvr_enc_queue_get *data, int repd_num)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
    int bs_offset, bs_length, mv_offset, mv_length, is_keyframe, NonRef;
    struct timeval timestamp;
    unsigned int timejiffies;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d_%d] %s \n", enc_data->ch_num, repd_num, __FUNCTION__);
	ebt = &enc_data->ebt[repd_num];  

TRY_DATAOUT_GET_AGAIN:
    ret = idataout_get(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), &job, &swap);
    if(ret==0) {
        parse_encoder_job(enc_data->ch_num, job, &bs_offset, &bs_length, &mv_offset, &mv_length, &timestamp, &timejiffies, &is_keyframe, &NonRef);
        data->bs.offset = video_get_buffer_remap_offset(job->in_buf)+bs_offset;
        data->bs.length = bs_length;
        data->bs.timestamp = timestamp;
        data->bs.timejiffies = timejiffies;
        data->bs.is_keyframe = is_keyframe;
        data->bs.mv_offset = video_get_buffer_remap_offset(job->in_buf)+mv_offset;
        data->bs.mv_length = mv_length;
        data->bs.stream = enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num);
        data->bs.NonRef = NonRef;
        data->new_bs = 0;
        if(swap && enc_data->update_param_flag[repd_num]) {
            data->new_bs = 1;
            enc_data->update_param_flag[repd_num] = 0;
        }
	    	
        data->bs.p_job = (void*) job;

        // Suppose MV is put right after BS, if any change, must update the range for sync.
        dma_consistent_sync(enc_data->pa_start, (void __iomem *) enc_data->vma_start+data->bs.mv_offset, PAGE_ALIGN(data->bs.offset-data->bs.mv_offset+data->bs.length), DMA_FROM_DEVICE);
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }
    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d]: DUPQUEUE GET, job(%d) offset(0x%x) length(%d)\n", enc_data->ch_num, job->id, data->bs.offset, data->bs.length);
    return 0;
}


//Sub-bitstream, main-bitstream
static int do_queue_sub_bs_get_copy(dvr_enc_data *enc_data, dvr_enc_copy_buf *t_data,int repd_num)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
    int bs_offset, bs_length, mv_offset, mv_length, is_keyframe, NonRef;
    struct timeval timestamp;
    unsigned int timejiffies;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d_%d] %s \n", enc_data->ch_num, repd_num, __FUNCTION__);
	ebt = &enc_data->ebt[repd_num];

TRY_DATAOUT_GET_AGAIN:
    ret = idataout_get(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), &job, &swap);
    if(ret==0) {
        parse_encoder_job(enc_data->ch_num, job, &bs_offset, &bs_length, &mv_offset,
            &mv_length, &timestamp, &timejiffies, &is_keyframe, &NonRef);
        if(t_data->bs_user_va) {
            if(t_data->bs_buf_length<bs_length) {
                printk("##### Error to get bitstream size %d (buffer too small 0x%x)\n",bs_length,
                    t_data->bs_buf_length);
                return -1;
            }
            t_data->timestamp=timestamp;
            t_data->timejiffies=timejiffies;
            t_data->NonRef=NonRef;
            t_data->stream=enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num);
            t_data->is_keyframe=is_keyframe;
            t_data->bs_length=bs_length;

            if(copy_to_user((void *)t_data->bs_user_va,
                (void *)(job->in_buf->addr_va+bs_offset),bs_length))
                return -1;

            t_data->new_bs = 0;
            if(swap && enc_data->update_param_flag[repd_num]) {
                t_data->new_bs = 1;
                enc_data->update_param_flag[repd_num] = 0;
            }
        }

        if(t_data->mv_user_va) {
            if(t_data->mv_buf_length<mv_length) {
                printk("##### Error to get MV size %d (buffer too small 0x%x)\n",mv_length,
                    t_data->mv_length);
                return -1;
            }
            t_data->mv_length=mv_length;
            if(copy_to_user((void *)t_data->mv_user_va,
                (void *)(job->in_buf->addr_va+mv_offset),mv_length))
                return -1;
        }
        
        t_data->p_job=(void *)job;
        idataout_put(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), job);
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }
    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d]: DUPQUEUE GET_COPY, job(%d) length(%d)\n",
        enc_data->ch_num, job->id, t_data->bs_length);
    return 0;
}


//-------------------------------------------------------------------------
static void get_bs(struct v_job_t * job, unsigned int * bs_off, int * bs_sz){
	struct v_graph_info * inb_hdr = (struct v_graph_info*)job->in_header->addr_va;
	mje_OutInfo * enc_in_info = (mje_OutInfo*)(job->in_header->addr_va + inb_hdr->drvinfo_offset);
	*bs_off = enc_in_info->bs_offset;
	*bs_sz = enc_in_info->bs_length;
} 

static int do_queue_ss_get(dvr_enc_data *enc_data, dvr_enc_queue_get *data)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
    int bs_off, bs_length;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d]\n", __FUNCTION__, enc_data->ch_num);
	ebt = &enc_data->ebt[0];  

TRY_DATAOUT_GET_AGAIN:

    ret = idataout_get(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM, &job, &swap);
    if(ret==0) {
        get_bs(job, &bs_off, &bs_length);
        data->bs.offset = video_get_buffer_remap_offset(job->in_buf)+bs_off;
        data->bs.length = bs_length;
        data->bs.p_job = (void*) job;
        data->bs.stream= enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM;
        dma_consistent_sync(enc_data->pa_start, (void __iomem *) enc_data->vma_start+data->bs.offset, PAGE_ALIGN(data->bs.length), DMA_FROM_DEVICE);
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d]: SSQUEUE GET, job(%d) offset(0x%x) length(%d)\n", enc_data->ch_num, job->id, data->bs.offset, data->bs.length);
    return 0;
}

static int do_queue_ss_get_copy(dvr_enc_data *enc_data, dvr_enc_copy_buf *t_data)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
    int bs_off, bs_length;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d]\n", __FUNCTION__, enc_data->ch_num);
	ebt = &enc_data->ebt[0];  

TRY_DATAOUT_GET_AGAIN:

    ret = idataout_get(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM, &job, &swap);
    if(ret==0) {
#if 1
        get_bs(job, &bs_off, &bs_length);
        if(t_data->bs_user_va) {
            if(t_data->bs_buf_length<bs_length) {
                printk("##### Error to get snapshot bitstream size %d (buffer too small 0x%x)\n",bs_length,
                    t_data->bs_buf_length);
                return -1;
            }
            //t_data->timestamp=timestamp;
            //t_data->NonRef=NonRef;
            t_data->stream=enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM;
            t_data->is_keyframe=1;
            t_data->bs_length=bs_length;
            t_data->mv_length=0;
        }
        if(copy_to_user((void *)t_data->bs_user_va, (void *)(job->in_buf->addr_va+bs_off),bs_length))
            return -1;

        t_data->new_bs = 0;
        t_data->p_job=(void *)job;
        idataout_put(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM, job);
#else
        get_bs(job, &bs_off, &bs_length);
        data->bs.offset = video_get_buffer_remap_offset(job->in_buf)+bs_off;
        data->bs.length = bs_length;
        data->bs.p_job = (void*) job;
        data->bs.stream= enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM;
        dma_consistent_sync(enc_data->pa_start, (void __iomem *) enc_data->vma_start+data->bs.offset, PAGE_ALIGN(data->bs.length), DMA_FROM_DEVICE);
#endif
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d]: SSQUEUE GET_COPY, job(%d) length(%d)\n", enc_data->ch_num, job->id, t_data->bs_length);
    return 0;
}

static int do_queue_sub_rawbs_get(dvr_enc_data *enc_data, dvr_enc_queue_get *data, int repd_num)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
	ReproduceBitStream *repd;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d_%d]\n", __FUNCTION__, enc_data->ch_num, repd_num);
	ebt = &enc_data->ebt[repd_num];  
	repd = &enc_data->repd[repd_num];

TRY_DATAOUT_GET_AGAIN:
    ret = idataout_get(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), &job, &swap);
    if(ret==0) {
        data->bs.offset = video_get_buffer_remap_offset(job->in_buf);
        data->bs.length = repd->dim.width * repd->dim.height * 2;
        data->bs.is_keyframe = 1;
        data->bs.mv_offset = 0;
        data->bs.mv_length = 0;
        data->bs.stream = enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num);
        data->bs.NonRef = 0;
        data->new_bs = 0;
        if(swap && enc_data->update_param_flag[repd_num]) {
            data->new_bs = 1;
            enc_data->update_param_flag[repd_num] = 0;
        }
	    	
        data->bs.p_job = (void*) job;

        // Suppose MV is put right after BS, if any change, must update the range for sync.
        dma_consistent_sync(enc_data->pa_start, (void __iomem *) enc_data->vma_start+data->bs.mv_offset, PAGE_ALIGN(data->bs.offset-data->bs.mv_offset+data->bs.length), DMA_FROM_DEVICE);
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }
    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d]: DUPQUEUE GET, job(%d) offset(0x%x) length(%d)\n", __FUNCTION__, enc_data->ch_num, job->id, data->bs.offset, data->bs.length);
    return 0;
}

static int do_queue_sub_rawbs_get_copy(dvr_enc_data *enc_data, dvr_enc_copy_buf *t_data,int repd_num)
{
    int ret, swap;
    struct v_job_t *job;
    dvr_enc_bt		*ebt;	
	ReproduceBitStream *repd;
	unsigned int bs_length;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d_%d]\n", __FUNCTION__, enc_data->ch_num, repd_num);
	ebt = &enc_data->ebt[repd_num];  
	repd = &enc_data->repd[repd_num];

TRY_DATAOUT_GET_AGAIN:
    ret = idataout_get(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), &job, &swap);
    if(ret==0) {
#if 1
        if(t_data->bs_user_va) {
            bs_length = repd->dim.width * repd->dim.height * 2;
            if(t_data->bs_buf_length<bs_length) {
                printk("##### Error to get bitstream size %d (buffer too small 0x%x)\n",bs_length,
                    t_data->bs_buf_length);
                return -1;
            }
            //t_data->timestamp=timestamp;
            t_data->NonRef=0;
            t_data->stream=enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num);
            t_data->is_keyframe=1;
            t_data->bs_length=bs_length;
            t_data->mv_length=0;

            if(copy_to_user((void *)t_data->bs_user_va,
                (void *)(job->in_buf->addr_va),bs_length))
                return -1;

            t_data->new_bs = 0;
            if(swap && enc_data->update_param_flag[repd_num]) {
                t_data->new_bs = 1;
                enc_data->update_param_flag[repd_num] = 0;
            }
        }
        t_data->p_job=(void *)job;
        idataout_put(enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num), job);

#else
        data->bs.offset = video_get_buffer_remap_offset(job->in_buf);
        data->bs.length = repd->dim.width * repd->dim.height * 2;
        data->bs.is_keyframe = 1;
        data->bs.mv_offset = 0;
        data->bs.mv_length = 0;
        data->bs.stream = enc_data->ch_num+(DVR_RECORD_CHANNEL_NUM*repd_num);
        data->bs.NonRef = 0;
        data->new_bs = 0;
        if(swap && enc_data->update_param_flag[repd_num]) {
            data->new_bs = 1;
            enc_data->update_param_flag[repd_num] = 0;
        }
	    	
        data->bs.p_job = (void*) job;

        // Suppose MV is put right after BS, if any change, must update the range for sync.
        dma_consistent_sync(enc_data->pa_start, (void __iomem *) enc_data->vma_start+data->bs.mv_offset, PAGE_ALIGN(data->bs.offset-data->bs.mv_offset+data->bs.length), DMA_FROM_DEVICE);
#endif
    }
    else if(ebt->is_blocked && ret==-2) {
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(1);
        goto TRY_DATAOUT_GET_AGAIN;
    }
    else {
        return -1;
    }
    VDBGPRINT(DBG_DVR_DATA_FLOW, "[%s:%d]: DUPQUEUE GET, job(%d) length(%d)\n", __FUNCTION__, enc_data->ch_num, job->id, t_data->bs_length);
    return 0;
}

static int do_queue_bs_get(dvr_enc_data *enc_data, dvr_enc_queue_get *data, int repd_num)
{
    int ret=0;
	ReproduceBitStream *repd;
	
	repd = &enc_data->repd[repd_num];
    switch(repd->enc_type) {
    	case ENC_TYPE_MPEG:
    	case ENC_TYPE_MJPEG:
    	case ENC_TYPE_H264:
    	    ret = do_queue_sub_bs_get(enc_data, data, repd_num);
    		break;
    	case ENC_TYPE_YUV422:
        	ret = do_queue_sub_rawbs_get(enc_data, data, repd_num);
    		break;
    	default: 
            panic("%s: enc_type %d failed! (ch%d_%d)\n", __FUNCTION__, repd->enc_type, enc_data->ch_num, repd_num);   		
            break;
    }
    return ret;
}


static int do_queue_bs_get_copy(dvr_enc_data *enc_data, dvr_enc_copy_buf *t_data, int repd_num)
{
    int ret=0;
	ReproduceBitStream *repd;
	
	repd = &enc_data->repd[repd_num];
    switch(repd->enc_type) {
    	case ENC_TYPE_MPEG:
    	case ENC_TYPE_MJPEG:
    	case ENC_TYPE_H264:
    	    ret = do_queue_sub_bs_get_copy(enc_data, t_data, repd_num);
    		break;
    	case ENC_TYPE_YUV422:
        	ret = do_queue_sub_rawbs_get_copy(enc_data, t_data, repd_num);
    		break;
    	default: 
            panic("%s: enc_type %d failed! (ch%d_%d)\n", __FUNCTION__, repd->enc_type, enc_data->ch_num, repd_num);   		
            break;
    }
    return ret;
}


static int do_snapshot_raw(dvr_enc_data *enc_data, int stream, int snapshot_cnt)
{
    dvr_enc_bt		*ebt;	

	ebt = &enc_data->ebt[stream];  
    if(ebt->enc_type!= ENC_TYPE_YUV422)
        panic("%s: failed! (channel=%d_%d, enc_type=%d)\n", __FUNCTION__, enc_data->ch_num, stream, ebt->enc_type);
        
	if (set_property(ebt->scl_fd, PROPID(ebt->scl_fd, "snapshot"), snapshot_cnt, 1) <0) {
		printk("set_property error\n");
		return -1;
	}
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <snapshot_cnt=%d>\n",__FUNCTION__,__LINE__, snapshot_cnt);
    return 0;
}

static int do_snapshot_jpg(dvr_enc_data *enc_data, int snapshot_cnt)
{
	int ss_enc_fd, scl_ss_fd;
    dvr_enc_bt		*ebt;	
	
	ebt = &enc_data->ebt[0];  // main-bitstream
	ss_enc_fd=ebt->ss_enc_fd;
	scl_ss_fd=ebt->scl_ss_fd;
	if (ss_enc_fd == 0)
		return -1;
	down(&dvr_enc_snapshot_sem);
	if (set_property(scl_ss_fd, PROPID(scl_ss_fd, "snapshot"), snapshot_cnt, 1) <0) {
		printk("set_property error\n");
		return -1;
	}
	if (set_property(ss_enc_fd, PROPID(ss_enc_fd, "VGID_MJE_SS_count"), snapshot_cnt, 1) <0) {
		printk("set_property error\n");
		return -1;
	}
    VDBGPRINT(DBG_DVR_FNC, "%s:%d <snapshot_cnt=%d>\n",__FUNCTION__,__LINE__, snapshot_cnt);
	up(&dvr_enc_snapshot_sem);
    return 0;
}


static int do_queue_put(dvr_enc_data *enc_data, dvr_enc_queue_get *data)
{
    int ret;

    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d] %s \n", enc_data->ch_num, __FUNCTION__);
    if(data->bs.p_job == NULL)
        return -1;
    
    ret = idataout_put(data->bs.stream, (struct v_job_t*)data->bs.p_job);
    VDBGPRINT(DBG_DVR_DATA_FLOW, "[dvr_enc:%d]: QUEUE PUT, job(%d) ret(%d)\n", enc_data->ch_num, ((struct v_job_t*)data->bs.p_job)->id, ret);

    return ret;
}

int video_register_preprocess(int entity_fd,int (*func)(void *,void *));

static int start_enc_graph(dvr_enc_data *enc_data, int stream)
{
    int ret;

    VDBGPRINT(DBG_DVR_FNC, "[%s:%d_%d] -- IN\n", __FUNCTION__, enc_data->ch_num, stream);
    if(stream >= DVR_ENC_REPD_BT_NUM) {
        printk("Error to %s stream=%d\n",__FUNCTION__, stream);
        return -EFAULT;
    }
    ret = do_setup_encoder(enc_data, stream);
    if(ret<0) {
        printk("Error to do_setup_encoder\n");
        return -EFAULT;
    }
    ret = do_set_channel_param(enc_data, stream);
    if(ret<0) {
        printk("Error to do_set_channel_param\n");
        return -EFAULT;
    }
    set_enc_graph(enc_data, stream);
    VDBGPRINT(DBG_DVR_FNC, "[%s:%d_%d] -- OUT\n", __FUNCTION__, enc_data->ch_num, stream);
    return 0;
}

int set_enc_intra_from_dataout(int dataout_fd,int intra)
{
    int i,j,id=0;
    dvr_enc_data    *enc_data;
    dvr_enc_bt	    *ebt=0;
    
    for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++)
    {
        if(enc_private_data[i].is_used)
        {
            id=i;
            enc_data = &enc_private_data[i];
	        for(j=0;j<DVR_ENC_REPD_BT_NUM; j++)
	        {
	            ebt = &enc_data->ebt[j];
        		if(ebt->enabled!=DVR_ENC_EBST_ENABLE)
        		    continue;
                if(dataout_fd==ebt->dataout_fd)
                {
                    if((ENTITY_FD_MINOR(ebt->enc_fd)&ENTITY_MINOR_MASK)==ENC_H264_FD_MINOR_ID)
                    {
                        p200_h264e_set_intra(ebt->enc_fd&0xff,intra);
                        return 1;
                    }
                    else if((ENTITY_FD_MINOR(ebt->enc_fd)&ENTITY_MINOR_MASK)==ENC_MPEG4_FD_MINOR_ID)
                    {
                        mpeg4_set_intra(ebt->enc_fd&0xff,intra);
                        return 1;
                    }
                    return 0;
                } 
				else if(dataout_fd==ebt->ss_dataout_fd) 
                    return 0;
            }
        }
    }
#if 0
    printk("Error to fd 0x%x set intra!\n",dataout_fd);
    enc_data = &enc_private_data[id];
    for(j=0;j<DVR_ENC_REPD_BT_NUM; j++)
    {
        ebt = &enc_data->ebt[j];
        printk("CH%d (%d)Enable %d for fd 0x%x\n",id,j,ebt->enabled,ebt->dataout_fd);
    }
#endif
    return 0;
}

static int stop_enc_graph(dvr_enc_data *enc_data, int sub_enc_num)
{
    FuncTag tag, *ptag;
    int i, j;

    VDBGPRINT(DBG_DVR_FNC, "[%s:%d_%d] -- IN\n", __FUNCTION__, enc_data->ch_num, sub_enc_num);
	ptag = &tag;
    if (sub_enc_num == 0) {
		for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
		    if(enc_data->ebt[i].enabled!=DVR_ENC_EBST_ENABLE)
		        continue;
            DV_RESET_TAG(ptag);
            DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[i], enc_data->ch_num);
        	dvr_graph_del_link(enc_data->graph, ptag);
	        idataout_setactive(enc_data->ch_num+ DVR_RECORD_CHANNEL_NUM * i, FALSE);
        	switch(enc_data->repd[i].enc_type) {
        		case ENC_TYPE_MPEG:
            		free_mpeg4_rc(enc_data->ebt[i].enc_fd&0xff);
            		break;
        		case ENC_TYPE_H264:
                	Free_RC(enc_data->ebt[i].enc_fd&0xff);
                	break;
        	}
        	enc_data->ebt[i].enabled = DVR_ENC_EBST_DISABLE;
        	enc_data->repd[i].enabled = DVR_ENC_EBST_DISABLE;
            for(j=0; j<DVR_ENC_SCALAR_NUM; j++) {
                enc_data->enc_scl[j].enabled = 0;
            }
		}
    
    } else {
	    if(enc_data->ebt[sub_enc_num].enabled!=DVR_ENC_EBST_ENABLE) 
            return 0;
        DV_RESET_TAG(ptag);
        DV_SET_REC_CH(ptag, dvr_enc_fun_rec_table[sub_enc_num], enc_data->ch_num);
    	dvr_graph_del_link(enc_data->graph, ptag);
    	idataout_setactive(enc_data->ch_num+ DVR_RECORD_CHANNEL_NUM * sub_enc_num, FALSE);
    	switch(enc_data->repd[sub_enc_num].enc_type) {
    		case ENC_TYPE_MPEG:
        		free_mpeg4_rc(enc_data->ebt[sub_enc_num].enc_fd&0xff);
        		break;
    		case ENC_TYPE_H264:
            	Free_RC(enc_data->ebt[sub_enc_num].enc_fd&0xff);
            	break;
    	}
    	enc_data->ebt[sub_enc_num].enabled = DVR_ENC_EBST_DISABLE;
    	enc_data->repd[sub_enc_num].enabled = DVR_ENC_EBST_DISABLE;
    }
    VDBGPRINT(DBG_DVR_FNC, "[%s:%d_%d] -- OUT\n", __FUNCTION__, enc_data->ch_num, sub_enc_num);
    return 0;
}

static int check_channel_param_data(dvr_enc_data *enc_data, dvr_enc_channel_param *ch_param)
{

    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:entry(%x)] %s -- IN\n", (int)enc_data, __FUNCTION__);

    if(ch_param->src.channel<0 || ch_param->src.channel>=DVR_RECORD_CHANNEL_NUM)
    {
        printk("Invalid channel number(%d)!\n", ch_param->src.channel);
        return -1;
    }
    if(ch_param->src.enc_src_type<0 || ch_param->src.enc_src_type>=ENC_SRC_TYPE_COUNT) {
        printk("Invalid source type(%d)!\n", ch_param->src.enc_src_type);
        return -1;
    }
	
	if(ch_param->main_bs.out_bs != 0){
		printk("Invalid out bitstream type(%d)!, it must be 0 (main-bitstream)\n", ch_param->main_bs.out_bs);
		return -1;
	}

	if(ch_param->main_bs.enabled != DVR_ENC_EBST_ENABLE){
		printk("Invalid enabled value(%d)!, \n", ch_param->main_bs.out_bs);
		return -1;
	}

	if(ch_param->main_bs.enc_type<0 || ch_param->main_bs.enc_type>=ENC_TYPE_COUNT){
		printk("Invalid encode type(%d)!\n", ch_param->main_bs.enc_type);
		return -1;
	}

    enc_data->ch_num = ch_param->src.channel;
    memcpy(&enc_data->src, &ch_param->src, sizeof(dvr_enc_src_param));
    memcpy(&enc_data->repd[0], &ch_param->main_bs, sizeof(ReproduceBitStream));  // 0:main-bitstream
    memset(&enc_data->enc_ext[0], 0, sizeof(enc_data->enc_ext[0]));
    if(DVR_ENC_CHECK_MAGIC(ch_param->main_bs.enc.ext_size) && DVR_ENC_GET_VALUE(ch_param->main_bs.enc.ext_size)  && ch_param->main_bs.enc.pext_data){
        if(DVR_ENC_GET_VALUE(ch_param->main_bs.enc.ext_size) <= sizeof(ENC_EXT_STRUCT)){
            if(copy_from_user((void *)&enc_data->enc_ext[0], (void *)ch_param->main_bs.enc.pext_data, DVR_ENC_GET_VALUE(ch_param->main_bs.enc.ext_size))) {
				printk("check_channel_param_data: copy_from_user failed!\n");
				return -1;
			}
        }
        else
            printk("check_channel_param_data: EncParam_Ext struct version is not correct!struct size=(ap=%d,drv=%d)\n", DVR_ENC_GET_VALUE(ch_param->main_bs.enc.ext_size), sizeof(ENC_EXT_STRUCT));    
    }

    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:entry(%x)] %s -- OUT\n", (int)enc_data, __FUNCTION__);
    return 0;
}

static int check_sub_bitstream_param_data(dvr_enc_data * enc_data, ReproduceBitStream *bs)
{
    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:(out_bs%d)] %s -- IN\n", bs->out_bs, __FUNCTION__);
	if(bs->out_bs < 0 || bs->out_bs > (DVR_ENC_REPD_BT_NUM-1)){
		printk("Invalid out bitstream type(%d)!\n", bs->out_bs);
		return -1;
	}
	if(bs->out_bs >=1){
		if(enc_data->repd[0].enabled != DVR_ENC_EBST_ENABLE){
			printk("Please init main-bitstream first! out_bs=%d\n", bs->out_bs);
			return -1;
		}
	}
	if(bs->enc_type<0 || bs->enc_type>=ENC_TYPE_COUNT){
		printk("Invalid encode type(%d)!\n", bs->enc_type);
		return -1;
	}

    memcpy(&enc_data->repd[bs->out_bs], bs, sizeof(ReproduceBitStream));
    memset(&enc_data->enc_ext[bs->out_bs], 0, sizeof(enc_data->enc_ext[bs->out_bs]));
    if(DVR_ENC_CHECK_MAGIC(bs->enc.ext_size) && DVR_ENC_GET_VALUE(bs->enc.ext_size)  && bs->enc.pext_data){
        if(DVR_ENC_GET_VALUE(bs->enc.ext_size) <= sizeof(ENC_EXT_STRUCT)){
            if(copy_from_user((void *)&enc_data->enc_ext[bs->out_bs], (void *)bs->enc.pext_data, DVR_ENC_GET_VALUE(bs->enc.ext_size))){
            	printk("check_sub_bitstream_param_data: copy_from_user failed!\n");    
				return -1;
			}
        }
        else
            printk("check_sub_bitstream_param_data: EncParam_Ext struct version is not correct!struct size=(ap=%d,drv=%d)\n", DVR_ENC_GET_VALUE(bs->enc.ext_size), sizeof(ENC_EXT_STRUCT));    
    }

    //VDBGPRINT(DBG_DVR_FNC, "[dvr_enc:(out_bs%d)] %s -- OUT\n", bs->out_bs, __FUNCTION__);
    return 0;
}

static void enc_data_init(void)
{
    dvr_enc_data *enc_data=NULL;
    dvr_enc_bt	 *ebt;	
    int i,j;

    for(j=0; j<DVR_RECORD_CHANNEL_NUM; j++) {
        enc_data = &enc_private_data[i];
        enc_data->is_used = FALSE;
        enc_data->ch_num = -1;
        enc_data->cap_fd[0] = 0;
        enc_data->cap_fd[1] = -1;;
        enc_data->cap_fd[2] = -1;;
        enc_data->DI_fd[0] = 0;
        enc_data->DI_fd[1] = -1;
        enc_data->DI_fd[2] = -1;
        enc_data->is_sub_denoise = 1;
        enc_data->fps = 0;
        enc_data->src_type = 0;
        enc_data->is_link_3DI = 0;
    	for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
    		ebt = &enc_data->ebt[i];  
    		ebt->enabled = DVR_ENC_EBST_DISABLE;
    		ebt->enc_type = 0;
    		ebt->enc_fd = 0; 
    		ebt->scl_fd = 0;
    		ebt->enc_fps = 0;
    		ebt->is_blocked = 0;
    		ebt->en_snapshot = DVR_ENC_EBST_DISABLE;
    		ebt->en_scl = 0;
    		ebt->dataout_fd = 0;
    		ebt->ss_enc_fd = 0;
    		ebt->scl_ss_fd = 0;
    		ebt->ss_dataout_fd = 0;
            ebt->wait_queue_stream = NULL;
            ebt->wait_queue_snapshot = NULL;
            ebt->old_timestamp.tv_sec=ebt->old_timestamp.tv_usec=0;
    	}
    }
}

int dvr_enc_open(struct inode *inode, struct file *filp)
{
    filp->private_data = NULL;
    return 0;
}

int dvr_enc_release(struct inode *inode, struct file *filp)
{
    int i;
    dvr_enc_data  *enc_data = filp->private_data;

    VDBGPRINT(DBG_DVR_FNC, "[%s:%d]-- IN\n", __FUNCTION__, enc_data->ch_num);
    if(enc_data == NULL) return 0;

    down(&enc_data->dvr_enc_sem);

    // return dataout!
    for(i=0; i<DVR_ENC_REPD_BT_NUM; i++)
    {
        if(enc_data->ebt[i].enabled==DVR_ENC_EBST_ENABLE)
        {
            idataout_reset(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*i);
            idataout_close(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*i);
        }
    }
    if(enc_data->ebt[0].en_snapshot==DVR_ENC_EBST_ENABLE)
    {
        idataout_reset(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM);
        idataout_close(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM);
    }

    if(enc_data->graph)
    {
        dvr_graph_destroy(enc_data->graph);
        enc_data->graph = NULL;
    }

    enc_data->is_used = FALSE;

    up(&enc_data->dvr_enc_sem);

    VDBGPRINT(DBG_DVR_FNC, "[%s:%d]-- OUT\n", __FUNCTION__, enc_data->ch_num);

    return 0;
}

int dvr_enc_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long size=0;
    dvr_enc_data  *enc_data = filp->private_data;
    dvr_enc_bt *ebt;	
	int ret = 0, stream;
    struct v_queue_t    *que;
    struct v_resource_t *res;

    VDBGPRINT(DBG_DVR_FNC, "[%s:%d] -- IN\n", __FUNCTION__, enc_data->ch_num);
	if (!(vma->vm_flags & VM_WRITE)) {
		printk("dvr_enc_mmap: please use PROT_WRITE in mmap()\n");
		ret = -EINVAL;
		goto EXIT_DVR_ENC_MMAP;
	}
	if (!(vma->vm_flags & VM_SHARED)) {
		printk("dvr_enc_mmap: please use MAP_SHARED in mmap()\n");
		ret = -EINVAL;
		goto EXIT_DVR_ENC_MMAP;
	}

	/* memory policy: cacheable and bufferable */
	//vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

    enc_data->vma_start = vma->vm_start;
	ebt=&enc_data->ebt[0];
	que= ebt->enc_out_qst.que;
    if(que->qshare)
        res=&que->qshare->res;
    else
        res=&que->res;
    enc_data->pa_start = res->start_pa[0];

    size = 0;
    for(stream=0; stream<DVR_ENC_REPD_BT_NUM; stream++) {
    	ebt=&enc_data->ebt[stream];
        if(ebt->enc_out_qst.que)
            size+=video_remap_pfn_range(ebt->enc_out_qst.que,vma,vma->vm_start+size,vma->vm_page_prot);
    }
    /* for snapshot queue */
    ebt = &enc_data->ebt[0];
    if(ebt->enc_out_qst_snap.que)
        size+=video_remap_pfn_range(ebt->enc_out_qst_snap.que,vma,vma->vm_start+size,vma->vm_page_prot);

EXIT_DVR_ENC_MMAP:
    
    VDBGPRINT(DBG_DVR_FNC, "[%s:%d] -- OUT\n", __FUNCTION__, enc_data->ch_num);

	return ret;
}

static unsigned int dvr_enc_poll(struct file *filp, poll_table *wait)
{
    int i;
    unsigned int mask = 0;
    dvr_enc_bt *ebt;
    dvr_enc_data  *enc_data = filp->private_data;

	/* bitstream */
	for(i=0; i<DVR_ENC_REPD_BT_NUM; i++) {
	    ebt = &enc_data->ebt[i];

		if(ebt->enabled!=DVR_ENC_EBST_ENABLE)
		        continue;
        poll_wait(filp, ebt->wait_queue_stream, wait);
        if( idataout_getlength(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*i)) {
        	/* (i=0, mask=POLLIN_MAIN_BS), (i=1, mask=POLLIN_SUB1_BS), (i=2, mask=POLLIN_SUB2_BS), ... */
            mask |= (POLLIN_MAIN_BS << i);
        }
	}
	/* snapshot of main-bitstream */
    ebt = &enc_data->ebt[0];
    if(ebt->enabled ==DVR_ENC_EBST_ENABLE && ebt->en_snapshot==DVR_ENC_EBST_ENABLE) {
        poll_wait(filp, ebt->wait_queue_snapshot, wait);
	    if(idataout_getlength(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM)) 
			mask |= POLLIN_SNAP_BS;
	}
	return mask;
}

static int dvr_enc_query_output_buffer_size(dvr_enc_data *enc_data, int stream)
{
    int   buf_size, i, j;
    dvr_enc_bt *ebt;	

    if(stream == 0) 
        j=DVR_ENC_REPD_BT_NUM; 
    else 
        j=stream; 
        
    buf_size = 0;
    for(i=0; i<j; i++) {
        ebt = &enc_data->ebt[i];  
        if(ebt->enc_out_qst.que)
            buf_size +=	PAGE_ALIGN(video_buf_arrange_size(ebt->enc_out_qst.que));
    }
    return buf_size;
}

static int dvr_enc_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret=-1, i, val;
    dvr_enc_data  *enc_data = filp->private_data;
    dvr_enc_bt *ebt;	
    dvr_info_buf_flow_info  buf_flow_info;

    if (enc_data == NULL) {
        if(cmd != DVR_ENC_SET_CHANNEL_PARAM) {
            printk("%s: Channel data does not init, please set DVR_ENC_SET_CHANNEL_PARAM first.\n",__FUNCTION__);
            return -EFAULT;
        } else { 
    		dvr_enc_channel_param ch_param;
    		if(copy_from_user((void *)&ch_param, (void *)arg, sizeof(dvr_enc_channel_param))) 
                return -EFAULT;
            enc_data = &enc_private_data[ch_param.src.channel];
            VDBGPRINT(DBG_DVR_FNC, "%s:%d <init enc_data[%d].>\n",__FUNCTION__,__LINE__,ch_param.src.channel);
            enc_data->is_used = TRUE;
	        filp->private_data = enc_data;
            memcpy(&enc_data->la_ch_param_set, &ch_param, sizeof(dvr_enc_channel_param));
            ret = check_channel_param_data(enc_data, &ch_param);
    		if(ret>=0) {
                if(copy_to_user((void *)arg,(void *)&ch_param,sizeof(dvr_enc_channel_param))) 
                    return -EFAULT;
            }
    		return ret;
        }
    }

    down(&enc_data->dvr_enc_sem);
    switch(cmd)
    {
      case DVR_ENC_QUERY_OUTPUT_BUFFER_SIZE:	//output buffer for h264 enc & mjpeg enc
      {
        int   buf_size;
        
		if(copy_from_user((void *)&buf_size, (void *)arg, sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        buf_size = dvr_enc_query_output_buffer_size(enc_data, 0);
        ebt = &enc_data->ebt[0]; // main bitStream Snapshot 
		if(ebt->enc_out_qst_snap.que)
		    buf_size +=	PAGE_ALIGN(video_buf_arrange_size(ebt->enc_out_qst_snap.que));
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_buf_size, &buf_size, sizeof(int));
        break;
      }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB1_BS_OFFSET:	//output buffer offset for Sub1-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 1);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_sub1_buf_size, &buf_size, sizeof(int));
        break;
      }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB2_BS_OFFSET:	//output buffer offset for Sub2-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 2);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_sub2_buf_size, &buf_size, sizeof(int));
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB3_BS_OFFSET:	//output buffer offset for Sub4-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 3);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB4_BS_OFFSET:	//output buffer offset for Sub4-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 4);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB5_BS_OFFSET:	//output buffer offset for Sub5-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 5);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB6_BS_OFFSET:	//output buffer offset for Sub6-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 6);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB7_BS_OFFSET:	//output buffer offset for Sub7-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 7);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SUB8_BS_OFFSET:	//output buffer offset for Sub8-bitstream
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 8);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        break;
    }
	case DVR_ENC_QUERY_OUTPUT_BUFFER_SNAP_OFFSET:	//output buffer offset  for mjpeg enc
	{
		int 	buf_size;

        buf_size = dvr_enc_query_output_buffer_size(enc_data, 0);
        ret = 0;
        if(copy_to_user((void *)arg,(void *)&buf_size,sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_ss_buf_size, &buf_size, sizeof(int));
        break;
      }
      case DVR_ENC_GET_CHANNEL_PARAM:
      {
        dvr_enc_channel_param   ch_param;
		if(copy_from_user((void *)&ch_param, (void *)arg, sizeof(dvr_enc_channel_param))) {
			ret = -EFAULT;
			goto exit1;
		}
        ret = do_get_channel_param(enc_data, &ch_param);
		if(ret>=0)
		{
            if(copy_to_user((void *)arg,(void *)&ch_param,sizeof(dvr_enc_channel_param))) {
				ret = -EFAULT;
				goto exit1;
			}
            memcpy(&enc_data->la_ch_param_get, &ch_param, sizeof(dvr_enc_channel_param));
        }
        break;
      }
      case DVR_ENC_SET_CHANNEL_PARAM:
      {
		dvr_enc_channel_param ch_param;
		if(copy_from_user((void *)&ch_param, (void *)arg, sizeof(dvr_enc_channel_param))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_ch_param_set, &ch_param, sizeof(dvr_enc_channel_param));
        ret = check_channel_param_data(enc_data, &ch_param);
		if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&ch_param,sizeof(dvr_enc_channel_param))) {
				ret = -EFAULT;
				goto exit1;
			}
		}
        break;
      }
      case DVR_ENC_SET_SUB_BS_PARAM:
      {
		ReproduceBitStream sub_bs;

		if(copy_from_user((void *)&sub_bs, (void *)arg, sizeof(ReproduceBitStream))){
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_sub_bs, &sub_bs, sizeof(ReproduceBitStream));
        ret = check_sub_bitstream_param_data(enc_data, &sub_bs);
		if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&sub_bs,sizeof(ReproduceBitStream))) {
				ret = -EFAULT;
				goto exit1;
			}
		}
        break;
      }

    case DVR_ENC_QUEUE_GET_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 0);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }
      case DVR_ENC_QUEUE_GET:
      {
        dvr_enc_queue_get   data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 0);
		if(ret>=0)
		{
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
#if DEBUG_ENC_MAIN
            struct v_graph_info *graph_info = (struct v_graph_info*)job->in_header->addr_va;
            unsigned int *drvinfo=(unsigned int *)(job->in_header->addr_va+graph_info->drvinfo_offset);

            printm("##### DVR_ENC_QUEUE_GET CH%d for %s job %d (stream %d)\n",
                enc_data->ch_num,job->entity->name,job->id,data.bs.stream);

            data.mb_len=drvinfo[4]; //for debug H.264 only
            data.channel=enc_data->ch_num;
#endif
            if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}


#if LOG_API_QUEUE_FLOW
            memcpy(&enc_data->la_que_get, &data, sizeof(dvr_enc_queue_get));
#endif
            update_perf(&gperf_info[enc_data->ch_num], job->id, 0);
        }
        show_perf();
        break;
      }

    case DVR_ENC_QUEUE_GET_SUB1_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 1);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB1_BS:  // sub1-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 1);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
#if DEBUG_ENC_MAIN
            printm("##### DVR_ENC_QUEUE_GET_SUB1 CH%d for %s job %d (stream %d)\n",
                enc_data->ch_num,job->entity->name,job->id,data.bs.stream);
#endif
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}

#if LOG_API_QUEUE_FLOW
            memcpy(&enc_data->la_sub1_que_get, &data, sizeof(dvr_enc_queue_get));
#endif
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM+enc_data->ch_num], job->id, 1);
        }
        show_perf();

		break;
	}

    case DVR_ENC_QUEUE_GET_SUB2_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 2);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB2_BS:  // sub2-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 2);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
#if DEBUG_ENC_MAIN
            printm("##### DVR_ENC_QUEUE_GET_SUB2 CH%d for %s job %d (stream %d)\n",
                enc_data->ch_num,job->entity->name,job->id,data.bs.stream);
#endif
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}

#if LOG_API_QUEUE_FLOW
            memcpy(&enc_data->la_sub2_que_get, &data, sizeof(dvr_enc_queue_get));
#endif
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*2+enc_data->ch_num], job->id, 2);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB3_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 3);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB3_BS:  // sub3-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 3);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*3+enc_data->ch_num], job->id, 3);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB4_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 4);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB4_BS:  // sub4-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 4);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*4+enc_data->ch_num], job->id, 4);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB5_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 5);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB5_BS:  // sub5-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 5);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*5+enc_data->ch_num], job->id, 5);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB6_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 6);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB6_BS:  // sub6-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 6);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*6+enc_data->ch_num], job->id, 6);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB7_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 7);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB7_BS:  // sub7-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 7);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*7+enc_data->ch_num], job->id, 7);
        }
        show_perf();
		break;
	}

    case DVR_ENC_QUEUE_GET_SUB8_BS_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get_copy(enc_data, &t_data, 8);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }

	case DVR_ENC_QUEUE_GET_SUB8_BS:  // sub8-bitstream
	{
		dvr_enc_queue_get 	data;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_bs_get(enc_data, &data, 8);

		if(ret>=0)
        {
            struct v_job_t *job=(struct v_job_t *)data.bs.p_job;
			if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
            update_perf(&gperf_info[DVR_RECORD_CHANNEL_NUM*8+enc_data->ch_num], job->id, 8);
        }
        show_perf();
		break;
	}
	case DVR_ENC_QUEUE_GET_SNAP:
	{
		dvr_enc_queue_get 	data;
		if (copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_ss_get(enc_data, &data);

		if(ret>=0)
		{
			if (copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}

#if LOG_API_QUEUE_FLOW
            memcpy(&enc_data->la_ss_que_get, &data, sizeof(dvr_enc_queue_get));
#endif
        }
		break;
	}
    case DVR_ENC_QUEUE_GET_SNAP_COPY:
    {
        dvr_enc_copy_buf t_data; 
        if(copy_from_user((void *)&t_data, (void *)arg, sizeof(dvr_enc_copy_buf))) {
            ret = -EFAULT;
			goto exit1;
		}
		ret = do_queue_ss_get_copy(enc_data, &t_data);
		if(ret>=0) {
		    if(copy_to_user((void *)arg,(void *)&t_data,sizeof(dvr_enc_copy_buf))) {
		        ret=-EFAULT;
		        goto exit1;
		    }
		}
        break;
    }
    case DVR_ENC_QUEUE_PUT:
      {
        dvr_enc_queue_get   data;
        int str_num=0, job_id=0;
		if(copy_from_user((void *)&data, (void *)arg, sizeof(dvr_enc_queue_get))) {
			ret = -EFAULT;
			goto exit1;
		}
#if LOG_API_QUEUE_FLOW
        memcpy(&enc_data->la_que_put, &data, sizeof(dvr_enc_queue_get));
#endif
        job_id=((struct v_job_t *)data.bs.p_job)->id;
#if DEBUG_ENC_MAIN
        { //sanity check
            if(job==0)
            {
                int *ptr=(int *)arg;
                printk("\ndvr_enc_ioctl:Error CH%d job ptr 0x%x\n",enc_data->ch_num,ptr[7]);
                panic("\ndvr_enc_ioctl:Error job NULL");
            }
            printm("##### DVR_ENC_QUEUE_PUT CH%d for %s job %d (stream %d)\n",enc_data->ch_num,job->entity->name,job->id,data.bs.stream);
        }
#endif

        ret = do_queue_put(enc_data, &data);
		if(ret>=0) {
            if(copy_to_user((void *)arg,(void *)&data,sizeof(dvr_enc_queue_get))) {
				ret = -EFAULT;
				goto exit1;
			}
		}
        if(bShowPerf && job_id)
        {
            if(gperf_info[enc_data->ch_num].job_id==job_id)
                str_num=0;
            else if(gperf_info[DVR_RECORD_CHANNEL_NUM+enc_data->ch_num].job_id==job_id)
                str_num=1;
            else 
                str_num=2;
            update_latance(&gperf_info[DVR_RECORD_CHANNEL_NUM*str_num+enc_data->ch_num]);
        }
        break;
      }

	case DVR_ENC_CONTROL:
	{
		dvr_enc_control  enc_ctrl;
		if(copy_from_user((void *)&enc_ctrl, (void *)arg, sizeof(dvr_enc_control))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_enc_ctrl, &enc_ctrl, sizeof(dvr_enc_control));
		switch(enc_ctrl.command)
		{
			case ENC_START:
	//			printk("@@@@@  %s:%d:ENC_START:%d @@@@@\n",__FILE__,__FUNCTION__,enc_ctrl.stream);
//printk("@@ENC_START=%d\n", enc_ctrl.stream);
				ret = start_enc_graph(enc_data, enc_ctrl.stream);
				break;
			case ENC_STOP:
//	printk("@@ENC_STOP=%d\n", enc_ctrl.stream);			
				ret = stop_enc_graph(enc_data, enc_ctrl.stream);
	
				break;
			case ENC_SNAP:
				ret = do_snapshot_jpg(enc_data, (int)enc_ctrl.output.count);
				break;
			case ENC_RAW:
				ret = do_snapshot_raw(enc_data, enc_ctrl.stream, (int)enc_ctrl.output.count);
				break;
			case ENC_UPDATE:
		//		printk("@@ENC_UPDATE=%d\n", enc_ctrl.stream);
				ret = do_update_channel(enc_data, &enc_ctrl);
					
				break;
			default:
				printk("Wrong DVR_ENC_CONTROL command!\n");
				break;
		}
        break;
	}
    case DVR_ENC_RESET_INTRA:
	{
        int   stream;  /* stream, 0:main-bitstream, 1: sub1-bitstream, 2: sub2-bitstream */

		if(copy_from_user((void *)&stream, (void *)arg, sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_reset_intra, &stream, sizeof(int));
		idataout_reset_intra(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*stream);
        ret = 0;
        break;
	}
    case DVR_ENC_SWAP_INTRA:
	{
        int   stream;  /* stream, 0:main-bitstream, 1: sub1-bitstream, 2: sub2-bitstream */

		if(copy_from_user((void *)&stream, (void *)arg, sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_swap_intra, &stream, sizeof(int));
		idataout_swap_intra(enc_data->ch_num+DVR_RECORD_CHANNEL_NUM*stream);
        ret = 0;
        break;
	}
    case DVR_ENC_SUB_PATH_DENOISE_CTRL:
    {
        int ctrl;
        if(copy_from_user((void *)&ctrl, (void *)arg, sizeof(int))) {
			ret = -EFAULT;
			goto exit1;
		}
        memcpy(&enc_data->la_sub_dn_ctrl, &ctrl, sizeof(int));
        if(ctrl){
            enc_data->is_sub_denoise = 1;
        }
        else{
            enc_data->is_sub_denoise = 0;
        }
        break;
    }
	default:
	    printk("enc_ioctl cmd(0x%x) no define!\n", cmd);
	    break;
	}

    if(enc_data->ch_num>=0 && cmd==DVR_ENC_CONTROL) {
        for(i=0; i<DVR_ENC_REPD_BT_NUM+1; i++) {
            buf_flow_info.fnc = BFI_FNC_REC(enc_data->ch_num, i);
            val = dvr_common_retrieve_info(INFO_CHECK_BUF_FLOW_PAIR, &buf_flow_info);
            if(val!=0) {
                printk("DVR_ENC_QUEUE_GET_XXX and DVR_ENC_QUEUE_PUT are not in pairs(in-out:%d).\n", val);
                ret = -2;
                goto exit1;
            }
        }
    }

exit1:
    up(&enc_data->dvr_enc_sem);
    return ret;
}


struct file_operations dvr_enc_fops = {
  	owner:      THIS_MODULE,
	ioctl:		dvr_enc_ioctl,
	mmap:       dvr_enc_mmap,
	open:		dvr_enc_open,
	release:	dvr_enc_release,
	poll:       dvr_enc_poll,
};

	
struct miscdevice dvr_enc_dev = {
	minor: MISC_DYNAMIC_MINOR,
	name: "dvr_enc",
	fops: &dvr_enc_fops,
};

static int dvr_global_buf_init(file_t *cfg_file)
{
    int ret=0;

    // for Main-Bitstream
    if(gmcfg_getfieldint( str_dvr_enc, "enc_in_size", &default_encq_cfg.size, cfg_file) < 0)   ret=-1;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_in_count", &default_encq_cfg.count, cfg_file) < 0)   ret=-2;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_in_ddr", &default_encq_cfg.ddr_num, cfg_file) < 0)   ret=-3;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_out_size", &default_dtoq_cfg.size, cfg_file) < 0)   ret=-4;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_out_count", &default_dtoq_cfg.count, cfg_file) < 0)   ret=-5;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_out_ddr", &default_dtoq_cfg.ddr_num, cfg_file) < 0)   ret=-6;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_in_limit_count", &default_encq_cfg.limit_count, cfg_file) < 0)   ret=-7;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_out_max_w", &default_max_dim.width, cfg_file) < 0)   ret=-8;
    if(gmcfg_getfieldint( str_dvr_enc, "enc_out_max_h", &default_max_dim.height, cfg_file) < 0)   ret=-9;
    if(gmcfg_getfieldint( str_dvr_enc, "scl_out_size", &default_scl_dtoq_cfg.size, cfg_file) < 0)   ret=-10;
    if(gmcfg_getfieldint( str_dvr_enc, "scl_out_count", &default_scl_dtoq_cfg.count, cfg_file) < 0)   ret=-11;
    if(gmcfg_getfieldint( str_dvr_enc, "scl_out_ddr", &default_scl_dtoq_cfg.ddr_num, cfg_file) < 0)   ret=-12;
    if(gmcfg_getfieldint( str_dvr_enc, "scl_out_max_w", &default_scl_max_dim.width, cfg_file) < 0)   ret=-13;
    if(gmcfg_getfieldint( str_dvr_enc, "scl_out_max_h", &default_scl_max_dim.height, cfg_file) < 0)   ret=-14;
    if(gmcfg_getfieldint( str_dvr_enc, "sub_enc_out_size", &default_sub_dtoq_cfg.size, cfg_file) < 0)   ret=-15;
    if(gmcfg_getfieldint( str_dvr_enc, "sub_enc_out_count", &default_sub_dtoq_cfg.count, cfg_file) < 0)   ret=-16;
    if(gmcfg_getfieldint( str_dvr_enc, "sub_enc_out_ddr", &default_sub_dtoq_cfg.ddr_num, cfg_file) < 0)   ret=-17;
    if(gmcfg_getfieldint( str_dvr_enc, "sub_enc_out_max_w", &default_sub_max_dim.width, cfg_file) < 0)   ret=-18;
    if(gmcfg_getfieldint( str_dvr_enc, "sub_enc_out_max_h", &default_sub_max_dim.height, cfg_file) < 0)   ret=-19;
    if(gmcfg_getfieldint( str_dvr_enc, "ss_enc_out_size", &default_ss_dtoq_cfg.size, cfg_file) < 0)   ret=-4;
    if(gmcfg_getfieldint( str_dvr_enc, "ss_enc_out_count", &default_ss_dtoq_cfg.count, cfg_file) < 0)   ret=-8;
    if(gmcfg_getfieldint( str_dvr_enc, "ss_enc_out_ddr", &default_ss_dtoq_cfg.ddr_num, cfg_file) < 0)   ret=-9;
    
    return ret;
}

static void dvr_enc_in_buf_init(int ch_num, file_t *cfg_file)
{
    unsigned char tmp_str[32];
    int buf_size=0, count=0, ddr_num=0, limit_count=3, ret=0;
    QueMemCfg   ch_cfg_enc;
	dvr_graph_vqueuet   *qst, *dup_qst;

    snprintf(tmp_str, 32, "enc_in%d_size", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.size, cfg_file);
    if(ret<0)   buf_size = default_encq_cfg.size;
    else        buf_size = ch_cfg_enc.size;
    snprintf(tmp_str, 32, "enc_in%d_count", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.count, cfg_file);
    if(ret<0)   count = default_encq_cfg.count;
    else        count = ch_cfg_enc.count;
    snprintf(tmp_str, 32, "enc_in%d_ddr", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.ddr_num, cfg_file);
    if(ret<0)   ddr_num = default_encq_cfg.ddr_num;
    else        ddr_num = ch_cfg_enc.ddr_num;
    snprintf(tmp_str, 32, "enc_in%d_limit_count", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.limit_count, cfg_file);
    if(ret<0)   limit_count = default_encq_cfg.limit_count;
    else        limit_count = ch_cfg_enc.limit_count;

    if(count && buf_size)
    {
		qst = &enc_private_data[ch_num].enc_in_qst;
		dup_qst = &enc_private_data[ch_num].enc_in_dup_q;
        snprintf(tmp_str, 32, "%s%d", QNAME_ENC_IN, ch_num);
        qst->size = buf_size;
        qst->count = count;
        qst->ddr = ddr_num;
        qst->limit_count = limit_count;
        qst->que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
        snprintf(tmp_str, 32, "%s%d_d", QNAME_ENC_IN, ch_num);
		dup_qst->size = qst->size;
		dup_qst->count = qst->count;
		dup_qst->ddr = qst->ddr;
		dup_qst->que = dvr_queue_allocate(TYPE_DUP, count, buf_size, tmp_str, qst->que, ddr_num, PATTERN_NONE);
    }
}

static void dvr_enc_out_buf_init(int ch_num, file_t *cfg_file)
{
    unsigned char tmp_str[32];
    int buf_size=0, count=0, ddr_num=0, ret=0;
    QueMemCfg   ch_cfg_enc;
	dvr_graph_vqueuet   *qst;
    dvr_enc_bt			*ebt;	

    ebt = &enc_private_data[ch_num].ebt[0];
	/* main-bitstream, enc_out */
    snprintf(tmp_str, 32, "enc_out%d_size", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.size, cfg_file);
    if(ret<0)   buf_size = default_dtoq_cfg.size;
    else        buf_size = ch_cfg_enc.size;
    snprintf(tmp_str, 32, "enc_out%d_count", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.count, cfg_file);
    if(ret<0)   count = default_dtoq_cfg.count;
    else        count = ch_cfg_enc.count;
    snprintf(tmp_str, 32, "enc_out%d_ddr", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.ddr_num, cfg_file);
    if(ret<0)   ddr_num = default_dtoq_cfg.ddr_num;
    else        ddr_num = ch_cfg_enc.ddr_num;

    snprintf(tmp_str, 32, "enc_out%d_max_w", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ebt->max_dim.width, cfg_file);
    if(ret<0)   ebt->max_dim.width = default_max_dim.width;

    snprintf(tmp_str, 32, "enc_out%d_max_h", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ebt->max_dim.height, cfg_file);
    if(ret<0)   ebt->max_dim.height = default_max_dim.height;

    if(count && buf_size)
    {
        printk("    enc_out%d=(res: %d,%d)\n", ch_num, ebt->max_dim.width, ebt->max_dim.height);
		qst = &enc_private_data[ch_num].ebt[0].enc_out_qst;
        snprintf(tmp_str, 32, "%s%d", QNAME_ENC_OUT, ch_num);
		qst->size = buf_size;
		qst->count = count;
		qst->ddr = ddr_num;
		qst->que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
    }
}

static void dvr_scl_out_buf_init(int ch_num, int scl_num, file_t *cfg_file)
{
    unsigned char tmp_str[32];
    int buf_size=0, count=0, ddr_num=0, ret=0;
    QueMemCfg   ch_cfg_enc;
	dvr_graph_vqueuet   *qst;
    dvr_enc_scl_t       *enc_scl;

    enc_scl = &enc_private_data[ch_num].enc_scl[scl_num];
	/* main-bitstream, enc_out */
    snprintf(tmp_str, 32, "scl%d_out%d_size", scl_num, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.size, cfg_file);
    if(ret<0)   buf_size = default_scl_dtoq_cfg.size;
    else        buf_size = ch_cfg_enc.size;
    snprintf(tmp_str, 32, "scl%d_out%d_count", scl_num, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.count, cfg_file);
    if(ret<0)   count = default_scl_dtoq_cfg.count;
    else        count = ch_cfg_enc.count;
    snprintf(tmp_str, 32, "scl%d_out%d_ddr", scl_num, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.ddr_num, cfg_file);
    if(ret<0)   ddr_num = default_scl_dtoq_cfg.ddr_num;
    else        ddr_num = ch_cfg_enc.ddr_num;

    snprintf(tmp_str, 32, "scl%d_out%d_max_w", scl_num, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &enc_scl->dim.width, cfg_file);
    if(ret<0)   enc_scl->dim.width = default_scl_max_dim.width;

    snprintf(tmp_str, 32, "scl%d_out%d_max_h", scl_num, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &enc_scl->dim.height, cfg_file);
    if(ret<0)   enc_scl->dim.height = default_scl_max_dim.height;

    if(count && buf_size)
    {
        printk("    scl%d_out%d=(res: %d,%d)\n", scl_num, ch_num, enc_scl->dim.width, enc_scl->dim.height);
		qst = &enc_private_data[ch_num].enc_scl[scl_num].enc_scl_q;
        snprintf(tmp_str, 32, "scl%d_out%d", scl_num, ch_num);
		qst->size = buf_size;
		qst->count = count;
		qst->ddr = ddr_num;
		qst->que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
    }
}


static void dvr_ss_enc_out_buf_init(int ch_num, file_t *cfg_file)
{
    unsigned char tmp_str[32];
    int buf_size=0, count=0, ddr_num=0, ret=0;
    QueMemCfg   ch_cfg_enc;
	dvr_graph_vqueuet   *qst;

	/* main-bitstream, enc_out_snap */
    snprintf(tmp_str, 32, "ss_enc_out%d_size", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.size, cfg_file);
    if(ret<0)   buf_size = default_ss_dtoq_cfg.size;
    else        buf_size = ch_cfg_enc.size;
    snprintf(tmp_str, 32, "ss_enc_out%d_count", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.count, cfg_file);
    if(ret<0)   count = default_ss_dtoq_cfg.count;
    else        count = ch_cfg_enc.count;
    snprintf(tmp_str, 32, "ss_enc_out%d_ddr", ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.ddr_num, cfg_file);
    if(ret<0)   ddr_num = default_ss_dtoq_cfg.ddr_num;
    else        ddr_num = ch_cfg_enc.ddr_num;
	// ss_enc_out
    if(count && buf_size)
    {
		qst = &enc_private_data[ch_num].ebt[0].enc_out_qst_snap;
        snprintf(tmp_str, 32, "%s%d", QNAME_SS_ENC_OUT, ch_num);
		qst->size = buf_size;
		qst->count = count;
		qst->ddr = ddr_num;
		qst->que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
    }
}

static void dvr_sub_enc_out_buf_init(int ch_num, int stream, file_t *cfg_file)
{
    unsigned char tmp_str[32];
    int buf_size=0, count=0, ddr_num=0, ret=0;
    QueMemCfg   ch_cfg_enc;
	dvr_graph_vqueuet   *qst;
    dvr_enc_bt			*ebt;	

    ebt = &enc_private_data[ch_num].ebt[stream];
    snprintf(tmp_str, 32, "sub%d_enc_out%d_size", stream, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.size, cfg_file);
    if(ret<0)   buf_size = default_sub_dtoq_cfg.size;
    else        buf_size = ch_cfg_enc.size;
    snprintf(tmp_str, 32, "sub%d_enc_out%d_count", stream, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.count, cfg_file);
    if(ret<0)   count = default_sub_dtoq_cfg.count;
    else        count = ch_cfg_enc.count;
    snprintf(tmp_str, 32, "sub%d_enc_out%d_ddr", stream, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ch_cfg_enc.ddr_num, cfg_file);
    if(ret<0)   ddr_num = default_sub_dtoq_cfg.ddr_num;
    else        ddr_num = ch_cfg_enc.ddr_num;

    snprintf(tmp_str, 32, "sub%d_enc_out%d_max_w", stream, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ebt->max_dim.width, cfg_file);
    if(ret<0)   ebt->max_dim.width = default_sub_max_dim.width;

    snprintf(tmp_str, 32, "sub%d_enc_out%d_max_h", stream, ch_num);
    ret = gmcfg_getfieldint( str_dvr_enc, tmp_str, &ebt->max_dim.height, cfg_file);
    if(ret<0)   ebt->max_dim.height = default_sub_max_dim.height;
    if(count && buf_size)
    {
        printk("    sub%d_enc_out%d=(res: %d,%d)\n", stream, ch_num, ebt->max_dim.width, ebt->max_dim.height);
		qst = &enc_private_data[ch_num].ebt[stream].enc_out_qst;
        snprintf(tmp_str, 32, "sub%d_enc_out%d", stream, ch_num);
		qst->size = buf_size;
		qst->count = count;
		qst->ddr = ddr_num;
		qst->que = dvr_queue_allocate(TYPE_NORMAL, count, buf_size, tmp_str, NULL, ddr_num,PATTERN_NONE);
    }
}

static int __init dvr_enc_init(void)
{
    int i, ret=0, stream, scl_num;
    //unsigned char tmp_str[32];
    file_t *cfg_file;
    
    memset(enc_private_data, 0x0, sizeof(enc_private_data));
    // Read DDR config file
    cfg_file = gmcfg_open(GMDVR_MEM_CFG_FILE, O_RDONLY);
    if(!cfg_file) {
        printk("[dvr_enc] open [%s] failed!\n", GMDVR_MEM_CFG_FILE);
        return -EFAULT;
    }

    platform_version = enc_get_platf_ver();
    printk("Platform GM%x \n", platform_version);

    ret = dvr_global_buf_init(cfg_file);
    if(ret<0) {
        panic("[dvr_enc] Wrong config content! err(%d)\n", ret);
        return -EFAULT;
    }

    for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++) {
		/* main-bitstream, enc_in */
        dvr_enc_in_buf_init(i, cfg_file);
		/* main-bitstream, enc_out */
        dvr_enc_out_buf_init(i, cfg_file);
        for(scl_num=0; scl_num<DVR_ENC_SCALAR_NUM ; scl_num++) {
            dvr_scl_out_buf_init(i, scl_num, cfg_file);
        }
        /* main-bitstream, enc_in_snap */
        dvr_ss_enc_out_buf_init(i, cfg_file);
        for(stream=1; stream < DVR_ENC_REPD_BT_NUM ; stream++) {
            dvr_sub_enc_out_buf_init(i, stream, cfg_file);
        }
        init_MUTEX(&enc_private_data[i].dvr_enc_sem);
    }

    gmcfg_close( cfg_file ) ;

    // Add proc node
    dvr_enc_max_fps = create_proc_entry("enc_max_fps", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_enc_max_fps==NULL)
        return -EFAULT;
    dvr_enc_max_fps->read_proc = (read_proc_t *) max_fps_read_mode;
    dvr_enc_max_fps->write_proc = (write_proc_t *) max_fps_write_mode;
    dvr_enc_max_fps->owner = THIS_MODULE;

    // Add proc node
    dvr_enc_proc = create_proc_entry("enc_log", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_enc_proc==NULL)
        return -EFAULT;
    dvr_enc_proc->read_proc = (read_proc_t *) proc_read_mode;
    dvr_enc_proc->write_proc = (write_proc_t *) proc_write_mode;
    dvr_enc_proc->owner = THIS_MODULE;

    // Add show performance proc node
    dvr_enc_perf = create_proc_entry("enc_perf", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(dvr_enc_perf==NULL)
        return -EFAULT;
    dvr_enc_perf->proc_fops = &proc_perf_ops;
    dvr_enc_perf->owner = THIS_MODULE;

    proc_last_api = create_proc_entry("enc_api", S_IRUGO | S_IXUGO, DvrProcRoot);
    if(proc_last_api==NULL)
        return -EFAULT;
    proc_last_api->read_proc = (read_proc_t *) proc_read_lastapi;
    proc_last_api->write_proc = (write_proc_t *) proc_write_lastapi;
    proc_last_api->owner = THIS_MODULE;

    dvrenc_wq = create_workqueue("dvrenc");

    idataout_init(DtoNotify);
    enc_data_init();
    
    init_MUTEX(&dvr_enc_open_sem);
    init_MUTEX(&dvr_enc_cfg_encoder_sem);
    init_MUTEX(&dvr_enc_snapshot_sem);
    for(i=0;i<MAX_DATAOUT_ENTITY_NUM;i++)
        INIT_LIST_HEAD(&dataout_callback_head[i]);

	return misc_register(&dvr_enc_dev);
}

void Free_RC(int);

static void __exit dvr_enc_clearnup(void)
{
    int i, j;
	dvr_graph_vqueuet   *qst;
//2012_03_06 add for clear waitqueue
     flush_workqueue(dvrenc_wq);
	idataout_terminate();
	 destroy_workqueue(dvrenc_wq);
	 dvrenc_wq=NULL;
///////////////////////
    for(i=0; i<DVR_RECORD_CHANNEL_NUM; i++) {
		qst = &enc_private_data[i].enc_in_qst;
		if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }

		qst = &enc_private_data[i].enc_in_dup_q;
		if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }

		for(j=1; j<DVR_ENC_REPD_BT_NUM; j++) {  
			qst = &enc_private_data[i].ebt[j].sub_enc_in_q;
			if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }

            qst = &enc_private_data[i].ebt[j].sub_enc_in_dup_q;
            if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }
		}
		for(j=0; j<DVR_ENC_REPD_BT_NUM; j++) {
			qst = &enc_private_data[i].ebt[j].enc_out_qst;
			if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }

			qst = &enc_private_data[i].ebt[j].enc_out_qst_snap;
			if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }
		}
		for(j=0; j<DVR_ENC_SCL_NUM; j++) {
			qst = &enc_private_data[i].enc_scl[j].enc_scl_q;
			if(qst->que) { dvr_queue_destroy(qst->que); qst->que = NULL; }
		}
    }


    remove_proc_entry("enc_log", DvrProcRoot);
    remove_proc_entry("enc_perf", DvrProcRoot);
    remove_proc_entry("enc_api", DvrProcRoot);
    remove_proc_entry("enc_max_fps", DvrProcRoot);
    misc_deregister(&dvr_enc_dev);
}


module_init(dvr_enc_init);
module_exit(dvr_enc_clearnup);
MODULE_LICENSE("GPL");
