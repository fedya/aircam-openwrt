/**
 * @file fscaler_hw.h
 * @brief Scaler hardware routines header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_HW_H_
#define _FSCALER_HW_H_

#include "fscaler_errno.h"
#include "fscaler_debug.h"
#include "fscaler_kmem.h"
#include "fscaler_job.h"
#include "fscaler_reg.h"
#include "fscaler_platform.h"

typedef unsigned int dword_t;

#ifndef PTR_ALIGN
#define PTR_ALIGN(p, a)                 ((typeof(p))ALIGN((unsigned long)(p), (a)))
#endif /* PTR_ALIGN */

#define DWORD_PTR_ALIGN(x)              PTR_ALIGN(x,sizeof(dword_t))
#define DWORD_ALIGN(x)                  ALIGN(x,sizeof(dword_t))
#define DWORD_SIZE                      sizeof(dword_t)

static inline void fscaler_hw_power(int ip, int b_power)
{
    platform_power(ip, b_power);
}

static inline void fscaler_hw_osd_enable(unsigned int base, int windex, int b_enable)
{
    unsigned int nr_table[] = {
        IORL_OSD_WIN0_EN,
        IORL_OSD_WIN1_EN,
        IORL_OSD_WIN2_EN,
        IORL_OSD_WIN3_EN
    };
    if (windex < ARRAY_SIZE(nr_table)) {
        if (b_enable) {
            fscaler_reg_ioset(base, IOAT_OSDEN, nr_table[windex]);
        } else {
            fscaler_reg_ioclear(base, IOAT_OSDEN, nr_table[windex]);
        }
    }
}

static inline void fscaler_hw_enable(unsigned int base, int b_enable)
{
    if (b_enable) {
        fscaler_reg_ioset(base, IOAT_SCAREN, IORL_SCAR_EN);
    } else {
        fscaler_reg_ioclear(base, IOAT_SCAREN, IORL_SCAR_EN);
    }
}

static inline void fscaler_hw_caps(unsigned int base)
{
    unsigned long reg = 0;
    reg = fscaler_reg_setbits(reg, IOBTS_ALBUM_EN, 1, IORL_ALBUM_EN);
    fscaler_reg_write(base, IOAT_SCAREN, IOMSK_SCAREN, IOBTS_SCAR_EN, reg);
    fscaler_reg_ioset(base, IOAT_OSDEN, IORL_OSD_EDGE_SMTH_EN);
}

static inline void fscaler_hw_update(unsigned int base, int b_forced)
{
    if (b_forced) {
        fscaler_reg_ioset(base, IOAT_SCARUPD, IORL_FORCE_UPDATE);
    } else {
        fscaler_reg_ioset(base, IOAT_SCARUPD, IORL_SYNC_REG_UPDATE);
    }
}

static inline void fscaler_hw_force_caculate(unsigned int base)
{
    fscaler_reg_ioset(base, IOAT_SCARUPD, IORL_FORCE_CU);
}

static inline void fscaler_hw_software_reset(unsigned int base, int check)
{
    if (check) {
        if (!fscaler_reg_iotest(base, IOAT_INITSTS, IORL_DAHB_IDLE_STS)) {
            printk(KERN_DEBUG "SCL: AHB idle is not set after scale done\n");
            while (!fscaler_reg_iotest(base, IOAT_INITSTS, IORL_DAHB_IDLE_STS)) {
                ndelay(100);
            }
            printk(KERN_DEBUG "SCL: AHB idle is set after polling\n");
        }
        fscaler_reg_ioset(base, IOAT_INITSTS, IORL_DAHB_IDLE_STS);
    }
    fscaler_reg_ioset(base, IOAT_SCARCLK, IORL_SW_RST);
    ndelay(100);
}

static inline void fscaler_hw_set_target_background(unsigned int base, int out_w, int out_h)
{
    unsigned long reg = 0;
    // set width and height independent
    reg = fscaler_reg_setbits(reg, IOBTS_ASPECT_MAJOR, 0, IORL_ASPECT_MAJOR);
    fscaler_reg_write(base, IOAT_ASPMJ, IOMSK_ASPMJ, 0, reg);
    // set target width and height
    reg = fscaler_reg_setbits(reg, IOBTS_TARGET_HEIGHT, out_h, IORL_TARGET_HEIGHT);
    reg = fscaler_reg_setbits(reg, IOBTS_TARGET_WIDTH, out_w, IORL_TARGET_WIDTH);
    fscaler_reg_iowrite(base, IOAT_SIZE0, reg);
}

static inline void fscaler_hw_set_target_window(unsigned int base, int out_w, int out_h)
{
    unsigned long reg = 0;
    M_DEVELOPP("SCL: dst size:0 0 %d %d\n", out_w, out_h);
    // set crop x and y
    reg = fscaler_reg_setbits(reg, IOBTS_CROP_Y, 0, IORL_CROP_Y);
    reg = fscaler_reg_setbits(reg, IOBTS_CROP_X, 0, IORL_CROP_X);
    fscaler_reg_write(base, IOAT_SIZE1, IOMSK_SIZE1, 0, reg);
    reg = 0;
    // set crop width and height
    reg = fscaler_reg_setbits(reg, IOBTS_CROP_HEIGHT, out_h, IORL_CROP_HEIGHT);
    reg = fscaler_reg_setbits(reg, IOBTS_CROP_WIDTH, out_w, IORL_CROP_WIDTH);
    fscaler_reg_write(base, IOAT_SIZE2, IOMSK_SIZE2, 0, reg);
}

