#ifndef __DVR_TYPE_DEFINE_H__
#define __DVR_TYPE_DEFINE_H__

#include "dvr_scenario_define.h"

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#define GMDVR_MEM_CFG_FILE  "/mnt/mtd/gmdvr_mem.cfg"
#define GMDVR_MAKE_FOURCC(a,b,c,d)         (int)((a)|(b)<<8|(c)<<16|(d)<<24)

#define GMVAL_DO_NOT_CARE       (-54172099)
#define GMVAL_RATE(val,base)    ((base<<16)|val)

#define GMVAL_RT_GET_VAL(rate)  (rate&0x0000FFFF)
#define GMVAL_RT_GET_BASE(rate) (rate>>16)

//Graph ID
#define GFID_DISP(plane_num)(0x10000+plane_num)
#define GFID_ENC(ch_num)    (0x20000+ch_num)
#define GFID_DEC(ch_num)    (0x30000+ch_num)

/**
 * @brief function tag for videograph level
 */
typedef struct FuncTag_tag{
    //bit24~27 is reserved
    int func : 28;          ///< functional tag
    int cas_ch : 4;         ///< cascade channel
    int lv_ch;              ///< liveview channel
    int rec_ch;             ///< record channel
    int pb_ch;              ///< playback channel
}FuncTag;
/*
                 func cas_ch                  lv_ch                 rec_ch                  pb_ch
                    |      |                      |                      |                      |
  xxxx xxxx xxxx   xx     xx    xxxx xxxx xxxx xxxx    xxxx xxxx xxxx xxxx    xxxx xxxx xxxx xxxx
*/
#define MTHD_USE_CMP        0x1

#define FN_NONE             0x0
#define FN_LIVEVIEW         0x1
#define FN_RECORD           0x2
#define FN_PLAYBACK         0x4
#define FN_CASCADE          0x8
#define FN_LCD_PARAM        0x10
#define FN_PLANE_PARAM      0x20
#define FN_SUB1_RECORD      0x40
#define FN_SUB2_RECORD      0x80
#define FN_SUB3_RECORD      0x100
#define FN_SUB4_RECORD      0x200
#define FN_SUB5_RECORD      0x400
#define FN_SUB6_RECORD      0x800
#define FN_SUB7_RECORD      0x1000
#define FN_SUB8_RECORD      0x2000
#define FN_UPDATE_METHOD    0x10000
#define FN_PB_SCL_LINK      0x20000
#define FN_METHOD_USE_CMP   0x8000000
#define FN_SUB_ALL_RECORD   (FN_RECORD|FN_SUB1_RECORD|FN_SUB2_RECORD|FN_SUB3_RECORD|FN_SUB4_RECORD|FN_SUB5_RECORD|FN_SUB6_RECORD|FN_SUB7_RECORD|FN_SUB8_RECORD)


#define FN_RESET_TAG(ptag)           {   (ptag)->func=0; (ptag)->lv_ch=0; (ptag)->rec_ch=0; (ptag)->pb_ch=0; (ptag)->cas_ch=0; }
#define FN_COPY_TAG(newt,ptag)       {   (newt)->func=(ptag)->func; (newt)->lv_ch=(ptag)->lv_ch; (newt)->rec_ch=(ptag)->rec_ch; (newt)->pb_ch=(ptag)->pb_ch; (newt)->cas_ch=(ptag)->cas_ch; }
#define FN_SET_LV_CH(ptag, ch_num)   {   (ptag)->func|=FN_LIVEVIEW;   (ptag)->lv_ch|=(0x1<<ch_num);  }
#define FN_SET_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB1_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB1_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB2_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB2_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB3_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB3_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB4_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB4_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB5_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB5_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB6_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB6_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB7_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB7_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_SUB8_REC_CH(ptag, ch_num)  {   (ptag)->func|=(FN_SUB8_RECORD); (ptag)->rec_ch|=(0x1<<ch_num); }
#define FN_SET_PB_CH(ptag, ch_num)   {   (ptag)->func|=FN_PLAYBACK;   (ptag)->pb_ch|=(0x1<<ch_num);  }
#define FN_SET_CAS_CH(ptag, ch_num)  {   (ptag)->func|=FN_CASCADE;    (ptag)->cas_ch|=(0x1<<ch_num);  }
#define FN_SET_FUNC(ptag, fnc)       {   (ptag)->func|=fnc;        }
#define FN_REMOVE_FUNC(ptag, fnc)    {   (ptag)->func&=(~fnc);     }
#define FN_SET_ALL(ptag)             {   (ptag)->func=(0xFF);   /* Note: 'func' value doesn't include FN_UPDATE_METHOD. */ \
                                           (ptag)->lv_ch=0xFFFFFFFF; (ptag)->rec_ch=0xFFFFFFFF; (ptag)->pb_ch=0xFFFFFFFF; (ptag)->cas_ch=0xF; }
