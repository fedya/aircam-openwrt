
        .equ    sym1, 0x12345678

        .text
        .global test_hwrd
test_hwrd:
        mov     r0, hi(0x87654321)
        mov     r0, hi(sym1)
        mov     r0, hi(sym2)
        mov     r0, hi(extsym1)

        orh	r0, hi(0x87654321)
        orh	r0, hi(sym1)
        orh	r0, hi(sym2)
        orh	r0, hi(extsym1)

        andh	r0, hi(0x87654321)
        andh	r0, hi(sym1)
        andh	r0, hi(sym2)
        andh	r0, hi(extsym1)

        .global test_lwrd
test_lwrd:
        mov     r0, lo(0x87654321)
        mov     r0, lo(sym1)
        mov     r0, lo(sym2)
        mov     r0, lo(extsym1)

        orl	r0, lo(0x87654321)
        orl	r0, lo(sym1)
        orl	r0, lo(sym2)
        orl	r0, lo(extsym1)

        andl	r0, lo(0x87654321)
        andl	r0, lo(sym1)
        andl	r0, lo(sym2)
        andl	r0, lo(extsym1)

sym2:
