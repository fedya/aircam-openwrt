#as:
#objdump: -dr
#name: pic_reloc

.*: +file format .*

Disassembly of section \.text:

00000000 <mcall_got>:
   0:	f0 16 00 00 	mcall r6\[0\]
			0: R_AVR32_GOT18SW	extfunc
   4:	f0 16 00 00 	mcall r6\[0\]
			4: R_AVR32_GOT18SW	\.L1
   8:	f0 16 00 00 	mcall r6\[0\]
			8: R_AVR32_GOT18SW	\.L2
   c:	f0 16 00 00 	mcall r6\[0\]
			c: R_AVR32_GOT18SW	mcall_got

00000010 <ldw_got>:
  10:	ec f0 00 00 	ld.w r0,r6\[0\]
			10: R_AVR32_GOT16S	extvar
  14:	ec f0 00 00 	ld.w r0,r6\[0\]
			14: R_AVR32_GOT16S	\.L3
  18:	ec f0 00 00 	ld.w r0,r6\[0\]
			18: R_AVR32_GOT16S	\.L4
  1c:	ec f0 00 00 	ld.w r0,r6\[0\]
			1c: R_AVR32_GOT16S	ldw_got