#define FN_IS_UPDATE(ptag)           ((ptag)->func&FN_UPDATE_METHOD)
#define FN_COMPARE(ptagA, ptagB)     ( ((ptagA)->func==(ptagB)->func) && ((ptagA)->lv_ch==(ptagB)->lv_ch) && ((ptagA)->rec_ch==(ptagB)->rec_ch) && ((ptagA)->pb_ch==(ptagB)->pb_ch) && ((ptagA)->cas_ch==(ptagB)->cas_ch) )
#define FN_IS_EMPTY(ptag)            ( ((ptag)->func==0) && ((ptag)->lv_ch==0) && ((ptag)->rec_ch==0) && ((ptag)->pb_ch==0) && ((ptag)->cas_ch==0) )
#define FN_IS_FN(ptag, fn)           ( (ptag)->func&(fn) )
#define FN_REMOVE_LV_CH(ptag, ch)  ( (ptag)->lv_ch&=(~ch) )
/* FN_CHECK_MASK uses AND(&) operation on ptagA with ptagB, and check channel values for LV,REC,PB,CAS */
#if 1
#define FN_CHECK_MASK(ptagA,ptagB) \
        ((((ptagA)->func&(ptagB)->func)==FN_LIVEVIEW)?((ptagA)->lv_ch&(ptagB)->lv_ch):              \
            ((((ptagA)->func&(ptagB)->func)==FN_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch):          \
                ((((ptagA)->func&(ptagB)->func)==FN_SUB1_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB2_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB3_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB4_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB5_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB6_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB7_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB8_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                        ((((ptagA)->func&(ptagB)->func)==FN_PLAYBACK)?((ptagA)->pb_ch&(ptagB)->pb_ch):  \
                            ((((ptagA)->func&(ptagB)->func)==FN_CASCADE)?((ptagA)->cas_ch&(ptagB)->cas_ch): \
                        ((ptagA)->func&(ptagB)->func) ))))))))))))
