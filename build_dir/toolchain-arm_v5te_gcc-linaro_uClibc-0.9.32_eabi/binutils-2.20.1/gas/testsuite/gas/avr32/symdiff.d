#source: symdiff.s
#as:
#objdump: -dr
#name: symdiff

.*: +file format .*

Disassembly of section \.text:

00000000 <diff32>:
   0:	00 00       	add r0,r0
   2:	00 04       	add r4,r0

00000004 <diff16>:
   4:	00 04       	add r4,r0

00000006 <diff8>:
   6:	04 00       	add r0,r2

00000008 <symdiff_test>:
   8:	d7 03       	nop
   a:	d7 03       	nop
   c:	d7 03       	nop
   e:	d7 03       	nop
