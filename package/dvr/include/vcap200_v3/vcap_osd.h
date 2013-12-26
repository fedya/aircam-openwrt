/** @file vcap_osd.h
 *
 *  @brief This file provides the capture OSD and Mask related function calls or IOCTLs
 *
 *  @author Francis Huang
 *  @version 0.1.0
 *  @date 2010/09/28
 *
 */

/** @mainpage GM8126/GM812X Capture IOCTL and API Reference Documentation
 *
 *  @n vcap_osd.h describe the capture OSD and Mask related function calls or IOCTLs
 */ 
 
/**
 *  @example cap_osd_string.c
 *  @example cap_osd_mask.c
*/ 

#ifndef _VCAP_OSD_H_
#define _VCAP_OSD_H_
#include <linux/ioctl.h>
#include <linux/videodev.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

#define MAX_TARGET  10000

#define X2OFFSET(base_w, x)     (MAX_TARGET * x / base_w)
#define Y2OFFSET(base_h, y)     (MAX_TARGET * y / base_h)
#define W2OFFSET(base_w, w)     (MAX_TARGET * w / base_w)
#define H2OFFSET(base_h, h)     (MAX_TARGET * h / base_h)

#define OFFSET2X(base_w, off_x) (off_x * base_w / MAX_TARGET)
#define OFFSET2Y(base_h, off_y) (off_y * base_h / MAX_TARGET)
#define OFFSET2W(base_w, off_w) (off_w * base_w / MAX_TARGET)
#define OFFSET2H(base_h, off_h) (off_h * base_h / MAX_TARGET)

/**
 * @brief This structure defines for setting OSD window postion and size.
 */ 
typedef struct fiosd_win {
    uint32_t x:16;          ///< left position of OSD window
    uint32_t y:16;          ///< top position of OSD window
    uint32_t hdim:16;       ///< the dimension width of OSD window
    uint32_t vdim:16;       ///< the dimension height of OSD window
    uint8_t windex;         ///< the OSD window index
} fiosd_win_t;

/**
 * @brief This structure defines for setting OSD font feature. 
 * @n
 * @n     If width and height smaller than fonts in OSD ram, the display font will be cut.
 * @n     For example:
 * @n     If the fonts size is 12x16 in sram, we set the fiosd_font_info as 6x8.
 * @n     The OSD will display the quarter font from top-left.
 */ 
typedef struct fiosd_font_info {
    uint32_t width:8;       ///< the font width that will be display
    uint32_t height:8;      ///< the font height that will be display
    uint32_t row_space:8;   ///< the font row space
    uint32_t col_space:8;   ///< the font column space
    uint8_t windex;         ///< the OSD window index
} fiosd_font_info_t;

/**
 * @brief This structure defines for setting palette color.
 */ 
typedef struct fiosd_palette {
    uint8_t index;          ///< the palette color index. The value is from 0 to 6
    uint8_t y;              ///< the color of Y
    uint8_t cb;             ///< the color of Cb
    uint8_t cr;             ///< the color of Cr
} fiosd_palette_t;

/**
 * @brief This structure defines for add or replace a character in OSD ram.
 * @n
 * @n     The index 0 is special for FIOSDS_RMCHAR.
 */ 
typedef struct fiosd_char {
    char font;              ///< the font index that you want to setting or replacing
    uint8_t fbitmap[36];    ///< the font bitmap
} fiosd_char_t;

/**
 * @brief This structure defines for setting display string.
 * @n
 * @n     The OSD will display string from fiosd_string->start to fiosd_string->start + 
 * @n     strlen(fiosd_string->string) in OSD display array. The string[256] is string 
 * @n     index that depend on char bit map. While using multiple OSD windows at the same 
 * @n     time, user has to manager string length in the common string buffer.
 */ 
typedef struct fiosd_string {
    uint8_t windex;         ///< the OSD window index
    uint8_t start;          ///< the string start index in OSD display array. 0-255
    uint8_t fg_color;       ///< the forward color index that assign by FIOSDS_PALTCOLOR
    uint8_t bg_color;       ///< the backward color index that assign by FIOSDS_PALTCOLOR
    char string[256];       ///< the string that will be displayed
} fiosd_string_t;

/**
 * @brief This structure defines for setting transparent level.
 */ 
typedef struct fiosd_transparent {
    uint8_t windex;         ///< the OSD window index
    uint8_t level;          ///< the transparent level 0, 50, 75, 100 percent                            
#define FOSD_TRANSPARENT_0PERCENT    0
#define FOSD_TRANSPARENT_50PERCENT   1
#define FOSD_TRANSPARENT_75PERCENT   2
#define FOSD_TRANSPARENT_100PERCENT  3
} fiosd_transparent_t;

/**
 * @brief This structure defines for getting the bit map of characters.
 */ 
typedef struct fiosd_charmap {
    uint32_t map[8];        ///< the bitmap of fonts that store in OSD ram
} fiosd_charmap_t;

/**
 * @brief This structure defines for setting mask window postion and size
 */ 
typedef struct fiosdmask_win {
    uint32_t x:16;          ///< left position of mask window
    uint32_t y:16;          ///< top position of mask window
    uint32_t width:16;      ///< the dimension width of mask window
    uint32_t height:16;     ///< the dimension height of mask window
    uint8_t windex;         ///< the mask window index
    uint8_t color;          ///< the mask color index that assign by FIOSDS_PALTCOLOR
} fiosdmask_win_t;

/**
 * @brief This structure defines for getting OSD hardware information.
 */ 
typedef struct fiosd_hw_info {
    int MaxFontNum;         ///< max OSD font number
    int MaxDispNum;         ///< max OSD display number
} fiosd_hw_info_t;

