# Platform Specification of FIA320D
#
# Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
#
# Last update : 09/08/2005 Luke Lee


Specification	AXI
IRQs		13
FIQs		11

#-------------------------------------------------------------------------------------------------
# IP     Name       Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 CPU     FA626TE    2_6       -  - -   -  - -  0x0-0x10000000          -                     - - -
 INTC    FTINTC010  1_6       -  - -   -  - -  0xA8300000-0xA83FFFFF  0xFA830000-0xFA830FFF  N N -
 UART	 FTUART010  1_4       4  L H   -  - -  0xA8000000-0xA80FFFFF  0xFA800000-0xFA800FFF  N N -
 UART	 FTUART010  1_4       5  L H   -  - -  0xA8100000-0xA81FFFFF  0xFA810000-0xFA810FFF  N N -
 TIMER   FTTMR010   1_3       1,2,  \
                              3\
 		                 E H   -  - -  0xA8200000-0xA820000F  0xFA820000-0xFA820FFF  N N -
 DMAC	 FTDMAC030  1_0       11,\
			      12,\
			      13 L H   -  - -  0xA8500000-0xA85FFFFF  0xFa850000-0xFA850FFF  N N -
# L2CC	 FTL2CC030  1_0       7  L H   -  - -  0xA0200000-0xA02FFFFF  0xFA020000-0xFA020FFF  N N -
# ROM	 EEPROM     1_0       -  - -  -  -  -  0x10000000-0x10080000  0xD0000000-0xD0080000  N N -


