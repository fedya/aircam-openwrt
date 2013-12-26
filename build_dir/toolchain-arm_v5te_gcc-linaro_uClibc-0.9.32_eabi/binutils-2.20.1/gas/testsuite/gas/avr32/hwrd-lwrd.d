#as:
#objdump: -dr
#name: hwrd-lwrd

.*: +file format .*

Disassembly of section \.text:

00000000 <test_hwrd>:
   0:	e0 60 87 65 	mov r0,34661
   4:	e0 60 12 34 	mov r0,4660
   8:	e0 60 00 00 	mov r0,0
			8: R_AVR32_HI16	\.text\+0x60
   c:	e0 60 00 00 	mov r0,0
			c: R_AVR32_HI16	extsym1
  10:	ea 10 87 65 	orh r0,0x8765
  14:	ea 10 12 34 	orh r0,0x1234
  18:	ea 10 00 00 	orh r0,0x0
			18: R_AVR32_HI16	\.text\+0x60
  1c:	ea 10 00 00 	orh r0,0x0
			1c: R_AVR32_HI16	extsym1
  20:	e4 10 87 65 	andh r0,0x8765
  24:	e4 10 12 34 	andh r0,0x1234
  28:	e4 10 00 00 	andh r0,0x0
			28: R_AVR32_HI16	\.text\+0x60
  2c:	e4 10 00 00 	andh r0,0x0
			2c: R_AVR32_HI16	extsym1

00000030 <test_lwrd>:
  30:	e0 60 43 21 	mov r0,17185
  34:	e0 60 56 78 	mov r0,22136
  38:	e0 60 00 00 	mov r0,0
			38: R_AVR32_LO16	\.text\+0x60
  3c:	e0 60 00 00 	mov r0,0
			3c: R_AVR32_LO16	extsym1
  40:	e8 10 43 21 	orl r0,0x4321
  44:	e8 10 56 78 	orl r0,0x5678
  48:	e8 10 00 00 	orl r0,0x0
			48: R_AVR32_LO16	\.text\+0x60
  4c:	e8 10 00 00 	orl r0,0x0
			4c: R_AVR32_LO16	extsym1
  50:	e0 10 43 21 	andl r0,0x4321
  54:	e0 10 56 78 	andl r0,0x5678
  58:	e0 10 00 00 	andl r0,0x0
			58: R_AVR32_LO16	\.text\+0x60
  5c:	e0 10 00 00 	andl r0,0x0
			5c: R_AVR32_LO16	extsym1