static inline void
fscaler_hw_set_target_frame(unsigned int base, int out_x, int out_y, int out_w,
                            int out_bg_w, struct color_buf *p_out_buf)
{
    int b_supp = 1;
    unsigned long reg_dformat = 0;
    unsigned long reg_udma0 = 0;
    unsigned long reg_dma1 = 0;
    unsigned long reg_mdest0 = 0, reg_mdest2 = 0, reg_mdest4 = 0;
    unsigned long reg_mdest1 = 0, reg_mdest3 = 0, reg_mdest5 = 0;
    unsigned long dest_yc_swap = 0, dest_rb_swap = 0;
    unsigned long dest_format = 0;
    unsigned long dma_wlen = 0;
    unsigned long dma_mst_en = 0, dma_order = 0, pdma_split = 0;
    unsigned long dma_chroma_wlen = 0, dma_luma_wlen = 0;
    size_t w_mb_y = 1, h_mb_y = 1, w_mb_uv = 1, h_mb_uv = 1;
    unsigned long num_4cc = 0;
    unsigned long den_4cc = 8, sub_win_pit = 0;
    size_t x = out_x, y = out_y, w = out_bg_w;
    unsigned int ofs_y = 0, ofs_uv = 0;
    if (p_out_buf->yuv_n >= 2) {
        dma_mst_en = 1;
    }
    do {
        /*
         * Assume that all formats are meet the condition U4cc == V4cc
         * It means U has the same size of V or U and V have the same ratio
         * of data
         *
         * For offset calculation:
         * Assume y, uv is varibales. z represents y or u or v or uv
         * MB(z) : The offset is multiple of macro block
         * FL(z) : The offset is related to last macro block
         * OFS(z) : The offset
         * x : The position of x direction
         * y : The position of y direction
         * w : The width of destination
         * h : The height of destination
         * w_mb(z) : The width of macro block
         * h_mb(z) : the height of macro block
         * Sh(z,y) : sampleing ratio of x direction
         * Sv(z,y) : sampling factor of y direction
         *
         * Sx(z,y) = w_mb(z)/w_mb(y)
         * Sy(z,y) = h_mb(z)/h_mb(y)
         *
         * Sh(y,y) = w_mb(y)/w_mb(y) = 1, Sv(y,y) = h_mb(y)/h_mb(y) = 1
         *
         * MB(z) = ( x/w_mb(z) )*( w_mb(z)*h_mb(z) )*Sh(z,y) + ( y/h_mb(z) )*( w*h_mb(z) )*Sv(z,y)
         * FL(z) = ( x%w_mb(z) )*Sh(z,y) + w_mb(z)*( y%h_mb(z) )*Sv(z,y)
         * OFS(z) = ( ( x/w_mb(z) )*( w_mb(z)*h_mb(z) ) + x%w_mb(z) )*Sh(z,y)
         *        + ( ( y/h_mb(z) )*( w*h_mb(z) )+( w_mb(z)*( y%h_mb(z) ) ) )*Sv(z,y)
         *
         * if x is multiple of w_mb(z) and y is multiple of h_mb(z)
         * MB(z) = Sx(z)*( x*h_mb(z) )+Sy(z)*( y*w )
         * FL(z) = 0
         * OFS(z) = MB(z)
         *
         * Assume destination address is Py/Puv or Py/Pu/Pv
         *
         * For 2 buffer split
         * Sx(uv) = 1, Sy(uv) = 1/2
         * P(y) = Py + OFS(y)
         * P(uv) = Puv + OFS(uv)
         *
         * For 3 buffer split
         * Sx(uv) = 1, Sy(uv) = 1
         * P(y) = Py + OFS(y)
         * P(uv) = Puv + OFS(uv)
         */
        if (COLOR_BUF_CHECK_LAYOUT(p_out_buf, FSCALER_JOB_LAYOUT_Y422)) {
            dest_format = 5;    ///< FSCALER_JOB_LAYOUT_Y422
            dma_wlen = 1;
            dma_mst_en = 0;     ///< 1 yuv set
            dma_order = 0;      ///< Sequence yuv buffer
            pdma_split = 0;     ///< Don't split buffer since sequence yuv layout
            dma_chroma_wlen = 2;
            dma_luma_wlen = 3;
            h_mb_y = 1;         ///< mb=1 since sequence yuv layout
            w_mb_uv = 1;
            h_mb_uv = 1;
            num_4cc = 8 + 4 + 4;
            den_4cc = 8;        ///< 8 bit per byte
            break;
        }
        if (COLOR_BUF_CHECK_LAYOUT(p_out_buf, FSCALER_JOB_LAYOUT_H2642D)) {
            dest_format = 6;    ///< I420M0
            dma_wlen = 0;
            dma_mst_en = 0;     ///< 1 yuv set
            dma_order = 3;      ///< mb16x16
            pdma_split = 2;     ///< Split into 2 buffers
            dma_chroma_wlen = 0;
            dma_luma_wlen = 0;
            h_mb_y = 16;        ///< mb=16
            w_mb_uv = 16;
            h_mb_uv = 8;        ///< 2D in 2 buffers layout is special mb16x8
            num_4cc = 8;
            den_4cc = 8;        ///< 8 bit per byte
            break;
        }
        if (COLOR_BUF_CHECK_LAYOUT(p_out_buf, FSCALER_JOB_LAYOUT_MPEG42D)) {
            dest_format = 6;    ///< I420M0
            dma_wlen = 0;
            dma_mst_en = 0;     ///< 1 yuv set
            dma_order = 3;      ///< mb16x16
            pdma_split = 1;     ///< Split into 3 buffers
            dma_chroma_wlen = 0;
            dma_luma_wlen = 0;
            h_mb_y = 16;        ///< mb=16
            w_mb_uv = 16;
            h_mb_uv = 16;
            num_4cc = 8;
            den_4cc = 8;        ///< 8 bit per byte
            break;
        }
        b_supp = 0;
    } while (0);
    if (!b_supp) {
        M_ERROR("SCL: Target doesn't support layout(%d)\n", p_out_buf->layout);
        return;
    }
    ofs_y = ((x / w_mb_y) * (w_mb_y * h_mb_y) + x % w_mb_y) * h_mb_y / h_mb_y
        + ((y / h_mb_y) * (w * h_mb_y) + (w_mb_y * (y % h_mb_y))) * h_mb_y / h_mb_y;
    ofs_y = ofs_y * num_4cc / den_4cc;
    ofs_uv = ((x / w_mb_uv) * (w_mb_uv * h_mb_uv) + x % w_mb_uv) * h_mb_uv / h_mb_y
        + ((y / h_mb_uv) * (w * h_mb_uv) + (w_mb_uv * (y % h_mb_uv))) * h_mb_uv / h_mb_y;
    dest_yc_swap = (unsigned long)COLOR_BUF_IS_YC_SWAP(p_out_buf);
    dest_rb_swap = (unsigned long)COLOR_BUF_IS_RB_SWAP(p_out_buf);
    /* Setup register DFORMAT */
    reg_dformat = 0;
    reg_dformat =
        fscaler_reg_setbits(reg_dformat, IOBTS_DEST_YC_SWAP, dest_yc_swap, IORL_DEST_YC_SWAP);
    reg_dformat =
        fscaler_reg_setbits(reg_dformat, IOBTS_DEST_RB_SWAP, dest_rb_swap, IORL_DEST_RB_SWAP);
    reg_dformat =
        fscaler_reg_setbits(reg_dformat, IOBTS_DEST_FORMAT, dest_format, IORL_DEST_FORMAT);
    fscaler_reg_write(base, IOAT_DFORMAT, IOMSK_DFORMAT, 0, reg_dformat);
    /* Setup register UDMA0 */
    reg_udma0 = 0;
    reg_udma0 = fscaler_reg_setbits(reg_udma0, IOBTS_DMA_FIFO_WM, 4, IORL_DMA_FIFO_WM);
    reg_udma0 = fscaler_reg_setbits(reg_udma0, IOBTS_DMA_MST_EN, dma_mst_en, IORL_DMA_MST_EN);
    reg_udma0 = fscaler_reg_setbits(reg_udma0, IOBTS_DMA_ORDER, dma_order, IORL_DMA_ORDER);
    reg_udma0 = fscaler_reg_setbits(reg_udma0, IOBTS_PDMA_SPLIT, pdma_split, IORL_PDMA_SPLIT);
    reg_udma0 = fscaler_reg_setbits(reg_udma0, IOBTS_DMA_WLEN, dma_wlen, IORL_DMA_WLEN);
    reg_udma0 =
        fscaler_reg_setbits(reg_udma0, IOBTS_DMA_CHROMA_WLEN, dma_chroma_wlen,
                            IORL_DMA_CHROMA_WLEN);
    reg_udma0 =
        fscaler_reg_setbits(reg_udma0, IOBTS_DMA_LUMA_WLEN, dma_luma_wlen, IORL_DMA_LUMA_WLEN);
    fscaler_reg_write(base, IOAT_UDMA0, IOMSK_UDMA0, 0, reg_udma0);
    /* Setup register DMA1 */
    sub_win_pit = DWORD_ALIGN(w * p_out_buf->sample_down * num_4cc / den_4cc) / DWORD_SIZE;
    reg_dma1 = 0;
    reg_dma1 = fscaler_reg_setbits(reg_dma1, IOBTS_SUB_WIN_PIT, sub_win_pit, IORL_SUB_WIN_PIT);
    fscaler_reg_write(base, IOAT_DMA1, IOMSK_DMA1, 0, reg_dma1);
    /* Setup register MDEST0/1/2 */
    if (pdma_split == 2)        ///< 2 buffers case
    {
        reg_mdest0 = 0;
        reg_mdest0 = fscaler_reg_setbits(reg_mdest0, IOBTS_MEM_DEST_YST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].y_uv[0] +
                                                         ofs_y) / DWORD_SIZE, IORL_MEM_DEST_YST0);
        fscaler_reg_write(base, IOAT_MDEST0, IOMSK_MDEST0, 0, reg_mdest0);
        reg_mdest2 = 0;
        reg_mdest2 = fscaler_reg_setbits(reg_mdest2, IOBTS_MEM_DEST_CBST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].y_uv[1] +
                                                         ofs_uv) / DWORD_SIZE, IORL_MEM_DEST_CBST0);
        fscaler_reg_write(base, IOAT_MDEST2, IOMSK_MDEST2, 0, reg_mdest2);
    } else if (pdma_split == 1) {       ///< 3 buffers case
        reg_mdest0 = 0;
        reg_mdest0 = fscaler_reg_setbits(reg_mdest0, IOBTS_MEM_DEST_YST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].y_u_v[0] +
                                                         ofs_y) / DWORD_SIZE, IORL_MEM_DEST_YST0);
        fscaler_reg_write(base, IOAT_MDEST0, IOMSK_MDEST0, 0, reg_mdest0);
        reg_mdest2 = 0;
        reg_mdest2 = fscaler_reg_setbits(reg_mdest2, IOBTS_MEM_DEST_CBST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].y_u_v[1] +
                                                         ofs_uv) / DWORD_SIZE, IORL_MEM_DEST_CBST0);
        fscaler_reg_write(base, IOAT_MDEST2, IOMSK_MDEST2, 0, reg_mdest2);
        reg_mdest4 = 0;
        reg_mdest4 = fscaler_reg_setbits(reg_mdest4, IOBTS_MEM_DEST_CRST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].y_u_v[2] +
                                                         ofs_uv) / DWORD_SIZE, IORL_MEM_DEST_CRST0);
        fscaler_reg_write(base, IOAT_MDEST4, IOMSK_MDEST4, 0, reg_mdest4);
    } else {                    ///< 1 buffer case
        reg_mdest0 = 0;
        reg_mdest0 = fscaler_reg_setbits(reg_mdest0, IOBTS_MEM_DEST_YST0,
                                         DWORD_PTR_ALIGN(p_out_buf->yuv[0].yuv[0] +
                                                         ofs_y) / DWORD_SIZE, IORL_MEM_DEST_YST0);
        fscaler_reg_write(base, IOAT_MDEST0, IOMSK_MDEST0, 0, reg_mdest0);
    }
    /* Setup 2nd register MDEST0/1/2 */
    if (dma_mst_en == 1) {
        if (pdma_split == 2)    ///< 2 buffers case
        {
            reg_mdest1 = 0;
            reg_mdest1 = fscaler_reg_setbits(reg_mdest1, IOBTS_MEM_DEST_YST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].y_uv[0] +
                                                             ofs_y) / DWORD_SIZE,
                                             IORL_MEM_DEST_YST0);
            fscaler_reg_write(base, IOAT_MDEST1, IOMSK_MDEST1, 0, reg_mdest1);
            reg_mdest3 = 0;
            reg_mdest3 = fscaler_reg_setbits(reg_mdest3, IOBTS_MEM_DEST_CBST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].y_uv[1] +
                                                             ofs_uv) / DWORD_SIZE,
                                             IORL_MEM_DEST_CBST0);
            fscaler_reg_write(base, IOAT_MDEST3, IOMSK_MDEST3, 0, reg_mdest3);
        } else if (pdma_split == 1) {   ///< 3 buffers case
            reg_mdest1 = 0;
            reg_mdest1 = fscaler_reg_setbits(reg_mdest1, IOBTS_MEM_DEST_YST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].y_u_v[0] +
                                                             ofs_y) / DWORD_SIZE,
                                             IORL_MEM_DEST_YST0);
            fscaler_reg_write(base, IOAT_MDEST1, IOMSK_MDEST1, 0, reg_mdest1);
            reg_mdest3 = 0;
            reg_mdest3 = fscaler_reg_setbits(reg_mdest3, IOBTS_MEM_DEST_CBST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].y_u_v[1] +
                                                             ofs_uv) / DWORD_SIZE,
                                             IORL_MEM_DEST_CBST0);
            fscaler_reg_write(base, IOAT_MDEST3, IOMSK_MDEST3, 0, reg_mdest3);
            reg_mdest5 = 0;
            reg_mdest5 = fscaler_reg_setbits(reg_mdest5, IOBTS_MEM_DEST_CRST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].y_u_v[2] +
                                                             ofs_uv) / DWORD_SIZE,
                                             IORL_MEM_DEST_CRST0);
            fscaler_reg_write(base, IOAT_MDEST5, IOMSK_MDEST5, 0, reg_mdest5);
        } else {                ///< 1 buffer case
            reg_mdest1 = 0;
            reg_mdest1 = fscaler_reg_setbits(reg_mdest1, IOBTS_MEM_DEST_YST0,
                                             DWORD_PTR_ALIGN(p_out_buf->yuv[1].yuv[0] +
                                                             ofs_y) / DWORD_SIZE,
                                             IORL_MEM_DEST_YST0);
            fscaler_reg_write(base, IOAT_MDEST1, IOMSK_MDEST1, 0, reg_mdest1);
        }
    }
}

