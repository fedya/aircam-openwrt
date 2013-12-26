Specification	A369
IRQs		64
FIQs		64

#-------------------------------------------------------------------------------------------------------------------------------------
# IP        Name                 Version    IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 CPUS       FA606TE              1          -                          - -   -           - - 0x0-0x3FFFFFFF        -                     N N N
 CPU        FA626TE              1          -                          - -   -           - - 0x0-0x3FFFFFFF        -                     N N N
 ADC        FTTSC010             1          19,20                      L H   19,20       L H 0x92400000-0x924FFFFF 0xF9240000-0xF9240FFF N N N
 AES        FTAES020             1          31                         L H   31          L H 0x90D00000-0x90DFFFFF 0xF90D0000-0xF90D0FFF N N N
 AHBB       FTAHBB020            1          -                          - -   -           - - 0x94400000-0x944FFFFF 0xF9440000-0xF9440FFF N N N
 AHBB       FTAHBB020            1          -                          - -   -           - - 0x94500000-0x945FFFFF 0xF9450000-0xF9450FFF N N N
 AHBB       FTAHBB020            1          -                          - -   -           - - 0x94600000-0x946FFFFF 0xF9460000-0xF9460FFF N N N
 AHBC       FTAHBC020            1          -                          - -   -           - - 0x94000000-0x940FFFFF 0xF9400000-0xF9400FFF N N N
 AHBC       FTAHBC020            1          -                          - -   -           - - 0x94100000-0x941FFFFF 0xF9410000-0xF9410FFF N N N
 AHBC       FTAHBC020            1          -                          - -   -           - - 0x94200000-0x942FFFFF 0xF9420000-0xF9420FFF N N N
 DDRC       FTDDRII030           1          12                         L H   12          L H 0x93100000-0x931FFFFF 0xF9310000-0xF9310FFF N N N
 DEVICE     FA606TE              1          -                          - -   -           - - 0xA0200000-0xA02FFFFF -                     N N N
 DEVICE     FA606TE              1          -                          - -   -           - - 0xA0300000-0xA03FFFFF -                     N N N
 DEVICE     FA626TE              1          -                          - -   -           - - 0x90000000-0x900FFFFF -                     N N N
 DEVICE     FTAHBB020            1          -                          - -   -           - - 0x94000000-0x95FFFFFF -                     N N N
 DEVICE     FTAHBB020            1          -                          - -   -           - - 0x94000000-0x95FFFFFF -                     N N N
 DEVICE     FTAHBB020            1          -                          - -   -           - - 0x96000000-0x96FFFFFF -                     N N N
 DEVICE     FTAHBB020            1          -                          - -   -           - - 0x90000000-0x93FFFFFF -                     N N N
 DEVICE     FTAPBBRG020          1          -                          - -   -           - - 0x92000000-0x93FFFFFF -                     N N N
 DEVICE     FTDDRII030           1          -                          - -   -           - - 0x10000000-0x2FFFFFFF -                     N N N
 DEVICE     FTDDRII030           1          -                          - -   -           - - 0x0-0x3FFFFFFF        -                     N N N
 DEVICE     FTDDRII030           1          -                          - -   -           - - 0x0-0x3FFFFFFF        -                     N N N
 DEVICE     FTEMC020             1          -                          - -   -           - - 0xA0000000-0xA00FFFFF -                     N N N
 DEVICE     FTEMC020             1          -                          - -   -           - - 0xA0000000-0xA00FFFFF -                     N N N
 DEVICE     FTSMC020             1          -                          - -   -           - - 0x0-0xFFFFFFF         -                     N N N
 EBIEBI     FTEBI020             1          -                          - -   -           - - 0x94900000-0x949FFFFF 0xF9490000-0xF9490FFF N N N
 EBIEBI     FTEBI020             1          -                          - -   -           - - 0x94900000-0x949FFFFF 0xF9490000-0xF9490FFF N N N
 GPIO       FTGPIO010            1          47                         L H   47          L H 0x92500000-0x925FFFFF 0xF9250000-0xF9250FFF N N N
 GPIO       FTGPIO010            1          48                         L H   48          L H 0x92600000-0x926FFFFF 0xF9260000-0xF9260FFF N N N
 IDE        FTIDE020             1          40                         L H   40          L H 0x90400000-0x904FFFFF 0xF9040000-0xF9040FFF N N N
 IIC        FTIIC010             1          51                         L H   51          L H 0x92900000-0x929FFFFF 0xF9290000-0xF9290FFF N N N
 IIC        FTIIC010             1          52                         L H   52          L H 0x92A00000-0x92AFFFFF 0xF92A0000-0xF92A0FFF N N N
 INTC       FTINTC020            1          -                          - -   -           - - 0x90100000-0x901FFFFF 0xF9010000-0xF9010FFF N N N
 INTC       FTINTC020            1          0,1,2,3,4,5,6,7            L H   0,1,2,3,4,5 L H 0x96000000-0x960FFFFF 0xF9600000-0xF9600FFF N N N
 KBC        FTKBC010             1          21                         L H   21          L H 0x92F00000-0x92FFFFFF 0xF92F0000-0xF92F0FFF N N N
 LCDC       FTLCDC200            1          22,23,24,25                L H   22,23,24,25 L H 0x94A00000-0x94AFFFFF 0xF94A0000-0xF94AFFFF N N N
 MAC        FTGMAC100            1          32                         L H   32          L H 0x90C00000-0x90CFFFFF 0xF90C0000-0xF90C0FFF N N N
 MCP        FTMCP100             1          27                         L H   27          L H 0x96200000-0x962FFFFF 0xF9700000-0xF97FFFFF N N N
 MCP        FTMCP220             1          28                         L H   28          L H 0x96300000-0x963FFFFF 0xF9800000-0xF98F0FFF N N N
 NANDC      FTNANDC021           1          30                         L H   30          L H 0x90200000-0x902FFFFF 0xF9020000-0xF9020FFF N N N
 PCI0       -                    -          35                         L H   -           - - -                     -                     N N N
 PCIIO      FTPCIE3914           1          -                          - -   -           - - 0x60000000-0x6FFFFFFF 0xF6000000-0xF6000FFF N N N
 PCIMEM     FTPCIE3914           1          -                          - -   -           - - 0x40000000-0x4FFFFFFF 0xF4000000-0xF4000FFF N N N
 RTC        FTRTC011             1          42,43,44,45                L H   42,43,44,45 L H 0x92100000-0x921FFFFF 0xF9210000-0xF9210FFF N N N
 SATA       FTSATA100            1          33                         L H   -           - - 0x90A00000-0x90AFFFFF 0xF90A0000-0xF90A0FFF N N N
 SATA       FTSATA110            1          34                         L H   -           - - 0x90B00000-0x90BFFFFF 0xF90B0000-0xF90B0FFF N N N
 SCU        FTSCU010             1          41                         L H   41          L H 0x92000000-0x920FFFFF 0xF9200000-0xF9200FFF N N N
 SDC        FTSDC010             1          38                         L H   38          L H 0x90500000-0x905FFFFF 0xF9050000-0xF9050FFF N N N
 SDC        FTSDC010             1          39                         L H   39          L H 0x90600000-0x906FFFFF 0xF9060000-0xF9060FFF N N N
 SMC        FTSMC020             1          -                          - -   -           - - 0x94800000-0x948FFFFF 0xF9480000-0xF9480FFF N N N
 SSP        FTSSP010             1          49                         L H   49          L H 0x92700000-0x927FFFFF 0xF9270000-0xF9270FFF N N N
 SSP        FTSSP010             1          50                         L H   50          L H 0x92800000-0x928FFFFF 0xF9280000-0xF9280FFF N N N
 TIMER      FTPWMTMR010          1          8,9,10,11                  L H   8,9,10,11   L H 0x92300000-0x923FFFFF 0xF9230000-0xF9230FFF N N N
 UART       FTUART010            1          53                         L H   53          L H 0x92B00000-0x92BFFFFF 0xF92B0000-0xF92B0FFF N N N
 UART       FTUART010            1          54                         L H   54          L H 0x92C00000-0x92CFFFFF 0xF92C0000-0xF92C0FFF N N N
 UART       FTUART010            1          55                         L H   55          L H 0x92D00000-0x92DFFFFF 0xF92D0000-0xF92D0FFF N N N
 UART       FTUART010            1          56                         L H   56          L H 0x92E00000-0x92EFFFFF 0xF92E0000-0xF92E0FFF N N N
 USB        FOTG2XX              1          37                         L H   37          L H 0x90900000-0x909FFFFF 0xF9090000-0xF9090FFF N N N
 USB        FUSBH200             1          36                         L H   36          L H 0x90800000-0x908FFFFF 0xF9080000-0xF9080FFF N N N
 WDT        FTWDT010             1          46                         L H   46          L H 0x92200000-0x922FFFFF 0xF9220000-0xF9220FFF N N N
 APBDMA     -                    -          14                         L H   -           - - -                     -                     N N N
 AHBDMA     -                    -          15                         L H   -           - - -                     -                     N N N
 AHBDMA_1   -                    -          17                         L H   -           - - -                     -                     N N N


