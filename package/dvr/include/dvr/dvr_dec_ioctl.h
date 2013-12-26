
#define DVR_DEC_IOC_MAGIC   'H'

/** 
 * \b ioctl(dec_fd, DVR_DEC_SET_CHANNEL_PARAM, &ch_param) 
 *
 * \arg explanation : get channel parameter from user space ,and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e ch_param : argument from user space ioctl parameter, it means structure dvr_dec_channel_param
 *  
 */
#define DVR_DEC_SET_CHANNEL_PARAM           _IOWR(DVR_DEC_IOC_MAGIC, 2, dvr_dec_channel_param) 

/** 
 * \b ioctl(dec_fd, DVR_DEC_GET_CHANNEL_PARAM, &ch_param) 
 * 
 * \arg explanation : get channel parameter from user space
 * \arg parameter : 
 * \n \b \e pointer \b \e ch_param : argument from user space ioctl parameter, it means structure dvr_dec_channel_param     
 */
#define DVR_DEC_GET_CHANNEL_PARAM           _IO(DVR_DEC_IOC_MAGIC, 3)

/**
 * \b ioctl(dec_fd, DVR_DEC_QUEUE_GET, &data)
 *
 * \arg explanation : Get buffer to user space. It includes the buffer length, offset.
 * \arg parameter : 
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_dec_queue_get
 */
#define DVR_DEC_QUEUE_GET                   _IOWR(DVR_DEC_IOC_MAGIC, 5, dvr_dec_queue_get)

/**
 * \b ioctl(dec_fd, DVR_DEC_QUEUE_PUT, &data)
 *
 * \arg explanation : get buffer from user space, and release buffer at videograph layer
 * \arg parameter :
 * \n \b \e pointer \b \e data : argument from user space ioctl parameter, it means structure dvr_dec_queue_get
 */
#define DVR_DEC_QUEUE_PUT                   _IOWR(DVR_DEC_IOC_MAGIC, 6, dvr_dec_queue_get)

/**
 * \b ioctl(dec_fd, DVR_DEC_CONTROL, &dec_ctrl)
 *
 * \arg explanation : get decode contorl command from user space, and set command to videograph layer
 * \arg parameter :
 * \n  \b \e pointer \b \e dec_ctrl : argument from user space ioctl parameter, it means structure dvr_dec_control
 */
#define DVR_DEC_CONTROL                     _IOW(DVR_DEC_IOC_MAGIC, 7, dvr_dec_control)

/**
 * \b ioctl(dec_fd, DVR_DEC_QUERY_OUTPUT_BUFFER_SIZE, &dec_buf_size)
 * 
 * \arg explanation : get output buffer size for dvr decode to user space.                      
 * \arg parameter : 
 * \n \b \e pointer \b \e dec_buf_size : argument from user space ioctl parameter, it means request buffer size.
 */
#define DVR_DEC_QUERY_OUTPUT_BUFFER_SIZE    _IOWR(DVR_DEC_IOC_MAGIC, 8, int)

/**
 * \b ioctl(dec_fd, DVR_DEC_CLEAR_WIN, &dec_clear_param)
 * 
 * \arg explanation : to prevent lcd unexpected image, when stop playback, then start another playback, 
 * \n use flow : ioctl(dvr_fd,DVR_COMMON_APPLY,&data);   //stop current playback
 * \n            ................
 * \n            ioctl(dec_fd,DVR_DEC_CLEAR_WIN,&data);  //must after stop, and before start apply
 * \n            ioctl(dvr_fd,DVR_COMMON_APPLY,&data);   //start new playback
 * \arg parameter : 
 * \n \b \e pointer \b \e dec_clear_param : argument from user space ioctl parameter, it means structure dvr_dec_clear_param.
 * 
 */
#define DVR_DEC_CLEAR_WIN                   _IOW(DVR_DEC_IOC_MAGIC, 18, dvr_dec_clear_param)

/**
 * \b ioctl(dec_fd, DVR_DEC_CLEAR_WIN2, &dec_clear_param)
 * 
 * \arg explanation : to prevent lcd unexpected image, when update current playback
 * \n use flow :  dec_ctrl.command = DEC_UPDATE;
 * \n             ioctl(dec_fd, DVR_DEC_CONTROL, &dec_ctrl);
 * \n             ioctl(dec_fd, DVR_DEC_CLEAR_WIN2, &data); // tell VG to clear buffer after apply
 * \n             ioctl(dvr_fd, DVR_COMMON_APPLY, &data);
 * \arg parameter : 
 * \n \b \e pointer \b \e dec_clear_param : argument from user space ioctl parameter, it means structure dvr_dec_clear_param.
 * 
 */
#define DVR_DEC_CLEAR_WIN2                  _IOW(DVR_DEC_IOC_MAGIC, 19, dvr_dec_clear_param)

/**
 * \b ioctl(dec_fd, DVR_DEC_PUT_COPY, &dvr_dec_copy_buf)
 *
 * \arg explanation : get bistream data from user sapce, and release buffer at videograph layer
 * \arg parameter :
 * \n \b \e pointer \b \e dvr_dec_copy_buf : argument from user space ioctl parameter, it means structure dvr_dec_copy_buf.
 */
#define DVR_DEC_PUT_COPY                    _IOWR(DVR_DEC_IOC_MAGIC, 20, dvr_dec_copy_buf)

