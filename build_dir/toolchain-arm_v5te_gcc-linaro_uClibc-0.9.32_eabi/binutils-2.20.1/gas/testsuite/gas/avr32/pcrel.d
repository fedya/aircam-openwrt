#as:
#objdump: -dr
#name: pcrel

.*: +file format .*

Disassembly of section \.text:

00000000 <test_rjmp>:
   0:	d7 03       	nop
   2:	c0 28       	rjmp 6 <test_rjmp\+0x6>
   4:	d7 03       	nop
   6:	e0 8f 00 00 	bral 6 <test_rjmp\+0x6>
			6: R_AVR32_22H_PCREL	extsym10

0000000a <test_rcall>:
   a:	d7 03       	nop
0000000c <test_rcall2>:
   c:	c0 2c       	rcall 10 <test_rcall2\+0x4>
   e:	d7 03       	nop
  10:	e0 a0 00 00 	rcall 10 <test_rcall2\+0x4>
			10: R_AVR32_22H_PCREL	extsym21

00000014 <test_branch>:
  14:	c0 31       	brne 1a <test_branch\+0x6>
  16:	e0 8f 00 00 	bral 16 <test_branch\+0x2>
			16: R_AVR32_22H_PCREL	test_branch
  1a:	e0 80 00 00 	breq 1a <test_branch\+0x6>
			1a: R_AVR32_22H_PCREL	extsym21

0000001e <test_lddpc>:
  1e:	48 30       	lddpc r0,28 <sym1>
  20:	48 20       	lddpc r0,28 <sym1>
  22:	fe f0 00 00 	ld.w r0,pc\[0\]
			22: R_AVR32_16B_PCREL	extsym16
	\.\.\.

00000028 <sym1>:
  28:	d7 03       	nop
  2a:	d7 03       	nop

0000002c <test_local>:
  2c:	48 20       	lddpc r0,34 <test_local\+0x8>
  2e:	48 30       	lddpc r0,38 <test_local\+0xc>
  30:	48 20       	lddpc r0,38 <test_local\+0xc>
  32:	00 00       	add r0,r0
  34:	d7 03       	nop
  36:	d7 03       	nop
  38:	d7 03       	nop
  3a:	d7 03       	nop

Disassembly of section \.text\.init:

00000000 <test_inter_section>:
   0:	e0 a0 .. .. 	rcall [0-9a-f]+ <.*>
			0: R_AVR32_22H_PCREL	test_rcall
   4:	d7 03       	nop
   6:	e0 a0 .. .. 	rcall [0-9a-f]+ <.*>
			6: R_AVR32_22H_PCREL	test_rcall
   a:	e0 a0 .. .. 	rcall [0-9a-z]+ <.*>
			a: R_AVR32_22H_PCREL	\.text\+0xc
   e:	d7 03       	nop
  10:	e0 a0 .. .. 	rcall [0-9a-f]+ <.*>
			10: R_AVR32_22H_PCREL	\.text\+0xc
