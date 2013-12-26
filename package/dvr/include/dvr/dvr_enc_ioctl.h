/*! \mainpage GM8126 dvr encode, decode and display ioctl functions
 *
 * dvr_enc_ioctl.h describe dvr encode ioctl functions behavior
 * \n dvr_dec_ioctl.h describe dvr decode ioctl functions behavior
 * \n dvr_disp_ioctl.h describe dvr display ioctl functions behavior
 * \n dvr_common_api.h describe dvr common ioctl functions behavior
 */
 
/**
 *  \example main-bitstream-record.c
 *  \example sub-bitstream-record.c
 *  \example update-record-setting.c
 *  \example mpeg4-record.c
 *  \example mjpeg-record.c
 *  \example snapshot.c
 *  \example motion-detection.c
 *  \example update-bitrate.c
 *  \example playback.c
 *  \example roi.c
 *  \example capture_raw.c
 *  \example liveview.c
 *  \example 2ch_liveview.c
 *  \example 2ch_playback.c
 *  \example motion-detection-mpeg4.c
 *  \example pip.c
*/

#define DVR_ENC_IOC_MAGIC  'B' 

/** 
 * \b ioctl(enc_fd, DVR_ENC_SET_CHANNEL_PARAM, &ch_param) 
 *
 * \arg explanation : get channel parameter from user space ,and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e ch_param : argument from user space ioctl parameter, it means structure dvr_enc_channel_param
 *  
 */
#define DVR_ENC_SET_CHANNEL_PARAM                   _IOWR(DVR_ENC_IOC_MAGIC, 2, dvr_enc_channel_param)

/** 
 * \b ioctl(enc_fd, DVR_ENC_GET_CHANNEL_PARAM, &ch_param) :    
 * 
 * \arg explanation : get channel parameter from user space
 * \arg parameter : 
 * \n \b \e pointer \b \e ch_param : argument from user space ioctl parameter, it means structure dvr_enc_channel_param     
 */
#define DVR_ENC_GET_CHANNEL_PARAM                   _IOWR(DVR_ENC_IOC_MAGIC, 3, dvr_enc_channel_param)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET, &data)
 *
 * \arg explanation : Get buffer to user space. It includes the buffer length, offset.
 * \arg parameter : 
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */
#define DVR_ENC_QUEUE_GET                           _IOWR(DVR_ENC_IOC_MAGIC, 5, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_PUT, &data)
 *
 * \arg explanation : get buffer from user space, and release buffer at videograph layer
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */
#define DVR_ENC_QUEUE_PUT                           _IOWR(DVR_ENC_IOC_MAGIC, 6, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_CONTROL, &enc_ctrl)
 *
 * \arg explanation : get encode contorl command from user space, and set command to videograph layer
 * \arg parameter :
 * \n  \b \e pointer \b \e enc_ctrl : argument from user space ioctl parameter, it means structure dvr_enc_control
 */
#define DVR_ENC_CONTROL                             _IOW(DVR_ENC_IOC_MAGIC, 7, dvr_enc_control)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SIZE, &enc_buf_size)
 * 
 * \arg explanation : get output buffer size for dvr encode to user space.
                      buffer size = main bitstream + sub1 bitstream + sub2 bitstream + snapshot 
 * \arg parameter : 
 * \n \b \e pointer \b \e enc_buf_size : argument from user space ioctl parameter, it means request buffer size.
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SIZE            _IOWR(DVR_ENC_IOC_MAGIC, 8, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SIZE, &enc_buf_size);
 *  
 * \arg explanation : get snapshot buffer to user space
 * \arg parameter :
 * \n \b \e pointer \b \e queue_data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */
#define DVR_ENC_QUEUE_GET_SNAP                      _IOWR(DVR_ENC_IOC_MAGIC, 9, dvr_enc_queue_get)

/**
 * ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SNAP_OFFSET, &bs_buf_snap_offset)
 * 
 * \arg explanation : get output buffer offset for snapshot to user space
 * \arg parameter :
 * \n \b \e pointer \b \e bs_buf_snap_offset : argument from user space ioctl parameter, it means snapshot offset.
 *
 */  
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SNAP_OFFSET     _IOR(DVR_ENC_IOC_MAGIC, 10, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB1_BS, &data) :
 *
 * \arg explanation : get sub1-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB1_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 11, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB2_BS, &data) 
 *
 * \arg explanation : get sub2-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB2_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 12, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB1_BS_OFFSET, &sub1_bs_buf_offset)
 * 
 * \arg explanation : get output buffer offset for Sub1-bitstream to user space 
 * \arg parameter :
 * \n \b \e pointer \b \e sub1_bs_buf_offset : argument from user space ioctl parameter, it means sub1 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB1_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 13, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB2_BS_OFFSET, &sub2_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub2-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub2_bs_buf_offset : argument from user space ioctl parameter, it means sub2 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB2_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 14, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_RESET_INTRA, &stream_num)
 *
 * \arg explanation : get i-frame as possible
 * \arg parameter :
 * \n \b \e pointer \b \e stream_num : argument from user space ioctl parameter, it means structure main-bitstream or sub1-bitstream or sub2-bitstream
 */
