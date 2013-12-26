
	.text
	.global	picosvmac
picosvmac:
	picosvmac	out0, in0, in0, in0
	picosvmac	out2, in11, in11, in11
	picosvmac	out1, in10, in0, in5
	picosvmac	out3, in6, in9, in0
	.global picosvmul
picosvmul:
	picosvmul	out0, in0, in0, in0
	picosvmul	out2, in11, in11, in11
	picosvmul	out1, in10, in0, in5
	picosvmul	out3, in6, in9, in0
	.global picovmac
picovmac:
	picovmac	out0, in0, in0, in0
	picovmac	out2, in11, in11, in11
	picovmac	out1, in10, in0, in5
	picovmac	out3, in6, in9, in0
	.global picovmul
picovmul:
	picovmul	out0, in0, in0, in0
	picovmul	out2, in11, in11, in11
	picovmul	out1, in10, in0, in5
	picovmul	out3, in6, in9, in0
	.global	picold_d
picold_d:
	picold.d	vmu2_out, pc[1020]
	picold.d	inpix2, r0[1020]
	picold.d	inpix2, r0[0]
	picold.d	coeff0_a, --r8
	picold.d	coeff1_a, --r7
	picold.d	inpix0, r10[r5 << 2]
	picold.d	vmu0_out, r3[r6 << 0]
	.global	picold_w
picold_w:	
	picold.w	config, pc[1020]
	picold.w	inpix2, r0[1020]
	picold.w	inpix2, r0[0]
	picold.w	coeff0_b, --r8
	picold.w	coeff1_a, --r7
	picold.w	inpix1, r10[r5 << 2]
	picold.w	vmu1_out, r3[r6 << 0]
	.global	picoldm_d
picoldm_d:
	picoldm.d	pc, inpix2-config
	picoldm.d	r0, inpix2, inpix1
	picoldm.d	r7, vmu2_out, config
	picoldm.d	r8, inpix2-vmu1_out
	.global	picoldm_d_pu
picoldm_d_pu:
	picoldm.d	pc++, inpix2, inpix1, inpix0, outpix2, outpix1, outpix0, coeff0_a, coeff0_b, coeff1_a, coeff1_b, coeff2_a, coeff2_b, vmu0_out, vmu1_out, vmu2_out, config
	picoldm.d	r0++, inpix2, inpix1
	picoldm.d	r7++, vmu2_out, config
	picoldm.d	r8++, inpix2, inpix1, inpix0, outpix2, outpix1, outpix0, coeff0_a, coeff0_b, coeff1_a, coeff1_b, coeff2_a, coeff2_b, vmu0_out, vmu1_out
	.global	picoldm_w
picoldm_w:
	picoldm.w	pc, inpix2-coeff0_b
	picoldm.w	r0, inpix2
	picoldm.w	r7, coeff0_b
	picoldm.w	r8, inpix2-coeff0_a
	picoldm.w	pc, coeff1_a-config
	picoldm.w	r0, coeff1_a
	picoldm.w	r7, config
	picoldm.w	r8, coeff1_a-vmu2_out
	.global	picoldm_w_pu
picoldm_w_pu:
	picoldm.w	pc++, inpix2-coeff0_b
	picoldm.w	r0++, inpix2
	picoldm.w	r7++, coeff0_b
	picoldm.w	r8++, inpix2-coeff0_a
	picoldm.w	pc++, coeff1_a-config
	picoldm.w	r0++, coeff1_a
	picoldm.w	r7++, config
	picoldm.w	r8++, coeff1_a-vmu2_out
	.global	picomv_d
picomv_d:
	picomv.d	vmu2_out, lr
	picomv.d	inpix2, r0
	picomv.d	coeff0_a, r8
	picomv.d	coeff1_a, r6
	picomv.d	pc, vmu2_out
	picomv.d	r0, inpix2
	picomv.d	r8, coeff0_a
	picomv.d	r6, coeff1_a
	.global	picomv_w
picomv_w:
	picomv.w	config, pc
	picomv.w	inpix2, r0
	picomv.w	coeff0_b, r8
	picomv.w	coeff1_a, r7
	picomv.w	pc, config
	picomv.w	r0, inpix2
	picomv.w	r8, coeff0_b
	picomv.w	r7, coeff1_a
	.global	picost_d
picost_d:
	picost.d	pc[1020], vmu2_out
	picost.d	r0[0], inpix2
	picost.d	r8++, coeff0_a
	picost.d	r7++, coeff1_a
	picost.d	r10[r5 << 2], inpix0
	picost.d	r3[r6 << 0], vmu0_out
	.global	picost_w
picost_w:	
	picost.w	pc[1020], config
	picost.w	r0[0], inpix2
	picost.w	r8++, coeff0_b
	picost.w	r7++, coeff1_a
	picost.w	r10[r5 << 2], inpix1
	picost.w	r3[r6 << 0], vmu1_out
	.global	picostm_d
picostm_d:
	picostm.d	pc, inpix2-config
	picostm.d	r0, inpix2, inpix1
	picostm.d	r7, vmu2_out, config
	picostm.d	r8, inpix2-vmu1_out
	.global	picostm_d_pu
picostm_d_pu:
	picostm.d	--pc, inpix2, inpix1, inpix0, outpix2, outpix1, outpix0, coeff0_a, coeff0_b, coeff1_a, coeff1_b, coeff2_a, coeff2_b, vmu0_out, vmu1_out, vmu2_out, config
	picostm.d	--r0, inpix2, inpix1
	picostm.d	--r7, vmu2_out, config
	picostm.d	--r8, inpix2, inpix1, inpix0, outpix2, outpix1, outpix0, coeff0_a, coeff0_b, coeff1_a, coeff1_b, coeff2_a, coeff2_b, vmu0_out, vmu1_out
	.global	picostm_w
picostm_w:
	picostm.w	pc, inpix2-coeff0_b
	picostm.w	r0, inpix2
	picostm.w	r7, coeff0_b
	picostm.w	r8, inpix2-coeff0_a
	picostm.w	pc, coeff1_a-config
	picostm.w	r0, coeff1_a
	picostm.w	r7, config
	picostm.w	r8, coeff1_a-vmu2_out
	.global	picostm_w_pu
picostm_w_pu:
	picostm.w	--pc, inpix2-coeff0_b
	picostm.w	--r0, inpix2
	picostm.w	--r7, coeff0_b
	picostm.w	--r8, inpix2-coeff0_a
	picostm.w	--pc, coeff1_a-config
	picostm.w	--r0, coeff1_a
	picostm.w	--r7, config
	picostm.w	--r8, coeff1_a-vmu2_out
