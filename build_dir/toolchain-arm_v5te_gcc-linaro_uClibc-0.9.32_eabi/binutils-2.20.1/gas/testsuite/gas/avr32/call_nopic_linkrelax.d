#source: call.s
#as: --linkrelax
#objdump: -dr
#name: call_nopic_linkrelax

.*: +file format .*

Disassembly of section \.text:

00000000 <call_test>:
       0:	d7 03       	nop

00000002 <toofar_negative>:
	\.\.\.
  1ffffe:	00 00       	add r0,r0
  200000:	e0 a0 00 00 	rcall 200000 <toofar_negative\+0x1ffffe>
			200000: R_AVR32_22H_PCREL	\.text
  200004:	f0 1f 00 00 	mcall 200004 <toofar_negative\+0x200002>
			200004: R_AVR32_CPCALL	\.text\+0x200034
  200008:	f0 1f 00 00 	mcall 200008 <toofar_negative\+0x200006>
			200008: R_AVR32_CPCALL	\.text\+0x200038
  20000c:	f0 1f 00 00 	mcall 20000c <toofar_negative\+0x20000a>
			20000c: R_AVR32_CPCALL	\.text\+0x20003c
  200010:	f0 1f 00 00 	mcall 200010 <toofar_negative\+0x20000e>
			200010: R_AVR32_CPCALL	\.text\+0x200040
	\.\.\.
  200030:	e0 a0 00 00 	rcall 200030 <toofar_negative\+0x20002e>
			200030: R_AVR32_22H_PCREL	\.text\+0x40002e
	\.\.\.
			200034: R_AVR32_ALIGN	\*ABS\*\+0x2
			200034: R_AVR32_32_CPENT	\.text\+0x2
			200038: R_AVR32_32_CPENT	\.text\.init
			20003c: R_AVR32_32_CPENT	undefined
			200040: R_AVR32_32_CPENT	\.text\+0x40002c

0040002c <toofar_positive>:
  40002c:	d7 03       	nop
0040002e <far_positive>:
  40002e:	d7 03       	nop
Disassembly of section \.text\.init:

00000000 <different_section>:
   0:	e2 c0 00 00 	sub r0,r1,0
