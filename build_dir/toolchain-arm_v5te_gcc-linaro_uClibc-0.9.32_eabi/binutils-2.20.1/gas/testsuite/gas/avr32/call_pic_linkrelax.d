#source: call.s
#as: --pic --linkrelax
#objdump: -dr
#name: call_pic_linkrelax

.*: +file format .*

Disassembly of section \.text:

00000000 <call_test>:
       0:	d7 03       	nop

00000002 <toofar_negative>:
	\.\.\.
  1ffffe:	00 00       	add r0,r0
  200000:	e0 a0 00 00 	rcall 200000 <toofar_negative\+0x1ffffe>
			200000: R_AVR32_22H_PCREL	\.text
  200004:	e0 6e 00 00 	mov lr,0
			200004: R_AVR32_GOTCALL	toofar_negative
  200008:	ec 0e 03 2e 	ld\.w lr,r6\[lr<<0x2\]
  20000c:	5d 1e       	icall lr
  20000e:	e0 6e 00 00 	mov lr,0
			20000e: R_AVR32_GOTCALL	different_section
  200012:	ec 0e 03 2e 	ld\.w lr,r6\[lr<<0x2\]
  200016:	5d 1e       	icall lr
  200018:	e0 6e 00 00 	mov lr,0
			200018: R_AVR32_GOTCALL	undefined
  20001c:	ec 0e 03 2e 	ld\.w lr,r6\[lr<<0x2\]
  200020:	5d 1e       	icall lr
  200022:	e0 6e 00 00 	mov lr,0
			200022: R_AVR32_GOTCALL	toofar_positive
  200026:	ec 0e 03 2e 	ld\.w lr,r6\[lr<<0x2\]
  20002a:	5d 1e       	icall lr
  20002c:	00 00       	add r0,r0
  20002e:	00 00       	add r0,r0
  200030:	e0 a0 00 00 	rcall 200030 <toofar_negative\+0x20002e>
			200030: R_AVR32_22H_PCREL	\.text\+0x40002e
	\.\.\.

0040002c <toofar_positive>:
  40002c:	d7 03       	nop
0040002e <far_positive>:
  40002e:	d7 03       	nop
Disassembly of section \.text\.init:

00000000 <different_section>:
   0:	e2 c0 00 00 	sub r0,r1,0
