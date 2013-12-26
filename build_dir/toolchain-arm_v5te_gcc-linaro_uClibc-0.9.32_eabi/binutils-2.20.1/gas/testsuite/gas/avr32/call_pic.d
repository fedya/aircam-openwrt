#source: call.s
#as: --pic
#objdump: -dr
#name: call_pic

.*: +file format .*

Disassembly of section \.text:

00000000 <call_test>:
       0:	d7 03       	nop

00000002 <toofar_negative>:
	\.\.\.
  1ffffe:	00 00       	add r0,r0
  200000:	f0 a0 00 00 	rcall 0 <call_test>
  200004:	f0 16 00 00 	mcall r6\[0\]
			200004: R_AVR32_GOT18SW	toofar_negative
  200008:	f0 16 00 00 	mcall r6\[0\]
			200008: R_AVR32_GOT18SW	different_section
  20000c:	f0 16 00 00 	mcall r6\[0\]
			20000c: R_AVR32_GOT18SW	undefined
  200010:	f0 16 00 00 	mcall r6\[0\]
			200010: R_AVR32_GOT18SW	toofar_positive
	\.\.\.
  200030:	ee b0 ff ff 	rcall 40002e <far_positive>
	\.\.\.

0040002c <toofar_positive>:
  40002c:	d7 03       	nop
0040002e <far_positive>:
  40002e:	d7 03       	nop
Disassembly of section \.text\.init:

00000000 <different_section>:
   0:	e2 c0 00 00 	sub r0,r1,0
