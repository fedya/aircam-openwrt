#source: lda.s
#as:
#objdump: -dr
#name: lda_nopic

.*: +file format .*

Disassembly of section \.text:

00000000 <lda_test>:
       0:	f2 c8 00 00 	sub r8,r9,0

00000004 <far_negative>:
       4:	f6 ca 00 00 	sub r10,r11,0
	...
    8000:	fe c0 7f fc 	sub r0,pc,32764
    8004:	48 31       	lddpc r1,8010 <far_negative\+0x800c>
    8006:	48 42       	lddpc r2,8014 <far_negative\+0x8010>
    8008:	48 43       	lddpc r3,8018 <far_negative\+0x8014>
    800a:	48 54       	lddpc r4,801c <far_negative\+0x8018>
    800c:	fe c5 80 04 	sub r5,pc,-32764
	...
			8010: R_AVR32_32_CPENT	\.text
			8014: R_AVR32_32_CPENT	\.data
			8018: R_AVR32_32_CPENT	undefined
			801c: R_AVR32_32_CPENT	\.text\+0x1001c

00010008 <far_positive>:
   10008:	fa cc 00 00 	sub r12,sp,0
	...
0001001c <toofar_positive>:
   1001c:	fe ce 00 00 	sub lr,pc,0