static inline void fscaler_hw_select_source(unsigned int base)
{
    unsigned long reg = 0;
    reg = fscaler_reg_setbits(reg, IOBTS_RS_H_BLANK_PIX, 0x08, IORL_RS_H_BLANK_PIX);
    reg = fscaler_reg_setbits(reg, IOBTS_VP_RANGE, 1, IORL_VP_RANGE);
    reg = fscaler_reg_setbits(reg, IOBTS_IMG_SRC_SEL, 1, IORL_IMG_SRC_SEL);
    fscaler_reg_iowrite(base, IOAT_SRCSEL, reg);
    fscaler_reg_ioset(base, IOAT_SRCIF, IORL_VP_PROG);
}

static inline unsigned long fscaler_hw_init_irqmask(unsigned int base)
{
    unsigned long reg = 0;
    reg = fscaler_reg_setbits(reg, IOBTS_DMA_OVF_MASK, 1, IORL_DMA_OVF_MASK);
    reg = fscaler_reg_setbits(reg, IOBTS_UAHB_ERR_MASK, 1, IORL_UAHB_ERR_MASK);
    reg = fscaler_reg_setbits(reg, IOBTS_DAHB_IDLE_MASK, 1, IORL_DAHB_IDLE_MASK);
    reg = fscaler_reg_setbits(reg, IOBTS_FM_END_MASK, 1, IORL_FM_END_MASK);
    reg = fscaler_reg_setbits(reg, IOBTS_FM_START_MASK, 1, IORL_FM_START_MASK);
    fscaler_reg_write(base, IOAT_INITMASK, IOMSK_INITMASK, 0, reg);
    return reg;
}

