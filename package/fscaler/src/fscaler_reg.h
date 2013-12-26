/**
 * @file fscaler_reg.h
 * @brief Scaler register header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_REG_H_
#define _FSCALER_REG_H_

#include <asm/io.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16))
#include <linux/io.h>
#endif /* >= 2.6.16 */

#define BTS0    0x00000000
#define BTS1    0x00000001
#define BTS2    0x00000003
#define BTS3    0x00000007
#define BTS4    0x0000000f
#define BTS5    0x0000001f
#define BTS6    0x0000003f
#define BTS7    0x0000007f
#define BTS8    0x000000ff
#define BTS9    0x000001ff
#define BTS10   0x000003ff
#define BTS11   0x000007ff
#define BTS12   0x00000fff
#define BTS13   0x00001fff
#define BTS14   0x00003fff
#define BTS15   0x00007fff
#define BTS16   0x0000ffff
#define BTS17   0x0001ffff
#define BTS18   0x0003ffff
#define BTS19   0x0007ffff
#define BTS20   0x000fffff
#define BTS21   0x001fffff
#define BTS22   0x003fffff
#define BTS23   0x007fffff
#define BTS24   0x00ffffff
#define BTS25   0x01ffffff
#define BTS26   0x03ffffff
#define BTS27   0x07ffffff
#define BTS28   0x0fffffff
#define BTS29   0x1fffffff
#define BTS30   0x3fffffff
#define BTS31   0x7fffffff
#define BTS32   0xffffffff

#define BTS_NR(bts,nr)      ((bts) << ((nr) % BITS_PER_LONG))

#define bts1(nr)    BTS_NR(BTS1,nr)
#define bts2(nr)    BTS_NR(BTS2,nr)
#define bts3(nr)    BTS_NR(BTS3,nr)
#define bts4(nr)    BTS_NR(BTS4,nr)
#define bts5(nr)    BTS_NR(BTS5,nr)
#define bts6(nr)    BTS_NR(BTS6,nr)
#define bts7(nr)    BTS_NR(BTS7,nr)
#define bts8(nr)    BTS_NR(BTS8,nr)
#define bts9(nr)    BTS_NR(BTS9,nr)
#define bts10(nr)   BTS_NR(BTS10,nr)
#define bts11(nr)   BTS_NR(BTS11,nr)
#define bts12(nr)   BTS_NR(BTS12,nr)
#define bts13(nr)   BTS_NR(BTS13,nr)
#define bts14(nr)   BTS_NR(BTS14,nr)
#define bts15(nr)   BTS_NR(BTS15,nr)
#define bts16(nr)   BTS_NR(BTS16,nr)
#define bts17(nr)   BTS_NR(BTS17,nr)
#define bts18(nr)   BTS_NR(BTS18,nr)
#define bts19(nr)   BTS_NR(BTS19,nr)
#define bts20(nr)   BTS_NR(BTS20,nr)
#define bts21(nr)   BTS_NR(BTS21,nr)
#define bts22(nr)   BTS_NR(BTS22,nr)
#define bts23(nr)   BTS_NR(BTS23,nr)
#define bts24(nr)   BTS_NR(BTS24,nr)
#define bts25(nr)   BTS_NR(BTS25,nr)
#define bts26(nr)   BTS_NR(BTS26,nr)
#define bts27(nr)   BTS_NR(BTS27,nr)
#define bts28(nr)   BTS_NR(BTS28,nr)
#define bts29(nr)   BTS_NR(BTS29,nr)
#define bts30(nr)   BTS_NR(BTS30,nr)
#define bts31(nr)   BTS_NR(BTS31,nr)
#define bts32(nr)   BTS_NR(BTS32,nr)

#define msk1(p0)  \
    bts1(p0)
#define msk2(p0,p1)  \
    (msk1(p0)|msk1(p1))
#define msk4(p0,p1,p2,p3)  \
    (msk2(p0,p1)|msk2(p2,p3))
#define msk8(p0,p1,p2,p3,p4,p5,p6,p7)  \
    (msk4(p0,p1,p2,p3)|msk4(p4,p5,p6,p7))
#define msk12(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11)  \
    (msk4(p0,p1,p2,p3)|msk8(p4,p5,p6,p7,p8,p9,p10,p11))
#define msk16(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15)  \
    (msk8(p0,p1,p2,p3,p4,p5,p6,p7)|msk8(p8,p9,p10,p11,p12,p13,p14,p15))
#define msk24(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23)  \
    (msk8(p0,p1,p2,p3,p4,p5,p6,p7)|msk16(p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23))
