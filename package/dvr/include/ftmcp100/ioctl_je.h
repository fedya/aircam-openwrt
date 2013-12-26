#ifndef __FMJPEG_AVCODEC_h
	#define __FMJPEG_AVCODEC_h

	/*
		Version 2.0: Upgrade to GM Version, add check version type
		Version 2.2: Update for data sync issue
		Version 2.3: Update for data sync issue 2
		Version 3.00: 1. change driver interface (, remove dummy input parameter)
							  2. reduce bitstream buffer (when works with mpeg4-codec/jpeg-decoder)
							  3. improve performance for bitstream copy
		Version 3.10: modify driver interface, structure FJPEG_ENC_PARAM
	 	Version 3.20: provide ReCreate function
	 	Version 3.21: support 1D-420/1D-422 input image format for DMA-wrapper has been configured (ex 8126)
	 	Version 3.22: fix bug of sys_buf_full at 8126 or later version
	  Version 3.23: Support ROI start position may not align at 16 pixels
			a. should be align at (hor. = 8 pxls, ver. = 2 pxls) when u82D = 3
			b. should be align at (hor. = 2 pxls, ver. = 1 pxls) when u82D = 4
            Version 3.2.4: prevent packing header error (wait auto buffer empty when packing quant/huffman table)
                                 support height crop (for encoding real 1080p pattern)
            Version 3.2.5: fix bug: user define quantization table not free
	 */
	#define MJPEGE_VER				0x00030025
	#define MJPEGE_VER_MAJOR	(MJPEGE_VER>>16)
	#define MJPEGE_VER_MINOR	((MJPEGE_VER&0xffff)>>4)
	#define MJPEGE_VER_MINOR2	(MJPEGE_VER&0x000f)

	#define FMJPEG_IOCTL_ENCODE_CREATE		0x5182
	#define FMJPEG_IOCTL_ENCODE_RECREATE	0x5183
	#define FMJPEG_IOCTL_ENCODE_ONE				0x5187
	#define FMJPEG_IOCTL_ENCODE_DEVBUF		0x5194
	#define FMJPEG_IOCTL_ENCODE_QUANTTBL	0x5195
	#define FMJPEG_IOCTL_ENCODE_CURR_QTBL	0x5197

	#define FMJPEG_ENCODER_DEV  "/dev/mjenc" //major:10 minior:61

	typedef enum {		//Y HxV,  U HxV,	V HxV,	u82D = 0,	u82D=1, 	u82D=2, 	u82D=3, 	u82D=4
		JCS_yuv420 = 0,	//2x2,		1x1,		1x1,		support,	support,	support,	support,	support
		JCS_yuv422 = 1,	//4x1,		2x1,		2x1,							support	
		JCS_yuv211 = 2,	//2x1,		1x1,		1x1,							support
		JCS_yuv333 = 3,	//3x1,		3x1,		3x1,							support
		JCS_yuv222 = 4,	//2x1,		2x1,		2x1,							support
		JCS_yuv111 = 5,	//1x1,		1x1,		1x1,							support
		JCS_yuv400 = 6	//1x1,		0x0,		0x0,		support,	support,	support
										//1x1,		0x0,		0x0,		NOT-support if sequencer mode
	} JCS_YUV;


	typedef struct {
		unsigned int u32API_version; // for checking API version
		unsigned int sample;				// JCS_YUV enum, (sequencer not support JCS_yuv400)
		unsigned int u32ImageMotionDetection;

		unsigned int   u32RestartInterval; /**< This variable specifies the number of MCUs between
    	                                  *   restart markers within the compressed data.
        	                              *   If the specified restart interval is zero, then
            	                          *   the restart marker is not used.
                	                      */										
		unsigned int u32ImageWidth;		// The image width in pixels.
		unsigned int u32ImageHeight;	// The image height in pixels.
		unsigned char u82D;					// input image type,
														// 0: MP4 2D, 420p,
														// 1: sequencial 1D,
														// 2: H264 2D, 420p
														// 3: sequencial 1D 420, one case of u82D=1, (only support when DMAWRP is configured)
														// 4: sequencial 1D 422, (only support when DMAWRP is configured)
		//for ROI data structure 
		int roi_x;	// ROI area x-axis start point, unit: pixel, must be multiple of Y sampling width
						// default 0
						// if -1 mean keep the original setting
		int roi_y;	// ROI area y-axis start point, unit: pixel, must be multiple of Y sampling width
						// default 0
						// if -1 mean keep the original setting
		int roi_w;	// ROI's width of input image.
						// default 0
						// if 0 mean roi_w equal to image width
		int roi_h;	// ROI's height of input image.
						// default 0
						// if 0 mean roi_h equal to image height
	} FJPEG_ENC_PARAM;

    typedef struct {
        unsigned int u32CropHeight;
        unsigned int Reserved1;
        unsigned int Reserved2;
        unsigned int Reserved3;
        unsigned int Reserved4;
        unsigned int Reserved5;
        unsigned int Reserved6;
        unsigned int Reserved7;
    } FJPEG_ENC_PARAM_A2;

	typedef struct {
		//[IP]:		means input parameter: 
		//[OP]:	means output parameter: 
		unsigned char *pu8YUVAddr[3];	//[IP]: virtual base address of input YUV frame buffer
		unsigned char *pu8BitstreamAddr;	//[IP]: virtual base address of output bitstream buffer
		unsigned int bitstream_size;			//[OP]: length of output bitstream
		unsigned int u32ImageQuality;		//[IP]: The range for this quality setting is between
															//		1 (worst quality) and 100 (best quality).
		unsigned char u8JPGPIC;				//[IP]: specify this is a JPG or MJPG
															//		1: JPG, (write huffman tables)
															//		0: MJPG, (no write huffman tables to shrink bitstream size)
		//for ROI data structure 
		int roi_x;	// ROI area x-axis start point, unit: pixel, must be multiple of Y sampling width
						// default 0
						// if -1 mean keep the original setting
		int roi_y;	// ROI area y-axis start point, unit: pixel, must be multiple of Y sampling width
						// default 0
						// if -1 mean keep the original setting
		int roi_w;	// ROI's width of input image.
						// default 0
						// if 0 mean roi_w equal to image width
		int roi_h;	// ROI's height of input image.
						// default 0
						// if 0 mean roi_h equal to image height
	} FJPEG_ENC_FRAME;

	typedef struct FJPEG_ENC_QTBLS_T {
		unsigned int luma_qtbl[64];
		unsigned int chroma_qtbl[64];
	} FJPEG_ENC_QTBLS;
#endif
