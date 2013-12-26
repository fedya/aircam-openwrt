 .data
foodata: .word 42
 .text
footext:
	.text
	.global ld_d5
ld_d5:
	ld.d lr,pc[pc<<3]
	ld.d r0,r0[r0<<0]
	ld.d r6,r5[r5<<2]
	ld.d r4,r4[r4<<1]
	ld.d lr,lr[lr<<1]
	ld.d r10,r3[sp<<2]
	ld.d r8,r10[r6<<2]
	ld.d r2,r7[r9<<0]
	.text
	.global ld_w5
ld_w5:
	ld.w pc,pc[pc<<0]
	ld.w r12,r12[r12<<3]
	ld.w r5,r5[r5<<2]
	ld.w r4,r4[r4<<1]
	ld.w lr,lr[lr<<1]
	ld.w r2,r9[r9<<0]
	ld.w r11,r2[r6<<0]
	ld.w r0,r2[sp<<3]
	.text
	.global ld_sh5
ld_sh5:
	ld.sh pc,pc[pc<<0]
	ld.sh r12,r12[r12<<3]
	ld.sh r5,r5[r5<<2]
	ld.sh r4,r4[r4<<1]
	ld.sh lr,lr[lr<<1]
	ld.sh r11,r0[pc<<2]
	ld.sh r10,sp[r6<<2]
	ld.sh r12,r2[r2<<0]
	.text
	.global ld_uh5
ld_uh5:
	ld.uh pc,pc[pc<<0]
	ld.uh r12,r12[r12<<3]
	ld.uh r5,r5[r5<<2]
	ld.uh r4,r4[r4<<1]
	ld.uh lr,lr[lr<<1]
	ld.uh r8,pc[lr<<3]
	ld.uh r6,r1[pc<<1]
	ld.uh r6,lr[sp<<1]
	.text
	.global ld_sb2
ld_sb2:
	ld.sb pc,pc[pc<<0]
	ld.sb r12,r12[r12<<3]
	ld.sb r5,r5[r5<<2]
	ld.sb r4,r4[r4<<1]
	ld.sb lr,lr[lr<<1]
	ld.sb r9,r1[pc<<3]
	ld.sb r0,r3[r11<<1]
	ld.sb r10,r5[r5<<1]
	.text
	.global ld_ub5
ld_ub5:
	ld.ub pc,pc[pc<<0]
	ld.ub r12,r12[r12<<3]
	ld.ub r5,r5[r5<<2]
	ld.ub r4,r4[r4<<1]
	ld.ub lr,lr[lr<<1]
	ld.ub r6,r12[r7<<3]
	ld.ub r2,r6[r12<<0]
	ld.ub r0,r7[r11<<1]
	.text
	.global st_d5
st_d5:
	st.d pc[pc<<0],r14
	st.d r12[r12<<3],r12
	st.d r5[r5<<2],r6
	st.d r4[r4<<1],r4
	st.d lr[lr<<1],lr
	st.d r1[r9<<1],r4
	st.d r10[r2<<1],r4
	st.d r12[r6<<0],lr
	.text
	.global st_w5
st_w5:
	st.w pc[pc<<0],pc
	st.w r12[r12<<3],r12
	st.w r5[r5<<2],r5
	st.w r4[r4<<1],r4
	st.w lr[lr<<1],lr
	st.w r1[r10<<0],r3
	st.w r0[r10<<1],r9
	st.w r4[r5<<3],pc
	.text
	.global st_h5
st_h5:
	st.h pc[pc<<0],pc
	st.h r12[r12<<3],r12
	st.h r5[r5<<2],r5
	st.h r4[r4<<1],r4
	st.h lr[lr<<1],lr
	st.h r2[r9<<0],r11
	st.h r5[r1<<2],r12
	st.h pc[r8<<2],r3
	.text
	.global st_b5
st_b5:
	st.b pc[pc<<0],pc
	st.b r12[r12<<3],r12
	st.b r5[r5<<2],r5
	st.b r4[r4<<1],r4
	st.b lr[lr<<1],lr
	st.b r1[r8<<1],r6
	st.b lr[lr<<3],r1
	st.b r5[r0<<2],pc
	.text
	.global divs
divs:
	divs pc,pc,pc
	divs r12,r12,r12
	divs r5,r5,r5
	divs r4,r4,r4
	divs lr,lr,lr
	divs r3,pc,pc
	divs r9,r12,r2
	divs r7,r4,r1
	.text
	.global add1
add1:
	add pc,pc
	add r12,r12
	add r5,r5
	add r4,r4
	add lr,lr
	add r12,r9
	add r6,r3
	add r10,r12
	.text
	.global sub1
sub1:
	sub pc,pc
	sub r12,r12
	sub r5,r5
	sub r4,r4
	sub lr,lr
	sub lr,r6
	sub r0,sp
	sub r6,r12
	.text
	.global rsub1
rsub1:
	rsub pc,pc
	rsub r12,r12
	rsub r5,r5
	rsub r4,r4
	rsub lr,lr
	rsub r11,sp
	rsub r7,r4
	rsub r9,r1
	.text
	.global cp1
cp1:
	cp pc,pc
	cp r12,r12
	cp r5,r5
	cp r4,r4
	cp lr,lr
	cp r6,r2
	cp r0,r9
	cp r3,sp
	.text
	.global or1
or1:
	or pc,pc
	or r12,r12
	or r5,r5
	or r4,r4
	or lr,lr
	or r4,r9
	or r11,r4
	or r4,r0
	.text
	.global eor1
eor1:
	eor pc,pc
	eor r12,r12
	eor r5,r5
	eor r4,r4
	eor lr,lr
	eor r12,r11
	eor r0,r1
	eor r5,pc
	.text
	.global and1
and1:
	and pc,pc
	and r12,r12
	and r5,r5
	and r4,r4
	and lr,lr
	and r8,r1
	and r0,sp
	and r10,r5
	.text
	.global tst
tst:
	tst pc,pc
	tst r12,r12
	tst r5,r5
	tst r4,r4
	tst lr,lr
	tst r0,r12
	tst r10,r6
	tst sp,r4
	.text
	.global andn
andn:
	andn pc,pc
	andn r12,r12
	andn r5,r5
	andn r4,r4
	andn lr,lr
	andn r9,r12
	andn r11,sp
	andn r12,r5
	.text
	.global mov3
mov3:
	mov pc,pc
	mov r12,r12
	mov r5,r5
	mov r4,r4
	mov lr,lr
	mov r5,r9
	mov r11,r11
	mov r2,lr
	.text
	.global st_w1
st_w1:
	st.w pc++,pc
	st.w r12++,r12
	st.w r5++,r5
	st.w r4++,r4
	st.w lr++,lr
	st.w r1++,r11
	st.w sp++,r0
	st.w sp++,r1
	.text
	.global st_h1
st_h1:
	st.h pc++,pc
	st.h r12++,r12
	st.h r5++,r5
	st.h r4++,r4
	st.h lr++,lr
	st.h r12++,sp
	st.h r7++,lr
	st.h r7++,r4
	.text
	.global st_b1
st_b1:
	st.b pc++,pc
	st.b r12++,r12
	st.b r5++,r5
	st.b r4++,r4
	st.b lr++,lr
	st.b r9++,sp
	st.b r1++,sp
	st.b r0++,r4
	.text
	.global st_w2
st_w2:
	st.w --pc,pc
	st.w --r12,r12
	st.w --r5,r5
	st.w --r4,r4
	st.w --lr,lr
	st.w --r1,r7
	st.w --r3,r9
	st.w --r5,r5
	.text
	.global st_h2
st_h2:
	st.h --pc,pc
	st.h --r12,r12
	st.h --r5,r5
	st.h --r4,r4
	st.h --lr,lr
	st.h --r5,r7
	st.h --r8,r8
	st.h --r7,r2
	.text
	.global st_b2
st_b2:
	st.b --pc,pc
	st.b --r12,r12
	st.b --r5,r5
	st.b --r4,r4
	st.b --lr,lr
	st.b --sp,sp
	st.b --sp,r11
	st.b --r4,r5
	.text
	.global ld_w1
ld_w1:
	ld.w pc,pc++
	ld.w r12,r12++
	ld.w r5,r5++
	ld.w r4,r4++
	ld.w lr,lr++
	ld.w r3,r7++
	ld.w r3,lr++
	ld.w r12,r5++
	.text
	.global ld_sh1
ld_sh1:
	ld.sh pc,pc++
	ld.sh r12,r12++
	ld.sh r5,r5++
	ld.sh r4,r4++
	ld.sh lr,lr++
	ld.sh r11,r2++
	ld.sh r2,r8++
	ld.sh r7,r6++
	.text
	.global ld_uh1
ld_uh1:
	ld.uh pc,pc++
	ld.uh r12,r12++
	ld.uh r5,r5++
	ld.uh r4,r4++
	ld.uh lr,lr++
	ld.uh r6,r7++
	ld.uh r10,r11++
	ld.uh lr,r4++
	.text
	.global ld_ub1
ld_ub1:
	ld.ub pc,pc++
	ld.ub r12,r12++
	ld.ub r5,r5++
	ld.ub r4,r4++
	ld.ub lr,lr++
	ld.ub r8,lr++
	ld.ub r12,r12++
	ld.ub r11,r10++
	.text
	.global ld_w2
ld_w2:
	ld.w pc,--pc
	ld.w r12,--r12
	ld.w r5,--r5
	ld.w r4,--r4
	ld.w lr,--lr
	ld.w r10,--lr
	ld.w r12,--r9
	ld.w r6,--r5
	.text
	.global ld_sh2
ld_sh2:
	ld.sh pc,--pc
	ld.sh r12,--r12
	ld.sh r5,--r5
	ld.sh r4,--r4
	ld.sh lr,--lr
	ld.sh pc,--r10
	ld.sh r6,--r3
	ld.sh r4,--r6
	.text
	.global ld_uh2
ld_uh2:
	ld.uh pc,--pc
	ld.uh r12,--r12
	ld.uh r5,--r5
	ld.uh r4,--r4
	ld.uh lr,--lr
	ld.uh r3,--r2
	ld.uh r1,--r0
	ld.uh r2,--r9
	.text
	.global ld_ub2
ld_ub2:
	ld.ub pc,--pc
	ld.ub r12,--r12
	ld.ub r5,--r5
	ld.ub r4,--r4
	ld.ub lr,--lr
	ld.ub r1,--r1
	ld.ub r0,--r6
	ld.ub r2,--r7
	.text
	.global ld_ub3
ld_ub3:
	ld.ub pc,pc[0]
	ld.ub r12,r12[7]
	ld.ub r5,r5[4]
	ld.ub r4,r4[3]
	ld.ub lr,lr[1]
	ld.ub r6,r9[6]
	ld.ub r2,lr[4]
	ld.ub r1,r8[0]
	.text
	.global sub3_sp
sub3_sp:
	sub sp,0
	sub sp,-4
	sub sp,-512
	sub sp,508
	sub sp,4
	sub sp,44
	sub sp,8
	sub sp,348
	.text
	.global sub3
sub3:
	sub pc,0
	sub r12,-1
	sub r5,-128
	sub r4,127
	sub lr,1
	sub r6,-41
	sub r4,37
	sub r12,56
	.text
	.global mov1
mov1:
	mov pc,0
	mov r12,-1
	mov r5,-128
	mov r4,127
	mov lr,1
	mov pc,14
	mov r6,-100
	mov lr,-122
	.text
	.global lddsp
lddsp:
	lddsp pc,sp[0]
	lddsp r12,sp[508]
	lddsp r5,sp[256]
	lddsp r4,sp[252]
	lddsp lr,sp[4]
	lddsp lr,sp[256]
	lddsp r12,sp[20]
	lddsp r9,sp[472]
	.text
	.global lddpc
