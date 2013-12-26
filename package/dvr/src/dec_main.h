#ifndef __DVR_DEC_MAIN_H__
#define __DVR_DEC_MAIN_H__

#include "dvr_type_define.h"
#include "dvr_dec_api.h"

typedef struct dvr_dec_data_tag
{
    int is_used;
    int dec_type;
    int dest_type;
    int ch_num;
    //int plane_num;
    int is_blocked;     // whether dvr-dec ioctl call is block until its job is done
    int is_use_scaler;
    int bs_rate;
    int display_rate;
    int datain_fd, dec_fd;
    FuncTag tag, tag_scl;
    int b_two_scls;

    struct semaphore    dvr_dec_sem;
    unsigned long vma_start;
    wait_queue_head_t   *wait_queue_stream;

    //user setting data
    dvr_dec_channel_param ch_param_data;
    dvr_dec_control dec_ctrl_data;

    //graph data
    dvr_graph   *graph;
    dvr_graph_vqueuet   dec_in_qst, pb_scl_qst;
#if TWO_STAGES_SCALER
    dvr_graph_vqueuet   pb_scl2_qst;
#endif

    // The following are used only for DEC_TYPE_TO_DISPLAY
    int disp_fd, scl_fd, disp2_fd;
#if TWO_STAGES_SCALER
    int scl2_fd;
#endif
    DIM lcd_dim;
    dvr_graph_vqueuet  lcd_qst;
    dvr_info_disp_count disp_info;

#define SET_CLEAR_TRIGGER   0x22113365
    int          clear_trigger;
    unsigned int clear_pattern;


    //last user call api
    int                     la_buf_size;
    dvr_dec_channel_param   la_ch_param_set;
    dvr_dec_channel_param   la_ch_param_get;
    dvr_dec_queue_get       la_q_data_get;
    dvr_dec_queue_get       la_q_data_put;
    dvr_dec_control         la_dec_ctrl;
    dvr_dec_clear_param     la_dec_clear_param;
    dvr_dec_clear_param     la_dec_clear_param2;

}dvr_dec_data;




#endif /* __DVR_DEC_MAIN_H__ */

