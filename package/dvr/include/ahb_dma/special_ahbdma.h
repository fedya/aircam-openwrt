/*! \mainpage GM8126/GM812X Special AHB DMA API Reference
 *
 * \n special_ahbdma.h describe Special AHB DMA ioctl functions behavior
 */

/**
 *  \example s_dma_test.c
*/
#ifndef _SPECIAL_DMA_H_
#define _SPECIAL_DMA_H_
#include <linux/ioctl.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

void move_2D(unsigned char channel_num, unsigned char channel_req, unsigned int src_pa_addr, unsigned int dst_pa_addr,
             unsigned int src_pitch, unsigned int dst_pitch, unsigned int width, unsigned int height, unsigned char pixel_byte, unsigned char llp_mode);
             
void fill_2D(unsigned char channel_num, unsigned char channel_req, unsigned int dst_pa_addr, unsigned int pitch,
             unsigned int width, unsigned int height, unsigned char pixel_byte, unsigned int data);

/**
 * @brief special AHB DMA argument and address
 */
typedef struct
{
    unsigned int *src_va_addr;///< source virtual address
    unsigned int *src_pa_addr;///< source physical address
    unsigned int *dst_pa_addr;///< dest physical address
    unsigned int src_pitch;///< pitch of src window
    unsigned int dst_pitch;///< pitch of dst window
    unsigned int width;///< move 2D window width
    unsigned int height;///< move 2D window height    
    unsigned int pattern;///< if we set YUV422 mode and pixel_byte is 2 bytes, pattern can set 0x2345 or 0x5678...
    unsigned int size;///< data size we want to change
    unsigned char pixel_byte;///< one pixel need N bytes, YUV422 is 2 bytes, RGB888 is 3 bytes
    unsigned char llp_mode;///< for copy 2D function 
                           ///< llp_mode = 0, move sequencial data to 2D address
                           ///< llp_mode = 2, move 2D data to 2D address    											 
    unsigned char no_wait;///< wait DMA ready set 0, not wait set 1	    
} ahb_trans_t;       
                                  
/* Use 'p' as magic number */
#define S_DMA_IOC_MAGIC  'p'

/**
 * \b ioctl(s_dma_fd, FSDMA_COPY_SEQUENTIAL, &tag)
 *
 * \arg From User mode AP copy a sequential data to kernel mode physical address data
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure ahb_trans_t  
 */
#define FSDMA_COPY_SEQUENTIAL				_IOW(S_DMA_IOC_MAGIC, 1, int)

/**
 * \b ioctl(s_dma_fd, FSDMA_FILL_2D, &tag)
 *
 * \arg Fill data into kernel mode physical address
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure ahb_trans_t  
 */
#define FSDMA_FILL_2D								_IOW(S_DMA_IOC_MAGIC, 2, int)

/**
 * \b ioctl(s_dma_fd, FSDMA_COPY_2D, &tag)
 *
 * \arg Copy data from kernel mode physical source address into destination address
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure ahb_trans_t  
 */
#define FSDMA_COPY_2D								_IOW(S_DMA_IOC_MAGIC, 3, int)
#endif	/* _SPECIAL_DMA_H_ */