lddpc:
	lddpc pc,pc[0]
	lddpc r0,pc[508]
	lddpc r8,pc[256]
	lddpc r7,pc[252]
	lddpc lr,pc[4]
	lddpc sp,pc[472]
	lddpc r6,pc[120]
	lddpc r11,pc[28]
	.text
	.global stdsp
stdsp:
	stdsp sp[0],pc
	stdsp sp[508],r12
	stdsp sp[256],r5
	stdsp sp[252],r4
	stdsp sp[4],lr
	stdsp sp[304],pc
	stdsp sp[256],r0
	stdsp sp[336],r5
	.text
	.global cp2
cp2:
	cp pc,0
	cp r12,-1
	cp r5,-32
	cp r4,31
	cp lr,1
	cp r8,3
	cp lr,16
	cp r7,-26
	.text
	.global acr
acr:
	acr pc
	acr r12
	acr r5
	acr r4
	acr lr
	acr r2
	acr r12
	acr pc
	.text
	.global scr
scr:
	scr pc
	scr r12
	scr r5
	scr r4
	scr lr
	scr pc
	scr r6
	scr r1
	.text
	.global cpc0
cpc0:
	cpc pc
	cpc r12
	cpc r5
	cpc r4
	cpc lr
	cpc pc
	cpc r4
	cpc r9
	.text
	.global neg
neg:
	neg pc
	neg r12
	neg r5
	neg r4
	neg lr
	neg r7
	neg r1
	neg r9
	.text
	.global abs
abs:
	abs pc
	abs r12
	abs r5
	abs r4
	abs lr
	abs r6
	abs r6
	abs r4
	.text
	.global castu_b
castu_b:
	castu.b pc
	castu.b r12
	castu.b r5
	castu.b r4
	castu.b lr
	castu.b r7
	castu.b sp
	castu.b r9
	.text
	.global casts_b
casts_b:
	casts.b pc
	casts.b r12
	casts.b r5
	casts.b r4
	casts.b lr
	casts.b r11
	casts.b r1
	casts.b r10
	.text
	.global castu_h
castu_h:
	castu.h pc
	castu.h r12
	castu.h r5
	castu.h r4
	castu.h lr
	castu.h r10
	castu.h r11
	castu.h r1
	.text
	.global casts_h
casts_h:
	casts.h pc
	casts.h r12
	casts.h r5
	casts.h r4
	casts.h lr
	casts.h r0
	casts.h r5
	casts.h r9
	.text
	.global brev
brev:
	brev pc
	brev r12
	brev r5
	brev r4
	brev lr
	brev r5
	brev r10
	brev r8
	.text
	.global swap_h
swap_h:
	swap.h pc
	swap.h r12
	swap.h r5
	swap.h r4
	swap.h lr
	swap.h r7
	swap.h r0
	swap.h r8
	.text
	.global swap_b
swap_b:
	swap.b pc
	swap.b r12
	swap.b r5
	swap.b r4
	swap.b lr
	swap.b r10
	swap.b r12
	swap.b r1
	.text
	.global swap_bh
swap_bh:
	swap.bh pc
	swap.bh r12
	swap.bh r5
	swap.bh r4
	swap.bh lr
	swap.bh r9
	swap.bh r4
	swap.bh r1
	.text
	.global One_s_compliment
One_s_compliment:
	com pc
	com r12
	com r5
	com r4
	com lr
	com r2
	com r2
	com r7
	.text
	.global tnbz
tnbz:
	tnbz pc
	tnbz r12
	tnbz r5
	tnbz r4
	tnbz lr
	tnbz r8
	tnbz r12
	tnbz pc
	.text
	.global rol
rol:
	rol pc
	rol r12
	rol r5
	rol r4
	rol lr
	rol r10
	rol r9
	rol r5
	.text
	.global ror
ror:
	ror pc
	ror r12
	ror r5
	ror r4
	ror lr
	ror r8
	ror r4
	ror r7
	.text
	.global icall
icall:
	icall pc
	icall r12
	icall r5
	icall r4
	icall lr
	icall r3
	icall r1
	icall r3
	.text
	.global mustr
mustr:
	mustr pc
	mustr r12
	mustr r5
	mustr r4
	mustr lr
	mustr r1
	mustr r4
	mustr r12
	.text
	.global musfr
musfr:
	musfr pc
	musfr r12
	musfr r5
	musfr r4
	musfr lr
	musfr r11
	musfr r12
	musfr r2
	.text
	.global ret_cond
ret_cond:
	reteq pc
	retal r12
	retls r5
	retpl r4
	retne lr
	retgt r0
	retgt r12
	retge r10
	.text
	.global sr_cond
sr_cond:
	sreq pc
	sral r12
	srls r5
	srpl r4
	srne lr
	srlt r0
	sral sp
	srge r9
	.text
	.global ld_w3
ld_w3:
	ld.w pc,pc[0]
	ld.w r12,r12[124]
	ld.w r5,r5[64]
	ld.w r4,r4[60]
	ld.w lr,lr[4]
	ld.w sp,r2[52]
	ld.w r9,r1[8]
	ld.w r5,sp[60]
	.text
	.global ld_sh3
ld_sh3:
	ld.sh pc,pc[0]
	ld.sh r12,r12[14]
	ld.sh r5,r5[8]
	ld.sh r4,r4[6]
	ld.sh lr,lr[2]
	ld.sh r4,r2[8]
	ld.sh sp,lr[10]
	ld.sh r2,r11[2]
	.text
	.global ld_uh3
ld_uh3:
	ld.uh pc,pc[0]
	ld.uh r12,r12[14]
	ld.uh r5,r5[8]
	ld.uh r4,r4[6]
	ld.uh lr,lr[2]
	ld.uh r10,r0[10]
	ld.uh r8,r11[8]
	ld.uh r10,r2[12]
	.text
	.global st_w3
st_w3:
	st.w pc[0],pc
	st.w r12[60],r12
	st.w r5[32],r5
	st.w r4[28],r4
	st.w lr[4],lr
	st.w r7[44],r11
	st.w r2[24],r6
	st.w r4[12],r9
	.text
	.global st_h3
st_h3:
	st.h pc[0],pc
	st.h r12[14],r12
	st.h r5[8],r5
	st.h r4[6],r4
	st.h lr[2],lr
	st.h lr[10],r12
	st.h r6[4],r0
	st.h r5[12],sp
	.text
	.global st_b3
st_b3:
	st.b pc[0],pc
	st.b r12[7],r12
	st.b r5[4],r5
	st.b r4[3],r4
	st.b lr[1],lr
	st.b r12[6],r9
	st.b r2[3],lr
	st.b r1[3],r11
	.text
	.global ldd
ldd:
	ld.d r0,pc
	ld.d r14,r12
	ld.d r8,r5
	ld.d r6,r4
	ld.d r2,lr
	ld.d r14,r7
	ld.d r4,r4
	ld.d r14,pc
	.text
	.global ldd_postinc
ldd_postinc:
	ld.d r0,pc++
	ld.d r14,r12++
	ld.d r8,r5++
	ld.d r6,r4++
	ld.d r2,lr++
	ld.d r14,r5++
	ld.d r12,r11++
	ld.d r2,r12++
	.text
	.global ldd_predec
ldd_predec:
	ld.d r0,--pc
	ld.d r14,--r12
	ld.d r8,--r5
	ld.d r6,--r4
	ld.d r2,--lr
	ld.d r8,--r0
	ld.d r10,--pc
	ld.d r2,--r4
	.text
	.global std
std:
	st.d pc,r0
	st.d r12,r14
	st.d r5,r8
	st.d r4,r6
	st.d lr,r2
	st.d r0,r12
	st.d sp,r4
	st.d r12,r12
	.text
	.global std_postinc
std_postinc:
	st.d pc++,r0
	st.d r12++,r14
	st.d r5++,r8
	st.d r4++,r6
	st.d lr++,r2
	st.d sp++,r6
	st.d r10++,r6
	st.d r7++,r2
	.text
	.global std_predec
std_predec:
	st.d --pc,r0
	st.d --r12,r14
	st.d --r5,r8
	st.d --r4,r6
	st.d --lr,r2
	st.d --r3,r6
	st.d --lr,r2
	st.d --r0,r4
	.text
	.global mul
mul:
	mul pc,pc
	mul r12,r12
	mul r5,r5
	mul r4,r4
	mul lr,lr
	mul r10,lr
	mul r0,r8
	mul r8,r5
	.text
	.global asr_imm5
asr_imm5:
	asr pc,0
	asr r12,31
	asr r5,16
	asr r4,15
	asr lr,1
	asr r6,23
	asr r6,18
	asr r5,8
	.text
	.global lsl_imm5
lsl_imm5:
	lsl pc,0
	lsl r12,31
	lsl r5,16
	lsl r4,15
	lsl lr,1
	lsl r12,13
	lsl r6,16
	lsl r1,25
	.text
	.global lsr_imm5
lsr_imm5:
	lsr pc,0
	lsr r12,31
	lsr r5,16
	lsr r4,15
	lsr lr,1
	lsr r0,1
	lsr r8,10
	lsr r7,26
	.text
	.global sbr
sbr:
	sbr pc,0
	sbr r12,31
	sbr r5,16
	sbr r4,15
	sbr lr,1
	sbr r8,31
	sbr r6,22
	sbr r1,23
	.text
	.global cbr
cbr:
	cbr pc,0
	cbr r12,31
	cbr r5,16
	cbr r4,15
	cbr lr,1
	cbr r12,10
	cbr r7,22
	cbr r8,9
	.text
	.global brc1
brc1:
	breq 0
	brpl -2
	brge -256
	brcs 254
	brne 2
	brcs 230
	breq -18
	breq 12
	.text
	.global rjmp
rjmp:
	rjmp 0
	rjmp -2
	rjmp -1024
	rjmp 1022
	rjmp 2
	rjmp -962
	rjmp 14
	rjmp -516
	.text
	.global rcall1
rcall1:
	rcall 0
	rcall -2
	rcall -1024
	rcall 1022
	rcall 2
	rcall 216
	rcall -530
	rcall -972
	.text
	.global acall
acall:
	acall 0
	acall 1020
	acall 512
	acall 508
	acall 4
	acall 356
	acall 304
	acall 172
	.text
	.global scall
scall:
	scall
	scall
	scall
	scall
	scall
	scall
	scall
	scall
	.text
	.global popm
popm:
	/* popm with no argument fails currently */
	popm pc
	popm r0-r11,pc,r12=-1
	popm lr
	popm r0-r11,pc,r12=1
	popm r0-r3
	popm r4-r10,pc
	popm r0-r3,r11,pc,r12=0
	popm r0-r7,r10-r12,lr
	.text
	.global pushm
pushm:
	pushm pc
	pushm r0-r12,lr,pc
	pushm pc
	pushm r0-r12,lr
	pushm r0-r3
	pushm r8-r10,lr,pc
	pushm r0-r3,r10
	pushm r8-r9,r12
	.text
	.global popm_n
popm_n:
	popm pc
	popm r0-r11,pc,r12=-1
	popm lr
	popm r0-r11,pc,r12=1
	popm r0-r3
	popm r4-r10,pc
	popm r0-r3,r11,pc,r12=0
	popm r0-r7,r10-r12,lr
	.text
	.global pushm_n
pushm_n:
	pushm pc
	pushm r0-r12,lr,pc
	pushm pc
	pushm r0-r12,lr
	pushm r0-r3
	pushm r8-r10,lr,pc
	pushm r0-r3,r10
	pushm r8-r9,r12
	.text
	.global csrfcz
