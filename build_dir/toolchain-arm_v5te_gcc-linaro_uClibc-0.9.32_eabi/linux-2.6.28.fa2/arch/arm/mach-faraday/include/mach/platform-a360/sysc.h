#ifndef __SYSC_H
#define __SYSC_H

/* SYSC Register */
typedef struct {
	unsigned int  IDId;          // 0x00
	unsigned int  GenControl;     // 0x04
	unsigned int  ClockConf;      // 0x08
	unsigned int  AHBClockEnable;         // 0x0c
	unsigned int  APBClockEnable; // 0x10
	unsigned int  ConfStrap;      // 0x14
	unsigned int  IOMulControl0;  // 0x18
	unsigned int  IOMulControl1;  // 0x1c
	unsigned int  IOMulControl2;  // 0x20
	unsigned int  IOMulControl3;  // 0x24
	unsigned int  IOParaControl0;  //0x28
	unsigned int  IOParaControl1;  //0x2C
	unsigned int  CPUEDGESYNC;    /* 0x30 */
	unsigned int  PCIECONR0;      /* 0x34 */
	unsigned int  PCIECONR1;      /* 0x38 */
	unsigned int  PCIECONR2;      /* 0x3C */
}SYSC_REG;


#define SYSTEM_I2C0		0
#define SYSTEM_I2C1		1
#define SYSTEM_GPIO0_A		2
#define SYSTEM_GPIO0_0		0x200
#define SYSTEM_GPIO0_1		0x201
#define SYSTEM_GPIO0_2		0x202
#define SYSTEM_GPIO0_3		0x203
#define SYSTEM_GPIO0_4		0x204
#define SYSTEM_GPIO0_5		0x205
#define SYSTEM_GPIO0_6		0x206
#define SYSTEM_GPIO0_7		0x207
#define SYSTEM_GPIO0_8		0x208
#define SYSTEM_GPIO0_9		0x209
#define SYSTEM_GPIO0_10		0x20a
#define SYSTEM_GPIO0_11		0x20b
#define SYSTEM_GPIO0_12		0x20c
#define SYSTEM_GPIO0_13		0x20d
#define SYSTEM_GPIO0_14		0x20e
#define SYSTEM_GPIO0_15		0x20f
#define SYSTEM_GPIO0_16		0x210
#define SYSTEM_GPIO0_17		0x211
#define SYSTEM_GPIO0_18		0x212
#define SYSTEM_GPIO0_19		0x213
#define SYSTEM_GPIO0_20		0x214
#define SYSTEM_GPIO0_21		0x215
#define SYSTEM_GPIO0_22		0x216
#define SYSTEM_GPIO0_23		0x217
#define SYSTEM_GPIO0_24		0x218
#define SYSTEM_GPIO0_25		0x219
#define SYSTEM_GPIO0_26		0x21a
#define SYSTEM_GPIO0_27		0x21b
#define SYSTEM_GPIO0_28		0x21c
#define SYSTEM_GPIO0_29		0x21d
#define SYSTEM_GPIO0_30		0x21e
#define SYSTEM_GPIO0_31		0x21f
#define SYSTEM_GPIO1_A		3
#define SYSTEM_GPIO1_0		0x30
#define SYSTEM_GPIO1_1		0x31
#define SYSTEM_GPIO1_2		0x32
#define SYSTEM_GPIO1_3		0x33
#define SYSTEM_GPIO1_4		0x34
#define SYSTEM_GPIO1_5		0x35
#define SYSTEM_GPIO1_6		0x36
#define SYSTEM_GPIO1_7		0x37
#define SYSTEM_LCD8B		0x40
#define SYSTEM_LCD18B		0x41
#define SYSTEM_LCDC		0x42
#define SYSTEM_FUART		5
#define SYSTEM_BTUART		6
#define SYSTEM_SSP0		7
#define SYSTEM_SSP1		8
#define SYSTEM_SDC		9
#define SYSTEM_EBC		0xa
#define SYSTEM_TV		0xb
#define SYSTEM_CPU		0xc
//=============================================================================
#define SYSTEM_SSP_INCLK	0x0
#define SYSTEM_SSP_EXCLK	0x1
//------------------------------------------------------------------------------
#define SYSTEM_SDC_DCLK		0x0
#define SYSTEM_SDC_MCLK		0x1
#define SYSTEM_SDC_HCLK		0x2
//------------------------------------------------------------------------------
#define SYSTEM_EBC_PCLK		0x0
#define SYSTEM_EBC_HCLK		0x1
#define SYSTEM_EBC_TXCLK	0x2
//------------------------------------------------------------------------------
#define SYSTEM_LCD_HCLK		0x0
//#define SYSTEM_LCD_HCLK	0x1
#define SYSTEM_LCD_EXCLK	0x2

extern void SetSysIOMul(unsigned int IoID);
extern void SetClockConf(unsigned int IoID, unsigned int val);
extern unsigned int GetClockConf(unsigned int IoID);

extern unsigned int scu_get_clock(unsigned int IoID);

#endif	// __SYSC_H