#define msk32(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31)  \
    (msk16(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15)|msk16(p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31))

#define __get_reg_m(reg,msk)                    ((reg)&(msk))
#define __clear_reg_m(reg,msk)                  ((reg)&~(msk))
#define __set_reg_m(reg,msk,val)                (__clear_reg_m(reg,msk)|__get_reg_m(val,msk))
#define fscaler_reg_getbits(reg,bts,nr)         (__get_reg_m(reg,bts)>>(nr))
#define fscaler_reg_setbits(reg,bts,bval,nr)    __set_reg_m(reg,bts,(bval)<<(nr))

/* The scaler register I/O offsets */
#define IOAT_SCAREN          0x0000
#define IOAT_SCARUPD         0x0004
#define IOAT_SCARCLK         0x0008
#define IOAT_ASPMJ           0x001c
#define IOAT_SIZE0           0x0020
#define IOAT_SIZE1           0x0024
#define IOAT_SIZE2           0x0028
#define IOAT_DFORMAT         0x0038
#define IOAT_SRCSEL          0x003c
#define IOAT_SRCIF           0x0044
#define IOAT_SRCSIZE0        0x0050
#define IOAT_SRCSIZE1        0x0064
#define IOAT_SRCSIZE2        0x0068
#define IOAT_DDMA0           0x0200
#define IOAT_UDMA0           0x0204
#define IOAT_MEMSRC0         0x0208
#define IOAT_MEMSRC1         0x020c
#define IOAT_MEMSRCPIT       0x0210
#define IOAT_MEMSRC2         0x0214
#define IOAT_DMA1            0x021c
#define IOAT_MDEST0          0x0250
#define IOAT_MDEST1          0x0254
#define IOAT_MDEST2          0x0260
#define IOAT_MDEST3          0x0264
#define IOAT_MDEST4          0x0270
#define IOAT_MDEST5          0x0274
#define IOAT_OSDFONT         0x0300
#define IOAT_OSDDISP         0x0304
#define IOAT_OSDREAD         0x0308
#define IOAT_OSDEN           0x030C
#define IOAT_OSDPAT0         0x0310
#define IOAT_OSDPAT1         0x0314
#define IOAT_OSDPAT2         0x0318
#define IOAT_OSDPAT3         0x031c
#define IOAT_OSDPAT4         0x0320
#define IOAT_OSDPAT5         0x0324
#define IOAT_OSDPAT6         0x0328
#define IOAT_OSDCOR0         0x0330
#define IOAT_OSDWSZ0         0x0334
#define IOAT_OSDSSZ0         0x0338
#define IOAT_OSDFSZ0         0x033c
#define IOAT_OSDCOR1         0x0340
#define IOAT_OSDWSZ1         0x0344
#define IOAT_OSDSSZ1         0x0348
#define IOAT_OSDFSZ1         0x034c
#define IOAT_OSDCOR2         0x0350
#define IOAT_OSDWSZ2         0x0354
#define IOAT_OSDSSZ2         0x0358
#define IOAT_OSDFSZ2         0x035c
#define IOAT_OSDCOR3         0x0360
#define IOAT_OSDWSZ3         0x0364
#define IOAT_OSDSSZ3         0x0368
#define IOAT_OSDFSZ3         0x036c
#define IOAT_INITSTS         0x03e0
#define IOAT_INITMASK        0x03e4
#define IOAT_SCARSTS         0x03ec

