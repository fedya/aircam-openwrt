#name: AVR32 ELF PC-relative external relocs
#source: symbols.s
#source: ../../../gas/testsuite/gas/avr32/pcrel.s
#ld: -T $srcdir/$subdir/pcrel.ld
#objdump: -d

.*:     file format elf.*avr32.*

Disassembly of section .text:

a0000000 <_start>:
a0000000:	d7 03       	nop
a0000002:	d7 03       	nop

a0000004 <test_rjmp>:
a0000004:	d7 03       	nop
a0000006:	c0 28       	rjmp a000000a <test_rjmp\+0x6>
a0000008:	d7 03       	nop
a000000a:	e0 8f 01 fb 	bral a0000400 <extsym10>

a000000e <test_rcall>:
a000000e:	d7 03       	nop
a0000010 <test_rcall2>:
a0000010:	c0 2c       	rcall a0000014 <test_rcall2\+0x4>
a0000012:	d7 03       	nop
a0000014:	ee b0 ff f6 	rcall a0200000 <extsym21>

a0000018 <test_branch>:
a0000018:	c0 31       	brne a000001e <test_branch\+0x6>
a000001a:	fe 9f ff ff 	bral a0000018 <test_branch>
a000001e:	ee 90 ff f1 	breq a0200000 <extsym21>

a0000022 <test_lddpc>:
a0000022:	48 30       	lddpc r0,a000002c <sym1>
a0000024:	48 20       	lddpc r0,a000002c <sym1>
a0000026:	fe f0 7f da 	ld.w r0,pc\[32730\]
	...

a000002c <sym1>:
a000002c:	d7 03       	nop
a000002e:	d7 03       	nop

a0000030 <test_local>:
a0000030:	48 20       	lddpc r0,a0000038 <test_local\+0x8>
a0000032:	48 30       	lddpc r0,a000003c <test_local\+0xc>
a0000034:	48 20       	lddpc r0,a000003c <test_local\+0xc>
a0000036:	00 00       	add r0,r0
a0000038:	d7 03       	nop
a000003a:	d7 03       	nop
a000003c:	d7 03       	nop
a000003e:	d7 03       	nop

Disassembly of section \.text\.init:
a0000040 <test_inter_section>:
a0000040:	fe b0 ff e7 	rcall a000000e <test_rcall>
a0000044:	d7 03       	nop
a0000046:	fe b0 ff e4 	rcall a000000e <test_rcall>
a000004a:	fe b0 ff e3 	rcall a0000010 <test_rcall2>
a000004e:	d7 03       	nop
a0000050:	fe b0 ff e0 	rcall a0000010 <test_rcall2>

Disassembly of section \.text\.pcrel10:

a0000400 <extsym10>:
a0000400:	d7 03       	nop

Disassembly of section \.text\.pcrel16:

a0008000 <extsym16>:
a0008000:	d7 03       	nop

Disassembly of section \.text\.pcrel21:
a0200000 <extsym21>:
a0200000:	d7 03       	nop
