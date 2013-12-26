#ifndef _FSCALER_OSDAPI_H_
#define _FSCALER_OSDAPI_H_

#include <linux/ioctl.h>
#include <linux/videodev.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

#define OSD_MAX_DISP_NUM	128
#define OSD_MAX_CHAR_NUM	64

#define MAX_TARGET  10000

#define X2OFFSET(base_w, x) (MAX_TARGET * x / base_w)
#define Y2OFFSET(base_h, y) (MAX_TARGET * y / base_h)

#define OFFSET2X(base_w, off_x) (off_x * base_w / MAX_TARGET)
#define OFFSET2Y(base_h, off_y) (off_y * base_h / MAX_TARGET)

typedef struct fiosd_ch {
    uint16_t channel;
} fiosd_ch_t;

typedef struct fiosd_win {
    uint32_t x:16;
    uint32_t y:16;
    uint32_t hdim:16;
    uint32_t vdim:16;
    uint16_t channel;
} fiosd_win_t;

typedef struct fiosd_font_info {
    uint32_t width:8;
    uint32_t height:8;
    uint32_t row_space:8;
    uint32_t col_space:8;
    uint16_t channel;
} fiosd_font_info_t;

typedef struct fiosd_palette {
    uint8_t index;
    uint8_t y;
    uint8_t cb;
    uint8_t cr;
} fiosd_palette_t;

typedef struct fiosd_char {
    char font;
    uint8_t fbitmap[36];
} fiosd_char_t;

typedef struct fiosd_string {
    uint8_t start;              ///< 0-255
    uint8_t fg_color;
    uint8_t bg_color;
    char string[128];
    uint32_t channel;
} fiosd_string_t;

typedef struct fiosd_transparent {
    uint8_t level;
#define FOSD_TRANSPARENT_0PERCENT    0
#define FOSD_TRANSPARENT_50PERCENT   1
#define FOSD_TRANSPARENT_75PERCENT   2
#define FOSD_TRANSPARENT_100PERCENT  3
} fiosd_transparent_t;

typedef struct fiosd_charmap {
    uint32_t map[8];
} fiosd_charmap_t;

typedef struct fiosdmask_win {
    uint32_t x:16;
    uint32_t y:16;
    uint32_t width:16;
    uint32_t height:16;
    uint8_t color;
} fiosdmask_win_t;

typedef struct fiosd_hw_info {
    int MaxFontNum;
    int MaxDispNum;
} fiosd_hw_info_t;

typedef struct fiosd_osd_font_data {
    uint32_t bitmap[8];
    uint32_t fbitmap[(OSD_MAX_CHAR_NUM + 31) >> 5];
    uint8_t index[256];
    uint8_t count;
} fiosd_osd_font_data_t;

/* Use 'f' as magic number */
#define VCAP_IOC_MAGIC  's'

#define FIOSDS_ON					_IOW(VCAP_IOC_MAGIC, 1, int)
#define FIOSDS_OFF					_IOW(VCAP_IOC_MAGIC, 2, int)
#define FIOSDS_WIN					_IOW(VCAP_IOC_MAGIC, 3, fiosd_win_t)
#define FIOSDS_FONTSETTING			_IOW(VCAP_IOC_MAGIC, 4, fiosd_font_info_t)
#define FIOSDS_TRANSPARENT			_IOW(VCAP_IOC_MAGIC, 5, fiosd_transparent_t)
#define FIOSDS_CHAR					_IOW(VCAP_IOC_MAGIC, 6, fiosd_char_t)
#define FIOSDS_STRING				_IOW(VCAP_IOC_MAGIC, 7, fiosd_string_t)
#define FIOSDS_PALTCOLOR			_IOW(VCAP_IOC_MAGIC, 8, fiosd_palette_t)
#define FIOSDS_RMCHAR				_IOW(VCAP_IOC_MAGIC, 9, char)
#define FIOSDG_CHARMAP				_IOR(VCAP_IOC_MAGIC, 10, fiosd_charmap_t)
#define FIOSDG_HWINFO               _IOR(VCAP_IOC_MAGIC, 11, fiosd_hw_info_t)
#define FIOSDS_BWINOFFSET           _IOR(VCAP_IOC_MAGIC, 12, int)
#define FIOSDMASKS_ON				_IOW(VCAP_IOC_MAGIC, 30, int)
#define FIOSDMASKS_OFF				_IOW(VCAP_IOC_MAGIC, 31, int)
#define FIOSDMASKS_TRANSPARENT	    _IOW(VCAP_IOC_MAGIC, 32, fiosd_transparent_t)
#define FIOSDMASKS_WIN				_IOW(VCAP_IOC_MAGIC, 33, fiosdmask_win_t)
#define FIOSDMASKS_BWINOFFSET		_IOW(VCAP_IOC_MAGIC, 34, int)

#endif /* _FSCALER_OSDAPI_H_ */
