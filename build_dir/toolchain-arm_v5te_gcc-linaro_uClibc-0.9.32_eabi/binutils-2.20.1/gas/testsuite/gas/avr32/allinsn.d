#as:
#objdump: -dr
#name: allinsn

.*: +file format .*

Disassembly of section \.text:

[0-9a-f]* <ld_d5>:
 *[0-9a-f]*:	fe 0f 02 3e 	ld\.d lr,pc\[pc<<0x3\]
 *[0-9a-f]*:	e0 00 02 00 	ld\.d r0,r0\[r0\]
 *[0-9a-f]*:	ea 05 02 26 	ld\.d r6,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 02 14 	ld\.d r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 02 1e 	ld\.d lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	e6 0d 02 2a 	ld\.d r10,r3\[sp<<0x2\]
 *[0-9a-f]*:	f4 06 02 28 	ld\.d r8,r10\[r6<<0x2\]
 *[0-9a-f]*:	ee 09 02 02 	ld\.d r2,r7\[r9\]

[0-9a-f]* <ld_w5>:
 *[0-9a-f]*:	fe 0f 03 0f 	ld\.w pc,pc\[pc\]
 *[0-9a-f]*:	f8 0c 03 3c 	ld\.w r12,r12\[r12<<0x3\]
 *[0-9a-f]*:	ea 05 03 25 	ld\.w r5,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 03 14 	ld\.w r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 03 1e 	ld\.w lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	f2 09 03 02 	ld\.w r2,r9\[r9\]
 *[0-9a-f]*:	e4 06 03 0b 	ld\.w r11,r2\[r6\]
 *[0-9a-f]*:	e4 0d 03 30 	ld\.w r0,r2\[sp<<0x3\]

[0-9a-f]* <ld_sh5>:
 *[0-9a-f]*:	fe 0f 04 0f 	ld\.sh pc,pc\[pc\]
 *[0-9a-f]*:	f8 0c 04 3c 	ld\.sh r12,r12\[r12<<0x3\]
 *[0-9a-f]*:	ea 05 04 25 	ld\.sh r5,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 04 14 	ld\.sh r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 04 1e 	ld\.sh lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	e0 0f 04 2b 	ld\.sh r11,r0\[pc<<0x2\]
 *[0-9a-f]*:	fa 06 04 2a 	ld\.sh r10,sp\[r6<<0x2\]
 *[0-9a-f]*:	e4 02 04 0c 	ld\.sh r12,r2\[r2\]

[0-9a-f]* <ld_uh5>:
 *[0-9a-f]*:	fe 0f 05 0f 	ld\.uh pc,pc\[pc\]
 *[0-9a-f]*:	f8 0c 05 3c 	ld\.uh r12,r12\[r12<<0x3\]
 *[0-9a-f]*:	ea 05 05 25 	ld\.uh r5,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 05 14 	ld\.uh r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 05 1e 	ld\.uh lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	fe 0e 05 38 	ld\.uh r8,pc\[lr<<0x3\]
 *[0-9a-f]*:	e2 0f 05 16 	ld\.uh r6,r1\[pc<<0x1\]
 *[0-9a-f]*:	fc 0d 05 16 	ld\.uh r6,lr\[sp<<0x1\]

[0-9a-f]* <ld_sb2>:
 *[0-9a-f]*:	fe 0f 06 0f 	ld\.sb pc,pc\[pc\]
 *[0-9a-f]*:	f8 0c 06 3c 	ld\.sb r12,r12\[r12<<0x3\]
 *[0-9a-f]*:	ea 05 06 25 	ld\.sb r5,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 06 14 	ld\.sb r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 06 1e 	ld\.sb lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	e2 0f 06 39 	ld\.sb r9,r1\[pc<<0x3\]
 *[0-9a-f]*:	e6 0b 06 10 	ld\.sb r0,r3\[r11<<0x1\]
 *[0-9a-f]*:	ea 05 06 1a 	ld\.sb r10,r5\[r5<<0x1\]

[0-9a-f]* <ld_ub5>:
 *[0-9a-f]*:	fe 0f 07 0f 	ld\.ub pc,pc\[pc\]
 *[0-9a-f]*:	f8 0c 07 3c 	ld\.ub r12,r12\[r12<<0x3\]
 *[0-9a-f]*:	ea 05 07 25 	ld\.ub r5,r5\[r5<<0x2\]
 *[0-9a-f]*:	e8 04 07 14 	ld\.ub r4,r4\[r4<<0x1\]
 *[0-9a-f]*:	fc 0e 07 1e 	ld\.ub lr,lr\[lr<<0x1\]
 *[0-9a-f]*:	f8 07 07 36 	ld\.ub r6,r12\[r7<<0x3\]
 *[0-9a-f]*:	ec 0c 07 02 	ld\.ub r2,r6\[r12\]
 *[0-9a-f]*:	ee 0b 07 10 	ld\.ub r0,r7\[r11<<0x1\]

[0-9a-f]* <st_d5>:
 *[0-9a-f]*:	fe 0f 08 0e 	st\.d pc\[pc\],lr
 *[0-9a-f]*:	f8 0c 08 3c 	st\.d r12\[r12<<0x3\],r12
 *[0-9a-f]*:	ea 05 08 26 	st\.d r5\[r5<<0x2\],r6
 *[0-9a-f]*:	e8 04 08 14 	st\.d r4\[r4<<0x1\],r4
 *[0-9a-f]*:	fc 0e 08 1e 	st\.d lr\[lr<<0x1\],lr
 *[0-9a-f]*:	e2 09 08 14 	st\.d r1\[r9<<0x1\],r4
 *[0-9a-f]*:	f4 02 08 14 	st\.d r10\[r2<<0x1\],r4
 *[0-9a-f]*:	f8 06 08 0e 	st\.d r12\[r6\],lr

[0-9a-f]* <st_w5>:
 *[0-9a-f]*:	fe 0f 09 0f 	st\.w pc\[pc\],pc
 *[0-9a-f]*:	f8 0c 09 3c 	st\.w r12\[r12<<0x3\],r12
 *[0-9a-f]*:	ea 05 09 25 	st\.w r5\[r5<<0x2\],r5
 *[0-9a-f]*:	e8 04 09 14 	st\.w r4\[r4<<0x1\],r4
 *[0-9a-f]*:	fc 0e 09 1e 	st\.w lr\[lr<<0x1\],lr
 *[0-9a-f]*:	e2 0a 09 03 	st\.w r1\[r10\],r3
 *[0-9a-f]*:	e0 0a 09 19 	st\.w r0\[r10<<0x1\],r9
 *[0-9a-f]*:	e8 05 09 3f 	st\.w r4\[r5<<0x3\],pc

[0-9a-f]* <st_h5>:
 *[0-9a-f]*:	fe 0f 0a 0f 	st\.h pc\[pc\],pc
 *[0-9a-f]*:	f8 0c 0a 3c 	st\.h r12\[r12<<0x3\],r12
 *[0-9a-f]*:	ea 05 0a 25 	st\.h r5\[r5<<0x2\],r5
 *[0-9a-f]*:	e8 04 0a 14 	st\.h r4\[r4<<0x1\],r4
 *[0-9a-f]*:	fc 0e 0a 1e 	st\.h lr\[lr<<0x1\],lr
 *[0-9a-f]*:	e4 09 0a 0b 	st\.h r2\[r9\],r11
 *[0-9a-f]*:	ea 01 0a 2c 	st\.h r5\[r1<<0x2\],r12
 *[0-9a-f]*:	fe 08 0a 23 	st\.h pc\[r8<<0x2\],r3

[0-9a-f]* <st_b5>:
 *[0-9a-f]*:	fe 0f 0b 0f 	st\.b pc\[pc\],pc
 *[0-9a-f]*:	f8 0c 0b 3c 	st\.b r12\[r12<<0x3\],r12
 *[0-9a-f]*:	ea 05 0b 25 	st\.b r5\[r5<<0x2\],r5
 *[0-9a-f]*:	e8 04 0b 14 	st\.b r4\[r4<<0x1\],r4
 *[0-9a-f]*:	fc 0e 0b 1e 	st\.b lr\[lr<<0x1\],lr
 *[0-9a-f]*:	e2 08 0b 16 	st\.b r1\[r8<<0x1\],r6
 *[0-9a-f]*:	fc 0e 0b 31 	st\.b lr\[lr<<0x3\],r1
 *[0-9a-f]*:	ea 00 0b 2f 	st\.b r5\[r0<<0x2\],pc

[0-9a-f]* <divs>:
 *[0-9a-f]*:	fe 0f 0c 0f 	divs pc,pc,pc
 *[0-9a-f]*:	f8 0c 0c 0c 	divs r12,r12,r12
 *[0-9a-f]*:	ea 05 0c 05 	divs r5,r5,r5
 *[0-9a-f]*:	e8 04 0c 04 	divs r4,r4,r4
 *[0-9a-f]*:	fc 0e 0c 0e 	divs lr,lr,lr
 *[0-9a-f]*:	fe 0f 0c 03 	divs r3,pc,pc
 *[0-9a-f]*:	f8 02 0c 09 	divs r9,r12,r2
 *[0-9a-f]*:	e8 01 0c 07 	divs r7,r4,r1

[0-9a-f]* <add1>:
 *[0-9a-f]*:	1e 0f       	add pc,pc
 *[0-9a-f]*:	18 0c       	add r12,r12
 *[0-9a-f]*:	0a 05       	add r5,r5
 *[0-9a-f]*:	08 04       	add r4,r4
 *[0-9a-f]*:	1c 0e       	add lr,lr
 *[0-9a-f]*:	12 0c       	add r12,r9
 *[0-9a-f]*:	06 06       	add r6,r3
 *[0-9a-f]*:	18 0a       	add r10,r12

[0-9a-f]* <sub1>:
 *[0-9a-f]*:	1e 1f       	sub pc,pc
 *[0-9a-f]*:	18 1c       	sub r12,r12
 *[0-9a-f]*:	0a 15       	sub r5,r5
 *[0-9a-f]*:	08 14       	sub r4,r4
 *[0-9a-f]*:	1c 1e       	sub lr,lr
 *[0-9a-f]*:	0c 1e       	sub lr,r6
 *[0-9a-f]*:	1a 10       	sub r0,sp
 *[0-9a-f]*:	18 16       	sub r6,r12

[0-9a-f]* <rsub1>:
 *[0-9a-f]*:	1e 2f       	rsub pc,pc
 *[0-9a-f]*:	18 2c       	rsub r12,r12
 *[0-9a-f]*:	0a 25       	rsub r5,r5
 *[0-9a-f]*:	08 24       	rsub r4,r4
 *[0-9a-f]*:	1c 2e       	rsub lr,lr
 *[0-9a-f]*:	1a 2b       	rsub r11,sp
 *[0-9a-f]*:	08 27       	rsub r7,r4
 *[0-9a-f]*:	02 29       	rsub r9,r1

[0-9a-f]* <cp1>:
 *[0-9a-f]*:	1e 3f       	cp\.w pc,pc
 *[0-9a-f]*:	18 3c       	cp\.w r12,r12
 *[0-9a-f]*:	0a 35       	cp\.w r5,r5
 *[0-9a-f]*:	08 34       	cp\.w r4,r4
 *[0-9a-f]*:	1c 3e       	cp\.w lr,lr
 *[0-9a-f]*:	04 36       	cp\.w r6,r2
 *[0-9a-f]*:	12 30       	cp\.w r0,r9
 *[0-9a-f]*:	1a 33       	cp\.w r3,sp

[0-9a-f]* <or1>:
 *[0-9a-f]*:	1e 4f       	or pc,pc
 *[0-9a-f]*:	18 4c       	or r12,r12
 *[0-9a-f]*:	0a 45       	or r5,r5
 *[0-9a-f]*:	08 44       	or r4,r4
 *[0-9a-f]*:	1c 4e       	or lr,lr
 *[0-9a-f]*:	12 44       	or r4,r9
 *[0-9a-f]*:	08 4b       	or r11,r4
 *[0-9a-f]*:	00 44       	or r4,r0

[0-9a-f]* <eor1>:
 *[0-9a-f]*:	1e 5f       	eor pc,pc
 *[0-9a-f]*:	18 5c       	eor r12,r12
 *[0-9a-f]*:	0a 55       	eor r5,r5
 *[0-9a-f]*:	08 54       	eor r4,r4
 *[0-9a-f]*:	1c 5e       	eor lr,lr
 *[0-9a-f]*:	16 5c       	eor r12,r11
 *[0-9a-f]*:	02 50       	eor r0,r1
 *[0-9a-f]*:	1e 55       	eor r5,pc

[0-9a-f]* <and1>:
 *[0-9a-f]*:	1e 6f       	and pc,pc
 *[0-9a-f]*:	18 6c       	and r12,r12
 *[0-9a-f]*:	0a 65       	and r5,r5
 *[0-9a-f]*:	08 64       	and r4,r4
 *[0-9a-f]*:	1c 6e       	and lr,lr
 *[0-9a-f]*:	02 68       	and r8,r1
 *[0-9a-f]*:	1a 60       	and r0,sp
 *[0-9a-f]*:	0a 6a       	and r10,r5

[0-9a-f]* <tst>:
 *[0-9a-f]*:	1e 7f       	tst pc,pc
 *[0-9a-f]*:	18 7c       	tst r12,r12
 *[0-9a-f]*:	0a 75       	tst r5,r5
 *[0-9a-f]*:	08 74       	tst r4,r4
 *[0-9a-f]*:	1c 7e       	tst lr,lr
 *[0-9a-f]*:	18 70       	tst r0,r12
 *[0-9a-f]*:	0c 7a       	tst r10,r6
 *[0-9a-f]*:	08 7d       	tst sp,r4

[0-9a-f]* <andn>:
 *[0-9a-f]*:	1e 8f       	andn pc,pc
 *[0-9a-f]*:	18 8c       	andn r12,r12
 *[0-9a-f]*:	0a 85       	andn r5,r5
 *[0-9a-f]*:	08 84       	andn r4,r4
 *[0-9a-f]*:	1c 8e       	andn lr,lr
 *[0-9a-f]*:	18 89       	andn r9,r12
 *[0-9a-f]*:	1a 8b       	andn r11,sp
 *[0-9a-f]*:	0a 8c       	andn r12,r5

[0-9a-f]* <mov3>:
 *[0-9a-f]*:	1e 9f       	mov pc,pc
 *[0-9a-f]*:	18 9c       	mov r12,r12
 *[0-9a-f]*:	0a 95       	mov r5,r5
 *[0-9a-f]*:	08 94       	mov r4,r4
 *[0-9a-f]*:	1c 9e       	mov lr,lr
 *[0-9a-f]*:	12 95       	mov r5,r9
 *[0-9a-f]*:	16 9b       	mov r11,r11
 *[0-9a-f]*:	1c 92       	mov r2,lr

[0-9a-f]* <st_w1>:
 *[0-9a-f]*:	1e af       	st\.w pc\+\+,pc
 *[0-9a-f]*:	18 ac       	st\.w r12\+\+,r12
 *[0-9a-f]*:	0a a5       	st\.w r5\+\+,r5
 *[0-9a-f]*:	08 a4       	st\.w r4\+\+,r4
 *[0-9a-f]*:	1c ae       	st\.w lr\+\+,lr
 *[0-9a-f]*:	02 ab       	st\.w r1\+\+,r11
 *[0-9a-f]*:	1a a0       	st\.w sp\+\+,r0
 *[0-9a-f]*:	1a a1       	st\.w sp\+\+,r1

[0-9a-f]* <st_h1>:
 *[0-9a-f]*:	1e bf       	st\.h pc\+\+,pc
 *[0-9a-f]*:	18 bc       	st\.h r12\+\+,r12
 *[0-9a-f]*:	0a b5       	st\.h r5\+\+,r5
 *[0-9a-f]*:	08 b4       	st\.h r4\+\+,r4
 *[0-9a-f]*:	1c be       	st\.h lr\+\+,lr
 *[0-9a-f]*:	18 bd       	st\.h r12\+\+,sp
 *[0-9a-f]*:	0e be       	st\.h r7\+\+,lr
 *[0-9a-f]*:	0e b4       	st\.h r7\+\+,r4

[0-9a-f]* <st_b1>:
 *[0-9a-f]*:	1e cf       	st\.b pc\+\+,pc
 *[0-9a-f]*:	18 cc       	st\.b r12\+\+,r12
 *[0-9a-f]*:	0a c5       	st\.b r5\+\+,r5
 *[0-9a-f]*:	08 c4       	st\.b r4\+\+,r4
 *[0-9a-f]*:	1c ce       	st\.b lr\+\+,lr
 *[0-9a-f]*:	12 cd       	st\.b r9\+\+,sp
 *[0-9a-f]*:	02 cd       	st\.b r1\+\+,sp
 *[0-9a-f]*:	00 c4       	st\.b r0\+\+,r4

[0-9a-f]* <st_w2>:
 *[0-9a-f]*:	1e df       	st\.w --pc,pc
 *[0-9a-f]*:	18 dc       	st\.w --r12,r12
 *[0-9a-f]*:	0a d5       	st\.w --r5,r5
 *[0-9a-f]*:	08 d4       	st\.w --r4,r4
 *[0-9a-f]*:	1c de       	st\.w --lr,lr
 *[0-9a-f]*:	02 d7       	st\.w --r1,r7
 *[0-9a-f]*:	06 d9       	st\.w --r3,r9
 *[0-9a-f]*:	0a d5       	st\.w --r5,r5

[0-9a-f]* <st_h2>:
 *[0-9a-f]*:	1e ef       	st\.h --pc,pc
 *[0-9a-f]*:	18 ec       	st\.h --r12,r12
 *[0-9a-f]*:	0a e5       	st\.h --r5,r5
 *[0-9a-f]*:	08 e4       	st\.h --r4,r4
 *[0-9a-f]*:	1c ee       	st\.h --lr,lr
 *[0-9a-f]*:	0a e7       	st\.h --r5,r7
 *[0-9a-f]*:	10 e8       	st\.h --r8,r8
 *[0-9a-f]*:	0e e2       	st\.h --r7,r2

[0-9a-f]* <st_b2>:
 *[0-9a-f]*:	1e ff       	st\.b --pc,pc
 *[0-9a-f]*:	18 fc       	st\.b --r12,r12
 *[0-9a-f]*:	0a f5       	st\.b --r5,r5
 *[0-9a-f]*:	08 f4       	st\.b --r4,r4
 *[0-9a-f]*:	1c fe       	st\.b --lr,lr
 *[0-9a-f]*:	1a fd       	st\.b --sp,sp
 *[0-9a-f]*:	1a fb       	st\.b --sp,r11
 *[0-9a-f]*:	08 f5       	st\.b --r4,r5

[0-9a-f]* <ld_w1>:
 *[0-9a-f]*:	1f 0f       	ld\.w pc,pc\+\+
 *[0-9a-f]*:	19 0c       	ld\.w r12,r12\+\+
 *[0-9a-f]*:	0b 05       	ld\.w r5,r5\+\+
 *[0-9a-f]*:	09 04       	ld\.w r4,r4\+\+
 *[0-9a-f]*:	1d 0e       	ld\.w lr,lr\+\+
 *[0-9a-f]*:	0f 03       	ld\.w r3,r7\+\+
 *[0-9a-f]*:	1d 03       	ld\.w r3,lr\+\+
 *[0-9a-f]*:	0b 0c       	ld\.w r12,r5\+\+

[0-9a-f]* <ld_sh1>:
 *[0-9a-f]*:	1f 1f       	ld\.sh pc,pc\+\+
 *[0-9a-f]*:	19 1c       	ld\.sh r12,r12\+\+
 *[0-9a-f]*:	0b 15       	ld\.sh r5,r5\+\+
 *[0-9a-f]*:	09 14       	ld\.sh r4,r4\+\+
 *[0-9a-f]*:	1d 1e       	ld\.sh lr,lr\+\+
 *[0-9a-f]*:	05 1b       	ld\.sh r11,r2\+\+
 *[0-9a-f]*:	11 12       	ld\.sh r2,r8\+\+
 *[0-9a-f]*:	0d 17       	ld\.sh r7,r6\+\+

[0-9a-f]* <ld_uh1>:
 *[0-9a-f]*:	1f 2f       	ld\.uh pc,pc\+\+
 *[0-9a-f]*:	19 2c       	ld\.uh r12,r12\+\+
 *[0-9a-f]*:	0b 25       	ld\.uh r5,r5\+\+
 *[0-9a-f]*:	09 24       	ld\.uh r4,r4\+\+
 *[0-9a-f]*:	1d 2e       	ld\.uh lr,lr\+\+
 *[0-9a-f]*:	0f 26       	ld\.uh r6,r7\+\+
 *[0-9a-f]*:	17 2a       	ld\.uh r10,r11\+\+
 *[0-9a-f]*:	09 2e       	ld\.uh lr,r4\+\+

[0-9a-f]* <ld_ub1>:
 *[0-9a-f]*:	1f 3f       	ld\.ub pc,pc\+\+
 *[0-9a-f]*:	19 3c       	ld\.ub r12,r12\+\+
 *[0-9a-f]*:	0b 35       	ld\.ub r5,r5\+\+
 *[0-9a-f]*:	09 34       	ld\.ub r4,r4\+\+
 *[0-9a-f]*:	1d 3e       	ld\.ub lr,lr\+\+
 *[0-9a-f]*:	1d 38       	ld\.ub r8,lr\+\+
 *[0-9a-f]*:	19 3c       	ld\.ub r12,r12\+\+
 *[0-9a-f]*:	15 3b       	ld\.ub r11,r10\+\+

[0-9a-f]* <ld_w2>:
 *[0-9a-f]*:	1f 4f       	ld\.w pc,--pc
 *[0-9a-f]*:	19 4c       	ld\.w r12,--r12
 *[0-9a-f]*:	0b 45       	ld\.w r5,--r5
 *[0-9a-f]*:	09 44       	ld\.w r4,--r4
 *[0-9a-f]*:	1d 4e       	ld\.w lr,--lr
 *[0-9a-f]*:	1d 4a       	ld\.w r10,--lr
 *[0-9a-f]*:	13 4c       	ld\.w r12,--r9
 *[0-9a-f]*:	0b 46       	ld\.w r6,--r5

[0-9a-f]* <ld_sh2>:
 *[0-9a-f]*:	1f 5f       	ld\.sh pc,--pc
 *[0-9a-f]*:	19 5c       	ld\.sh r12,--r12
 *[0-9a-f]*:	0b 55       	ld\.sh r5,--r5
 *[0-9a-f]*:	09 54       	ld\.sh r4,--r4
 *[0-9a-f]*:	1d 5e       	ld\.sh lr,--lr
 *[0-9a-f]*:	15 5f       	ld\.sh pc,--r10
 *[0-9a-f]*:	07 56       	ld\.sh r6,--r3
 *[0-9a-f]*:	0d 54       	ld\.sh r4,--r6

[0-9a-f]* <ld_uh2>:
 *[0-9a-f]*:	1f 6f       	ld\.uh pc,--pc
 *[0-9a-f]*:	19 6c       	ld\.uh r12,--r12
 *[0-9a-f]*:	0b 65       	ld\.uh r5,--r5
 *[0-9a-f]*:	09 64       	ld\.uh r4,--r4
 *[0-9a-f]*:	1d 6e       	ld\.uh lr,--lr
 *[0-9a-f]*:	05 63       	ld\.uh r3,--r2
 *[0-9a-f]*:	01 61       	ld\.uh r1,--r0
 *[0-9a-f]*:	13 62       	ld\.uh r2,--r9

[0-9a-f]* <ld_ub2>:
 *[0-9a-f]*:	1f 7f       	ld\.ub pc,--pc
 *[0-9a-f]*:	19 7c       	ld\.ub r12,--r12
 *[0-9a-f]*:	0b 75       	ld\.ub r5,--r5
 *[0-9a-f]*:	09 74       	ld\.ub r4,--r4
 *[0-9a-f]*:	1d 7e       	ld\.ub lr,--lr
 *[0-9a-f]*:	03 71       	ld\.ub r1,--r1
 *[0-9a-f]*:	0d 70       	ld\.ub r0,--r6
 *[0-9a-f]*:	0f 72       	ld\.ub r2,--r7

[0-9a-f]* <ld_ub3>:
 *[0-9a-f]*:	1f 8f       	ld\.ub pc,pc\[0x0\]
 *[0-9a-f]*:	19 fc       	ld\.ub r12,r12\[0x7\]
 *[0-9a-f]*:	0b c5       	ld\.ub r5,r5\[0x4\]
 *[0-9a-f]*:	09 b4       	ld\.ub r4,r4\[0x3\]
 *[0-9a-f]*:	1d 9e       	ld\.ub lr,lr\[0x1\]
 *[0-9a-f]*:	13 e6       	ld\.ub r6,r9\[0x6\]
 *[0-9a-f]*:	1d c2       	ld\.ub r2,lr\[0x4\]
 *[0-9a-f]*:	11 81       	ld\.ub r1,r8\[0x0\]

[0-9a-f]* <sub3_sp>:
 *[0-9a-f]*:	20 0d       	sub sp,0
 *[0-9a-f]*:	2f fd       	sub sp,-4
 *[0-9a-f]*:	28 0d       	sub sp,-512
 *[0-9a-f]*:	27 fd       	sub sp,508
 *[0-9a-f]*:	20 1d       	sub sp,4
 *[0-9a-f]*:	20 bd       	sub sp,44
 *[0-9a-f]*:	20 2d       	sub sp,8
 *[0-9a-f]*:	25 7d       	sub sp,348

