//------------------------------------------
//   frame layer Rate control header file
//				Richard Wang		
//				03/16/2006
//------------------------------------------
#ifndef _H264F_RATECONRTROL_
#define _H264F_RATECONRTROL_

//typedef long long int64_t;
//typedef unsigned long long uint64_t;
#define rc_MAX_QUANT  52
#define rc_MIN_QUANT  0

#define shift	17
#define shift1	7
#define scale  (1<<shift)
#define scale_2  (scale<<1)
#define scale_0_1  (long) (scale*0.1)
#define scale_internal  (1<<shift1)
typedef struct
{
	unsigned short frames;
	int64_t total_size_fix;
	int64_t target_rate1;		// target_rate1 = original_target_bitrate * fincr
	int64_t avg_framesize_fix;
	int64_t target_framesize_fix;
} H264FRC_statics;

typedef struct
{
	short rtn_quant;
	short init_quant;
	unsigned short fincr;			// framerate = fbase/fincr
	unsigned short fbase;		// framerate = fbase/fincr
	short max_quant;
	short min_quant;
	int64_t quant_error_fix[rc_MAX_QUANT];
	int64_t sequence_quality_fix;
	int averaging_period;
	int reaction_delay_factor;
	int buffer;
	int reaction_delay_max;		// max frame count will the rate-control stay when away from the target bitrate
	int state_fcount; 	// count up when keep at max_bitrate or target_bitrate
	int mustTarget; 		// = 1: must approach to target_bitrate
	H264FRC_statics normal;
	H264FRC_statics max;
	int qp_total_n;
	int qp_no_n;
	int64_t fsz_th;
	int64_t fsz_devation;
	int64_t framesbb;		// just for debug
	int rc_mode;		// 0: CBR, only target bitrate
							// 1: ECBR,
							//				usually stays at target bitrate,
							//				produce large bitrate when big motion, but never more than max bitrate
							// 2: VBR,
							//				usuall stays at init. qp,
							//				may be decrease quality (qp up) when hit max bitrate
	H264FRC_statics * rc_prev; 	//prevous rc
	int ch;
}
H264FRateControl;

#define RC_DELAY_FACTOR         	8//4
#define RC_AVERAGING_PERIOD	100
#define RC_BUFFER_SIZE				100

#define quality_const (double)((double)2/(double)rc_MAX_QUANT)
#define quality_fix_const (scale_2/rc_MAX_QUANT)
int H264FRateControlInit(H264FRateControl * rate_control,
				unsigned int target_rate,
				unsigned int reaction_delay_factor,
				unsigned int averaging_period,
				unsigned int buffer,
				unsigned short fincr,
				unsigned short fbase,
				int max_quant,
				int min_quant,
				unsigned int initq,
				unsigned int target_rate_max, 		// in bps
							// 1. CBR:	when (target_rate != 0) && (target_rate_max == 0)
							//		only target bitrate
							// 2: ECBR:	when (target_rate != 0) && (target_rate_max > target_rate)
							//		usually stays at target bitrate,
							//		produce large bitrate when big motion, but never more than max bitrate
							// 3: VBR:	when (target_rate_max != 0) && (target_rate == 0)
							//		usuall stays at init. qp,
							//		may be decrease quality (qp up) when hit max bitrate
				unsigned int reaction_delay_max);
							// max frame count will the rate-control stay when away from the target bitrate
							// only valid at ECBR

int H264FRateControlReInit(H264FRateControl * rate_control,
				unsigned int target_rate,
				unsigned short fincr,
				unsigned short fbase,
				int max_quant,
				int min_quant,
				unsigned int target_rate_max, 		// in bps
							// 1. CBR:	when (target_rate != 0) && (target_rate_max == 0)
							//		only target bitrate
							// 2: ECBR: when (target_rate != 0) && (target_rate_max > target_rate)
							//		usually stays at target bitrate,
							//		produce large bitrate when big motion, but never more than max bitrate
							// 3: VBR:	when (target_rate_max != 0) && (target_rate == 0)
							//		usuall stays at init. qp,
							//		may be decrease quality (qp up) when hit max bitrate
				unsigned int reaction_delay_max);
							// max frame count will the rate-control stay when away from the target bitrate
							// only valid at ECBR
				
void H264FRateControlUpdate(H264FRateControl *rate_control,
				  short quant,
				  int frame_size,
				  int keyframe);	// caller using rate_control->rtn_quant, it's recommanded qp value for next time use
#endif