static inline void fscaler_hw_init_status(unsigned int base, unsigned long clear)
{
    fscaler_reg_iowrite(base, IOAT_INITSTS, clear);
}

static inline void fscaler_hw_set_source_background(unsigned int base, int in_bg_w, int in_bg_h)
{
    unsigned long reg = 0;
    reg = fscaler_reg_setbits(reg, IOBTS_MEM_SRC_HEIGHT, in_bg_h, IORL_MEM_SRC_HEIGHT);
    reg = fscaler_reg_setbits(reg, IOBTS_MEM_SRC_WIDTH, in_bg_w, IORL_MEM_SRC_WIDTH);
    fscaler_reg_write(base, IOAT_MEMSRC2, IOMSK_MEMSRC2, 0, reg);
}

static inline void fscaler_hw_set_source_window(unsigned int base, int in_x, int in_y, int in_w,
                                                int in_h)
{
    unsigned long reg = 0;
    if (in_x > 0) {
        in_x = 0;
    }
    if (in_y > 0) {
        in_y = 0;
    }
    M_DEVELOPP("SCL: src size:%d %d %d %d\n", in_x, in_y, in_w, in_h);
    fscaler_reg_ioclear(base, IOAT_SRCSEL, IORL_SWC_CENTER_XY);
    reg = fscaler_reg_setbits(reg, IOBTS_SWC_Y, in_y, IORL_SWC_Y);
    reg = fscaler_reg_setbits(reg, IOBTS_SWC_X, in_x, IORL_SWC_X);
    fscaler_reg_write(base, IOAT_SRCSIZE1, IOMSK_SRCSIZE1, 0, reg);
    reg = 0;
    reg = fscaler_reg_setbits(reg, IOBTS_SWC_HEIGHT, in_h, IORL_SWC_HEIGHT);
    reg = fscaler_reg_setbits(reg, IOBTS_SWC_WIDTH, in_w, IORL_SWC_WIDTH);
    fscaler_reg_write(base, IOAT_SRCSIZE2, IOMSK_SRCSIZE2, 0, reg);
}

