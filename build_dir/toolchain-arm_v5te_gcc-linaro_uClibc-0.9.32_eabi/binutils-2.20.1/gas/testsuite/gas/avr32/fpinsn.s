
	.text
	.global	fadd_s
fadd_s:
	fadd.s fr15, fr15, fr15
	fadd.s fr0, fr0, fr0
	fadd.s fr0, fr15, fr15
	fadd.s fr15, fr0, fr15
	fadd.s fr15, fr15, fr0
	fadd.s fr7, fr8, fr8
	fadd.s fr8, fr7, fr8
	fadd.s fr8, fr8, fr7
	.global	fsub_s
fsub_s:
	fsub.s fr15, fr15, fr15
	fsub.s fr0, fr0, fr0
	fsub.s fr0, fr15, fr15
	fsub.s fr15, fr0, fr15
	fsub.s fr15, fr15, fr0
	fsub.s fr7, fr8, fr8
	fsub.s fr8, fr7, fr8
	fsub.s fr8, fr8, fr7
	.global	fmac_s
fmac_s:
	fmac.s fr15, fr15, fr15
	fmac.s fr0, fr0, fr0
	fmac.s fr0, fr15, fr15
	fmac.s fr15, fr0, fr15
	fmac.s fr15, fr15, fr0
	fmac.s fr7, fr8, fr8
	fmac.s fr8, fr7, fr8
	fmac.s fr8, fr8, fr7
	.global	fnmac_s
fnmac_s:
	fnmac.s fr15, fr15, fr15
	fnmac.s fr0, fr0, fr0
	fnmac.s fr0, fr15, fr15
	fnmac.s fr15, fr0, fr15
	fnmac.s fr15, fr15, fr0
	fnmac.s fr7, fr8, fr8
	fnmac.s fr8, fr7, fr8
	fnmac.s fr8, fr8, fr7
	.global	fmsc_s
fmsc_s:
	fmsc.s fr15, fr15, fr15
	fmsc.s fr0, fr0, fr0
	fmsc.s fr0, fr15, fr15
	fmsc.s fr15, fr0, fr15
	fmsc.s fr15, fr15, fr0
	fmsc.s fr7, fr8, fr8
	fmsc.s fr8, fr7, fr8
	fmsc.s fr8, fr8, fr7
	.global	fnmsc_s
fnmsc_s:
	fnmsc.s fr15, fr15, fr15
	fnmsc.s fr0, fr0, fr0
	fnmsc.s fr0, fr15, fr15
	fnmsc.s fr15, fr0, fr15
	fnmsc.s fr15, fr15, fr0
	fnmsc.s fr7, fr8, fr8
	fnmsc.s fr8, fr7, fr8
	fnmsc.s fr8, fr8, fr7
	.global	fmul_s
fmul_s:
	fmul.s fr15, fr15, fr15
	fmul.s fr0, fr0, fr0
	fmul.s fr0, fr15, fr15
	fmul.s fr15, fr0, fr15
	fmul.s fr15, fr15, fr0
	fmul.s fr7, fr8, fr8
	fmul.s fr8, fr7, fr8
	fmul.s fr8, fr8, fr7
	.global	fnmul_s
fnmul_s:
	fnmul.s fr15, fr15, fr15
	fnmul.s fr0, fr0, fr0
	fnmul.s fr0, fr15, fr15
	fnmul.s fr15, fr0, fr15
	fnmul.s fr15, fr15, fr0
	fnmul.s fr7, fr8, fr8
	fnmul.s fr8, fr7, fr8
	fnmul.s fr8, fr8, fr7
	.global	fneg_s
fneg_s:
	fneg.s fr15, fr15
	fneg.s fr0, fr0
	fneg.s fr0, fr15
	fneg.s fr15, fr0
	fneg.s fr7, fr8
	fneg.s fr8, fr7
	.global	fabs_s
fabs_s:
	fabs.s fr15, fr15
	fabs.s fr0, fr0
	fabs.s fr0, fr15
	fabs.s fr15, fr0
	fabs.s fr7, fr8
	fabs.s fr8, fr7
	.global	fcmp_s
fcmp_s:
	fcmp.s fr15, fr15
	fcmp.s fr0, fr0
	fcmp.s fr0, fr15
	fcmp.s fr15, fr0
	fcmp.s fr7, fr8
	fcmp.s fr8, fr7
	.global	fadd_d
fadd_d:
	fadd.d fr14, fr14, fr14
	fadd.d fr0, fr0, fr0
	fadd.d fr0, fr14, fr14
	fadd.d fr14, fr0, fr14
	fadd.d fr14, fr14, fr0
	fadd.d fr6, fr8, fr8
	fadd.d fr8, fr6, fr8
	fadd.d fr8, fr8, fr6
	.global	fsub_d
fsub_d:
	fsub.d fr14, fr14, fr14
	fsub.d fr0, fr0, fr0
	fsub.d fr0, fr14, fr14
	fsub.d fr14, fr0, fr14
	fsub.d fr14, fr14, fr0
	fsub.d fr6, fr8, fr8
	fsub.d fr8, fr6, fr8
	fsub.d fr8, fr8, fr6
	.global	fmac_d
