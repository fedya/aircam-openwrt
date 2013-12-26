# Platform Specification of FIA320D
#
# Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
#
# Last update : 09/08/2005 Luke Lee


Specification	FIA320D
IRQs		32
FIQs		32

#-------------------------------------------------------------------------------------------------
# IP     Name       Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 CPU     FA526      2_6       -  - -   -  - -  0x0       -0x03FFFFFF  -                      - - -
 INTC    FTINTC010  1_3_r5    -  - -   -  - -  0x98800000-0x988FFFFF  0xF9880000-0xF9880FFF  N Y -
 UART	 FTUART010  1_5_r1   10  L H   -  - -  0x98200000-0x982FFFFF  0xF9820000-0xF9820FFF  N Y -
 UART	 FTUART010  1_5_r1   11  L H   -  - -  0x98300000-0x983FFFFF  0xF9830000-0xF9830FFF  N Y -
 UART    FTUART010  1_5_r1    7  L H   -  - -  0x99600000-0x996FFFFF  0xF9960000-0xF9960FFF  N Y -
 TIMER   FTTMR010   1_4_r0   19,14,15\
 		                 E H   -  - -  0x98400000-0x9840000F  0xF9840000-0xF9840FFF  N Y -
 SSP     FTSSP010   1_x_rx    6  L H   -  - -  0x99400000-0x994FFFFF  0xF9940000-0xF9940FFF  N Y -
 FLH     FTFLH010   1_x_rx    -  - -   -  - -  0x80440000-0x8053FFFF  0xF8400000-0xF84FFFFF  N Y -
#-------------------------------------------------------------------------------------------------
 PMU     FTPMU010   1_x_rx    -  - -   -  - -  0x98100000-0x981FFFFF  0xF9810000-0xF9810FFF  N Y -
 MAC     FTMAC100   1_3_r2   25  L H   -  - -  0x90900000-0x909FFFFF  0xF9090000-0xF9090FFF  N Y Y
 SDC     FTSDC010   1_2_r0    5  L H   -  - -  0x98E00000-0x98EFFFFF  0xF98E0000-0xF98E0FFF  N Y Y
 SDC     FTSDC020   1_0_r0   29  L H   -  - -  0x92000000-0x9200FFFF  0xF9200000-0xF9200FFF  N Y Y
 AHBDMA  -          -        21  L H   -  - -  -                      -                      - - -
 APBDMA  -          -        24  L H   -  - -  -                      -                      - - Y
 NANDC	 FTNANDC020 1_0_r0   29  L L   -  - -  0x90c00000-0x90c0FFFF  0xF90C0000-0xF90C0FFF  N N -
 RTC	 FTRTC010   1_0_r0   17,18\
                                 E H   -  - -  0x98600000-0x9860FFFF  0xF9860000-0xF9860FFF  N Y -
 WDT	 FTWDT010   1_0_r0   16  L H   -  - -  0x98500000-0x9850FFFF  0xF9850000-0xF9850FFF  N Y -
 GPIO	 FTGPIO010  1_0_r0   13  E H   -  - -  0x98700000-0x9870FFFF  0xF9870000-0xF9870FFF  N Y -
 CFC	 FTCFC010   0_1_r0   0,1\
                                 L H   -  - -  0x98D00000-0x98D0FFFF  0xF98D0000-0xF98D0FFF  N Y -
 LCD	 FTLCDC100  0_3_r1   20  L H   -  - -  0x90600000-0x9060FFFF  0xF9060000-0xF9060FFF  N Y -
 I2C	 FTI2C010   1_5_r2   3   L H   -  - -  0x98A00000-0x98A0FFFF  0xF98A0000-0xF98A0FFF  N Y -
 USB	 FOTG2XX    1_0_r0   29  L H   -  - -  0x92000000-0x9200FFFF  0xF9200000-0xF9200FFF  N N -
 USB	 FUSBH200   1_0_r0   29  L H   -  - -  0x92000000-0x9200FFFF  0xF9200000-0xF9200FFF  N N -
 USB	 FUSB220    1_0_r0   26  L H   -  - -  0x90B00000-0x90B0FFFF  0xF90B0000-0xF90B0FFF  N N -
 FIA321  -          -        30  L H   -  - -  -	              -                      - - N
# FIQ router
 FIQ_RELAY -        -        15  E H   -  - -  -		      -                      - - -
Specification AHBDMA
IRQs		8
#-------------------------------------------------------------------------------------------------
# IP    Name   	    Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 DMAC   FTDMAC020   1_4_r2   0-7 L H   -  - -  0x90400000-0x904FFFFF  0xF9040000-0xF9040FFF  N Y Y


Specification APBDMA
IRQs		4
#--------------------------------------------------------------------------------------------------
# IP    Name   	     Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#--------------------------------------------------------------------------------------------------
 APBBRG FTAPBBRG020S 1_1_r0   0-3 L H   -  - -  0x90500000-0x905FFFFF  0xF9050000-0xF9050FFF  N Y Y

# Support total 32 relayed FIQs (0-31) on A320D, level triggered, active high
Specification FIQ_RELAY
IRQs		32
 FIQ_RELAYER -	     -	      0-31 L H   -  - -   -                      -                     - - -
include a321.spec
