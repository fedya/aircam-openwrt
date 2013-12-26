#source: jmptable.s
#as:
#objdump: -dr
#name: jmptable

.*: +file format .*

Disassembly of section \.text:

00000000 <jmptable_test>:
   0:	fe c8 ff f4 	sub r8,pc,-12
   4:	f0 00 00 2f 	add pc,r8,r0<<0x2
   8:	d7 03       	nop
   a:	00 00       	add r0,r0
   c:	c0 38       	rjmp 12 <jmptable_test\+0x12>
   e:	c0 38       	rjmp 14 <jmptable_test\+0x14>
  10:	c0 38       	rjmp 16 <jmptable_test\+0x16>
  12:	d7 03       	nop
  14:	d7 03       	nop
  16:	d7 03       	nop
