/*
 *  linux/arch/arm/mach-faraday/platform-a360/sysc.c
 *
 *  Faraday FTSCU000 Timer Device Driver Implementation
 *
 *  Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <asm/io.h>

#include <asm/mach/time.h>
#include <asm/irq.h>
#include <asm/param.h>

#include <mach/spec.h>
#include <mach/platform/sysc.h>

#define MASK_SYSR_I2C1_CR0 		0x000f0000
#define MASK_SYSR_I2C0_CR1 		0x3c000000 //26~29
#define MASK_SYSR_SDC_CR2 		0x0000ffff
#define MASK_SYSR_SSP1_CR1		0x03ffc000
#define MASK_SYSR_SSP0_CR0		0x000003ff
#define MASK_SYSR_LCD18_CR0		0xc000ffff
#define MASK_SYSR_LCD18_CR1		0x00000003
#define MASK_SYSR_LCD8_CR2		0xffff0000

#define MASK_SYSR_SDC_CCR 		0x0000000c
#define MASK_SYSR_LCD_CCR		0x00000003
#define MASK_SYSR_EBC_CCR		0x00000030
#define MASK_SYSR_SSP0_CCR		(1<<6)
#define MASK_SYSR_SSP1_CCR		(1<<7)

#define SET_SYSR_I2C1_CR0 		0x000a0000
#define SET_SYSR_I2C0_CR1 		0x28000000
#define SET_SYSR_SDC_CR2 		0x0000aaaa
#define SET_SYSR_SSP1_CR1		0x02aa8000
#define SET_SYSR_SSP0_CR0		0x000002aa
#define SET_SYSR_LCD18_CR0		0x40005555
#define SET_SYSR_LCD18_CR1		0x00000001
#define SET_SYSR_LCD8_CR2		0x00000000


#define	DRIVER_VERSION		"0.01"

#define IPMODULE SCU
#define IPNAME   FTSCU000


static volatile SYSC_REG *SYSCReg;

int __init platform_sysc_init(void)
{
	printk (KERN_INFO "Faraday FTSCU010 System Control driver v" DRIVER_VERSION "\n");
	SYSCReg = (SYSC_REG *) IP_va_base[0];

//LCD setting
	SetSysIOMul(SYSTEM_LCD8B);


//I2C setting
	SetSysIOMul(SYSTEM_I2C1);
	SetSysIOMul(SYSTEM_I2C0);

//SDC setting
	SetSysIOMul(SYSTEM_SDC);
	SetClockConf(SYSTEM_SDC,SYSTEM_SDC_HCLK);

//SSP setting
	SetClockConf(SYSTEM_SSP1,SYSTEM_SSP_EXCLK);
	SetSysIOMul(SYSTEM_SSP1);
	SetSysIOMul(SYSTEM_I2C1);

//LCD setting
#if defined(PRIME_VIEW_PD035VX2)|| \
    defined(KOROLA_NTSC_640)|| \
    defined(KOROLA_PAL_640)|| \
    defined(SHARP_LQ084V1DG21)
	SetSysIOMul(SYSTEM_LCD18B);
#endif
	return 0;
}

void SetSysIOMul(unsigned int IoID)
{
	/*Geans: Map SYSC register addresses*/

	switch(IoID)
	{
	case SYSTEM_I2C0:
		SYSCReg->IOMulControl1 = ((SYSCReg->IOMulControl1 & (~MASK_SYSR_I2C0_CR1)) | SET_SYSR_I2C0_CR1);
		break;
	case SYSTEM_I2C1:
		SYSCReg->IOMulControl0 = ((SYSCReg->IOMulControl0 & (~MASK_SYSR_I2C1_CR0)) | SET_SYSR_I2C1_CR0);
		break;

	case SYSTEM_GPIO0_A:
		break;
	case SYSTEM_GPIO1_A:
		break;

	case SYSTEM_LCD18B:
		SYSCReg->IOMulControl0 = ((SYSCReg->IOMulControl0 & (~MASK_SYSR_LCD18_CR0)) | SET_SYSR_LCD18_CR0);
		SYSCReg->IOMulControl1 = ((SYSCReg->IOMulControl1 & (~MASK_SYSR_LCD18_CR1)) | SET_SYSR_LCD18_CR1);
	case SYSTEM_LCD8B:
		SYSCReg->IOMulControl2 = ((SYSCReg->IOMulControl2 & (~MASK_SYSR_LCD8_CR2)) | SET_SYSR_LCD8_CR2);		
		break;

	case SYSTEM_FUART:
		break;
	case SYSTEM_BTUART:
		break;

	case SYSTEM_SSP0:
		SYSCReg->IOMulControl0 = ((SYSCReg->IOMulControl0 & (~MASK_SYSR_SSP0_CR0)) | SET_SYSR_SSP0_CR0);
		break;
	case SYSTEM_SSP1:
		SYSCReg->IOMulControl1 = ((SYSCReg->IOMulControl1 & (~MASK_SYSR_SSP1_CR1)) | SET_SYSR_SSP1_CR1);
		break;

	case SYSTEM_SDC:
		SYSCReg->IOMulControl2 = ((SYSCReg->IOMulControl2 & (~MASK_SYSR_SDC_CR2)) | SET_SYSR_SDC_CR2);
		break;

	case SYSTEM_TV:
		break;

	case SYSTEM_CPU:
		break;

	default:
		printk("Set SYSC IO ERROR\r\n");
//		DEBUGMSG(1, (TEXT("Set SYSC IO ERROR\r\n")));
	}
	return;
}