[0-9a-f]* <sub3>:
 *[0-9a-f]*:	20 0f       	sub pc,0
 *[0-9a-f]*:	2f fc       	sub r12,-1
 *[0-9a-f]*:	28 05       	sub r5,-128
 *[0-9a-f]*:	27 f4       	sub r4,127
 *[0-9a-f]*:	20 1e       	sub lr,1
 *[0-9a-f]*:	2d 76       	sub r6,-41
 *[0-9a-f]*:	22 54       	sub r4,37
 *[0-9a-f]*:	23 8c       	sub r12,56

[0-9a-f]* <mov1>:
 *[0-9a-f]*:	30 0f       	mov pc,0
 *[0-9a-f]*:	3f fc       	mov r12,-1
 *[0-9a-f]*:	38 05       	mov r5,-128
 *[0-9a-f]*:	37 f4       	mov r4,127
 *[0-9a-f]*:	30 1e       	mov lr,1
 *[0-9a-f]*:	30 ef       	mov pc,14
 *[0-9a-f]*:	39 c6       	mov r6,-100
 *[0-9a-f]*:	38 6e       	mov lr,-122

[0-9a-f]* <lddsp>:
 *[0-9a-f]*:	40 0f       	lddsp pc,sp\[0x0\]
 *[0-9a-f]*:	47 fc       	lddsp r12,sp\[0x1fc\]
 *[0-9a-f]*:	44 05       	lddsp r5,sp\[0x100\]
 *[0-9a-f]*:	43 f4       	lddsp r4,sp\[0xfc\]
 *[0-9a-f]*:	40 1e       	lddsp lr,sp\[0x4\]
 *[0-9a-f]*:	44 0e       	lddsp lr,sp\[0x100\]
 *[0-9a-f]*:	40 5c       	lddsp r12,sp\[0x14\]
 *[0-9a-f]*:	47 69       	lddsp r9,sp\[0x1d8\]

[0-9a-f]* <lddpc>:
 *[0-9a-f]*:	48 0f       	lddpc pc,[0-9a-f]* <.*>
 *[0-9a-f]*:	4f f0       	lddpc r0,[0-9a-f]* <.*>
 *[0-9a-f]*:	4c 08       	lddpc r8,[0-9a-f]* <.*>
 *[0-9a-f]*:	4b f7       	lddpc r7,[0-9a-f]* <.*>
 *[0-9a-f]*:	48 1e       	lddpc lr,[0-9a-f]* <.*>
 *[0-9a-f]*:	4f 6d       	lddpc sp,[0-9a-f]* <.*>
 *[0-9a-f]*:	49 e6       	lddpc r6,[0-9a-f]* <.*>
 *[0-9a-f]*:	48 7b       	lddpc r11,[0-9a-f]* <.*>

[0-9a-f]* <stdsp>:
 *[0-9a-f]*:	50 0f       	stdsp sp\[0x0\],pc
 *[0-9a-f]*:	57 fc       	stdsp sp\[0x1fc\],r12
 *[0-9a-f]*:	54 05       	stdsp sp\[0x100\],r5
 *[0-9a-f]*:	53 f4       	stdsp sp\[0xfc\],r4
 *[0-9a-f]*:	50 1e       	stdsp sp\[0x4\],lr
 *[0-9a-f]*:	54 cf       	stdsp sp\[0x130\],pc
 *[0-9a-f]*:	54 00       	stdsp sp\[0x100\],r0
 *[0-9a-f]*:	55 45       	stdsp sp\[0x150\],r5

[0-9a-f]* <cp2>:
 *[0-9a-f]*:	58 0f       	cp.w pc,0
 *[0-9a-f]*:	5b fc       	cp.w r12,-1
 *[0-9a-f]*:	5a 05       	cp.w r5,-32
 *[0-9a-f]*:	59 f4       	cp.w r4,31
 *[0-9a-f]*:	58 1e       	cp.w lr,1
 *[0-9a-f]*:	58 38       	cp.w r8,3
 *[0-9a-f]*:	59 0e       	cp.w lr,16
 *[0-9a-f]*:	5a 67       	cp.w r7,-26

[0-9a-f]* <acr>:
 *[0-9a-f]*:	5c 0f       	acr pc
 *[0-9a-f]*:	5c 0c       	acr r12
 *[0-9a-f]*:	5c 05       	acr r5
 *[0-9a-f]*:	5c 04       	acr r4
 *[0-9a-f]*:	5c 0e       	acr lr
 *[0-9a-f]*:	5c 02       	acr r2
 *[0-9a-f]*:	5c 0c       	acr r12
 *[0-9a-f]*:	5c 0f       	acr pc

[0-9a-f]* <scr>:
 *[0-9a-f]*:	5c 1f       	scr pc
 *[0-9a-f]*:	5c 1c       	scr r12
 *[0-9a-f]*:	5c 15       	scr r5
 *[0-9a-f]*:	5c 14       	scr r4
 *[0-9a-f]*:	5c 1e       	scr lr
 *[0-9a-f]*:	5c 1f       	scr pc
 *[0-9a-f]*:	5c 16       	scr r6
 *[0-9a-f]*:	5c 11       	scr r1

[0-9a-f]* <cpc0>:
 *[0-9a-f]*:	5c 2f       	cpc pc
 *[0-9a-f]*:	5c 2c       	cpc r12
 *[0-9a-f]*:	5c 25       	cpc r5
 *[0-9a-f]*:	5c 24       	cpc r4
 *[0-9a-f]*:	5c 2e       	cpc lr
 *[0-9a-f]*:	5c 2f       	cpc pc
 *[0-9a-f]*:	5c 24       	cpc r4
 *[0-9a-f]*:	5c 29       	cpc r9

[0-9a-f]* <neg>:
 *[0-9a-f]*:	5c 3f       	neg pc
 *[0-9a-f]*:	5c 3c       	neg r12
 *[0-9a-f]*:	5c 35       	neg r5
 *[0-9a-f]*:	5c 34       	neg r4
 *[0-9a-f]*:	5c 3e       	neg lr
 *[0-9a-f]*:	5c 37       	neg r7
 *[0-9a-f]*:	5c 31       	neg r1
 *[0-9a-f]*:	5c 39       	neg r9

[0-9a-f]* <abs>:
 *[0-9a-f]*:	5c 4f       	abs pc
 *[0-9a-f]*:	5c 4c       	abs r12
 *[0-9a-f]*:	5c 45       	abs r5
 *[0-9a-f]*:	5c 44       	abs r4
 *[0-9a-f]*:	5c 4e       	abs lr
 *[0-9a-f]*:	5c 46       	abs r6
 *[0-9a-f]*:	5c 46       	abs r6
 *[0-9a-f]*:	5c 44       	abs r4

[0-9a-f]* <castu_b>:
 *[0-9a-f]*:	5c 5f       	castu\.b pc
 *[0-9a-f]*:	5c 5c       	castu\.b r12
 *[0-9a-f]*:	5c 55       	castu\.b r5
 *[0-9a-f]*:	5c 54       	castu\.b r4
 *[0-9a-f]*:	5c 5e       	castu\.b lr
 *[0-9a-f]*:	5c 57       	castu\.b r7
 *[0-9a-f]*:	5c 5d       	castu\.b sp
 *[0-9a-f]*:	5c 59       	castu\.b r9

[0-9a-f]* <casts_b>:
 *[0-9a-f]*:	5c 6f       	casts\.b pc
 *[0-9a-f]*:	5c 6c       	casts\.b r12
 *[0-9a-f]*:	5c 65       	casts\.b r5
 *[0-9a-f]*:	5c 64       	casts\.b r4
 *[0-9a-f]*:	5c 6e       	casts\.b lr
 *[0-9a-f]*:	5c 6b       	casts\.b r11
 *[0-9a-f]*:	5c 61       	casts\.b r1
 *[0-9a-f]*:	5c 6a       	casts\.b r10

[0-9a-f]* <castu_h>:
 *[0-9a-f]*:	5c 7f       	castu\.h pc
 *[0-9a-f]*:	5c 7c       	castu\.h r12
 *[0-9a-f]*:	5c 75       	castu\.h r5
 *[0-9a-f]*:	5c 74       	castu\.h r4
 *[0-9a-f]*:	5c 7e       	castu\.h lr
 *[0-9a-f]*:	5c 7a       	castu\.h r10
 *[0-9a-f]*:	5c 7b       	castu\.h r11
 *[0-9a-f]*:	5c 71       	castu\.h r1

[0-9a-f]* <casts_h>:
 *[0-9a-f]*:	5c 8f       	casts\.h pc
 *[0-9a-f]*:	5c 8c       	casts\.h r12
 *[0-9a-f]*:	5c 85       	casts\.h r5
 *[0-9a-f]*:	5c 84       	casts\.h r4
 *[0-9a-f]*:	5c 8e       	casts\.h lr
 *[0-9a-f]*:	5c 80       	casts\.h r0
 *[0-9a-f]*:	5c 85       	casts\.h r5
 *[0-9a-f]*:	5c 89       	casts\.h r9

[0-9a-f]* <brev>:
 *[0-9a-f]*:	5c 9f       	brev pc
 *[0-9a-f]*:	5c 9c       	brev r12
 *[0-9a-f]*:	5c 95       	brev r5
 *[0-9a-f]*:	5c 94       	brev r4
 *[0-9a-f]*:	5c 9e       	brev lr
 *[0-9a-f]*:	5c 95       	brev r5
 *[0-9a-f]*:	5c 9a       	brev r10
 *[0-9a-f]*:	5c 98       	brev r8

[0-9a-f]* <swap_h>:
 *[0-9a-f]*:	5c af       	swap\.h pc
 *[0-9a-f]*:	5c ac       	swap\.h r12
 *[0-9a-f]*:	5c a5       	swap\.h r5
 *[0-9a-f]*:	5c a4       	swap\.h r4
 *[0-9a-f]*:	5c ae       	swap\.h lr
 *[0-9a-f]*:	5c a7       	swap\.h r7
 *[0-9a-f]*:	5c a0       	swap\.h r0
 *[0-9a-f]*:	5c a8       	swap\.h r8

[0-9a-f]* <swap_b>:
 *[0-9a-f]*:	5c bf       	swap\.b pc
 *[0-9a-f]*:	5c bc       	swap\.b r12
 *[0-9a-f]*:	5c b5       	swap\.b r5
 *[0-9a-f]*:	5c b4       	swap\.b r4
 *[0-9a-f]*:	5c be       	swap\.b lr
 *[0-9a-f]*:	5c ba       	swap\.b r10
 *[0-9a-f]*:	5c bc       	swap\.b r12
 *[0-9a-f]*:	5c b1       	swap\.b r1

[0-9a-f]* <swap_bh>:
 *[0-9a-f]*:	5c cf       	swap\.bh pc
 *[0-9a-f]*:	5c cc       	swap\.bh r12
 *[0-9a-f]*:	5c c5       	swap\.bh r5
 *[0-9a-f]*:	5c c4       	swap\.bh r4
 *[0-9a-f]*:	5c ce       	swap\.bh lr
 *[0-9a-f]*:	5c c9       	swap\.bh r9
 *[0-9a-f]*:	5c c4       	swap\.bh r4
 *[0-9a-f]*:	5c c1       	swap\.bh r1

[0-9a-f]* <One_s_compliment>:
 *[0-9a-f]*:	5c df       	com pc
 *[0-9a-f]*:	5c dc       	com r12
 *[0-9a-f]*:	5c d5       	com r5
 *[0-9a-f]*:	5c d4       	com r4
 *[0-9a-f]*:	5c de       	com lr
 *[0-9a-f]*:	5c d2       	com r2
 *[0-9a-f]*:	5c d2       	com r2
 *[0-9a-f]*:	5c d7       	com r7

[0-9a-f]* <tnbz>:
 *[0-9a-f]*:	5c ef       	tnbz pc
 *[0-9a-f]*:	5c ec       	tnbz r12
 *[0-9a-f]*:	5c e5       	tnbz r5
 *[0-9a-f]*:	5c e4       	tnbz r4
 *[0-9a-f]*:	5c ee       	tnbz lr
 *[0-9a-f]*:	5c e8       	tnbz r8
 *[0-9a-f]*:	5c ec       	tnbz r12
 *[0-9a-f]*:	5c ef       	tnbz pc

[0-9a-f]* <rol>:
 *[0-9a-f]*:	5c ff       	rol pc
 *[0-9a-f]*:	5c fc       	rol r12
 *[0-9a-f]*:	5c f5       	rol r5
 *[0-9a-f]*:	5c f4       	rol r4
 *[0-9a-f]*:	5c fe       	rol lr
 *[0-9a-f]*:	5c fa       	rol r10
 *[0-9a-f]*:	5c f9       	rol r9
 *[0-9a-f]*:	5c f5       	rol r5

[0-9a-f]* <ror>:
 *[0-9a-f]*:	5d 0f       	ror pc
 *[0-9a-f]*:	5d 0c       	ror r12
 *[0-9a-f]*:	5d 05       	ror r5
 *[0-9a-f]*:	5d 04       	ror r4
 *[0-9a-f]*:	5d 0e       	ror lr
 *[0-9a-f]*:	5d 08       	ror r8
 *[0-9a-f]*:	5d 04       	ror r4
 *[0-9a-f]*:	5d 07       	ror r7

[0-9a-f]* <icall>:
 *[0-9a-f]*:	5d 1f       	icall pc
 *[0-9a-f]*:	5d 1c       	icall r12
 *[0-9a-f]*:	5d 15       	icall r5
 *[0-9a-f]*:	5d 14       	icall r4
 *[0-9a-f]*:	5d 1e       	icall lr
 *[0-9a-f]*:	5d 13       	icall r3
 *[0-9a-f]*:	5d 11       	icall r1
 *[0-9a-f]*:	5d 13       	icall r3

[0-9a-f]* <mustr>:
 *[0-9a-f]*:	5d 2f       	mustr pc
 *[0-9a-f]*:	5d 2c       	mustr r12
 *[0-9a-f]*:	5d 25       	mustr r5
 *[0-9a-f]*:	5d 24       	mustr r4
 *[0-9a-f]*:	5d 2e       	mustr lr
 *[0-9a-f]*:	5d 21       	mustr r1
 *[0-9a-f]*:	5d 24       	mustr r4
 *[0-9a-f]*:	5d 2c       	mustr r12

[0-9a-f]* <musfr>:
 *[0-9a-f]*:	5d 3f       	musfr pc
 *[0-9a-f]*:	5d 3c       	musfr r12
 *[0-9a-f]*:	5d 35       	musfr r5
 *[0-9a-f]*:	5d 34       	musfr r4
 *[0-9a-f]*:	5d 3e       	musfr lr
 *[0-9a-f]*:	5d 3b       	musfr r11
 *[0-9a-f]*:	5d 3c       	musfr r12
 *[0-9a-f]*:	5d 32       	musfr r2

[0-9a-f]* <ret_cond>:
 *[0-9a-f]*:	5e 0f       	reteq 1
 *[0-9a-f]*:	5e fc       	retal r12
 *[0-9a-f]*:	5e 85       	retls r5
 *[0-9a-f]*:	5e 74       	retpl r4
 *[0-9a-f]*:	5e 1e       	retne -1
 *[0-9a-f]*:	5e 90       	retgt r0
 *[0-9a-f]*:	5e 9c       	retgt r12
 *[0-9a-f]*:	5e 4a       	retge r10

[0-9a-f]* <sr_cond>:
 *[0-9a-f]*:	5f 0f       	sreq pc
 *[0-9a-f]*:	5f fc       	sral r12
 *[0-9a-f]*:	5f 85       	srls r5
 *[0-9a-f]*:	5f 74       	srpl r4
 *[0-9a-f]*:	5f 1e       	srne lr
 *[0-9a-f]*:	5f 50       	srlt r0
 *[0-9a-f]*:	5f fd       	sral sp
 *[0-9a-f]*:	5f 49       	srge r9

[0-9a-f]* <ld_w3>:
 *[0-9a-f]*:	7e 0f       	ld\.w pc,pc\[0x0\]
 *[0-9a-f]*:	79 fc       	ld\.w r12,r12\[0x7c\]
 *[0-9a-f]*:	6b 05       	ld\.w r5,r5\[0x40\]
 *[0-9a-f]*:	68 f4       	ld\.w r4,r4\[0x3c\]
 *[0-9a-f]*:	7c 1e       	ld\.w lr,lr\[0x4\]
 *[0-9a-f]*:	64 dd       	ld\.w sp,r2\[0x34\]
 *[0-9a-f]*:	62 29       	ld\.w r9,r1\[0x8\]
 *[0-9a-f]*:	7a f5       	ld\.w r5,sp\[0x3c\]

[0-9a-f]* <ld_sh3>:
 *[0-9a-f]*:	9e 0f       	ld\.sh pc,pc\[0x0\]
 *[0-9a-f]*:	98 7c       	ld\.sh r12,r12\[0xe\]
 *[0-9a-f]*:	8a 45       	ld\.sh r5,r5\[0x8\]
 *[0-9a-f]*:	88 34       	ld\.sh r4,r4\[0x6\]
 *[0-9a-f]*:	9c 1e       	ld\.sh lr,lr\[0x2\]
 *[0-9a-f]*:	84 44       	ld\.sh r4,r2\[0x8\]
 *[0-9a-f]*:	9c 5d       	ld\.sh sp,lr\[0xa\]
 *[0-9a-f]*:	96 12       	ld\.sh r2,r11\[0x2\]

[0-9a-f]* <ld_uh3>:
 *[0-9a-f]*:	9e 8f       	ld\.uh pc,pc\[0x0\]
 *[0-9a-f]*:	98 fc       	ld\.uh r12,r12\[0xe\]
 *[0-9a-f]*:	8a c5       	ld\.uh r5,r5\[0x8\]
 *[0-9a-f]*:	88 b4       	ld\.uh r4,r4\[0x6\]
 *[0-9a-f]*:	9c 9e       	ld\.uh lr,lr\[0x2\]
 *[0-9a-f]*:	80 da       	ld\.uh r10,r0\[0xa\]
 *[0-9a-f]*:	96 c8       	ld\.uh r8,r11\[0x8\]
 *[0-9a-f]*:	84 ea       	ld\.uh r10,r2\[0xc\]

[0-9a-f]* <st_w3>:
 *[0-9a-f]*:	9f 0f       	st\.w pc\[0x0\],pc
 *[0-9a-f]*:	99 fc       	st\.w r12\[0x3c\],r12
 *[0-9a-f]*:	8b 85       	st\.w r5\[0x20\],r5
 *[0-9a-f]*:	89 74       	st\.w r4\[0x1c\],r4
 *[0-9a-f]*:	9d 1e       	st\.w lr\[0x4\],lr
 *[0-9a-f]*:	8f bb       	st\.w r7\[0x2c\],r11
 *[0-9a-f]*:	85 66       	st\.w r2\[0x18\],r6
 *[0-9a-f]*:	89 39       	st\.w r4\[0xc\],r9

[0-9a-f]* <st_h3>:
 *[0-9a-f]*:	be 0f       	st\.h pc\[0x0\],pc
 *[0-9a-f]*:	b8 7c       	st\.h r12\[0xe\],r12
 *[0-9a-f]*:	aa 45       	st\.h r5\[0x8\],r5
 *[0-9a-f]*:	a8 34       	st\.h r4\[0x6\],r4
 *[0-9a-f]*:	bc 1e       	st\.h lr\[0x2\],lr
 *[0-9a-f]*:	bc 5c       	st\.h lr\[0xa\],r12
 *[0-9a-f]*:	ac 20       	st\.h r6\[0x4\],r0
 *[0-9a-f]*:	aa 6d       	st\.h r5\[0xc\],sp

[0-9a-f]* <st_b3>:
 *[0-9a-f]*:	be 8f       	st\.b pc\[0x0\],pc
 *[0-9a-f]*:	b8 fc       	st\.b r12\[0x7\],r12
 *[0-9a-f]*:	aa c5       	st\.b r5\[0x4\],r5
 *[0-9a-f]*:	a8 b4       	st\.b r4\[0x3\],r4
 *[0-9a-f]*:	bc 9e       	st\.b lr\[0x1\],lr
 *[0-9a-f]*:	b8 e9       	st\.b r12\[0x6\],r9
 *[0-9a-f]*:	a4 be       	st\.b r2\[0x3\],lr
 *[0-9a-f]*:	a2 bb       	st\.b r1\[0x3\],r11

[0-9a-f]* <ldd>:
 *[0-9a-f]*:	bf 00       	ld\.d r0,pc
 *[0-9a-f]*:	b9 0e       	ld\.d lr,r12
 *[0-9a-f]*:	ab 08       	ld\.d r8,r5
 *[0-9a-f]*:	a9 06       	ld\.d r6,r4
 *[0-9a-f]*:	bd 02       	ld\.d r2,lr
 *[0-9a-f]*:	af 0e       	ld\.d lr,r7
 *[0-9a-f]*:	a9 04       	ld\.d r4,r4
 *[0-9a-f]*:	bf 0e       	ld\.d lr,pc

[0-9a-f]* <ldd_postinc>:
 *[0-9a-f]*:	bf 01       	ld\.d r0,pc\+\+
 *[0-9a-f]*:	b9 0f       	ld\.d lr,r12\+\+
 *[0-9a-f]*:	ab 09       	ld\.d r8,r5\+\+
 *[0-9a-f]*:	a9 07       	ld\.d r6,r4\+\+
 *[0-9a-f]*:	bd 03       	ld\.d r2,lr\+\+
 *[0-9a-f]*:	ab 0f       	ld\.d lr,r5\+\+
 *[0-9a-f]*:	b7 0d       	ld\.d r12,r11\+\+
 *[0-9a-f]*:	b9 03       	ld\.d r2,r12\+\+

[0-9a-f]* <ldd_predec>:
 *[0-9a-f]*:	bf 10       	ld\.d r0,--pc
 *[0-9a-f]*:	b9 1e       	ld\.d lr,--r12
 *[0-9a-f]*:	ab 18       	ld\.d r8,--r5
 *[0-9a-f]*:	a9 16       	ld\.d r6,--r4
 *[0-9a-f]*:	bd 12       	ld\.d r2,--lr
 *[0-9a-f]*:	a1 18       	ld\.d r8,--r0
 *[0-9a-f]*:	bf 1a       	ld\.d r10,--pc
 *[0-9a-f]*:	a9 12       	ld\.d r2,--r4

[0-9a-f]* <std>:
 *[0-9a-f]*:	bf 11       	st\.d pc,r0
 *[0-9a-f]*:	b9 1f       	st\.d r12,lr
 *[0-9a-f]*:	ab 19       	st\.d r5,r8
 *[0-9a-f]*:	a9 17       	st\.d r4,r6
 *[0-9a-f]*:	bd 13       	st\.d lr,r2
 *[0-9a-f]*:	a1 1d       	st\.d r0,r12
 *[0-9a-f]*:	bb 15       	st\.d sp,r4
 *[0-9a-f]*:	b9 1d       	st\.d r12,r12

[0-9a-f]* <std_postinc>:
 *[0-9a-f]*:	bf 20       	st\.d pc\+\+,r0
 *[0-9a-f]*:	b9 2e       	st\.d r12\+\+,lr
 *[0-9a-f]*:	ab 28       	st\.d r5\+\+,r8
 *[0-9a-f]*:	a9 26       	st\.d r4\+\+,r6
 *[0-9a-f]*:	bd 22       	st\.d lr\+\+,r2
 *[0-9a-f]*:	bb 26       	st\.d sp\+\+,r6
 *[0-9a-f]*:	b5 26       	st\.d r10\+\+,r6
 *[0-9a-f]*:	af 22       	st\.d r7\+\+,r2

[0-9a-f]* <std_predec>:
 *[0-9a-f]*:	bf 21       	st\.d --pc,r0
 *[0-9a-f]*:	b9 2f       	st\.d --r12,lr
 *[0-9a-f]*:	ab 29       	st\.d --r5,r8
 *[0-9a-f]*:	a9 27       	st\.d --r4,r6
 *[0-9a-f]*:	bd 23       	st\.d --lr,r2
 *[0-9a-f]*:	a7 27       	st\.d --r3,r6
 *[0-9a-f]*:	bd 23       	st\.d --lr,r2
 *[0-9a-f]*:	a1 25       	st\.d --r0,r4

[0-9a-f]* <mul>:
 *[0-9a-f]*:	bf 3f       	mul pc,pc
 *[0-9a-f]*:	b9 3c       	mul r12,r12
 *[0-9a-f]*:	ab 35       	mul r5,r5
 *[0-9a-f]*:	a9 34       	mul r4,r4
 *[0-9a-f]*:	bd 3e       	mul lr,lr
 *[0-9a-f]*:	bd 3a       	mul r10,lr
 *[0-9a-f]*:	b1 30       	mul r0,r8
 *[0-9a-f]*:	ab 38       	mul r8,r5

[0-9a-f]* <asr_imm5>:
 *[0-9a-f]*:	a1 4f       	asr pc,0x0
 *[0-9a-f]*:	bf 5c       	asr r12,0x1f
 *[0-9a-f]*:	b1 45       	asr r5,0x10
 *[0-9a-f]*:	af 54       	asr r4,0xf
 *[0-9a-f]*:	a1 5e       	asr lr,0x1
 *[0-9a-f]*:	b7 56       	asr r6,0x17
 *[0-9a-f]*:	b3 46       	asr r6,0x12
 *[0-9a-f]*:	a9 45       	asr r5,0x8

[0-9a-f]* <lsl_imm5>:
 *[0-9a-f]*:	a1 6f       	lsl pc,0x0
 *[0-9a-f]*:	bf 7c       	lsl r12,0x1f
 *[0-9a-f]*:	b1 65       	lsl r5,0x10
 *[0-9a-f]*:	af 74       	lsl r4,0xf
 *[0-9a-f]*:	a1 7e       	lsl lr,0x1
 *[0-9a-f]*:	ad 7c       	lsl r12,0xd
 *[0-9a-f]*:	b1 66       	lsl r6,0x10
 *[0-9a-f]*:	b9 71       	lsl r1,0x19

