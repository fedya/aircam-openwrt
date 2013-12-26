#as:
#objdump: -dr
#name: fpinsn

.*: +file format .*

Disassembly of section \.text:

[0-9a-f]* <fadd_s>:
 *[0-9a-f]*:	e1 a2 0f ff 	cop cp0,cr15,cr15,cr15,0x4
 *[0-9a-f]*:	e1 a2 00 00 	cop cp0,cr0,cr0,cr0,0x4
 *[0-9a-f]*:	e1 a2 00 ff 	cop cp0,cr0,cr15,cr15,0x4
 *[0-9a-f]*:	e1 a2 0f 0f 	cop cp0,cr15,cr0,cr15,0x4
 *[0-9a-f]*:	e1 a2 0f f0 	cop cp0,cr15,cr15,cr0,0x4
 *[0-9a-f]*:	e1 a2 07 88 	cop cp0,cr7,cr8,cr8,0x4
 *[0-9a-f]*:	e1 a2 08 78 	cop cp0,cr8,cr7,cr8,0x4
 *[0-9a-f]*:	e1 a2 08 87 	cop cp0,cr8,cr8,cr7,0x4

[0-9a-f]* <fsub_s>:
 *[0-9a-f]*:	e1 a2 1f ff 	cop cp0,cr15,cr15,cr15,0x5
 *[0-9a-f]*:	e1 a2 10 00 	cop cp0,cr0,cr0,cr0,0x5
 *[0-9a-f]*:	e1 a2 10 ff 	cop cp0,cr0,cr15,cr15,0x5
 *[0-9a-f]*:	e1 a2 1f 0f 	cop cp0,cr15,cr0,cr15,0x5
 *[0-9a-f]*:	e1 a2 1f f0 	cop cp0,cr15,cr15,cr0,0x5
 *[0-9a-f]*:	e1 a2 17 88 	cop cp0,cr7,cr8,cr8,0x5
 *[0-9a-f]*:	e1 a2 18 78 	cop cp0,cr8,cr7,cr8,0x5
 *[0-9a-f]*:	e1 a2 18 87 	cop cp0,cr8,cr8,cr7,0x5

[0-9a-f]* <fmac_s>:
 *[0-9a-f]*:	e1 a0 0f ff 	cop cp0,cr15,cr15,cr15,0x0
 *[0-9a-f]*:	e1 a0 00 00 	cop cp0,cr0,cr0,cr0,0x0
 *[0-9a-f]*:	e1 a0 00 ff 	cop cp0,cr0,cr15,cr15,0x0
 *[0-9a-f]*:	e1 a0 0f 0f 	cop cp0,cr15,cr0,cr15,0x0
 *[0-9a-f]*:	e1 a0 0f f0 	cop cp0,cr15,cr15,cr0,0x0
 *[0-9a-f]*:	e1 a0 07 88 	cop cp0,cr7,cr8,cr8,0x0
 *[0-9a-f]*:	e1 a0 08 78 	cop cp0,cr8,cr7,cr8,0x0
 *[0-9a-f]*:	e1 a0 08 87 	cop cp0,cr8,cr8,cr7,0x0

[0-9a-f]* <fnmac_s>:
 *[0-9a-f]*:	e1 a0 1f ff 	cop cp0,cr15,cr15,cr15,0x1
 *[0-9a-f]*:	e1 a0 10 00 	cop cp0,cr0,cr0,cr0,0x1
 *[0-9a-f]*:	e1 a0 10 ff 	cop cp0,cr0,cr15,cr15,0x1
 *[0-9a-f]*:	e1 a0 1f 0f 	cop cp0,cr15,cr0,cr15,0x1
 *[0-9a-f]*:	e1 a0 1f f0 	cop cp0,cr15,cr15,cr0,0x1
 *[0-9a-f]*:	e1 a0 17 88 	cop cp0,cr7,cr8,cr8,0x1
 *[0-9a-f]*:	e1 a0 18 78 	cop cp0,cr8,cr7,cr8,0x1
 *[0-9a-f]*:	e1 a0 18 87 	cop cp0,cr8,cr8,cr7,0x1

