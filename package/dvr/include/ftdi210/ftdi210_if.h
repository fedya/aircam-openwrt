/** file: ftdi210_if.h
 *  the file provides the 3DI function calls or IOCTLs
 *  author Harry Hsu
 *  version: 1.0
 *  date: 2010/9/24
 *
 */
#ifndef __FTDI210_IF_H__
#define __FTDI210_IF_H__

/*
AP usage:
    ioctl(FTDI210_IOCTL_PUT_START)
    ioctl(FTDI210_IOCTL_PUT_START)
    ...
    
Kernel usage:
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=0,wait=0)
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=0,wait=0)
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=0,wait=0)
    ...
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=1,wait=0)
        +----> callback
        +----> callback
        ...
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=1,wait=0)
        +----> callback
        +----> callback
        ...
    ftdi210_push_job(struct ftdi210_param_t *,callback,trigger=1,wait=0)
    ...
 */

/**
 * @brief deinterlace/denoise data structure
 */ 
struct ftdi210_param_t
{
    int          chan_num;      ///< index, from 0 ~ 
    unsigned int width;         ///< width 
    unsigned int height;        ///< height
    unsigned int yuv_format;    ///< don't care now, because only YUV422 is supported 
    unsigned int ff_y_ptr;      ///< Forward Forward luma/Y start address (Top frame) 
    unsigned int fw_y_ptr;      ///< Forward luma/Y start address (Bottom Field) 
    unsigned int cu_y_ptr;      ///< Current luma/Y start address (Top frame) 
    unsigned int nx_y_ptr;      ///< Next luma/Y start address (Bottom Field) 
    unsigned int cu_uv_ptr;     ///< it is only effective in YUV420 
    unsigned int nx_uv_ptr;     ///< it is only effective in YUV420 
    unsigned int result_ptr;    ///< this value should equal to ff_y_ptr 
    unsigned int command;       ///< denoise or deinterlace, value is either CMD_DEINTERLACE or CMD_DENOISE 
    unsigned int denoise_mode;  ///< field or frame, value is DENOISE_MODE_FIELD or DENOISE_MODE_FRAME 
    
    /* The followings are internal use only
     */
    unsigned int denoise_cfg;   ///< offset 0x100. Internal use only 
    int         denoise_luma;   ///< offset 0x110 ~, < 0 means don't care. Internal use only 
    int         denoise_chroma; ///< offset 0x120 ~, < 0 means don't care. Internal use only 
    void        *dn_stats;      ///< denoise_stats_t, denoise statistic, Internal use only 
};

/*
 * MACROs
 */
#define CMD_DEINTERLACE         0
#define CMD_DENOISE             1

#define DENOISE_MODE_FIELD      0
#define DENOISE_MODE_FRAME      1

#define FTDI210_SUCCESS         1
#define FTDI210_FAIL            0
 
/*
 * IOCTL
 */
#define FTDI210_IOC_MAGIC   'F'
#define FTDI210_IOCTL_PUT       _IOWR(FTDI210_IOC_MAGIC, 1, struct ftdi210_param_t) 

#endif /* __FTDI210_IF_H__ */