#define DVR_ENC_RESET_INTRA                         _IOR(DVR_ENC_IOC_MAGIC, 15, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_SET_SUB_BS_PARAM, &sub_bitstream) 
 *   
 * \arg explanation : get sub-bitstream parameter from user space, and set sub-bitstream parameter to device driver 
 * \arg parameter :
 * \n \b \e pointer \b \e sub_bitstream : argument from user space ioctl parameter, it means structure ReproduceBitStream
 */
#define DVR_ENC_SET_SUB_BS_PARAM                    _IOW(DVR_ENC_IOC_MAGIC, 16, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_SWAP_INTRA, &stream_num)
 * 
 * \arg explanation : get i-frame definitely when next switch
 * \arg parameter :
 * \n \b \e pointer \b \e stream_num : argument from user space ioctl parameter, it means structure main-bitstream or sub1-bitstream or sub2-bitstream
 */
#define DVR_ENC_SWAP_INTRA                          _IOR(DVR_ENC_IOC_MAGIC, 18, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_SUB_PATH_DENOISE_CTRL, &control) 
 *
 * \arg explanation : get denoise option from user space, and set to device driver 
 * \arg parameter :
 * \n \b \e pointer \b \e control : argument from user space ioctl parameter, it means denoise on/off 
  */
#define DVR_ENC_SUB_PATH_DENOISE_CTRL               _IOW(DVR_ENC_IOC_MAGIC, 19, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB3_BS, &data) 
 *
 * \arg explanation : get sub3-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB3_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 20, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB4_BS, &data) 
 *
 * \arg explanation : get sub4-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB4_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 21, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB5_BS, &data) 
 *
 * \arg explanation : get sub5-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB5_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 22, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB6_BS, &data) 
 *
 * \arg explanation : get sub6-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB6_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 23, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB7_BS, &data) 
 *
 * \arg explanation : get sub7-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB7_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 24, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB8_BS, &data) 
 *
 * \arg explanation : get sub8-bitstream buffer to user space. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_queue_get
 */  
#define DVR_ENC_QUEUE_GET_SUB8_BS                   _IOWR(DVR_ENC_IOC_MAGIC, 25, dvr_enc_queue_get)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB3_BS_OFFSET, &sub3_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub3-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub3_bs_buf_offset : argument from user space ioctl parameter, it means sub3 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB3_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 26, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB4_BS_OFFSET, &sub4_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub4-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub4_bs_buf_offset : argument from user space ioctl parameter, it means sub4 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB4_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 27, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB5_BS_OFFSET, &sub5_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub5-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub5_bs_buf_offset : argument from user space ioctl parameter, it means sub5 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB5_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 28, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB6_BS_OFFSET, &sub6_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub6-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub6_bs_buf_offset : argument from user space ioctl parameter, it means sub6 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB6_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 29, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB7_BS_OFFSET, &sub7_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub7-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub7_bs_buf_offset : argument from user space ioctl parameter, it means sub7 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB7_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 30, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUERY_OUTPUT_BUFFER_SUB8_BS_OFFSET, &sub8_bs_buf_offset)
 *   
 * \arg explanation : get output buffer offset for Sub8-bitstream to user space
 * \arg parameter :
 * \n \b \e pointer \b \e sub8_bs_buf_offset : argument from user space ioctl parameter, it means sub8 bitstream buffer offset.
 *
 */
#define DVR_ENC_QUERY_OUTPUT_BUFFER_SUB8_BS_OFFSET  _IOR(DVR_ENC_IOC_MAGIC, 31, int)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_COPY, &data)
 *
 * \arg explanation : Get buffer and copy to user space. It includes the bitstream length.
 * \arg parameter : 
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */
#define DVR_ENC_QUEUE_GET_COPY                      _IOWR(DVR_ENC_IOC_MAGIC, 35, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB1_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB1_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 36, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB2_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB2_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 37, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB3_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB3_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 38, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB4_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB4_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 39, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB5_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB5_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 40, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB6_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB6_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 41, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB7_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB7_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 42, dvr_enc_copy_buf)


/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB8_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SUB8_BS_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 43, dvr_enc_copy_buf)

/**
 * \b ioctl(enc_fd, DVR_ENC_QUEUE_GET_SUB8_BS_COPY, &data) 
 *
 * \arg explanation : get and copy sub8-bitstream to user space buffer. 
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_enc_copy_buf
 */  
#define DVR_ENC_QUEUE_GET_SNAP_COPY              _IOWR(DVR_ENC_IOC_MAGIC, 44, dvr_enc_copy_buf)


