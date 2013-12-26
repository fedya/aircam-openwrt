#source: jmptable.s
#as: --linkrelax
#objdump: -dr
#name: jmptable_linkrelax

.*: +file format .*

Disassembly of section \.text:

00000000 <jmptable_test>:
   0:	fe c8 00 00 	sub r8,pc,0
			0: R_AVR32_16N_PCREL	\.text\+0xc
   4:	f0 00 00 2f 	add pc,r8,r0<<0x2
   8:	d7 03       	nop
   a:	00 00       	add r0,r0
			a: R_AVR32_ALIGN	\*ABS\*\+0x2
   c:	c0 08       	rjmp c <jmptable_test\+0xc>
			c: R_AVR32_11H_PCREL	\.text\+0x12
   e:	c0 08       	rjmp e <jmptable_test\+0xe>
			e: R_AVR32_11H_PCREL	\.text\+0x14
  10:	c0 08       	rjmp 10 <jmptable_test\+0x10>
			10: R_AVR32_11H_PCREL	\.text\+0x16
  12:	d7 03       	nop
  14:	d7 03       	nop
  16:	d7 03       	nop
