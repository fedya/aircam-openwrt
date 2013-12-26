/* { dg-do compile } */
/* { dg-options "-O2 -fdump-rtl-ee" } */

unsigned char f(unsigned int a)
{
  unsigned int b = a & 0x10ff;
  return b;
}

/* { dg-final { scan-rtl-dump-times "and:" 0 "ee" { target mips*-*-* } } } */
/* { dg-final { scan-rtl-dump-times "superfluous extension \[0-9\]+ replaced" 1 "ee" { target mips*-*-* } } } */
/* { dg-final { cleanup-rtl-dump "ee" } } */

