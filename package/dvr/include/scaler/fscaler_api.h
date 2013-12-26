/**
 * @file fscaler_api.c
 *  This header file is for scaler ioctl interface.
 *  The modules who support this interface are as below:
 *  scaler_v2
 *  scaler_v2_old
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.1 $ 1.0.0
 * $Date: 2010/09/07 06:46:01 $ 2010/09/07
 *
 * ChangeLog:
 * 2010/09/07 slchen <slchen@grain-media.com>
 *  * Remove local copy and move to module/include directory.
 *  * Add comments and fit coding style
 */

#ifndef _FSCALER_API_H_
#define _FSCALER_API_H_

#define SCALER_FALSE 	0	///< FALSE
#define SCALER_TRUE	1	///< TRUE

#define SCALER_IDLE	0	///< Idle
#define SCALER_TIMEOUT	1	///< Timeout
#define SCALER_DONE	2	///< Done
#define SCALER_ERR	3	///< Error
#define SCALER_RESET	4	///< Reset

// Scaler I/O control ID
#define FSCALER_IOCTL_MEM2MEM  	0x2170  ///< Initialize scaler, Memory-to-Memory
#define FSCALER_IOCTL_CAP2MEM	0x2171	///< Initialize scaler, Capture-to-Memory (Reserved)
#define FSCALER_IOCTL_FRAME   	0x2172  ///< Trigger size down/up (single)
#define FSCALER_IOCTL_FRAMES	0x2173	///< Trigger size down/up (series)
#define FSCALER_IOCTL_RESET	0x2174  ///< Software reset
#define FSCALER_IOCTL_BORDER	0x2175  ///< Setup border
#define FSCALER_IOCTL_IE	0x2176  ///< Setup IE conditions
#define FSCALER_IOCTL_SHARP	0x2177  ///< Setup sharpness
#define FSCALER_IOCTL_TEST	0x2178  ///< Pattern test
#define FSCALER_IOCTL_RATIO	0x2179	///< Setup ratio
#define FSCALER_IOCTL_CLK	0x217A	///< Setup clock
#define FSCALER_IOCTL_INTERLACE 0x217B	///< Setup interlace
#define FSCALER_IOCTL_ALLOC     0x217C	///< Allocate memory
#define FSCALER_IOCTL_FREE	0x217D	///< Free memory
#define FSCALER_IOCTL_DEBUG	0x217E	///< Debug (Reserved)

// Function pointers for DMA/General memory allocation/free
typedef void *(*FSCALER_DMA_MALLOC_PTR) (unsigned int size, unsigned char align_size,
                                         unsigned char reserved_size, void **phy_ptr);
typedef void (*FSCALER_DMA_FREE_PTR) (void *virt_ptr, void *phy_ptr);
typedef void *(*FSCALER_MALLOC_PTR) (unsigned int size, unsigned char align_size,
                                     unsigned char reserved_size);
typedef void (*FSCALER_FREE_PTR) (void *virt_ptr);

// Scaler initialization parameters
struct scaler_init_param_t {
    unsigned int API_version;		///< API version, for API version check
    unsigned int baseaddr;		///< Scaler ip base address
    unsigned int buf_offset;		///< Source memory offset
    unsigned int vp_width;		///< Source width
    unsigned int vp_height;		///< Source height
    unsigned int swc_width;		///< Source window width
    unsigned int swc_height;		///< Source window height
    unsigned int swc_axis_x;		///< Source window x offset
    unsigned int swc_axis_y;		///< Source window y offset
    unsigned int target_width;		///< Target width
    unsigned int target_height;		///< Target height
    unsigned int crop_width;		///< Crop window width
    unsigned int crop_height;		///< Crop window height
    unsigned int crop_axis_x;		///< Crop window x offset
    unsigned int crop_axis_y;		///< Crop window y offset

    // Destination memory format
    unsigned int des_width;		///< Destination memory width
    unsigned int des_height;		///< Destination memory height
    unsigned char des_fmt;      	///< Destination format
    					///< 0: RGB888 1: RGB565 2,3: reserved 4:YCbCr444
    					///< 5: YCbCr422 6: YCbCr420 mode0 7: YCbCr420 mode1
    unsigned int des_order;     	///< Desination momory layout,
    					///< 0: sequential 1:4x4MB 2:8x8MB 3:16x16MB
    unsigned char des_level;    	///< Destination color level, 0: for 256 1: for 240
    unsigned char des_swap_CbCr;        ///< Is destination CbCr swap? 0: CrCb/RB 1: CbCr/BR
    unsigned char des_swap_Y;   	///< Is Destination Y swap?
    					///< 0: CbYCr/CrYCb or RBG/BRG 1: YCbCr/YCrCb or GRB/GBR

    // Source memory format
    unsigned int src_width;		///< Source memory width
    unsigned int src_height;		///< Source memory height
    unsigned char src_fmt;      	///< Source format, 4: YUV444 5: YUV422
    unsigned char src_interleaved;      ///< Is source interleaved?
    					///< 0: for interleaved 1: for non-interleaved
    unsigned char src_swap_CbCr;        ///< Is source CbCr swap? 0: CrCb/RB 1: CbCr/BR
    unsigned char src_swap_Y;		///< Is source Y swap?
    					///< 0: CbYCr/CrYCb or RBG/BRG 1: YCbCr/YCrCb or GRB/GBR
    unsigned char src_hblank;		///< Source memory hblank