// =============================================================================
// mask
#define IOMSK_SCAREN        (bts1(31)|bts1(28)|bts1(24)|bts1(12)|bts1(9)|bts1(8)|bts1(6)|bts1(4))
#define IOMSK_SCARUPD       (bts1(16)|bts1(1)|bts1(0))
#define IOMSK_SCARCLK       (bts1(28)|bts1(16)|bts1(4))
#define IOMSK_ASPMJ           bts2(8)
#define IOMSK_SIZE0         (bts13(16)|bts13(0))
#define IOMSK_SIZE1         (bts13(16)|bts13(0))
#define IOMSK_SIZE2         (bts13(16) |bts13(0))
#define IOMSK_DFORMAT       (msk1(13)|bts1(12)|bts4(8))
#define IOMSK_SRCSEL        (bts8(24)|bts1(9)|bts1(8)|bts1(7)|bts1(3))
#define IOMSK_SRCSIZE0      (bts13(16)|bts13(0))
#define IOMSK_SRCSIZE1      (bts13(16)|bts13(0))
#define IOMSK_SRCSIZE2      (bts13(16)|bts13(0))
#define IOMSK_DDMA0         (bts2(2)|bts2(0))
#define IOMSK_UDMA0         (bts3(28)|bts3(24)|bts1(16)|bts2(12)|bts2(8)|bts2(6)|bts2(4)|bts2(0))
#define IOMSK_MEMSRC0       (bts1(28)|bts3(24)|bts1(4)|bts3(0))
#define IOMSK_MEMSRC1         bts30(0)
#define IOMSK_MEMSRCPIT       bts13(0)
#define IOMSK_MEMSRC2       (bts13(16)|bts13(0))
#define IOMSK_DDMA0         (bts2(2)|bts2(0))
#define IOMSK_DMA1            bts11(0)
#define IOMSK_MDEST0          bts30(0)
#define IOMSK_MDEST1          bts30(0)
#define IOMSK_MDEST2          bts30(0)
#define IOMSK_MDEST3          bts30(0)
#define IOMSK_MDEST4          bts30(0)
#define IOMSK_MDEST5          bts30(0)
#define IOMSK_OSDFONT       (bts1(31)|bts13(16)|bts12(0))
#define IOMSK_OSDDISP       (bts1(31)|bts7(16)|bts8(0)) ///< Notice: The datasheet H264 4CH/8CH DVR SOC Rev:0.5 is worng, DISP_RAM_ADDR should be [22:16]
#define IOMSK_OSDREAD       (bts1(31)|bts1(30)|bts8(12)|bts12(0))
#define IOMSK_OSDEN         (bts1(16)|bts1(3)|bts1(2)|bts1(1)|bts1(0))
#define IOMSK_OSDPAT0       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT1       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT2       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT3       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT4       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT5       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDPAT6       (bts6(16)|bts5(8)|bts5(0))
#define IOMSK_OSDCOR0       (bts2(8)|bts3(4)|bts3(0))
#define IOMSK_OSDWSZ0       (bts7(16)|bts7(8)|bts7(0))
#define IOMSK_OSDSSZ0       (bts11(16)|bts11(0))
#define IOMSK_OSDFSZ0       (bts4(24)|bts4(16)|bts5(8)|bts4(0))
#define IOMSK_OSDCOR1       (bts2(8)|bts3(4)|bts3(0))
#define IOMSK_OSDWSZ1       (bts7(16)|bts7(8)|bts7(0))
#define IOMSK_OSDSSZ1       (bts11(16)|bts11(0))
#define IOMSK_OSDFSZ1       (bts4(24)|bts4(16)|bts5(8)|bts4(0))
#define IOMSK_OSDCOR2       (bts2(8)|bts3(4)|bts3(0))
#define IOMSK_OSDWSZ2       (bts7(16)|bts7(8)|bts7(0))
#define IOMSK_OSDSSZ2       (bts11(16)|bts11(0))
#define IOMSK_OSDFSZ2       (bts4(24)|bts4(16)|bts5(8)|bts4(0))
#define IOMSK_OSDCOR3       (bts2(8)|bts3(4)|bts3(0))
#define IOMSK_OSDWSZ3       (bts7(16)|bts7(8)|bts7(0))
#define IOMSK_OSDSSZ3       (bts11(16)|bts11(0))
#define IOMSK_OSDFSZ3       (bts4(24)|bts4(16)|bts5(8)|bts4(0))
#define IOMSK_INITSTS       (bts1(28)|bts1(27)|bts1(26)|bts1(25)|bts1(24)|bts1(11)|bts1(8)|bts1(5)|bts1(4))
#define IOMSK_INITMASK      (bts1(28)|bts1(27)|bts1(26)|bts1(24)|bts1(11)|bts1(8)|bts1(5)|bts1(4))
#define IOMSK_SCARSTS       (bts1(24)|bts3(16))