csrfcz:
	csrfcz 0
	csrfcz 31
	csrfcz 16
	csrfcz 15
	csrfcz 1
	csrfcz 5
	csrfcz 13
	csrfcz 23
	.text
	.global ssrf
ssrf:
	ssrf 0
	ssrf 31
	ssrf 16
	ssrf 15
	ssrf 1
	ssrf 29
	ssrf 13
	ssrf 13
	.text
	.global csrf
csrf:
	csrf 0
	csrf 31
	csrf 16
	csrf 15
	csrf 1
	csrf 10
	csrf 15
	csrf 11
	.text
	.global rete
rete:
	rete
	.text
	.global rets
rets:
	rets
	.text
	.global retd
retd:
	retd
	.text
	.global retj
retj:
	retj
	.text
	.global tlbr
tlbr:
	tlbr
	.text
	.global tlbs
tlbs:
	tlbs
	.text
	.global tlbw
tlbw:
	tlbw
	.text
	.global breakpoint
breakpoint:
	breakpoint
	.text
	.global incjosp
incjosp:
	incjosp 1
	incjosp 2
	incjosp 3
	incjosp 4
	incjosp -4
	incjosp -3
	incjosp -2
	incjosp -1
	.text
	.global nop
nop:
	nop
	.text
	.global popjc
popjc:
	popjc
	.text
	.global pushjc
pushjc:
	pushjc
	.text
	.global add2
add2:
	add pc,pc,pc<<0
	add r12,r12,r12<<3
	add r5,r5,r5<<2
	add r4,r4,r4<<1
	add lr,lr,lr<<1
	add r0,r12,r0<<1
	add r9,r12,r4<<0
	add r12,r12,r7<<2
	.text
	.global sub2
sub2:
	sub pc,pc,pc<<0
	sub r12,r12,r12<<3
	sub r5,r5,r5<<2
	sub r4,r4,r4<<1
	sub lr,lr,lr<<1
	sub sp,r3,r4<<0
	sub r3,r7,r3<<0
	sub sp,r10,sp<<1
	.text
	.global divu
divu:
	divu pc,pc,pc
	divu r12,r12,r12
	divu r5,r5,r5
	divu r4,r4,r4
	divu lr,lr,lr
	divu sp,r4,pc
	divu r5,r5,sp
	divu r10,sp,r0
	.text
	.global addhh_w
addhh_w:
	addhh.w pc,pc:b,pc:b
	addhh.w r12,r12:t,r12:t
	addhh.w r5,r5:t,r5:t
	addhh.w r4,r4:b,r4:b
	addhh.w lr,lr:t,lr:t
	addhh.w r0,r0:b,r3:b
	addhh.w lr,r12:t,r7:b
	addhh.w r3,r10:t,r2:b
	.text
	.global subhh_w
subhh_w:
	subhh.w pc,pc:b,pc:b
	subhh.w r12,r12:t,r12:t
	subhh.w r5,r5:t,r5:t
	subhh.w r4,r4:b,r4:b
	subhh.w lr,lr:t,lr:t
	subhh.w r10,r1:t,r7:b
	subhh.w pc,r10:t,lr:t
	subhh.w r3,r0:t,r12:b
	.text
	.global adc
adc:
	adc pc,pc,pc
	adc r12,r12,r12
	adc r5,r5,r5
	adc r4,r4,r4
	adc lr,lr,lr
	adc r4,r0,r7
	adc sp,r4,r3
	adc r2,r12,r0
	.text
	.global sbc
sbc:
	sbc pc,pc,pc
	sbc r12,r12,r12
	sbc r5,r5,r5
	sbc r4,r4,r4
	sbc lr,lr,lr
	sbc r6,r7,r9
	sbc r0,r8,r5
	sbc r1,r0,r4
	.text
	.global mul_2
mul_2:
	mul pc,pc,pc
	mul r12,r12,r12
	mul r5,r5,r5
	mul r4,r4,r4
	mul lr,lr,lr
	mul pc,r0,r0
	mul r8,pc,lr
	mul r4,r12,pc
	.text
	.global mac
mac:
	mac pc,pc,pc
	mac r12,r12,r12
	mac r5,r5,r5
	mac r4,r4,r4
	mac lr,lr,lr
	mac r10,r4,r0
	mac r7,lr,r0
	mac r2,r9,r12
	.text
	.global mulsd
mulsd:
	muls.d pc,pc,pc
	muls.d r12,r12,r12
	muls.d r5,r5,r5
	muls.d r4,r4,r4
	muls.d lr,lr,lr
	muls.d r2,r8,lr
	muls.d r4,r0,r11
	muls.d r5,lr,r6
	.text
	.global macsd
macsd:
	macs.d r0,pc,pc
	macs.d r14,r12,r12
	macs.d r8,r5,r5
	macs.d r6,r4,r4
	macs.d r2,lr,lr
	macs.d r8,r1,r9
	macs.d r14,r8,r8
	macs.d r4,r3,r12
	.text
	.global mulud
mulud:
	mulu.d r0,pc,pc
	mulu.d r14,r12,r12
	mulu.d r8,r5,r5
	mulu.d r6,r4,r4
	mulu.d r2,lr,lr
	mulu.d r6,r5,r0
	mulu.d r4,r6,r1
	mulu.d r8,r8,r2
	.text
	.global macud
macud:
	macu.d r0,pc,pc
	macu.d r14,r12,r12
	macu.d r8,r5,r5
	macu.d r6,r4,r4
	macu.d r2,lr,lr
	macu.d r6,sp,r11
	macu.d r2,r4,r8
	macu.d r6,r10,r9
	.text
	.global asr_1
asr_1:
	asr pc,pc,pc
	asr r12,r12,r12
	asr r5,r5,r5
	asr r4,r4,r4
	asr lr,lr,lr
	asr pc,r6,pc
	asr r0,r6,r12
	asr r4,sp,r0
	.text
	.global lsl_1
lsl_1:
	lsl pc,pc,pc
	lsl r12,r12,r12
	lsl r5,r5,r5
	lsl r4,r4,r4
	lsl lr,lr,lr
	lsl lr,r5,lr
	lsl r5,pc,r3
	lsl r1,pc,r9
	.text
	.global lsr_1
lsr_1:
	lsr pc,pc,pc
	lsr r12,r12,r12
	lsr r5,r5,r5
	lsr r4,r4,r4
	lsr lr,lr,lr
	lsr r2,r4,r1
	lsr r5,r1,r6
	lsr sp,r6,r7
	.text
	.global xchg
xchg:
	xchg pc,pc,pc
	xchg r12,r12,r12
	xchg r5,r5,r5
	xchg r4,r4,r4
	xchg lr,lr,lr
	xchg lr,r4,sp
	xchg r1,r5,r12
	xchg lr,r12,r0
	.text
	.global max
max:
	max pc,pc,pc
	max r12,r12,r12
	max r5,r5,r5
	max r4,r4,r4
	max lr,lr,lr
	max lr,r2,sp
	max r4,r10,r9
	max lr,r9,lr
	.text
	.global min
min:
	min pc,pc,pc
	min r12,r12,r12
	min r5,r5,r5
	min r4,r4,r4
	min lr,lr,lr
	min r9,r7,r8
	min sp,r5,r5
	min r4,r1,r4
	.text
	.global addabs
addabs:
	addabs pc,pc,pc
	addabs r12,r12,r12
	addabs r5,r5,r5
	addabs r4,r4,r4
	addabs lr,lr,lr
	addabs r7,r10,r0
	addabs r9,r9,r7
	addabs r2,r8,r12
	.text
	.global mulnhh_w
mulnhh_w:
	mulnhh.w pc,pc:b,pc:b
	mulnhh.w r12,r12:t,r12:t
	mulnhh.w r5,r5:t,r5:t
	mulnhh.w r4,r4:b,r4:b
	mulnhh.w lr,lr:t,lr:t
	mulnhh.w r11,sp:t,r9:b
	mulnhh.w sp,r4:b,lr:t
	mulnhh.w r12,r2:t,r11:b
	.text
	.global mulnwh_d
mulnwh_d:
	mulnwh.d r0,pc,pc:b
	mulnwh.d r14,r12,r12:t
	mulnwh.d r8,r5,r5:t
	mulnwh.d r6,r4,r4:b
	mulnwh.d r2,lr,lr:t
	mulnwh.d r14,r3,r2:t
	mulnwh.d r4,r5,r9:b
	mulnwh.d r12,r4,r4:t
	.text
	.global machh_w
machh_w:
	machh.w pc,pc:b,pc:b
	machh.w r12,r12:t,r12:t
	machh.w r5,r5:t,r5:t
	machh.w r4,r4:b,r4:b
	machh.w lr,lr:t,lr:t
	machh.w lr,r5:b,r1:t
	machh.w r9,r6:b,r7:b
	machh.w r5,lr:t,r12:b
	.text
	.global machh_d
machh_d:
	machh.d r0,pc:b,pc:b
	machh.d r14,r12:t,r12:t
	machh.d r8,r5:t,r5:t
	machh.d r6,r4:b,r4:b
	machh.d r2,lr:t,lr:t
	machh.d r10,r0:b,r8:b
	machh.d r14,r4:b,r5:t
	machh.d r8,r0:b,r4:t
	.text
	.global macsathh_w
macsathh_w:
	macsathh.w pc,pc:b,pc:b
	macsathh.w r12,r12:t,r12:t
	macsathh.w r5,r5:t,r5:t
	macsathh.w r4,r4:b,r4:b
	macsathh.w lr,lr:t,lr:t
	macsathh.w r7,r7:t,pc:t
	macsathh.w r4,r2:t,r4:b
	macsathh.w r4,r8:t,r3:t
	.text
	.global mulhh_w
mulhh_w:
	mulhh.w pc,pc:b,pc:b
	mulhh.w r12,r12:t,r12:t
	mulhh.w r5,r5:t,r5:t
	mulhh.w r4,r4:b,r4:b
	mulhh.w lr,lr:t,lr:t
	mulhh.w r7,r4:t,r9:b
	mulhh.w pc,r3:t,r7:t
	mulhh.w pc,r4:b,r9:t
	.text
	.global mulsathh_h
mulsathh_h:
	mulsathh.h pc,pc:b,pc:b
	mulsathh.h r12,r12:t,r12:t
	mulsathh.h r5,r5:t,r5:t
	mulsathh.h r4,r4:b,r4:b
	mulsathh.h lr,lr:t,lr:t
	mulsathh.h r3,r1:b,sp:b
	mulsathh.h r11,lr:t,r11:b
	mulsathh.h r8,r8:b,r11:t
	.text
	.global mulsathh_w
mulsathh_w:
	mulsathh.w pc,pc:b,pc:b
	mulsathh.w r12,r12:t,r12:t
	mulsathh.w r5,r5:t,r5:t
	mulsathh.w r4,r4:b,r4:b
	mulsathh.w lr,lr:t,lr:t
	mulsathh.w lr,r11:t,r6:b
	mulsathh.w r6,r6:b,r7:t
	mulsathh.w r10,r2:b,r3:b
	.text
	.global mulsatrndhh_h
mulsatrndhh_h:
	mulsatrndhh.h pc,pc:b,pc:b
	mulsatrndhh.h r12,r12:t,r12:t
	mulsatrndhh.h r5,r5:t,r5:t
	mulsatrndhh.h r4,r4:b,r4:b
	mulsatrndhh.h lr,lr:t,lr:t
	mulsatrndhh.h r11,r6:b,r9:b
	mulsatrndhh.h r11,r3:b,r8:t
	mulsatrndhh.h r5,sp:t,r7:t
	.text
	.global mulsatrndwh_w