static inline void fscaler_hw_set_source_frame(unsigned int base, int in_x, int in_y, int in_bg_w,
                                               struct color_buf *in_buf)
{
    int b_supp = 1;
    unsigned long reg_ddma0 = 0;
    unsigned long reg_memsrc0 = 0;
    unsigned long reg_memsrc1 = 0;
    unsigned long reg_memsrcpit = 0;
    unsigned long mem_src_rb_swap = (unsigned long)COLOR_BUF_IS_RB_SWAP(in_buf);
    unsigned long mem_src_format = 1;
    unsigned long mem_src_st = 0;
    unsigned long mem_src_pit = 0;
    unsigned long num_4cc = 0, den_4cc = 8;
    unsigned long dma_rlen = 1, dma_wlen = 1;
    do {
        if (COLOR_BUF_CHECK_LAYOUT(in_buf, FSCALER_JOB_LAYOUT_Y422)) {
            mem_src_format = 5;
            num_4cc = 16;
            den_4cc = 8;
            if (in_x > 0) {
                in_buf->yuv[0].yuv[0] += in_x * num_4cc / den_4cc;
            }
            if (in_y > 0) {
                in_buf->yuv[0].yuv[0] += in_y * in_bg_w * num_4cc / den_4cc;
            }
            mem_src_st = (unsigned long)DWORD_PTR_ALIGN(in_buf->yuv[0].yuv[0]) / DWORD_SIZE;
            mem_src_pit =
                DWORD_ALIGN(in_bg_w * in_buf->sample_down * num_4cc / den_4cc) / DWORD_SIZE;
            break;
        }
        b_supp = 0;
    } while (0);
    if (!b_supp) {
        M_ERROR("SCL: Source doesn't support layout(%d)\n", in_buf->layout);
        return;
    }
    /* Setup register DDMA0 */
    reg_ddma0 = fscaler_reg_setbits(reg_ddma0, IOBTS_DMA_RLEN, dma_rlen, IORL_DMA_RLEN);
    reg_ddma0 = fscaler_reg_setbits(reg_ddma0, IOBTS_DMA_WLEN, dma_wlen, IORL_DMA_WLEN);
    fscaler_reg_write(base, IOAT_DDMA0, IOMSK_DDMA0, 0, reg_ddma0);
    /* Setup register MEMSRC0 */
    reg_memsrc0 =
        fscaler_reg_setbits(reg_memsrc0, IOBTS_MEM_SRC_FORMAT, mem_src_format, IORL_MEM_SRC_FORMAT);
    reg_memsrc0 =
        fscaler_reg_setbits(reg_memsrc0, IOBTS_MEM_SRC_RB_SWAP, mem_src_rb_swap,
                            IORL_MEM_SRC_RB_SWAP);
    reg_memsrc0 = fscaler_reg_setbits(reg_memsrc0, IOBTS_MEM_SRC_HBLANK, 3, IORL_MEM_SRC_HBLANK);
    fscaler_reg_write(base, IOAT_MEMSRC0, IOMSK_MEMSRC0, 3, reg_memsrc0);
    /* Setup register MEMSRC1 */
    reg_memsrc1 = fscaler_reg_setbits(reg_memsrc1, IOBTS_MEM_SRC_ST, mem_src_st, IORL_MEM_SRC_ST);
    fscaler_reg_write(base, IOAT_MEMSRC1, IOMSK_MEMSRC1, 0, reg_memsrc1);
    /* Setup register MEMSRCPIT */
    reg_memsrcpit =
        fscaler_reg_setbits(reg_memsrcpit, IOBTS_MEM_SRC_PIT, mem_src_pit, IORL_MEM_SRC_PIT);
    fscaler_reg_write(base, IOAT_MEMSRCPIT, IOMSK_MEMSRCPIT, 0, reg_memsrcpit);
}

static inline void fscaler_hw_frame_trigger(unsigned int base, int after)
{
    unsigned int n = 3000;
    unsigned long delay_ns = 10;
    /* Before frame triger, device is always disable to enable,
     * It need not to be forced to update and calculate.
     * Disable->Enable will do force update and enable will do calculation.
     */
    if (!after) {
        fscaler_hw_update(base, 1);
        fscaler_hw_force_caculate(base);
        /* Wait for calculation finish */
        while (fscaler_reg_read(base, IOAT_SCARSTS, msk1(24), 0) && --n) {
            ndelay(delay_ns);
        }
        if (0 >= n) {
            M_ERROR("SCL: force cu time out!\n");
        }
    }
    ///< post image, fire!
    fscaler_reg_ioset(base, IOAT_SCAREN, IORL_PIMG_EN);
}

static inline int fscaler_hw_test_status(unsigned int base, unsigned long mask)
{
    int ret = FSCALER_HW_ERR_STSTEST;
    unsigned long msk_error = IOBTS_DMA_OVF_ERR | IOBTS_UAHB_ERR | IOBTS_NULL_ERR | IOBTS_DAHB_ERR;
    unsigned long reg_initsts = fscaler_reg_read(base, IOAT_INITSTS, IOMSK_INITSTS, 0);
    M_DEVELOPP("###### inist(real) = %08lx\n", reg_initsts);
    fscaler_hw_init_status(base, reg_initsts & ~(IOBTS_DAHB_IDLE_STS));
    reg_initsts &= ~mask;       ///< Clear the masked status bits
    if (reg_initsts) ;
    {
        M_DEVELOPP("###### inist(mask) = %08lx\n", reg_initsts);
        if (reg_initsts & IOBTS_FD_STS) {       ///< Done
            ret = FSCALER_HW_OK;
        } else {
            /* Error found, try to be recognized and handled */
            printk("SCL: Error ISR status 0x%x\n", (unsigned int)reg_initsts);
            if (reg_initsts & msk_error) {
                if (reg_initsts & IOBTS_DMA_OVF_ERR) {
                    if (BUG_FAKE_DMAOVF_ON()) {
                        ret = FSCALER_HW_OK_FAKE_DMAOVF;
                    } else {
                        //DBG_ERROR("SCL: DMA_OVF_ERR occurred\n");
                        ret = FSCALER_HW_ERR_DMAOVF;
                    }
                } else if (reg_initsts & IOBTS_UAHB_ERR) {
                    //DBG_ERROR("SCL: UAHB_ERR occurred\n");
                    ret = FSCALER_HW_ERR_UAHB;
                } else if (reg_initsts & IOBTS_NULL_ERR) {
                    //DBG_ERROR("SCL: NULL_ERR occurred\n");
                    ret = FSCALER_HW_ERR_NULL;
                } else if (reg_initsts & IOBTS_DAHB_ERR) {
                    //DBG_ERROR("SCL: DAHB_ERR occurred\n");
                    ret = FSCALER_HW_ERR_DAHB;
                }
            } else {
                ret = FSCALER_HW_ERR_STSTEST;
            }
        }
    }
    M_DEVELOPP("###### ret = %d\n", ret);
    return ret;
}