/*  The scaler register I/O rotaion length */
// SCAREN
#define IORL_SCAR_EN              31
#define IORL_ALBUM_EN             28
#define IORL_PIMG_EN              24
#define IORL_VP_EC_EN             12
#define IORL_US_EN                9
#define IORL_DITHER_EN            8
#define IORL_SP_EN                6
#define IORL_NLRS_EN              4
// SCARUPD
#define IORL_FORCE_CU             16
#define IORL_FORCE_UPDATE         1
#define IORL_SYNC_REG_UPDATE      0
// SCARCLK
#define IORL_SW_RST               28
#define IORL_INT_SYNC_CLR         16
#define IORL_VP_CLK_INV           4
// ASPMJ
#define IORL_ASPECT_MAJOR         8
// SIZE0
#define IORL_TARGET_HEIGHT        16
#define IORL_TARGET_WIDTH         0
// SIZE1
#define IORL_CROP_Y               16
#define IORL_CROP_X               0
// SIZE2
#define IORL_CROP_HEIGHT          16
#define IORL_CROP_WIDTH           0
// DFORMAT
#define IORL_DEST_YC_SWAP         13
#define IORL_DEST_RB_SWAP         12
#define IORL_DEST_FORMAT          8
// SRCSEL
#define IORL_RS_H_BLANK_PIX       24
#define IORL_SWC_CENTER_XY        9
#define IORL_VP_RANGE             8
#define IORL_US_SRC_SEL           7
#define IORL_IMG_SRC_SEL          3
// SRCIF
#define IORL_VP_PROG          19
// SRCSIZE0
#define IORL_VP_SRC_HEIGHT        16
#define IORL_VP_SRC_WIDTH         0
// SRCSIZE1
#define IORL_SWC_Y                16
#define IORL_SWC_X                0
// SRCSIZE2
#define IORL_SWC_HEIGHT           16
#define IORL_SWC_WIDTH            0
// DDMA0
#define IORL_DMA_RLEN             2
// UDMA0
#define IORL_DMA_FIFO_WM          28
#define IORL_DMA_MST_EN           16
#define IORL_DMA_ORDER            12
#define IORL_PDMA_SPLIT           8
#define IORL_DMA_CHROMA_WLEN      6
#define IORL_DMA_LUMA_WLEN        4
#define IORL_DMA_WLEN             0
// MEMSRC0
#define IORL_MEM_SRC_FIELD        28
#define IORL_MEM_SRC_HBLANK       24
#define IORL_MEM_SRC_RB_SWAP      4
#define IORL_MEM_SRC_FORMAT       0
// MEMSRC1
#define IORL_MEM_SRC_ST           0
// MEMSRCPIT
#define IORL_MEM_SRC_PIT          0
// MEMSRC2
#define IORL_MEM_SRC_HEIGHT       16
#define IORL_MEM_SRC_WIDTH        0
// DMA1
#define IORL_SUB_WIN_PIT          0
// MDEST0
#define IORL_MEM_DEST_YST0        0
// MDEST1
#define IORL_MEM_DEST_YST1        0
// MDEST2
#define IORL_MEM_DEST_CBST0       0
// MDEST3
#define IORL_MEM_DEST_CBST1       0
// MDEST4
#define IORL_MEM_DEST_CRST0       0
// MDEST5
#define IORL_MEM_DEST_CRST1       0
// OSDFONT
#define IORL_FONT_RAM_READ         31
#define IORL_FONT_RAM_ADDR         16
#define IORL_FONT_RAM_DATA         0
// OSDDISP
#define IORL_DISP_RAM_READ        31
#define IORL_DISP_RAM_ADDR        16
#define IORL_DISP_RAM_DATA        0
// OSDREAD
#define IORL_DISP_RAM_READ        31
#define IORL_DISP_RAM_ADDR        16
#define IORL_DISP_RAM_DATA        0
// OSDEN
#define IORL_OSD_EDGE_SMTH_EN     16
#define IORL_OSD_WIN3_EN          3
#define IORL_OSD_WIN2_EN          2
#define IORL_OSD_WIN1_EN          1
#define IORL_OSD_WIN0_EN          0
// OSDPAT0
#define IORL_OSD_PAL0_Y           16
#define IORL_OSD_PAL0_CB          8
#define IORL_OSD_PAL0_CR          0
// OSDPAT1
#define IORL_OSD_PAL1_Y           16
#define IORL_OSD_PAL1_CB          8
#define IORL_OSD_PAL1_CR          0
// OSDPAT2
#define IORL_OSD_PAL2_Y           16
#define IORL_OSD_PAL2_CB          8
#define IORL_OSD_PAL2_CR          0
// OSDPAT3
#define IORL_OSD_PAL3_Y           16
#define IORL_OSD_PAL3_CB          8
#define IORL_OSD_PAL3_CR          0
// OSDPAT4
#define IORL_OSD_PAL4_Y           16
#define IORL_OSD_PAL4_CB          8
#define IORL_OSD_PAL4_CR          0
// OSDPAT5
#define IORL_OSD_PAL5_Y           16
#define IORL_OSD_PAL5_CB          8
#define IORL_OSD_PAL5_CR          0
// OSDPAT6
#define IORL_OSD_PAL6_Y           16
#define IORL_OSD_PAL6_CB          8
#define IORL_OSD_PAL6_CR          0
// OSDCOR0
#define IORL_OSD_WIN0_TRAN        8
#define IORL_OSD_WIN0_BWCOLOR     4
#define IORL_OSD_WIN0_FWCOLOR     0
// OSDWSZ0
#define IORL_OSD_WIN0_DISPADDR    16
#define IORL_OSD_WIN0_HEIGHT      8
#define IORL_OSD_WIN0_WIDTH       0
// OSDSSZ0
#define IORL_OSD_WIN0_POS_Y       16
#define IORL_OSD_WIN0_POS_X       0
// OSDFSZ0
#define IORL_OSD_WIN0_FSPACE_ROW       24
#define IORL_OSD_WIN0_FSPACE_COL       16
#define IORL_OSD_WIN0_FONT_H           8
#define IORL_OSD_WIN0_FONT_W           0
// OSDCOR1
#define IORL_OSD_WIN1_TRAN        8
#define IORL_OSD_WIN1_BWCOLOR     4
#define IORL_OSD_WIN1_FWCOLOR     0
// OSDWSZ1
#define IORL_OSD_WIN1_DISPADDR    16
#define IORL_OSD_WIN1_HEIGHT      8
#define IORL_OSD_WIN1_WIDTH       0
// OSDSSZ1
#define IORL_OSD_WIN1_POS_Y       16
#define IORL_OSD_WIN1_POS_X       0
// OSDFSZ1
#define IORL_OSD_WIN1_FSPACE_ROW       24
#define IORL_OSD_WIN1_FSPACE_COL       16
#define IORL_OSD_WIN1_FONT_H           8
#define IORL_OSD_WIN1_FONT_W           0
// OSDCOR2
#define IORL_OSD_WIN2_TRAN        8
#define IORL_OSD_WIN2_BWCOLOR     4
#define IORL_OSD_WIN2_FWCOLOR     0
// OSDWSZ2
#define IORL_OSD_WIN2_DISPADDR    16
#define IORL_OSD_WIN2_HEIGHT      8
#define IORL_OSD_WIN2_WIDTH       0
// OSDSSZ2
#define IORL_OSD_WIN2_POS_Y       16
#define IORL_OSD_WIN2_POS_X       0
// OSDFSZ2
#define IORL_OSD_WIN2_FSPACE_ROW       24
#define IORL_OSD_WIN2_FSPACE_COL       16
#define IORL_OSD_WIN2_FONT_H           8
#define IORL_OSD_WIN2_FONT_W           0
// OSDCOR3
#define IORL_OSD_WIN3_TRAN        8
#define IORL_OSD_WIN3_BWCOLOR     4
#define IORL_OSD_WIN3_FWCOLOR     0
// OSDWSZ3
#define IORL_OSD_WIN3_DISPADDR    16
#define IORL_OSD_WIN3_HEIGHT      8
#define IORL_OSD_WIN3_WIDTH       0
// OSDSSZ3
#define IORL_OSD_WIN3_POS_Y       16
#define IORL_OSD_WIN3_POS_X       0
// OSDFSZ3
#define IORL_OSD_WIN3_FSPACE_ROW       24
#define IORL_OSD_WIN3_FSPACE_COL       16
#define IORL_OSD_WIN3_FONT_H           8
#define IORL_OSD_WIN3_FONT_W           0
// INITSTS
#define IORL_DMA_OVF_ERR          28
#define IORL_UAHB_ERR             27
#define IORL_NULL_ERR             26
#define IORL_FD_QUE_STS           25
#define IORL_FD_STS               24
#define IORL_DAHB_ERR             11
#define IORL_DAHB_IDLE_STS        8
#define IORL_FM_END_STS           5
#define IORL_FM_START_STS         4
// INITMASK
#define IORL_DMA_OVF_MASK         28
#define IORL_UAHB_ERR_MASK        27
#define IORL_NULL_ERR_MASK        26
#define IORL_FD_STS_MASK          24
#define IORL_DAHB_ERR_MASK        11
#define IORL_DAHB_IDLE_MASK       8
#define IORL_FM_END_MASK          5
#define IORL_FM_START_MASK        4
// SCARSTS
#define IORL_CU_BUSY              24
#define IORL_VP_STA               16

