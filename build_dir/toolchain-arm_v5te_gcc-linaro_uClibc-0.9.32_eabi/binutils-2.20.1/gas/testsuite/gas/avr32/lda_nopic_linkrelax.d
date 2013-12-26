#source: lda.s
#as: --linkrelax
#objdump: -dr
#name: lda_nopic_linkrelax

.*: +file format .*

Disassembly of section \.text:

00000000 <lda_test>:
       0:	f2 c8 00 00 	sub r8,r9,0

00000004 <far_negative>:
       4:	f6 ca 00 00 	sub r10,r11,0
	\.\.\.
    8000:	48 00       	lddpc r0,8000 <far_negative\+0x7ffc>
			8000: R_AVR32_9W_CP	\.text\+0x800c
    8002:	48 01       	lddpc r1,8000 <far_negative\+0x7ffc>
			8002: R_AVR32_9W_CP	\.text\+0x8010
    8004:	48 02       	lddpc r2,8004 <far_negative\+0x8000>
			8004: R_AVR32_9W_CP	\.text\+0x8014
    8006:	48 03       	lddpc r3,8004 <far_negative\+0x8000>
			8006: R_AVR32_9W_CP	\.text\+0x8018
    8008:	48 04       	lddpc r4,8008 <far_negative\+0x8004>
			8008: R_AVR32_9W_CP	\.text\+0x801c
    800a:	48 05       	lddpc r5,8008 <far_negative\+0x8004>
			800a: R_AVR32_9W_CP	\.text\+0x8020
	\.\.\.
			800c: R_AVR32_ALIGN	\*ABS\*\+0x2
			800c: R_AVR32_32_CPENT	\.text\+0x4
			8010: R_AVR32_32_CPENT	\.text
			8014: R_AVR32_32_CPENT	\.data
			8018: R_AVR32_32_CPENT	undefined
			801c: R_AVR32_32_CPENT	\.text\+0x10020
			8020: R_AVR32_32_CPENT	\.text\+0x1000c

0001000c <far_positive>:
   1000c:	fa cc 00 00 	sub r12,sp,0
	\.\.\.
00010020 <toofar_positive>:
   10020:	fe ce 00 00 	sub lr,pc,0
