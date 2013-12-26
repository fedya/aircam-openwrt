/** @file FRAMMAP.c
 *  @brief the file provides the APIs to other modules for memory allocation/release.
 *  @author Harry Hsu
 *  @version: 1.0
 *  @date: 2010/9/24
 *
 */
 
/**
 * @brief Allocate a buffer from the designated DDR. It is a pair of frm_free_buf_ddr().
 * @brief Note: the return value info->size will be updated according to the real allocated size.
 * 
 * @parm ddr_id is used to indicate the designated DDR
 *
 * @parm info->size in info is necessary.
 * @return  0 for success, otherwise for fail.
 */
int frm_get_buf_ddr(DDR_ID_T ddr_id, struct frammap_buf_info *info)
{
    /* this function is extracted from frmmap.c */
}

/**
 * @brief De-allocate a buffer from the designated DDR. It is a pair of frm_get_buf_ddr()
 *
 * @parm Both info->va_addr and info->phy_addr are necessary.
 * @return  0 for success, otherwise for fail.
 */ 
int frm_free_buf_ddr(struct frammap_buf_info *info)
{
    /* this function is extracted from frmmap.c */
}

/**
 * @brief De-allocate a buffer. It is a pair of frm_release_buf_info(). 
 * @brief Note: The return value info->size will be updated according to the real allocated size.
 *
 * @parm id indicates the unique module id given by FRAMMAP. 
 * @parm info->size indicates the allocated memory size.
 *
 * @return  0 for success, otherwise for fail.
 */
int frm_get_buf_info(u32 id, struct frammap_buf_info *info)
{
    /* this function is extracted from frmmap.c */
}

/**
 * @brief De-allocate a buffer. It is a pair of frm_get_buf_info().
 *
 * @parm id indicates the unique module id given by FRAMMAP. Besides, both info->va_addr and
 *          info->phy_addr are necessary.
 *
 * @return  0 for success, otherwise for fail.
 */  
int frm_release_buf_info(u32 id, struct frammap_buf_info *info)
{
    /* this function is extracted from frmmap.c */
}

/**
 * @brief Show the DDR usage status and memory allocation distribution.
 *
 * @parm No input parameter
 *
 * @return None.
 */  
void show_ddr_info(void)
{
}

/** 
 * @brief Get memory allocation alignment.
 * 
 * @parm No input parameter.
 *
 * @return return the memory alignment. It is not only suitable for system memory, but also for 
 *         all management DDR memory.
 */
u32 frm_get_sysmem_alignment(void)
{
    /* this function is extracted from frmmap.c */
}

/**
 * @brief FRM_IOGBUFINFO is an ioctl.
 *
 * @parm meminfo gives the maximum memory block can be allocated from AP once.
 *
 * @return  0 for success, otherwise for fail.
 */
int FRM_IOGBUFINFO(frmmap_meminfo_t meminfo)
{
    /* this function is extracted from frmmap.c */
}

