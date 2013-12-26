#include <linux/module.h>
#include "videograph_api.h"
#include <linux/semaphore.h>
#include "enc_main.h"

#define MAX_NUM 255

struct mp4_rc_private
{
    MP4FRateControl rc;
    int forceIntra;
    int used;
    int roi_x;          // if < 0, skip roi_x & roi_y
    int roi_y;          // if < 0, skip roi_x & roi_y
};

struct mp4_rc_private mp4_rc_channel[MAX_NUM];
mje_process mje_rc[MAX_NUM];

void init_mje_rc(mje_process *param, int ch)
{
	mje_rc[ch].u32ImageQuality = param->u32ImageQuality;
	mje_rc[ch].u8JPGPIC = param->u8JPGPIC;
	mje_rc[ch].roi_x = -1;
	mje_rc[ch].roi_y = -1;
}

void init_mpeg4_rc(rc_data *param,int ch)
{
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(mp4_rc_channel[ch].used==1)
    {
        printk("Rate control Channel fd 0x%x alrady init!\n",ch);
    }
    
    memset(&mp4_rc_channel[ch],0,sizeof(struct mp4_rc_private));
	MP4FRateControlInit (&(mp4_rc_channel[ch].rc),
		param->u32BitRate,				// target_rate,
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
    mp4_rc_channel[ch].used=1;
    mp4_rc_channel[ch].forceIntra=-1;
    mp4_rc_channel[ch].roi_x=-1;
    mp4_rc_channel[ch].roi_y=-1;
	return;
}

void free_mpeg4_rc(int ch)
{
    if(ch>MAX_NUM) panic("Free_Rc Error ch number %d, max is %d\n",ch,MAX_NUM);
	mp4_rc_channel[ch].used=0;
	return;
}

void update_mpeg4_rc(rc_data *param,int ch)
{
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(mp4_rc_channel[ch].used!=1)
    {
        printk("Rate control Channel fd 0x%x is not ready!\n",ch);
        return;
    }

	MP4FRateControlInit (&(mp4_rc_channel[ch].rc),
		param->u32BitRate,				// target_rate,
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

void update_mpeg4_rc_convergence(rc_data *param,int ch)
{
    if(ch>MAX_NUM)
        panic("Init_RC Error ch number %d, max is %d\n",ch,MAX_NUM);

    if(mp4_rc_channel[ch].used!=1)
    {
        printk("Rate control Channel fd 0x%x is not ready!\n",ch);
        return;
    }
    MP4FRateControlReInit(&(mp4_rc_channel[ch].rc),
		param->u32BitRate,				// target_rate,
		param->num_units_in_tick,
		param->time_scale,
		param->u32MaxQuant,	// qmax,
		param->u32MinQuant,		// qmin,
		param->u32TargetRateMax,
		param->u32ReactionDelayMax);

	return;
}

void mje_set_roi_coordinate(int ch, int x, int y)
{
    if(ch>MAX_NUM)
        panic("Error ch number %d, max is %d\n",ch,MAX_NUM);

    if((y != -1) && (y%2)) {
        printk("%s:%d <y=%d is not 2 alignment.>\n",__FUNCTION__,__LINE__,y);
        return;
    }
    mje_rc[ch].roi_x = x;
    mje_rc[ch].roi_y = y;
}

int mje_frame_pre(void *ent, void * param)
{
	struct video_entity_t *entity = (struct video_entity_t *)ent;
	int dev = entity->minor&0xFF;
	mje_process * mje_p = (mje_process *)param;
	
	mje_p->u32ImageQuality = mje_rc[dev].u32ImageQuality;
	mje_p->u8JPGPIC = mje_rc[dev].u8JPGPIC;
	mje_p->luma_qtbl = NULL;
	mje_p->chroma_qtbl= NULL;
    mje_p->roi_x = mje_rc[dev].roi_x;
    mje_p->roi_y = mje_rc[dev].roi_y;
	return 0;
}

int mje_frame_post(void *ent, void * param)
{
	return 0;
}

//-1:auto 1: force intra
void mpeg4_set_intra(int ch,int intra)
{
    if(ch>MAX_NUM)
        panic("Error ch number %d, max is %d\n",ch,MAX_NUM);
    
    if(mp4_rc_channel[ch].forceIntra==intra)
        return;
    mp4_rc_channel[ch].forceIntra=intra;
}

void mpeg4_set_roi_coordinate(int ch, int x, int y)
{
    if(ch>MAX_NUM)
        panic("Error ch number %d, max is %d\n",ch,MAX_NUM);

    if((y != -1) && (y%2)) {
        printk("%s:%d <y=%d is not 2 alignment.>\n",__FUNCTION__,__LINE__,y);
        return;
    }
    mp4_rc_channel[ch].roi_x = x;
    mp4_rc_channel[ch].roi_y = y;
}

int mpeg4_frame_pre(struct video_entity_t *entity, void * param)
{
	MP4FRateControl * rc;
	mp4e_process * mp4e_p = (mp4e_process *)param;
	int ch=(entity->fd&0xff);
	
	rc = &(mp4_rc_channel[ch].rc);
    
	mp4e_p->quant = rc->rtn_quant;
	mp4e_p->quant_intra_matrix = NULL;
	mp4e_p->quant_inter_matrix = NULL;
	mp4e_p->intra = mp4_rc_channel[ch].forceIntra;

    if(mp4_rc_channel[ch].forceIntra==2) { //force one intra then auto
        mp4e_p->intra = 1;
        mp4_rc_channel[ch].forceIntra=-1;
    } else
        mp4e_p->intra = mp4_rc_channel[ch].forceIntra;
        
	mp4e_p->module_time_base = 0;
    mp4e_p->roi_x = mp4_rc_channel[ch].roi_x;
    mp4e_p->roi_y = mp4_rc_channel[ch].roi_y;
	return 0;
}

int mpeg4_frame_post(struct video_entity_t *entity, void * param)
{
	MP4FRateControl * rc;
    mp4e_process * mp4e_p = (mp4e_process *)param;
	int ch=(entity->fd&0xff);
	
	rc = &(mp4_rc_channel[ch].rc);
    MP4FRateControlUpdate(rc, mp4e_p->quant, mp4e_p->length, mp4e_p->intra);
  	return 0;
}