#else
#define FN_CHECK_MASK(ptagA,ptagB) \
        ((((ptagA)->func&(ptagB)->func)==FN_LIVEVIEW)?((ptagA)->lv_ch&(ptagB)->lv_ch):              \
            ((((ptagA)->func&(ptagB)->func)==FN_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch):          \
                ((((ptagA)->func&(ptagB)->func)==FN_SUB1_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                    ((((ptagA)->func&(ptagB)->func)==FN_SUB2_RECORD)?((ptagA)->rec_ch&(ptagB)->rec_ch): \
                        ((((ptagA)->func&(ptagB)->func)==FN_PLAYBACK)?((ptagA)->pb_ch&(ptagB)->pb_ch):  \
                            ((((ptagA)->func&(ptagB)->func)==FN_CASCADE)?((ptagA)->cas_ch&(ptagB)->cas_ch): \
                        ((ptagA)->func&(ptagB)->func) ))))))
#endif
#define FN_ITEMS(ptag)               (ptag)->func, (ptag)->lv_ch, (ptag)->rec_ch, (ptag)->pb_ch, (ptag)->cas_ch

/* Queue name. 
   Note => the length of queue must be smaller than MAX_NAME_SIZE. */
#define QNAME_LCD           "lcd"
#define QNAME_3DI_SCL       "3di_scl"
#define QNAME_LV_SCL        "lv_scl"
#define QNAME_ENC_IN        "enc_in"
#define QNAME_ENC_OUT       "enc_out"
#define QNAME_SS_ENC_IN     "ssenc_in"
#define QNAME_SS_ENC_OUT    "ssenc_out"
#define QNAME_SUB1_ENC_IN   "sub1enc_in"
#define QNAME_SUB1_ENC_OUT  "sub1enc_out"
#define QNAME_SUB2_ENC_IN   "sub2enc_in"
#define QNAME_SUB2_ENC_OUT  "sub2enc_out"
#define QNAME_DEC_IN        "dec_in"
#define QNAME_PB_SCL        "pb_scl"
#if TWO_STAGES_SCALER
#define QNAME_PB_SCL2       "pb_scl2"
#endif
#define QNAME_OSD           "osd"


typedef enum QueueID_tag {
    QID_LCD=10,
    QID_3DI_SCL,
    QID_LV_SCL,
    QID_ENC_IN,
    QID_ENC_OUT,
    QID_SS_ENC_IN,
    QID_SS_ENC_OUT,
    QID_SUB1_ENC_IN,
    QID_SUB1_ENC_OUT,
    QID_SUB2_ENC_IN,
    QID_SUB2_ENC_OUT,
    QID_DEC_IN,
    QID_PB_SCL
}QueueID;

/**
 * @brief set width and height
 */
typedef struct DIM_tag {
    int  width;     ///< width in pixel
    int  height;    ///< height in pixel
}DIM;

/**
 * @brief set size and position
 */
typedef struct RECT_tag {
    int  x;             ///< x position
    int  y;             ///< y positon
    int  width;         ///< width in pixel
    int  height;        ///< height in pixel
}RECT;

/**
 * @brief set position x and y
 */
typedef struct POS_tag {
    int  x;     ///< x position
    int  y;     ///< y positon
}POS;

/**
 * @brief set ROI position x, y, width, height, and enable/disable
 */
typedef struct ROI_ALL_tag {
    /*! Enable the ROI function, TRUE/FALSE */
    int     is_use_ROI;
    /*! #RECT_tag               */
    RECT    win;
}ROI_ALL;

/**
 * @brief reinit rate control
 */
typedef struct Base_Frame_Rate_Control_tag {
    unsigned short frame_rate; ///< frame rate
    unsigned short frame_rate_base; ///< frame rate base
}Base_Frame_Rate_Control;

/**
 * @brief set queue memory configuration
 */
typedef struct QueueMemConfig_tag {
    int  size;          ///< size of buffer
    int  count;         ///< the number of buffers 
    int  ddr_num;       ///< the DDR number
    int  limit_count;   ///< Max count of in-used buffer
}QueMemCfg;

/**
 * @brief set dvr graph queue configuration
 */
typedef struct dvr_graph_vqueuet_tag
{
    struct v_queue_t    *que;
    int size;               ///< size of buffer
    int count;              ///< the number of buffers
    int ddr;                ///< the DDR number
    int limit_count;        ///< Max count of in-used buffer
    FuncTag user_tag;       ///< function tag for videograph level
}dvr_graph_vqueuet;

/**
 * @brief dvr bit-stream data
 */
typedef struct dvr_bs_data_tag {
    struct timeval timestamp;
    int  offset;            ///< bitstream buffer offset. 
    int  length;            ///< bitstream buffer length.
    int  is_keyframe;       ///< 1: current frame is a keyframe. 0: current frame is not a keyframe.
    int  mv_offset;         ///< motion vector offset
    int  mv_length;         ///< motion vector length

    //internal use
    void  *p_job;           ///< internal use
    int  stream;            ///< internal use
    /*! 0: current P frame was encoded as referenced. */
    /*! 1: current P frame was encoded as non-referenced */ 
    int NonRef;                          
    unsigned int timejiffies;
    int reserved[7];
}dvr_bs_data;

typedef struct dvr_rate_tag {
    int numerator;      ///< Not in use  
    int denominator;    ///< Not in use      
    int reserved[4];    ///< Not in use  
}dvr_ratio;

/**
 * @brief dvr video parameter.
 */
typedef struct video_process_tag {
    /*! TRUE : enable 3D deinterlace , False : disable 3D deinterlace   */
    int  is_3DI;
    /*! TRUE :  enable 3D denoise , False : disable 3D denoise  */
    int  is_denoise;        
    /*! #GM3DIFrameTypeTag      */    
    int  denoise_mode;         
    int  reserved[4];
}video_process;

/**
 * @brief scalar parameter
 */
typedef struct ScalerParamtag {
    /*! #ScaleColorModeTag  */
    int     src_fmt;        ///< source format 
    /*! #ScaleColorModeTag  */
    int     dst_fmt;        ///< destination format     
    /*! #ScaleMethodTag           */    
    int     scale_mode;
    int     is_dither;          ///< Not in use
    int     is_correction;      ///< Not in use
    int     is_album;           ///< Not in use
    int     des_level;          ///< Not in use    
    int     reserved[8];
} ScalerParam;

typedef enum EncodeType_tag {
    ENC_TYPE_H264 = 0,
    ENC_TYPE_MPEG,
    ENC_TYPE_MJPEG,
    ENC_TYPE_YUV422,
	ENC_TYPE_JPEG,
    ENC_TYPE_COUNT
}EncodeType;

/* debug level */
#define     DBG_ENTITY_FNC          0x01
#define     DBG_ENTITY_JOB_FLOW     0x02
#define     DBG_DVR_FNC             0x04
#define     DBG_DVR_DATA_FLOW       0x08
#define     DBG_GRAPH_FNC           0x10
#define     DBG_GRAPH_DATA          0x20


typedef enum LCDOutputColorTypeTag {
    LCD_COLOR_YUV422,
    LCD_COLOR_YUV420,
    LCD_COLOR_RGB = 16,
    LCD_COLOR_ARGB = 16,
    LCD_COLOR_RGB888,
    LCD_COLOR_RGB565,
    LCD_COLOR_RGB555,
    LCD_COLOR_RGB444,
    LCD_COLOR_RGB8
}LCDOutputColorType;

#define     MCP_VIDEO_NTSC  0   ///< video mode : NTSC
#define     MCP_VIDEO_PAL   1   ///< video mode : PAL
#define     MCP_VIDEO_VGA   2   ///< video mode : VGA

typedef enum LCDOutputModeTag {
    LCD_PROGRESSIVE = 0,
    LCD_INTERLACING = 1
}LCDOutputMode;

typedef enum LiveviewFrameTypeTag {
    LVFRAME_EVEN_ODD=0,
    LVFRAME_ENLARGE_ONE_FIELD=1,
    LVFRAME_WEAVED_TWO_FIELDS=2,
    LVFRAME_GM3DI_FORMAT=3
}LiveviewFrameType;

typedef enum LiveviewFrameModeTag {
    LVFRAME_FRAME_MODE=0,
    LVFRAME_FIELD_MODE=1,
    LVFRAME_FIELD_MODE2=2  /* Treat every field as a complete frame, must work with LVFRAME_ENLARGE_ONE_FIELD */
}LiveviewFrameMode;

typedef enum GM3DIFrameTypeTag {
    GM3DI_FIELD=1,
    GM3DI_FRAME=2
}GM3DIFrameType;


typedef enum LiveviewDMAOrderTag {
    DMAORDER_PACKET=0,
    DMAORDER_3PLANAR=1,
    DMAORDER_2PLANAR=2
}LiveviewDMAOrder;

typedef enum LiveviewScalerRatioTag {
    CAPSCALER_KEEP_RATIO=0,
    CAPSCALER_NOT_KEEP_RATIO=1
}LiveviewScalerRatio;


typedef enum CaptureColorModeTag {
    CAPCOLOR_RGB888=0,
    CAPCOLOR_RGB565=1,
    CAPCOLOR_YUV422=2,
    CAPCOLOR_YUV420_M0=3,
    CAPCOLOR_YUV420_M1=4
}CaptureColorMode;

typedef enum EncoderInputFormatTag {
    ENC_INPUT_H2642D=0,
    ENC_INPUT_MP42D=1,
    ENC_INPUT_1D420=2,
    ENC_INPUT_1D422=3
}EncoderInputFormat;

typedef enum DecoderOutputColorTag {
    DEC_OUTPUT_COLOR_YUV420=4,
    DEC_OUTPUT_COLOR_YUV422=5
}DecoderOutputColor;

typedef enum JpegEncInputFormatTag{		
	JCS_yuv420 = 0, 	
	JCS_yuv422 = 1, 	
	JCS_yuv211 = 2, 	
	JCS_yuv333 = 3, 	
	JCS_yuv222 = 4, 	
	JCS_yuv111 = 5, 	
	JCS_yuv400 = 6		
} JpegEncInputFormat;

typedef enum JpegEnc420InputFormatTag{
	JENC_INPUT_MP42D=0,
	JENC_INPUT_1D420=1,
	JENC_INPUT_H2642D=2,
	JENC_INPUT_DMAWRP420=3,
	JENC_INPUT_1D422=4
} JpegEnc420InputFormat;

typedef enum ScaleMethodTag {
    SCALE_LINEAR=0,
    SCALE_NON_LINEAR,
    SCALE_METHOD_COUNT
}ScaleMethod;

typedef enum ScaleColorModeTag {
    SCALE_RGB888=0,
    SCALE_RGB565=1,
    SCALE_H264_YUV420_MODE0=2,
    SCALE_H264_YUV420_MODE1=3,        
    SCALE_YUV444=4,
    SCALE_YUV422=5,
    SCALE_MP4_YUV420_MODE0=6,
    SCALE_MP4_YUV420_MODE1=7
}ScaleColorMode;


typedef enum CapturePathTag {
    CAPPATH_DEFAULT=0,  //If Liveview, use path1, If Record, use path2
    CAPPATH_PATH_1,
    CAPPATH_PATH_2
}CapturePath;


typedef enum LCDResolutionTag {
    LCD_RES_D1,         /* 720x576 or 720x480 */
    LCD_RES_SVGA,       /* 800x600 */
    LCD_RES_XGA,        /* 1024x768 */
    LCD_RES_XVGA,       /* 1280x960 */
    LCD_RES_SXGA,       /* 1280x1024*/
    LCD_RES_1360x768,   /* 1360x768 */
    LCD_RES_COUNT
}LCDResolution;


#define     DEFAULT_D1_WIDTH        720
#define     DEFAULT_D1_HEIGHT       576
#define     DEFAULT_CIF_WIDTH       352 
#define     DEFAULT_CIF_HEIGHT      288 
#define     DEFAULT_QCIF_WIDTH      176 
#define     DEFAULT_QCIF_HEIGHT     144 

#endif /* __DVR_TYPE_DEFINE_H__ */





