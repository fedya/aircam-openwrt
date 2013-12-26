#ifndef _IOCTL_MP4D_H_
	#define _IOCTL_MP4D_H_

	/*
		Version 1.0: Add H263 support, fix encoder ME copy timming issue
		Version 1.1: Add motion dection and ROI setting
		Version 1.2: optimized motion dection
        	     fix consistent_free parameter bug
		Version 1.3: add key check function
		Version 1.5: Update architecture
		Version 1.6: Update from TC for GM8180
		Version 1.9: Add module_time_base
		Version 2.0: Upgrade to GM Version, add check version type
		Version 2.1: Add special mpeg4 deinterlace display feature for GM8120
		Version 2.2: Update for data sync issue
		Version 2.3: Update for data sync issue 2
		Version 2.4: Add MPEG4 decoder cropping function

		Version 3.0: 1. change driver interface (, remove dummy input parameter)
							2. add ioctl for cropping (FMPEG4_IOCTL_DECODE_DISPLAY)
							3. reduce bitstream buffer (when works with mpeg4-encoder/jpeg-codec)
							4. improve performance for bitstream copy
		Version 3.10: 1. change driver interface (, add one element "output_image_format" at structure FMP4_DEC_PARAM)
							  2. remove FMPEG4_IOCTL_DECODE_422, (this function had merged into FMPEG4_IOCTL_DECODE_INIT)
	 	Version 3.20: provide ReInit function
	 	Version 3.21: expand time for wait_hw_decode depend on decoded resolution
	 	Version 3.2.2: change remain bitstream threshold from 1 to 4 because AVI need 4-byte align
	 */
	#define MP4D_VER          0x00030022
	#define MP4D_VER_MAJOR    (MP4D_VER>>16)
	#define MP4D_VER_MINOR    ((MP4D_VER&0xfff0)>>4)
	#define MP4D_VER_MINOR2    (MP4D_VER&0x000f)

	#define FMPEG4_IOCTL_DECODE_INIT    		0x4170
	#define FMPEG4_IOCTL_DECODE_REINIT	0x4171
	#define FMPEG4_IOCTL_DECODE_FRAME  0x4172
	#define FMPEG4_IOCTL_DECODE_DISPLAY   0x4176

	#define FMPEG4_DECODER_DEV  "/dev/fdec"
	typedef struct {
    	unsigned int bs_buf;     				//input bit stream VA address
	    unsigned int bs_length;
	    unsigned int output_va_y;      //output frame Y VA base address or RGB address
    	unsigned int output_va_u;      //output frame U VA base address
	    unsigned int output_va_v;      //output frame V VA base address
    	unsigned int got_picture;
	} FMP4_DEC_FRAME;

	typedef struct {
		// keep the followings as 0s if you don't use cropping function.
		unsigned int crop_x;		// cropping area x-axis start point, unit: pixel, must be multiple of 16
											// default 0
											// if 0xFFFFFFFF mean keeip the original setting
		unsigned int crop_y;		// cropping area y-axis start point, unit: pixel, must be multiple of 16
											// default 0
											// if 0xFFFFFFFF mean keeip the original setting
		unsigned int crop_w;		// cropping area width, unit: pixel, must be multiple of 16
											// default 0
											// if 0xFFFFFFFF mean keeip the original setting
		unsigned int crop_h;		// cropping area height, unit: pixel, must be multiple of 16
	} FMP4_DEC_DISPLAY;

	typedef struct {
	    unsigned int u32API_version;
    	unsigned int u32MaxWidth;
	    unsigned int u32MaxHeight;
    	unsigned int u32FrameWidth;			// output frame width, no matter decoded image size
    							// "set to 0", means equal to decoded image width
	    unsigned int u32FrameHeight;		// output frame height, no matter decoded image size
    							// "set to 0", means equal to decoded image height

	    unsigned int u32EnableDeinterlace; // to enable our special deinterlace function for GM8120
    	                               // And two limitations are required for this deinterlace feature :
        	                           //   - The number of macroblock rows must be even number.
            	                       //   - The image width and height must be equal to the display width and height.
		unsigned int output_image_format;	 // if 0, OUTPUT_FMT_CbYCrY
																	 // if 4, OUTPUT_FMT_YUV
																	 // if other, not allowed
	} FMP4_DEC_PARAM;
#endif