fmac_d:
	fmac.d fr14, fr14, fr14
	fmac.d fr0, fr0, fr0
	fmac.d fr0, fr14, fr14
	fmac.d fr14, fr0, fr14
	fmac.d fr14, fr14, fr0
	fmac.d fr6, fr8, fr8
	fmac.d fr8, fr6, fr8
	fmac.d fr8, fr8, fr6
	.global	fnmac_d
fnmac_d:
	fnmac.d fr14, fr14, fr14
	fnmac.d fr0, fr0, fr0
	fnmac.d fr0, fr14, fr14
	fnmac.d fr14, fr0, fr14
	fnmac.d fr14, fr14, fr0
	fnmac.d fr6, fr8, fr8
	fnmac.d fr8, fr6, fr8
	fnmac.d fr8, fr8, fr6
	.global	fmsc_d
fmsc_d:
	fmsc.d fr14, fr14, fr14
	fmsc.d fr0, fr0, fr0
	fmsc.d fr0, fr14, fr14
	fmsc.d fr14, fr0, fr14
	fmsc.d fr14, fr14, fr0
	fmsc.d fr6, fr8, fr8
	fmsc.d fr8, fr6, fr8
	fmsc.d fr8, fr8, fr6
	.global	fnmsc_d
fnmsc_d:
	fnmsc.d fr14, fr14, fr14
	fnmsc.d fr0, fr0, fr0
	fnmsc.d fr0, fr14, fr14
	fnmsc.d fr14, fr0, fr14
	fnmsc.d fr14, fr14, fr0
	fnmsc.d fr6, fr8, fr8
	fnmsc.d fr8, fr6, fr8
	fnmsc.d fr8, fr8, fr6
	.global	fmul_d
fmul_d:
	fmul.d fr14, fr14, fr14
	fmul.d fr0, fr0, fr0
	fmul.d fr0, fr14, fr14
	fmul.d fr14, fr0, fr14
	fmul.d fr14, fr14, fr0
	fmul.d fr6, fr8, fr8
	fmul.d fr8, fr6, fr8
	fmul.d fr8, fr8, fr6
	.global	fnmul_d
fnmul_d:
	fnmul.d fr14, fr14, fr14
	fnmul.d fr0, fr0, fr0
	fnmul.d fr0, fr14, fr14
	fnmul.d fr14, fr0, fr14
	fnmul.d fr14, fr14, fr0
	fnmul.d fr6, fr8, fr8
	fnmul.d fr8, fr6, fr8
	fnmul.d fr8, fr8, fr6
	.global	fneg_d
fneg_d:
	fneg.d fr14, fr14
	fneg.d fr0, fr0
	fneg.d fr0, fr14
	fneg.d fr14, fr0
	fneg.d fr6, fr8
	fneg.d fr8, fr6
	.global	fabs_d
fabs_d:
	fabs.d fr14, fr14
	fabs.d fr0, fr0
	fabs.d fr0, fr14
	fabs.d fr14, fr0
	fabs.d fr6, fr8
	fabs.d fr8, fr6
	.global	fcmp_d
fcmp_d:
	fcmp.d fr14, fr14
	fcmp.d fr0, fr0
	fcmp.d fr0, fr14
	fcmp.d fr14, fr0
	fcmp.d fr6, fr8
	fcmp.d fr8, fr6
	.global fmov_s
fmov_s:
	fmov.s fr15, fr15
	fmov.s fr0, fr0
	fmov.s fr15, fr0
	fmov.s fr0, fr15
	fmov.s fr8, fr7
	fmov.s fr7, fr8
	fmov.s pc, fr15
	fmov.s r0, fr0
	fmov.s pc, fr0
	fmov.s r0, fr15
	fmov.s r8, fr7
	fmov.s r7, fr8
	fmov.s fr15, pc
	fmov.s fr0, r0
	fmov.s fr15, r0
	fmov.s fr0, pc
	fmov.s fr8, r7
	fmov.s fr7, r8
	.global fmov_d
fmov_d:
	fmov.d fr14, fr14
	fmov.d fr0, fr0
	fmov.d fr14, fr0
	fmov.d fr0, fr14
	fmov.d fr8, fr6
	fmov.d fr6, fr8
	fmov.d lr, fr14
	fmov.d r0, fr0
	fmov.d lr, fr0
	fmov.d r0, fr14
	fmov.d r8, fr6
	fmov.d r6, fr8
	fmov.d fr14, lr
	fmov.d fr0, r0
	fmov.d fr14, r0
	fmov.d fr0, lr
	fmov.d fr8, r6
	fmov.d fr6, r8
	.global fcasts_d
fcasts_d:
	fcasts.d fr15, fr14
	fcasts.d fr0, fr0
	fcasts.d fr15, fr0
	fcasts.d fr0, fr14
	fcasts.d fr8, fr6
	fcasts.d fr7, fr8
	.global fcastd_s
fcastd_s:
	fcastd.s fr14, fr15
	fcastd.s fr0, fr0
	fcastd.s fr14, fr0
	fcastd.s fr0, fr15
	fcastd.s fr8, fr7
	fcastd.s fr6, fr8