mulsatrndwh_w:
	mulsatrndwh.w pc,pc,pc:b
	mulsatrndwh.w r12,r12,r12:t
	mulsatrndwh.w r5,r5,r5:t
	mulsatrndwh.w r4,r4,r4:b
	mulsatrndwh.w lr,lr,lr:t
	mulsatrndwh.w r5,r12,r0:b
	mulsatrndwh.w r7,r10,pc:b
	mulsatrndwh.w r10,r8,r5:t
	.text
	.global macwh_d
macwh_d:
	macwh.d r0,pc,pc:b
	macwh.d r14,r12,r12:t
	macwh.d r8,r5,r5:t
	macwh.d r6,r4,r4:b
	macwh.d r2,lr,lr:t
	macwh.d r4,r10,r12:t
	macwh.d r4,r7,sp:b
	macwh.d r14,r9,r11:b
	.text
	.global mulwh_d
mulwh_d:
	mulwh.d r0,pc,pc:b
	mulwh.d r14,r12,r12:t
	mulwh.d r8,r5,r5:t
	mulwh.d r6,r4,r4:b
	mulwh.d r2,lr,lr:t
	mulwh.d r12,r5,r1:b
	mulwh.d r0,r1,r3:t
	mulwh.d r0,r9,r2:b
	.text
	.global mulsatwh_w
mulsatwh_w:
	mulsatwh.w pc,pc,pc:b
	mulsatwh.w r12,r12,r12:t
	mulsatwh.w r5,r5,r5:t
	mulsatwh.w r4,r4,r4:b
	mulsatwh.w lr,lr,lr:t
	mulsatwh.w r11,pc,r10:t
	mulsatwh.w sp,r12,r9:t
	mulsatwh.w r0,r3,r2:t
	.text
	.global ldw7
ldw7:
	ld.w pc,pc[pc:b<<2]
	ld.w r12,r12[r12:t<<2]
	ld.w r5,r5[r5:u<<2]
	ld.w r4,r4[r4:l<<2]
	ld.w lr,lr[lr:l<<2]
	ld.w r9,r10[r6:l<<2]
	ld.w r2,r10[r10:b<<2]
	ld.w r11,r5[pc:b<<2]
	.text
	.global satadd_w
satadd_w:
	satadd.w pc,pc,pc
	satadd.w r12,r12,r12
	satadd.w r5,r5,r5
	satadd.w r4,r4,r4
	satadd.w lr,lr,lr
	satadd.w r4,r8,r11
	satadd.w r3,r12,r6
	satadd.w r3,lr,r9
	.text
	.global satsub_w1
satsub_w1:
	satsub.w pc,pc,pc
	satsub.w r12,r12,r12
	satsub.w r5,r5,r5
	satsub.w r4,r4,r4
	satsub.w lr,lr,lr
	satsub.w r8,sp,r0
	satsub.w r9,r8,r4
	satsub.w pc,lr,r2
	.text
	.global satadd_h
satadd_h:
	satadd.h pc,pc,pc
	satadd.h r12,r12,r12
	satadd.h r5,r5,r5
	satadd.h r4,r4,r4
	satadd.h lr,lr,lr
	satadd.h r7,r3,r9
	satadd.h r1,r0,r2
	satadd.h r1,r4,lr
	.text
	.global satsub_h
satsub_h:
	satsub.h pc,pc,pc
	satsub.h r12,r12,r12
	satsub.h r5,r5,r5
	satsub.h r4,r4,r4
	satsub.h lr,lr,lr
	satsub.h lr,lr,r3
	satsub.h r11,r6,r5
	satsub.h r3,sp,r0
	.text
	.global mul3
mul3:
	mul pc,pc,0
	mul r12,r12,-1
	mul r5,r5,-128
	mul r4,r4,127
	mul lr,lr,1
	mul r12,r2,-7
	mul r1,pc,95
	mul r4,r6,19
	.text
	.global rsub2
rsub2:
	rsub pc,pc,0
	rsub r12,r12,-1
	rsub r5,r5,-128
	rsub r4,r4,127
	rsub lr,lr,1
	rsub r9,lr,96
	rsub r11,r1,56
	rsub r0,r7,-87
	.text
	.global clz
clz:
	clz pc,pc
	clz r12,r12
	clz r5,r5
	clz r4,r4
	clz lr,lr
	clz r2,r3
	clz r5,r11
	clz pc,r3
	.text
	.global cpc1
cpc1:
	cpc pc,pc
	cpc r12,r12
	cpc r5,r5
	cpc r4,r4
	cpc lr,lr
	cpc pc,r4
	cpc r5,r9
	cpc r6,r7
	.text
	.global asr3
asr3:
	asr pc,pc,0
	asr r12,r12,31
	asr r5,r5,16
	asr r4,r4,15
	asr lr,lr,1
	asr r4,r11,19
	asr sp,pc,26
	asr r11,sp,8
	.text
	.global lsl3
lsl3:
	lsl pc,pc,0
	lsl r12,r12,31
	lsl r5,r5,16
	lsl r4,r4,15
	lsl lr,lr,1
	lsl r8,r10,17
	lsl r2,lr,3
	lsl lr,r11,14
	.text
	.global lsr3
lsr3:
	lsr pc,pc,0
	lsr r12,r12,31
	lsr r5,r5,16
	lsr r4,r4,15
	lsr lr,lr,1
	lsr r4,r3,31
	lsr pc,r9,14
	lsr r3,r0,6
/*	.text
	.global extract_b
extract_b:
	extract.b pc,pc:b
	extract.b r12,r12:t
	extract.b r5,r5:u
	extract.b r4,r4:l
	extract.b lr,lr:l
	extract.b r2,r5:l
	extract.b r12,r3:l
	extract.b sp,r3:l
	.text
	.global insert_b
insert_b:
	insert.b pc:b,pc
	insert.b r12:t,r12
	insert.b r5:u,r5
	insert.b r4:l,r4
	insert.b lr:l,lr
	insert.b r12:u,r3
	insert.b r10:l,lr
	insert.b r11:l,r12
	.text
	.global extract_h
extract_h:
	extract.h pc,pc:b
	extract.h r12,r12:t
	extract.h r5,r5:t
	extract.h r4,r4:b
	extract.h lr,lr:t
	extract.h r11,lr:b
	extract.h r10,r0:b
	extract.h r11,r12:b
	.text
	.global insert_h
insert_h:
	insert.h pc:b,pc
	insert.h r12:t,r12
	insert.h r5:t,r5
	insert.h r4:b,r4
	insert.h lr:t,lr
	insert.h r12:t,r11
	insert.h r7:b,r6
	insert.h r1:t,r11 */
	.text
	.global movc1
movc1:
	moveq pc,pc
	moval r12,r12
	movls r5,r5
	movpl r4,r4
	movne lr,lr
	movne pc,r11
	movmi r10,r2
	movls r8,r12
	.text
	.global padd_h
padd_h:
	padd.h pc,pc,pc
	padd.h r12,r12,r12
	padd.h r5,r5,r5
	padd.h r4,r4,r4
	padd.h lr,lr,lr
	padd.h r8,r2,r7
	padd.h r0,r0,r3
	padd.h sp,r11,r6
	.text
	.global psub_h
psub_h:
	psub.h pc,pc,pc
	psub.h r12,r12,r12
	psub.h r5,r5,r5
	psub.h r4,r4,r4
	psub.h lr,lr,lr
	psub.h lr,r6,r8
	psub.h r0,r1,sp
	psub.h pc,pc,sp
	.text
	.global paddx_h
paddx_h:
	paddx.h pc,pc,pc
	paddx.h r12,r12,r12
	paddx.h r5,r5,r5
	paddx.h r4,r4,r4
	paddx.h lr,lr,lr
	paddx.h pc,pc,r1
	paddx.h r10,r4,r5
	paddx.h r5,pc,r2
	.text
	.global psubx_h
psubx_h:
	psubx.h pc,pc,pc
	psubx.h r12,r12,r12
	psubx.h r5,r5,r5
	psubx.h r4,r4,r4
	psubx.h lr,lr,lr
	psubx.h r5,r12,r5
	psubx.h r3,r8,r3
	psubx.h r5,r2,r3
	.text
	.global padds_sh
padds_sh:
	padds.sh pc,pc,pc
	padds.sh r12,r12,r12
	padds.sh r5,r5,r5
	padds.sh r4,r4,r4
	padds.sh lr,lr,lr
	padds.sh r9,lr,r2
	padds.sh r6,r8,r1
	padds.sh r6,r4,r10
	.text
	.global psubs_sh
psubs_sh:
	psubs.sh pc,pc,pc
	psubs.sh r12,r12,r12
	psubs.sh r5,r5,r5
	psubs.sh r4,r4,r4
	psubs.sh lr,lr,lr
	psubs.sh r6,lr,r11
	psubs.sh r2,r12,r4
	psubs.sh r0,r9,r0
	.text
	.global paddxs_sh
paddxs_sh:
	paddxs.sh pc,pc,pc
	paddxs.sh r12,r12,r12
	paddxs.sh r5,r5,r5
	paddxs.sh r4,r4,r4
	paddxs.sh lr,lr,lr
	paddxs.sh r0,r3,r9
	paddxs.sh pc,r10,r11
	paddxs.sh pc,r10,pc
	.text
	.global psubxs_sh
psubxs_sh:
	psubxs.sh pc,pc,pc
	psubxs.sh r12,r12,r12
	psubxs.sh r5,r5,r5
	psubxs.sh r4,r4,r4
	psubxs.sh lr,lr,lr
	psubxs.sh r7,r4,r4
	psubxs.sh r7,r8,r3
	psubxs.sh pc,r6,r5
	.text
	.global padds_uh
padds_uh:
	padds.uh pc,pc,pc
	padds.uh r12,r12,r12
	padds.uh r5,r5,r5
	padds.uh r4,r4,r4
	padds.uh lr,lr,lr
	padds.uh r12,r11,r7
	padds.uh r7,r8,lr
	padds.uh r6,r9,r7
	.text
	.global psubs_uh
psubs_uh:
	psubs.uh pc,pc,pc
	psubs.uh r12,r12,r12
	psubs.uh r5,r5,r5
	psubs.uh r4,r4,r4
	psubs.uh lr,lr,lr
	psubs.uh lr,r10,r6
	psubs.uh sp,r2,pc
	psubs.uh r2,r9,r2
	.text
	.global paddxs_uh
paddxs_uh:
	paddxs.uh pc,pc,pc
	paddxs.uh r12,r12,r12
	paddxs.uh r5,r5,r5
	paddxs.uh r4,r4,r4
	paddxs.uh lr,lr,lr
	paddxs.uh r7,r9,r5
	paddxs.uh r9,r1,r4
	paddxs.uh r5,r2,r3
	.text
	.global psubxs_uh
psubxs_uh:
	psubxs.uh pc,pc,pc
	psubxs.uh r12,r12,r12
	psubxs.uh r5,r5,r5
	psubxs.uh r4,r4,r4
	psubxs.uh lr,lr,lr
	psubxs.uh sp,r5,sp
	psubxs.uh sp,r6,r6
	psubxs.uh r3,r11,r8
	.text
	.global paddh_sh
paddh_sh:
	paddh.sh pc,pc,pc
	paddh.sh r12,r12,r12
	paddh.sh r5,r5,r5
	paddh.sh r4,r4,r4
	paddh.sh lr,lr,lr
	paddh.sh r12,sp,r3
	paddh.sh pc,r5,r3
	paddh.sh r8,r8,sp
	.text
	.global psubh_sh