static inline int fscaler_hw_test_osd_display_busy(unsigned int base)
{
    unsigned int n = 3000;
    unsigned long delay_ns = 10;
    /* Wait for font ram */
    while (fscaler_reg_read(base, IOAT_OSDREAD, msk1(31), 0) && --n) {
        ndelay(delay_ns);
    }
    if (0 >= n) {
        M_ERROR("SCL: OSD dispaly ram busy time out!\n");
        return FSCALER_HW_ERR_BUSY;
    }
    return FSCALER_HW_OK;
}

static inline int fscaler_hw_test_osd_font_busy(unsigned int base)
{
    unsigned int n = 3000;
    unsigned long delay_ns = 10;
    /* Wait for font ram */
    while (fscaler_reg_read(base, IOAT_OSDREAD, msk1(30), 0) && --n) {
        ndelay(delay_ns);
    }
    if (0 >= n) {
        M_ERROR("SCL: OSD font ram busy time out!\n");
        return FSCALER_HW_ERR_BUSY;
    }
    return FSCALER_HW_OK;
}

static inline void fscaler_hw_set_font_data(unsigned int base, int findex, unsigned char *fbitmap,
                                            unsigned int height)
{
    int i = 0;
    unsigned long reg_osd_font = 0, font_addr = 0, font_data = 0;
    if (findex >= 0 && base && fbitmap && height > 0) {
        for (i = 0; i < height; ++i) {
            reg_osd_font = 0;
            font_addr = findex * height + i;
            font_data = ((fbitmap[2 * i + 1] >> 4) & 0x00f) | ((fbitmap[2 * i] & 0xff) << 4);
            reg_osd_font =
                fscaler_reg_setbits(reg_osd_font, IOBTS_FONT_RAM_ADDR, font_addr,
                                    IORL_FONT_RAM_ADDR);
            reg_osd_font =
                fscaler_reg_setbits(reg_osd_font, IOBTS_FONT_RAM_DATA, font_data,
                                    IORL_FONT_RAM_DATA);
            fscaler_reg_write(base, IOAT_OSDFONT, IOMSK_OSDFONT, 0, reg_osd_font);
            fscaler_hw_test_osd_font_busy(base);
            M_DEVELOPP("SCL: write %04x:%08x, findex:%d\n", IOAT_OSDFONT,
                       (unsigned int)reg_osd_font, findex);
        }
    }
}

static inline void fscaler_hw_set_osd_palette(unsigned int base, int id, unsigned int palette)
{
    unsigned int ofs = 0;
    unsigned long allow = 0;
    do {
        if (0 == id) {
            ofs = IOAT_OSDPAT0;
            allow = IOMSK_OSDPAT0;
            break;
        }
        if (1 == id) {
            ofs = IOAT_OSDPAT1;
            allow = IOMSK_OSDPAT1;
            break;
        }
        if (2 == id) {
            ofs = IOAT_OSDPAT2;
            allow = IOMSK_OSDPAT2;
            break;
        }
        if (3 == id) {
            ofs = IOAT_OSDPAT3;
            allow = IOMSK_OSDPAT3;
            break;
        }
        if (4 == id) {
            ofs = IOAT_OSDPAT4;
            allow = IOMSK_OSDPAT4;
            break;
        }
        if (5 == id) {
            ofs = IOAT_OSDPAT5;
            allow = IOMSK_OSDPAT5;
            break;
        }
        if (6 == id) {
            ofs = IOAT_OSDPAT6;
            allow = IOMSK_OSDPAT6;
            break;
        }
        return;
    } while (0);
    M_DEVELOPP("SCL: write %04x:%08x\n", (unsigned int)ofs, (unsigned int)palette);
    fscaler_reg_write(base, ofs, allow, 0, palette);
}

static inline void fscaler_hw_set_osd_font_size(unsigned int base, int windex,
                                                unsigned int row, unsigned int col,
                                                unsigned int width, unsigned int height)
{
    unsigned long reg = 0;
    unsigned int ofs = 0, nr_row = 0, nr_col = 0, nr_height = 0, nr_width = 0;
    unsigned long bts_row = 0, bts_col = 0, bts_height = 0, bts_width = 0;
    unsigned long allow = 0;
    do {
        if (0 == windex) {
            ofs = IOAT_OSDFSZ0;
            nr_row = IORL_OSD_WIN0_FSPACE_ROW;
            nr_col = IORL_OSD_WIN0_FSPACE_COL;
            nr_width = IORL_OSD_WIN0_FONT_W;
            nr_height = IORL_OSD_WIN0_FONT_H;
            bts_row = IOBTS_OSD_WIN0_FSPACE_ROW;
            bts_col = IOBTS_OSD_WIN0_FSPACE_COL;
            bts_width = IOBTS_OSD_WIN0_FONT_W;
            bts_height = IOBTS_OSD_WIN0_FONT_H;
            allow = IOMSK_OSDFSZ0;
            break;
        }
        if (1 == windex) {
            ofs = IOAT_OSDFSZ1;
            nr_row = IORL_OSD_WIN1_FSPACE_ROW;
            nr_col = IORL_OSD_WIN1_FSPACE_COL;
            nr_width = IORL_OSD_WIN1_FONT_W;
            nr_height = IORL_OSD_WIN1_FONT_H;
            bts_row = IOBTS_OSD_WIN1_FSPACE_ROW;
            bts_col = IOBTS_OSD_WIN1_FSPACE_COL;
            bts_width = IOBTS_OSD_WIN1_FONT_W;
            bts_height = IOBTS_OSD_WIN1_FONT_H;
            allow = IOMSK_OSDFSZ1;
            break;
        }
        if (2 == windex) {
            ofs = IOAT_OSDFSZ2;
            nr_row = IORL_OSD_WIN2_FSPACE_ROW;
            nr_col = IORL_OSD_WIN2_FSPACE_COL;
            nr_width = IORL_OSD_WIN2_FONT_W;
            nr_height = IORL_OSD_WIN2_FONT_H;
            bts_row = IOBTS_OSD_WIN2_FSPACE_ROW;
            bts_col = IOBTS_OSD_WIN2_FSPACE_COL;
            bts_width = IOBTS_OSD_WIN2_FONT_W;
            bts_height = IOBTS_OSD_WIN2_FONT_H;
            allow = IOMSK_OSDFSZ2;
            break;
        }
        if (3 == windex) {
            ofs = IOAT_OSDFSZ0;
            nr_row = IORL_OSD_WIN3_FSPACE_ROW;
            nr_col = IORL_OSD_WIN3_FSPACE_COL;
            nr_width = IORL_OSD_WIN3_FONT_W;
            nr_height = IORL_OSD_WIN3_FONT_H;
            bts_row = IOBTS_OSD_WIN3_FSPACE_ROW;
            bts_col = IOBTS_OSD_WIN3_FSPACE_COL;
            bts_width = IOBTS_OSD_WIN3_FONT_W;
            bts_height = IOBTS_OSD_WIN3_FONT_H;
            allow = IOMSK_OSDFSZ3;
            break;
        }
        return;
    } while (0);
    reg = fscaler_reg_setbits(reg, bts_row, row, nr_row);
    reg = fscaler_reg_setbits(reg, bts_col, col, nr_col);
    reg = fscaler_reg_setbits(reg, bts_width, width, nr_width);
    reg = fscaler_reg_setbits(reg, bts_height, height, nr_height);
    M_DEVELOPP("SCL: write %04x:%08x\n", (unsigned int)ofs, (unsigned int)reg);
    fscaler_reg_write(base, ofs, allow, 0, reg);
}