Specification	PCI0
IRQs		4

#-------------------------------------------------------------------------------------------------------------------------------------
# IP        Name                 Version    IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 PCI        FTPCIE3914           1          0-3                        L H   -           - - 0x90700000-0x907FFFFF 0xF9070000-0xF9070FFF N N N


Specification	APBDMA
IRQs		4

#-------------------------------------------------------------------------------------------------------------------------------------
# IP        Name                 Version    IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 APBBRG     FTAPBBRG020          1          0-3                        L H   -           - - 0x90F00000-0x90FFFFFF 0xF90F0000-0xF90F0FFF N N N


Specification	AHBDMA
IRQs		8

#-------------------------------------------------------------------------------------------------------------------------------------
# IP        Name                 Version    IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 DMAC       FTDMAC020            1          0-7                        L H   -           - - 0x90300000-0x903FFFFF 0xF9030000-0xF9030FFF N N N


Specification	AHBDMA_1
IRQs		8

#-------------------------------------------------------------------------------------------------------------------------------------
# IP        Name                 Version    IRQ                        E T   FIQ         E T Physical Address      Virtual Address       C B D
#-------------------------------------------------------------------------------------------------------------------------------------
 DMAC       FTDMAC020            -          0-7                        L H   -           - - 0x96100000-0x961FFFFF 0xF9610000-0xF9610FFF N N N