psubh_sh:
	psubh.sh pc,pc,pc
	psubh.sh r12,r12,r12
	psubh.sh r5,r5,r5
	psubh.sh r4,r4,r4
	psubh.sh lr,lr,lr
	psubh.sh r1,r5,r8
	psubh.sh r7,r3,r6
	psubh.sh r4,r3,r3
	.text
	.global paddxh_sh
paddxh_sh:
	paddxh.sh pc,pc,pc
	paddxh.sh r12,r12,r12
	paddxh.sh r5,r5,r5
	paddxh.sh r4,r4,r4
	paddxh.sh lr,lr,lr
	paddxh.sh r6,r0,r4
	paddxh.sh r9,r8,r9
	paddxh.sh r3,r0,sp
	.text
	.global psubxh_sh
psubxh_sh:
	psubxh.sh pc,pc,pc
	psubxh.sh r12,r12,r12
	psubxh.sh r5,r5,r5
	psubxh.sh r4,r4,r4
	psubxh.sh lr,lr,lr
	psubxh.sh r4,pc,r12
	psubxh.sh r8,r4,r6
	psubxh.sh r12,r9,r4
	.text
	.global paddsub_h
paddsub_h:
	paddsub.h pc,pc:b,pc:b
	paddsub.h r12,r12:t,r12:t
	paddsub.h r5,r5:t,r5:t
	paddsub.h r4,r4:b,r4:b
	paddsub.h lr,lr:t,lr:t
	paddsub.h r5,r2:t,lr:b
	paddsub.h r7,r1:b,r8:b
	paddsub.h r6,r10:t,r5:t
	.text
	.global psubadd_h
psubadd_h:
	psubadd.h pc,pc:b,pc:b
	psubadd.h r12,r12:t,r12:t
	psubadd.h r5,r5:t,r5:t
	psubadd.h r4,r4:b,r4:b
	psubadd.h lr,lr:t,lr:t
	psubadd.h r9,r11:t,r8:t
	psubadd.h r10,r7:t,lr:t
	psubadd.h r6,pc:t,pc:b
	.text
	.global paddsubs_sh
paddsubs_sh:
	paddsubs.sh pc,pc:b,pc:b
	paddsubs.sh r12,r12:t,r12:t
	paddsubs.sh r5,r5:t,r5:t
	paddsubs.sh r4,r4:b,r4:b
	paddsubs.sh lr,lr:t,lr:t
	paddsubs.sh r0,lr:t,r0:b
	paddsubs.sh r9,r2:t,r4:t
	paddsubs.sh r12,r9:t,sp:t
	.text
	.global psubadds_sh
psubadds_sh:
	psubadds.sh pc,pc:b,pc:b
	psubadds.sh r12,r12:t,r12:t
	psubadds.sh r5,r5:t,r5:t
	psubadds.sh r4,r4:b,r4:b
	psubadds.sh lr,lr:t,lr:t
	psubadds.sh pc,lr:b,r1:t
	psubadds.sh r11,r3:b,r12:b
	psubadds.sh r10,r2:t,r8:t
	.text
	.global paddsubs_uh
paddsubs_uh:
	paddsubs.uh pc,pc:b,pc:b
	paddsubs.uh r12,r12:t,r12:t
	paddsubs.uh r5,r5:t,r5:t
	paddsubs.uh r4,r4:b,r4:b
	paddsubs.uh lr,lr:t,lr:t
	paddsubs.uh r9,r2:b,r3:b
	paddsubs.uh sp,sp:b,r7:t
	paddsubs.uh lr,r0:b,r10:t
	.text
	.global psubadds_uh
psubadds_uh:
	psubadds.uh pc,pc:b,pc:b
	psubadds.uh r12,r12:t,r12:t
	psubadds.uh r5,r5:t,r5:t
	psubadds.uh r4,r4:b,r4:b
	psubadds.uh lr,lr:t,lr:t
	psubadds.uh r12,r9:t,pc:t
	psubadds.uh r8,r6:b,r8:b
	psubadds.uh r8,r8:b,r4:b
	.text
	.global paddsubh_sh
paddsubh_sh:
	paddsubh.sh pc,pc:b,pc:b
	paddsubh.sh r12,r12:t,r12:t
	paddsubh.sh r5,r5:t,r5:t
	paddsubh.sh r4,r4:b,r4:b
	paddsubh.sh lr,lr:t,lr:t
	paddsubh.sh r8,r9:t,r9:b
	paddsubh.sh r0,sp:t,r1:t
	paddsubh.sh r3,r1:b,r0:t
	.text
	.global psubaddh_sh
psubaddh_sh:
	psubaddh.sh pc,pc:b,pc:b
	psubaddh.sh r12,r12:t,r12:t
	psubaddh.sh r5,r5:t,r5:t
	psubaddh.sh r4,r4:b,r4:b
	psubaddh.sh lr,lr:t,lr:t
	psubaddh.sh r7,r3:t,r10:b
	psubaddh.sh r7,r2:t,r1:t
	psubaddh.sh r11,r3:b,r6:b
	.text
	.global padd_b
padd_b:
	padd.b pc,pc,pc
	padd.b r12,r12,r12
	padd.b r5,r5,r5
	padd.b r4,r4,r4
	padd.b lr,lr,lr
	padd.b r2,r6,pc
	padd.b r8,r9,r12
	padd.b r5,r12,r3
	.text
	.global psub_b
psub_b:
	psub.b pc,pc,pc
	psub.b r12,r12,r12
	psub.b r5,r5,r5
	psub.b r4,r4,r4
	psub.b lr,lr,lr
	psub.b r0,r12,pc
	psub.b r7,sp,r10
	psub.b r5,sp,r12
	.text
	.global padds_sb
padds_sb:
	padds.sb pc,pc,pc
	padds.sb r12,r12,r12
	padds.sb r5,r5,r5
	padds.sb r4,r4,r4
	padds.sb lr,lr,lr
	padds.sb sp,r11,r4
	padds.sb r11,r10,r11
	padds.sb r5,r12,r6
	.text
	.global psubs_sb
psubs_sb:
	psubs.sb pc,pc,pc
	psubs.sb r12,r12,r12
	psubs.sb r5,r5,r5
	psubs.sb r4,r4,r4
	psubs.sb lr,lr,lr
	psubs.sb r7,r6,r8
	psubs.sb r12,r10,r9
	psubs.sb pc,r11,r0
	.text
	.global padds_ub
padds_ub:
	padds.ub pc,pc,pc
	padds.ub r12,r12,r12
	padds.ub r5,r5,r5
	padds.ub r4,r4,r4
	padds.ub lr,lr,lr
	padds.ub r3,r2,r11
	padds.ub r10,r8,r1
	padds.ub r11,r8,r10
	.text
	.global psubs_ub
psubs_ub:
	psubs.ub pc,pc,pc
	psubs.ub r12,r12,r12
	psubs.ub r5,r5,r5
	psubs.ub r4,r4,r4
	psubs.ub lr,lr,lr
	psubs.ub r0,r2,r7
	psubs.ub lr,r5,r3
	psubs.ub r6,r7,r9
	.text
	.global paddh_ub
paddh_ub:
	paddh.ub pc,pc,pc
	paddh.ub r12,r12,r12
	paddh.ub r5,r5,r5
	paddh.ub r4,r4,r4
	paddh.ub lr,lr,lr
	paddh.ub lr,r1,r0
	paddh.ub r2,r7,r7
	paddh.ub r2,r1,r2
	.text
	.global psubh_ub
psubh_ub:
	psubh.ub pc,pc,pc
	psubh.ub r12,r12,r12
	psubh.ub r5,r5,r5
	psubh.ub r4,r4,r4
	psubh.ub lr,lr,lr
	psubh.ub r0,r1,r6
	psubh.ub r4,lr,r10
	psubh.ub r9,r8,r1
	.text
	.global pmax_ub
pmax_ub:
	pmax.ub pc,pc,pc
	pmax.ub r12,r12,r12
	pmax.ub r5,r5,r5
	pmax.ub r4,r4,r4
	pmax.ub lr,lr,lr
	pmax.ub pc,r2,r11
	pmax.ub r12,r1,r1
	pmax.ub r5,r2,r0
	.text
	.global pmax_sh
pmax_sh:
	pmax.sh pc,pc,pc
	pmax.sh r12,r12,r12
	pmax.sh r5,r5,r5
	pmax.sh r4,r4,r4
	pmax.sh lr,lr,lr
	pmax.sh lr,r6,r12
	pmax.sh r2,pc,r5
	pmax.sh pc,r2,r7
	.text
	.global pmin_ub
pmin_ub:
	pmin.ub pc,pc,pc
	pmin.ub r12,r12,r12
	pmin.ub r5,r5,r5
	pmin.ub r4,r4,r4
	pmin.ub lr,lr,lr
	pmin.ub r8,r1,r5
	pmin.ub r1,r8,r3
	pmin.ub r0,r2,r7
	.text
	.global pmin_sh
pmin_sh:
	pmin.sh pc,pc,pc
	pmin.sh r12,r12,r12
	pmin.sh r5,r5,r5
	pmin.sh r4,r4,r4
	pmin.sh lr,lr,lr
	pmin.sh r8,r4,r10
	pmin.sh lr,r10,r12
	pmin.sh r2,r6,r2
	.text
	.global pavg_ub
pavg_ub:
	pavg.ub pc,pc,pc
	pavg.ub r12,r12,r12
	pavg.ub r5,r5,r5
	pavg.ub r4,r4,r4
	pavg.ub lr,lr,lr
	pavg.ub r0,r1,r6
	pavg.ub r8,r3,r6
	pavg.ub pc,r12,r10
	.text
	.global pavg_sh
pavg_sh:
	pavg.sh pc,pc,pc
	pavg.sh r12,r12,r12
	pavg.sh r5,r5,r5
	pavg.sh r4,r4,r4
	pavg.sh lr,lr,lr
	pavg.sh r9,pc,sp
	pavg.sh pc,sp,r3
	pavg.sh r6,r1,r9
	.text
	.global pabs_sb
pabs_sb:
	pabs.sb pc,pc
	pabs.sb r12,r12
	pabs.sb r5,r5
	pabs.sb r4,r4
	pabs.sb lr,lr
	pabs.sb r11,r6
	pabs.sb lr,r9
	pabs.sb sp,r7
	.text
	.global pabs_sh
pabs_sh:
	pabs.sh pc,pc
	pabs.sh r12,r12
	pabs.sh r5,r5
	pabs.sh r4,r4
	pabs.sh lr,lr
	pabs.sh pc,r3
	pabs.sh r5,r7
	pabs.sh r4,r0
	.text
	.global psad
psad:
	psad pc,pc,pc
	psad r12,r12,r12
	psad r5,r5,r5
	psad r4,r4,r4
	psad lr,lr,lr
	psad r9,r11,r11
	psad lr,r4,sp
	psad lr,r4,r5
	.text
	.global pasr_b
pasr_b:
	pasr.b pc,pc,0
	pasr.b r12,r12,7
	pasr.b r5,r5,4
	pasr.b r4,r4,3
	pasr.b lr,lr,1
	pasr.b pc,r7,1
	pasr.b sp,lr,6
	pasr.b sp,r3,2
	.text
	.global plsl_b
plsl_b:
	plsl.b pc,pc,0
	plsl.b r12,r12,7
	plsl.b r5,r5,4
	plsl.b r4,r4,3
	plsl.b lr,lr,1
	plsl.b r2,r11,4
	plsl.b r8,r5,7
	plsl.b pc,r0,2
	.text
	.global plsr_b
plsr_b:
	plsr.b pc,pc,0
	plsr.b r12,r12,7
	plsr.b r5,r5,4
	plsr.b r4,r4,3
	plsr.b lr,lr,1
	plsr.b r12,r1,2
	plsr.b r6,pc,7
	plsr.b r12,r11,2
	.text
	.global pasr_h