[0-9a-f]* <lsr_imm5>:
 *[0-9a-f]*:	a1 8f       	lsr pc,0x0
 *[0-9a-f]*:	bf 9c       	lsr r12,0x1f
 *[0-9a-f]*:	b1 85       	lsr r5,0x10
 *[0-9a-f]*:	af 94       	lsr r4,0xf
 *[0-9a-f]*:	a1 9e       	lsr lr,0x1
 *[0-9a-f]*:	a1 90       	lsr r0,0x1
 *[0-9a-f]*:	ab 88       	lsr r8,0xa
 *[0-9a-f]*:	bb 87       	lsr r7,0x1a

[0-9a-f]* <sbr>:
 *[0-9a-f]*:	a1 af       	sbr pc,0x0
 *[0-9a-f]*:	bf bc       	sbr r12,0x1f
 *[0-9a-f]*:	b1 a5       	sbr r5,0x10
 *[0-9a-f]*:	af b4       	sbr r4,0xf
 *[0-9a-f]*:	a1 be       	sbr lr,0x1
 *[0-9a-f]*:	bf b8       	sbr r8,0x1f
 *[0-9a-f]*:	b7 a6       	sbr r6,0x16
 *[0-9a-f]*:	b7 b1       	sbr r1,0x17

[0-9a-f]* <cbr>:
 *[0-9a-f]*:	a1 cf       	cbr pc,0x0
 *[0-9a-f]*:	bf dc       	cbr r12,0x1f
 *[0-9a-f]*:	b1 c5       	cbr r5,0x10
 *[0-9a-f]*:	af d4       	cbr r4,0xf
 *[0-9a-f]*:	a1 de       	cbr lr,0x1
 *[0-9a-f]*:	ab cc       	cbr r12,0xa
 *[0-9a-f]*:	b7 c7       	cbr r7,0x16
 *[0-9a-f]*:	a9 d8       	cbr r8,0x9