// =============================================================================
// bts
// SCAREN
#define IOBTS_SCAR_EN             bts1(31)
#define IOBTS_ALBUM_EN            bts1(28)
#define IOBTS_PIMG_EN             bts1(24)
#define IOBTS_VP_EC_EN            bts1(12)
#define IOBTS_US_EN               bts1(9)
#define IOBTS_DITHER_EN           bts1(8)
#define IOBTS_SP_EN               bts1(6)
#define IOBTS_NLRS_EN             bts1(4)
// SCARUPD
#define IOBTS_FORCE_CU            bts1(16)
#define IOBTS_FORCE_UPDATE        bts1(1)
#define IOBTS_SYNC_REG_UPDATE     bts1(0)
// SCARCLK
#define IOBTS_SW_RST              bts1(28)
#define IOBTS_INT_SYNC_CLR        bts1(16)
#define IOBTS_VP_CLK_INV          bts1(4)
// ASPMJ
#define IOBTS_ASPECT_MAJOR        bts2(8)
// SIZE0
#define IOBTS_TARGET_HEIGHT       bts13(16)
#define IOBTS_TARGET_WIDTH        bts13(0)
// SIZE1
#define IOBTS_CROP_Y              bts13(16)
#define IOBTS_CROP_X              bts13(0)
// SIZE2
#define IOBTS_CROP_HEIGHT         bts13(16)
#define IOBTS_CROP_WIDTH          bts13(0)
// DFORMAT
#define IOBTS_DEST_YC_SWAP        bts1(13)
#define IOBTS_DEST_RB_SWAP        bts1(12)
#define IOBTS_DEST_FORMAT         bts4(8)
// SRCSEL
#define IOBTS_RS_H_BLANK_PIX      bts8(24)
#define IOBTS_SWC_CENTER_XY       bts1(9)
#define IOBTS_VP_RANGE            bts1(8)
#define IOBTS_US_SRC_SEL          bts1(7)
#define IOBTS_IMG_SRC_SEL         bts1(3)
// SRCSIZE0
#define IOBTS_VP_SRC_HEIGHT       bts13(16)
#define IOBTS_VP_SRC_WIDTH        bts13(0)
// SRCSIZE1
#define IOBTS_SWC_Y               bts13(16)
#define IOBTS_SWC_X               bts13(0)
// SRCSIZE2
#define IOBTS_SWC_HEIGHT          bts13(16)
#define IOBTS_SWC_WIDTH           bts13(0)
// DDMA0
#define IOBTS_DMA_RLEN            bts2(2)
// UDMA0
#define IOBTS_DMA_FIFO_WM         bts3(28)
#define IOBTS_DMA_MST_EN          bts1(16)
#define IOBTS_DMA_ORDER           bts2(12)
#define IOBTS_PDMA_SPLIT          bts2(8)
#define IOBTS_DMA_CHROMA_WLEN     bts2(6)
#define IOBTS_DMA_LUMA_WLEN       bts2(4)
#define IOBTS_DMA_WLEN            bts2(0)
// MEMSRC0
#define IOBTS_MEM_SRC_FIELD       bts1(28)
#define IOBTS_MEM_SRC_HBLANK      bts3(24)
#define IOBTS_MEM_SRC_RB_SWAP     bts1(4)
#define IOBTS_MEM_SRC_FORMAT      bts3(0)
// MEMSRC1
#define IOBTS_MEM_SRC_ST          bts30(0)
// MEMSRCPIT
#define IOBTS_MEM_SRC_PIT         bts13(0)
// MEMSRC2
#define IOBTS_MEM_SRC_HEIGHT      bts13(16)
#define IOBTS_MEM_SRC_WIDTH       bts13(0)
// DMA1
#define IOBTS_SUB_WIN_PIT         bts11(0)
// MDEST0
#define IOBTS_MEM_DEST_YST0       bts30(0)
// MDEST1
#define IOBTS_MEM_DEST_YST1       bts30(0)
// MDEST2
#define IOBTS_MEM_DEST_CBST0      bts30(0)
// MDEST3
#define IOBTS_MEM_DEST_CBST1      bts30(0)
// MDEST4
#define IOBTS_MEM_DEST_CRST0      bts30(0)
// MDEST5
#define IOBTS_MEM_DEST_CRST1      bts30(0)
// OSDFONT
#define IOBTS_FONT_RAM_READ         bts1(31)
#define IOBTS_FONT_RAM_ADDR         bts13(16)
#define IOBTS_FONT_RAM_DATA         bts12(0)
// OSDDISP
#define IOBTS_DISP_RAM_READ       bts1(31)
#define IOBTS_DISP_RAM_ADDR       bts7(16)      ///< Notice: The datasheet H264 4CH/8CH DVR SOC Rev:0.5 is worng, DISP_RAM_ADDR should be [22:16]
#define IOBTS_DISP_RAM_DATA       bts8(0)
// OSDREAD
#define IOBTS_DISP_RAM_BUSY       bts1(31)
#define IOBTS_FONT_RAM_BUSY       bts1(30)
#define IOBTS_DISP_RAM_DATAOUT    bts8(12)
#define IOBTS_FONT_RAM_DATAOUT    bts12(0)
// OSDEN
#define IOBTS_OSD_EFGE_SMTH_EN    bts1(16)
#define IOBTS_OSD_WIN3_EN         bts1(3)
#define IOBTS_OSD_WIN2_EN         bts1(2)
#define IOBTS_OSD_WIN1_EN         bts1(1)
#define IOBTS_OSD_WIN0_EN         bts1(0)
// OSDPAT0
#define IOBTS_OSD_PAL0_Y          bts6(16)
#define IOBTS_OSD_PAL0_CB         bts5(8)
#define IOBTS_OSD_PAL0_CR         bts5(0)
// OSDPAT1
#define IOBTS_OSD_PAL1_Y          bts6(16)
#define IOBTS_OSD_PAL1_CB         bts5(8)
#define IOBTS_OSD_PAL1_CR         bts5(0)
// OSDPAT2
#define IOBTS_OSD_PAL2_Y          bts6(16)
#define IOBTS_OSD_PAL2_CB         bts5(8)
#define IOBTS_OSD_PAL2_CR         bts5(0)
// OSDPAT3
#define IOBTS_OSD_PAL3_Y          bts6(16)
#define IOBTS_OSD_PAL3_CB         bts5(8)
#define IOBTS_OSD_PAL3_CR         bts5(0)
// OSDPAT4
#define IOBTS_OSD_PAL4_Y          bts6(16)
#define IOBTS_OSD_PAL4_CB         bts5(8)
#define IOBTS_OSD_PAL4_CR         bts5(0)
// OSDPAT5
#define IOBTS_OSD_PAL5_Y          bts6(16)
#define IOBTS_OSD_PAL5_CB         bts5(8)
#define IOBTS_OSD_PAL5_CR         bts5(0)
// OSDPAT6
#define IOBTS_OSD_PAL6_Y          bts6(16)
#define IOBTS_OSD_PAL6_CB         bts5(8)
#define IOBTS_OSD_PAL6_CR         bts5(0)
// OSDCOR0
#define IOBTS_OSD_WIN0_TRAN       bts2(8)
#define IOBTS_OSD_WIN0_BWCOLOR    bts3(4)
#define IOBTS_OSD_WIN0_FWCOLOR    bts3(0)
// OSDWSZ0
#define IOBTS_OSD_WIN0_DISPADDR  bts7(16)
#define IOBTS_OSD_WIN0_HEIGHT     bts7(8)
#define IOBTS_OSD_WIN0_WIDTH      bts7(0)
// OSDSSZ0
#define IOBTS_OSD_WIN0_POS_Y      bts11(16)
#define IOBTS_OSD_WIN0_POS_X      bts11(0)
// OSDFSZ0
#define IOBTS_OSD_WIN0_FSPACE_ROW       bts4(24)
#define IOBTS_OSD_WIN0_FSPACE_COL       bts4(16)
#define IOBTS_OSD_WIN0_FONT_H           bts5(8)
#define IOBTS_OSD_WIN0_FONT_W           bts4(0)
// OSDCOR1
#define IOBTS_OSD_WIN1_TRAN       bts2(8)
#define IOBTS_OSD_WIN1_BWCOLOR    bts3(4)
#define IOBTS_OSD_WIN1_FWCOLOR    bts3(0)
// OSDWSZ1
#define IOBTS_OSD_WIN1_DISPADDR  bts7(16)
#define IOBTS_OSD_WIN1_HEIGHT     bts7(8)
#define IOBTS_OSD_WIN1_WIDTH      bts7(0)
// OSDSSZ1
#define IOBTS_OSD_WIN1_POS_Y      bts11(16)
#define IOBTS_OSD_WIN1_POS_X      bts11(0)
// OSDFSZ1
#define IOBTS_OSD_WIN1_FSPACE_ROW       bts4(24)
#define IOBTS_OSD_WIN1_FSPACE_COL       bts4(16)
#define IOBTS_OSD_WIN1_FONT_H           bts5(8)
#define IOBTS_OSD_WIN1_FONT_W           bts4(0)
// OSDCOR2
#define IOBTS_OSD_WIN2_TRAN       bts2(8)
#define IOBTS_OSD_WIN2_BWCOLOR    bts3(4)
#define IOBTS_OSD_WIN2_FWCOLOR    bts3(0)
// OSDWSZ2
#define IOBTS_OSD_WIN2_DISPADDR  bts7(16)
#define IOBTS_OSD_WIN2_HEIGHT     bts7(8)
#define IOBTS_OSD_WIN2_WIDTH      bts7(0)
// OSDSSZ2
#define IOBTS_OSD_WIN2_POS_Y      bts11(16)
#define IOBTS_OSD_WIN2_POS_X      bts11(0)
// OSDFSZ2
#define IOBTS_OSD_WIN2_FSPACE_ROW       bts4(24)
#define IOBTS_OSD_WIN2_FSPACE_COL       bts4(16)
#define IOBTS_OSD_WIN2_FONT_H           bts5(8)
#define IOBTS_OSD_WIN2_FONT_W           bts4(0)
// OSDCOR3
#define IOBTS_OSD_WIN3_TRAN       bts2(8)
#define IOBTS_OSD_WIN3_BWCOLOR    bts3(4)
#define IOBTS_OSD_WIN3_FWCOLOR    bts3(0)
// OSDWSZ3
#define IOBTS_OSD_WIN3_DISPADDR  bts7(16)
#define IOBTS_OSD_WIN3_HEIGHT     bts7(8)
#define IOBTS_OSD_WIN3_WIDTH      bts7(0)
// OSDSSZ3
#define IOBTS_OSD_WIN3_POS_Y      bts11(16)
#define IOBTS_OSD_WIN3_POS_X      bts11(0)
// OSDFSZ3
#define IOBTS_OSD_WIN3_FSPACE_ROW       bts4(24)
#define IOBTS_OSD_WIN3_FSPACE_COL       bts4(16)
#define IOBTS_OSD_WIN3_FONT_H           bts5(8)
#define IOBTS_OSD_WIN3_FONT_W           bts4(0)
// INITSTS
#define IOBTS_DMA_OVF_ERR         bts1(28)
#define IOBTS_UAHB_ERR            bts1(27)
#define IOBTS_NULL_ERR            bts1(26)
#define IOBTS_FD_QUE_STS          bts1(25)
#define IOBTS_FD_STS              bts1(24)
#define IOBTS_DAHB_ERR            bts1(11)
#define IOBTS_DAHB_IDLE_STS       bts1(8)
#define IOBTS_FM_END_STS          bts1(5)
#define IOBTS_FM_START_STS        bts1(4)
// INTMASK
#define IOBTS_DMA_OVF_MASK        bts1(28)
#define IOBTS_UAHB_ERR_MASK       bts1(27)
#define IOBTS_NULL_ERR_MASK       bts1(26)
#define IOBTS_FD_STS_MASK         bts1(24)
#define IOBTS_DAHB_ERR_MASK       bts1(11)
#define IOBTS_DAHB_IDLE_MASK      bts1(8)
#define IOBTS_FM_END_MASK         bts1(5)
#define IOBTS_FM_START_MASK       bts1(4)
// SCARSTS
#define IOBTS_CU_BUSY             bts1(24)
#define IOBTS_VP_STA              bts3(16)