pasr_h:
	pasr.h pc,pc,0
	pasr.h r12,r12,15
	pasr.h r5,r5,8
	pasr.h r4,r4,7
	pasr.h lr,lr,1
	pasr.h r0,r11,10
	pasr.h r4,r6,8
	pasr.h r6,r2,4
	.text
	.global plsl_h
plsl_h:
	plsl.h pc,pc,0
	plsl.h r12,r12,15
	plsl.h r5,r5,8
	plsl.h r4,r4,7
	plsl.h lr,lr,1
	plsl.h r5,r10,9
	plsl.h sp,lr,8
	plsl.h r0,lr,7
	.text
	.global plsr_h
plsr_h:
	plsr.h pc,pc,0
	plsr.h r12,r12,15
	plsr.h r5,r5,8
	plsr.h r4,r4,7
	plsr.h lr,lr,1
	plsr.h r11,r0,15
	plsr.h lr,r3,3
	plsr.h r8,lr,10
	.text
	.global packw_sh
packw_sh:
	packw.sh pc,pc,pc
	packw.sh r12,r12,r12
	packw.sh r5,r5,r5
	packw.sh r4,r4,r4
	packw.sh lr,lr,lr
	packw.sh sp,r11,r10
	packw.sh r8,r2,r12
	packw.sh r8,r1,r5
	.text
	.global punpckub_h
punpckub_h:
	punpckub.h pc,pc:b
	punpckub.h r12,r12:t
	punpckub.h r5,r5:t
	punpckub.h r4,r4:b
	punpckub.h lr,lr:t
	punpckub.h r6,r1:t
	punpckub.h lr,r5:b
	punpckub.h lr,r2:t
	.text
	.global punpcksb_h
punpcksb_h:
	punpcksb.h pc,pc:b
	punpcksb.h r12,r12:t
	punpcksb.h r5,r5:t
	punpcksb.h r4,r4:b
	punpcksb.h lr,lr:t
	punpcksb.h r4,r7:t
	punpcksb.h r6,lr:b
	punpcksb.h r12,r12:t
	.text
	.global packsh_ub
packsh_ub:
	packsh.ub pc,pc,pc
	packsh.ub r12,r12,r12
	packsh.ub r5,r5,r5
	packsh.ub r4,r4,r4
	packsh.ub lr,lr,lr
	packsh.ub r3,r6,r3
	packsh.ub r8,r0,r3
	packsh.ub r9,r3,lr
	.text
	.global packsh_sb
packsh_sb:
	packsh.sb pc,pc,pc
	packsh.sb r12,r12,r12
	packsh.sb r5,r5,r5
	packsh.sb r4,r4,r4
	packsh.sb lr,lr,lr
	packsh.sb r6,r8,r1
	packsh.sb lr,r9,r8
	packsh.sb sp,r6,r6
	.text
	.global andl
andl:
	andl pc,0
	andl r12,65535
	andl r5,32768
	andl r4,32767
	andl lr,1
	andl pc,23128
	andl r8,47262
	andl r7,13719
	.text
	.global andl_coh
andl_coh:
	andl pc,0,COH
	andl r12,65535,COH
	andl r5,32768,COH
	andl r4,32767,COH
	andl lr,1,COH
	andl r6,22753,COH
	andl r0,40653,COH
	andl r4,48580,COH
	.text
	.global andh
andh:
	andh pc,0
	andh r12,65535
	andh r5,32768
	andh r4,32767
	andh lr,1
	andh r12,52312
	andh r3,8675
	andh r2,42987
	.text
	.global andh_coh
andh_coh:
	andh pc,0,COH
	andh r12,65535,COH
	andh r5,32768,COH
	andh r4,32767,COH
	andh lr,1,COH
	andh r11,34317,COH
	andh r8,52982,COH
	andh r10,23683,COH
	.text
	.global orl
orl:
	orl pc,0
	orl r12,65535
	orl r5,32768
	orl r4,32767
	orl lr,1
	orl sp,16766
	orl r0,21181
	orl pc,44103
	.text
	.global orh
orh:
	orh pc,0
	orh r12,65535
	orh r5,32768
	orh r4,32767
	orh lr,1
	orh r8,28285
	orh r12,30492
	orh r1,59930
	.text
	.global eorl
eorl:
	eorl pc,0
	eorl r12,65535
	eorl r5,32768
	eorl r4,32767
	eorl lr,1
	eorl r4,51129
	eorl r6,64477
	eorl r1,20913
	.text
	.global eorh
eorh:
	eorh pc,0
	eorh r12,65535
	eorh r5,32768
	eorh r4,32767
	eorh lr,1
	eorh r0,11732
	eorh r10,38069
	eorh r9,57130
	.text
	.global mcall
mcall:
	mcall pc[0]
	mcall r12[-4]
	mcall r5[-131072]
	mcall r4[131068]
	mcall lr[4]
	mcall sp[61180]
	mcall r4[-35000]
	mcall r0[9924]
	.text
	.global pref
pref:
	pref pc[0]
	pref r12[-1]
	pref r5[-32768]
	pref r4[32767]
	pref lr[1]
	pref r7[7748]
	pref r7[-7699]
	pref r2[-25892]
	.text
	.global cache
cache:
	cache pc[0],0
	cache r12[-1],31
	cache r5[-1024],16
	cache r4[1023],15
	cache lr[1],1
	cache r3[-964],17
	cache r4[-375],22
	cache r3[-888],17
	.text
	.global sub4
sub4:
	sub pc,0
	sub r12,-1
	sub r5,-1048576
	sub r4,1048575
	sub lr,1
	sub r2,-619156
	sub lr,461517
	sub r8,-185051
	.text
	.global cp3
cp3:
	cp pc,0
	cp r12,-1
	cp r5,-1048576
	cp r4,1048575
	cp lr,1
	cp r1,124078
	cp r0,-378909
	cp r4,-243180
	.text
	.global mov2
mov2:
	mov pc,0
	mov r12,-1
	mov r5,-1048576
	mov r4,1048575
	mov lr,1
	mov r5,-317021
	mov sp,-749164
	mov r5,940179
	.text
	.global brc2
brc2:
	breq 0
	bral -2
	brls -2097152
	brpl 2097150
	brne 2
	brhi -1796966
	brqs 1321368
	brls -577434
	.text
	.global rcall2
rcall2:
	rcall 0
	rcall -2
	rcall -2097152
	rcall 2097150
	rcall 2
	rcall 496820
	rcall 1085092
	rcall -1058
	.text
	.global sub5
sub5:
	sub pc,pc,0
	sub r12,r12,-1
	sub r5,r5,-32768
	sub r4,r4,32767
	sub lr,lr,1
	sub pc,pc,-12744
	sub r7,r7,-27365
	sub r2,r9,-17358
	.text
	.global satsub_w2
satsub_w2:
	satsub.w pc,pc,0
	satsub.w r12,r12,-1
	satsub.w r5,r5,-32768
	satsub.w r4,r4,32767
	satsub.w lr,lr,1
	satsub.w r2,lr,-2007
	satsub.w r7,r12,-784
	satsub.w r4,r7,23180
	.text
	.global ld_d4
ld_d4:
	ld.d r0,pc[0]
	ld.d r14,r12[-1]
	ld.d r8,r5[-32768]
	ld.d r6,r4[32767]
	ld.d r2,lr[1]
	ld.d r14,r11[14784]
	ld.d r6,r9[-18905]
	ld.d r2,r3[-6355]
	.text
	.global ld_w4
ld_w4:
	ld.w pc,pc[0]
	ld.w r12,r12[-1]
	ld.w r5,r5[-32768]
	ld.w r4,r4[32767]
	ld.w lr,lr[1]
	ld.w r0,r12[-22133]
	ld.w sp,pc[-20521]
	/* ld.w r3,r5[29035] */
	nop
	.text
	.global ld_sh4
ld_sh4:
	ld.sh pc,pc[0]
	ld.sh r12,r12[-1]
	ld.sh r5,r5[-32768]
	ld.sh r4,r4[32767]
	ld.sh lr,lr[1]
	ld.sh r6,r10[30930]
	ld.sh r6,r10[21973]
	/* ld.sh r11,r10[-2058] */
	nop
	.text
	.global ld_uh4
ld_uh4:
	ld.uh pc,pc[0]
	ld.uh r12,r12[-1]
	ld.uh r5,r5[-32768]
	ld.uh r4,r4[32767]
	ld.uh lr,lr[1]
	ld.uh r1,r9[-13354]
	ld.uh lr,r11[21337]
	/* ld.uh r2,lr[-25370] */
	nop
	.text
	.global ld_sb1
ld_sb1:
	ld.sb pc,pc[0]
	ld.sb r12,r12[-1]
	ld.sb r5,r5[-32768]
	ld.sb r4,r4[32767]
	ld.sb lr,lr[1]
	ld.sb r7,sp[-28663]
	ld.sb r2,r1[-5879]
	ld.sb r12,r3[18734]
	.text
	.global ld_ub4
ld_ub4:
	ld.ub pc,pc[0]
	ld.ub r12,r12[-1]
	ld.ub r5,r5[-32768]
	ld.ub r4,r4[32767]
	ld.ub lr,lr[1]
	ld.ub pc,r4[8277]
	ld.ub r5,r12[19172]
	ld.ub r10,lr[26347]
	.text
	.global st_d4
st_d4:
	st.d pc[0],r0
	st.d r12[-1],r14
	st.d r5[-32768],r8
	st.d r4[32767],r6
	st.d lr[1],r2
	st.d r5[13200],r10
	st.d r5[9352],r10
	st.d r5[32373],r4
	.text
	.global st_w4
st_w4:
	st.w pc[0],pc
	st.w r12[-1],r12
	st.w r5[-32768],r5
	st.w r4[32767],r4
	st.w lr[1],lr
	st.w sp[6136],r7
	st.w r6[27087],r12
	/* st.w r3[20143],r7 */
	nop
	.text
	.global st_h4
st_h4:
	st.h pc[0],pc
	st.h r12[-1],r12
	st.h r5[-32768],r5
	st.h r4[32767],r4
	st.h lr[1],lr
	st.h r4[-9962],r7
	st.h r9[-16250],r3
	/* st.h r8[-28810],r7 */
	nop
	.text
	.global st_b4
st_b4:
	st.b pc[0],pc
	st.b r12[-1],r12
	st.b r5[-32768],r5
	st.b r4[32767],r4
	st.b lr[1],lr
	st.b r12[30102],r6
	st.b r5[28977],r1
	st.b r0[5470],r1
	.text
	.global mfsr
mfsr:
	mfsr pc,0
	mfsr r12,1020
	mfsr r5,512
	mfsr r4,508
	mfsr lr,4
	mfsr r2,696
	mfsr r4,260
	mfsr r10,1016
	.text
	.global mtsr
mtsr:
	mtsr 0,pc
	mtsr 1020,r12
	mtsr 512,r5
	mtsr 508,r4
	mtsr 4,lr
	mtsr 224,r10
	mtsr 836,r12
	mtsr 304,r9
	.text
	.global mfdr
mfdr:
	mfdr pc,0
	mfdr r12,1020
	mfdr r5,512
	mfdr r4,508
	mfdr lr,4
	mfdr r6,932
	mfdr r5,36
	mfdr r9,300
	.text
	.global mtdr
mtdr:
	mtdr 0,pc
	mtdr 1020,r12
	mtdr 512,r5
	mtdr 508,r4
	mtdr 4,lr
	mtdr 180,r8
	mtdr 720,r10
	mtdr 408,lr
	.text
	.global sleep
