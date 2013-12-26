	.text
	.global	ld_nodisp
ld_nodisp:
	ld.ub	r0, r12
	ld.sb	r0, r12
	ld.uh	r0, r12
	ld.sh	r0, r12
	ld.w	r0, r12

	.global st_nodisp
st_nodisp:
	st.b	r12, r0
	st.h	r12, r0
	st.w	r12, r0
