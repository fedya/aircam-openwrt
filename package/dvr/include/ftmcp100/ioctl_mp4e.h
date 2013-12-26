#ifndef _IOCTL_MP4E_H_
	#define _IOCTL_MP4E_H_

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
							2. support non-integer frame rate
							3. reduce bitstream buffer (when works with mpeg4-encoder/jpeg-codec)
							4. improve performance for bitstream copy
							5. play with mp4_ratecontrol.c (when rate-control is needed)
							6. play with get_fr.c (when non-integer-frame-rate is needed)
		Version 3.10:	1. change driver interface (, change parameter name and type)
								2. support Video-Graph
		Version 3.20: 1. provide ReInit function
							   2. change driver interface (, add two elements at structure FMP4_ENC_PARAM)
		Version 3.21: 1. provide FMPEG4_IOCTL_ENCODE_INIT_264 function for H264-2D input format
							   2. provide FMPEG4_IOCTL_ENCODE_INIT_WRP function for all input format (only for GM8126)
	 	Version 3.22: expand time for wait_hw_encode depend on encoded resolution
	 	Version 3.23: fix bug when using FMPEG4_IOCTL_ENCODE_REINIT with different resolutions & ROI disabled
	  Version 3.24: Support ROI start position may not align at 16 pixels
		 a. should be align at (hor. = 8 pxls, ver. = 2 pxls) when input image format is RASTER_SCAN_420
		 b. should be align at (hor. = 2 pxls, ver. = 1 pxls) when input image format is RASTER_SCAN_422
		 Version 3.25: support whether to output mbinfo or not
		 Version 3.26: support height crop (for encoding real 1080p pattern)
		 Version 3.2.7: adaptive IP interval & disable intra in P frame & maxloop to keep frame rate
		 Version 3.2.8: add preoperty to force MPG4 header the same as 8120
	 */
	#define MP4E_VER					0x00030028
	#define MP4E_VER_MAJOR		(MP4E_VER>>16)
	#define MP4E_VER_MINOR		((MP4E_VER&0xffff)>>4)
	#define MP4E_VER_MINOR2		(MP4E_VER&0x000f)

	#define FMPEG4_IOCTL_ENCODE_INIT    0x4173					// mp4_2d input format
	#define FMPEG4_IOCTL_ENCODE_REINIT	0x4174
	#define FMPEG4_IOCTL_ENCODE_FRAME   0x4175
	#define FMPEG4_IOCTL_ENCODE_INFO		0x4176
	#define FMPEG4_IOCTL_ENCODE_FRAME_NO_VOL 0x4178
	#define FMPEG4_IOCTL_SET_INTER_QUANT	0x4184
	#define FMPEG4_IOCTL_SET_INTRA_QUANT	0x4185
	#define FMPEG4_IOCTL_ENCODE_INIT_264		0x4171		// h264_2d input format
	#define FMPEG4_IOCTL_ENCODE_INIT_WRP	0x4172		// support all input format

	#define FMPEG4_ENCODER_DEV  "/dev/fenc"

    /* Structure used to pass a frame to the encoder */
	typedef struct fmpeg4_enc_frame {
		void *bitstream;                   /*< [out]
    										*
    										* Output MPEG4 bitstream buffer pointer (physical addr.) */
		int length;                        /*< [out]
    										*
    										* Output MPEG4 bitstream length (bytes) */

		unsigned char *quant_intra_matrix; /*< [in]
    										*
    										* Custom intra quantization matrix when MPEG4 quant is enabled*/
		unsigned char *quant_inter_matrix; /*< [in]
    										*
    										* Custom inter quantization matrix when MPEG4 quant is enabled */

		int quant;                         /*< [in]
 		   									*
    										* Frame quantizer :
    										* <li> !=  0  :  Then you  force  the  encoder  to use  this  specific
    										*                  quantizer   value.     It   is   clamped    in   the   interval
    										*                  [1..31]. Tipically used  during the 2nd pass of  a VBR encoding
    										*                  session. 
    										* </ul> */
		int intra;                         /*< [in/out]
    										*
    										* <ul>
    										* <li> [in] : tells Gm if the frame must be encoded as an intra frame
    										*     <ul>
    										*     <li> 1: forces the encoder  to create a keyframe. Mainly used during
    										*              a VBR 2nd pass.
    										*     <li> 0:  forces the  encoder not to  create a keyframe.  Minaly used
    										*               during a VBR second pass
    										*     <li> -1: let   the  encoder   decide  (based   on   contents  and
    										*              max_key_interval). Mainly  used in ABR  mode and during  a 1st
    										*              VBR pass. 
    										*     </ul>
    										* <li> [out] : When first set to -1, the encoder returns the effective keyframe state
    										*              of the frame. 
    										* </ul>
                                    	*/
    
		/// The base address for input Y frame buffer.
		unsigned char *pu8YFrameBaseAddr;  /**< To set input Y frame buffer's base address.\n
        		                      *   <B>N.B.</B> : the input frame buffer address must be <B>physical address</B> with <B>8-byte aligned</B>.
                		              *   @see pu8UFrameBaseAddr
                        		      *   @see pu8VFrameBaseAddr
		                              *
        		                      *   Also, this variable can be set by utilizing the function FMpeg4EncSetYUVAddr().
                		              *   @see FMpeg4EncSetYUVAddr
                        		      */
		/// The base address for input U frame buffer.	
		unsigned char *pu8UFrameBaseAddr;  /**< To set input U frame buffer's base address.\n
        		                      *   <B>N.B.</B> : the input frame buffer address must be <B>physical address</B> with <B>8-byte aligned</B>.
                		              *   @see pu8YFrameBaseAddr
                        		      *   @see pu8VFrameBaseAddr
		                              *
        		                      *   Also, this variable can be set by utilizing the function FMpeg4EncSetYUVAddr().
                		              *   @see FMpeg4EncSetYUVAddr
                        		      */
		/// The base address for input V frame buffer.
		unsigned char *pu8VFrameBaseAddr;  /**< To set input V frame buffer's base address.\n
        		                      *   <B>N.B.</B> : the input frame buffer address must be <B>physical address</B> with <B>8-byte aligned</B>.
                		              *   @see pu8YFrameBaseAddr
                        		      *   @see pu8UFrameBaseAddr
		                              *
        		                      *   Also, this variable can be set by utilizing the function FMpeg4EncSetYUVAddr().
                		              *   @see FMpeg4EncSetYUVAddr
                        		      */
		int    frameindex;     //the frame index for motion dection
		int    roi_x;	// ROI area x-axis start point, unit: pixel
					// default 0
					// if -1 mean keep the original setting
		int    roi_y;	// ROI area x-axis start point, unit: pixel
					// default 0
					// if -1 mean keep the original setting
		int module_time_base; //VOP header module_time_base, using magic number 0x55aaXXXX, XXXX is the number
	} FMP4_ENC_FRAME;

	typedef struct {
		unsigned int u32API_version;  	// this value is to check the driver version
		unsigned int u32MaxWidth;		// The MAX width of encoded frame in pels.
		unsigned int u32MaxHeight;		// The MAX height of encoded frame in pels.
		unsigned int u32FrameWidth;		// The width of encoded frame in pels.
		unsigned int u32FrameHeight;	// The height of encoded frame in pels.
		/// To enable the function of encoding rectangular region of interest(ROI) within captured frame.
		int bROIEnable;  // if 0: To disable the function of encoding rectangular region of interest.
									// discard the following setting roi_x, roi_y, roi_w, roi_h
									// if 1: To enable the function of encoding rectangular region of interest.

		/// The upper-left corner x coordinate of rectangular region of interest within captured frame.
		int roi_x;		// if < 0, keep original value
									// default: 0
		/// The upper-left corner coordinate y of region of interest within captured frame.
		int roi_y;		// if < 0, keep original value
									// default: 0
		/// The width of user-defined rectangular region of interest within the captured frame in pixel units.
		int roi_w; 		// if < 0, keep original value
									// default: 0
									// if == 0, auto set to frame width
									// multiple of 16 pixels
		/// The height of user-defined rectangular region of interest within the captured frame in pixel units.
		int roi_h; 		// if < 0, keep original value
									// default: 0
									// if == 0, auto set to frame height
									// multiple of 16 pixels

		unsigned int fincr;		// frame rate = fbase/fincr
		unsigned int fbase;

		/// The frame interval between I-frames.
		unsigned int   u32IPInterval;    /**< This variable was used to set the interval between I-frames.
        		                            */
		/// The short header (H263) mode.
		int            bShortHeader;     /**< A flag of enabling short header (H.263) mode or not.
        		                            *   - 0: disable short header.
                		                    *   - 1: enable short header.
                        		            */
		/// The 4 motion vectors (4MV) mode.
		int            bEnable4MV;       /**< To select 4MV (4 motion vectors) mode or 1MV (1 motion vector) mode.
        		                            *   - 0: disable 4MV mode and use 1MV (1 motion vector) mode instead.
                		                    *   - 1: enable 4MV mode and select 4MV mode.
                        		            */
		/// The H.263 quantization method.		                            
		int            bH263Quant;       /**< To select H.263 quantization method or MPEG4 quantization method.
        		                            *   - 0: select MPEG4 quantization method.
                		                    *   - 1: select H.263 quantization method.
                        		            *   - 2: select MPEG4 user defined quant tbl "quant.txt"
                                		    */
		/// The resync marker option.
		int            bResyncMarker;    /**< A flag of enabling resync marker mechanism or not.
        		                            *   - 0: disable resync marker.
                		                    *   - 1: enable resync marker.
                        		            */
		int   ac_disable;  /** < This variable specifies the AC prediction Disable. */
	} FMP4_ENC_PARAM;

    typedef struct {
        unsigned int u32CropHeight;
        unsigned int u32Force8120;
        unsigned int Reserved2;
        unsigned int Reserved3;
        unsigned int Reserved4;
        unsigned int Reserved5;
        unsigned int Reserved6;
        unsigned int Reserved7;
    } FMP4_ENC_PARAM_A2;

	typedef struct {
		FMP4_ENC_PARAM enc_p;
		int img_fmt; 		// 0: 2D format, CbCr interleave, named H264_2D
									// 1: 2D format, CbCr non-interleave, named MP4_2D
									// 2: 1D format, YUV420 planar, named RASTER_SCAN_420
									// 3: 1D format, YUV420 packed, named RASTER_SCAN_422
									// others: no allowed
	} FMP4_ENC_PARAM_WRP;
	#ifndef DEFINE_VECTOR
		#define DEFINE_VECTOR
		union VECTOR1 {
			uint32_t u32num;
			struct {
				int16_t s16y;
				int16_t s16x;
			} vec;
		};
	#endif

	typedef struct {
		int16_t quant;  /**< This variable indicates the macroblock level quantization value (1 ~ 31)
			*/
		int16_t mode;  /**< This variable indicates the mode of macroblock.
			*   Possible values are :
			*   <ul>
			*     <li> == INTER_MODE   : Indicate that this macroblock is encoded in INTER mode
			*                                   with 1MV.
			*     <li> == INTER4V_MODE : Indicate that this macroblock is encoded in INTER mode
			*                                   with 4MV.
			*     <li> == INTRA_MODE   : Indicate that this macroblock is encoded in INTRA mode.
			*   </ul>
			*
			*/
		union VECTOR1 mvs[4];/*
			*   - when mode = INTER_MODE :
			*			mvs[n]
			*			--n = 0 ~ 2: invalid.
			*			--n = 3: indicates the motion vector of whole macroblock.
			*   - when mode = INTER4V_MODE :
			*			mvs[n]
			*			--n = 0 ~3: indicates the motion vector of block n within this macroblock.
			*   - when mode = INTRA_MODE :
			*			mvs[n]
			*			--n = 0 ~ 3: invalid.
			*/
		int32_t sad16;        // SAD value for inter-VECTOR
	} MACROBLOCK_INFO;

#endif
