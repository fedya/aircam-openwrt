# Platform Specification of FIA321
#
# Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
#
# Last update : 09/08/2005 Luke Lee


Specification	FIA321
IRQs		32
FIQs		32
#-------------------------------------------------------------------------------------------------
# IP     Name       Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#-------------------------------------------------------------------------------------------------
 INTC    FTINTC010  1_3_r5    -  - -   -  - -  0xB0800000-0xB08FFFFF  0xFB080000-0xFB080FFF  N Y N
 KMI     FTKBC010   1_4_r2    4  E H   -  - -  0xB1300000-0xB13FFFFF  0xFB130000-0xFB130FFF  N N N
 KMI     FTKBC010   1_4_r2    9  E H   -  - -  0xB1700000-0xB17FFFFF  0xFB170000-0xFB170FFF  N N N
 APBDMA  -          -        24  L H   -  - -  -                      -                      - - -
 MAC     FTMAC100   1_3_r2   25  L H   -  - -  0x92000000-0x920FFFFF  0xF9200000-0xF9200FFF  N Y Y
 PCI     -          -        28  L H   -  - -  -                      -                      N Y N
 PCIMEM  -          -         -  - -   -  - -  0xA0000000-0xAFFFFFFF  0xE0000000-0xEFFFFFFF  N Y N
 PCIIO   -          -         -  - -   -  - -  0x90C01000-0x90CFFFFF  0xF9D01000-0xF9DFFFFF  N Y N

Specification APBDMA
IRQs		4
#--------------------------------------------------------------------------------------------------
# IP     Name         Version IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#--------------------------------------------------------------------------------------------------
 APBBRG  FTAPBBRG020S 1_1_r0  0-3 L H   -  - -  0x90E00000-0x90EFFFFF  0xF90E0000-0xF90E0FFF  N Y Y

Specification	PCI
IRQs		4
#------------------------------------------------------------------------------------------------
# IP    Name       Version  IRQ E T  FIQ E T  Physical Address       Virtual Address        C B D
#------------------------------------------------------------------------------------------------
 PCIC   FTPCI100   1_6_r2   0-3 L H   -  - -  0x90C00000-0x90C00FFF  0xF90C0000-0xF90C0FFF  N Y Y