/* Use 'f' as magic number */
#define VCAP_IOC_MAGIC  'f'

/*
 * IOCTL
 */
 
/**
 * @brief   Use to enable OSD window
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_ON, &windex)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e windex : specify to enable which OSD window, from 0~3 
 */
#define FIOSDS_ON               _IOW(VCAP_IOC_MAGIC,  1, int)

/**
 * @brief   Use to disable OSD window
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_OFF, &windex)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e windex : specify to disable which OSD window, from 0~3 
 */
#define FIOSDS_OFF              _IOW(VCAP_IOC_MAGIC,  2, int)

/**
 * @brief   Use to setup OSD window size and position
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_WIN, &win)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e win : reference to fiosd_win_t structure
 */
#define FIOSDS_WIN              _IOW(VCAP_IOC_MAGIC,  3, fiosd_win_t)

/**
 * @brief   Use to setup OSD font feature
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_FONTSETTING, &font_info)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e font_info : reference to fiosd_font_info_t structure
 */
#define FIOSDS_FONTSETTING      _IOW(VCAP_IOC_MAGIC,  4, fiosd_font_info_t)

/**
 * @brief   Use to setup OSD font transparent level
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_TRANSPARENT, &font_tran)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e font_tran : reference to fiosd_transparent_t structure
 */
#define FIOSDS_TRANSPARENT      _IOW(VCAP_IOC_MAGIC,  5, fiosd_transparent_t)

/**
 * @brief   Use to add or replace a character in OSD ram
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_CHAR, &font_char)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e font_char : reference to fiosd_char_t structure
 */
#define FIOSDS_CHAR             _IOW(VCAP_IOC_MAGIC,  6, fiosd_char_t)

/**
 * @brief   Use to setup OSD display string
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_STRING, &font_string)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e font_string : reference to fiosd_string_t structure
 */
#define FIOSDS_STRING           _IOW(VCAP_IOC_MAGIC,  7, fiosd_string_t)

/**
 * @brief   Use to setup OSD palette color
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_PALTCOLOR, &palette)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e palette : reference to fiosd_palette_t structure
 */
#define FIOSDS_PALTCOLOR        _IOW(VCAP_IOC_MAGIC,  8, fiosd_palette_t)

/**
 * @brief   Use to remove a character from OSD ram
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_RMCHAR, &font)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e font : character index
 */
#define FIOSDS_RMCHAR           _IOW(VCAP_IOC_MAGIC,  9, char)

/**
 * @brief   Use to get the bit map of characters
 * @n
 * @n @b    ioctl(osd_fd, FIOSDG_CHARMAP, &charmap)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e charmap : reference to fiosd_charmap_t structure
 */
#define FIOSDG_CHARMAP          _IOR(VCAP_IOC_MAGIC, 10, fiosd_charmap_t)

/**
 * @brief   Use to get OSD hardware information
 * @n
 * @n @b    ioctl(osd_fd, FIOSDG_HWINFO, &hwinfo)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e hwinfo : reference to fiosd_hw_info_t structure
 */
#define FIOSDG_HWINFO           _IOR(VCAP_IOC_MAGIC, 11, fiosd_hw_info_t)

/**
 * @brief   Use relative OSD window postion and dimension setup control
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_BWINOFFSET, &enable)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e enable : 0: disable, 1: enable
 */
#define FIOSDS_BWINOFFSET       _IOR(VCAP_IOC_MAGIC, 12, int)

/**
 * @brief   Use to enable/disable OSD force frame type
 * @n
 * @n @b    ioctl(osd_fd, FIOSDS_FRAMEMODE, &enable)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e enable : 0: disable, 1: enable
 */
#define FIOSDS_FRAMEMODE        _IOR(VCAP_IOC_MAGIC, 13, int)

/**
 * @brief   Use to enable OSD mask window
 * @n
 * @n @b    ioctl(osd_fd, FIOSDMASKS_ON, &windex)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e windex : specify to enable which OSD mask window, from 0~7 
 */
#define FIOSDMASKS_ON           _IOW(VCAP_IOC_MAGIC, 30, int)

/**
 * @brief   Use to disable OSD mask window
 * @n
 * @n @b    ioctl(osd_fd, FIOSDMASKS_OFF, &windex)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e windex : specify to disable which OSD mask window, from 0~7 
 */
#define FIOSDMASKS_OFF          _IOW(VCAP_IOC_MAGIC, 31, int)

/**
 * @brief   Use to setup OSD mask transparent level
 * @n
 * @n @b    ioctl(osd_fd, FIOSDMASKS_TRANSPARENT, &mask_tran)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e mask_tran : reference to fiosd_transparent_t structure
 */
#define FIOSDMASKS_TRANSPARENT  _IOW(VCAP_IOC_MAGIC, 32, fiosd_transparent_t)

/**
 * @brief   Use to setup OSD mask window size and position
 * @n
 * @n @b    ioctl(osd_fd, FIOSDMASKS_WIN, &win)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e win : reference to fiosdmask_win_t structure
 */
#define FIOSDMASKS_WIN          _IOW(VCAP_IOC_MAGIC, 33, fiosdmask_win_t)

/**
 * @brief   Use relative OSD mask window postion and dimension setup control
 * @n
 * @n @b    ioctl(osd_fd, FIOSDMASKS_BWINOFFSET, &enable)
 * @arg     parameter :
 * @n @b @e osd_fd : OSD device handler
 * @n @b @e enable : 0: disable, 1: enable
 */
#define FIOSDMASKS_BWINOFFSET   _IOW(VCAP_IOC_MAGIC, 34, int)
#endif  /* _VCAP_OSD_H_ */
