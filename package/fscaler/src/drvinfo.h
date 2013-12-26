/**
 * @file drvinfo.h
 *  The driver information definition from each video entity
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.7 $
 * $Date: 2011/06/07 12:24:00 $
 *
 * ChangeLog:
 *  $Log: drvinfo.h,v $
 *  Revision 1.7  2011/06/07 12:24:00  tuba_ch
 *  videograph
 *
 *  Revision 1.6  2011/02/24 06:18:01  ben
 *  add mjd_InInfo/mp4d_InInfo structure
 *
 *  Revision 1.5  2010/10/28 08:13:49  ben
 *  remove NonRef from MCP200Rpt_PARAMs
 *
 *  Revision 1.4  2010/10/26 10:25:18  ben
 *  output NonRef to next entity
 *
 *  Revision 1.3  2010/10/26 09:59:04  ben
 *  1. 264 encoder:
 *    a. support to specify ForceNonRef for each frame
 *    b. support to specify roi_x, roi_y for for each frame
 *    c. report NonRef of Current frame
 *  2. mpeg4/jpeg encoder:
 *    support to specify roi_x, roi_y for for each frame
 *
 *  Revision 1.2  2010/10/19 08:22:49  ben
 *  Add key-frame for mpeg4-encoder post-frame-processing
 *
 *  Revision 1.1  2010/09/08 06:16:45  ivan
 *  initial add for 2.6.28
 *
 *  Revision 1.12  2010/09/06 08:10:47  ivan
 *  initial version
 *
 *  Revision 1.11  2010/08/20 03:35:25  ivan
 *  add function comment
 *
 *  Revision 1.119  2010/08/03 08:44:52  ivan
 *  Make source code compatible with linux 2.6.14 and 2.6.28
 *
 */
 
#ifndef _DRVINFO_H_
#define _DRVINFO_H_

typedef struct {
  	unsigned int DatainTag;
    unsigned int DataSize;
    unsigned int DataOffset;
    unsigned int Reserved0;
    unsigned int Reserved1;
    unsigned int Reserved2;
    unsigned int Reserved3;
    unsigned int Reserved4;
}Datain_info;

typedef struct {
    unsigned int EncoderInTag;
    unsigned int Qp;
    int ForceIntra;
    unsigned int Reserved0;
    unsigned int Reserved1;
    unsigned int Reserved2;
    unsigned int Reserved3;
    unsigned int Reserved4;
}EncoderIn_info;

                     
typedef struct {
	unsigned int EncoderOutTag;
	unsigned int BitstreamOffset;
	unsigned int BitstreamSize;
	unsigned int MbInfoOffset;
	unsigned int MbInfoLength;
	unsigned int Keyframe;
	int NonRef;
	unsigned int HalfFrameOffset;
}EncoderOut_info;

typedef struct
{
	int ForceIntra;
	int Qp;
	int ForceNonRef;
							// specify this P frame not to be reference. skip this when current frame is I-frame
							// 1: Force this P frame not to be reference
							// 0: Force this P frame to be reference
							// -1:let encoder decide, reference to FWD_Mode parameter.
	int roi_x;// if < 0, skip roi_x & roi_y
	int roi_y;// if < 0, skip roi_x & roi_y
} MCP200P_PARAMs;

typedef struct
{
	int keyframe;
	int length;
	unsigned int Qp;
	int reserved3;
} MCP200Rpt_PARAMs;

typedef struct
{
	int quant;
	unsigned char * quant_intra_matrix;	// Custom intra quantization matrix when MPEG4 quant is enabled
	unsigned char * quant_inter_matrix;	// Custom inter quantization matrix when MPEG4 quant is enabled 
	int intra;		/* in/out variable
							when in: tells Gm if the frame must be encoded as an intra frame
							1: forces the encoder  to create a keyframe.
							0:  forces the  encoder not to  create a keyframe.
							-1: let   the  encoder   decide  (based   on the ip_interval).
							when out : When first set to -1, the encoder returns the effective keyframe state of the frame. */
	int length;		//Output MPEG4 bitstream length (bytes)
	int module_time_base; //VOP header module_time_base, using magic number 0x55aaXXXX, XXXX is the number
	int roi_x;// if < 0, skip roi_x & roi_y
	int roi_y;// if < 0, skip roi_x & roi_y
} mp4e_process;

