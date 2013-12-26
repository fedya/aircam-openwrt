#source: lda.s
#as: --pic
#objdump: -dr
#name: lda_pic

.*: +file format .*

Disassembly of section \.text:

00000000 <lda_test>:
       0:	f2 c8 00 00 	sub r8,r9,0

00000004 <far_negative>:
       4:	f6 ca 00 00 	sub r10,r11,0
	...
    8000:	fe c0 7f fc 	sub r0,pc,32764
    8004:	ec f1 00 00 	ld.w r1,r6\[0\]
			8004: R_AVR32_GOT16S	toofar_negative
    8008:	ec f2 00 00 	ld.w r2,r6\[0\]
			8008: R_AVR32_GOT16S	different_section
    800c:	ec f3 00 00 	ld.w r3,r6\[0\]
			800c: R_AVR32_GOT16S	undefined
    8010:	ec f4 00 00 	ld.w r4,r6\[0\]
			8010: R_AVR32_GOT16S	toofar_positive
    8014:	fe c5 80 14 	sub r5,pc,-32748
	...

00010000 <far_positive>:
   10000:	fa cc 00 00 	sub r12,sp,0
	...
00010014 <toofar_positive>:
   10014:	fe ce 00 00 	sub lr,pc,0
