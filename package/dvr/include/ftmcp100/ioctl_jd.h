#ifndef _IOCTL_JD_H_
	#define _IOCTL_JD_H_

	/*
		Version 2.0: Upgrade to GM Version, add check version type
		Version 2.2: Update for data sync issue
		Version 2.3: Update for data sync issue 2

		Version 3.00: 1. change driver interface (, remove dummy input parameter)
							2. fix hand on when bitstream error (missing restart marker)
							3. add cropping function (see FMJPEG_IOCTL_DECODE_DISPSET)
							4. support multiple format (pls see enum JDS_YUV)
							5. merge into mcp_drv.ko (original mcp420_drv, mcp422_drv)
							6. reduce bitstream buffer (when works with mpeg4-codec/jpeg-encoder)
							7. improve performance for bitstream copy
							8. add reading JPEG file header for AP
	 Version 3.10: provide ReCreate function
	 Version 3.11: fix bug of sys_buf_empty at 8126 or later version
	 Version 3.12: support image width & height not to be necessary with the multiple of sampling
	 Version 3.13: fix bug of to allocate memory
				   add work queue to avoid allocate memory within interrupt
	 Version 3.1.4: add function to transform planar 422 to packed 422 (SEQ mode)
	 */
	#define MJPEGD_VER				0x00030014
	#define MJPEGD_VER_MAJOR	(MJPEGD_VER>>16)
	#define MJPEGD_VER_MINOR	((MJPEGD_VER&0xffff)>>4)
	#define MJPEGD_VER_MINOR2	(MJPEGD_VER&0x000f)

	#define FMJPEG_IOCTL_DECODE_CREATE		0x5171
	#define FMJPEG_IOCTL_DECODE_DISPSET		0x5172		// not support at SEQUENCER mode
	#define FMJPEG_IOCTL_DECODE_RECREATE	0x5173
	#define FMJPEG_IOCTL_DECODE_CREATE_PA2		0x5174
	#define FMJPEG_IOCTL_DECODE_ONE				0x5178
	#define FMJPEG_IOCTL_DECODE_PASSHUF 	0x5180

	#define FMJPEG_DECODER_DEV  "/dev/mjdec" //major:10 minior:60

	typedef struct {
		unsigned int u32API_version; // for checking API version
		unsigned int frame_width;
		unsigned int frame_height;
		unsigned int output_format; 	/*< This value is use to define output format
						* OUTPUT_YUV=0,   			// for all sampling
						* OUTPUT_420_YUV=1,		// only for 420, 422_0, 422_1
						* OUTPUT_CbYCrY=2,		// only for 420, 422_0, 422_1		*/
	} FJPEG_DEC_PARAM;
	typedef struct {
		// input
		unsigned short buf_width;			// default: equal to frame_width in FJPEG_DEC_PARAM
		unsigned short buf_height;			// default: equal to frame_height in FJPEG_DEC_PARAM
		unsigned int reserved1;
		unsigned int reserved2;
		unsigned int reserved3;
	}FJPEG_DEC_PARAM_A2;


											//										|		support output format:	
	typedef enum {					// YUV sample		interleave	|	YUV	420_YUV		CbYCrY
		JDS_420=0x00,			// 0x00221111	yes			|	Y			Y					Y
		JDS_422_0=0x01,		// 0x00412121	yes			|	Y			Y					Y
		JDS_422_1=0x02,		// 0x00211111	yes			|	Y			Y					Y
		JDS_444_0=0x03,		// 0x00313131	yes			|	Y
		JDS_444_1=0x04,		// 0x00212121	yes			|	Y
		JDS_444_2=0x05,		// 0x00111111	yes			|	Y

		JDS_422_2=0x06,		// 0x00222121	yes			|	Y
		JDS_422_3=0x07,		// 0x00121111	yes			|	Y
		JDS_444_3=0x08,		// 0x00121212	yes			|	Y
		JDS_400=0x09,    		// 0x00000011	yes			|	Y

		JDS_420n=0x10,			// 0x00221111	no				|	Y			Y
		JDS_422n_0=0x11,		// 0x00412121	no				|	Y			Y
		JDS_422n_1=0x12,		// 0x00211111	no				|	Y			Y
		JDS_444n_0=0x13,		// 0x00313131	no				|	Y
		JDS_444n_1=0x14,		// 0x00212121	no				|	Y
		JDS_444n_2=0x15,		// 0x00111111	no				|	Y

		JDS_422n_2=0x16,		// 0x00222121	no				|	Y
		JDS_422n_3=0x17,		// 0x00121111	no				|	Y
		JDS_444n_3=0x18		// 0x00121212	no				|	Y
	} JDS_YUV;

	typedef enum {
		JDE_OK=0,
		JDE_SYSERR=-1,			// system error, ex: malloc err
		JDE_BSERR=-2,				// bitstream error
		JDE_USPERR=-3,			// support error, unsupport format
		JDE_SETERR=-4				// SETTING error, ex: output format, crop_parameter
	} JD_ERROR;

	typedef struct {
		// input
		unsigned char * pu8YUVAddr[3];		// mean parsing header only if leave pu8YUVAddr[0] as NULL
		unsigned char * buf;
		unsigned int buf_size;
		// output
		unsigned int NumofComponents;
		unsigned int img_width;
		unsigned int img_height;
		unsigned int sample;				// JDS_YUV enum
		int err;									// JD_ERR enum
	}FJPEG_DEC_FRAME;

	typedef struct {
		// keep the followings as 0s if you don't use cropping function.
		unsigned int crop_x;		// cropping area x-axis start point, unit: pixel, must be multiple of Y sampling width
											// default 0
											// if 0xFFFFFFFF mean keep the original setting
		unsigned int crop_y;		// cropping area y-axis start point, unit: pixel, must be multiple of Y sampling height
											// default 0
											// if 0xFFFFFFFF mean keep the original setting
		unsigned int crop_w;		// cropping area width, unit: pixel, must be multiple of Y sampling width
											// default 0
											// if 0xFFFFFFFF mean keep the original setting
											// if 0 mean crop_w equal to image width
		unsigned int crop_h;		// cropping area height, unit: pixel, must be multiple of Y sampling height
											// default 0
											// if 0xFFFFFFFF mean keep the original setting
											// if 0 mean crop_h equal to image height
		unsigned int output_format; 	// same as FJPEG_DEC_PARAM's output_format,
													// if 0xFFFFFFFF mean keep the original setting
	}FJPEG_DEC_DISPLAY;
#endif