[0-9a-f]* <brc1>:
 *[0-9a-f]*:	c0 00       	breq [0-9a-f]* <.*>
 *[0-9a-f]*:	cf f7       	brpl [0-9a-f]* <.*>
 *[0-9a-f]*:	c8 04       	brge [0-9a-f]* <.*>
 *[0-9a-f]*:	c7 f3       	brcs [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 11       	brne [0-9a-f]* <.*>
 *[0-9a-f]*:	c7 33       	brcs [0-9a-f]* <.*>
 *[0-9a-f]*:	cf 70       	breq [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 60       	breq [0-9a-f]* <.*>

[0-9a-f]* <rjmp>:
 *[0-9a-f]*:	c0 08       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	cf fb       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 0a       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	cf f9       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 18       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	c1 fa       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 78       	rjmp [0-9a-f]* <.*>
 *[0-9a-f]*:	cf ea       	rjmp [0-9a-f]* <.*>

[0-9a-f]* <rcall1>:
 *[0-9a-f]*:	c0 0c       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	cf ff       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 0e       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	cf fd       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 1c       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	c6 cc       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	cf 7e       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	c1 ae       	rcall [0-9a-f]* <.*>

[0-9a-f]* <acall>:
 *[0-9a-f]*:	d0 00       	acall 0x0
 *[0-9a-f]*:	df f0       	acall 0x3fc
 *[0-9a-f]*:	d8 00       	acall 0x200
 *[0-9a-f]*:	d7 f0       	acall 0x1fc
 *[0-9a-f]*:	d0 10       	acall 0x4
 *[0-9a-f]*:	d5 90       	acall 0x164
 *[0-9a-f]*:	d4 c0       	acall 0x130
 *[0-9a-f]*:	d2 b0       	acall 0xac

[0-9a-f]* <scall>:
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall
 *[0-9a-f]*:	d7 33       	scall

[0-9a-f]* <popm>:
 *[0-9a-f]*:	d8 02       	popm pc
 *[0-9a-f]*:	dd fa       	popm r0-r11,pc,r12=-1
 *[0-9a-f]*:	d4 02       	popm lr
 *[0-9a-f]*:	db fa       	popm r0-r11,pc,r12=1
 *[0-9a-f]*:	d0 12       	popm r0-r3
 *[0-9a-f]*:	d8 e2       	popm r4-r10,pc
 *[0-9a-f]*:	d9 1a       	popm r0-r3,r11,pc,r12=0
 *[0-9a-f]*:	d7 b2       	popm r0-r7,r10-r12,lr

[0-9a-f]* <pushm>:
 *[0-9a-f]*:	d8 01       	pushm pc
 *[0-9a-f]*:	df f1       	pushm r0-r12,lr-pc
 *[0-9a-f]*:	d8 01       	pushm pc
 *[0-9a-f]*:	d7 f1       	pushm r0-r12,lr
 *[0-9a-f]*:	d0 11       	pushm r0-r3
 *[0-9a-f]*:	dc c1       	pushm r8-r10,lr-pc
 *[0-9a-f]*:	d0 91       	pushm r0-r3,r10
 *[0-9a-f]*:	d2 41       	pushm r8-r9,r12

[0-9a-f]* <popm_n>:
.*
.*
.*
.*
.*
.*
.*
.*

[0-9a-f]* <pushm_n>:
.*
.*
.*
.*
.*
.*
.*
.*

[0-9a-f]* <csrfcz>:
 *[0-9a-f]*:	d0 03       	csrfcz 0x0
 *[0-9a-f]*:	d1 f3       	csrfcz 0x1f
 *[0-9a-f]*:	d1 03       	csrfcz 0x10
 *[0-9a-f]*:	d0 f3       	csrfcz 0xf
 *[0-9a-f]*:	d0 13       	csrfcz 0x1
 *[0-9a-f]*:	d0 53       	csrfcz 0x5
 *[0-9a-f]*:	d0 d3       	csrfcz 0xd
 *[0-9a-f]*:	d1 73       	csrfcz 0x17

[0-9a-f]* <ssrf>:
 *[0-9a-f]*:	d2 03       	ssrf 0x0
 *[0-9a-f]*:	d3 f3       	ssrf 0x1f
 *[0-9a-f]*:	d3 03       	ssrf 0x10
 *[0-9a-f]*:	d2 f3       	ssrf 0xf
 *[0-9a-f]*:	d2 13       	ssrf 0x1
 *[0-9a-f]*:	d3 d3       	ssrf 0x1d
 *[0-9a-f]*:	d2 d3       	ssrf 0xd
 *[0-9a-f]*:	d2 d3       	ssrf 0xd

[0-9a-f]* <csrf>:
 *[0-9a-f]*:	d4 03       	csrf 0x0
 *[0-9a-f]*:	d5 f3       	csrf 0x1f
 *[0-9a-f]*:	d5 03       	csrf 0x10
 *[0-9a-f]*:	d4 f3       	csrf 0xf
 *[0-9a-f]*:	d4 13       	csrf 0x1
 *[0-9a-f]*:	d4 a3       	csrf 0xa
 *[0-9a-f]*:	d4 f3       	csrf 0xf
 *[0-9a-f]*:	d4 b3       	csrf 0xb

[0-9a-f]* <rete>:
 *[0-9a-f]*:	d6 03       	rete

[0-9a-f]* <rets>:
 *[0-9a-f]*:	d6 13       	rets

[0-9a-f]* <retd>:
 *[0-9a-f]*:	d6 23       	retd

[0-9a-f]* <retj>:
 *[0-9a-f]*:	d6 33       	retj

[0-9a-f]* <tlbr>:
 *[0-9a-f]*:	d6 43       	tlbr

[0-9a-f]* <tlbs>:
 *[0-9a-f]*:	d6 53       	tlbs

[0-9a-f]* <tlbw>:
 *[0-9a-f]*:	d6 63       	tlbw

[0-9a-f]* <breakpoint>:
 *[0-9a-f]*:	d6 73       	breakpoint

[0-9a-f]* <incjosp>:
 *[0-9a-f]*:	d6 83       	incjosp 1
 *[0-9a-f]*:	d6 93       	incjosp 2
 *[0-9a-f]*:	d6 a3       	incjosp 3
 *[0-9a-f]*:	d6 b3       	incjosp 4
 *[0-9a-f]*:	d6 c3       	incjosp -4
 *[0-9a-f]*:	d6 d3       	incjosp -3
 *[0-9a-f]*:	d6 e3       	incjosp -2
 *[0-9a-f]*:	d6 f3       	incjosp -1

[0-9a-f]* <nop>:
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <popjc>:
 *[0-9a-f]*:	d7 13       	popjc

[0-9a-f]* <pushjc>:
 *[0-9a-f]*:	d7 23       	pushjc

[0-9a-f]* <add2>:
 *[0-9a-f]*:	fe 0f 00 0f 	add pc,pc,pc
 *[0-9a-f]*:	f8 0c 00 3c 	add r12,r12,r12<<0x3
 *[0-9a-f]*:	ea 05 00 25 	add r5,r5,r5<<0x2
 *[0-9a-f]*:	e8 04 00 14 	add r4,r4,r4<<0x1
 *[0-9a-f]*:	fc 0e 00 1e 	add lr,lr,lr<<0x1
 *[0-9a-f]*:	f8 00 00 10 	add r0,r12,r0<<0x1
 *[0-9a-f]*:	f8 04 00 09 	add r9,r12,r4
 *[0-9a-f]*:	f8 07 00 2c 	add r12,r12,r7<<0x2

[0-9a-f]* <sub2>:
 *[0-9a-f]*:	fe 0f 01 0f 	sub pc,pc,pc
 *[0-9a-f]*:	f8 0c 01 3c 	sub r12,r12,r12<<0x3
 *[0-9a-f]*:	ea 05 01 25 	sub r5,r5,r5<<0x2
 *[0-9a-f]*:	e8 04 01 14 	sub r4,r4,r4<<0x1
 *[0-9a-f]*:	fc 0e 01 1e 	sub lr,lr,lr<<0x1
 *[0-9a-f]*:	e6 04 01 0d 	sub sp,r3,r4
 *[0-9a-f]*:	ee 03 01 03 	sub r3,r7,r3
 *[0-9a-f]*:	f4 0d 01 1d 	sub sp,r10,sp<<0x1

[0-9a-f]* <divu>:
 *[0-9a-f]*:	fe 0f 0d 0f 	divu pc,pc,pc
 *[0-9a-f]*:	f8 0c 0d 0c 	divu r12,r12,r12
 *[0-9a-f]*:	ea 05 0d 05 	divu r5,r5,r5
 *[0-9a-f]*:	e8 04 0d 04 	divu r4,r4,r4
 *[0-9a-f]*:	fc 0e 0d 0e 	divu lr,lr,lr
 *[0-9a-f]*:	e8 0f 0d 0d 	divu sp,r4,pc
 *[0-9a-f]*:	ea 0d 0d 05 	divu r5,r5,sp
 *[0-9a-f]*:	fa 00 0d 0a 	divu r10,sp,r0

[0-9a-f]* <addhh_w>:
 *[0-9a-f]*:	fe 0f 0e 0f 	addhh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 0e 3c 	addhh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 0e 35 	addhh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 0e 04 	addhh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 0e 3e 	addhh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	e0 03 0e 00 	addhh\.w r0,r0:b,r3:b
 *[0-9a-f]*:	f8 07 0e 2e 	addhh\.w lr,r12:t,r7:b
 *[0-9a-f]*:	f4 02 0e 23 	addhh\.w r3,r10:t,r2:b

[0-9a-f]* <subhh_w>:
 *[0-9a-f]*:	fe 0f 0f 0f 	subhh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 0f 3c 	subhh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 0f 35 	subhh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 0f 04 	subhh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 0f 3e 	subhh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	e2 07 0f 2a 	subhh\.w r10,r1:t,r7:b
 *[0-9a-f]*:	f4 0e 0f 3f 	subhh\.w pc,r10:t,lr:t
 *[0-9a-f]*:	e0 0c 0f 23 	subhh\.w r3,r0:t,r12:b

[0-9a-f]* <adc>:
 *[0-9a-f]*:	fe 0f 00 4f 	adc pc,pc,pc
 *[0-9a-f]*:	f8 0c 00 4c 	adc r12,r12,r12
 *[0-9a-f]*:	ea 05 00 45 	adc r5,r5,r5
 *[0-9a-f]*:	e8 04 00 44 	adc r4,r4,r4
 *[0-9a-f]*:	fc 0e 00 4e 	adc lr,lr,lr
 *[0-9a-f]*:	e0 07 00 44 	adc r4,r0,r7
 *[0-9a-f]*:	e8 03 00 4d 	adc sp,r4,r3
 *[0-9a-f]*:	f8 00 00 42 	adc r2,r12,r0

[0-9a-f]* <sbc>:
 *[0-9a-f]*:	fe 0f 01 4f 	sbc pc,pc,pc
 *[0-9a-f]*:	f8 0c 01 4c 	sbc r12,r12,r12
 *[0-9a-f]*:	ea 05 01 45 	sbc r5,r5,r5
 *[0-9a-f]*:	e8 04 01 44 	sbc r4,r4,r4
 *[0-9a-f]*:	fc 0e 01 4e 	sbc lr,lr,lr
 *[0-9a-f]*:	ee 09 01 46 	sbc r6,r7,r9
 *[0-9a-f]*:	f0 05 01 40 	sbc r0,r8,r5
 *[0-9a-f]*:	e0 04 01 41 	sbc r1,r0,r4

[0-9a-f]* <mul_2>:
 *[0-9a-f]*:	fe 0f 02 4f 	mul pc,pc,pc
 *[0-9a-f]*:	f8 0c 02 4c 	mul r12,r12,r12
 *[0-9a-f]*:	ea 05 02 45 	mul r5,r5,r5
 *[0-9a-f]*:	e8 04 02 44 	mul r4,r4,r4
 *[0-9a-f]*:	fc 0e 02 4e 	mul lr,lr,lr
 *[0-9a-f]*:	e0 00 02 4f 	mul pc,r0,r0
 *[0-9a-f]*:	fe 0e 02 48 	mul r8,pc,lr
 *[0-9a-f]*:	f8 0f 02 44 	mul r4,r12,pc

[0-9a-f]* <mac>:
 *[0-9a-f]*:	fe 0f 03 4f 	mac pc,pc,pc
 *[0-9a-f]*:	f8 0c 03 4c 	mac r12,r12,r12
 *[0-9a-f]*:	ea 05 03 45 	mac r5,r5,r5
 *[0-9a-f]*:	e8 04 03 44 	mac r4,r4,r4
 *[0-9a-f]*:	fc 0e 03 4e 	mac lr,lr,lr
 *[0-9a-f]*:	e8 00 03 4a 	mac r10,r4,r0
 *[0-9a-f]*:	fc 00 03 47 	mac r7,lr,r0
 *[0-9a-f]*:	f2 0c 03 42 	mac r2,r9,r12

[0-9a-f]* <mulsd>:
 *[0-9a-f]*:	fe 0f 04 4f 	muls\.d pc,pc,pc
 *[0-9a-f]*:	f8 0c 04 4c 	muls\.d r12,r12,r12
 *[0-9a-f]*:	ea 05 04 45 	muls\.d r5,r5,r5
 *[0-9a-f]*:	e8 04 04 44 	muls\.d r4,r4,r4
 *[0-9a-f]*:	fc 0e 04 4e 	muls\.d lr,lr,lr
 *[0-9a-f]*:	f0 0e 04 42 	muls\.d r2,r8,lr
 *[0-9a-f]*:	e0 0b 04 44 	muls\.d r4,r0,r11
 *[0-9a-f]*:	fc 06 04 45 	muls\.d r5,lr,r6

[0-9a-f]* <macsd>:
 *[0-9a-f]*:	fe 0f 05 40 	macs\.d r0,pc,pc
 *[0-9a-f]*:	f8 0c 05 4e 	macs\.d lr,r12,r12
 *[0-9a-f]*:	ea 05 05 48 	macs\.d r8,r5,r5
 *[0-9a-f]*:	e8 04 05 46 	macs\.d r6,r4,r4
 *[0-9a-f]*:	fc 0e 05 42 	macs\.d r2,lr,lr
 *[0-9a-f]*:	e2 09 05 48 	macs\.d r8,r1,r9
 *[0-9a-f]*:	f0 08 05 4e 	macs\.d lr,r8,r8
 *[0-9a-f]*:	e6 0c 05 44 	macs\.d r4,r3,r12

[0-9a-f]* <mulud>:
 *[0-9a-f]*:	fe 0f 06 40 	mulu\.d r0,pc,pc
 *[0-9a-f]*:	f8 0c 06 4e 	mulu\.d lr,r12,r12
 *[0-9a-f]*:	ea 05 06 48 	mulu\.d r8,r5,r5
 *[0-9a-f]*:	e8 04 06 46 	mulu\.d r6,r4,r4
 *[0-9a-f]*:	fc 0e 06 42 	mulu\.d r2,lr,lr
 *[0-9a-f]*:	ea 00 06 46 	mulu\.d r6,r5,r0
 *[0-9a-f]*:	ec 01 06 44 	mulu\.d r4,r6,r1
 *[0-9a-f]*:	f0 02 06 48 	mulu\.d r8,r8,r2

[0-9a-f]* <macud>:
 *[0-9a-f]*:	fe 0f 07 40 	macu\.d r0,pc,pc
 *[0-9a-f]*:	f8 0c 07 4e 	macu\.d lr,r12,r12
 *[0-9a-f]*:	ea 05 07 48 	macu\.d r8,r5,r5
 *[0-9a-f]*:	e8 04 07 46 	macu\.d r6,r4,r4
 *[0-9a-f]*:	fc 0e 07 42 	macu\.d r2,lr,lr
 *[0-9a-f]*:	fa 0b 07 46 	macu\.d r6,sp,r11
 *[0-9a-f]*:	e8 08 07 42 	macu\.d r2,r4,r8
 *[0-9a-f]*:	f4 09 07 46 	macu\.d r6,r10,r9

[0-9a-f]* <asr_1>:
 *[0-9a-f]*:	fe 0f 08 4f 	asr pc,pc,pc
 *[0-9a-f]*:	f8 0c 08 4c 	asr r12,r12,r12
 *[0-9a-f]*:	ea 05 08 45 	asr r5,r5,r5
 *[0-9a-f]*:	e8 04 08 44 	asr r4,r4,r4
 *[0-9a-f]*:	fc 0e 08 4e 	asr lr,lr,lr
 *[0-9a-f]*:	ec 0f 08 4f 	asr pc,r6,pc
 *[0-9a-f]*:	ec 0c 08 40 	asr r0,r6,r12
 *[0-9a-f]*:	fa 00 08 44 	asr r4,sp,r0

[0-9a-f]* <lsl_1>:
 *[0-9a-f]*:	fe 0f 09 4f 	lsl pc,pc,pc
 *[0-9a-f]*:	f8 0c 09 4c 	lsl r12,r12,r12
 *[0-9a-f]*:	ea 05 09 45 	lsl r5,r5,r5
 *[0-9a-f]*:	e8 04 09 44 	lsl r4,r4,r4
 *[0-9a-f]*:	fc 0e 09 4e 	lsl lr,lr,lr
 *[0-9a-f]*:	ea 0e 09 4e 	lsl lr,r5,lr
 *[0-9a-f]*:	fe 03 09 45 	lsl r5,pc,r3
 *[0-9a-f]*:	fe 09 09 41 	lsl r1,pc,r9

[0-9a-f]* <lsr_1>:
 *[0-9a-f]*:	fe 0f 0a 4f 	lsr pc,pc,pc
 *[0-9a-f]*:	f8 0c 0a 4c 	lsr r12,r12,r12
 *[0-9a-f]*:	ea 05 0a 45 	lsr r5,r5,r5
 *[0-9a-f]*:	e8 04 0a 44 	lsr r4,r4,r4
 *[0-9a-f]*:	fc 0e 0a 4e 	lsr lr,lr,lr
 *[0-9a-f]*:	e8 01 0a 42 	lsr r2,r4,r1
 *[0-9a-f]*:	e2 06 0a 45 	lsr r5,r1,r6
 *[0-9a-f]*:	ec 07 0a 4d 	lsr sp,r6,r7

[0-9a-f]* <xchg>:
 *[0-9a-f]*:	fe 0f 0b 4f 	xchg pc,pc,pc
 *[0-9a-f]*:	f8 0c 0b 4c 	xchg r12,r12,r12
 *[0-9a-f]*:	ea 05 0b 45 	xchg r5,r5,r5
 *[0-9a-f]*:	e8 04 0b 44 	xchg r4,r4,r4
 *[0-9a-f]*:	fc 0e 0b 4e 	xchg lr,lr,lr
 *[0-9a-f]*:	e8 0d 0b 4e 	xchg lr,r4,sp
 *[0-9a-f]*:	ea 0c 0b 41 	xchg r1,r5,r12
 *[0-9a-f]*:	f8 00 0b 4e 	xchg lr,r12,r0

[0-9a-f]* <max>:
 *[0-9a-f]*:	fe 0f 0c 4f 	max pc,pc,pc
 *[0-9a-f]*:	f8 0c 0c 4c 	max r12,r12,r12
 *[0-9a-f]*:	ea 05 0c 45 	max r5,r5,r5
 *[0-9a-f]*:	e8 04 0c 44 	max r4,r4,r4
 *[0-9a-f]*:	fc 0e 0c 4e 	max lr,lr,lr
 *[0-9a-f]*:	e4 0d 0c 4e 	max lr,r2,sp
 *[0-9a-f]*:	f4 09 0c 44 	max r4,r10,r9
 *[0-9a-f]*:	f2 0e 0c 4e 	max lr,r9,lr

[0-9a-f]* <min>:
 *[0-9a-f]*:	fe 0f 0d 4f 	min pc,pc,pc
 *[0-9a-f]*:	f8 0c 0d 4c 	min r12,r12,r12
 *[0-9a-f]*:	ea 05 0d 45 	min r5,r5,r5
 *[0-9a-f]*:	e8 04 0d 44 	min r4,r4,r4
 *[0-9a-f]*:	fc 0e 0d 4e 	min lr,lr,lr
 *[0-9a-f]*:	ee 08 0d 49 	min r9,r7,r8
 *[0-9a-f]*:	ea 05 0d 4d 	min sp,r5,r5
 *[0-9a-f]*:	e2 04 0d 44 	min r4,r1,r4

[0-9a-f]* <addabs>:
 *[0-9a-f]*:	fe 0f 0e 4f 	addabs pc,pc,pc
 *[0-9a-f]*:	f8 0c 0e 4c 	addabs r12,r12,r12
 *[0-9a-f]*:	ea 05 0e 45 	addabs r5,r5,r5
 *[0-9a-f]*:	e8 04 0e 44 	addabs r4,r4,r4
 *[0-9a-f]*:	fc 0e 0e 4e 	addabs lr,lr,lr
 *[0-9a-f]*:	f4 00 0e 47 	addabs r7,r10,r0
 *[0-9a-f]*:	f2 07 0e 49 	addabs r9,r9,r7
 *[0-9a-f]*:	f0 0c 0e 42 	addabs r2,r8,r12

[0-9a-f]* <mulnhh_w>:
 *[0-9a-f]*:	fe 0f 01 8f 	mulnhh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 01 bc 	mulnhh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 01 b5 	mulnhh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 01 84 	mulnhh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 01 be 	mulnhh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	fa 09 01 ab 	mulnhh\.w r11,sp:t,r9:b
 *[0-9a-f]*:	e8 0e 01 9d 	mulnhh\.w sp,r4:b,lr:t
 *[0-9a-f]*:	e4 0b 01 ac 	mulnhh\.w r12,r2:t,r11:b

[0-9a-f]* <mulnwh_d>:
 *[0-9a-f]*:	fe 0f 02 80 	mulnwh\.d r0,pc,pc:b
 *[0-9a-f]*:	f8 0c 02 9e 	mulnwh\.d lr,r12,r12:t
 *[0-9a-f]*:	ea 05 02 98 	mulnwh\.d r8,r5,r5:t
 *[0-9a-f]*:	e8 04 02 86 	mulnwh\.d r6,r4,r4:b
 *[0-9a-f]*:	fc 0e 02 92 	mulnwh\.d r2,lr,lr:t
 *[0-9a-f]*:	e6 02 02 9e 	mulnwh\.d lr,r3,r2:t
 *[0-9a-f]*:	ea 09 02 84 	mulnwh\.d r4,r5,r9:b
 *[0-9a-f]*:	e8 04 02 9c 	mulnwh\.d r12,r4,r4:t

[0-9a-f]* <machh_w>:
 *[0-9a-f]*:	fe 0f 04 8f 	machh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 04 bc 	machh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 04 b5 	machh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 04 84 	machh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 04 be 	machh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	ea 01 04 9e 	machh\.w lr,r5:b,r1:t
 *[0-9a-f]*:	ec 07 04 89 	machh\.w r9,r6:b,r7:b
 *[0-9a-f]*:	fc 0c 04 a5 	machh\.w r5,lr:t,r12:b

[0-9a-f]* <machh_d>:
 *[0-9a-f]*:	fe 0f 05 80 	machh\.d r0,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 05 be 	machh\.d lr,r12:t,r12:t
 *[0-9a-f]*:	ea 05 05 b8 	machh\.d r8,r5:t,r5:t
 *[0-9a-f]*:	e8 04 05 86 	machh\.d r6,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 05 b2 	machh\.d r2,lr:t,lr:t
 *[0-9a-f]*:	e0 08 05 8a 	machh\.d r10,r0:b,r8:b
 *[0-9a-f]*:	e8 05 05 9e 	machh\.d lr,r4:b,r5:t
 *[0-9a-f]*:	e0 04 05 98 	machh\.d r8,r0:b,r4:t

[0-9a-f]* <macsathh_w>:
 *[0-9a-f]*:	fe 0f 06 8f 	macsathh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 06 bc 	macsathh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 06 b5 	macsathh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 06 84 	macsathh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 06 be 	macsathh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	ee 0f 06 b7 	macsathh\.w r7,r7:t,pc:t
 *[0-9a-f]*:	e4 04 06 a4 	macsathh\.w r4,r2:t,r4:b
 *[0-9a-f]*:	f0 03 06 b4 	macsathh\.w r4,r8:t,r3:t

[0-9a-f]* <mulhh_w>:
 *[0-9a-f]*:	fe 0f 07 8f 	mulhh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 07 bc 	mulhh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 07 b5 	mulhh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 07 84 	mulhh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 07 be 	mulhh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	e8 09 07 a7 	mulhh\.w r7,r4:t,r9:b
 *[0-9a-f]*:	e6 07 07 bf 	mulhh\.w pc,r3:t,r7:t
 *[0-9a-f]*:	e8 09 07 9f 	mulhh\.w pc,r4:b,r9:t

[0-9a-f]* <mulsathh_h>:
 *[0-9a-f]*:	fe 0f 08 8f 	mulsathh\.h pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 08 bc 	mulsathh\.h r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 08 b5 	mulsathh\.h r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 08 84 	mulsathh\.h r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 08 be 	mulsathh\.h lr,lr:t,lr:t
 *[0-9a-f]*:	e2 0d 08 83 	mulsathh\.h r3,r1:b,sp:b
 *[0-9a-f]*:	fc 0b 08 ab 	mulsathh\.h r11,lr:t,r11:b
 *[0-9a-f]*:	f0 0b 08 98 	mulsathh\.h r8,r8:b,r11:t

[0-9a-f]* <mulsathh_w>:
 *[0-9a-f]*:	fe 0f 09 8f 	mulsathh\.w pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 09 bc 	mulsathh\.w r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 09 b5 	mulsathh\.w r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 09 84 	mulsathh\.w r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 09 be 	mulsathh\.w lr,lr:t,lr:t
 *[0-9a-f]*:	f6 06 09 ae 	mulsathh\.w lr,r11:t,r6:b
 *[0-9a-f]*:	ec 07 09 96 	mulsathh\.w r6,r6:b,r7:t
 *[0-9a-f]*:	e4 03 09 8a 	mulsathh\.w r10,r2:b,r3:b

[0-9a-f]* <mulsatrndhh_h>:
 *[0-9a-f]*:	fe 0f 0a 8f 	mulsatrndhh\.h pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 0a bc 	mulsatrndhh\.h r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 0a b5 	mulsatrndhh\.h r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 0a 84 	mulsatrndhh\.h r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 0a be 	mulsatrndhh\.h lr,lr:t,lr:t
 *[0-9a-f]*:	ec 09 0a 8b 	mulsatrndhh\.h r11,r6:b,r9:b
 *[0-9a-f]*:	e6 08 0a 9b 	mulsatrndhh\.h r11,r3:b,r8:t
 *[0-9a-f]*:	fa 07 0a b5 	mulsatrndhh\.h r5,sp:t,r7:t

[0-9a-f]* <mulsatrndwh_w>:
 *[0-9a-f]*:	fe 0f 0b 8f 	mulsatrndwh\.w pc,pc,pc:b
 *[0-9a-f]*:	f8 0c 0b 9c 	mulsatrndwh\.w r12,r12,r12:t
 *[0-9a-f]*:	ea 05 0b 95 	mulsatrndwh\.w r5,r5,r5:t
 *[0-9a-f]*:	e8 04 0b 84 	mulsatrndwh\.w r4,r4,r4:b
 *[0-9a-f]*:	fc 0e 0b 9e 	mulsatrndwh\.w lr,lr,lr:t
 *[0-9a-f]*:	f8 00 0b 85 	mulsatrndwh\.w r5,r12,r0:b
 *[0-9a-f]*:	f4 0f 0b 87 	mulsatrndwh\.w r7,r10,pc:b
 *[0-9a-f]*:	f0 05 0b 9a 	mulsatrndwh\.w r10,r8,r5:t

[0-9a-f]* <macwh_d>:
 *[0-9a-f]*:	fe 0f 0c 80 	macwh\.d r0,pc,pc:b
 *[0-9a-f]*:	f8 0c 0c 9e 	macwh\.d lr,r12,r12:t
 *[0-9a-f]*:	ea 05 0c 98 	macwh\.d r8,r5,r5:t
 *[0-9a-f]*:	e8 04 0c 86 	macwh\.d r6,r4,r4:b
 *[0-9a-f]*:	fc 0e 0c 92 	macwh\.d r2,lr,lr:t
 *[0-9a-f]*:	f4 0c 0c 94 	macwh\.d r4,r10,r12:t
 *[0-9a-f]*:	ee 0d 0c 84 	macwh\.d r4,r7,sp:b
 *[0-9a-f]*:	f2 0b 0c 8e 	macwh\.d lr,r9,r11:b

[0-9a-f]* <mulwh_d>:
 *[0-9a-f]*:	fe 0f 0d 80 	mulwh\.d r0,pc,pc:b
 *[0-9a-f]*:	f8 0c 0d 9e 	mulwh\.d lr,r12,r12:t
 *[0-9a-f]*:	ea 05 0d 98 	mulwh\.d r8,r5,r5:t
 *[0-9a-f]*:	e8 04 0d 86 	mulwh\.d r6,r4,r4:b
 *[0-9a-f]*:	fc 0e 0d 92 	mulwh\.d r2,lr,lr:t
 *[0-9a-f]*:	ea 01 0d 8c 	mulwh\.d r12,r5,r1:b
 *[0-9a-f]*:	e2 03 0d 90 	mulwh\.d r0,r1,r3:t
 *[0-9a-f]*:	f2 02 0d 80 	mulwh\.d r0,r9,r2:b

[0-9a-f]* <mulsatwh_w>:
 *[0-9a-f]*:	fe 0f 0e 8f 	mulsatwh\.w pc,pc,pc:b
 *[0-9a-f]*:	f8 0c 0e 9c 	mulsatwh\.w r12,r12,r12:t
 *[0-9a-f]*:	ea 05 0e 95 	mulsatwh\.w r5,r5,r5:t
 *[0-9a-f]*:	e8 04 0e 84 	mulsatwh\.w r4,r4,r4:b
 *[0-9a-f]*:	fc 0e 0e 9e 	mulsatwh\.w lr,lr,lr:t
 *[0-9a-f]*:	fe 0a 0e 9b 	mulsatwh\.w r11,pc,r10:t
 *[0-9a-f]*:	f8 09 0e 9d 	mulsatwh\.w sp,r12,r9:t
 *[0-9a-f]*:	e6 02 0e 90 	mulsatwh\.w r0,r3,r2:t

[0-9a-f]* <ldw7>:
 *[0-9a-f]*:	fe 0f 0f 8f 	ld\.w pc,pc\[pc:b<<2\]
 *[0-9a-f]*:	f8 0c 0f bc 	ld\.w r12,r12\[r12:t<<2\]
 *[0-9a-f]*:	ea 05 0f a5 	ld\.w r5,r5\[r5:u<<2\]
 *[0-9a-f]*:	e8 04 0f 94 	ld\.w r4,r4\[r4:l<<2\]
 *[0-9a-f]*:	fc 0e 0f 9e 	ld\.w lr,lr\[lr:l<<2\]
 *[0-9a-f]*:	f4 06 0f 99 	ld\.w r9,r10\[r6:l<<2\]
 *[0-9a-f]*:	f4 0a 0f 82 	ld\.w r2,r10\[r10:b<<2\]
 *[0-9a-f]*:	ea 0f 0f 8b 	ld\.w r11,r5\[pc:b<<2\]

[0-9a-f]* <satadd_w>:
 *[0-9a-f]*:	fe 0f 00 cf 	satadd\.w pc,pc,pc
 *[0-9a-f]*:	f8 0c 00 cc 	satadd\.w r12,r12,r12
 *[0-9a-f]*:	ea 05 00 c5 	satadd\.w r5,r5,r5
 *[0-9a-f]*:	e8 04 00 c4 	satadd\.w r4,r4,r4
 *[0-9a-f]*:	fc 0e 00 ce 	satadd\.w lr,lr,lr
 *[0-9a-f]*:	f0 0b 00 c4 	satadd\.w r4,r8,r11
 *[0-9a-f]*:	f8 06 00 c3 	satadd\.w r3,r12,r6
 *[0-9a-f]*:	fc 09 00 c3 	satadd\.w r3,lr,r9

[0-9a-f]* <satsub_w1>:
 *[0-9a-f]*:	fe 0f 01 cf 	satsub\.w pc,pc,pc
 *[0-9a-f]*:	f8 0c 01 cc 	satsub\.w r12,r12,r12
 *[0-9a-f]*:	ea 05 01 c5 	satsub\.w r5,r5,r5
 *[0-9a-f]*:	e8 04 01 c4 	satsub\.w r4,r4,r4
 *[0-9a-f]*:	fc 0e 01 ce 	satsub\.w lr,lr,lr
 *[0-9a-f]*:	fa 00 01 c8 	satsub\.w r8,sp,r0
 *[0-9a-f]*:	f0 04 01 c9 	satsub\.w r9,r8,r4
 *[0-9a-f]*:	fc 02 01 cf 	satsub\.w pc,lr,r2

[0-9a-f]* <satadd_h>:
 *[0-9a-f]*:	fe 0f 02 cf 	satadd\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 02 cc 	satadd\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 02 c5 	satadd\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 02 c4 	satadd\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 02 ce 	satadd\.h lr,lr,lr
 *[0-9a-f]*:	e6 09 02 c7 	satadd\.h r7,r3,r9
 *[0-9a-f]*:	e0 02 02 c1 	satadd\.h r1,r0,r2
 *[0-9a-f]*:	e8 0e 02 c1 	satadd\.h r1,r4,lr

[0-9a-f]* <satsub_h>:
 *[0-9a-f]*:	fe 0f 03 cf 	satsub\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 03 cc 	satsub\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 03 c5 	satsub\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 03 c4 	satsub\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 03 ce 	satsub\.h lr,lr,lr
 *[0-9a-f]*:	fc 03 03 ce 	satsub\.h lr,lr,r3
 *[0-9a-f]*:	ec 05 03 cb 	satsub\.h r11,r6,r5
 *[0-9a-f]*:	fa 00 03 c3 	satsub\.h r3,sp,r0

[0-9a-f]* <mul3>:
 *[0-9a-f]*:	fe 0f 10 00 	mul pc,pc,0
 *[0-9a-f]*:	f8 0c 10 ff 	mul r12,r12,-1
 *[0-9a-f]*:	ea 05 10 80 	mul r5,r5,-128
 *[0-9a-f]*:	e8 04 10 7f 	mul r4,r4,127
 *[0-9a-f]*:	fc 0e 10 01 	mul lr,lr,1
 *[0-9a-f]*:	e4 0c 10 f9 	mul r12,r2,-7
 *[0-9a-f]*:	fe 01 10 5f 	mul r1,pc,95
 *[0-9a-f]*:	ec 04 10 13 	mul r4,r6,19

[0-9a-f]* <rsub2>:
 *[0-9a-f]*:	fe 0f 11 00 	rsub pc,pc,0
 *[0-9a-f]*:	f8 0c 11 ff 	rsub r12,r12,-1
 *[0-9a-f]*:	ea 05 11 80 	rsub r5,r5,-128
 *[0-9a-f]*:	e8 04 11 7f 	rsub r4,r4,127
 *[0-9a-f]*:	fc 0e 11 01 	rsub lr,lr,1
 *[0-9a-f]*:	fc 09 11 60 	rsub r9,lr,96
 *[0-9a-f]*:	e2 0b 11 38 	rsub r11,r1,56
 *[0-9a-f]*:	ee 00 11 a9 	rsub r0,r7,-87

[0-9a-f]* <clz>:
 *[0-9a-f]*:	fe 0f 12 00 	clz pc,pc
 *[0-9a-f]*:	f8 0c 12 00 	clz r12,r12
 *[0-9a-f]*:	ea 05 12 00 	clz r5,r5
 *[0-9a-f]*:	e8 04 12 00 	clz r4,r4
 *[0-9a-f]*:	fc 0e 12 00 	clz lr,lr
 *[0-9a-f]*:	e6 02 12 00 	clz r2,r3
 *[0-9a-f]*:	f6 05 12 00 	clz r5,r11
 *[0-9a-f]*:	e6 0f 12 00 	clz pc,r3

[0-9a-f]* <cpc1>:
 *[0-9a-f]*:	fe 0f 13 00 	cpc pc,pc
 *[0-9a-f]*:	f8 0c 13 00 	cpc r12,r12
 *[0-9a-f]*:	ea 05 13 00 	cpc r5,r5
 *[0-9a-f]*:	e8 04 13 00 	cpc r4,r4
 *[0-9a-f]*:	fc 0e 13 00 	cpc lr,lr
 *[0-9a-f]*:	e8 0f 13 00 	cpc pc,r4
 *[0-9a-f]*:	f2 05 13 00 	cpc r5,r9
 *[0-9a-f]*:	ee 06 13 00 	cpc r6,r7

[0-9a-f]* <asr3>:
 *[0-9a-f]*:	fe 0f 14 00 	asr pc,pc,0x0
 *[0-9a-f]*:	f8 0c 14 1f 	asr r12,r12,0x1f
 *[0-9a-f]*:	ea 05 14 10 	asr r5,r5,0x10
 *[0-9a-f]*:	e8 04 14 0f 	asr r4,r4,0xf
 *[0-9a-f]*:	fc 0e 14 01 	asr lr,lr,0x1
 *[0-9a-f]*:	f6 04 14 13 	asr r4,r11,0x13
 *[0-9a-f]*:	fe 0d 14 1a 	asr sp,pc,0x1a
 *[0-9a-f]*:	fa 0b 14 08 	asr r11,sp,0x8

[0-9a-f]* <lsl3>:
 *[0-9a-f]*:	fe 0f 15 00 	lsl pc,pc,0x0
 *[0-9a-f]*:	f8 0c 15 1f 	lsl r12,r12,0x1f
 *[0-9a-f]*:	ea 05 15 10 	lsl r5,r5,0x10
 *[0-9a-f]*:	e8 04 15 0f 	lsl r4,r4,0xf
 *[0-9a-f]*:	fc 0e 15 01 	lsl lr,lr,0x1
 *[0-9a-f]*:	f4 08 15 11 	lsl r8,r10,0x11
 *[0-9a-f]*:	fc 02 15 03 	lsl r2,lr,0x3
 *[0-9a-f]*:	f6 0e 15 0e 	lsl lr,r11,0xe

[0-9a-f]* <lsr3>:
 *[0-9a-f]*:	fe 0f 16 00 	lsr pc,pc,0x0
 *[0-9a-f]*:	f8 0c 16 1f 	lsr r12,r12,0x1f
 *[0-9a-f]*:	ea 05 16 10 	lsr r5,r5,0x10
 *[0-9a-f]*:	e8 04 16 0f 	lsr r4,r4,0xf
 *[0-9a-f]*:	fc 0e 16 01 	lsr lr,lr,0x1
 *[0-9a-f]*:	e6 04 16 1f 	lsr r4,r3,0x1f
 *[0-9a-f]*:	f2 0f 16 0e 	lsr pc,r9,0xe
 *[0-9a-f]*:	e0 03 16 06 	lsr r3,r0,0x6

[0-9a-f]* <movc1>:
 *[0-9a-f]*:	fe 0f 17 00 	moveq pc,pc
 *[0-9a-f]*:	f8 0c 17 f0 	moval r12,r12
 *[0-9a-f]*:	ea 05 17 80 	movls r5,r5
 *[0-9a-f]*:	e8 04 17 70 	movpl r4,r4
 *[0-9a-f]*:	fc 0e 17 10 	movne lr,lr
 *[0-9a-f]*:	f6 0f 17 10 	movne pc,r11
 *[0-9a-f]*:	e4 0a 17 60 	movmi r10,r2
 *[0-9a-f]*:	f8 08 17 80 	movls r8,r12

[0-9a-f]* <padd_h>:
 *[0-9a-f]*:	fe 0f 20 0f 	padd\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 0c 	padd\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 20 05 	padd\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 20 04 	padd\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 0e 	padd\.h lr,lr,lr
 *[0-9a-f]*:	e4 07 20 08 	padd\.h r8,r2,r7
 *[0-9a-f]*:	e0 03 20 00 	padd\.h r0,r0,r3
 *[0-9a-f]*:	f6 06 20 0d 	padd\.h sp,r11,r6

[0-9a-f]* <psub_h>:
 *[0-9a-f]*:	fe 0f 20 1f 	psub\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 1c 	psub\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 20 15 	psub\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 20 14 	psub\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 1e 	psub\.h lr,lr,lr
 *[0-9a-f]*:	ec 08 20 1e 	psub\.h lr,r6,r8
 *[0-9a-f]*:	e2 0d 20 10 	psub\.h r0,r1,sp
 *[0-9a-f]*:	fe 0d 20 1f 	psub\.h pc,pc,sp

[0-9a-f]* <paddx_h>:
 *[0-9a-f]*:	fe 0f 20 2f 	paddx\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 2c 	paddx\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 20 25 	paddx\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 20 24 	paddx\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 2e 	paddx\.h lr,lr,lr
 *[0-9a-f]*:	fe 01 20 2f 	paddx\.h pc,pc,r1
 *[0-9a-f]*:	e8 05 20 2a 	paddx\.h r10,r4,r5
 *[0-9a-f]*:	fe 02 20 25 	paddx\.h r5,pc,r2

[0-9a-f]* <psubx_h>:
 *[0-9a-f]*:	fe 0f 20 3f 	psubx\.h pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 3c 	psubx\.h r12,r12,r12
 *[0-9a-f]*:	ea 05 20 35 	psubx\.h r5,r5,r5
 *[0-9a-f]*:	e8 04 20 34 	psubx\.h r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 3e 	psubx\.h lr,lr,lr
 *[0-9a-f]*:	f8 05 20 35 	psubx\.h r5,r12,r5
 *[0-9a-f]*:	f0 03 20 33 	psubx\.h r3,r8,r3
 *[0-9a-f]*:	e4 03 20 35 	psubx\.h r5,r2,r3

[0-9a-f]* <padds_sh>:
 *[0-9a-f]*:	fe 0f 20 4f 	padds\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 4c 	padds\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 45 	padds\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 44 	padds\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 4e 	padds\.sh lr,lr,lr
 *[0-9a-f]*:	fc 02 20 49 	padds\.sh r9,lr,r2
 *[0-9a-f]*:	f0 01 20 46 	padds\.sh r6,r8,r1
 *[0-9a-f]*:	e8 0a 20 46 	padds\.sh r6,r4,r10

[0-9a-f]* <psubs_sh>:
 *[0-9a-f]*:	fe 0f 20 5f 	psubs\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 5c 	psubs\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 55 	psubs\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 54 	psubs\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 5e 	psubs\.sh lr,lr,lr
 *[0-9a-f]*:	fc 0b 20 56 	psubs\.sh r6,lr,r11
 *[0-9a-f]*:	f8 04 20 52 	psubs\.sh r2,r12,r4
 *[0-9a-f]*:	f2 00 20 50 	psubs\.sh r0,r9,r0

[0-9a-f]* <paddxs_sh>:
 *[0-9a-f]*:	fe 0f 20 6f 	paddxs\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 6c 	paddxs\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 65 	paddxs\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 64 	paddxs\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 6e 	paddxs\.sh lr,lr,lr
 *[0-9a-f]*:	e6 09 20 60 	paddxs\.sh r0,r3,r9
 *[0-9a-f]*:	f4 0b 20 6f 	paddxs\.sh pc,r10,r11
 *[0-9a-f]*:	f4 0f 20 6f 	paddxs\.sh pc,r10,pc

[0-9a-f]* <psubxs_sh>:
 *[0-9a-f]*:	fe 0f 20 7f 	psubxs\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 7c 	psubxs\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 75 	psubxs\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 74 	psubxs\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 7e 	psubxs\.sh lr,lr,lr
 *[0-9a-f]*:	e8 04 20 77 	psubxs\.sh r7,r4,r4
 *[0-9a-f]*:	f0 03 20 77 	psubxs\.sh r7,r8,r3
 *[0-9a-f]*:	ec 05 20 7f 	psubxs\.sh pc,r6,r5

[0-9a-f]* <padds_uh>:
 *[0-9a-f]*:	fe 0f 20 8f 	padds\.uh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 8c 	padds\.uh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 85 	padds\.uh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 84 	padds\.uh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 8e 	padds\.uh lr,lr,lr
 *[0-9a-f]*:	f6 07 20 8c 	padds\.uh r12,r11,r7
 *[0-9a-f]*:	f0 0e 20 87 	padds\.uh r7,r8,lr
 *[0-9a-f]*:	f2 07 20 86 	padds\.uh r6,r9,r7

[0-9a-f]* <psubs_uh>:
 *[0-9a-f]*:	fe 0f 20 9f 	psubs\.uh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 9c 	psubs\.uh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 95 	psubs\.uh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 94 	psubs\.uh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 9e 	psubs\.uh lr,lr,lr
 *[0-9a-f]*:	f4 06 20 9e 	psubs\.uh lr,r10,r6
 *[0-9a-f]*:	e4 0f 20 9d 	psubs\.uh sp,r2,pc
 *[0-9a-f]*:	f2 02 20 92 	psubs\.uh r2,r9,r2

[0-9a-f]* <paddxs_uh>:
 *[0-9a-f]*:	fe 0f 20 af 	paddxs\.uh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 ac 	paddxs\.uh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 a5 	paddxs\.uh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 a4 	paddxs\.uh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 ae 	paddxs\.uh lr,lr,lr
 *[0-9a-f]*:	f2 05 20 a7 	paddxs\.uh r7,r9,r5
 *[0-9a-f]*:	e2 04 20 a9 	paddxs\.uh r9,r1,r4
 *[0-9a-f]*:	e4 03 20 a5 	paddxs\.uh r5,r2,r3

[0-9a-f]* <psubxs_uh>:
 *[0-9a-f]*:	fe 0f 20 bf 	psubxs\.uh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 bc 	psubxs\.uh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 b5 	psubxs\.uh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 b4 	psubxs\.uh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 be 	psubxs\.uh lr,lr,lr
 *[0-9a-f]*:	ea 0d 20 bd 	psubxs\.uh sp,r5,sp
 *[0-9a-f]*:	ec 06 20 bd 	psubxs\.uh sp,r6,r6
 *[0-9a-f]*:	f6 08 20 b3 	psubxs\.uh r3,r11,r8

[0-9a-f]* <paddh_sh>:
 *[0-9a-f]*:	fe 0f 20 cf 	paddh\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 cc 	paddh\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 c5 	paddh\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 c4 	paddh\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 ce 	paddh\.sh lr,lr,lr
 *[0-9a-f]*:	fa 03 20 cc 	paddh\.sh r12,sp,r3
 *[0-9a-f]*:	ea 03 20 cf 	paddh\.sh pc,r5,r3
 *[0-9a-f]*:	f0 0d 20 c8 	paddh\.sh r8,r8,sp

[0-9a-f]* <psubh_sh>:
 *[0-9a-f]*:	fe 0f 20 df 	psubh\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 dc 	psubh\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 d5 	psubh\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 d4 	psubh\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 de 	psubh\.sh lr,lr,lr
 *[0-9a-f]*:	ea 08 20 d1 	psubh\.sh r1,r5,r8
 *[0-9a-f]*:	e6 06 20 d7 	psubh\.sh r7,r3,r6
 *[0-9a-f]*:	e6 03 20 d4 	psubh\.sh r4,r3,r3

[0-9a-f]* <paddxh_sh>:
 *[0-9a-f]*:	fe 0f 20 ef 	paddxh\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 ec 	paddxh\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 e5 	paddxh\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 e4 	paddxh\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 ee 	paddxh\.sh lr,lr,lr
 *[0-9a-f]*:	e0 04 20 e6 	paddxh\.sh r6,r0,r4
 *[0-9a-f]*:	f0 09 20 e9 	paddxh\.sh r9,r8,r9
 *[0-9a-f]*:	e0 0d 20 e3 	paddxh\.sh r3,r0,sp

[0-9a-f]* <psubxh_sh>:
 *[0-9a-f]*:	fe 0f 20 ff 	psubxh\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 20 fc 	psubxh\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 20 f5 	psubxh\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 20 f4 	psubxh\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 20 fe 	psubxh\.sh lr,lr,lr
 *[0-9a-f]*:	fe 0c 20 f4 	psubxh\.sh r4,pc,r12
 *[0-9a-f]*:	e8 06 20 f8 	psubxh\.sh r8,r4,r6
 *[0-9a-f]*:	f2 04 20 fc 	psubxh\.sh r12,r9,r4

[0-9a-f]* <paddsub_h>:
 *[0-9a-f]*:	fe 0f 21 0f 	paddsub\.h pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 21 3c 	paddsub\.h r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 21 35 	paddsub\.h r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 21 04 	paddsub\.h r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 21 3e 	paddsub\.h lr,lr:t,lr:t
 *[0-9a-f]*:	e4 0e 21 25 	paddsub\.h r5,r2:t,lr:b
 *[0-9a-f]*:	e2 08 21 07 	paddsub\.h r7,r1:b,r8:b
 *[0-9a-f]*:	f4 05 21 36 	paddsub\.h r6,r10:t,r5:t

[0-9a-f]* <psubadd_h>:
 *[0-9a-f]*:	fe 0f 21 4f 	psubadd\.h pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 21 7c 	psubadd\.h r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 21 75 	psubadd\.h r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 21 44 	psubadd\.h r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 21 7e 	psubadd\.h lr,lr:t,lr:t
 *[0-9a-f]*:	f6 08 21 79 	psubadd\.h r9,r11:t,r8:t
 *[0-9a-f]*:	ee 0e 21 7a 	psubadd\.h r10,r7:t,lr:t
 *[0-9a-f]*:	fe 0f 21 66 	psubadd\.h r6,pc:t,pc:b

[0-9a-f]* <paddsubs_sh>:
 *[0-9a-f]*:	fe 0f 21 8f 	paddsubs\.sh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 21 bc 	paddsubs\.sh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 21 b5 	paddsubs\.sh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 21 84 	paddsubs\.sh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 21 be 	paddsubs\.sh lr,lr:t,lr:t
 *[0-9a-f]*:	fc 00 21 a0 	paddsubs\.sh r0,lr:t,r0:b
 *[0-9a-f]*:	e4 04 21 b9 	paddsubs\.sh r9,r2:t,r4:t
 *[0-9a-f]*:	f2 0d 21 bc 	paddsubs\.sh r12,r9:t,sp:t

[0-9a-f]* <psubadds_sh>:
 *[0-9a-f]*:	fe 0f 21 cf 	psubadds\.sh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 21 fc 	psubadds\.sh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 21 f5 	psubadds\.sh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 21 c4 	psubadds\.sh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 21 fe 	psubadds\.sh lr,lr:t,lr:t
 *[0-9a-f]*:	fc 01 21 df 	psubadds\.sh pc,lr:b,r1:t
 *[0-9a-f]*:	e6 0c 21 cb 	psubadds\.sh r11,r3:b,r12:b
 *[0-9a-f]*:	e4 08 21 fa 	psubadds\.sh r10,r2:t,r8:t

[0-9a-f]* <paddsubs_uh>:
 *[0-9a-f]*:	fe 0f 22 0f 	paddsubs\.uh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 22 3c 	paddsubs\.uh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 22 35 	paddsubs\.uh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 22 04 	paddsubs\.uh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 22 3e 	paddsubs\.uh lr,lr:t,lr:t
 *[0-9a-f]*:	e4 03 22 09 	paddsubs\.uh r9,r2:b,r3:b
 *[0-9a-f]*:	fa 07 22 1d 	paddsubs\.uh sp,sp:b,r7:t
 *[0-9a-f]*:	e0 0a 22 1e 	paddsubs\.uh lr,r0:b,r10:t

[0-9a-f]* <psubadds_uh>:
 *[0-9a-f]*:	fe 0f 22 4f 	psubadds\.uh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 22 7c 	psubadds\.uh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 22 75 	psubadds\.uh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 22 44 	psubadds\.uh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 22 7e 	psubadds\.uh lr,lr:t,lr:t
 *[0-9a-f]*:	f2 0f 22 7c 	psubadds\.uh r12,r9:t,pc:t
 *[0-9a-f]*:	ec 08 22 48 	psubadds\.uh r8,r6:b,r8:b
 *[0-9a-f]*:	f0 04 22 48 	psubadds\.uh r8,r8:b,r4:b

[0-9a-f]* <paddsubh_sh>:
 *[0-9a-f]*:	fe 0f 22 8f 	paddsubh\.sh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 22 bc 	paddsubh\.sh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 22 b5 	paddsubh\.sh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 22 84 	paddsubh\.sh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 22 be 	paddsubh\.sh lr,lr:t,lr:t
 *[0-9a-f]*:	f2 09 22 a8 	paddsubh\.sh r8,r9:t,r9:b
 *[0-9a-f]*:	fa 01 22 b0 	paddsubh\.sh r0,sp:t,r1:t
 *[0-9a-f]*:	e2 00 22 93 	paddsubh\.sh r3,r1:b,r0:t

[0-9a-f]* <psubaddh_sh>:
 *[0-9a-f]*:	fe 0f 22 cf 	psubaddh\.sh pc,pc:b,pc:b
 *[0-9a-f]*:	f8 0c 22 fc 	psubaddh\.sh r12,r12:t,r12:t
 *[0-9a-f]*:	ea 05 22 f5 	psubaddh\.sh r5,r5:t,r5:t
 *[0-9a-f]*:	e8 04 22 c4 	psubaddh\.sh r4,r4:b,r4:b
 *[0-9a-f]*:	fc 0e 22 fe 	psubaddh\.sh lr,lr:t,lr:t
 *[0-9a-f]*:	e6 0a 22 e7 	psubaddh\.sh r7,r3:t,r10:b
 *[0-9a-f]*:	e4 01 22 f7 	psubaddh\.sh r7,r2:t,r1:t
 *[0-9a-f]*:	e6 06 22 cb 	psubaddh\.sh r11,r3:b,r6:b

[0-9a-f]* <padd_b>:
 *[0-9a-f]*:	fe 0f 23 0f 	padd\.b pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 0c 	padd\.b r12,r12,r12
 *[0-9a-f]*:	ea 05 23 05 	padd\.b r5,r5,r5
 *[0-9a-f]*:	e8 04 23 04 	padd\.b r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 0e 	padd\.b lr,lr,lr
 *[0-9a-f]*:	ec 0f 23 02 	padd\.b r2,r6,pc
 *[0-9a-f]*:	f2 0c 23 08 	padd\.b r8,r9,r12
 *[0-9a-f]*:	f8 03 23 05 	padd\.b r5,r12,r3

[0-9a-f]* <psub_b>:
 *[0-9a-f]*:	fe 0f 23 1f 	psub\.b pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 1c 	psub\.b r12,r12,r12
 *[0-9a-f]*:	ea 05 23 15 	psub\.b r5,r5,r5
 *[0-9a-f]*:	e8 04 23 14 	psub\.b r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 1e 	psub\.b lr,lr,lr
 *[0-9a-f]*:	f8 0f 23 10 	psub\.b r0,r12,pc
 *[0-9a-f]*:	fa 0a 23 17 	psub\.b r7,sp,r10
 *[0-9a-f]*:	fa 0c 23 15 	psub\.b r5,sp,r12

[0-9a-f]* <padds_sb>:
 *[0-9a-f]*:	fe 0f 23 2f 	padds\.sb pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 2c 	padds\.sb r12,r12,r12
 *[0-9a-f]*:	ea 05 23 25 	padds\.sb r5,r5,r5
 *[0-9a-f]*:	e8 04 23 24 	padds\.sb r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 2e 	padds\.sb lr,lr,lr
 *[0-9a-f]*:	f6 04 23 2d 	padds\.sb sp,r11,r4
 *[0-9a-f]*:	f4 0b 23 2b 	padds\.sb r11,r10,r11
 *[0-9a-f]*:	f8 06 23 25 	padds\.sb r5,r12,r6

[0-9a-f]* <psubs_sb>:
 *[0-9a-f]*:	fe 0f 23 3f 	psubs\.sb pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 3c 	psubs\.sb r12,r12,r12
 *[0-9a-f]*:	ea 05 23 35 	psubs\.sb r5,r5,r5
 *[0-9a-f]*:	e8 04 23 34 	psubs\.sb r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 3e 	psubs\.sb lr,lr,lr
 *[0-9a-f]*:	ec 08 23 37 	psubs\.sb r7,r6,r8
 *[0-9a-f]*:	f4 09 23 3c 	psubs\.sb r12,r10,r9
 *[0-9a-f]*:	f6 00 23 3f 	psubs\.sb pc,r11,r0

[0-9a-f]* <padds_ub>:
 *[0-9a-f]*:	fe 0f 23 4f 	padds\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 4c 	padds\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 45 	padds\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 44 	padds\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 4e 	padds\.ub lr,lr,lr
 *[0-9a-f]*:	e4 0b 23 43 	padds\.ub r3,r2,r11
 *[0-9a-f]*:	f0 01 23 4a 	padds\.ub r10,r8,r1
 *[0-9a-f]*:	f0 0a 23 4b 	padds\.ub r11,r8,r10

[0-9a-f]* <psubs_ub>:
 *[0-9a-f]*:	fe 0f 23 5f 	psubs\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 5c 	psubs\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 55 	psubs\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 54 	psubs\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 5e 	psubs\.ub lr,lr,lr
 *[0-9a-f]*:	e4 07 23 50 	psubs\.ub r0,r2,r7
 *[0-9a-f]*:	ea 03 23 5e 	psubs\.ub lr,r5,r3
 *[0-9a-f]*:	ee 09 23 56 	psubs\.ub r6,r7,r9

[0-9a-f]* <paddh_ub>:
 *[0-9a-f]*:	fe 0f 23 6f 	paddh\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 6c 	paddh\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 65 	paddh\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 64 	paddh\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 6e 	paddh\.ub lr,lr,lr
 *[0-9a-f]*:	e2 00 23 6e 	paddh\.ub lr,r1,r0
 *[0-9a-f]*:	ee 07 23 62 	paddh\.ub r2,r7,r7
 *[0-9a-f]*:	e2 02 23 62 	paddh\.ub r2,r1,r2

[0-9a-f]* <psubh_ub>:
 *[0-9a-f]*:	fe 0f 23 7f 	psubh\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 7c 	psubh\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 75 	psubh\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 74 	psubh\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 7e 	psubh\.ub lr,lr,lr
 *[0-9a-f]*:	e2 06 23 70 	psubh\.ub r0,r1,r6
 *[0-9a-f]*:	fc 0a 23 74 	psubh\.ub r4,lr,r10
 *[0-9a-f]*:	f0 01 23 79 	psubh\.ub r9,r8,r1

[0-9a-f]* <pmax_ub>:
 *[0-9a-f]*:	fe 0f 23 8f 	pmax\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 8c 	pmax\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 85 	pmax\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 84 	pmax\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 8e 	pmax\.ub lr,lr,lr
 *[0-9a-f]*:	e4 0b 23 8f 	pmax\.ub pc,r2,r11
 *[0-9a-f]*:	e2 01 23 8c 	pmax\.ub r12,r1,r1
 *[0-9a-f]*:	e4 00 23 85 	pmax\.ub r5,r2,r0

[0-9a-f]* <pmax_sh>:
 *[0-9a-f]*:	fe 0f 23 9f 	pmax\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 9c 	pmax\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 23 95 	pmax\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 23 94 	pmax\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 9e 	pmax\.sh lr,lr,lr
 *[0-9a-f]*:	ec 0c 23 9e 	pmax\.sh lr,r6,r12
 *[0-9a-f]*:	fe 05 23 92 	pmax\.sh r2,pc,r5
 *[0-9a-f]*:	e4 07 23 9f 	pmax\.sh pc,r2,r7

[0-9a-f]* <pmin_ub>:
 *[0-9a-f]*:	fe 0f 23 af 	pmin\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 ac 	pmin\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 a5 	pmin\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 a4 	pmin\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 ae 	pmin\.ub lr,lr,lr
 *[0-9a-f]*:	e2 05 23 a8 	pmin\.ub r8,r1,r5
 *[0-9a-f]*:	f0 03 23 a1 	pmin\.ub r1,r8,r3
 *[0-9a-f]*:	e4 07 23 a0 	pmin\.ub r0,r2,r7

[0-9a-f]* <pmin_sh>:
 *[0-9a-f]*:	fe 0f 23 bf 	pmin\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 bc 	pmin\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 23 b5 	pmin\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 23 b4 	pmin\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 be 	pmin\.sh lr,lr,lr
 *[0-9a-f]*:	e8 0a 23 b8 	pmin\.sh r8,r4,r10
 *[0-9a-f]*:	f4 0c 23 be 	pmin\.sh lr,r10,r12
 *[0-9a-f]*:	ec 02 23 b2 	pmin\.sh r2,r6,r2

[0-9a-f]* <pavg_ub>:
 *[0-9a-f]*:	fe 0f 23 cf 	pavg\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 cc 	pavg\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 23 c5 	pavg\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 23 c4 	pavg\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 ce 	pavg\.ub lr,lr,lr
 *[0-9a-f]*:	e2 06 23 c0 	pavg\.ub r0,r1,r6
 *[0-9a-f]*:	e6 06 23 c8 	pavg\.ub r8,r3,r6
 *[0-9a-f]*:	f8 0a 23 cf 	pavg\.ub pc,r12,r10

[0-9a-f]* <pavg_sh>:
 *[0-9a-f]*:	fe 0f 23 df 	pavg\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 23 dc 	pavg\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 23 d5 	pavg\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 23 d4 	pavg\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 23 de 	pavg\.sh lr,lr,lr
 *[0-9a-f]*:	fe 0d 23 d9 	pavg\.sh r9,pc,sp
 *[0-9a-f]*:	fa 03 23 df 	pavg\.sh pc,sp,r3
 *[0-9a-f]*:	e2 09 23 d6 	pavg\.sh r6,r1,r9

[0-9a-f]* <pabs_sb>:
 *[0-9a-f]*:	e0 0f 23 ef 	pabs\.sb pc,pc
 *[0-9a-f]*:	e0 0c 23 ec 	pabs\.sb r12,r12
 *[0-9a-f]*:	e0 05 23 e5 	pabs\.sb r5,r5
 *[0-9a-f]*:	e0 04 23 e4 	pabs\.sb r4,r4
 *[0-9a-f]*:	e0 0e 23 ee 	pabs\.sb lr,lr
 *[0-9a-f]*:	e0 06 23 eb 	pabs\.sb r11,r6
 *[0-9a-f]*:	e0 09 23 ee 	pabs\.sb lr,r9
 *[0-9a-f]*:	e0 07 23 ed 	pabs\.sb sp,r7

[0-9a-f]* <pabs_sh>:
 *[0-9a-f]*:	e0 0f 23 ff 	pabs\.sh pc,pc
 *[0-9a-f]*:	e0 0c 23 fc 	pabs\.sh r12,r12
 *[0-9a-f]*:	e0 05 23 f5 	pabs\.sh r5,r5
 *[0-9a-f]*:	e0 04 23 f4 	pabs\.sh r4,r4
 *[0-9a-f]*:	e0 0e 23 fe 	pabs\.sh lr,lr
 *[0-9a-f]*:	e0 03 23 ff 	pabs\.sh pc,r3
 *[0-9a-f]*:	e0 07 23 f5 	pabs\.sh r5,r7
 *[0-9a-f]*:	e0 00 23 f4 	pabs\.sh r4,r0

[0-9a-f]* <psad>:
 *[0-9a-f]*:	fe 0f 24 0f 	psad pc,pc,pc
 *[0-9a-f]*:	f8 0c 24 0c 	psad r12,r12,r12
 *[0-9a-f]*:	ea 05 24 05 	psad r5,r5,r5
 *[0-9a-f]*:	e8 04 24 04 	psad r4,r4,r4
 *[0-9a-f]*:	fc 0e 24 0e 	psad lr,lr,lr
 *[0-9a-f]*:	f6 0b 24 09 	psad r9,r11,r11
 *[0-9a-f]*:	e8 0d 24 0e 	psad lr,r4,sp
 *[0-9a-f]*:	e8 05 24 0e 	psad lr,r4,r5

[0-9a-f]* <pasr_b>:
 *[0-9a-f]*:	fe 00 24 1f 	pasr\.b pc,pc,0x0
 *[0-9a-f]*:	f8 07 24 1c 	pasr\.b r12,r12,0x7
 *[0-9a-f]*:	ea 04 24 15 	pasr\.b r5,r5,0x4
 *[0-9a-f]*:	e8 03 24 14 	pasr\.b r4,r4,0x3
 *[0-9a-f]*:	fc 01 24 1e 	pasr\.b lr,lr,0x1
 *[0-9a-f]*:	ee 01 24 1f 	pasr\.b pc,r7,0x1
 *[0-9a-f]*:	fc 06 24 1d 	pasr\.b sp,lr,0x6
 *[0-9a-f]*:	e6 02 24 1d 	pasr\.b sp,r3,0x2

[0-9a-f]* <plsl_b>:
 *[0-9a-f]*:	fe 00 24 2f 	plsl\.b pc,pc,0x0
 *[0-9a-f]*:	f8 07 24 2c 	plsl\.b r12,r12,0x7
 *[0-9a-f]*:	ea 04 24 25 	plsl\.b r5,r5,0x4
 *[0-9a-f]*:	e8 03 24 24 	plsl\.b r4,r4,0x3
 *[0-9a-f]*:	fc 01 24 2e 	plsl\.b lr,lr,0x1
 *[0-9a-f]*:	f6 04 24 22 	plsl\.b r2,r11,0x4
 *[0-9a-f]*:	ea 07 24 28 	plsl\.b r8,r5,0x7
 *[0-9a-f]*:	e0 02 24 2f 	plsl\.b pc,r0,0x2

[0-9a-f]* <plsr_b>:
 *[0-9a-f]*:	fe 00 24 3f 	plsr\.b pc,pc,0x0
 *[0-9a-f]*:	f8 07 24 3c 	plsr\.b r12,r12,0x7
 *[0-9a-f]*:	ea 04 24 35 	plsr\.b r5,r5,0x4
 *[0-9a-f]*:	e8 03 24 34 	plsr\.b r4,r4,0x3
 *[0-9a-f]*:	fc 01 24 3e 	plsr\.b lr,lr,0x1
 *[0-9a-f]*:	e2 02 24 3c 	plsr\.b r12,r1,0x2
 *[0-9a-f]*:	fe 07 24 36 	plsr\.b r6,pc,0x7
 *[0-9a-f]*:	f6 02 24 3c 	plsr\.b r12,r11,0x2

[0-9a-f]* <pasr_h>:
 *[0-9a-f]*:	fe 00 24 4f 	pasr\.h pc,pc,0x0
 *[0-9a-f]*:	f8 0f 24 4c 	pasr\.h r12,r12,0xf
 *[0-9a-f]*:	ea 08 24 45 	pasr\.h r5,r5,0x8
 *[0-9a-f]*:	e8 07 24 44 	pasr\.h r4,r4,0x7
 *[0-9a-f]*:	fc 01 24 4e 	pasr\.h lr,lr,0x1
 *[0-9a-f]*:	f6 0a 24 40 	pasr\.h r0,r11,0xa
 *[0-9a-f]*:	ec 08 24 44 	pasr\.h r4,r6,0x8
 *[0-9a-f]*:	e4 04 24 46 	pasr\.h r6,r2,0x4

[0-9a-f]* <plsl_h>:
 *[0-9a-f]*:	fe 00 24 5f 	plsl\.h pc,pc,0x0
 *[0-9a-f]*:	f8 0f 24 5c 	plsl\.h r12,r12,0xf
 *[0-9a-f]*:	ea 08 24 55 	plsl\.h r5,r5,0x8
 *[0-9a-f]*:	e8 07 24 54 	plsl\.h r4,r4,0x7
 *[0-9a-f]*:	fc 01 24 5e 	plsl\.h lr,lr,0x1
 *[0-9a-f]*:	f4 09 24 55 	plsl\.h r5,r10,0x9
 *[0-9a-f]*:	fc 08 24 5d 	plsl\.h sp,lr,0x8
 *[0-9a-f]*:	fc 07 24 50 	plsl\.h r0,lr,0x7

[0-9a-f]* <plsr_h>:
 *[0-9a-f]*:	fe 00 24 6f 	plsr\.h pc,pc,0x0
 *[0-9a-f]*:	f8 0f 24 6c 	plsr\.h r12,r12,0xf
 *[0-9a-f]*:	ea 08 24 65 	plsr\.h r5,r5,0x8
 *[0-9a-f]*:	e8 07 24 64 	plsr\.h r4,r4,0x7
 *[0-9a-f]*:	fc 01 24 6e 	plsr\.h lr,lr,0x1
 *[0-9a-f]*:	e0 0f 24 6b 	plsr\.h r11,r0,0xf
 *[0-9a-f]*:	e6 03 24 6e 	plsr\.h lr,r3,0x3
 *[0-9a-f]*:	fc 0a 24 68 	plsr\.h r8,lr,0xa

[0-9a-f]* <packw_sh>:
 *[0-9a-f]*:	fe 0f 24 7f 	packw\.sh pc,pc,pc
 *[0-9a-f]*:	f8 0c 24 7c 	packw\.sh r12,r12,r12
 *[0-9a-f]*:	ea 05 24 75 	packw\.sh r5,r5,r5
 *[0-9a-f]*:	e8 04 24 74 	packw\.sh r4,r4,r4
 *[0-9a-f]*:	fc 0e 24 7e 	packw\.sh lr,lr,lr
 *[0-9a-f]*:	f6 0a 24 7d 	packw\.sh sp,r11,r10
 *[0-9a-f]*:	e4 0c 24 78 	packw\.sh r8,r2,r12
 *[0-9a-f]*:	e2 05 24 78 	packw\.sh r8,r1,r5

[0-9a-f]* <punpckub_h>:
 *[0-9a-f]*:	fe 00 24 8f 	punpckub\.h pc,pc:b
 *[0-9a-f]*:	f8 00 24 9c 	punpckub\.h r12,r12:t
 *[0-9a-f]*:	ea 00 24 95 	punpckub\.h r5,r5:t
 *[0-9a-f]*:	e8 00 24 84 	punpckub\.h r4,r4:b
 *[0-9a-f]*:	fc 00 24 9e 	punpckub\.h lr,lr:t
 *[0-9a-f]*:	e2 00 24 96 	punpckub\.h r6,r1:t
 *[0-9a-f]*:	ea 00 24 8e 	punpckub\.h lr,r5:b
 *[0-9a-f]*:	e4 00 24 9e 	punpckub\.h lr,r2:t

[0-9a-f]* <punpcksb_h>:
 *[0-9a-f]*:	fe 00 24 af 	punpcksb\.h pc,pc:b
 *[0-9a-f]*:	f8 00 24 bc 	punpcksb\.h r12,r12:t
 *[0-9a-f]*:	ea 00 24 b5 	punpcksb\.h r5,r5:t
 *[0-9a-f]*:	e8 00 24 a4 	punpcksb\.h r4,r4:b
 *[0-9a-f]*:	fc 00 24 be 	punpcksb\.h lr,lr:t
 *[0-9a-f]*:	ee 00 24 b4 	punpcksb\.h r4,r7:t
 *[0-9a-f]*:	fc 00 24 a6 	punpcksb\.h r6,lr:b
 *[0-9a-f]*:	f8 00 24 bc 	punpcksb\.h r12,r12:t

[0-9a-f]* <packsh_ub>:
 *[0-9a-f]*:	fe 0f 24 cf 	packsh\.ub pc,pc,pc
 *[0-9a-f]*:	f8 0c 24 cc 	packsh\.ub r12,r12,r12
 *[0-9a-f]*:	ea 05 24 c5 	packsh\.ub r5,r5,r5
 *[0-9a-f]*:	e8 04 24 c4 	packsh\.ub r4,r4,r4
 *[0-9a-f]*:	fc 0e 24 ce 	packsh\.ub lr,lr,lr
 *[0-9a-f]*:	ec 03 24 c3 	packsh\.ub r3,r6,r3
 *[0-9a-f]*:	e0 03 24 c8 	packsh\.ub r8,r0,r3
 *[0-9a-f]*:	e6 0e 24 c9 	packsh\.ub r9,r3,lr

[0-9a-f]* <packsh_sb>:
 *[0-9a-f]*:	fe 0f 24 df 	packsh\.sb pc,pc,pc
 *[0-9a-f]*:	f8 0c 24 dc 	packsh\.sb r12,r12,r12
 *[0-9a-f]*:	ea 05 24 d5 	packsh\.sb r5,r5,r5
 *[0-9a-f]*:	e8 04 24 d4 	packsh\.sb r4,r4,r4
 *[0-9a-f]*:	fc 0e 24 de 	packsh\.sb lr,lr,lr
 *[0-9a-f]*:	f0 01 24 d6 	packsh\.sb r6,r8,r1
 *[0-9a-f]*:	f2 08 24 de 	packsh\.sb lr,r9,r8
 *[0-9a-f]*:	ec 06 24 dd 	packsh\.sb sp,r6,r6

[0-9a-f]* <andl>:
 *[0-9a-f]*:	e0 1f 00 00 	andl pc,0x0
 *[0-9a-f]*:	e0 1c ff ff 	andl r12,0xffff
 *[0-9a-f]*:	e0 15 80 00 	andl r5,0x8000
 *[0-9a-f]*:	e0 14 7f ff 	andl r4,0x7fff
 *[0-9a-f]*:	e0 1e 00 01 	andl lr,0x1
 *[0-9a-f]*:	e0 1f 5a 58 	andl pc,0x5a58
 *[0-9a-f]*:	e0 18 b8 9e 	andl r8,0xb89e
 *[0-9a-f]*:	e0 17 35 97 	andl r7,0x3597

[0-9a-f]* <andl_coh>:
 *[0-9a-f]*:	e2 1f 00 00 	andl pc,0x0,COH
 *[0-9a-f]*:	e2 1c ff ff 	andl r12,0xffff,COH
 *[0-9a-f]*:	e2 15 80 00 	andl r5,0x8000,COH
 *[0-9a-f]*:	e2 14 7f ff 	andl r4,0x7fff,COH
 *[0-9a-f]*:	e2 1e 00 01 	andl lr,0x1,COH
 *[0-9a-f]*:	e2 16 58 e1 	andl r6,0x58e1,COH
 *[0-9a-f]*:	e2 10 9e cd 	andl r0,0x9ecd,COH
 *[0-9a-f]*:	e2 14 bd c4 	andl r4,0xbdc4,COH

[0-9a-f]* <andh>:
 *[0-9a-f]*:	e4 1f 00 00 	andh pc,0x0
 *[0-9a-f]*:	e4 1c ff ff 	andh r12,0xffff
 *[0-9a-f]*:	e4 15 80 00 	andh r5,0x8000
 *[0-9a-f]*:	e4 14 7f ff 	andh r4,0x7fff
 *[0-9a-f]*:	e4 1e 00 01 	andh lr,0x1
 *[0-9a-f]*:	e4 1c cc 58 	andh r12,0xcc58
 *[0-9a-f]*:	e4 13 21 e3 	andh r3,0x21e3
 *[0-9a-f]*:	e4 12 a7 eb 	andh r2,0xa7eb

[0-9a-f]* <andh_coh>:
 *[0-9a-f]*:	e6 1f 00 00 	andh pc,0x0,COH
 *[0-9a-f]*:	e6 1c ff ff 	andh r12,0xffff,COH
 *[0-9a-f]*:	e6 15 80 00 	andh r5,0x8000,COH
 *[0-9a-f]*:	e6 14 7f ff 	andh r4,0x7fff,COH
 *[0-9a-f]*:	e6 1e 00 01 	andh lr,0x1,COH
 *[0-9a-f]*:	e6 1b 86 0d 	andh r11,0x860d,COH
 *[0-9a-f]*:	e6 18 ce f6 	andh r8,0xcef6,COH
 *[0-9a-f]*:	e6 1a 5c 83 	andh r10,0x5c83,COH

[0-9a-f]* <orl>:
 *[0-9a-f]*:	e8 1f 00 00 	orl pc,0x0
 *[0-9a-f]*:	e8 1c ff ff 	orl r12,0xffff
 *[0-9a-f]*:	e8 15 80 00 	orl r5,0x8000
 *[0-9a-f]*:	e8 14 7f ff 	orl r4,0x7fff
 *[0-9a-f]*:	e8 1e 00 01 	orl lr,0x1
 *[0-9a-f]*:	e8 1d 41 7e 	orl sp,0x417e
 *[0-9a-f]*:	e8 10 52 bd 	orl r0,0x52bd
 *[0-9a-f]*:	e8 1f ac 47 	orl pc,0xac47

[0-9a-f]* <orh>:
 *[0-9a-f]*:	ea 1f 00 00 	orh pc,0x0
 *[0-9a-f]*:	ea 1c ff ff 	orh r12,0xffff
 *[0-9a-f]*:	ea 15 80 00 	orh r5,0x8000
 *[0-9a-f]*:	ea 14 7f ff 	orh r4,0x7fff
 *[0-9a-f]*:	ea 1e 00 01 	orh lr,0x1
 *[0-9a-f]*:	ea 18 6e 7d 	orh r8,0x6e7d
 *[0-9a-f]*:	ea 1c 77 1c 	orh r12,0x771c
 *[0-9a-f]*:	ea 11 ea 1a 	orh r1,0xea1a

[0-9a-f]* <eorl>:
 *[0-9a-f]*:	ec 1f 00 00 	eorl pc,0x0
 *[0-9a-f]*:	ec 1c ff ff 	eorl r12,0xffff
 *[0-9a-f]*:	ec 15 80 00 	eorl r5,0x8000
 *[0-9a-f]*:	ec 14 7f ff 	eorl r4,0x7fff
 *[0-9a-f]*:	ec 1e 00 01 	eorl lr,0x1
 *[0-9a-f]*:	ec 14 c7 b9 	eorl r4,0xc7b9
 *[0-9a-f]*:	ec 16 fb dd 	eorl r6,0xfbdd
 *[0-9a-f]*:	ec 11 51 b1 	eorl r1,0x51b1

[0-9a-f]* <eorh>:
 *[0-9a-f]*:	ee 1f 00 00 	eorh pc,0x0
 *[0-9a-f]*:	ee 1c ff ff 	eorh r12,0xffff
 *[0-9a-f]*:	ee 15 80 00 	eorh r5,0x8000
 *[0-9a-f]*:	ee 14 7f ff 	eorh r4,0x7fff
 *[0-9a-f]*:	ee 1e 00 01 	eorh lr,0x1
 *[0-9a-f]*:	ee 10 2d d4 	eorh r0,0x2dd4
 *[0-9a-f]*:	ee 1a 94 b5 	eorh r10,0x94b5
 *[0-9a-f]*:	ee 19 df 2a 	eorh r9,0xdf2a

[0-9a-f]* <mcall>:
 *[0-9a-f]*:	f0 1f 00 00 	mcall [0-9a-f]* <.*>
 *[0-9a-f]*:	f0 1c ff ff 	mcall r12\[-4\]
 *[0-9a-f]*:	f0 15 80 00 	mcall r5\[-131072\]
 *[0-9a-f]*:	f0 14 7f ff 	mcall r4\[131068\]
 *[0-9a-f]*:	f0 1e 00 01 	mcall lr\[4\]
 *[0-9a-f]*:	f0 1d 3b bf 	mcall sp\[61180\]
 *[0-9a-f]*:	f0 14 dd d2 	mcall r4\[-35000\]
 *[0-9a-f]*:	f0 10 09 b1 	mcall r0\[9924\]

[0-9a-f]* <pref>:
 *[0-9a-f]*:	f2 1f 00 00 	pref pc\[0\]
 *[0-9a-f]*:	f2 1c ff ff 	pref r12\[-1\]
 *[0-9a-f]*:	f2 15 80 00 	pref r5\[-32768\]
 *[0-9a-f]*:	f2 14 7f ff 	pref r4\[32767\]
 *[0-9a-f]*:	f2 1e 00 01 	pref lr\[1\]
 *[0-9a-f]*:	f2 17 1e 44 	pref r7\[7748\]
 *[0-9a-f]*:	f2 17 e1 ed 	pref r7\[-7699\]
 *[0-9a-f]*:	f2 12 9a dc 	pref r2\[-25892\]

[0-9a-f]* <cache>:
 *[0-9a-f]*:	f4 1f 00 00 	cache pc\[0\],0x0
 *[0-9a-f]*:	f4 1c ff ff 	cache r12\[-1\],0x1f
 *[0-9a-f]*:	f4 15 84 00 	cache r5\[-1024\],0x10
 *[0-9a-f]*:	f4 14 7b ff 	cache r4\[1023\],0xf
 *[0-9a-f]*:	f4 1e 08 01 	cache lr\[1\],0x1
 *[0-9a-f]*:	f4 13 8c 3c 	cache r3\[-964\],0x11
 *[0-9a-f]*:	f4 14 b6 89 	cache r4\[-375\],0x16
 *[0-9a-f]*:	f4 13 8c 88 	cache r3\[-888\],0x11

[0-9a-f]* <sub4>:
 *[0-9a-f]*:	20 0f       	sub pc,0
 *[0-9a-f]*:	2f fc       	sub r12,-1
 *[0-9a-f]*:	f0 25 00 00 	sub r5,-1048576
 *[0-9a-f]*:	ee 34 ff ff 	sub r4,1048575
 *[0-9a-f]*:	20 1e       	sub lr,1
 *[0-9a-f]*:	f6 22 8d 6c 	sub r2,-619156
 *[0-9a-f]*:	e6 3e 0a cd 	sub lr,461517
 *[0-9a-f]*:	fc 38 2d 25 	sub r8,-185051

[0-9a-f]* <cp3>:
 *[0-9a-f]*:	58 0f       	cp.w pc,0
 *[0-9a-f]*:	5b fc       	cp.w r12,-1
 *[0-9a-f]*:	f0 45 00 00 	cp.w r5,-1048576
 *[0-9a-f]*:	ee 54 ff ff 	cp.w r4,1048575
 *[0-9a-f]*:	58 1e       	cp.w lr,1
 *[0-9a-f]*:	e0 51 e4 ae 	cp.w r1,124078
 *[0-9a-f]*:	fa 40 37 e3 	cp.w r0,-378909
 *[0-9a-f]*:	fc 44 4a 14 	cp.w r4,-243180

[0-9a-f]* <mov2>:
 *[0-9a-f]*:	30 0f       	mov pc,0
 *[0-9a-f]*:	3f fc       	mov r12,-1
 *[0-9a-f]*:	f0 65 00 00 	mov r5,-1048576
 *[0-9a-f]*:	ee 74 ff ff 	mov r4,1048575
 *[0-9a-f]*:	30 1e       	mov lr,1
 *[0-9a-f]*:	fa 75 29 a3 	mov r5,-317021
 *[0-9a-f]*:	f4 6d 91 94 	mov sp,-749164
 *[0-9a-f]*:	ee 65 58 93 	mov r5,940179

[0-9a-f]* <brc2>:
 *[0-9a-f]*:	c0 00       	breq [0-9a-f]* <.*>
 *[0-9a-f]*:	fe 9f ff ff 	bral [0-9a-f]* <.*>
 *[0-9a-f]*:	f0 88 00 00 	brls [0-9a-f]* <.*>
 *[0-9a-f]*:	ee 97 ff ff 	brpl [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 11       	brne [0-9a-f]* <.*>
 *[0-9a-f]*:	f2 8b 4a 4d 	brhi [0-9a-f]* <.*>
 *[0-9a-f]*:	ea 8e 14 cc 	brqs [0-9a-f]* <.*>
 *[0-9a-f]*:	fa 98 98 33 	brls [0-9a-f]* <.*>

[0-9a-f]* <rcall2>:
 *[0-9a-f]*:	c0 0c       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	cf ff       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	f0 a0 00 00 	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	ee b0 ff ff 	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	c0 1c       	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	e2 b0 ca 5a 	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	e8 a0 47 52 	rcall [0-9a-f]* <.*>
 *[0-9a-f]*:	fe b0 fd ef 	rcall [0-9a-f]* <.*>

[0-9a-f]* <sub5>:
 *[0-9a-f]*:	fe cf 00 00 	sub pc,pc,0
 *[0-9a-f]*:	f8 cc ff ff 	sub r12,r12,-1
 *[0-9a-f]*:	ea c5 80 00 	sub r5,r5,-32768
 *[0-9a-f]*:	e8 c4 7f ff 	sub r4,r4,32767
 *[0-9a-f]*:	fc ce 00 01 	sub lr,lr,1
 *[0-9a-f]*:	fe cf ce 38 	sub pc,pc,-12744
 *[0-9a-f]*:	ee c7 95 1b 	sub r7,r7,-27365
 *[0-9a-f]*:	f2 c2 bc 32 	sub r2,r9,-17358

[0-9a-f]* <satsub_w2>:
 *[0-9a-f]*:	fe df 00 00 	satsub\.w pc,pc,0
 *[0-9a-f]*:	f8 dc ff ff 	satsub\.w r12,r12,-1
 *[0-9a-f]*:	ea d5 80 00 	satsub\.w r5,r5,-32768
 *[0-9a-f]*:	e8 d4 7f ff 	satsub\.w r4,r4,32767
 *[0-9a-f]*:	fc de 00 01 	satsub\.w lr,lr,1
 *[0-9a-f]*:	fc d2 f8 29 	satsub\.w r2,lr,-2007
 *[0-9a-f]*:	f8 d7 fc f0 	satsub\.w r7,r12,-784
 *[0-9a-f]*:	ee d4 5a 8c 	satsub\.w r4,r7,23180

[0-9a-f]* <ld_d4>:
 *[0-9a-f]*:	fe e0 00 00 	ld\.d r0,pc\[0\]
 *[0-9a-f]*:	f8 ee ff ff 	ld\.d lr,r12\[-1\]
 *[0-9a-f]*:	ea e8 80 00 	ld\.d r8,r5\[-32768\]
 *[0-9a-f]*:	e8 e6 7f ff 	ld\.d r6,r4\[32767\]
 *[0-9a-f]*:	fc e2 00 01 	ld\.d r2,lr\[1\]
 *[0-9a-f]*:	f6 ee 39 c0 	ld\.d lr,r11\[14784\]
 *[0-9a-f]*:	f2 e6 b6 27 	ld\.d r6,r9\[-18905\]
 *[0-9a-f]*:	e6 e2 e7 2d 	ld\.d r2,r3\[-6355\]

[0-9a-f]* <ld_w4>:
 *[0-9a-f]*:	7e 0f       	ld\.w pc,pc\[0x0\]
 *[0-9a-f]*:	f8 fc ff ff 	ld\.w r12,r12\[-1\]
 *[0-9a-f]*:	ea f5 80 00 	ld\.w r5,r5\[-32768\]
 *[0-9a-f]*:	e8 f4 7f ff 	ld\.w r4,r4\[32767\]
 *[0-9a-f]*:	fc fe 00 01 	ld\.w lr,lr\[1\]
 *[0-9a-f]*:	f8 f0 a9 8b 	ld\.w r0,r12\[-22133\]
 *[0-9a-f]*:	fe fd af d7 	ld\.w sp,pc\[-20521\]
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <ld_sh4>:
 *[0-9a-f]*:	9e 0f       	ld\.sh pc,pc\[0x0\]
 *[0-9a-f]*:	f9 0c ff ff 	ld\.sh r12,r12\[-1\]
 *[0-9a-f]*:	eb 05 80 00 	ld\.sh r5,r5\[-32768\]
 *[0-9a-f]*:	e9 04 7f ff 	ld\.sh r4,r4\[32767\]
 *[0-9a-f]*:	fd 0e 00 01 	ld\.sh lr,lr\[1\]
 *[0-9a-f]*:	f5 06 78 d2 	ld\.sh r6,r10\[30930\]
 *[0-9a-f]*:	f5 06 55 d5 	ld\.sh r6,r10\[21973\]
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <ld_uh4>:
 *[0-9a-f]*:	9e 8f       	ld\.uh pc,pc\[0x0\]
 *[0-9a-f]*:	f9 1c ff ff 	ld\.uh r12,r12\[-1\]
 *[0-9a-f]*:	eb 15 80 00 	ld\.uh r5,r5\[-32768\]
 *[0-9a-f]*:	e9 14 7f ff 	ld\.uh r4,r4\[32767\]
 *[0-9a-f]*:	fd 1e 00 01 	ld\.uh lr,lr\[1\]
 *[0-9a-f]*:	f3 11 cb d6 	ld\.uh r1,r9\[-13354\]
 *[0-9a-f]*:	f7 1e 53 59 	ld\.uh lr,r11\[21337\]
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <ld_sb1>:
 *[0-9a-f]*:	ff 2f 00 00 	ld\.sb pc,pc\[0\]
 *[0-9a-f]*:	f9 2c ff ff 	ld\.sb r12,r12\[-1\]
 *[0-9a-f]*:	eb 25 80 00 	ld\.sb r5,r5\[-32768\]
 *[0-9a-f]*:	e9 24 7f ff 	ld\.sb r4,r4\[32767\]
 *[0-9a-f]*:	fd 2e 00 01 	ld\.sb lr,lr\[1\]
 *[0-9a-f]*:	fb 27 90 09 	ld\.sb r7,sp\[-28663\]
 *[0-9a-f]*:	e3 22 e9 09 	ld\.sb r2,r1\[-5879\]
 *[0-9a-f]*:	e7 2c 49 2e 	ld\.sb r12,r3\[18734\]

[0-9a-f]* <ld_ub4>:
 *[0-9a-f]*:	1f 8f       	ld\.ub pc,pc\[0x0\]
 *[0-9a-f]*:	f9 3c ff ff 	ld\.ub r12,r12\[-1\]
 *[0-9a-f]*:	eb 35 80 00 	ld\.ub r5,r5\[-32768\]
 *[0-9a-f]*:	e9 34 7f ff 	ld\.ub r4,r4\[32767\]
 *[0-9a-f]*:	1d 9e       	ld\.ub lr,lr\[0x1\]
 *[0-9a-f]*:	e9 3f 20 55 	ld\.ub pc,r4\[8277\]
 *[0-9a-f]*:	f9 35 4a e4 	ld\.ub r5,r12\[19172\]
 *[0-9a-f]*:	fd 3a 66 eb 	ld\.ub r10,lr\[26347\]

[0-9a-f]* <st_d4>:
 *[0-9a-f]*:	fe e1 00 00 	st\.d pc\[0\],r0
 *[0-9a-f]*:	f8 ef ff ff 	st\.d r12\[-1\],lr
 *[0-9a-f]*:	ea e9 80 00 	st\.d r5\[-32768\],r8
 *[0-9a-f]*:	e8 e7 7f ff 	st\.d r4\[32767\],r6
 *[0-9a-f]*:	fc e3 00 01 	st\.d lr\[1\],r2
 *[0-9a-f]*:	ea eb 33 90 	st\.d r5\[13200\],r10
 *[0-9a-f]*:	ea eb 24 88 	st\.d r5\[9352\],r10
 *[0-9a-f]*:	ea e5 7e 75 	st\.d r5\[32373\],r4

[0-9a-f]* <st_w4>:
 *[0-9a-f]*:	9f 0f       	st\.w pc\[0x0\],pc
 *[0-9a-f]*:	f9 4c ff ff 	st\.w r12\[-1\],r12
 *[0-9a-f]*:	eb 45 80 00 	st\.w r5\[-32768\],r5
 *[0-9a-f]*:	e9 44 7f ff 	st\.w r4\[32767\],r4
 *[0-9a-f]*:	fd 4e 00 01 	st\.w lr\[1\],lr
 *[0-9a-f]*:	fb 47 17 f8 	st\.w sp\[6136\],r7
 *[0-9a-f]*:	ed 4c 69 cf 	st\.w r6\[27087\],r12
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <st_h4>:
 *[0-9a-f]*:	be 0f       	st\.h pc\[0x0\],pc
 *[0-9a-f]*:	f9 5c ff ff 	st\.h r12\[-1\],r12
 *[0-9a-f]*:	eb 55 80 00 	st\.h r5\[-32768\],r5
 *[0-9a-f]*:	e9 54 7f ff 	st\.h r4\[32767\],r4
 *[0-9a-f]*:	fd 5e 00 01 	st\.h lr\[1\],lr
 *[0-9a-f]*:	e9 57 d9 16 	st\.h r4\[-9962\],r7
 *[0-9a-f]*:	f3 53 c0 86 	st\.h r9\[-16250\],r3
 *[0-9a-f]*:	d7 03       	nop

[0-9a-f]* <st_b4>:
 *[0-9a-f]*:	be 8f       	st\.b pc\[0x0\],pc
 *[0-9a-f]*:	f9 6c ff ff 	st\.b r12\[-1\],r12
 *[0-9a-f]*:	eb 65 80 00 	st\.b r5\[-32768\],r5
 *[0-9a-f]*:	e9 64 7f ff 	st\.b r4\[32767\],r4
 *[0-9a-f]*:	bc 9e       	st\.b lr\[0x1\],lr
 *[0-9a-f]*:	f9 66 75 96 	st\.b r12\[30102\],r6
 *[0-9a-f]*:	eb 61 71 31 	st\.b r5\[28977\],r1
 *[0-9a-f]*:	e1 61 15 5e 	st\.b r0\[5470\],r1

[0-9a-f]* <mfsr>:
 *[0-9a-f]*:	e1 bf 00 00 	mfsr pc,0x0
 *[0-9a-f]*:	e1 bc 00 ff 	mfsr r12,0x3fc
 *[0-9a-f]*:	e1 b5 00 80 	mfsr r5,0x200
 *[0-9a-f]*:	e1 b4 00 7f 	mfsr r4,0x1fc
 *[0-9a-f]*:	e1 be 00 01 	mfsr lr,0x4
 *[0-9a-f]*:	e1 b2 00 ae 	mfsr r2,0x2b8
 *[0-9a-f]*:	e1 b4 00 41 	mfsr r4,0x104
 *[0-9a-f]*:	e1 ba 00 fe 	mfsr r10,0x3f8

[0-9a-f]* <mtsr>:
 *[0-9a-f]*:	e3 bf 00 00 	mtsr 0x0,pc
 *[0-9a-f]*:	e3 bc 00 ff 	mtsr 0x3fc,r12
 *[0-9a-f]*:	e3 b5 00 80 	mtsr 0x200,r5
 *[0-9a-f]*:	e3 b4 00 7f 	mtsr 0x1fc,r4
 *[0-9a-f]*:	e3 be 00 01 	mtsr 0x4,lr
 *[0-9a-f]*:	e3 ba 00 38 	mtsr 0xe0,r10
 *[0-9a-f]*:	e3 bc 00 d1 	mtsr 0x344,r12
 *[0-9a-f]*:	e3 b9 00 4c 	mtsr 0x130,r9

[0-9a-f]* <mfdr>:
 *[0-9a-f]*:	e5 bf 00 00 	mfdr pc,0x0
 *[0-9a-f]*:	e5 bc 00 ff 	mfdr r12,0x3fc
 *[0-9a-f]*:	e5 b5 00 80 	mfdr r5,0x200
 *[0-9a-f]*:	e5 b4 00 7f 	mfdr r4,0x1fc
 *[0-9a-f]*:	e5 be 00 01 	mfdr lr,0x4
 *[0-9a-f]*:	e5 b6 00 e9 	mfdr r6,0x3a4
 *[0-9a-f]*:	e5 b5 00 09 	mfdr r5,0x24
 *[0-9a-f]*:	e5 b9 00 4b 	mfdr r9,0x12c

[0-9a-f]* <mtdr>:
 *[0-9a-f]*:	e7 bf 00 00 	mtdr 0x0,pc
 *[0-9a-f]*:	e7 bc 00 ff 	mtdr 0x3fc,r12
 *[0-9a-f]*:	e7 b5 00 80 	mtdr 0x200,r5
 *[0-9a-f]*:	e7 b4 00 7f 	mtdr 0x1fc,r4
 *[0-9a-f]*:	e7 be 00 01 	mtdr 0x4,lr
 *[0-9a-f]*:	e7 b8 00 2d 	mtdr 0xb4,r8
 *[0-9a-f]*:	e7 ba 00 b4 	mtdr 0x2d0,r10
 *[0-9a-f]*:	e7 be 00 66 	mtdr 0x198,lr

[0-9a-f]* <sleep>:
 *[0-9a-f]*:	e9 b0 00 00 	sleep 0x0
 *[0-9a-f]*:	e9 b0 00 ff 	sleep 0xff
 *[0-9a-f]*:	e9 b0 00 80 	sleep 0x80
 *[0-9a-f]*:	e9 b0 00 7f 	sleep 0x7f
 *[0-9a-f]*:	e9 b0 00 01 	sleep 0x1
 *[0-9a-f]*:	e9 b0 00 fe 	sleep 0xfe
 *[0-9a-f]*:	e9 b0 00 0f 	sleep 0xf
 *[0-9a-f]*:	e9 b0 00 2b 	sleep 0x2b

[0-9a-f]* <sync>:
 *[0-9a-f]*:	eb b0 00 00 	sync 0x0
 *[0-9a-f]*:	eb b0 00 ff 	sync 0xff
 *[0-9a-f]*:	eb b0 00 80 	sync 0x80
 *[0-9a-f]*:	eb b0 00 7f 	sync 0x7f
 *[0-9a-f]*:	eb b0 00 01 	sync 0x1
 *[0-9a-f]*:	eb b0 00 a6 	sync 0xa6
 *[0-9a-f]*:	eb b0 00 e6 	sync 0xe6
 *[0-9a-f]*:	eb b0 00 b4 	sync 0xb4

[0-9a-f]* <bld>:
 *[0-9a-f]*:	ed bf 00 00 	bld pc,0x0
 *[0-9a-f]*:	ed bc 00 1f 	bld r12,0x1f
 *[0-9a-f]*:	ed b5 00 10 	bld r5,0x10
 *[0-9a-f]*:	ed b4 00 0f 	bld r4,0xf
 *[0-9a-f]*:	ed be 00 01 	bld lr,0x1
 *[0-9a-f]*:	ed b9 00 0f 	bld r9,0xf
 *[0-9a-f]*:	ed b0 00 04 	bld r0,0x4
 *[0-9a-f]*:	ed be 00 1a 	bld lr,0x1a

[0-9a-f]* <bst>:
 *[0-9a-f]*:	ef bf 00 00 	bst pc,0x0
 *[0-9a-f]*:	ef bc 00 1f 	bst r12,0x1f
 *[0-9a-f]*:	ef b5 00 10 	bst r5,0x10
 *[0-9a-f]*:	ef b4 00 0f 	bst r4,0xf
 *[0-9a-f]*:	ef be 00 01 	bst lr,0x1
 *[0-9a-f]*:	ef ba 00 1c 	bst r10,0x1c
 *[0-9a-f]*:	ef b0 00 03 	bst r0,0x3
 *[0-9a-f]*:	ef bd 00 02 	bst sp,0x2

[0-9a-f]* <sats>:
 *[0-9a-f]*:	f1 bf 00 00 	sats pc,0x0
 *[0-9a-f]*:	f1 bc 03 ff 	sats r12>>0x1f,0x1f
 *[0-9a-f]*:	f1 b5 02 10 	sats r5>>0x10,0x10
 *[0-9a-f]*:	f1 b4 01 ef 	sats r4>>0xf,0xf
 *[0-9a-f]*:	f1 be 00 21 	sats lr>>0x1,0x1
 *[0-9a-f]*:	f1 ba 02 63 	sats r10>>0x3,0x13
 *[0-9a-f]*:	f1 ba 03 42 	sats r10>>0x2,0x1a
 *[0-9a-f]*:	f1 b1 00 34 	sats r1>>0x14,0x1

[0-9a-f]* <satu>:
 *[0-9a-f]*:	f1 bf 04 00 	satu pc,0x0
 *[0-9a-f]*:	f1 bc 07 ff 	satu r12>>0x1f,0x1f
 *[0-9a-f]*:	f1 b5 06 10 	satu r5>>0x10,0x10
 *[0-9a-f]*:	f1 b4 05 ef 	satu r4>>0xf,0xf
 *[0-9a-f]*:	f1 be 04 21 	satu lr>>0x1,0x1
 *[0-9a-f]*:	f1 bf 04 e5 	satu pc>>0x5,0x7
 *[0-9a-f]*:	f1 b7 04 a5 	satu r7>>0x5,0x5
 *[0-9a-f]*:	f1 b2 06 7a 	satu r2>>0x1a,0x13

[0-9a-f]* <satrnds>:
 *[0-9a-f]*:	f3 bf 00 00 	satrnds pc,0x0
 *[0-9a-f]*:	f3 bc 03 ff 	satrnds r12>>0x1f,0x1f
 *[0-9a-f]*:	f3 b5 02 10 	satrnds r5>>0x10,0x10
 *[0-9a-f]*:	f3 b4 01 ef 	satrnds r4>>0xf,0xf
 *[0-9a-f]*:	f3 be 00 21 	satrnds lr>>0x1,0x1
 *[0-9a-f]*:	f3 b0 02 75 	satrnds r0>>0x15,0x13
 *[0-9a-f]*:	f3 bd 00 40 	satrnds sp,0x2
 *[0-9a-f]*:	f3 b7 03 a6 	satrnds r7>>0x6,0x1d

[0-9a-f]* <satrndu>:
 *[0-9a-f]*:	f3 bf 04 00 	satrndu pc,0x0
 *[0-9a-f]*:	f3 bc 07 ff 	satrndu r12>>0x1f,0x1f
 *[0-9a-f]*:	f3 b5 06 10 	satrndu r5>>0x10,0x10
 *[0-9a-f]*:	f3 b4 05 ef 	satrndu r4>>0xf,0xf
 *[0-9a-f]*:	f3 be 04 21 	satrndu lr>>0x1,0x1
 *[0-9a-f]*:	f3 bc 07 40 	satrndu r12,0x1a
 *[0-9a-f]*:	f3 b4 04 75 	satrndu r4>>0x15,0x3
 *[0-9a-f]*:	f3 ba 06 03 	satrndu r10>>0x3,0x10

[0-9a-f]* <subfc>:
 *[0-9a-f]*:	f5 bf 00 00 	subfeq pc,0
 *[0-9a-f]*:	f5 bc 0f ff 	subfal r12,-1
 *[0-9a-f]*:	f5 b5 08 80 	subfls r5,-128
 *[0-9a-f]*:	f5 b4 07 7f 	subfpl r4,127
 *[0-9a-f]*:	f5 be 01 01 	subfne lr,1
 *[0-9a-f]*:	f5 ba 08 08 	subfls r10,8
 *[0-9a-f]*:	f5 bb 0d 63 	subfvc r11,99
 *[0-9a-f]*:	f5 b2 0c 49 	subfvs r2,73

[0-9a-f]* <subc>:
 *[0-9a-f]*:	f7 bf 00 00 	subeq pc,0
 *[0-9a-f]*:	f7 bc 0f ff 	subal r12,-1
 *[0-9a-f]*:	f7 b5 08 80 	subls r5,-128
 *[0-9a-f]*:	f7 b4 07 7f 	subpl r4,127
 *[0-9a-f]*:	f7 be 01 01 	subne lr,1
 *[0-9a-f]*:	f7 bc 08 76 	subls r12,118
 *[0-9a-f]*:	f7 be 0d f4 	subvc lr,-12
 *[0-9a-f]*:	f7 b4 06 f3 	submi r4,-13

[0-9a-f]* <movc2>:
 *[0-9a-f]*:	f9 bf 00 00 	moveq pc,0
 *[0-9a-f]*:	f9 bc 0f ff 	moval r12,-1
 *[0-9a-f]*:	f9 b5 08 80 	movls r5,-128
 *[0-9a-f]*:	f9 b4 07 7f 	movpl r4,127
 *[0-9a-f]*:	f9 be 01 01 	movne lr,1
 *[0-9a-f]*:	f9 b3 05 86 	movlt r3,-122
 *[0-9a-f]*:	f9 b8 0d 02 	movvc r8,2
 *[0-9a-f]*:	f9 b7 01 91 	movne r7,-111

[0-9a-f]* <cp_b>:
 *[0-9a-f]*:	e0 0f 18 00 	cp\.b pc,r0
 *[0-9a-f]*:	fe 00 18 00 	cp\.b r0,pc
 *[0-9a-f]*:	f0 07 18 00 	cp\.b r7,r8
 *[0-9a-f]*:	ee 08 18 00 	cp\.b r8,r7

[0-9a-f]* <cp_h>:
 *[0-9a-f]*:	e0 0f 19 00 	cp\.h pc,r0
 *[0-9a-f]*:	fe 00 19 00 	cp\.h r0,pc
 *[0-9a-f]*:	f0 07 19 00 	cp\.h r7,r8
 *[0-9a-f]*:	ee 08 19 00 	cp\.h r8,r7

[0-9a-f]* <ldm>:
 *[0-9a-f]*:	e1 cf 00 7e 	ldm pc,r1-r6
 *[0-9a-f]*:	e1 cc ff ff 	ldm r12,r0-pc
 *[0-9a-f]*:	e1 c5 80 00 	ldm r5,pc
 *[0-9a-f]*:	e1 c4 7f ff 	ldm r4,r0-lr
 *[0-9a-f]*:	e1 ce 00 01 	ldm lr,r0
 *[0-9a-f]*:	e1 c9 40 22 	ldm r9,r1,r5,lr
 *[0-9a-f]*:	e1 cb 81 ec 	ldm r11,r2-r3,r5-r8,pc
 *[0-9a-f]*:	e1 c6 a2 09 	ldm r6,r0,r3,r9,sp,pc

[0-9a-f]* <ldm_pu>:
 *[0-9a-f]*:	e3 cf 03 c0 	ldm pc\+\+,r6-r9
 *[0-9a-f]*:	e3 cc ff ff 	ldm r12\+\+,r0-pc
 *[0-9a-f]*:	e3 c5 80 00 	ldm r5\+\+,pc
 *[0-9a-f]*:	e3 c4 7f ff 	ldm r4\+\+,r0-lr
 *[0-9a-f]*:	e3 ce 00 01 	ldm lr\+\+,r0
 *[0-9a-f]*:	e3 cc d5 38 	ldm r12\+\+,r3-r5,r8,r10,r12,lr-pc
 *[0-9a-f]*:	e3 ca c0 74 	ldm r10\+\+,r2,r4-r6,lr-pc
 *[0-9a-f]*:	e3 c6 7e 1a 	ldm r6\+\+,r1,r3-r4,r9-lr

[0-9a-f]* <ldmts>:
 *[0-9a-f]*:	e5 cf 01 80 	ldmts pc,r7-r8
 *[0-9a-f]*:	e5 cc ff ff 	ldmts r12,r0-pc
 *[0-9a-f]*:	e5 c5 80 00 	ldmts r5,pc
 *[0-9a-f]*:	e5 c4 7f ff 	ldmts r4,r0-lr
 *[0-9a-f]*:	e5 ce 00 01 	ldmts lr,r0
 *[0-9a-f]*:	e5 c0 18 06 	ldmts r0,r1-r2,r11-r12
 *[0-9a-f]*:	e5 ce 61 97 	ldmts lr,r0-r2,r4,r7-r8,sp-lr
 *[0-9a-f]*:	e5 cc c2 3b 	ldmts r12,r0-r1,r3-r5,r9,lr-pc

[0-9a-f]* <ldmts_pu>:
 *[0-9a-f]*:	e7 cf 02 00 	ldmts pc\+\+,r9
 *[0-9a-f]*:	e7 cc ff ff 	ldmts r12\+\+,r0-pc
 *[0-9a-f]*:	e7 c5 80 00 	ldmts r5\+\+,pc
 *[0-9a-f]*:	e7 c4 7f ff 	ldmts r4\+\+,r0-lr
 *[0-9a-f]*:	e7 ce 00 01 	ldmts lr\+\+,r0
 *[0-9a-f]*:	e7 cd 0a bd 	ldmts sp\+\+,r0,r2-r5,r7,r9,r11
 *[0-9a-f]*:	e7 c5 0c 8e 	ldmts r5\+\+,r1-r3,r7,r10-r11
 *[0-9a-f]*:	e7 c8 a1 9c 	ldmts r8\+\+,r2-r4,r7-r8,sp,pc

[0-9a-f]* <stm>:
 *[0-9a-f]*:	e9 cf 00 80 	stm pc,r7
 *[0-9a-f]*:	e9 cc ff ff 	stm r12,r0-pc
 *[0-9a-f]*:	e9 c5 80 00 	stm r5,pc
 *[0-9a-f]*:	e9 c4 7f ff 	stm r4,r0-lr
 *[0-9a-f]*:	e9 ce 00 01 	stm lr,r0
 *[0-9a-f]*:	e9 cd 49 2c 	stm sp,r2-r3,r5,r8,r11,lr
 *[0-9a-f]*:	e9 c4 4c 5f 	stm r4,r0-r4,r6,r10-r11,lr
 *[0-9a-f]*:	e9 c9 f2 22 	stm r9,r1,r5,r9,r12-pc

[0-9a-f]* <stm_pu>:
 *[0-9a-f]*:	eb cf 00 70 	stm --pc,r4-r6
 *[0-9a-f]*:	eb cc ff ff 	stm --r12,r0-pc
 *[0-9a-f]*:	eb c5 80 00 	stm --r5,pc
 *[0-9a-f]*:	eb c4 7f ff 	stm --r4,r0-lr
 *[0-9a-f]*:	eb ce 00 01 	stm --lr,r0
 *[0-9a-f]*:	eb cb fb f1 	stm --r11,r0,r4-r9,r11-pc
 *[0-9a-f]*:	eb cb 56 09 	stm --r11,r0,r3,r9-r10,r12,lr
 *[0-9a-f]*:	eb c6 63 04 	stm --r6,r2,r8-r9,sp-lr

[0-9a-f]* <stmts>:
 *[0-9a-f]*:	ed cf 01 00 	stmts pc,r8
 *[0-9a-f]*:	ed cc ff ff 	stmts r12,r0-pc
 *[0-9a-f]*:	ed c5 80 00 	stmts r5,pc
 *[0-9a-f]*:	ed c4 7f ff 	stmts r4,r0-lr
 *[0-9a-f]*:	ed ce 00 01 	stmts lr,r0
 *[0-9a-f]*:	ed c1 c6 5b 	stmts r1,r0-r1,r3-r4,r6,r9-r10,lr-pc
 *[0-9a-f]*:	ed c3 1d c1 	stmts r3,r0,r6-r8,r10-r12
 *[0-9a-f]*:	ed cb d6 d1 	stmts r11,r0,r4,r6-r7,r9-r10,r12,lr-pc

[0-9a-f]* <stmts_pu>:
 *[0-9a-f]*:	ef cf 01 c0 	stmts --pc,r6-r8
 *[0-9a-f]*:	ef cc ff ff 	stmts --r12,r0-pc
 *[0-9a-f]*:	ef c5 80 00 	stmts --r5,pc
 *[0-9a-f]*:	ef c4 7f ff 	stmts --r4,r0-lr
 *[0-9a-f]*:	ef ce 00 01 	stmts --lr,r0
 *[0-9a-f]*:	ef c2 36 19 	stmts --r2,r0,r3-r4,r9-r10,r12-sp
 *[0-9a-f]*:	ef c3 c0 03 	stmts --r3,r0-r1,lr-pc
 *[0-9a-f]*:	ef c0 44 7d 	stmts --r0,r0,r2-r6,r10,lr

[0-9a-f]* <ldins_h>:
 *[0-9a-f]*:	ff df 00 00 	ldins\.h pc:b,pc\[0\]
 *[0-9a-f]*:	f9 dc 1f ff 	ldins\.h r12:t,r12\[-2\]
 *[0-9a-f]*:	eb d5 18 00 	ldins\.h r5:t,r5\[-4096\]
 *[0-9a-f]*:	e9 d4 07 ff 	ldins\.h r4:b,r4\[4094\]
 *[0-9a-f]*:	fd de 10 01 	ldins\.h lr:t,lr\[2\]
 *[0-9a-f]*:	fd d0 13 c5 	ldins\.h r0:t,lr\[1930\]
 *[0-9a-f]*:	ef d3 0e f5 	ldins\.h r3:b,r7\[-534\]
 *[0-9a-f]*:	f9 d2 0b 9a 	ldins\.h r2:b,r12\[-2252\]

[0-9a-f]* <ldins_b>:
 *[0-9a-f]*:	ff df 40 00 	ldins\.b pc:b,pc\[0\]
 *[0-9a-f]*:	f9 dc 7f ff 	ldins\.b r12:t,r12\[-1\]
 *[0-9a-f]*:	eb d5 68 00 	ldins\.b r5:u,r5\[-2048\]
 *[0-9a-f]*:	e9 d4 57 ff 	ldins\.b r4:l,r4\[2047\]
 *[0-9a-f]*:	fd de 50 01 	ldins\.b lr:l,lr\[1\]
 *[0-9a-f]*:	e9 d6 7d 6a 	ldins\.b r6:t,r4\[-662\]
 *[0-9a-f]*:	e3 d5 4f 69 	ldins\.b r5:b,r1\[-151\]
 *[0-9a-f]*:	f7 da 78 7d 	ldins\.b r10:t,r11\[-1923\]

[0-9a-f]* <ldswp_sh>:
 *[0-9a-f]*:	ff df 20 00 	ldswp\.sh pc,pc\[0\]
 *[0-9a-f]*:	f9 dc 2f ff 	ldswp\.sh r12,r12\[-2\]
 *[0-9a-f]*:	eb d5 28 00 	ldswp\.sh r5,r5\[-4096\]
 *[0-9a-f]*:	e9 d4 27 ff 	ldswp\.sh r4,r4\[4094\]
 *[0-9a-f]*:	fd de 20 01 	ldswp\.sh lr,lr\[2\]
 *[0-9a-f]*:	f5 d9 27 84 	ldswp\.sh r9,r10\[3848\]
 *[0-9a-f]*:	f9 d4 2c 04 	ldswp\.sh r4,r12\[-2040\]
 *[0-9a-f]*:	e5 da 26 08 	ldswp\.sh r10,r2\[3088\]

[0-9a-f]* <ldswp_uh>:
 *[0-9a-f]*:	ff df 30 00 	ldswp\.uh pc,pc\[0\]
 *[0-9a-f]*:	f9 dc 3f ff 	ldswp\.uh r12,r12\[-2\]
 *[0-9a-f]*:	eb d5 38 00 	ldswp\.uh r5,r5\[-4096\]
 *[0-9a-f]*:	e9 d4 37 ff 	ldswp\.uh r4,r4\[4094\]
 *[0-9a-f]*:	fd de 30 01 	ldswp\.uh lr,lr\[2\]
 *[0-9a-f]*:	f3 d4 37 46 	ldswp\.uh r4,r9\[3724\]
 *[0-9a-f]*:	fb de 3c bc 	ldswp\.uh lr,sp\[-1672\]
 *[0-9a-f]*:	f9 d8 38 7d 	ldswp\.uh r8,r12\[-3846\]

[0-9a-f]* <ldswp_w>:
 *[0-9a-f]*:	ff df 80 00 	ldswp\.w pc,pc\[0\]
 *[0-9a-f]*:	f9 dc 8f ff 	ldswp\.w r12,r12\[-4\]
 *[0-9a-f]*:	eb d5 88 00 	ldswp\.w r5,r5\[-8192\]
 *[0-9a-f]*:	e9 d4 87 ff 	ldswp\.w r4,r4\[8188\]
 *[0-9a-f]*:	fd de 80 01 	ldswp\.w lr,lr\[4\]
 *[0-9a-f]*:	ef dd 81 d1 	ldswp\.w sp,r7\[1860\]
 *[0-9a-f]*:	eb df 8c c1 	ldswp\.w pc,r5\[-3324\]
 *[0-9a-f]*:	f5 dc 8c c8 	ldswp\.w r12,r10\[-3296\]

[0-9a-f]* <stswp_h>:
 *[0-9a-f]*:	ff df 90 00 	stswp\.h pc\[0\],pc
 *[0-9a-f]*:	f9 dc 9f ff 	stswp\.h r12\[-2\],r12
 *[0-9a-f]*:	eb d5 98 00 	stswp\.h r5\[-4096\],r5
 *[0-9a-f]*:	e9 d4 97 ff 	stswp\.h r4\[4094\],r4
 *[0-9a-f]*:	fd de 90 01 	stswp\.h lr\[2\],lr
 *[0-9a-f]*:	ef da 90 20 	stswp\.h r7\[64\],r10
 *[0-9a-f]*:	f5 d2 95 e8 	stswp\.h r10\[3024\],r2
 *[0-9a-f]*:	e1 da 9b 74 	stswp\.h r0\[-2328\],r10

[0-9a-f]* <stswp_w>:
 *[0-9a-f]*:	ff df a0 00 	stswp\.w pc\[0\],pc
 *[0-9a-f]*:	f9 dc af ff 	stswp\.w r12\[-4\],r12
 *[0-9a-f]*:	eb d5 a8 00 	stswp\.w r5\[-8192\],r5
 *[0-9a-f]*:	e9 d4 a7 ff 	stswp\.w r4\[8188\],r4
 *[0-9a-f]*:	fd de a0 01 	stswp\.w lr\[4\],lr
 *[0-9a-f]*:	ff d8 a1 21 	stswp\.w pc\[1156\],r8
 *[0-9a-f]*:	fb da a7 ce 	stswp\.w sp\[7992\],r10
 *[0-9a-f]*:	f1 d5 ae db 	stswp\.w r8\[-1172\],r5

[0-9a-f]* <and2>:
 *[0-9a-f]*:	ff ef 00 0f 	and pc,pc,pc
 *[0-9a-f]*:	f9 ec 01 fc 	and r12,r12,r12<<0x1f
 *[0-9a-f]*:	eb e5 01 05 	and r5,r5,r5<<0x10
 *[0-9a-f]*:	e9 e4 00 f4 	and r4,r4,r4<<0xf
 *[0-9a-f]*:	fd ee 00 1e 	and lr,lr,lr<<0x1
 *[0-9a-f]*:	e5 e1 00 1a 	and r10,r2,r1<<0x1
 *[0-9a-f]*:	f1 eb 01 bc 	and r12,r8,r11<<0x1b
 *[0-9a-f]*:	ef e0 00 3a 	and r10,r7,r0<<0x3

[0-9a-f]* <and3>:
 *[0-9a-f]*:	ff ef 02 0f 	and pc,pc,pc
 *[0-9a-f]*:	f9 ec 03 fc 	and r12,r12,r12>>0x1f
 *[0-9a-f]*:	eb e5 03 05 	and r5,r5,r5>>0x10
 *[0-9a-f]*:	e9 e4 02 f4 	and r4,r4,r4>>0xf
 *[0-9a-f]*:	fd ee 02 1e 	and lr,lr,lr>>0x1
 *[0-9a-f]*:	f1 e7 03 1c 	and r12,r8,r7>>0x11
 *[0-9a-f]*:	e9 e9 03 4f 	and pc,r4,r9>>0x14
 *[0-9a-f]*:	f3 ea 02 ca 	and r10,r9,r10>>0xc

[0-9a-f]* <or2>:
 *[0-9a-f]*:	ff ef 10 0f 	or pc,pc,pc
 *[0-9a-f]*:	f9 ec 11 fc 	or r12,r12,r12<<0x1f
 *[0-9a-f]*:	eb e5 11 05 	or r5,r5,r5<<0x10
 *[0-9a-f]*:	e9 e4 10 f4 	or r4,r4,r4<<0xf
 *[0-9a-f]*:	fd ee 10 1e 	or lr,lr,lr<<0x1
 *[0-9a-f]*:	fb eb 11 d8 	or r8,sp,r11<<0x1d
 *[0-9a-f]*:	f3 e2 11 cf 	or pc,r9,r2<<0x1c
 *[0-9a-f]*:	e3 e2 10 35 	or r5,r1,r2<<0x3

[0-9a-f]* <or3>:
 *[0-9a-f]*:	ff ef 12 0f 	or pc,pc,pc
 *[0-9a-f]*:	f9 ec 13 fc 	or r12,r12,r12>>0x1f
 *[0-9a-f]*:	eb e5 13 05 	or r5,r5,r5>>0x10
 *[0-9a-f]*:	e9 e4 12 f4 	or r4,r4,r4>>0xf
 *[0-9a-f]*:	fd ee 12 1e 	or lr,lr,lr>>0x1
 *[0-9a-f]*:	fb ed 12 21 	or r1,sp,sp>>0x2
 *[0-9a-f]*:	e3 e1 13 d0 	or r0,r1,r1>>0x1d
 *[0-9a-f]*:	f9 e8 12 84 	or r4,r12,r8>>0x8

[0-9a-f]* <eor2>:
 *[0-9a-f]*:	ff ef 20 0f 	eor pc,pc,pc
 *[0-9a-f]*:	f9 ec 21 fc 	eor r12,r12,r12<<0x1f
 *[0-9a-f]*:	eb e5 21 05 	eor r5,r5,r5<<0x10
 *[0-9a-f]*:	e9 e4 20 f4 	eor r4,r4,r4<<0xf
 *[0-9a-f]*:	fd ee 20 1e 	eor lr,lr,lr<<0x1
 *[0-9a-f]*:	f3 e4 20 ba 	eor r10,r9,r4<<0xb
 *[0-9a-f]*:	e1 e1 21 f4 	eor r4,r0,r1<<0x1f
 *[0-9a-f]*:	e5 ec 20 d6 	eor r6,r2,r12<<0xd

[0-9a-f]* <eor3>:
 *[0-9a-f]*:	ff ef 22 0f 	eor pc,pc,pc
 *[0-9a-f]*:	f9 ec 23 fc 	eor r12,r12,r12>>0x1f
 *[0-9a-f]*:	eb e5 23 05 	eor r5,r5,r5>>0x10
 *[0-9a-f]*:	e9 e4 22 f4 	eor r4,r4,r4>>0xf
 *[0-9a-f]*:	fd ee 22 1e 	eor lr,lr,lr>>0x1
 *[0-9a-f]*:	eb e5 23 65 	eor r5,r5,r5>>0x16
 *[0-9a-f]*:	e3 ee 22 3a 	eor r10,r1,lr>>0x3
 *[0-9a-f]*:	fd ed 23 a7 	eor r7,lr,sp>>0x1a

[0-9a-f]* <sthh_w2>:
 *[0-9a-f]*:	ff ef 8f 0f 	sthh\.w pc\[pc\],pc:b,pc:b
 *[0-9a-f]*:	f9 ec bc 3c 	sthh\.w r12\[r12<<0x3\],r12:t,r12:t
 *[0-9a-f]*:	eb e5 b5 25 	sthh\.w r5\[r5<<0x2\],r5:t,r5:t
 *[0-9a-f]*:	e9 e4 84 14 	sthh\.w r4\[r4<<0x1\],r4:b,r4:b
 *[0-9a-f]*:	fd ee be 1e 	sthh\.w lr\[lr<<0x1\],lr:t,lr:t
 *[0-9a-f]*:	e3 ec b6 3d 	sthh\.w sp\[r6<<0x3\],r1:t,r12:t
 *[0-9a-f]*:	f3 e9 b6 06 	sthh\.w r6\[r6\],r9:t,r9:t
 *[0-9a-f]*:	e1 eb 93 0a 	sthh\.w r10\[r3\],r0:b,r11:t

[0-9a-f]* <sthh_w1>:
 *[0-9a-f]*:	ff ef c0 0f 	sthh\.w pc\[0x0\],pc:b,pc:b
 *[0-9a-f]*:	f9 ec ff fc 	sthh\.w r12\[0x3fc\],r12:t,r12:t
 *[0-9a-f]*:	eb e5 f8 05 	sthh\.w r5\[0x200\],r5:t,r5:t
 *[0-9a-f]*:	e9 e4 c7 f4 	sthh\.w r4\[0x1fc\],r4:b,r4:b
 *[0-9a-f]*:	fd ee f0 1e 	sthh\.w lr\[0x4\],lr:t,lr:t
 *[0-9a-f]*:	f3 e0 e6 54 	sthh\.w r4\[0x194\],r9:t,r0:b
 *[0-9a-f]*:	e5 ea e5 78 	sthh\.w r8\[0x15c\],r2:t,r10:b
 *[0-9a-f]*:	f3 e2 c2 bd 	sthh\.w sp\[0xac\],r9:b,r2:b

[0-9a-f]* <cop>:
 *[0-9a-f]*:	e1 a0 00 00 	cop cp0,cr0,cr0,cr0,0x0
 *[0-9a-f]*:	e7 af ff ff 	cop cp7,cr15,cr15,cr15,0x7f
 *[0-9a-f]*:	e3 a8 75 55 	cop cp3,cr5,cr5,cr5,0x31
 *[0-9a-f]*:	e3 a8 44 44 	cop cp2,cr4,cr4,cr4,0x30
 *[0-9a-f]*:	e5 ad a8 37 	cop cp5,cr8,cr3,cr7,0x5a

[0-9a-f]* <ldc_w1>:
 *[0-9a-f]*:	e9 a0 00 00 	ldc\.w cp0,cr0,r0\[0x0\]
 *[0-9a-f]*:	e9 af ef ff 	ldc\.w cp7,cr15,pc\[0x3fc\]
 *[0-9a-f]*:	e9 a5 65 80 	ldc\.w cp3,cr5,r5\[0x200\]
 *[0-9a-f]*:	e9 a4 44 7f 	ldc\.w cp2,cr4,r4\[0x1fc\]
 *[0-9a-f]*:	e9 ad 89 24 	ldc\.w cp4,cr9,sp\[0x90\]

[0-9a-f]* <ldc_w2>:
 *[0-9a-f]*:	ef a0 00 40 	ldc\.w cp0,cr0,--r0
 *[0-9a-f]*:	ef af ef 40 	ldc\.w cp7,cr15,--pc
 *[0-9a-f]*:	ef a5 65 40 	ldc\.w cp3,cr5,--r5
 *[0-9a-f]*:	ef a4 44 40 	ldc\.w cp2,cr4,--r4
 *[0-9a-f]*:	ef ad 89 40 	ldc\.w cp4,cr9,--sp

[0-9a-f]* <ldc_w3>:
 *[0-9a-f]*:	ef a0 10 00 	ldc\.w cp0,cr0,r0\[r0\]
 *[0-9a-f]*:	ef af ff 3f 	ldc\.w cp7,cr15,pc\[pc<<0x3\]
 *[0-9a-f]*:	ef a5 75 24 	ldc\.w cp3,cr5,r5\[r4<<0x2\]
 *[0-9a-f]*:	ef a4 54 13 	ldc\.w cp2,cr4,r4\[r3<<0x1\]
 *[0-9a-f]*:	ef ad 99 0c 	ldc\.w cp4,cr9,sp\[r12\]

[0-9a-f]* <ldc_d1>:
 *[0-9a-f]*:	e9 a0 10 00 	ldc\.d cp0,cr0,r0\[0x0\]
 *[0-9a-f]*:	e9 af fe ff 	ldc\.d cp7,cr14,pc\[0x3fc\]
 *[0-9a-f]*:	e9 a5 76 80 	ldc\.d cp3,cr6,r5\[0x200\]
 *[0-9a-f]*:	e9 a4 54 7f 	ldc\.d cp2,cr4,r4\[0x1fc\]
 *[0-9a-f]*:	e9 ad 98 24 	ldc\.d cp4,cr8,sp\[0x90\]

[0-9a-f]* <ldc_d2>:
 *[0-9a-f]*:	ef a0 00 50 	ldc\.d cp0,cr0,--r0
 *[0-9a-f]*:	ef af ee 50 	ldc\.d cp7,cr14,--pc
 *[0-9a-f]*:	ef a5 66 50 	ldc\.d cp3,cr6,--r5
 *[0-9a-f]*:	ef a4 44 50 	ldc\.d cp2,cr4,--r4
 *[0-9a-f]*:	ef ad 88 50 	ldc\.d cp4,cr8,--sp

[0-9a-f]* <ldc_d3>:
 *[0-9a-f]*:	ef a0 10 40 	ldc\.d cp0,cr0,r0\[r0\]
 *[0-9a-f]*:	ef af fe 7f 	ldc\.d cp7,cr14,pc\[pc<<0x3\]
 *[0-9a-f]*:	ef a5 76 64 	ldc\.d cp3,cr6,r5\[r4<<0x2\]
 *[0-9a-f]*:	ef a4 54 53 	ldc\.d cp2,cr4,r4\[r3<<0x1\]
 *[0-9a-f]*:	ef ad 98 4c 	ldc\.d cp4,cr8,sp\[r12\]

[0-9a-f]* <stc_w1>:
 *[0-9a-f]*:	eb a0 00 00 	stc\.w cp0,r0\[0x0\],cr0
 *[0-9a-f]*:	eb af ef ff 	stc\.w cp7,pc\[0x3fc\],cr15
 *[0-9a-f]*:	eb a5 65 80 	stc\.w cp3,r5\[0x200\],cr5
 *[0-9a-f]*:	eb a4 44 7f 	stc\.w cp2,r4\[0x1fc\],cr4
 *[0-9a-f]*:	eb ad 89 24 	stc\.w cp4,sp\[0x90\],cr9

[0-9a-f]* <stc_w2>:
 *[0-9a-f]*:	ef a0 00 60 	stc\.w cp0,r0\+\+,cr0
 *[0-9a-f]*:	ef af ef 60 	stc\.w cp7,pc\+\+,cr15
 *[0-9a-f]*:	ef a5 65 60 	stc\.w cp3,r5\+\+,cr5
 *[0-9a-f]*:	ef a4 44 60 	stc\.w cp2,r4\+\+,cr4
 *[0-9a-f]*:	ef ad 89 60 	stc\.w cp4,sp\+\+,cr9

[0-9a-f]* <stc_w3>:
 *[0-9a-f]*:	ef a0 10 80 	stc\.w cp0,r0\[r0\],cr0
 *[0-9a-f]*:	ef af ff bf 	stc\.w cp7,pc\[pc<<0x3\],cr15
 *[0-9a-f]*:	ef a5 75 a4 	stc\.w cp3,r5\[r4<<0x2\],cr5
 *[0-9a-f]*:	ef a4 54 93 	stc\.w cp2,r4\[r3<<0x1\],cr4
 *[0-9a-f]*:	ef ad 99 8c 	stc\.w cp4,sp\[r12\],cr9

[0-9a-f]* <stc_d1>:
 *[0-9a-f]*:	eb a0 10 00 	stc\.d cp0,r0\[0x0\],cr0
 *[0-9a-f]*:	eb af fe ff 	stc\.d cp7,pc\[0x3fc\],cr14
 *[0-9a-f]*:	eb a5 76 80 	stc\.d cp3,r5\[0x200\],cr6
 *[0-9a-f]*:	eb a4 54 7f 	stc\.d cp2,r4\[0x1fc\],cr4
 *[0-9a-f]*:	eb ad 98 24 	stc\.d cp4,sp\[0x90\],cr8

[0-9a-f]* <stc_d2>:
 *[0-9a-f]*:	ef a0 00 70 	stc\.d cp0,r0\+\+,cr0
 *[0-9a-f]*:	ef af ee 70 	stc\.d cp7,pc\+\+,cr14
 *[0-9a-f]*:	ef a5 66 70 	stc\.d cp3,r5\+\+,cr6
 *[0-9a-f]*:	ef a4 44 70 	stc\.d cp2,r4\+\+,cr4
 *[0-9a-f]*:	ef ad 88 70 	stc\.d cp4,sp\+\+,cr8

[0-9a-f]* <stc_d3>:
 *[0-9a-f]*:	ef a0 10 c0 	stc\.d cp0,r0\[r0\],cr0
 *[0-9a-f]*:	ef af fe ff 	stc\.d cp7,pc\[pc<<0x3\],cr14
 *[0-9a-f]*:	ef a5 76 e4 	stc\.d cp3,r5\[r4<<0x2\],cr6
 *[0-9a-f]*:	ef a4 54 d3 	stc\.d cp2,r4\[r3<<0x1\],cr4
 *[0-9a-f]*:	ef ad 98 cc 	stc\.d cp4,sp\[r12\],cr8

[0-9a-f]* <ldc0_w>:
 *[0-9a-f]*:	f1 a0 00 00 	ldc0\.w cr0,r0\[0x0\]
 *[0-9a-f]*:	f1 af ff ff 	ldc0\.w cr15,pc\[0x3ffc\]
 *[0-9a-f]*:	f1 a5 85 00 	ldc0\.w cr5,r5\[0x2000\]
 *[0-9a-f]*:	f1 a4 74 ff 	ldc0\.w cr4,r4\[0x1ffc\]
 *[0-9a-f]*:	f1 ad 09 93 	ldc0\.w cr9,sp\[0x24c\]

[0-9a-f]* <ldc0_d>:
 *[0-9a-f]*:	f3 a0 00 00 	ldc0\.d cr0,r0\[0x0\]
 *[0-9a-f]*:	f3 af fe ff 	ldc0\.d cr14,pc\[0x3ffc\]
 *[0-9a-f]*:	f3 a5 86 00 	ldc0\.d cr6,r5\[0x2000\]
 *[0-9a-f]*:	f3 a4 74 ff 	ldc0\.d cr4,r4\[0x1ffc\]
 *[0-9a-f]*:	f3 ad 08 93 	ldc0\.d cr8,sp\[0x24c\]

[0-9a-f]* <stc0_w>:
 *[0-9a-f]*:	f5 a0 00 00 	stc0\.w r0\[0x0\],cr0
 *[0-9a-f]*:	f5 af ff ff 	stc0\.w pc\[0x3ffc\],cr15
 *[0-9a-f]*:	f5 a5 85 00 	stc0\.w r5\[0x2000\],cr5
 *[0-9a-f]*:	f5 a4 74 ff 	stc0\.w r4\[0x1ffc\],cr4
 *[0-9a-f]*:	f5 ad 09 93 	stc0\.w sp\[0x24c\],cr9

[0-9a-f]* <stc0_d>:
 *[0-9a-f]*:	f7 a0 00 00 	stc0\.d r0\[0x0\],cr0
 *[0-9a-f]*:	f7 af fe ff 	stc0\.d pc\[0x3ffc\],cr14
 *[0-9a-f]*:	f7 a5 86 00 	stc0\.d r5\[0x2000\],cr6
 *[0-9a-f]*:	f7 a4 74 ff 	stc0\.d r4\[0x1ffc\],cr4
 *[0-9a-f]*:	f7 ad 08 93 	stc0\.d sp\[0x24c\],cr8

[0-9a-f]* <memc>:
 *[0-9a-f]*:	f6 10 00 00 	memc 0,0x0
 *[0-9a-f]*:	f6 1f ff ff 	memc -4,0x1f
 *[0-9a-f]*:	f6 18 40 00 	memc -65536,0x10
 *[0-9a-f]*:	f6 17 bf ff 	memc 65532,0xf

[0-9a-f]* <mems>:
 *[0-9a-f]*:	f8 10 00 00 	mems 0,0x0
 *[0-9a-f]*:	f8 1f ff ff 	mems -4,0x1f
 *[0-9a-f]*:	f8 18 40 00 	mems -65536,0x10
 *[0-9a-f]*:	f8 17 bf ff 	mems 65532,0xf

[0-9a-f]* <memt>:
 *[0-9a-f]*:	fa 10 00 00 	memt 0,0x0
 *[0-9a-f]*:	fa 1f ff ff 	memt -4,0x1f
 *[0-9a-f]*:	fa 18 40 00 	memt -65536,0x10
 *[0-9a-f]*:	fa 17 bf ff 	memt 65532,0xf

[0-9a-f]* <stcond>:
 *[0-9a-f]*:	e1 70 00 00 	stcond r0\[0\],r0
 *[0-9a-f]*:	ff 7f ff ff 	stcond pc\[-1\],pc
 *[0-9a-f]*:	f1 77 80 00 	stcond r8\[-32768\],r7
 *[0-9a-f]*:	ef 78 7f ff 	stcond r7\[32767\],r8
 *[0-9a-f]*:	eb 7a 12 34 	stcond r5\[4660\],r10

[0-9a-f]* <ldcm_w>:
 *[0-9a-f]*:	ed af 00 ff 	ldcm\.w cp0,pc,cr0-cr7
 *[0-9a-f]*:	ed a0 e0 01 	ldcm\.w cp7,r0,cr0
 *[0-9a-f]*:	ed a4 90 7f 	ldcm\.w cp4,r4\+\+,cr0-cr6
 *[0-9a-f]*:	ed a7 60 80 	ldcm\.w cp3,r7,cr7
 *[0-9a-f]*:	ed ac 30 72 	ldcm\.w cp1,r12\+\+,cr1,cr4-cr6
 *[0-9a-f]*:	ed af 01 ff 	ldcm\.w cp0,pc,cr8-cr15
 *[0-9a-f]*:	ed a0 e1 01 	ldcm\.w cp7,r0,cr8
 *[0-9a-f]*:	ed a4 91 7f 	ldcm\.w cp4,r4\+\+,cr8-cr14
 *[0-9a-f]*:	ed a7 61 80 	ldcm\.w cp3,r7,cr15
 *[0-9a-f]*:	ed ac 31 72 	ldcm\.w cp1,r12\+\+,cr9,cr12-cr14

[0-9a-f]* <ldcm_d>:
 *[0-9a-f]*:	ed af 04 ff 	ldcm\.d cp0,pc,cr0-cr15
 *[0-9a-f]*:	ed a0 e4 01 	ldcm\.d cp7,r0,cr0-cr1
 *[0-9a-f]*:	ed a4 94 7f 	ldcm\.d cp4,r4\+\+,cr0-cr13
 *[0-9a-f]*:	ed a7 64 80 	ldcm\.d cp3,r7,cr14-cr15
 *[0-9a-f]*:	ed ac 54 93 	ldcm\.d cp2,r12\+\+,cr0-cr3,cr8-cr9,cr14-cr15

[0-9a-f]* <stcm_w>:
 *[0-9a-f]*:	ed af 02 ff 	stcm\.w cp0,pc,cr0-cr7
 *[0-9a-f]*:	ed a0 e2 01 	stcm\.w cp7,r0,cr0
 *[0-9a-f]*:	ed a4 92 7f 	stcm\.w cp4,--r4,cr0-cr6
 *[0-9a-f]*:	ed a7 62 80 	stcm\.w cp3,r7,cr7
 *[0-9a-f]*:	ed ac 32 72 	stcm\.w cp1,--r12,cr1,cr4-cr6
 *[0-9a-f]*:	ed af 03 ff 	stcm\.w cp0,pc,cr8-cr15
 *[0-9a-f]*:	ed a0 e3 01 	stcm\.w cp7,r0,cr8
 *[0-9a-f]*:	ed a4 93 7f 	stcm\.w cp4,--r4,cr8-cr14
 *[0-9a-f]*:	ed a7 63 80 	stcm\.w cp3,r7,cr15
 *[0-9a-f]*:	ed ac 33 72 	stcm\.w cp1,--r12,cr9,cr12-cr14

[0-9a-f]* <stcm_d>:
 *[0-9a-f]*:	ed af 05 ff 	stcm\.d cp0,pc,cr0-cr15
 *[0-9a-f]*:	ed a0 e5 01 	stcm\.d cp7,r0,cr0-cr1
 *[0-9a-f]*:	ed a4 95 7f 	stcm\.d cp4,--r4,cr0-cr13
 *[0-9a-f]*:	ed a7 65 80 	stcm\.d cp3,r7,cr14-cr15
 *[0-9a-f]*:	ed ac 55 93 	stcm\.d cp2,--r12,cr0-cr3,cr8-cr9,cr14-cr15

[0-9a-f]* <mvcr_w>:
 *[0-9a-f]*:	ef af ef 00 	mvcr\.w cp7,pc,cr15
 *[0-9a-f]*:	ef a0 00 00 	mvcr\.w cp0,r0,cr0
 *[0-9a-f]*:	ef af 0f 00 	mvcr\.w cp0,pc,cr15
 *[0-9a-f]*:	ef a0 ef 00 	mvcr\.w cp7,r0,cr15
 *[0-9a-f]*:	ef af e0 00 	mvcr\.w cp7,pc,cr0
 *[0-9a-f]*:	ef a7 88 00 	mvcr\.w cp4,r7,cr8
 *[0-9a-f]*:	ef a8 67 00 	mvcr\.w cp3,r8,cr7

[0-9a-f]* <mvcr_d>:
 *[0-9a-f]*:	ef ae ee 10 	mvcr\.d cp7,lr,cr14
 *[0-9a-f]*:	ef a0 00 10 	mvcr\.d cp0,r0,cr0
 *[0-9a-f]*:	ef ae 0e 10 	mvcr\.d cp0,lr,cr14
 *[0-9a-f]*:	ef a0 ee 10 	mvcr\.d cp7,r0,cr14
 *[0-9a-f]*:	ef ae e0 10 	mvcr\.d cp7,lr,cr0
 *[0-9a-f]*:	ef a6 88 10 	mvcr\.d cp4,r6,cr8
 *[0-9a-f]*:	ef a8 66 10 	mvcr\.d cp3,r8,cr6

[0-9a-f]* <mvrc_w>:
 *[0-9a-f]*:	ef af ef 20 	mvrc\.w cp7,cr15,pc
 *[0-9a-f]*:	ef a0 00 20 	mvrc\.w cp0,cr0,r0
 *[0-9a-f]*:	ef af 0f 20 	mvrc\.w cp0,cr15,pc
 *[0-9a-f]*:	ef a0 ef 20 	mvrc\.w cp7,cr15,r0
 *[0-9a-f]*:	ef af e0 20 	mvrc\.w cp7,cr0,pc
 *[0-9a-f]*:	ef a7 88 20 	mvrc\.w cp4,cr8,r7
 *[0-9a-f]*:	ef a8 67 20 	mvrc\.w cp3,cr7,r8

[0-9a-f]* <mvrc_d>:
 *[0-9a-f]*:	ef ae ee 30 	mvrc\.d cp7,cr14,lr
 *[0-9a-f]*:	ef a0 00 30 	mvrc\.d cp0,cr0,r0
 *[0-9a-f]*:	ef ae 0e 30 	mvrc\.d cp0,cr14,lr
 *[0-9a-f]*:	ef a0 ee 30 	mvrc\.d cp7,cr14,r0
 *[0-9a-f]*:	ef ae e0 30 	mvrc\.d cp7,cr0,lr
 *[0-9a-f]*:	ef a6 88 30 	mvrc\.d cp4,cr8,r6
 *[0-9a-f]*:	ef a8 66 30 	mvrc\.d cp3,cr6,r8

[0-9a-f]* <bfexts>:
 *[0-9a-f]*:	ff df b3 ff 	bfexts pc,pc,0x1f,0x1f
 *[0-9a-f]*:	e1 d0 b0 00 	bfexts r0,r0,0x0,0x0
 *[0-9a-f]*:	e1 df b3 ff 	bfexts r0,pc,0x1f,0x1f
 *[0-9a-f]*:	ff d0 b3 ff 	bfexts pc,r0,0x1f,0x1f
 *[0-9a-f]*:	ff df b0 1f 	bfexts pc,pc,0x0,0x1f
 *[0-9a-f]*:	ff df b3 e0 	bfexts pc,pc,0x1f,0x0
 *[0-9a-f]*:	ef d8 b1 f0 	bfexts r7,r8,0xf,0x10
 *[0-9a-f]*:	f1 d7 b2 0f 	bfexts r8,r7,0x10,0xf

[0-9a-f]* <bfextu>:
 *[0-9a-f]*:	ff df c3 ff 	bfextu pc,pc,0x1f,0x1f
 *[0-9a-f]*:	e1 d0 c0 00 	bfextu r0,r0,0x0,0x0
 *[0-9a-f]*:	e1 df c3 ff 	bfextu r0,pc,0x1f,0x1f
 *[0-9a-f]*:	ff d0 c3 ff 	bfextu pc,r0,0x1f,0x1f
 *[0-9a-f]*:	ff df c0 1f 	bfextu pc,pc,0x0,0x1f
 *[0-9a-f]*:	ff df c3 e0 	bfextu pc,pc,0x1f,0x0
 *[0-9a-f]*:	ef d8 c1 f0 	bfextu r7,r8,0xf,0x10
 *[0-9a-f]*:	f1 d7 c2 0f 	bfextu r8,r7,0x10,0xf

[0-9a-f]* <bfins>:
 *[0-9a-f]*:	ff df d3 ff 	bfins pc,pc,0x1f,0x1f
 *[0-9a-f]*:	e1 d0 d0 00 	bfins r0,r0,0x0,0x0
 *[0-9a-f]*:	e1 df d3 ff 	bfins r0,pc,0x1f,0x1f
 *[0-9a-f]*:	ff d0 d3 ff 	bfins pc,r0,0x1f,0x1f
 *[0-9a-f]*:	ff df d0 1f 	bfins pc,pc,0x0,0x1f
 *[0-9a-f]*:	ff df d3 e0 	bfins pc,pc,0x1f,0x0
 *[0-9a-f]*:	ef d8 d1 f0 	bfins r7,r8,0xf,0x10
 *[0-9a-f]*:	f1 d7 d2 0f 	bfins r8,r7,0x10,0xf

[0-9a-f]* <rsubc>:
 *[0-9a-f]*:	fb bf 00 00 	rsubeq pc,0
 *[0-9a-f]*:	fb bc 0f ff 	rsubal r12,-1
 *[0-9a-f]*:	fb b5 08 80 	rsubls r5,-128
 *[0-9a-f]*:	fb b4 07 7f 	rsubpl r4,127
 *[0-9a-f]*:	fb be 01 01 	rsubne lr,1
 *[0-9a-f]*:	fb bc 08 76 	rsubls r12,118
 *[0-9a-f]*:	fb be 0d f4 	rsubvc lr,-12
 *[0-9a-f]*:	fb b4 06 f3 	rsubmi r4,-13

[0-9a-f]* <addc>:
 *[0-9a-f]*:	ff df e0 0f 	addeq pc,pc,pc
 *[0-9a-f]*:	f9 dc ef 0c 	addal r12,r12,r12
 *[0-9a-f]*:	eb d5 e8 05 	addls r5,r5,r5
 *[0-9a-f]*:	e9 d4 e7 04 	addpl r4,r4,r4   
 *[0-9a-f]*:	fd de e1 0e 	addne lr,lr,lr
 *[0-9a-f]*:	e5 d1 e8 0a 	addls r10,r2,r1
 *[0-9a-f]*:	f1 db ed 0c 	addvc r12,r8,r11
 *[0-9a-f]*:	ef d0 e6 0a 	addmi r10,r7,r0

[0-9a-f]* <subc2>:
 *[0-9a-f]*:	ff df e0 1f 	subeq pc,pc,pc
 *[0-9a-f]*:	f9 dc ef 1c 	subal r12,r12,r12
 *[0-9a-f]*:	eb d5 e8 15 	subls r5,r5,r5
 *[0-9a-f]*:	e9 d4 e7 14 	subpl r4,r4,r4   
 *[0-9a-f]*:	fd de e1 1e 	subne lr,lr,lr
 *[0-9a-f]*:	e5 d1 e8 1a 	subls r10,r2,r1
 *[0-9a-f]*:	f1 db ed 1c 	subvc r12,r8,r11
 *[0-9a-f]*:	ef d0 e6 1a 	submi r10,r7,r0

[0-9a-f]* <andc>:
 *[0-9a-f]*:	ff df e0 2f 	andeq pc,pc,pc
 *[0-9a-f]*:	f9 dc ef 2c 	andal r12,r12,r12
 *[0-9a-f]*:	eb d5 e8 25 	andls r5,r5,r5
 *[0-9a-f]*:	e9 d4 e7 24 	andpl r4,r4,r4   
 *[0-9a-f]*:	fd de e1 2e 	andne lr,lr,lr
 *[0-9a-f]*:	e5 d1 e8 2a 	andls r10,r2,r1
 *[0-9a-f]*:	f1 db ed 2c 	andvc r12,r8,r11
 *[0-9a-f]*:	ef d0 e6 2a 	andmi r10,r7,r0

[0-9a-f]* <orc>:
 *[0-9a-f]*:	ff df e0 3f 	oreq pc,pc,pc
 *[0-9a-f]*:	f9 dc ef 3c 	oral r12,r12,r12
 *[0-9a-f]*:	eb d5 e8 35 	orls r5,r5,r5
 *[0-9a-f]*:	e9 d4 e7 34 	orpl r4,r4,r4   
 *[0-9a-f]*:	fd de e1 3e 	orne lr,lr,lr
 *[0-9a-f]*:	e5 d1 e8 3a 	orls r10,r2,r1
 *[0-9a-f]*:	f1 db ed 3c 	orvc r12,r8,r11
 *[0-9a-f]*:	ef d0 e6 3a 	ormi r10,r7,r0

[0-9a-f]* <eorc>:
 *[0-9a-f]*:	ff df e0 4f 	eoreq pc,pc,pc
 *[0-9a-f]*:	f9 dc ef 4c 	eoral r12,r12,r12
 *[0-9a-f]*:	eb d5 e8 45 	eorls r5,r5,r5
 *[0-9a-f]*:	e9 d4 e7 44 	eorpl r4,r4,r4   
 *[0-9a-f]*:	fd de e1 4e 	eorne lr,lr,lr
 *[0-9a-f]*:	e5 d1 e8 4a 	eorls r10,r2,r1
 *[0-9a-f]*:	f1 db ed 4c 	eorvc r12,r8,r11
 *[0-9a-f]*:	ef d0 e6 4a 	eormi r10,r7,r0

[0-9a-f]* <ldcond>:
 *[0-9a-f]*:	ff ff 01 ff     ld.weq  pc,pc[0x7fc]
 *[0-9a-f]*:	f9 fc f3 ff     ld.shal r12,r12[0x3fe]
 *[0-9a-f]*:	eb f5 84 00     ld.shls r5,r5[0x0]
 *[0-9a-f]*:	e9 f4 79 ff     ld.ubpl r4,r4[0x1ff]
 *[0-9a-f]*:	fd fe 16 00     ld.sbne lr,lr[0x0]
 *[0-9a-f]*:	e5 fa 80 00     ld.wls  r10,r2[0x0]
 *[0-9a-f]*:	f1 fc d3 ff     ld.shvc r12,r8[0x3fe]
 *[0-9a-f]*:	ef fa 68 01     ld.ubmi r10,r7[0x1]

[0-9a-f]* <stcond2>:
 *[0-9a-f]*:	ff ff 0b ff     st.weq pc[0x7fc],pc
 *[0-9a-f]*:	f9 fc fd ff     st.hal r12[0x3fe],r12
 *[0-9a-f]*:	eb f5 8c 00     st.hls r5[0x0],r5
 *[0-9a-f]*:	e9 f4 7f ff     st.bpl r4[0x1ff],r4
 *[0-9a-f]*:	fd fe 1e 00     st.bne lr[0x0],lr
 *[0-9a-f]*:	e5 fa 8a 00     st.wls r2[0x0],r10
 *[0-9a-f]*:	f1 fc dd ff     st.hvc r8[0x3fe],r12
 *[0-9a-f]*:	ef fa 6e 01     st.bmi r7[0x1],r10

[0-9a-f]* <movh>:
 *[0-9a-f]*:	fc 1f ff ff     movh pc,0xffff
 *[0-9a-f]*:	fc 10 00 00     movh r0,0x0
 *[0-9a-f]*:	fc 15 00 01     movh r5,0x1
 *[0-9a-f]*:	fc 1c 7f ff     movh r12,0x7fff

