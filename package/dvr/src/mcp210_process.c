/* fmjpeg.c */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/hdreg.h>	/* HDIO_GETGEO */
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/pci.h>
#include <linux/spinlock.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/kfifo.h>
//#include <asm/arch/fmem.h>
#include <linux/moduleparam.h>

//#include "../ioctl_h264.h"
#include "h264e_ratecontrol.h"
#include "enc_main.h"
#include "dvr_scenario_define.h"

extern dvr_enc_data    enc_private_data[DVR_RECORD_CHANNEL_NUM];

#define MAX_NUM 255

struct rc_private
{
    H264FRateControl rc;
    int forceIntra;
    int used;
    int ForceNonRef;    // specify this P frame not to be reference. skip this when current frame is I-frame
                        // 1: Force this P frame not to be reference
                        // 0: Force this P frame to be reference
                        // -1:let encoder decide, reference to FWD_Mode parameter.
    int roi_x;          // if < 0, skip roi_x & roi_y
    int roi_y;          // if < 0, skip roi_x & roi_y
};

struct rc_private rc_channel[MAX_NUM];

void Init_RC(rc_data *param,int ch)
{
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(rc_channel[ch].used==1)
    {
        printk("Rate control Channel fd 0x%x alrady init!\n",ch);
        return;
    }
    
    memset(&rc_channel[ch],0,sizeof(struct rc_private));
    
	H264FRateControlInit (&(rc_channel[ch].rc),
		param->u32BitRate,					// target_rate,
		RC_DELAY_FACTOR,			// reaction_delay_factor,
	    RC_AVERAGING_PERIOD,	// averaging_period,
    	RC_BUFFER_SIZE,				//buffer,
		param->num_units_in_tick,
		param->time_scale,
	    param->u32MaxQuant,	// qmax,
    	param->u32MinQuant,		// qmin,
	    param->u32Quant,	// qinit,
        param->u32TargetRateMax,
        param->u32ReactionDelayMax);
    rc_channel[ch].used=1;
    
    rc_channel[ch].forceIntra=-1;
    rc_channel[ch].ForceNonRef=-1;
    rc_channel[ch].roi_x=-1;
    rc_channel[ch].roi_y=-1;
	return;
}

void Update_RC(rc_data *param,int ch)
{
    //int i;
    
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(rc_channel[ch].used!=1)
    {
        printk("Rate control Channel fd 0x%x is not ready!\n",ch);
        return;
    }

	H264FRateControlInit (&(rc_channel[ch].rc),
		param->u32BitRate,					// target_rate,
		RC_DELAY_FACTOR,			// reaction_delay_factor,
	    RC_AVERAGING_PERIOD,	// averaging_period,
    	RC_BUFFER_SIZE,				//buffer,
		param->num_units_in_tick,
		param->time_scale,
	    param->u32MaxQuant,	// qmax,
    	param->u32MinQuant,		// qmin,
	    param->u32Quant,	// qinit,
        param->u32TargetRateMax,
        param->u32ReactionDelayMax);

	return;
}

void Update_RC_Convergence(rc_data *param,int ch)
{
    //int i;
    
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(rc_channel[ch].used!=1)
    {
        printk("Rate control Channel fd 0x%x is not ready!\n",ch);
        return;
    }

    H264FRateControlReInit(&(rc_channel[ch].rc),
		param->u32BitRate,					// target_rate,
		param->num_units_in_tick,
		param->time_scale,
	    param->u32MaxQuant,	// qmax,
    	param->u32MinQuant,		// qmin,
        param->u32TargetRateMax,
        param->u32ReactionDelayMax);
	return;
}

void Free_RC(int ch)
{
    if(ch>MAX_NUM) panic("Free_Rc Error ch number %d, max is %d\n",ch,MAX_NUM);
	rc_channel[ch].used=0;
	return;
}

//-1:auto 1: force intra
void p200_h264e_set_intra(int ch,int intra)
{
    if(ch>MAX_NUM)
        panic("Error ch number %d, max is %d\n",ch,MAX_NUM);
    
    if(rc_channel[ch].forceIntra==intra)
        return;
    rc_channel[ch].forceIntra=intra;
}

void p200_h264e_set_roi_coordinate(int ch, int x, int y)
{
    if(ch>MAX_NUM)
        panic("Error ch number %d, max is %d\n",ch,MAX_NUM);

    if((y != -1) && (y%2)) {
        printk("%s:%d <y=%d is not 2 alignment.>\n",__FUNCTION__,__LINE__,y);
        return;
    }

    rc_channel[ch].roi_x = x;
    rc_channel[ch].roi_y = y;
}

int p200_h264e_frame_pre(struct video_entity_t *entity,MCP200P_PARAMs *rc_enc_param)
{
	H264FRateControl * rc;
	int ch=(entity->fd&0xff);
	
	rc = &(rc_channel[ch].rc);

	rc_enc_param->Qp = rc->rtn_quant;
    if(rc_channel[ch].forceIntra==2) { //force one intra then auto
        rc_enc_param->ForceIntra = 1;
        rc_channel[ch].forceIntra=-1;
    } else
        rc_enc_param->ForceIntra = rc_channel[ch].forceIntra;
    rc_enc_param->ForceNonRef = -1;
    rc_enc_param->roi_x = rc_channel[ch].roi_x;
    rc_enc_param->roi_y = rc_channel[ch].roi_y; 
	return 0;
}

int p200_h264e_frame_post(struct video_entity_t *entity,MCP200Rpt_PARAMs *rc_rpt_param)
{
	H264FRateControl * rc;
	int ch=(entity->fd&0xff);
	
	rc = &(rc_channel[ch].rc);
	H264FRateControlUpdate(rc,rc_rpt_param->Qp,rc_rpt_param->length,rc_rpt_param->keyframe);
  	return 0;
}



