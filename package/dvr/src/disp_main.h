
#ifndef __DVR_DISP_MAIN_H__
#define __DVR_DISP_MAIN_H__

#include "dvr_type_define.h"
#include "dvr_disp_api.h"
#include "dec_main.h"

typedef struct dvr_disp_data_tag
{
    int  is_used;

    struct {
        int plane_comb;
        int out_system;   // PAL or NTSC
        int display_rate;
        int out_format;   // progressive or interlacing
        int reserved[8];
    }lcd[2];

    struct {
        int  is_link_3DI;
        int  is_use_scaler;
        int  is_set;
        DispParam_Ext1      disp_ext;
        dvr_disp_control    disp_ctrl_params;
    }lv_info[DVR_LIVEVIEW_CHANNEL_NUM];

    //graph data
    dvr_graph   *graph;
    
    dvr_graph_vqueuet   lcd_qst, lcd_dup_qst, osd_qst, osd_dup_qst;
    dvr_graph_vqueuet   di_scl_in_qst[DVR_MAX_3DI_DISP_NUM];
    dvr_graph_vqueuet   di_scl_in_dup_qst[DVR_MAX_3DI_DISP_NUM];
    dvr_graph_vqueuet   scl_in_qst[DVR_LIVEVIEW_CHANNEL_NUM];
    dvr_graph_vqueuet   scl_in_dup_qst[DVR_LIVEVIEW_CHANNEL_NUM];

    //last user call api
    dvr_disp_disp_param  la_disp_param_get;
    dvr_disp_disp_param  la_disp_param_set;
    dvr_disp_update_disp_param  la_disp_param_update;
    dvr_disp_plane_param la_plane_param_get;
    dvr_disp_plane_param la_plane_param_set;
    dvr_disp_update_plane_param la_plane_param_update;
    dvr_disp_control la_disp_ctrl;
    dvr_disp_clear_param la_disp_clear_param;
    
}dvr_disp_data;

#endif /* __DVR_DISP_MAIN_H__ */