[0-9a-f]* <fmsc_s>:
 *[0-9a-f]*:	e1 a1 0f ff 	cop cp0,cr15,cr15,cr15,0x2
 *[0-9a-f]*:	e1 a1 00 00 	cop cp0,cr0,cr0,cr0,0x2
 *[0-9a-f]*:	e1 a1 00 ff 	cop cp0,cr0,cr15,cr15,0x2
 *[0-9a-f]*:	e1 a1 0f 0f 	cop cp0,cr15,cr0,cr15,0x2
 *[0-9a-f]*:	e1 a1 0f f0 	cop cp0,cr15,cr15,cr0,0x2
 *[0-9a-f]*:	e1 a1 07 88 	cop cp0,cr7,cr8,cr8,0x2
 *[0-9a-f]*:	e1 a1 08 78 	cop cp0,cr8,cr7,cr8,0x2
 *[0-9a-f]*:	e1 a1 08 87 	cop cp0,cr8,cr8,cr7,0x2

[0-9a-f]* <fnmsc_s>:
 *[0-9a-f]*:	e1 a1 1f ff 	cop cp0,cr15,cr15,cr15,0x3
 *[0-9a-f]*:	e1 a1 10 00 	cop cp0,cr0,cr0,cr0,0x3
 *[0-9a-f]*:	e1 a1 10 ff 	cop cp0,cr0,cr15,cr15,0x3
 *[0-9a-f]*:	e1 a1 1f 0f 	cop cp0,cr15,cr0,cr15,0x3
 *[0-9a-f]*:	e1 a1 1f f0 	cop cp0,cr15,cr15,cr0,0x3
 *[0-9a-f]*:	e1 a1 17 88 	cop cp0,cr7,cr8,cr8,0x3
 *[0-9a-f]*:	e1 a1 18 78 	cop cp0,cr8,cr7,cr8,0x3
 *[0-9a-f]*:	e1 a1 18 87 	cop cp0,cr8,cr8,cr7,0x3

[0-9a-f]* <fmul_s>:
 *[0-9a-f]*:	e1 a3 0f ff 	cop cp0,cr15,cr15,cr15,0x6
 *[0-9a-f]*:	e1 a3 00 00 	cop cp0,cr0,cr0,cr0,0x6
 *[0-9a-f]*:	e1 a3 00 ff 	cop cp0,cr0,cr15,cr15,0x6
 *[0-9a-f]*:	e1 a3 0f 0f 	cop cp0,cr15,cr0,cr15,0x6
 *[0-9a-f]*:	e1 a3 0f f0 	cop cp0,cr15,cr15,cr0,0x6
 *[0-9a-f]*:	e1 a3 07 88 	cop cp0,cr7,cr8,cr8,0x6
 *[0-9a-f]*:	e1 a3 08 78 	cop cp0,cr8,cr7,cr8,0x6
 *[0-9a-f]*:	e1 a3 08 87 	cop cp0,cr8,cr8,cr7,0x6

[0-9a-f]* <fnmul_s>:
 *[0-9a-f]*:	e1 a3 1f ff 	cop cp0,cr15,cr15,cr15,0x7
 *[0-9a-f]*:	e1 a3 10 00 	cop cp0,cr0,cr0,cr0,0x7
 *[0-9a-f]*:	e1 a3 10 ff 	cop cp0,cr0,cr15,cr15,0x7
 *[0-9a-f]*:	e1 a3 1f 0f 	cop cp0,cr15,cr0,cr15,0x7
 *[0-9a-f]*:	e1 a3 1f f0 	cop cp0,cr15,cr15,cr0,0x7
 *[0-9a-f]*:	e1 a3 17 88 	cop cp0,cr7,cr8,cr8,0x7
 *[0-9a-f]*:	e1 a3 18 78 	cop cp0,cr8,cr7,cr8,0x7
 *[0-9a-f]*:	e1 a3 18 87 	cop cp0,cr8,cr8,cr7,0x7

[0-9a-f]* <fneg_s>:
 *[0-9a-f]*:	e1 a4 0f f0 	cop cp0,cr15,cr15,cr0,0x8
 *[0-9a-f]*:	e1 a4 00 00 	cop cp0,cr0,cr0,cr0,0x8
 *[0-9a-f]*:	e1 a4 00 f0 	cop cp0,cr0,cr15,cr0,0x8
 *[0-9a-f]*:	e1 a4 0f 00 	cop cp0,cr15,cr0,cr0,0x8
 *[0-9a-f]*:	e1 a4 07 80 	cop cp0,cr7,cr8,cr0,0x8
 *[0-9a-f]*:	e1 a4 08 70 	cop cp0,cr8,cr7,cr0,0x8