sleep:
	sleep 0
	sleep 255
	sleep 128
	sleep 127
	sleep 1
	sleep 254
	sleep 15
	sleep 43
	.text
	.global sync
sync:
	sync 0
	sync 255
	sync 128
	sync 127
	sync 1
	sync 166
	sync 230
	sync 180
	.text
	.global bld
bld:
	bld pc,0
	bld r12,31
	bld r5,16
	bld r4,15
	bld lr,1
	bld r9,15
	bld r0,4
	bld lr,26
	.text
	.global bst
bst:
	bst pc,0
	bst r12,31
	bst r5,16
	bst r4,15
	bst lr,1
	bst r10,28
	bst r0,3
	bst sp,2
	.text
	.global sats
sats:
	sats pc>>0,0
	sats r12>>31,31
	sats r5>>16,16
	sats r4>>15,15
	sats lr>>1,1
	sats r10>>3,19
	sats r10>>2,26
	sats r1>>20,1
	.text
	.global satu
satu:
	satu pc>>0,0
	satu r12>>31,31
	satu r5>>16,16
	satu r4>>15,15
	satu lr>>1,1
	satu pc>>5,7
	satu r7>>5,5
	satu r2>>26,19
	.text
	.global satrnds
satrnds:
	satrnds pc>>0,0
	satrnds r12>>31,31
	satrnds r5>>16,16
	satrnds r4>>15,15
	satrnds lr>>1,1
	satrnds r0>>21,19
	satrnds sp>>0,2
	satrnds r7>>6,29
	.text
	.global satrndu
satrndu:
	satrndu pc>>0,0
	satrndu r12>>31,31
	satrndu r5>>16,16
	satrndu r4>>15,15
	satrndu lr>>1,1
	satrndu r12>>0,26
	satrndu r4>>21,3
	satrndu r10>>3,16
	.text
	.global subfc
subfc:
	subfeq pc,0
	subfal r12,-1
	subfls r5,-128
	subfpl r4,127
	subfne lr,1
	subfls r10,8
	subfvc r11,99
	subfvs r2,73
	.text
	.global subc
subc:
	subeq pc,0
	subal r12,-1
	subls r5,-128
	subpl r4,127
	subne lr,1
	subls r12,118
	subvc lr,-12
	submi r4,-13
	.text
	.global movc2
movc2:
	moveq pc,0
	moval r12,-1
	movls r5,-128
	movpl r4,127
	movne lr,1
	movlt r3,-122
	movvc r8,2
	movne r7,-111
	.text
	.global cp_b
cp_b:
	cp.b pc,r0
	cp.b r0,pc
	cp.b r7,r8
	cp.b r8,r7
	.text
	.global cp_h
cp_h:
	cp.h pc,r0
	cp.h r0,pc
	cp.h r7,r8
	cp.h r8,r7
	.text
	.global ldm
ldm:
	ldm pc,r1-r6
	ldm r12,r0-r15
	ldm r5,r15
	ldm r4,r0-r14
	ldm lr,r0
	ldm r9,r1,r5,r14
	ldm r11,r2-r3,r5-r8,r15
	ldm r6,r0,r3,r9,r13,r15
	.text
	.global ldm_pu
ldm_pu:
	ldm pc++,r6-r9
	ldm r12++,r0-r15
	ldm r5++,r15
	ldm r4++,r0-r14
	ldm lr++,r0
	ldm r12++,r3-r5,r8,r10,r12,r14-r15
	ldm r10++,r2,r4-r6,r14-r15
	ldm r6++,r1,r3-r4,r9-r14
	.text
	.global ldmts
ldmts:
	ldmts pc,r7-r8
	ldmts r12,r0-r15
	ldmts r5,r15
	ldmts r4,r0-r14
	ldmts lr,r0
	ldmts r0,r1-r2,r11-r12
	ldmts lr,r0-r2,r4,r7-r8,r13-r14
	ldmts r12,r0-r1,r3-r5,r9,r14-r15
	.text
	.global ldmts_pu
ldmts_pu:
	ldmts pc++,r9
	ldmts r12++,r0-r15
	ldmts r5++,r15
	ldmts r4++,r0-r14
	ldmts lr++,r0
	ldmts sp++,r0,r2-r5,r7,r9,r11
	ldmts r5++,r1-r3,r7,r10-r11
	ldmts r8++,r2-r4,r7-r8,r13,r15
	.text
	.global stm
stm:
	stm pc,r7
	stm r12,r0-r15
	stm r5,r15
	stm r4,r0-r14
	stm lr,r0
	stm sp,r2-r3,r5,r8,r11,r14
	stm r4,r0-r4,r6,r10-r11,r14
	stm r9,r1,r5,r9,r12-r15
	.text
	.global stm_pu
stm_pu:
	stm --pc,r4-r6
	stm --r12,r0-r15
	stm --r5,r15
	stm --r4,r0-r14
	stm --lr,r0
	stm --r11,r0,r4-r9,r11-r15
	stm --r11,r0,r3,r9-r10,r12,r14
	stm --r6,r2,r8-r9,r13-r14
	.text
	.global stmts
stmts:
	stmts pc,r8
	stmts r12,r0-r15
	stmts r5,r15
	stmts r4,r0-r14
	stmts lr,r0
	stmts r1,r0-r1,r3-r4,r6,r9-r10,r14-r15
	stmts r3,r0,r6-r8,r10-r12
	stmts r11,r0,r4,r6-r7,r9-r10,r12,r14-r15
	.text
	.global stmts_pu
stmts_pu:
	stmts --pc,r6-r8
	stmts --r12,r0-r15
	stmts --r5,r15
	stmts --r4,r0-r14
	stmts --lr,r0
	stmts --r2,r0,r3-r4,r9-r10,r12-r13
	stmts --r3,r0-r1,r14-r15
	stmts --r0,r0,r2-r6,r10,r14
	.text
	.global ldins_h
ldins_h:
	ldins.h pc:b,pc[0]
	ldins.h r12:t,r12[-2]
	ldins.h r5:t,r5[-4096]
	ldins.h r4:b,r4[4094]
	ldins.h lr:t,lr[2]
	ldins.h r0:t,lr[1930]
	ldins.h r3:b,r7[-534]
	ldins.h r2:b,r12[-2252]
	.text
	.global ldins_b
ldins_b:
	ldins.b pc:b,pc[0]
	ldins.b r12:t,r12[-1]
	ldins.b r5:u,r5[-2048]
	ldins.b r4:l,r4[2047]
	ldins.b lr:l,lr[1]
	ldins.b r6:t,r4[-662]
	ldins.b r5:b,r1[-151]
	ldins.b r10:t,r11[-1923]
	.text
	.global ldswp_sh
ldswp_sh:
	ldswp.sh pc,pc[0]
	ldswp.sh r12,r12[-2]
	ldswp.sh r5,r5[-4096]
	ldswp.sh r4,r4[4094]
	ldswp.sh lr,lr[2]
	ldswp.sh r9,r10[3848]
	ldswp.sh r4,r12[-2040]
	ldswp.sh r10,r2[3088]
	.text
	.global ldswp_uh
ldswp_uh:
	ldswp.uh pc,pc[0]
	ldswp.uh r12,r12[-2]
	ldswp.uh r5,r5[-4096]
	ldswp.uh r4,r4[4094]
	ldswp.uh lr,lr[2]
	ldswp.uh r4,r9[3724]
	ldswp.uh lr,sp[-1672]
	ldswp.uh r8,r12[-3846]
	.text
	.global ldswp_w
ldswp_w:
	ldswp.w pc,pc[0]
	ldswp.w r12,r12[-4]
	ldswp.w r5,r5[-8192]
	ldswp.w r4,r4[8188]
	ldswp.w lr,lr[4]
	ldswp.w sp,r7[1860]
	ldswp.w pc,r5[-3324]
	ldswp.w r12,r10[-3296]
	.text
	.global stswp_h
stswp_h:
	stswp.h pc[0],pc
	stswp.h r12[-2],r12
	stswp.h r5[-4096],r5
	stswp.h r4[4094],r4
	stswp.h lr[2],lr
	stswp.h r7[64],r10
	stswp.h r10[3024],r2
	stswp.h r0[-2328],r10
	.text
	.global stswp_w
stswp_w:
	stswp.w pc[0],pc
	stswp.w r12[-4],r12
	stswp.w r5[-8192],r5
	stswp.w r4[8188],r4
	stswp.w lr[4],lr
	stswp.w pc[1156],r8
	stswp.w sp[7992],r10
	stswp.w r8[-1172],r5
	.text
	.global and2
and2:
	and pc,pc,pc<<0
	and r12,r12,r12<<31
	and r5,r5,r5<<16
	and r4,r4,r4<<15
	and lr,lr,lr<<1
	and r10,r2,r1<<1
	and r12,r8,r11<<27
	and r10,r7,r0<<3
	.text
	.global and3
and3:
	and pc,pc,pc>>0
	and r12,r12,r12>>31
	and r5,r5,r5>>16
	and r4,r4,r4>>15
	and lr,lr,lr>>1
	and r12,r8,r7>>17
	and pc,r4,r9>>20
	and r10,r9,r10>>12
	.text
	.global or2
or2:
	or pc,pc,pc<<0
	or r12,r12,r12<<31
	or r5,r5,r5<<16
	or r4,r4,r4<<15
	or lr,lr,lr<<1
	or r8,sp,r11<<29
	or pc,r9,r2<<28
	or r5,r1,r2<<3
	.text
	.global or3
or3:
	or pc,pc,pc>>0
	or r12,r12,r12>>31
	or r5,r5,r5>>16
	or r4,r4,r4>>15
	or lr,lr,lr>>1
	or r1,sp,sp>>2
	or r0,r1,r1>>29
	or r4,r12,r8>>8
	.text
	.global eor2
eor2:
	eor pc,pc,pc<<0
	eor r12,r12,r12<<31
	eor r5,r5,r5<<16
	eor r4,r4,r4<<15
	eor lr,lr,lr<<1
	eor r10,r9,r4<<11
	eor r4,r0,r1<<31
	eor r6,r2,r12<<13
	.text
	.global eor3
eor3:
	eor pc,pc,pc>>0
	eor r12,r12,r12>>31
	eor r5,r5,r5>>16
	eor r4,r4,r4>>15
	eor lr,lr,lr>>1
	eor r5,r5,r5>>22
	eor r10,r1,lr>>3
	eor r7,lr,sp>>26
	.text
	.global sthh_w2
sthh_w2:
	sthh.w pc[pc<<0],pc:b,pc:b
	sthh.w r12[r12<<3],r12:t,r12:t
	sthh.w r5[r5<<2],r5:t,r5:t
	sthh.w r4[r4<<1],r4:b,r4:b
	sthh.w lr[lr<<1],lr:t,lr:t
	sthh.w sp[r6<<3],r1:t,r12:t
	sthh.w r6[r6<<0],r9:t,r9:t
	sthh.w r10[r3<<0],r0:b,r11:t
	.text
	.global sthh_w1
sthh_w1:
	sthh.w pc[0],pc:b,pc:b
	sthh.w r12[1020],r12:t,r12:t
	sthh.w r5[512],r5:t,r5:t
	sthh.w r4[508],r4:b,r4:b
	sthh.w lr[4],lr:t,lr:t
	sthh.w r4[404],r9:t,r0:b
	sthh.w r8[348],r2:t,r10:b
	sthh.w sp[172],r9:b,r2:b
	.text
	.global cop
cop:
	cop cp0,cr0,cr0,cr0,0
	cop cp7,cr15,cr15,cr15,0x7f
	cop cp3,cr5,cr5,cr5,0x31
	cop cp2,cr4,cr4,cr4,0x30
	cop cp5,cr8,cr3,cr7,0x5a
	.text
	.global ldc_w1
