#ifndef __DVR_DISP_API_H__
#define __DVR_DISP_API_H__

#include <linux/ioctl.h>

#include "dvr_type_define.h"
#include "dvr_disp_ioctl.h"

#define DVR_PLANE_ID(d,p)   ((d<<8)|p)
#define GET_DISP_NUM_FROM_PLANE_ID(h)     (h>>8)
#define GET_PLANE_NUM_FROM_PLANE_ID(h)    (0xFF&h)


enum dvr_disp_type {
    DISP_TYPE_LIVEVIEW,
    DISP_TYPE_CASCADE,
    DISP_TYPE_PLAYBACK,
    DISP_TYPE_ON_BUFFER,
};

/* TODO: the following should be replaced by including capture header file.... */
/*
#define VIDEO_MODE_PAL      0
#define VIDEO_MODE_NTSC     1
#define VIDEO_MODE_SECAM    2
#define VIDEO_MODE_AUTO     3
*/
/**/


//### Declaration for DVR_DISP_GET_DISP_PARAM and DVR_DISP_SET_DISP_PARAM ###
typedef struct dvr_disp_color_attribute_tag {
    int brightness;
    int saturation;
    int contrast;
    int huesin;
    int huecos;
    int sharpnessk0;
    int sharpnessk1;
    int sharpness_thres0;
    int shaprness_thres1;
    int reserved[2];
}dvr_disp_color_attribute;

typedef struct dvr_disp_color_key_tag {
    int is_enable;
    int color;
    int reserved[2];
}dvr_disp_color_key;

typedef enum dvr_disp_plane_combination_tag {
    BG_ONLY         =0,   ///< only backgraound
    BG_AND_1PLANE   =1,   ///< background and another plane
    BG_AND_2PLANE   =2    ///< background and another 2 planes
}dvr_disp_plane_combination;

typedef struct dvr_disp_vbi_info_tag {
    int filler;     ///<    0:driver do it,  1:ap can set values for it(need to fill the following fields)
    int lineno;
    int lineheight;
    int fb_offset;
    int fb_size;
    int reserved[2];
}dvr_disp_vbi_info;

typedef struct dvr_disp_scaler_info_tag {
    int is_enable;
    /*! #DIM_tag    */
    DIM dim;        
    int reserved[2];
}dvr_disp_scaler_info;

typedef struct dvr_disp_resolution_tag {
    int input_res;
    int output_type;
    int reserved[2];
}dvr_disp_resolution;


typedef struct dvr_disp_disp_param_tag {
    int disp_num;   ///< input(write only)

    int target_id;      ///< where to output.  1:LCD1, 2:LCD2
    /*! #dvr_disp_plane_combination_tag */
    int plane_comb;     ///< the number of active plane for this LCD
    /*! #MCP_VIDEO_NTSC */
    /*! #MCP_VIDEO_PAL  */
    /*! #MCP_VIDEO_VGA  */
    int output_system;  
    /*! #LCDOutputModeTag   */
    int output_mode;    
    /*! #dvr_disp_color_attribute_tag   */
    dvr_disp_color_attribute    color_attrib;
    /*! #dvr_disp_color_key_tag */
    dvr_disp_color_key transparent_color[2];
    /*! #dvr_disp_vbi_info_tag  */
    dvr_disp_vbi_info   vbi_info;
    /*! #dvr_disp_scaler_info_tag   */
    dvr_disp_scaler_info    scl_info;
    /*! #DIM_tag    */
    DIM dim;            ///< default background dimension
    /*! #dvr_disp_resolution_tag    */
    dvr_disp_resolution res;
    int reserved[8];
}dvr_disp_disp_param;



//### Declaration for DVR_DISP_UPDATE_DISP_PARAM ###
typedef enum dvr_disp_disp_param_name_tag {
    DISP_PARAM_TARGET,
    DISP_PARAM_PLANE_COMBINATION,
    DISP_PARAM_OUTPUT_MODE,
    DISP_PARAM_OUTPUT_SYSTEM,
    DISP_PARAM_COLOR_ATTRIBUTE,
    DISP_PARAM_TRANSPARENT_COLOR,
    DISP_PARAM_RESOLUTION,
    DISP_PARAM_APPLY        /* Use this to apply all parameters. */
}dvr_disp_disp_param_name; 