static inline void fscaler_hw_set_osd_display_ram(unsigned int base, int dindex, int findex)
{
    // dindex: display index findex: font index, write font index into the display address by display index
    unsigned long reg = 0;
    findex = findex | (1 << 7);
    reg = fscaler_reg_setbits(reg, IOBTS_DISP_RAM_ADDR, dindex, IORL_DISP_RAM_ADDR);
    reg = fscaler_reg_setbits(reg, IOBTS_DISP_RAM_DATA, findex, IORL_DISP_RAM_DATA);
    M_DEVELOPP("SCL: write %04x:%08x\n", IOAT_OSDDISP, (unsigned int)reg);
    fscaler_reg_write(base, IOAT_OSDDISP, IOMSK_OSDDISP, 0, reg);
    fscaler_hw_test_osd_display_busy(base);
}

static inline unsigned int fscaler_hw_get_osd_display_ram(unsigned int base, int dindex, int findex)
{
    // dindex: display index findex: font index, write font index into the display address by display index
    unsigned long reg = 0;
    findex = findex | (1 << 7);
    reg = fscaler_reg_setbits(reg, IOBTS_DISP_RAM_READ, 1, IORL_DISP_RAM_READ);
    reg = fscaler_reg_setbits(reg, IOBTS_DISP_RAM_ADDR, dindex, IORL_DISP_RAM_ADDR);
    reg = fscaler_reg_setbits(reg, IOBTS_DISP_RAM_DATA, findex, IORL_DISP_RAM_DATA);
    M_DEVELOPP("SCL: write %04x:%08x\n", IOAT_OSDDISP, (unsigned int)reg);
    fscaler_reg_write(base, IOAT_OSDDISP, IOMSK_OSDDISP, 0, reg);
    fscaler_hw_test_osd_display_busy(base);
    reg = fscaler_reg_read(base, IOAT_OSDREAD, IOMSK_OSDREAD, 0);
    return (unsigned int)reg;
}

static inline void fscaler_hw_set_osd_color(unsigned int base, int windex, int level,
                                            unsigned char bc, unsigned char fc)
{
    unsigned long reg = 0;
    unsigned int ofs = 0, nr_tran = 0, nr_bc = 0, nr_fc = 0;
    unsigned long bts_tran = 0, bts_bc = 0, bts_fc = 0;
    unsigned long allow = 0;
    do {
        if (0 == windex) {
            ofs = IOAT_OSDCOR0;
            nr_tran = IORL_OSD_WIN0_TRAN;
            nr_bc = IORL_OSD_WIN0_BWCOLOR;
            nr_fc = IORL_OSD_WIN0_FWCOLOR;
            bts_tran = IOBTS_OSD_WIN0_TRAN;
            bts_bc = IOBTS_OSD_WIN0_BWCOLOR;
            bts_fc = IOBTS_OSD_WIN0_FWCOLOR;
            allow = IOMSK_OSDCOR0;
            break;
        }
        if (1 == windex) {
            ofs = IOAT_OSDCOR1;
            nr_tran = IORL_OSD_WIN1_TRAN;
            nr_bc = IORL_OSD_WIN1_BWCOLOR;
            nr_fc = IORL_OSD_WIN1_FWCOLOR;
            bts_tran = IOBTS_OSD_WIN1_TRAN;
            bts_bc = IOBTS_OSD_WIN1_BWCOLOR;
            bts_fc = IOBTS_OSD_WIN1_FWCOLOR;
            allow = IOMSK_OSDCOR1;
            break;
        }
        if (2 == windex) {
            ofs = IOAT_OSDCOR2;
            nr_tran = IORL_OSD_WIN2_TRAN;
            nr_bc = IORL_OSD_WIN2_BWCOLOR;
            nr_fc = IORL_OSD_WIN2_FWCOLOR;
            bts_tran = IOBTS_OSD_WIN2_TRAN;
            bts_bc = IOBTS_OSD_WIN2_BWCOLOR;
            bts_fc = IOBTS_OSD_WIN2_FWCOLOR;
            allow = IOMSK_OSDCOR2;
            break;
        }
        if (3 == windex) {
            ofs = IOAT_OSDCOR3;
            nr_tran = IORL_OSD_WIN3_TRAN;
            nr_bc = IORL_OSD_WIN3_BWCOLOR;
            nr_fc = IORL_OSD_WIN3_FWCOLOR;
            bts_tran = IOBTS_OSD_WIN3_TRAN;
            bts_bc = IOBTS_OSD_WIN3_BWCOLOR;
            bts_fc = IOBTS_OSD_WIN3_FWCOLOR;
            allow = IOMSK_OSDCOR3;
            break;
        }
        return;
    } while (0);
    reg = fscaler_reg_setbits(reg, bts_tran, level, nr_tran);
    reg = fscaler_reg_setbits(reg, bts_bc, bc, nr_bc);
    reg = fscaler_reg_setbits(reg, bts_fc, fc, nr_fc);
    M_DEVELOPP("SCL: write %04x:%08x\n", (unsigned int)ofs, (unsigned int)reg);
    fscaler_reg_write(base, ofs, allow, 0, reg);
}

