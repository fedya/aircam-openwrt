#source: lda.s
#as: --pic --linkrelax
#objdump: -dr
#name: lda_pic_linkrelax

.*: +file format .*

Disassembly of section \.text:

00000000 <lda_test>:
       0:	f2 c8 00 00 	sub r8,r9,0

00000004 <far_negative>:
       4:	f6 ca 00 00 	sub r10,r11,0
	...
    8000:	e0 60 00 00 	mov r0,0
			8000: R_AVR32_LDA_GOT	far_negative
    8004:	ec 00 03 20 	ld\.w r0,r6\[r0<<0x2\]
    8008:	e0 61 00 00 	mov r1,0
			8008: R_AVR32_LDA_GOT	toofar_negative
    800c:	ec 01 03 21 	ld\.w r1,r6\[r1<<0x2\]
    8010:	e0 62 00 00 	mov r2,0
			8010: R_AVR32_LDA_GOT	different_section
    8014:	ec 02 03 22 	ld\.w r2,r6\[r2<<0x2\]
    8018:	e0 63 00 00 	mov r3,0
			8018: R_AVR32_LDA_GOT	undefined
    801c:	ec 03 03 23 	ld\.w r3,r6\[r3<<0x2\]
    8020:	e0 64 00 00 	mov r4,0
			8020: R_AVR32_LDA_GOT	toofar_positive
    8024:	ec 04 03 24 	ld\.w r4,r6\[r4<<0x2\]
    8028:	e0 65 00 00 	mov r5,0
			8028: R_AVR32_LDA_GOT	far_positive
    802c:	ec 05 03 25 	ld\.w r5,r6\[r5<<0x2\]
	...

00010018 <far_positive>:
   10018:	fa cc 00 00 	sub r12,sp,0
	...
0001002c <toofar_positive>:
   1002c:	fe ce 00 00 	sub lr,pc,0