typedef struct dvr_disp_update_disp_param_tag {
    int disp_num;   ///< input
    /*! #dvr_disp_disp_param_name_tag   */
    dvr_disp_disp_param_name  param;
    // output
    struct val_t {
        int target_id;
        /*! #dvr_disp_plane_combination_tag */
        int plane_comb;
        /*! #MCP_VIDEO_NTSC */
        /*! #MCP_VIDEO_PAL  */
        /*! #MCP_VIDEO_VGA  */
        int output_system;
        /*! #LCDOutputModeTag   */
        int output_mode;
        int display_rate;   ///< NTSC : 30, PAL : 25
        /*! #dvr_disp_color_attribute_tag   */
        dvr_disp_color_attribute    color_attrib;
        /*! #dvr_disp_color_key_tag */
        dvr_disp_color_key transparent_color[2];
        /*! #dvr_disp_vbi_info_tag  */
        dvr_disp_vbi_info   vbi_info;
        /*! #dvr_disp_scaler_info_tag   */
        dvr_disp_scaler_info    scl_info;
        /*! #dvr_disp_resolution_tag    */
        dvr_disp_resolution res;
        int reserved[8];
    }val;
}dvr_disp_update_disp_param;



//### Declaration for DVR_DISP_GET_PLANE_PARAM and DVR_DISP_SET_PLANE_PARAM ###
typedef struct dvr_disp_plane_param_tag {
    int   plane_id;
    /*! #RECT_tag               */
    RECT  win;
    /*! #LCDOutputModeTag   */
    int   data_mode;              
    /*! #LCDOutputColorTypeTag  */
    int   color_mode;
    int   reserved[2];
}dvr_disp_plane_param_st;

typedef struct dvr_disp_plane_param_st_tag {
    //input
    int disp_num;
    int plane_num;
    /*! #dvr_disp_plane_param_tag   */
    dvr_disp_plane_param_st  param;
    int reserved[2];
}dvr_disp_plane_param;


//### Declaration for DVR_DISP_UPDATE_PLANE_PARAM ###
typedef enum dvr_disp_plane_param_name_tag {
    PLANE_PARAM_COLOR_MODE,
    PLANE_PARAM_WINDOW,
    PLANE_PARAM_DATA_MODE,
    PLANE_PARAM_APPLY
}dvr_disp_plane_param_name;

typedef struct dvr_disp_update_plane_param_tag {
    //input
    int   plane_id;
    /*! #dvr_disp_plane_param_name_tag  */
    dvr_disp_plane_param_name   param;
    int reserved[8];
    union {
        /*! #RECT_tag               */
        RECT  win;
        /*! #LCDOutputModeTag   */
        int   data_mode;
        /*! #LCDOutputColorTypeTag  */
        int   color_mode;
    }val;
    int reserved1[8];
}dvr_disp_update_plane_param;


//### Declaration for DVR_DISP_CONTROL ###
typedef enum dvr_disp_ctrl_cmd_tag {
    DISP_START,
    DISP_STOP,
    DISP_UPDATE,
    DISP_RUN,
}dvr_disp_ctrl_cmd;
//+++ stanley add
typedef enum dvr_disp_channel_type_tag {
    DISP_NORMAL_CHN,
    DISP_LAYER0_CHN,
    DISP_LAYER1_CHN,    
    DISP_WITH_ENC,    
}dvr_disp_channel_type; 

typedef struct dvr_osd_setting_tag {
    int             enable;
    DIM             dim;
    RECT            win;     
    unsigned int    obj_va;    
    unsigned int    alpha;
    unsigned int    magic_number;
} dvr_osd_setting;

typedef struct DispParam_Ext1_tag {
    /*! #dvr_disp_channel_type_tag   */
    dvr_disp_channel_type   chn_type;
    /*! #dvr_osd_setting_tag   */
    dvr_osd_setting         osd_setting;         
}DispParam_Ext1;

