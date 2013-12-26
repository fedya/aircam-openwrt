#as:
#objdump: -dr
#name: pico

.*: +file format .*

Disassembly of section \.text:

[0-9a-f]* <picosvmac>:
 *[0-9a-f]*:	e1 a6 20 00 	cop cp1,cr0,cr0,cr0,0xc
 *[0-9a-f]*:	e1 a7 2b bb 	cop cp1,cr11,cr11,cr11,0xe
 *[0-9a-f]*:	e1 a6 3a 05 	cop cp1,cr10,cr0,cr5,0xd
 *[0-9a-f]*:	e1 a7 36 90 	cop cp1,cr6,cr9,cr0,0xf

[0-9a-f]* <picosvmul>:
 *[0-9a-f]*:	e1 a4 20 00 	cop cp1,cr0,cr0,cr0,0x8
 *[0-9a-f]*:	e1 a5 2b bb 	cop cp1,cr11,cr11,cr11,0xa
 *[0-9a-f]*:	e1 a4 3a 05 	cop cp1,cr10,cr0,cr5,0x9
 *[0-9a-f]*:	e1 a5 36 90 	cop cp1,cr6,cr9,cr0,0xb

[0-9a-f]* <picovmac>:
 *[0-9a-f]*:	e1 a2 20 00 	cop cp1,cr0,cr0,cr0,0x4
 *[0-9a-f]*:	e1 a3 2b bb 	cop cp1,cr11,cr11,cr11,0x6
 *[0-9a-f]*:	e1 a2 3a 05 	cop cp1,cr10,cr0,cr5,0x5
 *[0-9a-f]*:	e1 a3 36 90 	cop cp1,cr6,cr9,cr0,0x7

[0-9a-f]* <picovmul>:
 *[0-9a-f]*:	e1 a0 20 00 	cop cp1,cr0,cr0,cr0,0x0
 *[0-9a-f]*:	e1 a1 2b bb 	cop cp1,cr11,cr11,cr11,0x2
 *[0-9a-f]*:	e1 a0 3a 05 	cop cp1,cr10,cr0,cr5,0x1
 *[0-9a-f]*:	e1 a1 36 90 	cop cp1,cr6,cr9,cr0,0x3

[0-9a-f]* <picold_d>:
 *[0-9a-f]*:	e9 af 3e ff 	ldc\.d cp1,cr14,pc\[0x3fc\]
 *[0-9a-f]*:	e9 a0 30 ff 	ldc\.d cp1,cr0,r0\[0x3fc\]
 *[0-9a-f]*:	e9 a0 30 00 	ldc\.d cp1,cr0,r0\[0x0\]
 *[0-9a-f]*:	ef a8 26 50 	ldc\.d cp1,cr6,--r8
 *[0-9a-f]*:	ef a7 28 50 	ldc\.d cp1,cr8,--r7
 *[0-9a-f]*:	ef aa 32 65 	ldc\.d cp1,cr2,r10\[r5<<0x2\]
 *[0-9a-f]*:	ef a3 3c 46 	ldc\.d cp1,cr12,r3\[r6\]

[0-9a-f]* <picold_w>:
 *[0-9a-f]*:	e9 af 2f ff 	ldc\.w cp1,cr15,pc\[0x3fc\]
 *[0-9a-f]*:	e9 a0 20 ff 	ldc\.w cp1,cr0,r0\[0x3fc\]
 *[0-9a-f]*:	e9 a0 20 00 	ldc\.w cp1,cr0,r0\[0x0\]
 *[0-9a-f]*:	ef a8 27 40 	ldc\.w cp1,cr7,--r8
 *[0-9a-f]*:	ef a7 28 40 	ldc\.w cp1,cr8,--r7
 *[0-9a-f]*:	ef aa 31 25 	ldc\.w cp1,cr1,r10\[r5<<0x2\]
 *[0-9a-f]*:	ef a3 3d 06 	ldc\.w cp1,cr13,r3\[r6\]

[0-9a-f]* <picoldm_d>:
 *[0-9a-f]*:	ed af 24 ff 	ldcm\.d cp1,pc,cr0-cr15
 *[0-9a-f]*:	ed a0 24 01 	ldcm\.d cp1,r0,cr0-cr1
 *[0-9a-f]*:	ed a7 24 80 	ldcm\.d cp1,r7,cr14-cr15
 *[0-9a-f]*:	ed a8 24 7f 	ldcm\.d cp1,r8,cr0-cr13