[0-9a-f]* <fabs_s>:
 *[0-9a-f]*:	e1 a4 1f f0 	cop cp0,cr15,cr15,cr0,0x9
 *[0-9a-f]*:	e1 a4 10 00 	cop cp0,cr0,cr0,cr0,0x9
 *[0-9a-f]*:	e1 a4 10 f0 	cop cp0,cr0,cr15,cr0,0x9
 *[0-9a-f]*:	e1 a4 1f 00 	cop cp0,cr15,cr0,cr0,0x9
 *[0-9a-f]*:	e1 a4 17 80 	cop cp0,cr7,cr8,cr0,0x9
 *[0-9a-f]*:	e1 a4 18 70 	cop cp0,cr8,cr7,cr0,0x9

[0-9a-f]* <fcmp_s>:
 *[0-9a-f]*:	e1 a6 10 ff 	cop cp0,cr0,cr15,cr15,0xd
 *[0-9a-f]*:	e1 a6 10 00 	cop cp0,cr0,cr0,cr0,0xd
 *[0-9a-f]*:	e1 a6 10 0f 	cop cp0,cr0,cr0,cr15,0xd
 *[0-9a-f]*:	e1 a6 10 f0 	cop cp0,cr0,cr15,cr0,0xd
 *[0-9a-f]*:	e1 a6 10 78 	cop cp0,cr0,cr7,cr8,0xd
 *[0-9a-f]*:	e1 a6 10 87 	cop cp0,cr0,cr8,cr7,0xd

[0-9a-f]* <fadd_d>:
 *[0-9a-f]*:	e5 a2 0e ee 	cop cp0,cr14,cr14,cr14,0x44
 *[0-9a-f]*:	e5 a2 00 00 	cop cp0,cr0,cr0,cr0,0x44
 *[0-9a-f]*:	e5 a2 00 ee 	cop cp0,cr0,cr14,cr14,0x44
 *[0-9a-f]*:	e5 a2 0e 0e 	cop cp0,cr14,cr0,cr14,0x44
 *[0-9a-f]*:	e5 a2 0e e0 	cop cp0,cr14,cr14,cr0,0x44
 *[0-9a-f]*:	e5 a2 06 88 	cop cp0,cr6,cr8,cr8,0x44
 *[0-9a-f]*:	e5 a2 08 68 	cop cp0,cr8,cr6,cr8,0x44
 *[0-9a-f]*:	e5 a2 08 86 	cop cp0,cr8,cr8,cr6,0x44

[0-9a-f]* <fsub_d>:
 *[0-9a-f]*:	e5 a2 1e ee 	cop cp0,cr14,cr14,cr14,0x45
 *[0-9a-f]*:	e5 a2 10 00 	cop cp0,cr0,cr0,cr0,0x45
 *[0-9a-f]*:	e5 a2 10 ee 	cop cp0,cr0,cr14,cr14,0x45
 *[0-9a-f]*:	e5 a2 1e 0e 	cop cp0,cr14,cr0,cr14,0x45
 *[0-9a-f]*:	e5 a2 1e e0 	cop cp0,cr14,cr14,cr0,0x45
 *[0-9a-f]*:	e5 a2 16 88 	cop cp0,cr6,cr8,cr8,0x45
 *[0-9a-f]*:	e5 a2 18 68 	cop cp0,cr8,cr6,cr8,0x45
 *[0-9a-f]*:	e5 a2 18 86 	cop cp0,cr8,cr8,cr6,0x45

[0-9a-f]* <fmac_d>:
 *[0-9a-f]*:	e5 a0 0e ee 	cop cp0,cr14,cr14,cr14,0x40
 *[0-9a-f]*:	e5 a0 00 00 	cop cp0,cr0,cr0,cr0,0x40
 *[0-9a-f]*:	e5 a0 00 ee 	cop cp0,cr0,cr14,cr14,0x40
 *[0-9a-f]*:	e5 a0 0e 0e 	cop cp0,cr14,cr0,cr14,0x40
 *[0-9a-f]*:	e5 a0 0e e0 	cop cp0,cr14,cr14,cr0,0x40
 *[0-9a-f]*:	e5 a0 06 88 	cop cp0,cr6,cr8,cr8,0x40
 *[0-9a-f]*:	e5 a0 08 68 	cop cp0,cr8,cr6,cr8,0x40
 *[0-9a-f]*:	e5 a0 08 86 	cop cp0,cr8,cr8,cr6,0x40