//---
#define DVR_PARAM_MAGIC 0x1688
#define DVR_PARAM_MAGIC_SHIFT   16
#define CAP_BUF_ID(id)  ((DVR_PARAM_MAGIC << DVR_PARAM_MAGIC_SHIFT)|(id))
//+++ stanley add
#define DVR_DISP_MAGIC_ADD_VAL(val)  ((DVR_PARAM_MAGIC << DVR_PARAM_MAGIC_SHIFT)|(val))
#define DVR_DISP_CHECK_MAGIC(val)  (((val)>>DVR_PARAM_MAGIC_SHIFT)==DVR_PARAM_MAGIC)
#define DVR_DISP_GET_VALUE(val)    ((val)&((1<<DVR_PARAM_MAGIC_SHIFT)-1))
//---

typedef struct dvr_disp_control_tag {
    /*! #dvr_disp_type  */
    int   type;
    int   channel;
    /*! #dvr_disp_ctrl_cmd_tag  */
    dvr_disp_ctrl_cmd   command;

    int   reserved[8];
    
    struct
    {
        struct {
            int   cap_path;
            /*! #LiveviewFrameTypeTag   */
            int   di_mode;          
            /*! #LiveviewFrameModeTag   */
            int   mode;
            /*! #LiveviewDMAOrderTag    */
            int   dma_order;        
            /*! #LiveviewScalerRatioTag */
            int   scale_indep;      ///< 0:fixed aspect-ratio
            /*! #MCP_VIDEO_NTSC */
            /*! #MCP_VIDEO_PAL  */
            /*! #MCP_VIDEO_VGA  */
            int   input_system;  
            int   cap_rate;
            /*! #CaptureColorModeTag    */
            int   color_mode;
            int   is_use_scaler;    //if TRUE, capture as "src_para.lv.dim" and scale it to "des_param.lv.win"
                                    //if FALSE, capture as "des_param.lv.win" and display it directly
            /*! #DIM_tag    */
            DIM   dim;
            /*! #RECT_tag           */
            RECT  win;              
            /*! #video_process_tag  */
            video_process vp_param;
            /*! #ScalerParamtag     */
            ScalerParam scl_param;  //if is_use_scaler is TRUE, user needs to fill this structure
            int   cap_buf_id;       
            /*! display parameter extension size  */        
            int   ext_size;
            /*! point to display parameter extension structure  */
            void  *pext_data;
            int   reserved[2];
        }lv;
        struct {
            /*! #RECT_tag               */
            RECT  win;
            int   rate;
            /*! #LiveviewFrameTypeTag   */
            int   di_mode;          
            /*! #LiveviewFrameModeTag   */
            int   mode;
            /*! #LiveviewDMAOrderTag    */
            int   dma_order;        
            /*! #LiveviewScalerRatioTag */
            int   scale_indep;      
            /*! #MCP_VIDEO_NTSC */
            /*! #MCP_VIDEO_PAL  */
            /*! #MCP_VIDEO_VGA  */
            int   input_system;     
            int   cap_rate;
            /*! #CaptureColorModeTag    */
            int   color_mode;
            int   is_use_scaler;    //if TRUE, capture as "src_para.lv.dim" and scale it to "des_param.lv.win"
                                    //if FALSE, capture as "des_param.lv.win" and display it directly
            /*! #DIM_tag    */
            DIM   dim;              
            /*! #video_process_tag  */
            video_process vp_param;
            /*! #ScalerParamtag */
            ScalerParam scl_param;  ///< if is_use_scaler is TRUE, user needs to fill this structure
            int   reserved[5];
        }cas;        
        int   reserved[5];
    }src_param;

    struct 
    {
        struct {
            /*! #RECT_tag               */
            RECT  win;
            int   plane_id;
            int   reserved[5];
        }lv;

        struct {
            int   path;
            /*! #RECT_tag               */
            RECT  win;
            /*! #RECT_tag               */
            RECT  rect0;
            /*! #RECT_tag               */
            RECT  rect1;
            /*! #RECT_tag               */
            RECT  swc_rect0;
            /*! #RECT_tag               */
            RECT  swc_rect1;
            /*! #RECT_tag               */
            RECT  bg_rect0;
            /*! #RECT_tag               */
            RECT  bg_rect1;
            int   plane_id;
            int   reserved[5];
        }cas;
        int   reserved[5];
    }dst_param;
}dvr_disp_control;


typedef struct dvr_disp_clear_param_tag
{
  int           plane_id;
  /*! #RECT_tag               */
  RECT          win;
  unsigned int  pattern;
  int           reserved[2];
}dvr_disp_clear_param;

#endif /* __DVR_DISP_API_H__ */