[0-9a-f]* <picoldm_d_pu>:
 *[0-9a-f]*:	ed af 34 ff 	ldcm\.d cp1,pc\+\+,cr0-cr15
 *[0-9a-f]*:	ed a0 34 01 	ldcm\.d cp1,r0\+\+,cr0-cr1
 *[0-9a-f]*:	ed a7 34 80 	ldcm\.d cp1,r7\+\+,cr14-cr15
 *[0-9a-f]*:	ed a8 34 7f 	ldcm\.d cp1,r8\+\+,cr0-cr13

[0-9a-f]* <picoldm_w>:
 *[0-9a-f]*:	ed af 20 ff 	ldcm\.w cp1,pc,cr0-cr7
 *[0-9a-f]*:	ed a0 20 01 	ldcm\.w cp1,r0,cr0
 *[0-9a-f]*:	ed a7 20 80 	ldcm\.w cp1,r7,cr7
 *[0-9a-f]*:	ed a8 20 7f 	ldcm\.w cp1,r8,cr0-cr6
 *[0-9a-f]*:	ed af 21 ff 	ldcm\.w cp1,pc,cr8-cr15
 *[0-9a-f]*:	ed a0 21 01 	ldcm\.w cp1,r0,cr8
 *[0-9a-f]*:	ed a7 21 80 	ldcm\.w cp1,r7,cr15
 *[0-9a-f]*:	ed a8 21 7f 	ldcm\.w cp1,r8,cr8-cr14

[0-9a-f]* <picoldm_w_pu>:
 *[0-9a-f]*:	ed af 30 ff 	ldcm\.w cp1,pc\+\+,cr0-cr7
 *[0-9a-f]*:	ed a0 30 01 	ldcm\.w cp1,r0\+\+,cr0
 *[0-9a-f]*:	ed a7 30 80 	ldcm\.w cp1,r7\+\+,cr7
 *[0-9a-f]*:	ed a8 30 7f 	ldcm\.w cp1,r8\+\+,cr0-cr6
 *[0-9a-f]*:	ed af 31 ff 	ldcm\.w cp1,pc\+\+,cr8-cr15
 *[0-9a-f]*:	ed a0 31 01 	ldcm\.w cp1,r0\+\+,cr8
 *[0-9a-f]*:	ed a7 31 80 	ldcm\.w cp1,r7\+\+,cr15
 *[0-9a-f]*:	ed a8 31 7f 	ldcm\.w cp1,r8\+\+,cr8-cr14

[0-9a-f]* <picomv_d>:
 *[0-9a-f]*:	ef ae 2e 30 	mvrc\.d cp1,cr14,lr
 *[0-9a-f]*:	ef a0 20 30 	mvrc\.d cp1,cr0,r0
 *[0-9a-f]*:	ef a8 26 30 	mvrc\.d cp1,cr6,r8
 *[0-9a-f]*:	ef a6 28 30 	mvrc\.d cp1,cr8,r6
 *[0-9a-f]*:	ef ae 2e 10 	mvcr\.d cp1,lr,cr14
 *[0-9a-f]*:	ef a0 20 10 	mvcr\.d cp1,r0,cr0
 *[0-9a-f]*:	ef a8 26 10 	mvcr\.d cp1,r8,cr6
 *[0-9a-f]*:	ef a6 28 10 	mvcr\.d cp1,r6,cr8

[0-9a-f]* <picomv_w>:
 *[0-9a-f]*:	ef af 2f 20 	mvrc\.w cp1,cr15,pc
 *[0-9a-f]*:	ef a0 20 20 	mvrc\.w cp1,cr0,r0
 *[0-9a-f]*:	ef a8 27 20 	mvrc\.w cp1,cr7,r8
 *[0-9a-f]*:	ef a7 28 20 	mvrc\.w cp1,cr8,r7
 *[0-9a-f]*:	ef af 2f 00 	mvcr\.w cp1,pc,cr15
 *[0-9a-f]*:	ef a0 20 00 	mvcr\.w cp1,r0,cr0
 *[0-9a-f]*:	ef a8 27 00 	mvcr\.w cp1,r8,cr7
 *[0-9a-f]*:	ef a7 28 00 	mvcr\.w cp1,r7,cr8