typedef struct {
	unsigned int u32ImageQuality;	//[IP]: The range for this quality setting is between
														//		1 (worst quality) and 100 (best quality).
	unsigned int u8JPGPIC; 			//[IP]: specify this is a JPG or MJPG
														//		1: JPG, (write huffman tables)
														//		0: MJPG, (no write huffman tables to shrink bitstream size)
	unsigned int *luma_qtbl;		// if not NULL, a pointer to specify 64 elements of user define luma quant. table
	unsigned int *chroma_qtbl;	// if not NULL, a pointer to specify 64 elements of user define chroma quant. table
	int roi_x;// if < 0, skip roi_x & roi_y
	int roi_y;// if < 0, skip roi_x & roi_y
} mje_process;


typedef struct {
    unsigned int DecoderInTag;
    unsigned int BitstreamSize;
    unsigned int Reserved0;
    unsigned int Reserved1;
    unsigned int Reserved2;
    unsigned int Reserved3;
    unsigned int Reserved4;
    unsigned int Reserved5;
}DecoderIn_info;

                     

typedef struct {
    unsigned int DecoderOutTag;
    unsigned int YUVFormat;
    unsigned int HeightOrg;
    unsigned int Width;
    unsigned int Height;
    unsigned int Reserved5;
    unsigned int Reserved6;
    unsigned int Reserved7;
}DecoderOut_info;

typedef struct{
	unsigned int Tag;
	unsigned int bs_offset;
	unsigned int bs_length; // encoder fail if 0
	unsigned int Reserved3;
	unsigned int Reserved4;
	unsigned int Reserved5;
	unsigned int Reserved6;
	unsigned int Reserved7;
} mje_OutInfo;
typedef struct {
    unsigned int Tag;
    unsigned int bs_length;
    unsigned int Reserved2;
    unsigned int Reserved3;
    unsigned int Reserved4;
    unsigned int Reserved5;
    unsigned int Reserved6;
    unsigned int Reserved7;
} mjd_InInfo;
typedef struct {
	unsigned int Tag;
	unsigned int NumofComponents;
	unsigned int img_width;
	unsigned int img_height;
	unsigned int sample;				// JDS_YUV enum
	int err;									// JD_ERR enum
	unsigned int Reserved6;
	unsigned int Reserved7;
} mjd_OutInfo;

typedef struct {
	unsigned int Tag;
	unsigned int bs_offset;
	unsigned int bs_length;		// encoder fail if 0
	unsigned int Keyframe;
	unsigned int Reserved4;
	unsigned int Reserved5;
	unsigned int Reserved6;
	unsigned int Reserved7;
} mp4e_OutInfo;
typedef struct {
    unsigned int Tag;
    unsigned int bs_length;
    unsigned int Reserved2;
    unsigned int Reserved3;
    unsigned int Reserved4;
    unsigned int Reserved5;
    unsigned int Reserved6;
    unsigned int Reserved7;
} mp4d_InInfo;
typedef struct {
	unsigned int Tag;
	unsigned int YUV_offset;
	unsigned int u32VopWidth;
	unsigned int u32VopHeight;
	unsigned int u32UsedBytes;
	unsigned int Reserved5;
	unsigned int Reserved6;
	unsigned int Reserved7;
} mp4d_OutInfo;

typedef struct {
	unsigned int Tag;
	unsigned int Reserved1;
	unsigned int Reserved2;
	unsigned int Reserved3;
	unsigned int Reserved4;
	unsigned int Reserved5;
	unsigned int Reserved6;
	unsigned int Reserved7;
} osd_OutInfo;

