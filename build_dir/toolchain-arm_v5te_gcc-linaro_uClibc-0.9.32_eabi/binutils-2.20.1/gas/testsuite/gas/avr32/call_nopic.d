#source: call.s
#as:
#objdump: -dr
#name: call_nopic

.*: +file format .*

Disassembly of section \.text:

00000000 <call_test>:
       0:	d7 03       	nop

00000002 <toofar_negative>:
	\.\.\.
  1ffffe:	00 00       	add r0,r0
  200000:	f0 a0 00 00 	rcall 0 <call_test>
  200004:	f0 1f 00 0c 	mcall 200034 <toofar_negative\+0x200032>
  200008:	f0 1f 00 0c 	mcall 200038 <toofar_negative\+0x200036>
  20000c:	f0 1f 00 0c 	mcall 20003c <toofar_negative\+0x20003a>
  200010:	f0 1f 00 0c 	mcall 200040 <toofar_negative\+0x20003e>
	\.\.\.
  200030:	ee b0 ff ff 	rcall 40002e <far_positive>
	\.\.\.
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
