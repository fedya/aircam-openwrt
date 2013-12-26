Specification	A360
IRQs		32
FIQs		32

#-------------------------------------------------------------------------------------------------------------------------------------
# IP       Name           Version   IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 CPU       FA626TE        1.0.0     -                          - -   -           - - 0x0-0x3FFFFFFF        -                     N N N
 ADC       FTTSC010       1.0.0     -                          - -   -           - - 0x99800000-0x998FFFFF 0xF9980000-0xF9980FFF N N N
 AES       FTAES020       0.3.0     12                         L H   12          L H 0x90800000-0x908FFFFF 0xF9080000-0xF9080FFF N N N
 AHBC      FTAHBC020      1.3.1     0                          L H   0           L H 0x90100000-0x901FFFFF 0xF9010000-0xF9010FFF N N N
 DDRC      FTDDRII020     1.0.0     -                          - -   -           - - 0x90300000-0x903FFFFF 0xF9030000-0xF9030FFF N N N
 EBC       FTLVBC020      1.0.0     7,9                        L H   7,9         L H 0x90E00000-0x90EFFFFF 0xF90E0000-0xF90E0FFF N N N
 EBCIO     FTLVBC020      -         -                          - -   -           - - 0xD0000000-0xD7FFFFFF 0xFD000000-0xFD000FFF N N N
 EBCMEM    FTLVBC020      -         -                          - -   -           - - 0xD8000000-0xDFFFFFFF 0xFD800000-0xFD800FFF N N N
 GPIO      FTGPIO010      1.7.0     13                         L H   13          L H 0x98700000-0x987FFFFF 0xF9870000-0xF9870FFF N N N
 GPIO      FTGPIO010      1.7.0     20                         L H   20          L H 0x99700000-0x997FFFFF 0xF9970000-0xF9970FFF N N N
 IIC       FTIIC010       1.8.0     3                          L H   3           L H 0x98A00000-0x98AFFFFF 0xF98A0000-0xF98A0FFF N N N
 IIC       FTIIC010       1.8.0     22                         L H   22          L H 0x98C00000-0x98CFFFFF 0xF98C0000-0xF98C0FFF N N N
 INTC      FTINTC010      1.8.0     -                          - -   -           - - 0x98800000-0x988FFFFF 0xF9880000-0xF9880FFF N N N
 LCDC      FTLCDC200      1.0.0     27                         L H   27          L H 0x90600000-0x906FFFFF 0xF9060000-0xF9060FFF N N N
 MAC       FTMAC110       1.0.0     25                         L H   25          L H 0x90900000-0x909FFFFF 0xF9090000-0xF9090FFF N N N
 NANDC     FTNANDC020     1.0.0     30                         L H   30          L H 0x91000000-0x910FFFFF 0xF9100000-0xF9100FFF N N N
 PCI1      -              -         4                          L H   -           - - -                     -                     N N N
 PCIIO     FTPCIE3914     -         -                          - -   -           - - 0xC8000000-0xC83FFFFF 0xFC800000-0xFC800FFF N N N
 PCIMEM    FTPCIE3914     -         -                          - -   -           - - 0xB0000000-0xBFFFFFFF 0xFB000000-0xFB000FFF N N N
 PMU       FTPMU010       1.0.0     8                          L H   8           L H 0x98100000-0x981FFFFF 0xF9810000-0xF9810FFF N N N
 RAM       FTDDRII020     -         -                          - -   -           - - 0x10000000-0x4FFFFFFF -                     N N N
 RTC       FTRTC010       1.0.0     17,18                      E H   -           - - 0x98600000-0x986FFFFF 0xF9860000-0xF9860FFF N N N
 SCU       FTSCU000       1.0.0     -                          - -   -           - - 0x99900000-0x999FFFFF 0xF9990000-0xF9990FFF N N N
 SDC       FTSDC010       1.0.0     5                          L H   5           L H 0x90700000-0x907FFFFF 0xF9070000-0xF9070FFF N N N
 SMC       FTSMC020       1.8.0     -                          - -   -           - - 0x90200000-0x902FFFFF 0xF9020000-0xF9020FFF N N N
 SSP       FTSSP010       1.15.0    2                          L H   2           L H 0x98B00000-0x98BFFFFF 0xF98B0000-0xF98B0FFF N N N
 SSP       FTSSP010       1.15.0    6                          E H   6           E H 0x99400000-0x994FFFFF 0xF9940000-0xF9940FFF N N N
 TIMER     FTTMR010       1.6.0     19,14,15                   E H   19,14,15    L H 0x98400000-0x984FFFFF 0xF9840000-0xF9840FFF N N N
 UART      FTUART010      1.15.0    10                         L H   10          L H 0x98200000-0x982FFFFF 0xF9820000-0xF9820FFF N N N
 UART      FTUART010      1.15.0    11                         L H   11          L H 0x98300000-0x983FFFFF 0xF9830000-0xF9830FFF N N N
 USB       FOTG2XX        1.9.0     26                         L H   26          L H 0x90B00000-0x90BFFFFF 0xF90B0000-0xF90B0FFF N N N
 USB       FUSBH200       1.0.0     23                         L H   23          L H 0x90A00000-0x90AFFFFF 0xF90A0000-0xF90A0FFF N N N
 WDT       FTWDT010       1.5.0     16                         L H   16          L H 0x98500000-0x985FFFFF 0xF9850000-0xF9850FFF N N N
 APBDMA    -              -         24                         L H   -           - - -                     -                     N N N
 AHBDMA    -              -         21                         L H   -           - - -                     -                     N N N


Specification	PCI1
IRQs		4

#-------------------------------------------------------------------------------------------------------------------------------------
# IP       Name           Version   IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 PCI       FTPCIE3914     1.0.0     0-3                        L H   -           - - 0x90D00000-0x90DFFFFF 0xF90D0000-0xF90D0FFF N N N


Specification	APBDMA
IRQs		4

#-------------------------------------------------------------------------------------------------------------------------------------
# IP       Name           Version   IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 APBBRG    FTAPBBRG020    1.4.0     0-3                        L H   -           - - 0x90500000-0x905FFFFF 0xF9050000-0xF9050FFF N N N


Specification	AHBDMA
IRQs		8

#-------------------------------------------------------------------------------------------------------------------------------------
# IP       Name           Version   IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 DMAC      FTDMAC020      1.13.0    0-7                        L H   -           - - 0x90400000-0x904FFFFF 0xF9040000-0xF9040FFF N N N