void SetClockConf(unsigned int IoID, unsigned int val)
{
	switch(IoID)
	{
	case SYSTEM_LCDC:
		SYSCReg->ClockConf = ((SYSCReg->ClockConf & (~MASK_SYSR_LCD_CCR)) | val);
		break;

	case SYSTEM_SDC:
		SYSCReg->ClockConf = ((SYSCReg->ClockConf & (~MASK_SYSR_SDC_CCR)) | val<<2);
		break;

	case SYSTEM_EBC:
		SYSCReg->ClockConf = ((SYSCReg->ClockConf & (~MASK_SYSR_EBC_CCR)) | val<<4);
		break;

	case SYSTEM_SSP0:
		SYSCReg->ClockConf = ((SYSCReg->ClockConf & (~MASK_SYSR_SSP0_CCR)) | val<<6);
		break;
	case SYSTEM_SSP1:
		SYSCReg->ClockConf = ((SYSCReg->ClockConf & (~MASK_SYSR_SSP1_CCR)) | val<<7);
		break;

	default:
		printk("Set Clock ERROR\r\n");
		//DEBUGMSG(1, (TEXT("Set Clock ERROR\r\n")));

	}
	return;
}

unsigned int GetClockConf(unsigned int IoID)
{
	unsigned int tmp=0;

	switch(IoID)
	{
	case SYSTEM_LCDC:
		tmp = ((SYSCReg->ClockConf & (MASK_SYSR_LCD_CCR)) );
		break;

	case SYSTEM_SDC:
		tmp = ((SYSCReg->ClockConf & (MASK_SYSR_SDC_CCR)) >> 2);
		break;

	case SYSTEM_EBC:
		tmp = ((SYSCReg->ClockConf & (MASK_SYSR_EBC_CCR)) >> 4);
		break;

	case SYSTEM_SSP0:
		tmp = ((SYSCReg->ClockConf & (MASK_SYSR_SSP0_CCR)) >> 6);
		break;
	case SYSTEM_SSP1:
		tmp = ((SYSCReg->ClockConf & (MASK_SYSR_SSP1_CCR)) >> 7);
		break;

	default:
		printk("Get Clock ERROR %d \n",IoID);
		//DEBUGMSG(1, (TEXT("Get Clock ERROR\r\n")));
	}
	return tmp;
}

unsigned int scu_get_clock(unsigned int IoID)
{
	unsigned int tmp;

	switch(IoID)
	{
	case SYSTEM_LCD8B:
	case SYSTEM_LCD18B:
	case SYSTEM_LCDC:
		tmp = GetClockConf(SYSTEM_LCDC) & 0x3;

		if(tmp == 0x0)
			return AHB_CLK_IN;//AHB clock
		else if(tmp == 0x1)
			return AHB_CLK_IN/2;//APB clock
		else{
			printk(" %s :please modify LCD external clock GPIO0[10]\n\n",__func__);
			return AHB_CLK_IN;//external clock GPIO0[10]
		}

	case SYSTEM_SDC:
		tmp = GetClockConf(SYSTEM_SDC) & 0x3;

		if(tmp == 0x0)
			return AHB_CLK_IN*2;//sys_dclk
		else if(tmp == 0x1)
			return AHB_CLK_IN*3/2;//sys_mclk
		else
			return AHB_CLK_IN;//sys_hclk

	case SYSTEM_EBC:
	case SYSTEM_SSP0:
	case SYSTEM_SSP1:
	default:
		printk("Get Clock ERROR %d \n",IoID);
		//DEBUGMSG(1, (TEXT("Get Clock ERROR\r\n")));
	}
	return 0;
}


subsys_initcall(platform_sysc_init);
//EXPORT_SYMBOL(SetSysIOMul);
//EXPORT_SYMBOL(SetClockConf);
//EXPORT_SYMBOL(GetClockConf);
EXPORT_SYMBOL(scu_get_clock);

//Geans:test