[0-9a-f]* <picost_d>:
 *[0-9a-f]*:	eb af 3e ff 	stc\.d cp1,pc\[0x3fc\],cr14
 *[0-9a-f]*:	eb a0 30 00 	stc\.d cp1,r0\[0x0\],cr0
 *[0-9a-f]*:	ef a8 26 70 	stc\.d cp1,r8\+\+,cr6
 *[0-9a-f]*:	ef a7 28 70 	stc\.d cp1,r7\+\+,cr8
 *[0-9a-f]*:	ef aa 32 e5 	stc\.d cp1,r10\[r5<<0x2\],cr2
 *[0-9a-f]*:	ef a3 3c c6 	stc\.d cp1,r3\[r6\],cr12

[0-9a-f]* <picost_w>:
 *[0-9a-f]*:	eb af 2f ff 	stc\.w cp1,pc\[0x3fc\],cr15
 *[0-9a-f]*:	eb a0 20 00 	stc\.w cp1,r0\[0x0\],cr0
 *[0-9a-f]*:	ef a8 27 60 	stc\.w cp1,r8\+\+,cr7
 *[0-9a-f]*:	ef a7 28 60 	stc\.w cp1,r7\+\+,cr8
 *[0-9a-f]*:	ef aa 31 a5 	stc\.w cp1,r10\[r5<<0x2\],cr1
 *[0-9a-f]*:	ef a3 3d 86 	stc\.w cp1,r3\[r6\],cr13

[0-9a-f]* <picostm_d>:
 *[0-9a-f]*:	ed af 25 ff 	stcm\.d cp1,pc,cr0-cr15
 *[0-9a-f]*:	ed a0 25 01 	stcm\.d cp1,r0,cr0-cr1
 *[0-9a-f]*:	ed a7 25 80 	stcm\.d cp1,r7,cr14-cr15
 *[0-9a-f]*:	ed a8 25 7f 	stcm\.d cp1,r8,cr0-cr13

[0-9a-f]* <picostm_d_pu>:
 *[0-9a-f]*:	ed af 35 ff 	stcm\.d cp1,--pc,cr0-cr15
 *[0-9a-f]*:	ed a0 35 01 	stcm\.d cp1,--r0,cr0-cr1
 *[0-9a-f]*:	ed a7 35 80 	stcm\.d cp1,--r7,cr14-cr15
 *[0-9a-f]*:	ed a8 35 7f 	stcm\.d cp1,--r8,cr0-cr13

[0-9a-f]* <picostm_w>:
 *[0-9a-f]*:	ed af 22 ff 	stcm\.w cp1,pc,cr0-cr7
 *[0-9a-f]*:	ed a0 22 01 	stcm\.w cp1,r0,cr0
 *[0-9a-f]*:	ed a7 22 80 	stcm\.w cp1,r7,cr7
 *[0-9a-f]*:	ed a8 22 7f 	stcm\.w cp1,r8,cr0-cr6
 *[0-9a-f]*:	ed af 23 ff 	stcm\.w cp1,pc,cr8-cr15
 *[0-9a-f]*:	ed a0 23 01 	stcm\.w cp1,r0,cr8
 *[0-9a-f]*:	ed a7 23 80 	stcm\.w cp1,r7,cr15
 *[0-9a-f]*:	ed a8 23 7f 	stcm\.w cp1,r8,cr8-cr14

[0-9a-f]* <picostm_w_pu>:
 *[0-9a-f]*:	ed af 32 ff 	stcm\.w cp1,--pc,cr0-cr7
 *[0-9a-f]*:	ed a0 32 01 	stcm\.w cp1,--r0,cr0
 *[0-9a-f]*:	ed a7 32 80 	stcm\.w cp1,--r7,cr7
 *[0-9a-f]*:	ed a8 32 7f 	stcm\.w cp1,--r8,cr0-cr6
 *[0-9a-f]*:	ed af 33 ff 	stcm\.w cp1,--pc,cr8-cr15
 *[0-9a-f]*:	ed a0 33 01 	stcm\.w cp1,--r0,cr8
 *[0-9a-f]*:	ed a7 33 80 	stcm\.w cp1,--r7,cr15
 *[0-9a-f]*:	ed a8 33 7f 	stcm\.w cp1,--r8,cr8-cr14
