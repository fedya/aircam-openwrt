#as:
#objdump: -dr
#name: aliases

.*: +file format .*

Disassembly of section \.text:

00000000 <ld_nodisp>:
   0:	19 80      [ \t]+ld\.ub r0,r12\[0x0\]
   2:	f9 20 00 00[ \t]+ld\.sb r0,r12\[0\]
   6:	98 80      [ \t]+ld\.uh r0,r12\[0x0\]
   8:	98 00      [ \t]+ld\.sh r0,r12\[0x0\]
   a:	78 00      [ \t]+ld\.w r0,r12\[0x0\]

0000000c <st_nodisp>:
   c:	b8 80      [ \t]+st\.b r12\[0x0\],r0
   e:	b8 00      [ \t]+st\.h r12\[0x0\],r0
  10:	99 00      [ \t]+st\.w r12\[0x0\],r0