ldc_w1:
	ldc.w cp0,cr0,r0[0]
	ldc.w cp7,cr15,pc[255<<2]
	ldc.w cp3,cr5,r5[128<<2]
	ldc.w cp2,cr4,r4[127<<2]
	ldc.w cp4,cr9,r13[36<<2]
	.text
	.global ldc_w2
ldc_w2:
	ldc.w cp0,cr0,--r0
	ldc.w cp7,cr15,--pc
	ldc.w cp3,cr5,--r5
	ldc.w cp2,cr4,--r4
	ldc.w cp4,cr9,--r13
	.text
	.global ldc_w3
ldc_w3:
	ldc.w cp0,cr0,r0[r0]
	ldc.w cp7,cr15,pc[pc<<3]
	ldc.w cp3,cr5,r5[r4<<2]
	ldc.w cp2,cr4,r4[r3<<1]
	ldc.w cp4,cr9,r13[r12<<0]
	.text
	.global ldc_d1
ldc_d1:
	ldc.d cp0,cr0,r0[0]
	ldc.d cp7,cr14,pc[255<<2]
	ldc.d cp3,cr6,r5[128<<2]
	ldc.d cp2,cr4,r4[127<<2]
	ldc.d cp4,cr8,r13[36<<2]
	.text
	.global ldc_d2
ldc_d2:
	ldc.d cp0,cr0,--r0
	ldc.d cp7,cr14,--pc
	ldc.d cp3,cr6,--r5
	ldc.d cp2,cr4,--r4
	ldc.d cp4,cr8,--r13
	.text
	.global ldc_d3
ldc_d3:
	ldc.d cp0,cr0,r0[r0]
	ldc.d cp7,cr14,pc[pc<<3]
	ldc.d cp3,cr6,r5[r4<<2]
	ldc.d cp2,cr4,r4[r3<<1]
	ldc.d cp4,cr8,r13[r12<<0]
	.text
	.global stc_w1
stc_w1:
	stc.w cp0,r0[0],cr0
	stc.w cp7,pc[255<<2],cr15
	stc.w cp3,r5[128<<2],cr5
	stc.w cp2,r4[127<<2],cr4
	stc.w cp4,r13[36<<2],cr9
	.text
	.global stc_w2
stc_w2:
	stc.w cp0,r0++,cr0
	stc.w cp7,pc++,cr15
	stc.w cp3,r5++,cr5
	stc.w cp2,r4++,cr4
	stc.w cp4,r13++,cr9
	.text
	.global stc_w3
stc_w3:
	stc.w cp0,r0[r0],cr0
	stc.w cp7,pc[pc<<3],cr15
	stc.w cp3,r5[r4<<2],cr5
	stc.w cp2,r4[r3<<1],cr4
	stc.w cp4,r13[r12<<0],cr9
	.text
	.global stc_d1
stc_d1:
	stc.d cp0,r0[0],cr0
	stc.d cp7,pc[255<<2],cr14
	stc.d cp3,r5[128<<2],cr6
	stc.d cp2,r4[127<<2],cr4
	stc.d cp4,r13[36<<2],cr8
	.text
	.global stc_d2
stc_d2:
	stc.d cp0,r0++,cr0
	stc.d cp7,pc++,cr14
	stc.d cp3,r5++,cr6
	stc.d cp2,r4++,cr4
	stc.d cp4,r13++,cr8
	.text
	.global stc_d3
stc_d3:
	stc.d cp0,r0[r0],cr0
	stc.d cp7,pc[pc<<3],cr14
	stc.d cp3,r5[r4<<2],cr6
	stc.d cp2,r4[r3<<1],cr4
	stc.d cp4,r13[r12<<0],cr8
	.text
	.global ldc0_w
ldc0_w:
	ldc0.w cr0,r0[0]
	ldc0.w cr15,pc[4095<<2]
	ldc0.w cr5,r5[2048<<2]
	ldc0.w cr4,r4[2047<<2]
	ldc0.w cr9,r13[147<<2]
	.text
	.global ldc0_d
ldc0_d:
	ldc0.d cr0,r0[0]
	ldc0.d cr14,pc[4095<<2]
	ldc0.d cr6,r5[2048<<2]
	ldc0.d cr4,r4[2047<<2]
	ldc0.d cr8,r13[147<<2]
	.text
	.global stc0_w
stc0_w:
	stc0.w r0[0],cr0
	stc0.w pc[4095<<2],cr15
	stc0.w r5[2048<<2],cr5
	stc0.w r4[2047<<2],cr4
	stc0.w r13[147<<2],cr9
	.text
	.global stc0_d
stc0_d:
	stc0.d r0[0],cr0
	stc0.d pc[4095<<2],cr14
	stc0.d r5[2048<<2],cr6
	stc0.d r4[2047<<2],cr4
	stc0.d r13[147<<2],cr8
	.text
	.global memc
memc:
	memc 0, 0
	memc -4, 31
	memc -65536, 16
	memc 65532, 15
	.text
	.global mems
mems:
	mems 0, 0
	mems -4, 31
	mems -65536, 16
	mems 65532, 15
	.text
	.global memt
memt:
	memt 0, 0
	memt -4, 31
	memt -65536, 16
	memt 65532, 15

	.text
	.global stcond
stcond:
	stcond r0[0], r0
	stcond pc[-1], pc
	stcond r8[-32768], r7
	stcond r7[32767], r8
	stcond r5[0x1234], r10

ldcm_w:
	ldcm.w cp0,pc,cr0-cr7
	ldcm.w cp7,r0,cr0
	ldcm.w cp4,r4++,cr0-cr6
	ldcm.w cp3,r7,cr7
	ldcm.w cp1,r12++,cr1,cr4-cr6
	ldcm.w cp0,pc,cr8-cr15
	ldcm.w cp7,r0,cr8
	ldcm.w cp4,r4++,cr8-cr14
	ldcm.w cp3,r7,cr15
	ldcm.w cp1,r12++,cr9,cr12-cr14

ldcm_d:
	ldcm.d cp0,pc,cr0-cr15
	ldcm.d cp7,r0,cr0,cr1
	ldcm.d cp4,r4++,cr0-cr13
	ldcm.d cp3,r7,cr14-cr15
	ldcm.d cp2,r12++,cr0-cr3,cr8-cr9,cr14-cr15

stcm_w:
	stcm.w cp0,pc,cr0-cr7
	stcm.w cp7,r0,cr0
	stcm.w cp4,--r4,cr0-cr6
	stcm.w cp3,r7,cr7
	stcm.w cp1,--r12,cr1,cr4-cr6
	stcm.w cp0,pc,cr8-cr15
	stcm.w cp7,r0,cr8
	stcm.w cp4,--r4,cr8-cr14
	stcm.w cp3,r7,cr15
	stcm.w cp1,--r12,cr9,cr12-cr14

stcm_d:
	stcm.d cp0,pc,cr0-cr15
	stcm.d cp7,r0,cr0,cr1
	stcm.d cp4,--r4,cr0-cr13
	stcm.d cp3,r7,cr14-cr15
	stcm.d cp2,--r12,cr0-cr3,cr8-cr9,cr14-cr15

mvcr_w:
	mvcr.w cp7,pc,cr15
	mvcr.w cp0,r0,cr0
	mvcr.w cp0,pc,cr15
	mvcr.w cp7,r0,cr15
	mvcr.w cp7,pc,cr0
	mvcr.w cp4,r7,cr8
	mvcr.w cp3,r8,cr7

mvcr_d:
	mvcr.d cp7,lr,cr14
	mvcr.d cp0,r0,cr0
	mvcr.d cp0,lr,cr14
	mvcr.d cp7,r0,cr14
	mvcr.d cp7,lr,cr0
	mvcr.d cp4,r6,cr8
	mvcr.d cp3,r8,cr6

mvrc_w:
	mvrc.w cp7,cr15,pc
	mvrc.w cp0,cr0,r0
	mvrc.w cp0,cr15,pc
	mvrc.w cp7,cr15,r0
	mvrc.w cp7,cr0,pc
	mvrc.w cp4,cr8,r7
	mvrc.w cp3,cr7,r8

mvrc_d:
	mvrc.d cp7,cr14,lr
	mvrc.d cp0,cr0,r0
	mvrc.d cp0,cr14,lr
	mvrc.d cp7,cr14,r0
	mvrc.d cp7,cr0,lr
	mvrc.d cp4,cr8,r6
	mvrc.d cp3,cr6,r8

bfexts:
	bfexts pc,pc,31,31
	bfexts r0,r0,0,0
	bfexts r0,pc,31,31
	bfexts pc,r0,31,31
	bfexts pc,pc,0,31
	bfexts pc,pc,31,0
	bfexts r7,r8,15,16
	bfexts r8,r7,16,15

bfextu:
	bfextu pc,pc,31,31
	bfextu r0,r0,0,0
	bfextu r0,pc,31,31
	bfextu pc,r0,31,31
	bfextu pc,pc,0,31
	bfextu pc,pc,31,0
	bfextu r7,r8,15,16
	bfextu r8,r7,16,15

bfins:
	bfins pc,pc,31,31
	bfins r0,r0,0,0
	bfins r0,pc,31,31
	bfins pc,r0,31,31
	bfins pc,pc,0,31
	bfins pc,pc,31,0
	bfins r7,r8,15,16
	bfins r8,r7,16,15

rsubc:
	rsubeq pc,0
	rsubal r12,-1
	rsubls r5,-128
	rsubpl r4,127
	rsubne lr,1
	rsubls r12,118
	rsubvc lr,-12
	rsubmi r4,-13

addc:
	addeq pc,pc,pc
	addal r12,r12,r12
	addls r5,r5,r5
	addpl r4,r4,r4
	addne lr,lr,lr
	addls r10,r2,r1
	addvc r12,r8,r11
	addmi r10,r7,r0   

subc2:
	subeq pc,pc,pc
	subal r12,r12,r12
	subls r5,r5,r5
	subpl r4,r4,r4
	subne lr,lr,lr
	subls r10,r2,r1
	subvc r12,r8,r11
	submi r10,r7,r0   

andc:
	andeq pc,pc,pc
	andal r12,r12,r12
	andls r5,r5,r5
	andpl r4,r4,r4
	andne lr,lr,lr
	andls r10,r2,r1
	andvc r12,r8,r11
	andmi r10,r7,r0   

orc:
	oreq pc,pc,pc
	oral r12,r12,r12
	orls r5,r5,r5
	orpl r4,r4,r4
	orne lr,lr,lr
	orls r10,r2,r1
	orvc r12,r8,r11
	ormi r10,r7,r0   

eorc:
	eoreq pc,pc,pc
	eoral r12,r12,r12
	eorls r5,r5,r5
	eorpl r4,r4,r4
	eorne lr,lr,lr
	eorls r10,r2,r1
	eorvc r12,r8,r11
	eormi r10,r7,r0   

ldcond:
	ld.weq  pc,pc[2044]
	ld.shal r12,r12[1022]
	ld.uhls r5,r5[0]
	ld.ubpl r4,r4[511]
	ld.sbne lr,lr[0]
	ld.wls  r10,r2[0]
	ld.shvc r12,r8[0x3fe]
	ld.ubmi r10,r7[1]
  
stcond2:
	st.weq pc[2044],pc
	st.hal r12[1022],r12
	st.hls r5[0],r5
	st.bpl r4[511],r4
	st.bne lr[0],lr
	st.wls r2[0],r10
	st.hvc r8[0x3fe],r12
	st.bmi r7[1],r10
	
movh:	
	movh	pc, 65535
	movh	r0, 0
	movh	r5, 1
	movh	r12, 32767
	
		
