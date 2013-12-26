
#define DVR_DISP_IOC_MAGIC  'I'

/** 
 * \b ioctl(disp_fd, DVR_DISP_INITIATE, 0) 
 *
 * \arg explanation : not used 
 *  
 */
#define DVR_DISP_INITIATE           _IO(DVR_DISP_IOC_MAGIC, 1)

/** 
 * \b ioctl(disp_fd, DVR_DISP_TERMINATE, 0) 
 *
 * \arg explanation : not used 
 *  
 */
#define DVR_DISP_TERMINATE          _IO(DVR_DISP_IOC_MAGIC, 2)

/** 
 * \b ioctl(disp_fd, DVR_DISP_GET_DISP_PARAM, &disp_param) 
 *
 * \arg explanation : get LCD color parameter from user space ,and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e disp_param : argument from user space ioctl parameter, it means structure dvr_disp_disp_param
 *  
 */
#define DVR_DISP_GET_DISP_PARAM     _IOWR(DVR_DISP_IOC_MAGIC, 4, dvr_disp_disp_param)

/** 
 * \b ioctl(disp_fd, DVR_DISP_SET_DISP_PARAM, &disp_param) 
 *
 * \arg explanation : get LCD color parameter from user space ,and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e disp_param : argument from user space ioctl parameter, it means structure dvr_disp_disp_param
 *  
 */
#define DVR_DISP_SET_DISP_PARAM     _IOWR(DVR_DISP_IOC_MAGIC, 5, dvr_disp_disp_param)

/** 
 * \b ioctl(disp_fd, DVR_DISP_UPDATE_DISP_PARAM, &disp_update_param) 
 *
 * \arg explanation : get LCD color parameter from user space ,and update parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e disp_update_param : argument from user space ioctl parameter, it means structure dvr_disp_update_disp_param
 *  
 */
#define DVR_DISP_UPDATE_DISP_PARAM  _IOWR(DVR_DISP_IOC_MAGIC, 6, dvr_disp_update_disp_param)

/** 
 * \b ioctl(disp_fd, DVR_DISP_GET_PLANE_PARAM, &plane_param) 
 *
 * \arg explanation : get LCD plane(window) parameter from user space, and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e plane_param : argument from user space ioctl parameter, it means structure dvr_disp_plane_param
 *  
 */
#define DVR_DISP_GET_PLANE_PARAM    _IOWR(DVR_DISP_IOC_MAGIC, 7, dvr_disp_plane_param)

/** 
 * \b ioctl(disp_fd, DVR_DISP_SET_PLANE_PARAM, &plane_param_set) 
 *
 * \arg explanation : get LCD plane(window) parameter from user space, and set parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e plane_param_set : argument from user space ioctl parameter, it means structure dvr_disp_plane_param
 *  
 */
 #define DVR_DISP_SET_PLANE_PARAM    _IOWR(DVR_DISP_IOC_MAGIC, 8, dvr_disp_plane_param)

/** 
 * \b ioctl(disp_fd, DVR_DISP_UPDATE_PLANE_PARAM, &plane_param_update) 
 *
 * \arg explanation : get LCD plane(window) parameter from user space, and update parameter to device driver
 * \arg parameter : 
 * \n \b \e pointer \b \e plane_param_update : argument from user space ioctl parameter, it means structure dvr_disp_update_plane_param
 *  
 */
#define DVR_DISP_UPDATE_PLANE_PARAM _IOWR(DVR_DISP_IOC_MAGIC, 9, dvr_disp_update_plane_param) 

/** 
 * \b ioctl(disp_fd, DVR_DISP_CONTROL, &disp_ctrl) 
 *
 * \arg explanation : get display control command from user space, and set command to videograph layer
 * \arg parameter : 
 * \n \b \e pointer \b \e disp_ctrl : argument from user space ioctl parameter, it means structure dvr_disp_control
 *  
 */
#define DVR_DISP_CONTROL            _IOR(DVR_DISP_IOC_MAGIC, 10, dvr_disp_control)

/** 
 * \b ioctl(disp_fd, DVR_DISP_INITIATE, &disp_clear_param) 
 *
 * \arg explanation : get channel parameter from user space ,and clear lcd buffer
 * \arg parameter : 
 * \n \b \e pointer \b \e disp_clear_param; : argument from user space ioctl parameter, it means structure dvr_disp_clear_param
 *  
 */
#define DVR_DISP_CLEAR_WIN          _IOWR(DVR_DISP_IOC_MAGIC, 11, dvr_disp_clear_param) 