/* List of entity

datain_0 fd=0xaab00
datain_1 fd=0xaab01
datain_2 fd=0xaab02
datain_3 fd=0xaab03
datain_4 fd=0xaab04
datain_5 fd=0xaab05
datain_6 fd=0xaab06
datain_7 fd=0xaab07

dataout_0 fd=0xaaa00
dataout_1 fd=0xaaa01
dataout_2 fd=0xaaa02
dataout_3 fd=0xaaa03
dataout_4 fd=0xaaa04
dataout_5 fd=0xaaa05
dataout_6 fd=0xaaa06
dataout_7 fd=0xaaa07

lcd0_vg fd=0xa3200

vcap200_lli-C0-P0 fd=0x510000
vcap200_lli-C0-P1 fd=0x510001
vcap200_lli-C0-P2 fd=0x510002
vcap200_lli-C0-P3 fd=0x510003
vcap200_lli-C1-P0 fd=0x510010
vcap200_lli-C1-P1 fd=0x510011
vcap200_lli-C1-P2 fd=0x510012
vcap200_lli-C1-P3 fd=0x510013
vcap200_lli-C2-P0 fd=0x510020
vcap200_lli-C2-P1 fd=0x510021
vcap200_lli-C2-P2 fd=0x510022
vcap200_lli-C2-P3 fd=0x510023
vcap200_lli-C3-P0 fd=0x510030
vcap200_lli-C3-P1 fd=0x510031
vcap200_lli-C3-P2 fd=0x510032
vcap200_lli-C3-P3 fd=0x510033
vcap200_lli-C4-P0 fd=0x510040
vcap200_lli-C4-P1 fd=0x510041
vcap200_lli-C4-P2 fd=0x510042
vcap200_lli-C4-P3 fd=0x510043
vcap200_lli-C5-P0 fd=0x510050
vcap200_lli-C5-P1 fd=0x510051
vcap200_lli-C5-P2 fd=0x510052
vcap200_lli-C5-P3 fd=0x510053
vcap200_lli-C6-P0 fd=0x510060
vcap200_lli-C6-P1 fd=0x510061
vcap200_lli-C6-P2 fd=0x510062
vcap200_lli-C6-P3 fd=0x510063
vcap200_lli-C7-P0 fd=0x510070
vcap200_lli-C7-P1 fd=0x510071
vcap200_lli-C7-P2 fd=0x510072
vcap200_lli-C7-P3 fd=0x510073

f264enc_0 fd=0xa1700          
f264enc_1 fd=0xa1701          
f264enc_2 fd=0xa1702          
f264enc_3 fd=0xa1703          
f264enc_4 fd=0xa1704          
f264enc_5 fd=0xa1705          
f264enc_6 fd=0xa1706          
f264enc_7 fd=0xa1707          
f264enc_8 fd=0xa1708          
f264enc_9 fd=0xa1709          
f264enc_10 fd=0xa170a         
f264enc_11 fd=0xa170b         
f264enc_12 fd=0xa170c         
f264enc_13 fd=0xa170d         
f264enc_14 fd=0xa170e         
f264enc_15 fd=0xa170f         
f264dec_0 fd=0xa1600          
f264dec_1 fd=0xa1601          
f264dec_2 fd=0xa1602          
f264dec_3 fd=0xa1603          
f264dec_4 fd=0xa1604          
f264dec_5 fd=0xa1605          
f264dec_6 fd=0xa1606          
f264dec_7 fd=0xa1607          
f264dec_8 fd=0xa1608          
f264dec_9 fd=0xa1609          
f264dec_10 fd=0xa160a         
f264dec_11 fd=0xa160b         
f264dec_12 fd=0xa160c         
f264dec_13 fd=0xa160d         
f264dec_14 fd=0xa160e         
f264dec_15 fd=0xa160f         

scaler_0 fd=0xa1e00
scaler_1 fd=0xa1e01
scaler_2 fd=0xa1e02
scaler_3 fd=0xa1e03
scaler_4 fd=0xa1e04
scaler_5 fd=0xa1e05
scaler_6 fd=0xa1e06
scaler_7 fd=0xa1e07
scaler_8 fd=0xa1e08
scaler_9 fd=0xa1e09
scaler_10 fd=0xa1e0a
scaler_11 fd=0xa1e0b
scaler_12 fd=0xa1e0c
scaler_13 fd=0xa1e0d
scaler_14 fd=0xa1e0e
scaler_15 fd=0xa1e0f

ftdi210_vg_0 fd=0xa3c00
ftdi210_vg_1 fd=0xa3c01
ftdi210_vg_2 fd=0xa3c02
ftdi210_vg_3 fd=0xa3c03
ftdi210_vg_4 fd=0xa3c04
ftdi210_vg_5 fd=0xa3c05
ftdi210_vg_6 fd=0xa3c06
ftdi210_vg_7 fd=0xa3c07
ftdi210_vg_8 fd=0xa3c08
ftdi210_vg_9 fd=0xa3c09
ftdi210_vg_10 fd=0xa3c0a
ftdi210_vg_11 fd=0xa3c0b
ftdi210_vg_12 fd=0xa3c0c
ftdi210_vg_13 fd=0xa3c0d
ftdi210_vg_14 fd=0xa3c0e
ftdi210_vg_15 fd=0xa3c0f


mjpeg_decoder_0 fd=0xa2800
mjpeg_decoder_1 fd=0xa2801
mjpeg_encoder_0 fd=0xa2900
mjpeg_encoder_1 fd=0xa2901

Queue:
Queue disp1_que type 0 num 5 element 1572864 ddr 0
Queue DIdisp1_que type 1 num 5 element 1572864 ddr 0
Queue sclq_0 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_0 type 1 num 5 element 1036800 ddr 1
Queue sclq_1 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_1 type 1 num 5 element 1036800 ddr 1
Queue sclq_2 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_2 type 1 num 5 element 1036800 ddr 1
Queue sclq_3 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_3 type 1 num 5 element 1036800 ddr 1
Queue sclq_4 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_4 type 1 num 5 element 1036800 ddr 1
Queue sclq_5 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_5 type 1 num 5 element 1036800 ddr 1
Queue sclq_6 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_6 type 1 num 5 element 1036800 ddr 1
Queue sclq_7 type 0 num 5 element 1036800 ddr 1
Queue DIsclq_7 type 1 num 5 element 1036800 ddr 1
Queue encq_0 type 0 num 3 element 1244160 ddr 1
Queue DIencq_0 type 1 num 3 element 1244160 ddr 1
Queue dtoq_0 type 0 num 3 element 725760 ddr 0
Queue encq_1 type 0 num 3 element 1244160 ddr 1
Queue DIencq_1 type 1 num 3 element 1244160 ddr 1
Queue dtoq_1 type 0 num 3 element 725760 ddr 0
Queue encq_2 type 0 num 3 element 1244160 ddr 1
Queue DIencq_2 type 1 num 3 element 1244160 ddr 1
Queue dtoq_2 type 0 num 3 element 725760 ddr 0
Queue encq_3 type 0 num 3 element 1244160 ddr 1
Queue DIencq_3 type 1 num 3 element 1244160 ddr 1
Queue dtoq_3 type 0 num 3 element 725760 ddr 0
Queue encq_4 type 0 num 3 element 1244160 ddr 1
Queue DIencq_4 type 1 num 3 element 1244160 ddr 1
Queue dtoq_4 type 0 num 3 element 725760 ddr 0
Queue encq_5 type 0 num 3 element 1244160 ddr 1
Queue DIencq_5 type 1 num 3 element 1244160 ddr 1
Queue dtoq_5 type 0 num 3 element 725760 ddr 0
Queue encq_6 type 0 num 3 element 1244160 ddr 1
Queue DIencq_6 type 1 num 3 element 1244160 ddr 1
Queue dtoq_6 type 0 num 3 element 725760 ddr 0
Queue encq_7 type 0 num 3 element 1244160 ddr 1
Queue DIencq_7 type 1 num 3 element 1244160 ddr 1
Queue dtoq_7 type 0 num 3 element 725760 ddr 0
Queue decq_0 type 0 num 3 element 622080 ddr 0
Queue decq_1 type 0 num 3 element 622080 ddr 0
Queue decq_2 type 0 num 3 element 622080 ddr 0
Queue decq_3 type 0 num 3 element 622080 ddr 0
Queue decq_4 type 0 num 3 element 622080 ddr 0
Queue decq_5 type 0 num 3 element 622080 ddr 0
Queue decq_6 type 0 num 3 element 622080 ddr 0
Queue decq_7 type 0 num 3 element 622080 ddr 0
Queue drvinfo type 6 num 128 element 512 ddr 0
Queue disp1_que type 0 num 6 element 1244160 ddr 1
Queue DIdisp1_que type 1 num 6 element 1244160 ddr 1
Queue sclq_0 type 0 num 5 element 829440 ddr 1
Queue DIsclq_0 type 1 num 5 element 829440 ddr 1
Queue sclq_1 type 0 num 5 element 829440 ddr 1
Queue DIsclq_1 type 1 num 5 element 829440 ddr 1
Queue sclq_2 type 0 num 5 element 829440 ddr 1
Queue DIsclq_2 type 1 num 5 element 829440 ddr 1
Queue sclq_3 type 0 num 5 element 829440 ddr 1
Queue DIsclq_3 type 1 num 5 element 829440 ddr 1
Queue sclq_4 type 0 num 5 element 829440 ddr 1
Queue DIsclq_4 type 1 num 5 element 829440 ddr 1
Queue sclq_5 type 0 num 5 element 829440 ddr 1
Queue DIsclq_5 type 1 num 5 element 829440 ddr 1
Queue sclq_6 type 0 num 5 element 829440 ddr 1
Queue DIsclq_6 type 1 num 5 element 829440 ddr 1
Queue sclq_7 type 0 num 5 element 829440 ddr 1
Queue DIsclq_7 type 1 num 5 element 829440 ddr 1
Queue encq_0 type 0 num 3 element 1244160 ddr 0
Queue DIencq_0 type 1 num 3 element 1244160 ddr 0
Queue dtoq_0 type 0 num 3 element 725760 ddr 1
Queue encq_1 type 0 num 3 element 1244160 ddr 0
Queue DIencq_1 type 1 num 3 element 1244160 ddr 0
Queue dtoq_1 type 0 num 3 element 725760 ddr 1
Queue encq_2 type 0 num 3 element 1244160 ddr 0
Queue DIencq_2 type 1 num 3 element 1244160 ddr 0
Queue dtoq_2 type 0 num 3 element 725760 ddr 1
Queue encq_3 type 0 num 3 element 1244160 ddr 0
Queue DIencq_3 type 1 num 3 element 1244160 ddr 0
Queue dtoq_3 type 0 num 3 element 725760 ddr 1
Queue encq_4 type 0 num 3 element 1244160 ddr 0
Queue DIencq_4 type 1 num 3 element 1244160 ddr 0
Queue dtoq_4 type 0 num 3 element 725760 ddr 1
Queue encq_5 type 0 num 3 element 1244160 ddr 0
Queue DIencq_5 type 1 num 3 element 1244160 ddr 0
Queue dtoq_5 type 0 num 3 element 725760 ddr 1
Queue encq_6 type 0 num 3 element 1244160 ddr 0
Queue DIencq_6 type 1 num 3 element 1244160 ddr 0
Queue dtoq_6 type 0 num 3 element 725760 ddr 1
Queue encq_7 type 0 num 3 element 1244160 ddr 0
Queue DIencq_7 type 1 num 3 element 1244160 ddr 0
Queue dtoq_7 type 0 num 3 element 725760 ddr 1
Queue decq_0 type 0 num 3 element 622080 ddr 1
Queue decq_1 type 0 num 3 element 622080 ddr 1
Queue decq_2 type 0 num 3 element 622080 ddr 1
Queue decq_3 type 0 num 3 element 622080 ddr 1
Queue decq_4 type 0 num 3 element 622080 ddr 1
Queue decq_5 type 0 num 3 element 622080 ddr 1
Queue decq_6 type 0 num 3 element 622080 ddr 1
Queue decq_7 type 0 num 3 element 622080 ddr 1

 */

#endif