[0-9a-f]* <fnmac_d>:
 *[0-9a-f]*:	e5 a0 1e ee 	cop cp0,cr14,cr14,cr14,0x41
 *[0-9a-f]*:	e5 a0 10 00 	cop cp0,cr0,cr0,cr0,0x41
 *[0-9a-f]*:	e5 a0 10 ee 	cop cp0,cr0,cr14,cr14,0x41
 *[0-9a-f]*:	e5 a0 1e 0e 	cop cp0,cr14,cr0,cr14,0x41
 *[0-9a-f]*:	e5 a0 1e e0 	cop cp0,cr14,cr14,cr0,0x41
 *[0-9a-f]*:	e5 a0 16 88 	cop cp0,cr6,cr8,cr8,0x41
 *[0-9a-f]*:	e5 a0 18 68 	cop cp0,cr8,cr6,cr8,0x41
 *[0-9a-f]*:	e5 a0 18 86 	cop cp0,cr8,cr8,cr6,0x41

[0-9a-f]* <fmsc_d>:
 *[0-9a-f]*:	e5 a1 0e ee 	cop cp0,cr14,cr14,cr14,0x42
 *[0-9a-f]*:	e5 a1 00 00 	cop cp0,cr0,cr0,cr0,0x42
 *[0-9a-f]*:	e5 a1 00 ee 	cop cp0,cr0,cr14,cr14,0x42
 *[0-9a-f]*:	e5 a1 0e 0e 	cop cp0,cr14,cr0,cr14,0x42
 *[0-9a-f]*:	e5 a1 0e e0 	cop cp0,cr14,cr14,cr0,0x42
 *[0-9a-f]*:	e5 a1 06 88 	cop cp0,cr6,cr8,cr8,0x42
 *[0-9a-f]*:	e5 a1 08 68 	cop cp0,cr8,cr6,cr8,0x42
 *[0-9a-f]*:	e5 a1 08 86 	cop cp0,cr8,cr8,cr6,0x42

[0-9a-f]* <fnmsc_d>:
 *[0-9a-f]*:	e5 a1 1e ee 	cop cp0,cr14,cr14,cr14,0x43
 *[0-9a-f]*:	e5 a1 10 00 	cop cp0,cr0,cr0,cr0,0x43
 *[0-9a-f]*:	e5 a1 10 ee 	cop cp0,cr0,cr14,cr14,0x43
 *[0-9a-f]*:	e5 a1 1e 0e 	cop cp0,cr14,cr0,cr14,0x43
 *[0-9a-f]*:	e5 a1 1e e0 	cop cp0,cr14,cr14,cr0,0x43
 *[0-9a-f]*:	e5 a1 16 88 	cop cp0,cr6,cr8,cr8,0x43
 *[0-9a-f]*:	e5 a1 18 68 	cop cp0,cr8,cr6,cr8,0x43
 *[0-9a-f]*:	e5 a1 18 86 	cop cp0,cr8,cr8,cr6,0x43

[0-9a-f]* <fmul_d>:
 *[0-9a-f]*:	e5 a3 0e ee 	cop cp0,cr14,cr14,cr14,0x46
 *[0-9a-f]*:	e5 a3 00 00 	cop cp0,cr0,cr0,cr0,0x46
 *[0-9a-f]*:	e5 a3 00 ee 	cop cp0,cr0,cr14,cr14,0x46
 *[0-9a-f]*:	e5 a3 0e 0e 	cop cp0,cr14,cr0,cr14,0x46
 *[0-9a-f]*:	e5 a3 0e e0 	cop cp0,cr14,cr14,cr0,0x46
 *[0-9a-f]*:	e5 a3 06 88 	cop cp0,cr6,cr8,cr8,0x46
 *[0-9a-f]*:	e5 a3 08 68 	cop cp0,cr8,cr6,cr8,0x46
 *[0-9a-f]*:	e5 a3 08 86 	cop cp0,cr8,cr8,cr6,0x46

