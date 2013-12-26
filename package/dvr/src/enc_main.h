
#ifndef __DVR_ENC_MAIN_H__
#define __DVR_ENC_MAIN_H__

#include "dvr_type_define.h"
#include "dvr_graph.h"
#include "h264e_ratecontrol.h"
#include "mp4f_ratecontrol.h"
#include "dvr_enc_api.h"

#define	DVR_ENC_SCALAR_NUM			3  // the max number of scalar per channel
#define	DVR_ENC_MAX_STRING			32 
#define	DVR_ENC_MAX_GRAPH_LINK_NUM	10 

typedef struct dvr_enc_bt_tag
{
    int enabled; 		// 0:disabled, 1:enabled
    int enc_type;		// 0:ENC_TYPE_H264, 1:ENC_TYPE_MPEG, 2:ENC_TYPE_MJPEG
	int enc_fps;		// frame rate for this bitstream.
    int is_blocked;     // whether dvr-enc ioctl call is block until its job is done
    int en_snapshot, en_scl_ss;
    int en_scl;
    int enc_fd, scl_fd, dataout_fd;
    int ss_enc_fd, ss_dataout_fd, scl_ss_fd;	// snapshot 
    dvr_graph_vqueuet   sub_enc_in_q;	// for scalar queue
    dvr_graph_vqueuet   sub_enc_in_dup_q;   // for scalar queue
    dvr_graph_vqueuet   enc_out_qst;
    dvr_graph_vqueuet   enc_out_qst_snap;  //for snapshot
    dvr_graph_vqueuet   scl_qst_snap;    
    wait_queue_head_t   *wait_queue_stream, *wait_queue_snapshot;
    struct timeval old_timestamp;
    DIM     max_dim;  //for codec max buffer allocate
    int reserved[6];
}	dvr_enc_bt;

#define ENC_EXT_STRUCT  EncParam_Ext6

typedef struct dvr_enc_scl_tag
{
    int enabled; // DVR_ENC_EBST_DISABLE:disabled, DVR_ENC_EBST_ENABLE:enabled
    int fd; // scalar fd
    dvr_graph_vqueuet enc_scl_q; // for scalar queue
    DIM dim; // scalar dimension
    int format; // scalar format 
} dvr_enc_scl_t;

typedef struct dvr_enc_graph_link_tag
{
    char name[DVR_ENC_MAX_STRING];
	int fd;
	int fps;
	int fps_boundary;
    struct v_queue_t    *que;
} dvr_enc_graph_link_t;

typedef struct 

{
    int is_used;    // used when USE_DYNAMIC_ALLOCATE is disabled
    int ch_num;         // the target to encode
	int input_system;
    int cap_fd[DVR_ENC_REPD_BT_NUM], DI_fd[DVR_ENC_REPD_BT_NUM];
	int fps;
    int src_type;	// ENC_TYPE_FROM_CAPTURE, ENC_TYPE_FROM_CASCADE, ENC_TYPE_FROM_BUFFER
    int is_link_3DI;
    int CapPathMode;        ///0:monopolize 1:Top/Bottom independent 2:Share path
    int is_sub_denoise;
    int SubCapRate;
    struct semaphore    dvr_enc_sem;
    unsigned long 		vma_start;
    unsigned long 		pa_start;
    dvr_graph   		*graph;
    dvr_enc_scl_t       enc_scl[DVR_ENC_SCALAR_NUM];
    dvr_enc_graph_link_t graph_link_info[DVR_ENC_REPD_BT_NUM][DVR_ENC_MAX_GRAPH_LINK_NUM];
    dvr_enc_graph_link_t graph_ss_link_info[DVR_ENC_MAX_GRAPH_LINK_NUM];
    dvr_graph_vqueuet   enc_in_qst;
    dvr_graph_vqueuet   enc_in_dup_q;	    // for 3DI enable out_queue.
    dvr_graph_vqueuet   *sub_enc_in_q;	    // for Sub 3DI enable out_queue.
    dvr_graph_vqueuet   *sub_enc_in_dup_q;	// for Sub 3DI enable out_queue.
	/* ebt[0]: main encoder bitStream, ebt[1]: for reproduction encoder bitSteam_1. */
    dvr_enc_bt			ebt[DVR_ENC_REPD_BT_NUM];	
	H264FRateControl *rc_data;
    //user setting data
	dvr_enc_src_param src;
	ReproduceBitStream repd[DVR_ENC_REPD_BT_NUM];
    ENC_EXT_STRUCT enc_ext[DVR_ENC_REPD_BT_NUM];
	int update_param_flag[DVR_ENC_REPD_BT_NUM];

    //last user call api
    int la_buf_size;   //out
    int la_sub1_buf_size;  //out
    int la_sub2_buf_size;  //out
    int la_ss_buf_size;  //out
    dvr_enc_channel_param   la_ch_param_get;   //out
    dvr_enc_channel_param   la_ch_param_set;   //in
    ReproduceBitStream la_sub_bs; //in
    dvr_enc_queue_get la_que_get;  //out
    dvr_enc_queue_get la_sub1_que_get;  //out
    dvr_enc_queue_get la_sub2_que_get;  //out
    dvr_enc_queue_get la_ss_que_get;   //out
    dvr_enc_queue_get la_que_put;  //in
    dvr_enc_control  la_enc_ctrl;  //in
    int la_reset_intra;    //in
    int la_swap_intra;     //in
    int la_sub_dn_ctrl;    //in
	int isp_fd;//for isp 5M issue
}	dvr_enc_data;

typedef struct rc_data_tag
{
    unsigned int u32BitRate;    
    unsigned int num_units_in_tick;
	unsigned int time_scale;
	unsigned int u32MaxQuant;
	unsigned int u32MinQuant;
	unsigned int u32Quant; 
    unsigned int u32TargetRateMax;
    unsigned int u32ReactionDelayMax; 
	int 		 reserved[6];
}rc_data;

#endif /* __DVR_ENC_MAIN_H__ */

