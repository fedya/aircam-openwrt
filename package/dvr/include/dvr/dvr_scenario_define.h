#ifndef __DVR_SCENARIO_DEFINE_H__
#define __DVR_SCENARIO_DEFINE_H__


//#ifdef GM8181
#define DVR_LCD_NUM                 1
#define DVR_PLANE_NUM_PER_LCD       3
#define DVR_LIVEVIEW_CHANNEL_NUM    4
#define DVR_RECORD_CHANNEL_NUM      5
#define DVR_PLAYBACK_CHANNEL_NUM    4
#define	DVR_ENC_REPD_BT_NUM			9  // 9: (main + 8 sub) bitstream per channel
#define	DVR_ENC_SCL_NUM			    3  //  for 3 different input resolution for encoder.
#define DVR_MAX_3DI_DISP_NUM        1  // in v1.0, only 1 is allowed

#define TWO_STAGES_SCALER           1
#define SHARE_LV_PB_SCL_BUF         1
#define LOG_API_QUEUE_FLOW          0

//#endif



#endif /* __DVR_SCENARIO_DEFINE_H__ */