[0-9a-f]* <fnmul_d>:
 *[0-9a-f]*:	e5 a3 1e ee 	cop cp0,cr14,cr14,cr14,0x47
 *[0-9a-f]*:	e5 a3 10 00 	cop cp0,cr0,cr0,cr0,0x47
 *[0-9a-f]*:	e5 a3 10 ee 	cop cp0,cr0,cr14,cr14,0x47
 *[0-9a-f]*:	e5 a3 1e 0e 	cop cp0,cr14,cr0,cr14,0x47
 *[0-9a-f]*:	e5 a3 1e e0 	cop cp0,cr14,cr14,cr0,0x47
 *[0-9a-f]*:	e5 a3 16 88 	cop cp0,cr6,cr8,cr8,0x47
 *[0-9a-f]*:	e5 a3 18 68 	cop cp0,cr8,cr6,cr8,0x47
 *[0-9a-f]*:	e5 a3 18 86 	cop cp0,cr8,cr8,cr6,0x47

[0-9a-f]* <fneg_d>:
 *[0-9a-f]*:	e5 a4 0e e0 	cop cp0,cr14,cr14,cr0,0x48
 *[0-9a-f]*:	e5 a4 00 00 	cop cp0,cr0,cr0,cr0,0x48
 *[0-9a-f]*:	e5 a4 00 e0 	cop cp0,cr0,cr14,cr0,0x48
 *[0-9a-f]*:	e5 a4 0e 00 	cop cp0,cr14,cr0,cr0,0x48
 *[0-9a-f]*:	e5 a4 06 80 	cop cp0,cr6,cr8,cr0,0x48
 *[0-9a-f]*:	e5 a4 08 60 	cop cp0,cr8,cr6,cr0,0x48

[0-9a-f]* <fabs_d>:
 *[0-9a-f]*:	e5 a4 1e e0 	cop cp0,cr14,cr14,cr0,0x49
 *[0-9a-f]*:	e5 a4 10 00 	cop cp0,cr0,cr0,cr0,0x49
 *[0-9a-f]*:	e5 a4 10 e0 	cop cp0,cr0,cr14,cr0,0x49
 *[0-9a-f]*:	e5 a4 1e 00 	cop cp0,cr14,cr0,cr0,0x49
 *[0-9a-f]*:	e5 a4 16 80 	cop cp0,cr6,cr8,cr0,0x49
 *[0-9a-f]*:	e5 a4 18 60 	cop cp0,cr8,cr6,cr0,0x49

[0-9a-f]* <fcmp_d>:
 *[0-9a-f]*:	e5 a6 10 ee 	cop cp0,cr0,cr14,cr14,0x4d
 *[0-9a-f]*:	e5 a6 10 00 	cop cp0,cr0,cr0,cr0,0x4d
 *[0-9a-f]*:	e5 a6 10 0e 	cop cp0,cr0,cr0,cr14,0x4d
 *[0-9a-f]*:	e5 a6 10 e0 	cop cp0,cr0,cr14,cr0,0x4d
 *[0-9a-f]*:	e5 a6 10 68 	cop cp0,cr0,cr6,cr8,0x4d
 *[0-9a-f]*:	e5 a6 10 86 	cop cp0,cr0,cr8,cr6,0x4d

[0-9a-f]* <fmov_s>:
 *[0-9a-f]*:	e1 a5 0f f0 	cop cp0,cr15,cr15,cr0,0xa
 *[0-9a-f]*:	e1 a5 00 00 	cop cp0,cr0,cr0,cr0,0xa
 *[0-9a-f]*:	e1 a5 0f 00 	cop cp0,cr15,cr0,cr0,0xa
 *[0-9a-f]*:	e1 a5 00 f0 	cop cp0,cr0,cr15,cr0,0xa
 *[0-9a-f]*:	e1 a5 08 70 	cop cp0,cr8,cr7,cr0,0xa
 *[0-9a-f]*:	e1 a5 07 80 	cop cp0,cr7,cr8,cr0,0xa
 *[0-9a-f]*:	ef af 0f 00 	mvcr.w cp0,pc,cr15
 *[0-9a-f]*:	ef a0 00 00 	mvcr.w cp0,r0,cr0
 *[0-9a-f]*:	ef af 00 00 	mvcr.w cp0,pc,cr0
 *[0-9a-f]*:	ef a0 0f 00 	mvcr.w cp0,r0,cr15
 *[0-9a-f]*:	ef a8 07 00 	mvcr.w cp0,r8,cr7
 *[0-9a-f]*:	ef a7 08 00 	mvcr.w cp0,r7,cr8
 *[0-9a-f]*:	ef af 0f 20 	mvrc.w cp0,cr15,pc
 *[0-9a-f]*:	ef a0 00 20 	mvrc.w cp0,cr0,r0
 *[0-9a-f]*:	ef a0 0f 20 	mvrc.w cp0,cr15,r0
 *[0-9a-f]*:	ef af 00 20 	mvrc.w cp0,cr0,pc
 *[0-9a-f]*:	ef a7 08 20 	mvrc.w cp0,cr8,r7
 *[0-9a-f]*:	ef a8 07 20 	mvrc.w cp0,cr7,r8

