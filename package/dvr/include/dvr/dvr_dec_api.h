#ifndef __DVR_DEC_API_H__
#define __DVR_DEC_API_H__

#include <linux/ioctl.h>
#include "dvr_type_define.h"
#include "dvr_dec_ioctl.h"

typedef enum dvr_dec_dest_type_tag {
    DEC_TYPE_TO_DISPLAY=0,
    DEC_TYPE_TO_BUFFER,
    DEC_TYPE_COUNT
}dvr_dec_dest_type;

typedef struct DecParam_tag {
    /*! #DecoderOutputColorTag  */
    int     output_type;
    int     reserved[2];
} DecParam;

typedef struct dvr_dec_channel_param_tag {
    /*! #EncodeType_tag */
    int   dec_type;
    /*! #dvr_dec_dest_type_tag */
    int   dec_dest_type;
    int   channel;
    int   is_blocked;
    int   is_use_scaler;
    /*! #DecParam_tag   */
    DecParam    dec_param;
    /*! #ScalerParamtag */
    ScalerParam scl_param;
    int   reserved[8];
}dvr_dec_channel_param;


typedef struct dvr_dec_queue_get_tag {
    /*! #dvr_bs_data_tag    */
    dvr_bs_data bs;
    int     channel;   //for debug only
    int     reserved[1];
}dvr_dec_queue_get;

typedef struct dvr_dec_copy_buf_tag{
    unsigned int bs_buf_length;  /* the bitstream length */
    void *p_bs_buf;
    int reserved[6];
}dvr_dec_copy_buf;

typedef enum dvr_dec_ctrl_cmd_tag {
    DEC_START,
    DEC_STOP,
    DEC_UPDATE
}dvr_dec_ctrl_cmd;

typedef struct dvr_dec_control_tag {
    /*! #dvr_dec_ctrl_cmd_tag   */
    dvr_dec_ctrl_cmd   command;
    struct
    {
        /*! #DIM_tag    */
        DIM     dim;
        /*! #RECT_tag   */
        RECT    win;
        int     bs_rate;    //bitstream frame rate
        int     reserved[2];
    }src_param;
    struct 
    {
        int     plane_id;
        /*! #RECT_tag   */
        RECT    win;
        int     is_display;
        int     display_rate;
        int     reserved[2];
    }dst_param;
}dvr_dec_control;


typedef struct dvr_dec_clear_param_tag
{
  /*! #RECT_tag   */
  RECT          win;
  unsigned int  pattern;
  int           reserved[2];
}dvr_dec_clear_param;

#endif /* __DVR_DEC_API_H__ */