    // Functionality
    unsigned char non_linear;		///< Is non-linear scale enable?
    unsigned char dither;		///< Is dither enable?
    unsigned char us_en;		///< Is CbCr color upsampling enable?
    unsigned char correction;		///< Is error correction enable?(For capture source)
    unsigned char album;		///< Is album mode enable?
    unsigned char disable_dispaly;      ///< Is diable display? (For video graph use case)

    /// The function pointers to user-defined DMA/General memory allocation/free function.
    FSCALER_DMA_MALLOC_PTR pfnDmaMalloc;
    FSCALER_DMA_FREE_PTR pfnDmaFree;
    FSCALER_MALLOC_PTR pfnMalloc;
    FSCALER_FREE_PTR pfnFree;
    unsigned int u32CacheAlign;		///< Cache align size
    unsigned int mem_src_pit;		///< Source memory pitch
};
typedef struct scaler_init_param_t fscaler_param;

// Scaler memory ioctl parameters
struct scaler_mem_param_t {
    unsigned int size;			///< memory size
    unsigned int align;			///< Align size
    unsigned int phy_addr;		///< Physical address
    unsigned int virt_addr;		///< Virtual address
};
typedef struct scaler_mem_param_t fscaler_mem_param;

// Scaler frame ioctl parameters
struct scaler_init_frame_t {
    unsigned int des_axis_x;		///< Destination x offset
    unsigned int des_axis_y;		///< Destination y offset
    unsigned int memsrc;		///< Source memory address
    unsigned int pu8Display0_addr[3];	///< Distination memory address (Group 0)
    unsigned int pu8Display1_addr[3];	///< Distination memory address (Group 1)
    unsigned int done;			///< Is frame ioctl process done?
};
typedef struct scaler_init_frame_t fscaler_frame;

// Scaler border ioctl parameters
struct scaler_border_param_t {
    unsigned char border_width; 	///< Border width, 0,2,4~14
    unsigned char border_height;        ///< Border height, 0,2,4~14
    unsigned char B_Cb;         	///< B color, 0x0~0x1F
    unsigned char G_Y;          	///< G color, 0x0~0x1F
    unsigned char R_Cr;         	///< R color, 0x0~0x1F
};
typedef struct scaler_border_param_t fscaler_border_param;

// Scaler IE parameters
struct scaler_IE_param_t {
    unsigned char bright_sign;		///< Bright sign
    unsigned char bright;       	///< Brightness, 0x0~0x1F
    unsigned char mx0_sign;		///< Matrix 0 sign
    unsigned int mx0;           	///< Color matrix 0, 0x0~0x7FF
    unsigned char mx1_sign;		///< Matrix 1 sign
    unsigned int mx1;           	///< Color matrix 1, 0x0~0x7FF
    unsigned char mx2_sign;		///< Matrix 2 sign
    unsigned int mx2;           	///< color matrix 2, 0x0~0x7FF
    unsigned char mx3_sign;		///< Matrix 3 sign
    unsigned int mx3;           	///< Color matrix 3, 0x0~0x7FF
    unsigned int gainy;         	///< Y gain, 0x0~0xFF
    unsigned int gainCb;        	///< Cb gain, 0x0~0xFF
    unsigned int gainCr;        	///< Cr gain, 0x0~0xFF
};
typedef struct scaler_IE_param_t fscaler_IE_param;

// Scaler sharpness ioctl parameters
struct scaler_sharpness_param_t {
    unsigned char threshold1;		///< Sharpness threshold 1
    unsigned char gain_factor1;		///< Sharpness gain factor 1
    unsigned char threshold2;		///< Sharpness threshold 2
    unsigned char gain_factor2;		///< Sharpness gain factor 2
};
typedef struct scaler_sharpness_param_t fscaler_sharpness_param;

// Scaler ratio ioctl parameters
struct scaler_ratio_ctl_param_t {
    unsigned char depend;       	///< Are x axis and y axis dependent?
    					///< 0: X and Y independ
    					///< 1: X depend, 2: Y depend
};
typedef struct scaler_ratio_ctl_param_t fscaler_ratio_param;

// Scaler clock ioctl parameters
struct scaler_external_clk_param_t {
    unsigned char invert_pclk;  	///< Invert clock, 0: disable 1: enable
    unsigned char ext_clk;      	///< Clock selection, 0: pclk/sear_a_clk 1: ext_clk
};
typedef struct scaler_external_clk_param_t fscaler_extclk_param;

// Scaler interleave ioctl parameters
struct scaler_interlace_param_t {
    unsigned char field_rate;		///< Filed rate
    unsigned char field_seq;		///< Field sequence
    unsigned char progressive;		///< Profressive
};
typedef struct scaler_interlace_param_t fscaler_inter_param;

#endif /* _FSCALER_API_H_ */
