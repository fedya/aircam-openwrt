# Platform Specification of GM8181
#
# Copyright (C) 2008 GM Corp.
#


Specification	GM8181
IRQs		64
FIQs		32

#-------------------------------------------------------------------------------------------------
# IP     Name       Version  IRQ       E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 CPU     FA626      2_6       -        - -   -  - -  0x0-0x5000000          -                      - - -
 PMU     FTPMU010   1_x_rx    -        - -   -  - -  0x99000000-0x990FFFFF  0xF9900000-0xF990FFFF  N N -

 DDRC	 FTDDRC010  1_x_rx    -        - -   -  - -  0x99300000-0x99300FFF  0xF9930000-0xF9930FFF  N N -
 DDRC	 FTDDRC010  1_x_rx    -        - -   -  - -  0x99700000-0x99700FFF  0xF9970000-0xF9970FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90000000-0x90000FFF  0xF9000000-0xF9000FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90100000-0x90100FFF  0xF9010000-0xF9010FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90200000-0x90200FFF  0xF9020000-0xF9020FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90300000-0x90300FFF  0xF9030000-0xF9030FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90400000-0x90400FFF  0xF9040000-0xF9040FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90500000-0x90500FFF  0xF9050000-0xF9050FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90600000-0x90600FFF  0xF9060000-0xF9060FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90700000-0x90700FFF  0xF9070000-0xF9070FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90800000-0x90800FFF  0xF9080000-0xF9080FFF  N N -
 AHBC	 FTAHBC010  1_x_rx    -        - -   -  - -  0x90900000-0x90900FFF  0xF9090000-0xF9090FFF  N N -
 AHBC    FTAHBC010  1_x_rx    -        - -   -  - -  0x90A00000-0x90A00FFF  0xF90A0000-0xF90A0FFF  N N -
 UART	 FTUART010  1_5_r1    9        L H   -  - -  0x98300000-0x983FFFFF  0xF9830000-0xF9830FFF  N N -
 UART	 FTUART010  1_5_r1   10        L H   -  - -  0x98400000-0x984FFFFF  0xF9840000-0xF9840FFF  N N -
 UART    FTUART010  1_5_r1   20        L H   -  - -  0x98500000-0x985FFFFF  0xF9850000-0xF9850FFF  N N -
 UART    FTUART010  1_5_r1   21        L H   -  - -  0x98800000-0x988FFFFF  0xF9880000-0xF9880FFF  N N -
 UART    FTUART010  1_5_r1   22        L H   -  - -  0x98900000-0x989FFFFF  0xF9890000-0xF9890FFF  N N -

 TIMER   FTTMR010   1_4_r0   14,14,14  L H   -  - -  0x99100000-0x9910000F  0xF9910000-0xF9910FFF  N N -
 INTC    FTINTC010  1_3_r5    -        - -   -  - -  0x99500000-0x995FFFFF  0xF9950000-0xF9950FFF  N N -
 
 SATA    FTSATA100  0_4_0    43        L H   -  - -  0x92D00000-0x92DFFFFF  0xF92D0000-0xF92D0FFF  N N -
 SATA    FTSATA100  0_4_0    44        L H   -  - -  0x92E00000-0x92EFFFFF  0xF92E0000-0xF92E0FFF  N N -
#-------------------------------------------------------------------------------------------------
 SRAM    FTSMC010   -         -        - -   -  - -  0x92100000-0x921FFFFF  0xF9210000-0xF921FFFF  N N -
 NAND    FTNAND020  -        23        - -   -  - -  0x92C00000-0x92CFFFFF  0xF92C0000-0xF92CFFFF  N N -
 TVE     FTTVE100   1_0_r0   24        L H   -  - -  0x92B00000-0x92BFFFFF  0xF92B0000-0xF92B0FFF  N N -
 USB     FOTG2XX    1_3_r1    4        L H   -  - -  0x92200000-0x92200FFF  0xF9220000-0xF9220FFF  N N -
 USB     FUSBH200   1_3_r1    5        L H   -  - -  0x92300000-0x92300FFF  0xF9230000-0xF9230FFF  N N -
 PCI     -          -        17        L H   -  - -  -                      -                      N N -
 PCIMEM  -          -         -        - -   -  - -  0x96300000-0x963FFFFF  0xE0000000-0xEFFFFFFF  N N N
 PCIIO   -          -         -        - -   -  - -  0x96200000-0x962FFFFF  0xF9620000-0xF962FFFF  N N N

 MTD     -          -         -        - -   -  - -  0x10000000-0x10FFFFFF  0xF3000000-0xF3FFFFFF  N Y N
 AHBDMA  -          -         1        L H   -  - -  -                      -                      - - -
 APBDMA  -          -         2        L H   -  - -  -                      -                      - - -
Specification AHBDMA
IRQs		8
#-------------------------------------------------------------------------------------------------
# IP    Name   	     Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 DMAC   FTDMAC020    1_4_r2   0-7 L H   -  - -  0x92600000-0x926FFFFF  0xF9260000-0xF9260FFF  N N -

Specification APBDMA
IRQs		4
#--------------------------------------------------------------------------------------------------
# IP    Name   	     Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#--------------------------------------------------------------------------------------------------
 APBBRG FTAPBBRG020S 1_1_r0   0-3 L H   -  - -  0x92000000-0x920FFFFF  0xF9200000-0xF9200FFF  N N -

Specification	PCI
IRQs		4
#------------------------------------------------------------------------------------------------
# IP    Name         Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#------------------------------------------------------------------------------------------------
 PCIC   FTPCI100     1_6_r2   0-3 L H   -  - -  0x96400000-0x96400FFF  0xF9640000-0xF9640FFF  N N -