static inline void fscaler_reg_iowrite(unsigned int base, unsigned int ofs, unsigned long val)
{
    iowrite32(val, base + ofs);
}

static inline unsigned long fscaler_reg_ioread(unsigned int base, unsigned int ofs)
{
    return (unsigned long)ioread32(base + ofs);
}

static inline void fscaler_reg_ioset(unsigned int base, unsigned int ofs, unsigned int nr)
{
    set_bit(nr, (volatile unsigned long *)(base + ofs));
}

static inline void fscaler_reg_ioclear(unsigned int base, unsigned int ofs, unsigned int nr)
{
    clear_bit(nr, (volatile unsigned long *)(base + ofs));
}

static inline void fscaler_reg_iochange(unsigned int base, unsigned int ofs, unsigned int nr)
{
    change_bit(nr, (volatile unsigned long *)(base + ofs));
}

static inline int fscaler_reg_iotest(unsigned int base, unsigned int ofs, unsigned int nr)
{
    return test_bit(nr, (volatile unsigned long *)(base + ofs));
}

static inline unsigned long fscaler_reg_read(unsigned int base, unsigned int ofs,
                                             unsigned long allow, unsigned long deny)
{
    return (unsigned long)__get_reg_m(fscaler_reg_ioread(base, ofs), allow & (~deny));
}

static inline void fscaler_reg_write(unsigned int base, unsigned int ofs, unsigned long allow,
                                     unsigned long deny, unsigned long val)
{
    fscaler_reg_iowrite(base, ofs,
                        __set_reg_m(fscaler_reg_ioread(base, ofs), allow & (~deny), val));
}

#endif /* _FSCALER_REG_H_ */
