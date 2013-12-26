#ifndef __DVR_MOD_DEFINE_H__
#define __DVR_MOD_DEFINE_H__



#define CAP_FD_MAJOR                 81
#define CAP_FD_MINOR(ch, path)       (((ch)<<4)|path)

#define CASCADE_FD_MAJOR             81
#define CASCADE_FD_MINOR(ch, path)   ((1<<15) | ((ch)<<4) | path)
 
#define LCD_FD_MAJOR                 10
#define LCD_FD_MINOR(n)              ((50<<8) | n)

#define OSD_FD_MAJOR                 10
#define OSD_FD_MINOR(n)              ((46<<8) | n)

/*
In 8CH case...
--------------------
LV       :  0~7
PB       :  8~15
PB(2scl) :  16~23
REC REPD :  main:24~31, sub1:32~39, sub2:40~47
SS       :  48~55
--------------------
*/
#define SCALER_FD_MAJOR              10
#define SCALER_FD_MINOR_LV(ch)       ((30<<8)|ch)
#define SCALER_FD_MINOR_PB(ch)       ((30<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM))
#define SCALER_FD_MINOR_PB2(ch)      ((30<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM+DVR_PLAYBACK_CHANNEL_NUM))
#define SCALER_FD_MINOR_REPD(ch)     ((30<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM+DVR_PLAYBACK_CHANNEL_NUM*2))
#define SCALER_FD_MINOR_SS(ch)       ((30<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM+DVR_PLAYBACK_CHANNEL_NUM*2+DVR_RECORD_CHANNEL_NUM*DVR_ENC_SCL_NUM))
#define SCALER_FD_MINOR_RAW(ch)       ((30<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM+DVR_PLAYBACK_CHANNEL_NUM*2+DVR_RECORD_CHANNEL_NUM*DVR_ENC_SCL_NUM + DVR_RECORD_CHANNEL_NUM))

#define GM3DI_FD_MAJOR               10
#define GM3DI_FD_MINOR_LV(ch)       ((60<<8)|ch)
#define GM3DI_FD_MINOR_RC(ch)       ((60<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM))
#define GM3DI_FD_MINOR_SUBRC(ch)    ((60<<8)|(ch+DVR_LIVEVIEW_CHANNEL_NUM+DVR_RECORD_CHANNEL_NUM))

#define ENTITY_MINOR_MASK           0xff00

#define ENC_H264_FD_MAJOR            10
#define ENC_H264_FD_MINOR_ID         (23<<8)  
#define ENC_H264_FD_MINOR(ch)        (ENC_H264_FD_MINOR_ID|ch)

#define ENC_MPEG4_FD_MAJOR           10
#define ENC_MPEG4_FD_MINOR_ID        (21<<8)
#define ENC_MPEG4_FD_MINOR(ch)       (ENC_MPEG4_FD_MINOR_ID|ch)

#define ENC_MJPEG_FD_MAJOR           10
#define ENC_MJPEG_FD_MINOR_ID        (41<<8)
#define ENC_MJPEG_FD_MINOR(ch)       (ENC_MJPEG_FD_MINOR_ID|ch)
#define ENC_MJPEG_FD_MINOR_SS(ch)    ((ENC_MJPEG_FD_MINOR_ID|ch)+(DVR_RECORD_CHANNEL_NUM*DVR_ENC_REPD_BT_NUM))

#define DEC_H264_FD_MAJOR            10
#define DEC_H264_FD_MINOR_ID         (22<<8)
#define DEC_H264_FD_MINOR(ch)        (DEC_H264_FD_MINOR_ID|ch)

#define DEC_MPEG4_FD_MAJOR           10
#define DEC_MPEG4_FD_MINOR_ID        (20<<8)
#define DEC_MPEG4_FD_MINOR(ch)       (DEC_MPEG4_FD_MINOR_ID|ch)

#define DEC_MJPEG_FD_MAJOR           10
#define DEC_MJPEG_FD_MINOR_ID        (40<<8)
#define DEC_MJPEG_FD_MINOR(ch)       (DEC_MJPEG_FD_MINOR_ID|ch)

#define DATA_OUT_FD_MAJOR            10
#define DATA_OUT_FD_MINOR_ID         (170<<8)
#define DATA_OUT_FD_MINOR(ch)        (DATA_OUT_FD_MINOR_ID|ch)

#define DATA_IN_FD_MAJOR             10
#define DATA_IN_FD_MINOR_ID          (171<<8)
#define DATA_IN_FD_MINOR(ch)         (DATA_IN_FD_MINOR_ID|ch)

#define VBI_PARSER_FD_MAJOR          10
#define VBI_PARSER_FD_MINOR_ID       (62<<8)
#define VBI_PARSER_FD_MINOR(ch)      (VBI_PARSER_FD_MINOR_ID|ch)

#define VBI_INSERT_FD_MAJOR          10
#define VBI_INSERT_FD_MINOR_ID       (63<<8)
#define VBI_INSERT_FD_MINOR(ch)      (VBI_INSERT_FD_MINOR_ID|ch)

#endif /* __DVR_MOD_DEFINE_H__ */