static inline void fscaler_hw_set_osd_win(unsigned int base, int windex, int dindex, int x, int y,
                                          unsigned int w, unsigned int h)
{
    unsigned long reg_wsz = 0, reg_ssz = 0;
    unsigned int ofs_wsz = 0, ofs_ssz = 0;
    unsigned int nr_addr = 0, nr_h = 0, nr_w = 0, nr_x = 0, nr_y = 0;
    unsigned long bts_addr = 0, bts_h = 0, bts_w = 0, bts_x = 0, bts_y = 0;
    unsigned long allow_wsz = 0, allow_ssz = 0;
    do {
        if (0 == windex) {
            ofs_wsz = IOAT_OSDWSZ0;
            nr_addr = IORL_OSD_WIN0_DISPADDR;
            nr_h = IORL_OSD_WIN0_HEIGHT;
            nr_w = IORL_OSD_WIN0_WIDTH;
            ofs_ssz = IOAT_OSDSSZ0;
            nr_x = IORL_OSD_WIN0_POS_X;
            nr_y = IORL_OSD_WIN0_POS_Y;
            bts_addr = IOBTS_OSD_WIN0_DISPADDR;
            bts_h = IOBTS_OSD_WIN0_HEIGHT;
            bts_w = IOBTS_OSD_WIN0_WIDTH;
            bts_x = IOBTS_OSD_WIN0_POS_X;
            bts_y = IOBTS_OSD_WIN0_POS_Y;
            allow_wsz = IOMSK_OSDWSZ0;
            allow_ssz = IOMSK_OSDSSZ0;
            break;
        }
        if (1 == windex) {
            ofs_wsz = IOAT_OSDWSZ1;
            nr_addr = IORL_OSD_WIN1_DISPADDR;
            nr_h = IORL_OSD_WIN1_HEIGHT;
            nr_w = IORL_OSD_WIN1_WIDTH;
            ofs_ssz = IOAT_OSDSSZ1;
            nr_x = IORL_OSD_WIN1_POS_X;
            nr_y = IORL_OSD_WIN1_POS_Y;
            bts_addr = IOBTS_OSD_WIN1_DISPADDR;
            bts_h = IOBTS_OSD_WIN1_HEIGHT;
            bts_w = IOBTS_OSD_WIN1_WIDTH;
            bts_x = IOBTS_OSD_WIN1_POS_X;
            bts_y = IOBTS_OSD_WIN1_POS_Y;
            allow_wsz = IOMSK_OSDWSZ1;
            allow_ssz = IOMSK_OSDSSZ1;
            break;
        }
        if (2 == windex) {
            ofs_wsz = IOAT_OSDWSZ2;
            nr_addr = IORL_OSD_WIN2_DISPADDR;
            nr_h = IORL_OSD_WIN0_HEIGHT;
            nr_w = IORL_OSD_WIN0_WIDTH;
            ofs_ssz = IOAT_OSDSSZ2;
            nr_x = IORL_OSD_WIN2_POS_X;
            nr_y = IORL_OSD_WIN2_POS_Y;
            bts_addr = IOBTS_OSD_WIN2_DISPADDR;
            bts_h = IOBTS_OSD_WIN2_HEIGHT;
            bts_w = IOBTS_OSD_WIN2_WIDTH;
            bts_x = IOBTS_OSD_WIN2_POS_X;
            bts_y = IOBTS_OSD_WIN2_POS_Y;
            allow_wsz = IOMSK_OSDWSZ2;
            allow_ssz = IOMSK_OSDSSZ2;
            break;
        }
        if (3 == windex) {
            ofs_wsz = IOAT_OSDWSZ3;
            nr_addr = IORL_OSD_WIN3_DISPADDR;
            nr_h = IORL_OSD_WIN3_HEIGHT;
            nr_w = IORL_OSD_WIN3_WIDTH;
            ofs_ssz = IOAT_OSDSSZ3;
            nr_x = IORL_OSD_WIN3_POS_X;
            nr_y = IORL_OSD_WIN3_POS_Y;
            bts_addr = IOBTS_OSD_WIN3_DISPADDR;
            bts_h = IOBTS_OSD_WIN3_HEIGHT;
            bts_w = IOBTS_OSD_WIN3_WIDTH;
            bts_x = IOBTS_OSD_WIN3_POS_X;
            bts_y = IOBTS_OSD_WIN3_POS_Y;
            allow_wsz = IOMSK_OSDWSZ3;
            allow_ssz = IOMSK_OSDSSZ3;
            break;
        }
        return;
    } while (0);
    reg_wsz = fscaler_reg_setbits(reg_wsz, bts_addr, dindex, nr_addr);
    reg_wsz = fscaler_reg_setbits(reg_wsz, bts_h, h, nr_h);
    reg_wsz = fscaler_reg_setbits(reg_wsz, bts_w, w, nr_w);
    M_DEVELOPP("SCL: write %04x:%08x\n", (unsigned int)ofs_wsz, (unsigned int)reg_wsz);
    fscaler_reg_write(base, ofs_wsz, allow_wsz, 0, reg_wsz);
    reg_ssz = fscaler_reg_setbits(reg_ssz, bts_y, y, nr_y);
    reg_ssz = fscaler_reg_setbits(reg_ssz, bts_x, x, nr_x);
    M_DEVELOPP("SCL: write %04x:%08x\n", (unsigned int)ofs_ssz, (unsigned int)reg_ssz);
    fscaler_reg_write(base, ofs_ssz, allow_ssz, 0, reg_ssz);
}

#endif /* _FSCALER_HW_H_ */