[0-9a-f]* <fmov_d>:
 *[0-9a-f]*:	e5 a5 0e e0 	cop cp0,cr14,cr14,cr0,0x4a
 *[0-9a-f]*:	e5 a5 00 00 	cop cp0,cr0,cr0,cr0,0x4a
 *[0-9a-f]*:	e5 a5 0e 00 	cop cp0,cr14,cr0,cr0,0x4a
 *[0-9a-f]*:	e5 a5 00 e0 	cop cp0,cr0,cr14,cr0,0x4a
 *[0-9a-f]*:	e5 a5 08 60 	cop cp0,cr8,cr6,cr0,0x4a
 *[0-9a-f]*:	e5 a5 06 80 	cop cp0,cr6,cr8,cr0,0x4a
 *[0-9a-f]*:	ef ae 0e 10 	mvcr.d cp0,lr,cr14
 *[0-9a-f]*:	ef a0 00 10 	mvcr.d cp0,r0,cr0
 *[0-9a-f]*:	ef ae 00 10 	mvcr.d cp0,lr,cr0
 *[0-9a-f]*:	ef a0 0e 10 	mvcr.d cp0,r0,cr14
 *[0-9a-f]*:	ef a8 06 10 	mvcr.d cp0,r8,cr6
 *[0-9a-f]*:	ef a6 08 10 	mvcr.d cp0,r6,cr8
 *[0-9a-f]*:	ef ae 0e 30 	mvrc.d cp0,cr14,lr
 *[0-9a-f]*:	ef a0 00 30 	mvrc.d cp0,cr0,r0
 *[0-9a-f]*:	ef a0 0e 30 	mvrc.d cp0,cr14,r0
 *[0-9a-f]*:	ef ae 00 30 	mvrc.d cp0,cr0,lr
 *[0-9a-f]*:	ef a6 08 30 	mvrc.d cp0,cr8,r6
 *[0-9a-f]*:	ef a8 06 30 	mvrc.d cp0,cr6,r8

[0-9a-f]* <fcasts_d>:
 *[0-9a-f]*:	e1 a7 1f e0 	cop cp0,cr15,cr14,cr0,0xf
 *[0-9a-f]*:	e1 a7 10 00 	cop cp0,cr0,cr0,cr0,0xf
 *[0-9a-f]*:	e1 a7 1f 00 	cop cp0,cr15,cr0,cr0,0xf
 *[0-9a-f]*:	e1 a7 10 e0 	cop cp0,cr0,cr14,cr0,0xf
 *[0-9a-f]*:	e1 a7 18 60 	cop cp0,cr8,cr6,cr0,0xf
 *[0-9a-f]*:	e1 a7 17 80 	cop cp0,cr7,cr8,cr0,0xf

[0-9a-f]* <fcastd_s>:
 *[0-9a-f]*:	e1 a8 0e f0 	cop cp0,cr14,cr15,cr0,0x10
 *[0-9a-f]*:	e1 a8 00 00 	cop cp0,cr0,cr0,cr0,0x10
 *[0-9a-f]*:	e1 a8 0e 00 	cop cp0,cr14,cr0,cr0,0x10
 *[0-9a-f]*:	e1 a8 00 f0 	cop cp0,cr0,cr15,cr0,0x10
 *[0-9a-f]*:	e1 a8 08 70 	cop cp0,cr8,cr7,cr0,0x10
 *[0-9a-f]*:	e1 a8 06 80 	cop cp0,cr6,cr8,cr0,0x10
